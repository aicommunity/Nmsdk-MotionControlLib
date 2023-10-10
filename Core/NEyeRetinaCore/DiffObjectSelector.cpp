#ifndef DIFFOBJECTSELECTOR_CPP
#define DIFFOBJECTSELECTOR_CPP

#include "DiffObjectSelector.h"
#include <algorithm>


// --------------------------
// Методы управления
// --------------------------
// Расширяет текущий объект до размеров объекта 'segm'+текущий
void MSIntegrate(MovingSegment &currsegm, MovingSegment &segm)
{
 currsegm.Width=(currsegm.X+currsegm.Width>segm.X+segm.Width)?
 					currsegm.Width:(segm.X+segm.Width-currsegm.X);
 currsegm.X=(currsegm.X<segm.X)?currsegm.X:segm.X;
}
// --------------------------


// --------------------------
// Методы управления
// --------------------------
// Расширяет текущий объект до размеров объекта 'rect'+текущий
void MRIntegrate(MovingRect &currrect, MovingRect &rect)
{
 currrect.X1=(currrect.X1<rect.X1)?currrect.X1:rect.X1;
 currrect.Y1=(currrect.Y1<rect.Y1)?currrect.Y1:rect.Y1;
 currrect.X2=(currrect.X2>rect.X2)?currrect.X2:rect.X2;
 currrect.Y2=(currrect.Y2>rect.Y2)?currrect.Y2:rect.Y2;
}
// --------------------------

/*
  class MovingSegmentContainer
  Описывает хранилище набора результатов.
*/
// --------------------------
// Методы инициализации и деинциализации
// --------------------------
void MSCInit(MovingSegmentContainer &obj, int numlines, int numsegments)
{
 obj.MovingSegmentList=0;
 obj.NumLines=0;
 obj.NumSegments=0;
 obj.CNumSegments=0;

 if(numlines && numsegments)
  {
   MSCSetDimensions(obj,numlines,numsegments);
  }  

}

void MSCUninit(MovingSegmentContainer &obj)
{
 int i;

 for(i=0;i<obj.NumLines;i++)
  {
   delete []obj.MovingSegmentList[i];
  }

 delete []obj.MovingSegmentList;
 delete []obj.CNumSegments;

 obj.MovingSegmentList=0;
 obj.NumSegments=0;
 obj.CNumSegments=0;
 obj.NumLines=0;
}
// --------------------------

// --------------------------
// Методы управления параметрами
// --------------------------
// Число строк
bool MSCSetDimensions(MovingSegmentContainer &obj, int numlines, int numsegments)
//bool MSCSetNumLines(MovingSegmentContainer &obj, int num)
{
 int i;

 if(numlines > 0 && numlines != obj.NumLines)
  {
   if(numlines < obj.NumLines)
    {
     for(i=numlines;i<obj.NumLines;i++)
      delete []obj.MovingSegmentList[i];
    }

   obj.MovingSegmentList=(MovingSegment**)realloc(obj.MovingSegmentList,
											sizeof(MovingSegment*)*numlines);

   obj.CNumSegments=(int*)realloc(obj.CNumSegments,sizeof(int)*numlines);

   memset(obj.CNumSegments,0,sizeof(int)*numlines);
  }

 if((numsegments && obj.NumSegments != numsegments) || numlines > obj.NumLines)
  {
   if(!numsegments)
    numsegments=obj.NumSegments;

   for(i=0;i<obj.NumLines;i++)
    {
     delete []obj.MovingSegmentList[i];
     obj.MovingSegmentList[i]=(MovingSegment*)calloc(numsegments,sizeof(MovingSegment));
    }
  }

 obj.NumLines=numlines;
 obj.NumSegments=numsegments;

 return true;
}
/*
// Число найденных движущихся сегментов
bool MSCSetNumSegments(MovingSegmentContainer &obj, int line, int num)
{
 if(num <0 || line < 0)
  return false;

 obj.MovingSegmentList[line]=(MovingSegment*)realloc(obj.MovingSegmentList[line],
												 sizeof(MovingSegment)*num);
 obj.NumSegments[line]=num;
 if(obj.CNumSegments[line] >= num)
  obj.CNumSegments[line]=num-1;

 return true;
}                */
// --------------------------


