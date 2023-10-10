#ifndef NEYERETINABWCORE_CPP
#define NEYERETINABWCORE_CPP

#include "NEyeRetinaBWCore.h"

// ������
// --------------------------
// ������������ � �����������
// --------------------------
NEyeRetinaBWCore::NEyeRetinaBWCore(void)
{
 ReadyState=false;

 InputImage=0;
 GanglionicHistory=0;
 ARetinaOut=0;

 BipolarHistory=0;
 BipolarHistorySum=0;
 GanglionicHistorySum=0;

 BiMin=BiMax=GangMin=GangMax=0;
 BiMinHistory=BiMaxHistory=0;
 GangMinHistory=GangMaxHistory=0;

 GangCalcEnabled=true;
 BiWidth=BiHeight=0;
 GangWidth=GangHeight=0;
 BipolarRecSize=0;
 HorizontRecSize=0;
 GanglionicRecSize=0;
 AmocrineRecSize=0;
 BipolarHistorySize=0;
 GanglionicHistorySize=0;

 BiNeibBuf=0;
 BiNeibBufSize=0;

 BiNeibBufNumbers=0;
 GangNeibBuf=0;

 BiWeights=0;
 HorWeights=0;
 GangWeights=0;
 AmWeights=0;

 CurrBipolarHistIndex=0;
 CurrGanglionicHistIndex=0;
 CurrMinMaxHistIndex=0;

 RodReduceCoef = 2.0;
 ConeFreqThr = 40;

 InputImageLength=GangLength=BiWidth=BiHeight=GangWidth=GangHeight=0;
 GangStep=0;

}

NEyeRetinaBWCore::~NEyeRetinaBWCore(void)
{
 DestroyDataStructure();

 delete []BiWeights;
 delete []HorWeights;
 delete []GangWeights;
 delete []AmWeights;
}
// --------------------------

// --------------------------
// ������ ���������� �����������
// --------------------------
bool NEyeRetinaBWCore::SetGangCalcEnabled(bool enabled)
{
 if(GangCalcEnabled == enabled)
  return true;

 GangCalcEnabled=enabled;
 if(GangCalcEnabled)
  SetRetinaRes(BiWidth, BiHeight, 1);

 return true;
}

bool NEyeRetinaBWCore::SetRetinaRes(int width, int height, int gangstep)
{
 if(BiWidth == width && BiHeight == height && GangStep == gangstep)
  return true;

 DestroyDataStructure();

 BiWidth=width;
 BiHeight=height;
 InputImageLength=BiWidth*BiHeight;
 GangStep=gangstep;
 GangWidth=BiWidth/GangStep;
 GangHeight=BiHeight/GangStep;
 GangLength=GangWidth*GangHeight;

 return true;
}

void NEyeRetinaBWCore::SetBipolarRecSize(int size)
{
 if(BipolarRecSize == size+1)// || BipolarRecSize > HorizontRecSize)
  return;

 BipolarRecSize=size+1;

 delete []BiWeights;
 BiWeights=new int[BipolarRecSize];

 CreateWeights(BiWeights, BipolarRecSize);
}

void NEyeRetinaBWCore::SetHorizontRecSize(int size)
{
 if(HorizontRecSize == size+1)
  return;

 DestroyDataStructure();

 HorizontRecSize=size+1;
 BiNeibBufSize=HorizontRecSize;

 delete []HorWeights;
 HorWeights=new int[HorizontRecSize];

 CreateWeights(HorWeights, HorizontRecSize);
}

void NEyeRetinaBWCore::SetGanglionicRecSize(int size)
{
 if(GanglionicRecSize == size+1)// || GanglionicRecSize > AmocrineRecSize)
  return;

 GanglionicRecSize=size+1;

 delete []GangWeights;
 GangWeights=new int[GanglionicRecSize];

 CreateWeights(GangWeights, GanglionicRecSize);
}

