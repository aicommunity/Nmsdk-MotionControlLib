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

#ifndef NMANIPULATOR_INPUT_CPP
#define NMANIPULATOR_INPUT_CPP

#include "NManipulatorInput.h"

namespace NMSDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NManipulatorInput::NManipulatorInput(void)
: NSource(),
  Voltage("Voltage",this,&NManipulatorInput::SetVoltage),
  Input("Input",this)
{
 UpdateOutputFlag=false;
}

NManipulatorInput::~NManipulatorInput(void)
{
}
// --------------------------

// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
// Устанавливает напряжение
bool NManipulatorInput::SetVoltage(const double &value)
{
 return true;
}

// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NManipulatorInput* NManipulatorInput::New(void)
{
 return new NManipulatorInput;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NManipulatorInput::ADefault(void)
{
 Voltage=0;   
 
 Input->Assign(1,1,0.0);
 Output.Assign(1,1,0.0);


 return NSource::ADefault();
}


// Сброс процесса счета.
bool NManipulatorInput::AReset(void)
{
 UpdateOutputFlag=true;
 return NSource::AReset();
}

// Выполняет расчет этого объекта
bool NManipulatorInput::ACalculate(void)
{
 Voltage=(*Input)(0,0);
 Output(0,0)=Voltage;
 
 return true;
}
// --------------------------
}
#endif
