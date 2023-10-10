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

#ifndef NMANIPULATOR_SOURCE_EMULATOR_CPP
#define NMANIPULATOR_SOURCE_EMULATOR_CPP

#include "NManipulatorSourceEmulator.h"

namespace NMSDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NManipulatorSourceEmulator::NManipulatorSourceEmulator(void)
: NManipulatorSource()
{
}

NManipulatorSourceEmulator::~NManipulatorSourceEmulator(void)
{
}
// --------------------------

// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NManipulatorSourceEmulator* NManipulatorSourceEmulator::New(void)
{
 return new NManipulatorSourceEmulator;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NManipulatorSourceEmulator::ADefault(void)
{
 Angle=2;
 Speed=0;
 Force=5;   

 return NManipulatorSource::ADefault();
}


// Сброс процесса счета.
bool NManipulatorSourceEmulator::AReset(void)
{
 UpdateOutputFlag=true;
 return NManipulatorSource::AReset();
}

// Выполняет расчет этого объекта
bool NManipulatorSourceEmulator::ACalculate(void)
{
 if(InputAngle.IsConnected())
  OutputAngle=*InputAngle;
 else
  OutputAngle.Resize(0,0);

 if(InputSpeed.IsConnected())
  OutputSpeed=*InputSpeed;
 else
  OutputSpeed.Resize(0,0);

 if(InputForce.IsConnected())
  OutputForce=*InputForce;
 else
  OutputForce.Resize(0,0);

 if(InputMovement.IsConnected())
  OutputMovement=*InputMovement;
 else
  OutputMovement.Resize(0,0);

 return true;
}
// --------------------------
}
#endif