void NEyeRetinaBWCore::SetAmocrineRecSize(int size)
{
 if(AmocrineRecSize == size+1)
  return;

 DestroyDataStructure();

 AmocrineRecSize=size+1;
 GangNeibBufSize=AmocrineRecSize;

 delete []AmWeights;
 AmWeights=new int[AmocrineRecSize];

 CreateWeights(AmWeights, AmocrineRecSize);
}


bool NEyeRetinaBWCore::SetBipolarHistorySize(int size)
{
 if(size < 1)
  return false;

 if(BipolarHistorySize == size+1)
  return true;

 DestroyDataStructure(); 

 BipolarHistorySize=size+1;
 return true;
}

bool NEyeRetinaBWCore::SetGanglionicHistorySize(int size)
{
 if(size < 1)
  return false;

 if(GanglionicHistorySize == size+1)
  return true;

 DestroyDataStructure(); 

 GanglionicHistorySize=size+1;

 return true;
}


bool NEyeRetinaBWCore::SetMinMaxHistorySize(int size)
{
 if(size < 1)
  return false;

 if(MinMaxHistorySize == size)
  return true;

 DestroyDataStructure(); 

 MinMaxHistorySize=size;
 return true;
}

bool NEyeRetinaBWCore::SetRodReduceCoef(double coef)
{
 if(coef < 1.0)
  return false;

 RodReduceCoef=coef;
 return true;
}

bool NEyeRetinaBWCore::SetConeFreqThr(int thr)
{
 if(thr < 0)
  return false;

 ConeFreqThr=thr;
 return true;
}
// --------------------------

// --------------------------
// ������ �����
// --------------------------
// ������������� ����� �����������
bool NEyeRetinaBWCore::SetInputImage(UColorT *bmp, ReceptorType rec_type)
{
 if(!ReadyState)
  UpdateDataStructure();

 if(!bmp)
  {
   InputImage=0;
   Reset();
   return true;
  }

  if(rec_type != Rod && rec_type != RCone && rec_type != GCone && rec_type != BCone)
	return false;

 InputImage=(UColorT*)bmp;
 InputImageCalculate(rec_type);

 return true;
}

// ����� ������ � �������� ��������� (� �����������
// �������� ����������)
void NEyeRetinaBWCore::Reset(void)
{
 if(!ReadyState)
  UpdateDataStructure();
  
 ResetBipolarHistory();
 if(GangCalcEnabled) // ���� ������ ������ ����������� ������ ��������
  ResetGanglionicHistory();

 BiMin=BiMax=GangMin=GangMax=0;
 ResetMinMaxHistory(); 
}

// ����� ������ � �������� ��������� (� ���������������
// �������� �� ���������)
void NEyeRetinaBWCore::ResetDefaults(void)
{
 SetRetinaRes(10, 10, 1);

 // ���� � �������
 // �� ������ �������� �� ����������
 /*SetHorizontRecSize(3);
 SetBipolarRecSize(3);

 SetAmocrineRecSize(3);
 SetGanglionicRecSize(3);

 SetBipolarHistorySize(10);
 SetGanglionicHistorySize(10); */
 SetHorizontRecSize(7);
 SetBipolarRecSize(7);

 SetAmocrineRecSize(7);
 SetGanglionicRecSize(7);

 SetBipolarHistorySize(10);
 SetGanglionicHistorySize(10);

 SetMinMaxHistorySize(100);

 UpdateDataStructure();
}

