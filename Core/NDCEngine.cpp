// ===========================================================
// Version:        1.0.1
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2009.
E-mail:			alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#include "NDCEngine.h"

//---------------------------------------------------------------------------
namespace NMSDK{

/* *************************************************************************** */
// --------------------------
// Constructors & destructors
// --------------------------
NDCEngine::NDCEngine(void)
// : NADItem(name),
 : EMFactor("EMFactor",this,&NDCEngine::SetEMFactor),
   Inductance("Inductance",this,&NDCEngine::SetInductance),
   Resistance("Resistance",this,&NDCEngine::SetResistance),
   Tm("Tm",this,&NDCEngine::SetTm),
   ReductionRate("ReductionRate",this,&NDCEngine::SetReductionRate),
   OutMoment("OutMoment",this),
   InputVoltage("InputVoltage",this),
   InputMomentum("InputMomentum",this),
   OutputMomentum("OutputMomentum",this),
   OutputAngle("OutputAngle",this),
   OutputAngleSpeed("OutputAngleSpeed",this)
{
 AddLookupProperty("Angle",ptPubState,new UVProperty<double,NDCEngine>(this,&Angle));
}

NDCEngine::~NDCEngine(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NDCEngine* NDCEngine::New(void)
{
 return new NDCEngine;
}
// --------------------------

// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
bool NDCEngine::SetEMFactor(const double &value)
{
 if(value<=0)
  return false;

 return true;
}

bool NDCEngine::SetInductance(const double &value)
{
 if(value<=0)
  return false;

 return true;
}

bool NDCEngine::SetResistance(const double &value)
{
 if(value<=0)
  return false;

 return true;
}

// Электромеханическая постоянная времени
bool NDCEngine::SetTm(const double &value)
{
 if(value<=0)
  return false;

 return true;
}

// Передаточное число
bool NDCEngine::SetReductionRate(const double &value)
{
 if(value<=0)
  return false;

 return true;
}
// --------------------------

// --------------------------
// Computation methods
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NDCEngine::ADefault(void)
{
 EMFactor=2;
 Inductance=0.1;
 Resistance=1;
 Tm=0.1;
 ReductionRate=1;
 OutMoment=0.0;

 InputVoltage->Assign(1,1,0.0);
 InputMomentum->Assign(1,1,0.0);

 OutputMomentum.Assign(1,1,0.0);
 OutputAngle.Assign(1,1,0.0);
 OutputAngleSpeed.Assign(1,1,0.0);

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NDCEngine::ABuild(void)
{

 return true;
}

// Reset computation
bool NDCEngine::AReset(void)
{
 Current=0;
 EMF=0;
 Angle=0;
 AngleSpeed=0;

 DiffMoment=0;

 return true;
}

// Execute math. computations of current object on current step
bool NDCEngine::ACalculate(void)
{
 vector<double> input;

 input.resize(2);

 if(InputVoltage.IsConnected())
  input[0]=(*InputVoltage)(0,0);
 else
  input[0]=0;


 if(InputMomentum.IsConnected())
  //input[1]=OutMoment();
  input[1]=(*InputMomentum)(0,0)+OutMoment();
 else
  input[1]=OutMoment();


 Current=((input[0]-EMF)*Resistance)/(TimeStep*Inductance)+(1.0-Resistance/(TimeStep*Inductance))*Current;
 EMF+=(Current-input[1]*Resistance/EMFactor)/(TimeStep*Tm);

 Moment=Current*EMFactor/Resistance;//EMF;

 OutputAngleSpeed(0,0)=EMF/EMFactor; // Угловая скорость //AngleSpeed;
 Angle+=OutputAngleSpeed(0,0)/(ReductionRate*TimeStep);
 OutputAngle(0,0)=Angle;
 OutputMomentum(0,0)=Moment;

 return true;
}
// --------------------------
/* *************************************************************************** */

}
//---------------------------------------------------------------------------