/*
  class MovingSegmentSelector.
  Обеспечивает выделение движущихся объектов
*/
// --------------------------
// инициализация и деинициализация
// --------------------------
void MSSInit(MovingSegmentSelector &obj)
{
 obj.MoveObjects=0;

 obj.NumMoveObjects=0;

 obj.CNumMoveObjects=0;

 obj.FrameSize=0;

 obj.IsSegmentedFlag=false;

 obj.IsCalculatedFlag=false;

 obj.SNWeights=0;

 obj.SNTrFuncCoeff=0;

 obj.SNOutput=0;

 obj.SNThresold=0;

 obj.NumFrames=0;

 obj.Image=0;
 obj.XRes=0;
 obj.YRes=0;


 MSCInit(obj.Segments, 0, 0);
 MSSDefaults(obj);
}

void MSSUninit(MovingSegmentSelector &obj)
{
 int i;

 delete []obj.MoveObjects;
 obj.MoveObjects=0;
 obj.NumMoveObjects=0;


 delete []obj.FrameSize;
 obj.FrameSize=0;

 obj.IsSegmentedFlag=false;

 obj.IsCalculatedFlag=false;

 for(i=0;i<obj.NumFrames;i++)
  {
   delete []obj.SNWeights[i];
   delete []obj.SNTrFuncCoeff[i];
  }

 delete []obj.SNWeights;
 obj.SNWeights=0;

 delete []obj.SNTrFuncCoeff;
 obj.SNTrFuncCoeff=0;

 delete []obj.SNOutput;
 obj.SNOutput=0;

 delete []obj.SNThresold;
 obj.SNThresold=0;

 obj.NumFrames=0;

 MSCUninit(obj.Segments);
}
// --------------------------

// --------------------------
// Методы управления параметрами
// --------------------------
// Число различных фреймов наблюдения
bool MSSSetNumFrames(MovingSegmentSelector &obj, int num)
{
 int i;
 
 if(num == obj.NumFrames)
  return true;

 if(num <= 0)
  return false;

 obj.NumFrames=num;
 obj.FrameSize=(int*)realloc(obj.FrameSize,sizeof(int)*obj.NumFrames);

 // Параметры пространственного нейрона
 obj.SNWeights=(int**)realloc(obj.SNWeights, sizeof(int*)*obj.NumFrames);
 obj.SNOutput=(int*)realloc(obj.SNOutput, sizeof(int)*obj.NumFrames);
 obj.SNThresold=(int*)realloc(obj.SNThresold, sizeof(int)*obj.NumFrames);
 obj.SNTrFuncCoeff=(int**)realloc(obj.SNTrFuncCoeff, sizeof(int*)*obj.NumFrames);
 for(i=0;i<obj.NumFrames;i++)
  obj.SNTrFuncCoeff[i]=(int*)realloc(obj.SNTrFuncCoeff[i], sizeof(int));

 return true;
}

// Размеры фреймов наблюдения
bool MSSSetFrameSize(MovingSegmentSelector &obj, int frame, int size)
{
 if(obj.FrameSize[frame] == size)
  return true;

 if(size < 0)
  return false;

 obj.FrameSize[frame] = size;
 obj.SNWeights[frame]=(int*)realloc(obj.SNWeights[frame],sizeof(int)*size);

 for(int i=0;i<size;i++)
  {
   obj.SNWeights[frame][i]=1;
  }

 // Порог нейрона
 obj.SNThresold[frame]=96*size;

 return true;
}

bool MSSSetNumMoveObjects(MovingSegmentSelector &obj, int num)
{
 if(num == obj.NumMoveObjects)
  return true;

 if(num <= 0)
  return false;

 obj.NumMoveObjects=num;
 obj.MoveObjects=(MovingRect*)realloc(obj.MoveObjects,sizeof(MovingRect)*obj.NumFrames);

 if(obj.CNumMoveObjects >= num)
  obj.CNumMoveObjects=num-1;

 return true;
}




// --------------------------