// ������
void NEyeRetinaBWCore::Calculate(void)
{
 if(!ReadyState)
  UpdateDataStructure();

 if(!InputImage)
  return;

 BipolarCalculate();
 if(GangCalcEnabled) // ���� ������ ������ ����������� ������ ��������
  GanglionicCalculate();

 // ���������� ����������� ������ �� ���������
 int threshold = int(ConeFreqThr / RodReduceCoef);

 LeftOnActivity=0;
 LeftOffActivity=0;
 int on_counter = 0;
 int off_counter = 0;
 int left_shift = int(GangWidth/3);
 for(int i=0;i<GangHeight;i++)
  for(int j=0;j<left_shift;j++)
  {
   if(ARetinaOut[i*GangWidth+j] > threshold)
   {
	 LeftOnActivity+=ARetinaOut[i*GangWidth+j];
	 on_counter++;
   }
   else if(ARetinaOut[i*GangWidth+j] < -threshold)
   {
	 LeftOffActivity-=ARetinaOut[i*GangWidth+j];
	 off_counter++;
   }
  }
 LeftOnActivity = (on_counter)? LeftOnActivity/on_counter : 0;
 LeftOffActivity = (off_counter)? LeftOffActivity/off_counter : 0;


 TopOnActivity=0;
 TopOffActivity=0;
 on_counter = 0;
 off_counter = 0;
 int top_shift = int(GangHeight/3);
 for(int i=0;i<GangWidth;i++)
  for(int j=0;j<top_shift;j++)
  //for(int j=GangHeight-top_shift;j<GangHeight;j++)
  {
   if(ARetinaOut[j*GangWidth+i] > threshold)
   {
	 TopOnActivity+=ARetinaOut[j*GangWidth+i];
	 on_counter++;
   }
   else if(ARetinaOut[j*GangWidth+i] < -threshold)
   {
	 TopOffActivity-=ARetinaOut[j*GangWidth+i];
	 off_counter++;
   }
  }
 TopOnActivity = (on_counter)? TopOnActivity/on_counter : 0;
 TopOffActivity = (off_counter)? TopOffActivity/off_counter : 0;

 RightOnActivity=0;
 RightOffActivity=0;
 on_counter = 0;
 off_counter = 0;
 int right_shift = int(GangWidth/3);
 for(int i=0;i<GangHeight;i++)
  for(int j=GangWidth-right_shift;j<GangWidth;j++)
  {
   if(ARetinaOut[i*GangWidth+j] > threshold)
   {
	 RightOnActivity+=ARetinaOut[i*GangWidth+j];
	 on_counter++;
   }
   else if(ARetinaOut[i*GangWidth+j] < -threshold)
   {
	 RightOffActivity-=ARetinaOut[i*GangWidth+j];
	 off_counter++;
   }
  }
 RightOnActivity = (on_counter)? RightOnActivity/on_counter : 0;
 RightOffActivity = (off_counter)? RightOffActivity/off_counter : 0;

 BottomOnActivity=0;
 BottomOffActivity=0;
 on_counter = 0;
 off_counter = 0;
 int bottom_shift = (GangHeight/3);
 for(int i=0;i<GangWidth;i++)
  for(int j=GangHeight-bottom_shift;j<GangHeight;j++)
  //for(int j=0;j<bottom_shift;j++)
  {
   if(ARetinaOut[j*GangWidth+i] > threshold)
   {
	 BottomOnActivity+=ARetinaOut[j*GangWidth+i];
	 on_counter++;
   }
   else if(ARetinaOut[j*GangWidth+i] < -threshold)
   {
	 BottomOffActivity-=ARetinaOut[j*GangWidth+i];
	 off_counter++;
   }
  }
 BottomOnActivity = (on_counter)? BottomOnActivity/on_counter : 0;
 BottomOffActivity = (off_counter)? BottomOffActivity/off_counter : 0;
}
// --------------------------


// --------------------------
// ������� ������ ���������� �����������
// --------------------------
// ������� ����� BiNeibBuf 
void NEyeRetinaBWCore::CreateBiNeibBuf(void)
{
 int i,j;

 BiNeibBuf=new int**[InputImageLength];
 BiNeibBufNumbers=new int*[InputImageLength];
 for(j=0;j<InputImageLength;j++)
  {
   BiNeibBuf[j]=new int*[BiNeibBufSize];
   BiNeibBuf[j][0]=new int[1];
   for(i=1;i<BiNeibBufSize;i++)
    BiNeibBuf[j][i]=new int[i<<3];
   BiNeibBufNumbers[j]=new int[BiNeibBufSize];
   SearchNeighbors(j,j,BiNeibBufSize-1,BiNeibBuf,BiNeibBufNumbers);
  }
}

