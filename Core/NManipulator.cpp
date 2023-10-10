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

#include "NManipulator.h"

//---------------------------------------------------------------------------
namespace NMSDK{

/* *************************************************************************** */
// --------------------------
// Constructors & destructors
// --------------------------
NManipulator::NManipulator(void)
// : NADItem(name),
 :  EMFactor("EMFactor",this,&NManipulator::SetEMFactor),
   Inductance("Inductance",this,&NManipulator::SetInductance),
   Resistance("Resistance",this,&NManipulator::SetResistance),
   Input("Input",this),
   Output("Output",this)
{
 Current=0;
}

NManipulator::~NManipulator(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NManipulator* NManipulator::New(void)
{
 return new NManipulator;
}
// --------------------------

// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
bool NManipulator::SetEMFactor(const double &value)
{
 if(value<=0)
  return false;

 return true;
}

bool NManipulator::SetInductance(const double &value)
{
 if(value<=0)
  return false;

 return true;
}

bool NManipulator::SetResistance(const double &value)
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
bool NManipulator::ADefault(void)
{
 EMFactor=1;
 Inductance=1;
 Resistance=1;
 Input->Assign(1,1,0.0);
 Output.Assign(1,1,0.0);

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NManipulator::ABuild(void)
{

 return true;
}

// Reset computation
bool NManipulator::AReset(void)
{
 Current=0;
 return true;
}

// Execute math. computations of current object on current step
bool NManipulator::ACalculate(void)
{
 double cs=(*Input)(0,0);

 Current=(cs/TimeStep-0*EMFactor)/Inductance+(1.0-Resistance/(TimeStep*Inductance))*Current;

 Output(0,0)=Current;

 return true;
}
// --------------------------
/* *************************************************************************** */

}
//---------------------------------------------------------------------------

