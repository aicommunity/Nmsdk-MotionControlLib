#ifndef NEYERETINAMUSCLECORE_CPP
#define NEYERETINAMUSCLECORE_CPP

#include "NEyeRetinaMuscleCore.h"

// --------------------------
// ������������ � �����������
// --------------------------
NEyeRetinaMuscleCore::NEyeRetinaMuscleCore(void)
{
 LeftInputs=0;
 RightInputs=0;
 TopInputs=0;
 BottomInputs=0;

 RetinaOutWidth=0;
 RetinaOutHeight=0;
 MRecSize=0;
 ReadyState=false;
}

NEyeRetinaMuscleCore::~NEyeRetinaMuscleCore(void)
{
 DestroyDataStructure();
}
// --------------------------

// --------------------------
// ������ ���������� �����������
// --------------------------
// ������������� ���������� �������� � ������ ������������ ���� �����
void NEyeRetinaMuscleCore::SetRetinaRecParams(size_t rwidth, size_t rheight, size_t mrecsize)
{
 if(RetinaOutWidth == rwidth && RetinaOutHeight == rheight && MRecSize == mrecsize)
  return;

 DestroyDataStructure();

 RetinaOutWidth=rwidth;
 RetinaOutHeight=rheight;
 MRecSize=mrecsize;
}
// --------------------------


// --------------------------
// ������ �����
// --------------------------
// ��������� �������� ���������� ����������� �����
void NEyeRetinaMuscleCore::SetMRecData(unsigned char *retinaout)
{
 size_t i,j,k;

 for(i=0;i<RetinaOutWidth*MRecSize;i++)
  TopInputs[i]=&(retinaout[i]);

 j=(RetinaOutHeight-MRecSize)*RetinaOutWidth;
 for(i=j;i<RetinaOutWidth*RetinaOutHeight;i++)
  BottomInputs[i]=&(retinaout[i]);

 k=0;
 for(j=0;j<RetinaOutHeight;j++)
  for(i=0;i<MRecSize;i++)
   {
    LeftInputs[k]=&(retinaout[j*RetinaOutWidth+i]);
    k++;
   }

 k=0;
 for(j=0;j<RetinaOutHeight;j++)
  for(i=RetinaOutWidth-MRecSize;i<RetinaOutWidth;i++)
   {
    LeftInputs[k]=&(retinaout[j*RetinaOutWidth+i]);
    k++;
   }
}

// ����� ������ (� ����������� �������� ����������)
void NEyeRetinaMuscleCore::Reset(void)
{
 if(!ReadyState)
  CreateDataStructure();

 memset(TopInputs,0,sizeof(pbyte)*RetinaOutWidth*MRecSize);
 memset(BottomInputs,0,sizeof(pbyte)*RetinaOutWidth*MRecSize);
 memset(LeftInputs,0,sizeof(pbyte)*RetinaOutHeight*MRecSize);
 memset(RightInputs,0,sizeof(pbyte)*RetinaOutHeight*MRecSize);
}

// ����� ������ � ��������������� �������� �� ���������
void NEyeRetinaMuscleCore::ResetDefaults(void)
{

 Reset();
}

// ������
void NEyeRetinaMuscleCore::Calculate(void)
{
 // ��������!
 CalcMuscles();
}
// --------------------------


// --------------------------
// ������� ������ �����
// --------------------------
// ������� ��� ��������� ������
void NEyeRetinaMuscleCore::DestroyDataStructure(void)
{
 if(!ReadyState)
  return;

 if(LeftInputs)
  delete []LeftInputs;
 LeftInputs=0;

 if(RightInputs)
  delete []RightInputs;
 RightInputs=0;

 if(TopInputs)
  delete []TopInputs;
 TopInputs=0;

 if(BottomInputs)
  delete []BottomInputs;
 BottomInputs=0;

 ReadyState=false;
}

// ������� ��������� ������
void NEyeRetinaMuscleCore::CreateDataStructure(void)
{
 if(ReadyState)
  return;

 LeftInputs=new pbyte[RetinaOutHeight*MRecSize];
 RightInputs=new pbyte[RetinaOutHeight*MRecSize];
 TopInputs=new pbyte[RetinaOutWidth*MRecSize];
 BottomInputs=new pbyte[RetinaOutWidth*MRecSize];

 Reset();
 ReadyState=true; 
}


// ��������� ������ ����
void NEyeRetinaMuscleCore::CalcMuscles(void)
{
    //
}

// --------------------------


#endif


 