// ������� ����� GangNeibBuf 
void NEyeRetinaBWCore::CreateGangNeibBuf(void)
{
 int i,j,k;
 int x,y;

 GangNeibBuf=new int**[GangLength];
 GangNeibBufNumbers=new int*[GangLength];
 for(j=0;j<GangLength;j++)
  {
   GangNeibBuf[j]=new int*[GangNeibBufSize];
   GangNeibBuf[j][0]=new int[1];
   for(i=1;i<GangNeibBufSize;i++)
	GangNeibBuf[j][i]=new int[i<<3];
   GangNeibBufNumbers[j]=new int[GangNeibBufSize];

   y=j/GangWidth;
   x=j-y*GangWidth;
   k=(y*BiWidth+x)*GangStep;
   SearchNeighbors(k,j,GangNeibBufSize-1,GangNeibBuf,GangNeibBufNumbers);
  }
}

// ���������� ���� ������� �� ����������� ���� ������� 0-level
void NEyeRetinaBWCore::CreateWeights(int *weights, size_t level)
{
// int sum=100;

/* for(size_t i=level-1;i>=0;i--)
  {
   sum>>=1;
   weights[i]=sum;
  }
 weights[0]+=sum;*/
 if(level == 2)
  {
   weights[0]=33;
   weights[1]=67;
  }
 else
 if(level == 4)
  {
   weights[0]=44;
   weights[1]=48;
   weights[2]=6;
   weights[3]=2;
  } 
 else
 if(level == 8)
  {
   weights[0]=48;
   weights[1]=29;
   weights[2]=15;
   weights[3]=10;
   weights[4]=6;
   weights[5]=3;
   weights[6]=2;
   weights[7]=1;
  } 
/* if(level == 2)
  {
   weights[0]=33;
   weights[1]=67;
  }
 else
 if(level == 4)
  {
   weights[0]=44;
   weights[1]=48;
   weights[2]=6;
   weights[3]=2;
  } 
 else
 if(level == 8)
  {
   weights[0]=24;
   weights[1]=38;
   weights[2]=20;
   weights[3]=10;
   weights[4]=6;
   weights[5]=3;
   weights[6]=2;
   weights[7]=1;
  }         */

}
// --------------------------

// --------------------------
// ������� ������ �����
// --------------------------
// ������� ��� ��������� ������
void NEyeRetinaBWCore::DestroyDataStructure(void)
{
 int i,j;

 InputImage=0;

 if(!ReadyState)
  return;

 if(BipolarHistory)
  {
   for(i=0;i<BipolarHistorySize;i++)
    delete []BipolarHistory[i];
   delete []BipolarHistory;
   BipolarHistory=0;
  }
 delete []BipolarHistorySum;
 BipolarHistorySum=0;

 if(GanglionicHistory && GangCalcEnabled)
  {
   for(i=0;i<GanglionicHistorySize;i++)
    delete []GanglionicHistory[i];
   delete []GanglionicHistory;
   GanglionicHistory=0;
  }
 delete []GanglionicHistorySum;
 GanglionicHistorySum=0;

 delete []BiMinHistory;
 BiMinHistory=0;
 delete []BiMaxHistory;
 BiMaxHistory=0;
 if(GangMinHistory && GangCalcEnabled)
  {
   delete []GangMinHistory;
   GangMinHistory=0;
  }
 if(GangMaxHistory && GangCalcEnabled)
  {
   delete []GangMaxHistory;
   GangMaxHistory=0;
  } 

 delete []ARetinaOut;
 ARetinaOut=0;

 for(j=0;j<InputImageLength;j++)
  {
   for(i=0;i<BiNeibBufSize;i++)
    delete []BiNeibBuf[j][i];
   delete []BiNeibBuf[j];
   delete []BiNeibBufNumbers[j];
  }
 delete []BiNeibBuf;
 BiNeibBuf=0;
 delete []BiNeibBufNumbers;
 BiNeibBufNumbers=0;

 if(GangNeibBuf && GangCalcEnabled)
  {
   for(j=0;j<GangLength;j++)
    {
     for(i=0;i<GangNeibBufSize;i++)
      delete []GangNeibBuf[j][i];
     delete []GangNeibBuf[j];
     delete []GangNeibBufNumbers[j];
    }
  }  
 delete []GangNeibBuf;
 GangNeibBuf=0;
 delete []GangNeibBufNumbers;
 GangNeibBufNumbers=0;

 ReadyState=false;
}

