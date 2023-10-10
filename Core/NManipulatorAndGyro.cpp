/* ***********************************************************
File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NManipulatorAndGyro_CPP
#define NManipulatorAndGyro_CPP
#include "NManipulatorAndGyro.h"
namespace NMSDK {
// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NManipulatorAndGyro::NManipulatorAndGyro(void)
 : Mass("Mass",this),
   Length ("Length", this),
   InputMomentumExt("InputMomentumExt",this),
   InputMomentum("InputMomentum",this),
   InputAngle("InputAngle",this),
   Output("Output",this)
{
 gravity_constant=0;
}
NManipulatorAndGyro::~NManipulatorAndGyro(void)
{
}
// --------------------------    

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NManipulatorAndGyro* NManipulatorAndGyro::New(void)
{
 return new NManipulatorAndGyro;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом 
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NManipulatorAndGyro::ADefault(void)
{
 Mass = 1;
 Length = 1;

 InputMomentumExt->Assign(1,1,0.0);
 InputMomentum->Assign(1,1,0.0);
 InputAngle->Assign(1,1,0.0);

 Output.Assign(1,1,0.0);

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NManipulatorAndGyro::ABuild(void)
{
 return true;
}

// Сброс процесса счета без потери настроек
bool NManipulatorAndGyro::AReset(void)
{
 gravity_constant = 9.8;
 return true;
}

// Выполняет расчет этого объекта
bool NManipulatorAndGyro::ACalculate(void)
{
 vector<double> input;

 input.resize(3);

 input[0]=(*InputMomentumExt)(0,0);
 input[1]=(*InputMomentum)(0,0);
 input[2]=(*InputAngle)(0,0);

 double external_moment = input[0]; //Внешний момент
 double engine_moment = input[1]; //Момент на валу двигателя
 double angle = input[2]; //текущий угол поворота двигателя

 Output(0,0) = - external_moment + engine_moment - Mass*gravity_constant*Length*sin(angle);

 return true;
}
// --------------------------
}
#endif
