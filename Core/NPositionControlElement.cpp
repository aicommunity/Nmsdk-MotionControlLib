/* ***********************************************************

E-mail:        
Url:           

This file is part of the project: 

File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NPositionControlElement_CPP
#define NPositionControlElement_CPP

#include "NPositionControlElement.h"

namespace NMSDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NPositionControlElement::NPositionControlElement(void)
:   MotionControl("MotionControl",this,0)
{
}

NPositionControlElement::~NPositionControlElement(void)
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
NPositionControlElement* NPositionControlElement::New(void)
{
 return new NPositionControlElement;
}
// --------------------------


// --------------------------
// Скрытые методы управления счетом 
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NPositionControlElement::ADefault(void)
{           
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NPositionControlElement::ABuild(void)
{
 return true;
}

// Сброс процесса счета без потери настроек
bool NPositionControlElement::AReset(void)
{
 return true;
}

// Выполняет расчет этого объекта
bool NPositionControlElement::ACalculate(void)
{
 MotionControl->NumMotionElements;
// vector<NNet*> Motions = MotionControl->GetMotion();
// NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[0]);
 return true;
}
// --------------------------

}
#endif