// �������� ������������ ���������� ��������� ������
// � ����� ������ (� ����������� ��������)
bool NEyeRetinaBWCore::UpdateDataStructure(void)
{
 int i;

 if(ReadyState)
  return true;

 BipolarHistory=new int*[BipolarHistorySize];
 for(i=0;i<BipolarHistorySize;i++)
  BipolarHistory[i]=new int[InputImageLength];
 BipolarHistorySum=new int[InputImageLength];

 if(GangCalcEnabled)
  {
   GanglionicHistory=new int*[InputImageLength];
   for(i=0;i<GanglionicHistorySize;i++)
    GanglionicHistory[i]=new int[InputImageLength];
   GanglionicHistorySum=new int[InputImageLength];
  }
 else
  {
   GanglionicHistory=0;
   GanglionicHistorySum=0;
  }

 BiMinHistory=new int[MinMaxHistorySize];
 BiMaxHistory=new int[MinMaxHistorySize];
 if(GangCalcEnabled)
  {
   GangMinHistory=new int[MinMaxHistorySize];
   GangMaxHistory=new int[MinMaxHistorySize];
  }

 ARetinaOut=new int[GangLength];

 CreateBiNeibBuf();
 if(GangCalcEnabled)
  CreateGangNeibBuf();

 ReadyState=true;
 Reset();
 return true;
}

// ������������� ������� �����������
void NEyeRetinaBWCore::InputImageCalculate(ReceptorType rec_type)
{
 int i;
 int k;
 int *CurrentBipolarHistory;
 int *CurrentBipolarHistorySum;
 //double retina_pix_length;

// retina_pix_length = sqrt(double((BiWidth/2)*(BiWidth/2) + (BiHeight/2)*(BiHeight/2)));

 // ������������ ������� ������ ���������
 if(CurrBipolarHistIndex >= BipolarHistorySize-1)
  CurrBipolarHistIndex=0;
 else
  ++CurrBipolarHistIndex;

 CurrentBipolarHistory=BipolarHistory[CurrBipolarHistIndex];
 for(i=0;i<InputImageLength;i++)
 {
  double h_pix_length, w_pix_length;
  double pix_length;
  double max_pix_length;
  double cone_reducu_lvl;

  int x, y;
  y =  i / BiWidth;
  x = i - y * BiWidth;

  max_pix_length = sqrt(double(BiHeight*BiHeight + BiWidth*BiWidth))/2.0;
  h_pix_length = fabs(double(BiHeight/2) - double(y));
  w_pix_length = fabs(double(BiWidth/2) - double(x));
  pix_length = sqrt(h_pix_length*h_pix_length + w_pix_length*w_pix_length);
  cone_reducu_lvl = 1.0 - (pix_length/max_pix_length);


  switch(rec_type)
  {
   case Rod:
   {
	CurrentBipolarHistory[i]=int(double((InputImage[i].rgb.r+InputImage[i].rgb.g+InputImage[i].rgb.b)/3)/RodReduceCoef);
	break;
   }
   case RCone:
   {
	CurrentBipolarHistory[i]=InputImage[i].rgb.r;
	if(CurrentBipolarHistory[i] < ConeFreqThr) CurrentBipolarHistory[i] = 0;
	CurrentBipolarHistory[i]= int(double(CurrentBipolarHistory[i])*cone_reducu_lvl);
	break;
   }
   case GCone:
   {
	CurrentBipolarHistory[i]=InputImage[i].rgb.g;
	if(CurrentBipolarHistory[i] < ConeFreqThr) CurrentBipolarHistory[i] = 0;
	CurrentBipolarHistory[i]= int(double(CurrentBipolarHistory[i])*cone_reducu_lvl);
	break;
   }
   case BCone:
   {
	CurrentBipolarHistory[i]=InputImage[i].rgb.b;
	if(CurrentBipolarHistory[i] < ConeFreqThr) CurrentBipolarHistory[i] = 0;
	CurrentBipolarHistory[i]= int(double(CurrentBipolarHistory[i])*cone_reducu_lvl);
	break;
   }
  }
 }

 // ��������� ����� ������� ������ ���������
 for(i=0;i<InputImageLength;i++)
  {
   CurrentBipolarHistorySum=&(BipolarHistorySum[i]);
   *CurrentBipolarHistorySum=0;

   for(k=0;k<(int)CurrBipolarHistIndex;k++)
    *CurrentBipolarHistorySum+=BipolarHistory[k][i];
   for(k=CurrBipolarHistIndex+1;k<(int)BipolarHistorySize;k++)
    *CurrentBipolarHistorySum+=BipolarHistory[k][i];
  }

}

