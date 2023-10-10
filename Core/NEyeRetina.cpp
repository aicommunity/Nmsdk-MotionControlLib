#ifndef NMSDK_NEyeRetina_CPP
#define NMSDK_NEyeRetina_CPP

#include "NEyeRetina.h"

namespace NMSDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NEyeRetina::NEyeRetina(void)
: CaptureImage("CaptureImage",this),
  InputImage("InputImage",this),
  //BipolarOuts("BipolarOuts",this),
  GanglionicOuts("GanglionicOuts",this),
  RConeGanglionicOuts("RConeGanglionicOuts",this),
  GConeGanglionicOuts("GConeGanglionicOuts",this),
  BConeGanglionicOuts("BConeGanglionicOuts",this),
  RodGanglionicOuts("RodGanglionicOuts",this),
  OnGanglionicOuts("OnGanglionicOuts",this),
  OffGanglionicOuts("OffGanglionicOuts",this),
  LeftGanglionicOut("LeftGanglionicOut",this),
  RightGanglionicOut("RightGanglionicOut",this),
  TopGanglionicOut("TopGanglionicOut",this),
  BottomGanglionicOut("BottomGanglionicOut",this),
  // ДЛЯ ОТЛАДКИ
  // АКТИВНОСТЬ ВХОДОВ ДЛЯ 4-ч ГЛАЗНЫХ МЫШЦ
  LeftMuscle("LeftMuscle",this),
  RightMuscle("RightMuscle",this),
  TopMuscle("TopMuscle",this),
  BottomMuscle("BottomMuscle",this)
{
// MSSInit(MovingObjects);
}

NEyeRetina::~NEyeRetina(void)
{
}
// --------------------------    


// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------

// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NEyeRetina* NEyeRetina::New(void)
{
 return new NEyeRetina;
}
// --------------------------


