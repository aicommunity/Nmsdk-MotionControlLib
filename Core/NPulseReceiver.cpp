/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2012.
E-mail:        alexab@ailab.ru
url:        http://ailab.ru

This file - part of the project: RDK

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

//#pragma hdrstop

#include "NPulseReceiver.h"

//---------------------------------------------------------------------------
namespace NMSDK {

/* *************************************************************************** */
// --------------------------
// Constructors & destructors
// --------------------------
NPulseReceiver::NPulseReceiver(void)
 : MaxAccumulationRange("MaxAccumulationRange",this,&NPulseReceiver::SetMaxAccumulationRange),
   Inputs("Inputs",this),
   Output("Output",this)
{
}

NPulseReceiver::~NPulseReceiver(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NPulseReceiver* NPulseReceiver::New(void)
{
 return new NPulseReceiver;
}
// --------------------------

// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
// Суммарное число импульсов
bool NPulseReceiver::SetMaxAccumulationRange(const double &value)
{
 if(value<0)
  return false;

 Ready=false;

 return true;
}
// --------------------------

// --------------------------
// Computation methods
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NPulseReceiver::ADefault(void)
{
 MaxAccumulationRange=10;

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NPulseReceiver::ABuild(void)
{
 return true;
}

// Reset computation
bool NPulseReceiver::AReset(void)
{
 Output.Assign(int(Inputs->size())*2,1,0.0);
 MatrixData.clear();
 for(size_t i=0;i<PulseFlag.size();i++)
  PulseFlag[i]=false;

 return true;
}

// Execute math. computations of current object on current step
bool NPulseReceiver::ACalculate(void)
{
 Data.resize(Inputs->size());
 MatrixData.resize(Inputs->size()*2);
 PulseFlag.resize(Inputs->size());

  //Для выхода MDMatrix
 for(int i=0;i<int(Inputs->size());i++)
 {
  if(Inputs[i](0,0)>0)
  {
   if(!PulseFlag[i])
   {
    MatrixData[i*2].push_back(Environment->GetTime().GetDoubleTime());
    MatrixData[i*2+1].push_back(1.0);
    PulseFlag[i]=true;
   }
  }
  else
  {
   if(PulseFlag[i])
   {
    MatrixData[i*2].push_back(Environment->GetTime().GetDoubleTime());
    MatrixData[i*2+1].push_back(0.0);
    PulseFlag[i]=false;
   }
  }

  list<double>::iterator I,J,K,L;
  I=MatrixData[i*2].begin(),J=MatrixData[i*2].end();
  K=MatrixData[i*2+1].begin(),L=MatrixData[i*2+1].end();

  if(MaxAccumulationRange!=0)
  {
   while((MatrixData[i*2].back()-MatrixData[i*2].front())>MaxAccumulationRange)
   {
    ++I;
    MatrixData[i*2].pop_front();
    ++K;
    MatrixData[i*2+1].pop_front();
   }
  }
  Output.Resize(int(Inputs->size())*2, int(MatrixData[i*2].size()));
  for(int j=0;I!=J;++I,++j)
   Output(i*2,j)=*I;
  for(int j=0;K!=L;++K,++j)
   Output(i*2+1,j)=*K;

 }

 return true;
}
// --------------------------
/* *************************************************************************** */

}
//---------------------------------------------------------------------------