// --------------------------
// Методы управления пространственным нейроном
// --------------------------
// Веса нейрона
bool MSSSetSNWeight(MovingSegmentSelector &obj, int frame, int num, int value)
{
 if(value < 0 || frame < 0 || num < 0)
  return false;
 obj.SNWeights[frame][num]=value;

 return true;
}

// Порог нейрона
bool MSSSetSNThresold(MovingSegmentSelector &obj, int frame, int value)
{
 if(value < 0 || frame < 0)
  return false;
 obj.SNThresold[frame]=value;

 return true;
}

// Коэффициент передаточной функции
bool MSSSetSNTrFuncCoeff(MovingSegmentSelector &obj, int frame, int num, int value)
{
 if(value < 0 || num <0 || frame < 0)
  return false;
 obj.SNTrFuncCoeff[frame][num]=value;

 return true;
}
// --------------------------


// --------------------------
// Методы счета
// --------------------------
// Устанавливает кадр для счета
bool MSSSetImage(MovingSegmentSelector &obj, char *addr,
										int xres, int yres)
{
 if(!addr || xres == 0 || yres == 0)
  return false;

 obj.Image=addr;

 obj.XRes=xres;
 if(obj.YRes != yres)
  {
   MSCSetDimensions(obj.Segments,yres,0);
  }

 obj.YRes=yres;

 obj.IsSegmentedFlag=obj.IsCalculatedFlag=false;

 return true;
}

// Устанавливает набор движущихся сегментов
bool MSSSetSegments(MovingSegmentSelector &obj, int numlines,
										int *numsegments,
                                        int *x_coords, int *widths)
{
 int j,k;

 if(numlines <= 0 || !numsegments ||
    !x_coords || !widths)
  return false;

 for(j=0;j<numlines;j++)
  {
   MSCSetDimensions(obj.Segments,numlines,numsegments[j]);
   obj.Segments.CNumSegments[j]=numsegments[j];
   for(k=0;k<numsegments[j];k++)
    {
	 obj.Segments.MovingSegmentList[j][k].X=x_coords[j];
	 obj.Segments.MovingSegmentList[j][k].Width=widths[j];
    }
  }

 obj.IsSegmentedFlag=true;
 obj.IsCalculatedFlag=false;
 return true;
}

// Эмулирует аппаратное определение движущихся сегментов
void MSSSimulate(MovingSegmentSelector &obj)
{
 int i,j,k,n,m;
 int sum;
 int Counter;

 if(!obj.Image)
  return;

 for(j=0;j<obj.YRes;j++)
  {
   obj.CNOutput=0;
   Counter=0;
   for(i=0;i<obj.XRes;i++)
	{
	 for(k=0;k<obj.NumFrames;k++)
	  {
	   if(i+obj.FrameSize[k]>=obj.XRes)
		{
		 obj.SNOutput[k]=0;
		 continue;
		}
	   sum=0;
	   for(n=0;n<obj.FrameSize[k];n++)
		sum+=obj.SNWeights[k][n]*obj.Image[(j*obj.XRes+i+n)*4];

       obj.SNOutput[k]=MSSSNActivationFn(obj, k, sum);
      }
	 for(k=0;k<obj.NumFrames;k++)
	  {
	   n=MSSCNActivationFn(obj);
	   if(n)
		{
		 if(!Counter)
		  {
           m=obj.Segments.CNumSegments[obj.YRes-1-j];
           if(m >= obj.Segments.NumSegments) // !!!
            break;

           obj.Segments.CNumSegments[obj.YRes-1-j]++; 
//		   MSCSetNumSegments(obj.Segments, obj.YRes-1-j,m+1);
		   obj.Segments.MovingSegmentList[obj.YRes-1-j][m].X=i;
           obj.Segments.MovingSegmentList[obj.YRes-1-j][m].Width=1;
          }

		 obj.CNOutput=obj.FrameSize[n-1];
		 Counter++;
		}
       else
	   if(obj.CNOutput)
		{
		 obj.CNOutput--;
         Counter++;
        }
        
	   if(!obj.CNOutput)
        {
         if(Counter)
          {
		   obj.Segments.MovingSegmentList[obj.YRes-1-j][m].Width=Counter;
           Counter=0;
		  }
        }
      }

    }

  }


 obj.IsSegmentedFlag=true;
 obj.IsCalculatedFlag=false;
}