// ������ ������� ����� ���������
void NEyeRetinaBWCore::BipolarCalculate(void)
{
 int l,i,j;
 int k;
 int hsum,sum;
 int** currneibbuf;
 int* currbufnumbers,*currneiblevel;
 int* BipolarOutput;
 int *CurrentGanglionicHistorySum;
 int *CurrentBipolarHistory;
 int BiPosMin=255,BiNegMax=-255;

  // ������������ ������� ��������� �������
 if(CurrMinMaxHistIndex >= MinMaxHistorySize-1)
  CurrMinMaxHistIndex=0;
 else
  CurrMinMaxHistIndex++;

 BiMinHistory[CurrMinMaxHistIndex]=0;
 BiMaxHistory[CurrMinMaxHistIndex]=0;

 if(GangCalcEnabled) // ���� ������ ������ ����������� ������ ��������
  {
   // ������������ ������� ������ ����������� ������
   if(CurrGanglionicHistIndex >= GanglionicHistorySize-1)
	CurrGanglionicHistIndex=0;
   else
	CurrGanglionicHistIndex++;

   BipolarOutput=GanglionicHistory[CurrGanglionicHistIndex];
  }
 else // ���� ������ ������ ����������� ������ ��������
  {
   BipolarOutput=ARetinaOut;
  }
 CurrentBipolarHistory=BipolarHistory[CurrBipolarHistIndex];

 for(l=0;l<InputImageLength;l++)
  {
   BipolarOutput[l]=0;
   currneibbuf=BiNeibBuf[l];
   currbufnumbers=BiNeibBufNumbers[l];

   // ������������ ����� �� ���������
   for(i=0;i<BipolarRecSize;i++)
    {
     sum=0;
     currneiblevel=currneibbuf[i];
	 for(j=0;j<currbufnumbers[i];j++)
	  sum+=CurrentBipolarHistory[currneiblevel[j]];
	 if(currbufnumbers[i])
	  BipolarOutput[l]+=(sum*BiWeights[i])/currbufnumbers[i];
	}
	BipolarOutput[l]/=100;

   // ������������ ����� �� �������������� ������
   sum=0;
   for(i=0;i<HorizontRecSize;i++)
    {
     hsum=0;
     currneiblevel=currneibbuf[i];
	 for(j=0;j<currbufnumbers[i];j++)
	  hsum+=BipolarHistorySum[currneiblevel[j]];
	 if(currbufnumbers[i])
	  sum+=(hsum*HorWeights[i])/currbufnumbers[i];
    }
   BipolarOutput[l]-=sum/((int)(BipolarHistorySize-1)*100);

   // ����������� ��������� �������� ������ ���������
   if(BipolarOutput[l]<BiMinHistory[CurrMinMaxHistIndex])
	BiMinHistory[CurrMinMaxHistIndex]=BipolarOutput[l];
   else
   if(BipolarOutput[l]>BiMaxHistory[CurrMinMaxHistIndex])
	BiMaxHistory[CurrMinMaxHistIndex]=BipolarOutput[l];

   if(BipolarOutput[l]>0 && BiPosMin>BipolarOutput[l])
	BiPosMin=BipolarOutput[l];

   if(BipolarOutput[l]<0 && BiNegMax<BipolarOutput[l])
	BiNegMax=BipolarOutput[l];

   BiMin=0; BiMax=0;
   for(i=0;i<MinMaxHistorySize;i++)
    {
     if(BiMinHistory[i]<BiMin)
      BiMin=BiMinHistory[i];
     if(BiMaxHistory[i]>BiMax)
      BiMax=BiMaxHistory[i];
    }
  }

 // ���������������
/* for(l=0;l<InputImageLength;l++)
 {
   if(BipolarOutput[l]>=0 && (BiMaxHistory[CurrMinMaxHistIndex]-BiPosMin))
	BipolarOutput[l]=((BipolarOutput[l]-BiPosMin)*255)/(BiMaxHistory[CurrMinMaxHistIndex]-BiPosMin);
   else
   if(BiNegMax-BiMinHistory[CurrMinMaxHistIndex])
	BipolarOutput[l]=((BipolarOutput[l]-BiNegMax)*255)/(BiNegMax-BiMinHistory[CurrMinMaxHistIndex]);

//  if(BipolarOutput[l]>0)
//  BipolarOutput[l]=(BipolarOutput[l]*510)/(BiMaxHistory[CurrMinMaxHistIndex]-BiMinHistory[CurrMinMaxHistIndex]);
 }*/


 if(GangCalcEnabled) // ���� ������ ������ ����������� ������ ��������
  {
   // ��������� ����� ������� ������ ����������� ������
   for(i=0;i<InputImageLength;i++)
    {
     CurrentGanglionicHistorySum=&(GanglionicHistorySum[i]);
     *CurrentGanglionicHistorySum=0;

     for(k=0;k<(int)CurrGanglionicHistIndex;k++)
      *CurrentGanglionicHistorySum+=GanglionicHistory[k][i];
     for(k=CurrGanglionicHistIndex+1;k<(int)GanglionicHistorySize;k++)
      *CurrentGanglionicHistorySum+=GanglionicHistory[k][i];
    }
  }  
}

