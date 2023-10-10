// ===========================================================
// Version:        3.0.2
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2002.
E-mail:        alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NCONTROL_OBJECT_SOURCE_CPP
#define NCONTROL_OBJECT_SOURCE_CPP

#include "NControlObjectSource.h"



namespace NMSDK {

// ������
// --------------------------
// ������������ � �����������
// --------------------------
NControlObjectSource::NControlObjectSource(void)
: NSource(),
  DataIndexes("DataIndexes",this),
  DataShift("DataShift",this,&NControlObjectSource::SetDataShift),
  DataMul("DataMul",this),
  Input("Input",this)
{
 UpdateOutputFlag=false;
}

NControlObjectSource::~NControlObjectSource(void)
{
}
// --------------------------

// --------------------------
// ������ ���������� �������������� ����������
// --------------------------
// ������������� ����, ��������, ������ 
bool NControlObjectSource::SetDataShift(const MDVector<double> &value)
{
 return true;
}
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
NControlObjectSource* NControlObjectSource::New(void)
{
 return new NControlObjectSource;
}
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool NControlObjectSource::ADefault(void)
{
// SetNumOutputs(5);
// SetOutputDataSize(0,MMatrixSize(1,1));
// SetOutputDataSize(1,MMatrixSize(1,1));
// SetOutputDataSize(2,MMatrixSize(1,1));
// SetOutputDataSize(3,MMatrixSize(1,1));
// SetOutputDataSize(4,MMatrixSize(1,1));

 return NSource::ADefault();
}


// ����� �������� �����.
bool NControlObjectSource::AReset(void)
{
 UpdateOutputFlag=true;

 if(Input.IsConnected() && Input->GetSize()>0)
 {
  DataShift.Resize(Input->GetSize(),0.0);
  DataIndexes.Resize(Input->GetSize(),0);
  DataMul.Resize(Input->GetSize(),1.0);
 }
 return NSource::AReset();
}

// ��������� ������ ����� �������
bool NControlObjectSource::ACalculate(void)
{
 if(Input.IsConnected() && Input->GetSize()>0)
 {
  DataShift.Resize(Input->GetSize(),0.0);
  DataIndexes.Resize(Input->GetSize(),0);
  DataMul.Resize(Input->GetSize(),1.0);
  Output.Resize(1,Input->GetSize());
 }
 else
  Output.Resize(0,0);

 for(int i=0;i<Input->GetSize();i++)
 {
  Output(0,i)=DataMul(i)*((*Input)(0,DataIndexes(i))-DataShift(i));
 }
 return true;
}
// --------------------------
}
#endif