// Производит расчет зон движения по имеющимся сегментам
void MSSCalculate(MovingSegmentSelector &obj)
{
 int j,k,n;
 bool createkey=false;
 int x,w,x1,x2;

 if(!obj.IsSegmentedFlag)
  return;

   obj.CNumMoveObjects=0;
//   MSSSetNumMoveObjects(obj, 0);
   for(j=0;j<obj.Segments.NumLines;j++)
	for(k=0;k<obj.Segments.CNumSegments[j];k++)
     {
      createkey=true;
      for(n=0;n<obj.CNumMoveObjects;n++)
       {
		if(j >= obj.MoveObjects[n].Y1-1 && j <= obj.MoveObjects[n].Y2+1)
		 {
		  x=obj.Segments.MovingSegmentList[j][k].X;
		  w=obj.Segments.MovingSegmentList[j][k].Width;
		  x1=obj.MoveObjects[n].X1;
          x2=obj.MoveObjects[n].X2;
          if(x <= x1 && x+w >= x1-1)
           {
            if(x+w >= x1-1)
			 obj.MoveObjects[n].X1=x;
            if(x+w >= x2)
			 obj.MoveObjects[n].X2=x+w;

			if(j == obj.MoveObjects[n].Y1-1)
			 obj.MoveObjects[n].Y1=j;
			if(j == obj.MoveObjects[n].Y2+1)
			 obj.MoveObjects[n].Y2=j;
            createkey=false;
            break;
           }
          else
          if(x <= x2+1 && x+w >= x1-1)
           {
            if(x+w >= x2+1)
			 obj.MoveObjects[n].X2=x+w;

			if(j == obj.MoveObjects[n].Y1-1)
			 obj.MoveObjects[n].Y1=j;
			if(j == obj.MoveObjects[n].Y2+1)
			 obj.MoveObjects[n].Y2=j;
            createkey=false;
            break;
           }
         }
       }
      // Если не нашли подходящих объектов то создаем новый
      if(createkey)
       {
        obj.CNumMoveObjects++;
//        MSSSetNumMoveObjects(obj,obj.NumMoveObjects+1);
		obj.MoveObjects[obj.CNumMoveObjects-1].X1=obj.Segments.MovingSegmentList[j][k].X;
		obj.MoveObjects[obj.CNumMoveObjects-1].X2=obj.Segments.MovingSegmentList[j][k].X+
					   						obj.Segments.MovingSegmentList[j][k].Width;
		obj.MoveObjects[obj.CNumMoveObjects-1].Y1=j;
		obj.MoveObjects[obj.CNumMoveObjects-1].Y2=j;
       }
     }


/*
 // Объединяем и удаляем лишние объекты
 vector<int> eraselist;
 vector<MovingRect>::iterator I;

 for(i=0;i<1;i++)
  {
   eraselist.clear();
   for(j=0;j<MoveObjects.size();j++)
    for(k=0;k<MoveObjects.size();k++)
     {
      if(j == k)
       continue;
      if(find(eraselist.begin(),eraselist.end(),k) != eraselist.end()
       || find(eraselist.begin(),eraselist.end(),j) != eraselist.end())
       continue;
      if(
         (((MoveObjects[j].Y1<=MoveObjects[k].Y1 &&
            MoveObjects[j].Y2>=MoveObjects[k].Y1) ||
           (MoveObjects[j].Y2>=MoveObjects[k].Y2 &&
            MoveObjects[j].Y1<=MoveObjects[k].Y2)) ||

          ((MoveObjects[k].Y1<=MoveObjects[j].Y1 &&
            MoveObjects[k].Y2>=MoveObjects[j].Y1) ||
           (MoveObjects[k].Y2>=MoveObjects[j].Y2 &&
            MoveObjects[k].Y1<=MoveObjects[j].Y2)) ||

          ((MoveObjects[j].Y1>=MoveObjects[k].Y2 &&
            MoveObjects[j].Y1-MoveObjects[k].Y2<=FrameSize[0]) ||
		   (MoveObjects[k].Y1>=MoveObjects[j].Y2 &&
			MoveObjects[k].Y1-MoveObjects[j].Y2<=FrameSize[0])))
          &&
         (((MoveObjects[j].X1<=MoveObjects[k].X1 &&
            MoveObjects[j].X2>=MoveObjects[k].X1) ||
           (MoveObjects[j].X2>=MoveObjects[k].X2 &&
            MoveObjects[j].X1<=MoveObjects[k].X2)) ||
          ((MoveObjects[k].X1<=MoveObjects[j].X1 &&
            MoveObjects[k].X2>=MoveObjects[j].X1) ||
           (MoveObjects[k].X2>=MoveObjects[j].X2 &&
            MoveObjects[k].X1<=MoveObjects[j].X2)))
        )
       {
        MoveObjects[j].Integrate(MoveObjects[k]);
        eraselist.push_back(k);
       }

     }

   sort(eraselist.begin(),eraselist.end());  
   for(j=eraselist.size()-1;j>=0;j--)
    {
     for(k=0;k<MoveObjects.size();k++)
      {
       MoveObjects[k].X1;
       MoveObjects[k].X2;
       MoveObjects[k].Y1;
       MoveObjects[k].Y2;
      }
     I=MoveObjects.begin()+eraselist[j];
     MoveObjects.erase(I);
    }
  }
                          */
 obj.IsCalculatedFlag=true;
}