// ������ ������� ����� ���������
void NEyeRetinaBWCore::GanglionicCalculate(void)
{
 int l,i,j;
 int hsum,sum;
 int** currneibbuf;
 int* currbufnumbers,*currneiblevel;
 int *CurrentGanglionicHistory;

 CurrentGanglionicHistory=GanglionicHistory[CurrGanglionicHistIndex];
 GangMinHistory[CurrMinMaxHistIndex]=0;
 GangMaxHistory[CurrMinMaxHistIndex]=0;

 for(l=0;l<GangLength;l++)
  {
   ARetinaOut[l]=0;
   currneibbuf=GangNeibBuf[l];
   currbufnumbers=GangNeibBufNumbers[l];

   // ������������ ����� �� ����������� ������
   for(i=0;i<GanglionicRecSize;i++)
    {
     sum=0;
     currneiblevel=currneibbuf[i];
	 for(j=0;j<currbufnumbers[i];j++)
	  sum+=CurrentGanglionicHistory[currneiblevel[j]];
	 if(currbufnumbers[i])
	  ARetinaOut[l]+=(sum*GangWeights[i])/currbufnumbers[i];
	}
   ARetinaOut[l]/=100;

   // ������������ ����� �� ����������� ������                               
   sum=0;
   for(i=0;i<AmocrineRecSize;i++)
    {
     hsum=0;
     currneiblevel=currneibbuf[i];
	 for(j=0;j<currbufnumbers[i];j++)
	  hsum+=GanglionicHistorySum[currneiblevel[j]];
	 if(currbufnumbers[i])
	  sum+=(hsum*AmWeights[i])/currbufnumbers[i];
	}
   ARetinaOut[l]-=sum/((int)(GanglionicHistorySize-1)*100);


   // ����������� ��������� �������� ������ ����������� ������ 
   if(ARetinaOut[l]<GangMinHistory[CurrMinMaxHistIndex])
    GangMinHistory[CurrMinMaxHistIndex]=ARetinaOut[l];
   else
   if(ARetinaOut[l]>GangMaxHistory[CurrMinMaxHistIndex])
    GangMaxHistory[CurrMinMaxHistIndex]=ARetinaOut[l];

   GangMin=0; GangMax=0;
   for(i=0;i<MinMaxHistorySize;i++)
    {
     if(GangMinHistory[i]<GangMin)
      GangMin=GangMinHistory[i];
     if(GangMaxHistory[i]>GangMax)
      GangMax=GangMaxHistory[i];
    }  
  }
}