// --------------------------
// Скрытые методы управления счетом 
// --------------------------
void NEyeRetina::UpdateImages(void)
{
 //int max,min,bmax,bmin;
 int i;

  /*
 bmax=0; bmin=0;
 BipolarOuts->SetRes(EyeRetina.GetBiWidth(),EyeRetina.GetBiHeight(),ubmY8);

if(EyeRetina.GetBiMax()==0) bmax=1;
 else bmax=EyeRetina.GetBiMax();

 if(EyeRetina.GetBiMin()==0) bmin=1;
 else bmin=EyeRetina.GetBiMin();

 for(i=0;i<EyeRetina.GetBiWidth()*EyeRetina.GetBiHeight();i++)
  {
   (*BipolarOuts)[i]=abs(EyeRetina.GetBipolarOut()[i]);
  } */

 // ДОБАВЛЯЮ ЦВЕТНОЕ ЗРЕНИЕ -
 // Заккоменчен старый код ------->
 /*
 max=0; min=0;
 GanglionicOuts->SetRes(EyeRetina.GetGangWidth(),EyeRetina.GetGangHeight(),ubmY8);

 if(EyeRetina.GetGangMax()==0) max=1;
 else max=EyeRetina.GetGangMax();
 if(EyeRetina.GetGangMin()==0) min=1;
 else min=EyeRetina.GetGangMin();

 int posmin=max, negmax=min;
 for(i=0;i<EyeRetina.GetGangWidth()*EyeRetina.GetGangHeight();i++)
 {
   if(EyeRetina.GetARetinaOut()[i]>=0 && posmin>EyeRetina.GetARetinaOut()[i])
	posmin=EyeRetina.GetARetinaOut()[i];
   else
   if(EyeRetina.GetARetinaOut()[i]<0 && negmax<EyeRetina.GetARetinaOut()[i])
	negmax=EyeRetina.GetARetinaOut()[i];

 }

 posmin=negmax=0;
 for(i=0;i<EyeRetina.GetGangWidth()*EyeRetina.GetGangHeight();i++)
  {
   (*GanglionicOuts)[i]=abs(EyeRetina.GetARetinaOut()[i]);
  }


 GanglionicOuts->SetRes(EyeRetina.GetGangWidth(),EyeRetina.GetGangHeight(),ubmY8);

 if(EyeRetina.GetGangMax()==0) max=1;
 else max=EyeRetina.GetGangMax();
 if(EyeRetina.GetGangMin()==0) min=1;
 else min=EyeRetina.GetGangMin();

 int posmin=max, negmax=min;
 for(i=0;i<EyeRetina.GetGangWidth()*EyeRetina.GetGangHeight();i++)
 {
   if(EyeRetina.GetARetinaOut()[i]>=0 && posmin>EyeRetina.GetARetinaOut()[i])
	posmin=EyeRetina.GetARetinaOut()[i];
   else
   if(EyeRetina.GetARetinaOut()[i]<0 && negmax<EyeRetina.GetARetinaOut()[i])
	negmax=EyeRetina.GetARetinaOut()[i];

 } */
 // ДОБАВЛЯЮ ЦВЕТНОЕ ЗРЕНИЕ
 // -------------->
  GanglionicOuts->SetRes(EyeRetina.GetGangWidth(),EyeRetina.GetGangHeight(),ubmRGB24);
  RConeGanglionicOuts->SetRes(EyeRetina.GetGangWidth(),EyeRetina.GetGangHeight(),ubmRGB24);
  GConeGanglionicOuts->SetRes(EyeRetina.GetGangWidth(),EyeRetina.GetGangHeight(),ubmRGB24);
  BConeGanglionicOuts->SetRes(EyeRetina.GetGangWidth(),EyeRetina.GetGangHeight(),ubmRGB24);
  RodGanglionicOuts->SetRes(EyeRetina.GetGangWidth(),EyeRetina.GetGangHeight(),ubmY8);
  OnGanglionicOuts->SetRes(EyeRetina.GetGangWidth(),EyeRetina.GetGangHeight(),ubmRGB24);
  OffGanglionicOuts->SetRes(EyeRetina.GetGangWidth(),EyeRetina.GetGangHeight(),ubmRGB24);

 // Выводим совместно он- и офф- каналы. Выбираем цвет с максимальным откликом
 for(i=0;i<EyeRetina.GetGangWidth()*EyeRetina.GetGangHeight();i++)
  {
   if(abs(EyeRetina.GetARetinaOut()[i]) > abs(EyeRetinaRCone.GetARetinaOut()[i]) &&
	  abs(EyeRetina.GetARetinaOut()[i]) > abs(EyeRetinaGCone.GetARetinaOut()[i]) &&
	  abs(EyeRetina.GetARetinaOut()[i]) > abs(EyeRetinaBCone.GetARetinaOut()[i]))
	  {
			(*GanglionicOuts)(i).rgb.r=(*GanglionicOuts)(i).rgb.g=(*GanglionicOuts)(i).rgb.b=abs(EyeRetina.GetARetinaOut()[i]);
	  }
   else if(abs(EyeRetinaRCone.GetARetinaOut()[i]) >= abs(EyeRetinaGCone.GetARetinaOut()[i]) &&
	   abs(EyeRetinaRCone.GetARetinaOut()[i]) >= abs(EyeRetinaBCone.GetARetinaOut()[i]))
	   {
		(*GanglionicOuts)(i).rgb.r=abs(EyeRetinaRCone.GetARetinaOut()[i]);
		(*GanglionicOuts)(i).rgb.g=0;
		(*GanglionicOuts)(i).rgb.b=0;
	   }
	else if(abs(EyeRetinaGCone.GetARetinaOut()[i]) >= abs(EyeRetinaBCone.GetARetinaOut()[i]))
	   {
		(*GanglionicOuts)(i).rgb.r=0;
		(*GanglionicOuts)(i).rgb.g=abs(EyeRetinaGCone.GetARetinaOut()[i]);
		(*GanglionicOuts)(i).rgb.b=0;
	   }
	else
	   {
		(*GanglionicOuts)(i).rgb.r=0;
		(*GanglionicOuts)(i).rgb.g=0;
		(*GanglionicOuts)(i).rgb.b=abs(EyeRetinaBCone.GetARetinaOut()[i]);
	   }
  }

 // Выводим отдельно он- и офф- каналы. Для каждого выбираем цвет с максимальным откликом
 for(i=0;i<EyeRetina.GetGangWidth()*EyeRetina.GetGangHeight();i++)
  {
   // Он- канал
   if(EyeRetina.GetARetinaOut()[i] >= EyeRetinaRCone.GetARetinaOut()[i] &&
	  EyeRetina.GetARetinaOut()[i] >= EyeRetinaGCone.GetARetinaOut()[i] &&
	  EyeRetina.GetARetinaOut()[i] >= EyeRetinaBCone.GetARetinaOut()[i] &&
	  EyeRetina.GetARetinaOut()[i] >= 0)
	  {
			(*OnGanglionicOuts)(i).rgb.r=(*OnGanglionicOuts)(i).rgb.g=(*OnGanglionicOuts)(i).rgb.b=abs(EyeRetina.GetARetinaOut()[i]);
	  }
   else	if(EyeRetinaRCone.GetARetinaOut()[i] >= EyeRetinaGCone.GetARetinaOut()[i] &&
	   EyeRetinaRCone.GetARetinaOut()[i] >= EyeRetinaBCone.GetARetinaOut()[i] &&
	   EyeRetinaRCone.GetARetinaOut()[i] >= 0)
	   {
		 (*OnGanglionicOuts)(i).rgb.r=abs(EyeRetinaRCone.GetARetinaOut()[i]);
		 (*OnGanglionicOuts)(i).rgb.g=0;
		 (*OnGanglionicOuts)(i).rgb.b=0;
	   }
	else if(EyeRetinaGCone.GetARetinaOut()[i] >= EyeRetinaBCone.GetARetinaOut()[i] &&
			EyeRetinaGCone.GetARetinaOut()[i] >= 0)
	   {
		 (*OnGanglionicOuts)(i).rgb.r=0;
		 (*OnGanglionicOuts)(i).rgb.g=abs(EyeRetinaGCone.GetARetinaOut()[i]);
		 (*OnGanglionicOuts)(i).rgb.b=0;
	   }
	else if(EyeRetinaBCone.GetARetinaOut()[i] >= 0)
	   {
		 (*OnGanglionicOuts)(i).rgb.r=0;
		 (*OnGanglionicOuts)(i).rgb.g=0;
		 (*OnGanglionicOuts)(i).rgb.b=abs(EyeRetinaBCone.GetARetinaOut()[i]);
	   }
	   else
	   {
		 (*OnGanglionicOuts)(i).rgb.r=0;
		 (*OnGanglionicOuts)(i).rgb.g=0;
		 (*OnGanglionicOuts)(i).rgb.b=0;
	   }
   // Офф- канал
   if(EyeRetina.GetARetinaOut()[i] < EyeRetinaRCone.GetARetinaOut()[i] &&
	  EyeRetina.GetARetinaOut()[i] < EyeRetinaGCone.GetARetinaOut()[i] &&
	  EyeRetina.GetARetinaOut()[i] < EyeRetinaBCone.GetARetinaOut()[i] &&
	  EyeRetina.GetARetinaOut()[i] < 0)
	  {
			(*OffGanglionicOuts)(i).rgb.r=(*OffGanglionicOuts)(i).rgb.g=(*OffGanglionicOuts)(i).rgb.b=abs(EyeRetina.GetARetinaOut()[i]);
	  }
   else	if(EyeRetinaRCone.GetARetinaOut()[i] < EyeRetinaGCone.GetARetinaOut()[i] &&
	   EyeRetinaRCone.GetARetinaOut()[i] < EyeRetinaBCone.GetARetinaOut()[i] &&
	   EyeRetinaRCone.GetARetinaOut()[i] < 0)
	   {
		 (*OffGanglionicOuts)(i).rgb.r=abs(EyeRetinaRCone.GetARetinaOut()[i]);
		 (*OffGanglionicOuts)(i).rgb.g=0;
		 (*OffGanglionicOuts)(i).rgb.b=0;
	   }
	else if(EyeRetinaGCone.GetARetinaOut()[i] < EyeRetinaBCone.GetARetinaOut()[i] &&
			EyeRetinaGCone.GetARetinaOut()[i] < 0)
	   {
		 (*OffGanglionicOuts)(i).rgb.r=0;
		 (*OffGanglionicOuts)(i).rgb.g=abs(EyeRetinaGCone.GetARetinaOut()[i]);
		 (*OffGanglionicOuts)(i).rgb.b=0;
	   }
	else if(EyeRetinaBCone.GetARetinaOut()[i] < 0)
	   {
		 (*OffGanglionicOuts)(i).rgb.r=0;
		 (*OffGanglionicOuts)(i).rgb.g=0;
		 (*OffGanglionicOuts)(i).rgb.b=abs(EyeRetinaBCone.GetARetinaOut()[i]);
	   }
       else
	   {
		 (*OffGanglionicOuts)(i).rgb.r=0;
		 (*OffGanglionicOuts)(i).rgb.g=0;
		 (*OffGanglionicOuts)(i).rgb.b=0;
	   }
  }

  // Выводим отдельно цветные каналы он- и офф-
  for(i=0;i<EyeRetina.GetGangWidth()*EyeRetina.GetGangHeight();i++)
  {
   (*RConeGanglionicOuts)(i).rgb.r=0;
   (*RConeGanglionicOuts)(i).rgb.g=0;
   (*RConeGanglionicOuts)(i).rgb.b=abs(EyeRetinaBCone.GetARetinaOut()[i]);
   (*GConeGanglionicOuts)(i).rgb.r=0;
   (*GConeGanglionicOuts)(i).rgb.g=abs(EyeRetinaGCone.GetARetinaOut()[i]);
   (*GConeGanglionicOuts)(i).rgb.b=0;
   (*BConeGanglionicOuts)(i).rgb.r=abs(EyeRetinaRCone.GetARetinaOut()[i]);
   (*BConeGanglionicOuts)(i).rgb.g=0;
   (*BConeGanglionicOuts)(i).rgb.b=0;
   //(*RodGanglionicOuts)[i]=abs(EyeRetina.GetARetinaOut()[i]);
   // Инвертнём
   (*RodGanglionicOuts)[i]=255-abs(EyeRetina.GetARetinaOut()[i]);
//   if(EyeRetina.GetARetinaOut()[i] < 0)
//    int a = 0;
  }
  // <--------------

  // Выходы для мотонейронов
  // Активность выходов с групп ганглиозных клеток, отвечающих за рефлекторные
  // движения четырёх глазных мышц
  LeftGanglionicOut(0,0) = (EyeRetina.LeftOnActivity * EyeRetina.GetRodReduceCoef() + EyeRetinaGCone.LeftOnActivity + EyeRetinaRCone.LeftOnActivity + EyeRetinaBCone.LeftOnActivity)/4;
  RightGanglionicOut(0,0) = (EyeRetina.RightOnActivity * EyeRetina.GetRodReduceCoef() + EyeRetinaGCone.RightOnActivity + EyeRetinaRCone.RightOnActivity + EyeRetinaBCone.RightOnActivity)/4;
  TopGanglionicOut(0,0) = (EyeRetina.TopOnActivity * EyeRetina.GetRodReduceCoef() + EyeRetinaGCone.TopOnActivity + EyeRetinaRCone.TopOnActivity + EyeRetinaBCone.TopOnActivity)/4;
  BottomGanglionicOut(0,0) = (EyeRetina.BottomOnActivity * EyeRetina.GetRodReduceCoef() + EyeRetinaGCone.BottomOnActivity + EyeRetinaRCone.BottomOnActivity + EyeRetinaBCone.BottomOnActivity)/4;

  // ДЛЯ ОТЛАДКИ
  // АКТИВНОСТЬ ВХОДОВ ДЛЯ 4-ч ГЛАЗНЫХ МЫШЦ
  LeftMuscle->SetRes(EyeRetina.GetGangWidth(),EyeRetina.GetGangHeight(),ubmRGB24);
  RightMuscle->SetRes(EyeRetina.GetGangWidth(),EyeRetina.GetGangHeight(),ubmRGB24);
  TopMuscle->SetRes(EyeRetina.GetGangWidth(),EyeRetina.GetGangHeight(),ubmRGB24);
  BottomMuscle->SetRes(EyeRetina.GetGangWidth(),EyeRetina.GetGangHeight(),ubmRGB24);
  // Заполняем белым
  LeftMuscle->Fill(0x00FFFFFF);
  RightMuscle->Fill(0x00FFFFFF);
  TopMuscle->Fill(0x00FFFFFF);
  BottomMuscle->Fill(0x00FFFFFF);
  // Рисуем ось
  const UBRect rect(0,EyeRetina.GetGangHeight()/2,EyeRetina.GetGangWidth(),1);
  LeftMuscle->Fill(0x00000000,rect);
  RightMuscle->Fill(0x00000000,rect);
  TopMuscle->Fill(0x00000000,rect);
  BottomMuscle->Fill(0x00000000,rect);
  // Активность мышц
  int activity = 0;
  // Левая
  activity = int(double(LeftGanglionicOut(0,0)*((EyeRetinaGCone.GetGangHeight()/2)-1)) / 255.0);
  if(activity > (EyeRetinaGCone.GetGangHeight()/2)-1) activity = (EyeRetinaGCone.GetGangHeight()/2)-1;
  const UBRect rect_on_left(0,(EyeRetinaGCone.GetGangHeight()/2)-activity,EyeRetinaGCone.GetGangWidth(),activity);
  LeftMuscle->Fill(0x0000FF00,rect_on_left);
  activity = int(double(EyeRetina.LeftOffActivity*((EyeRetina.GetGangHeight()/2)-1)) / 255.0);
  if(activity > (EyeRetinaGCone.GetGangHeight()/2)-1) activity = (EyeRetinaGCone.GetGangHeight()/2)-1;
  const UBRect rect_off_left(0,(EyeRetinaGCone.GetGangHeight()/2)+1,EyeRetinaGCone.GetGangWidth(),activity);
  LeftMuscle->Fill(0x000000FF,rect_off_left);
  // Правая
  activity = int(double(RightGanglionicOut(0,0)*((EyeRetinaGCone.GetGangHeight()/2)-1)) / 255.0);
  if(activity > (EyeRetinaGCone.GetGangHeight()/2)-1) activity = (EyeRetinaGCone.GetGangHeight()/2)-1;
  const UBRect rect_on_right(0,(EyeRetinaGCone.GetGangHeight()/2)-activity,EyeRetinaGCone.GetGangWidth(),activity);
  RightMuscle->Fill(0x0000FF00,rect_on_right);
  activity = int(double(EyeRetina.RightOffActivity*((EyeRetinaGCone.GetGangHeight()/2)-1)) / 255.0);
  if(activity > (EyeRetinaGCone.GetGangHeight()/2)-1) activity = (EyeRetinaGCone.GetGangHeight()/2)-1;
  const UBRect rect_off_right(0,(EyeRetinaGCone.GetGangHeight()/2)+1,EyeRetinaGCone.GetGangWidth(),activity);
  RightMuscle->Fill(0x000000FF,rect_off_right);
  // Верхняя
  activity = int(double(TopGanglionicOut(0,0)*((EyeRetinaGCone.GetGangHeight()/2)-1)) / 255.0);
  if(activity > (EyeRetinaGCone.GetGangHeight()/2)-1) activity = (EyeRetinaGCone.GetGangHeight()/2)-1;
  const UBRect rect_on_top(0,(EyeRetinaGCone.GetGangHeight()/2)-activity,EyeRetinaGCone.GetGangWidth(),activity);
  TopMuscle->Fill(0x0000FF00,rect_on_top);
  activity = int(double(EyeRetina.TopOffActivity*((EyeRetinaGCone.GetGangHeight()/2)-1)) / 255.0);
  if(activity > (EyeRetinaGCone.GetGangHeight()/2)-1) activity = (EyeRetinaGCone.GetGangHeight()/2)-1;
  const UBRect rect_off_top(0,(EyeRetinaGCone.GetGangHeight()/2)+1,EyeRetinaGCone.GetGangWidth(),activity);
  TopMuscle->Fill(0x000000FF,rect_off_top);
  // Нижняя
  activity = int(double(BottomGanglionicOut(0,0)*((EyeRetinaGCone.GetGangHeight()/2)-1)) / 255.0);
  if(activity > (EyeRetinaGCone.GetGangHeight()/2)-1) activity = (EyeRetinaGCone.GetGangHeight()/2)-1;
  const UBRect rect_on_bottom(0,(EyeRetinaGCone.GetGangHeight()/2)-activity,EyeRetinaGCone.GetGangWidth(),activity);
  BottomMuscle->Fill(0x0000FF00,rect_on_bottom);
  activity = int(double(EyeRetinaGCone.BottomOffActivity*((EyeRetinaGCone.GetGangHeight()/2)-1)) / 255.0);
  if(activity > (EyeRetina.GetGangHeight()/2)-1) activity = (EyeRetinaGCone.GetGangHeight()/2)-1;
  const UBRect rect_off_bottom(0,(EyeRetinaGCone.GetGangHeight()/2)+1,EyeRetinaGCone.GetGangWidth(),activity);
  BottomMuscle->Fill(0x000000FF,rect_off_bottom);
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool NEyeRetina::ADefault(void)
{
 EyeRetina.ResetDefaults();
 // Колбочки
 EyeRetinaRCone.ResetDefaults();
 EyeRetinaGCone.ResetDefaults();
 EyeRetinaBCone.ResetDefaults();
 // Выходы для мотонейронов
 // Активность выходов с групп ганглиозных клеток, отвечающих за рефлекторные
 // движения четырёх глазных мышц
 LeftGanglionicOut.Assign(1,1,0.0);
 RightGanglionicOut.Assign(1,1,0.0);
 TopGanglionicOut.Assign(1,1,0.0);
 BottomGanglionicOut.Assign(1,1,0.0);
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NEyeRetina::ABuild(void)
{
 return true;
}

// Сброс процесса счета без потери настроек
bool NEyeRetina::AReset(void)
{
 EyeWidth=320; EyeHeight=240;
 InputImage->SetRes(EyeWidth,EyeHeight);
 EyeX=CaptureImage->GetWidth()/2;
 EyeY=CaptureImage->GetHeight()/2;

 EyeRetina.SetRetinaRes(EyeWidth,
						 EyeHeight,1);
  EyeRetina.SetGangCalcEnabled(true);

 // Колбочки
 EyeRetinaRCone.SetRetinaRes(EyeWidth,
						 EyeHeight,1);
 EyeRetinaRCone.SetGangCalcEnabled(true);
 EyeRetinaGCone.SetRetinaRes(EyeWidth,
						 EyeHeight,1);
 EyeRetinaGCone.SetGangCalcEnabled(true);
 EyeRetinaBCone.SetRetinaRes(EyeWidth,
						 EyeHeight,1);
 EyeRetinaBCone.SetGangCalcEnabled(true);

 EyeTraectory.clear();

 return true;
}

// Выполняет расчет этого объекта
bool NEyeRetina::ACalculate(void)
{
 pair<int,int> coord;
 coord.first=EyeX;
 coord.second=EyeY;
 EyeTraectory.push_back(coord);

// if(!VideoOutputForm->GetActiveVideoOutputFrame())
// {
//  MCaptureStopClick(Sender);
//  MCaptureOptionsClick(Sender);
//  return;
// }
// CaptureImage=VideoOutputForm->GetActiveVideoOutputFrame()->BmpSource;
 if(CaptureImage->GetWidth() <=0 || CaptureImage->GetHeight() <=0)
  return true;

 EyeWidth=CaptureImage->GetWidth(); EyeHeight=CaptureImage->GetHeight();

 InputImage->SetRes(EyeWidth,EyeHeight,ubmRGB32);
 CaptureImage->SetColorModel(ubmRGB24);

 InputImage->SetRes(EyeWidth,EyeHeight);
 EyeX=CaptureImage->GetWidth()/2;
 EyeY=CaptureImage->GetHeight()/2;

 EyeRetina.SetRetinaRes(EyeWidth,
						 EyeHeight,1);
 // Колбочки
 EyeRetinaRCone.SetRetinaRes(EyeWidth,
						 EyeHeight,1);
 EyeRetinaGCone.SetRetinaRes(EyeWidth,
						 EyeHeight,1);
 EyeRetinaBCone.SetRetinaRes(EyeWidth,
						 EyeHeight,1);
 //UBResizeEdges(CaptureImage,InputImage);            // TODO: Включить
 CaptureImage->ConvertTo(*InputImage);
// *InputImage=*CaptureImage;
// InputImage->SetColorModel(ubmRGB32);

 // Выводим траекторию
/* CaptureForm->Image1->Canvas->Pen->Color=clRed;
 for(int i=0;i<int(EyeTraectory.size());i++)
 {
  CaptureForm->Image1->Canvas->MoveTo(EyeTraectory[i].first,EyeTraectory[i].second);
  if(i<int(EyeTraectory.size())-1);
   CaptureForm->Image1->Canvas->LineTo(EyeTraectory[i+1].first,EyeTraectory[i+1].second);
 }                */

   // ЕСЛИ СЕТЧАТКА ОБРАБАТЫВАЕТ ТОЛЬКО Ч/Б УСТАНОВИТЬ:
   //EyeRetina.SetRodReduceCoef(1.0);
   // В ПРОТИВНОМ СЛУЧАЕ ЗАКОММЕНТИТЬ

   EyeRetina.SetInputImage((UColorT*)InputImage->GetData());
   EyeRetina.Calculate();

   // Колбочки
   EyeRetinaRCone.SetInputImage((UColorT*)InputImage->GetData(),RCone);
   EyeRetinaRCone.Calculate();
   EyeRetinaGCone.SetInputImage((UColorT*)InputImage->GetData(),GCone);
   EyeRetinaGCone.Calculate();
   EyeRetinaBCone.SetInputImage((UColorT*)InputImage->GetData(),BCone);
   EyeRetinaBCone.Calculate();

 UpdateImages();

 /*if(EyeRetina.LeftActivity>0)
  EyeX-=4;
 if(EyeRetina.RightActivity>0)
  EyeX+=4;
 if(EyeRetina.TopActivity>0)
  EyeY-=4;
 if(EyeRetina.BottomActivity>0)
  EyeY+=4; */

 return true;
}
// --------------------------

}
#endif



