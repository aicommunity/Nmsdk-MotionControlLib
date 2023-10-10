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

#ifndef NMANIPULATOR_SOURCE_CPP
#define NMANIPULATOR_SOURCE_CPP

#include "NManipulatorSource.h"

namespace NMSDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NManipulatorSource::NManipulatorSource(void)
: NSource(),
  Angle("Angle",this,&NManipulatorSource::SetAngle),
  Speed("Speed",this,&NManipulatorSource::SetSpeed),
  Force("Force",this,&NManipulatorSource::SetForce),
  Movement("Movement",this),
  InputAngle("InputAngle",this),
  InputSpeed("InputSpeed",this),
  InputForce("InputForce",this),
  InputMovement("InputMovement",this),
  OutputAngle("OutputAngle",this),
  OutputSpeed("OutputSpeed",this),
  OutputForce("OutputForce",this),
  OutputMovement("OutputMovement",this)
{
 UpdateOutputFlag=0;
}

NManipulatorSource::~NManipulatorSource(void)
{
}
// --------------------------

// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
// Устанавливает угол, скорость, момент 
bool NManipulatorSource::SetAngle(const double &value)
{
 return true;
}

bool NManipulatorSource::SetSpeed(const double &value)
{
 return true;
}

bool NManipulatorSource::SetForce(const double &value)
{
 return true;
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NManipulatorSource* NManipulatorSource::New(void)
{
 return new NManipulatorSource;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NManipulatorSource::ADefault(void)
{
 Angle=0;
 Speed=0;
 Force=0;
 Movement=0;

 return NSource::ADefault();
}


// Сброс процесса счета.
bool NManipulatorSource::AReset(void)
{
 UpdateOutputFlag=true;

 OutputAngle.ToZero();
 OutputSpeed.ToZero();
 OutputForce.ToZero();
 OutputMovement.ToZero();
 return NSource::AReset();
}

// Выполняет расчет этого объекта
bool NManipulatorSource::ACalculate(void)
{
 if(InputAngle.IsConnected())
  OutputAngle=*InputAngle-Angle;
 else
  OutputAngle.Resize(0,0);

 if(InputSpeed.IsConnected())
  OutputSpeed=*InputSpeed-Speed;
 else
  OutputSpeed.Resize(0,0);

 if(InputForce.IsConnected())
  OutputForce=*InputForce-Force;
 else
  OutputForce.Resize(0,0);

 if(InputMovement.IsConnected())
  OutputMovement=*InputMovement-Movement;
 else
  OutputMovement.Resize(0,0);

 return true;
}
// --------------------------
}
#endif