// Сбрасывает модель в исходное состояние
void MSSReset(MovingSegmentSelector &obj)
{
 int i;

 // Модель считающего нейрона (Counter Neuron)
 // Выходы нейрона
 obj.CNOutput=0;

 // Флаг наличия сформированного набора движущихся сегментов
 obj.IsSegmentedFlag=false;

 // Флаг успешного рачета
 obj.IsCalculatedFlag=false;

 for(i=0;i<obj.Segments.NumLines;i++)
  obj.Segments.CNumSegments[i]=0;
//  MSCSetNumSegments(obj.Segments, i, 0);

 obj.CNumMoveObjects=0;
}

// Восстанавливает настройки по умолчанию
void MSSDefaults(MovingSegmentSelector &obj)
{
 // Указатель на разностное изображение-источник
 obj.Image=0; obj.XRes=0; obj.YRes=0;

 // Число различных фреймов наблюдения
 MSSSetNumFrames(obj, 3);

 MSSSetNumMoveObjects(obj, 100);

 // Размеры фреймов наблюдения
 MSSSetFrameSize(obj, 0, 4);
 MSSSetFrameSize(obj, 1, 10);
 MSSSetFrameSize(obj, 2, 15);
//etFrameSize(3, 20);
//etFrameSize(4, 25);

 // Модель пространственного нейрона (Space Neuron)
 // Веса нейрона
 for(int j=0;j<obj.NumFrames;j++)
  for(int i=0;i<obj.FrameSize[j];i++)
   {
//    if(i<GetFrameSize(j)/2)
//     SNWeights[j][i]=i;
//    else
//     SNWeights[j][i]=GetFrameSize(j)-i;

    obj.SNWeights[j][i]=1;
   }

 // Порог нейрона
 obj.SNThresold[0]=96*obj.FrameSize[0];
 obj.SNThresold[1]=96*obj.FrameSize[1];

 // Коэффициенты передаточной функции
// SNTrFuncCoeff[0][0]=100;
// SNTrFuncCoeff[1][0]=100;
                                        
 MSSReset(obj);
}
// --------------------------

// --------------------------
// Скрытые методы счета
// --------------------------
// Функция активации пространственного нейрона
int MSSSNActivationFn(MovingSegmentSelector &obj, int frame, int input)
{
 if(input < obj.SNThresold[frame])
  return 0;

 return 255;
/*
 if(input > SNTrFuncCoeff[frame][0])
  return 255;

 return (input-SNThresold[frame])*(double)(255)/
                      (double)(SNTrFuncCoeff[frame][0]-SNThresold[frame]);
*/
}

// Функция активации считающего нейрона
int MSSCNActivationFn(MovingSegmentSelector &obj)
{
 for(int i=0;i<obj.NumFrames;i++)
  if(obj.SNOutput[i])
   return i+1;

 return 0;  
}
// --------------------------

#endif