// ������� ����� ������� ������ ���������
void NEyeRetinaBWCore::ResetBipolarHistory(void)
{
 int i;

 // ������������ ������� ������ ���������
 for(i=0;i<BipolarHistorySize;i++)
  memset(BipolarHistory[i],0,InputImageLength*sizeof(int));

 CurrBipolarHistIndex=BipolarHistorySize-1;
}

// ������� ����� ������� ������ ����������� ������
void NEyeRetinaBWCore::ResetGanglionicHistory(void)
{
 int i;

 // ������������ ������� ������ ���������
 for(i=0;i<GanglionicHistorySize;i++)
  memset(GanglionicHistory[i],0,InputImageLength*sizeof(int));

 CurrGanglionicHistIndex=GanglionicHistorySize-1;
}

// ������� ����� ������� ��������� ������� �������� ��������
void NEyeRetinaBWCore::ResetMinMaxHistory(void)
{
 memset(BiMinHistory,0,MinMaxHistorySize*sizeof(int));
 memset(BiMaxHistory,0,MinMaxHistorySize*sizeof(int));
 if(GangCalcEnabled)
  {
   memset(GangMinHistory,0,MinMaxHistorySize*sizeof(int));
   memset(GangMaxHistory,0,MinMaxHistorySize*sizeof(int));
  }
 CurrMinMaxHistIndex=MinMaxHistorySize-1;
}


// ���������� ����� ������� �������� � ������� index, ������ level
// � ���������� ��������� � ������� NeibBuf � NeibBufNumbers
// �� ������� putindex
void NEyeRetinaBWCore::SearchNeighbors(int index, int putindex, int level,
						int ***NeibBuf,
						int **NeibBufNumbers)
{
 int i,j;
 int x,y,k;
 int up,down,left,right;

 if(index > (int)InputImageLength || !level)
  return;

 for(k=0;k<level+1;k++)
  NeibBufNumbers[putindex][k]=0;

 y=index/BiWidth;
 x=index-y*BiWidth;
 up=(y-level<0)?-y:-level;
 down=(y+level<(int)BiHeight)?level:BiHeight-y-level;
 left=(x-level<0)?-x:-level;
 right=(x+level<(int)BiWidth)?level:BiWidth-x-level;
 for(j=up;j<=down;j++)
  for(i=left;i<=right;i++)
   {
    k=int(sqrt((double)(i*i+j*j)));
    if(k<=level)
     {
      NeibBuf[putindex][k][NeibBufNumbers[putindex][k]]=(j+y)*BiWidth+x+i;
      NeibBufNumbers[putindex][k]+=1;
     }
   }
}
// --------------------------


#endif
