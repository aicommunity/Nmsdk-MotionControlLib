/* ***********************************************************
File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NAstaticGyro_CPP
#define NAstaticGyro_CPP
#include "NAstaticGyro.h"

namespace NMSDK {
// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NAstaticGyro::NAstaticGyro(void)
 : natural_freq("NaturalFrequency",this),
 z_g ("Z_g", this),
 alpha ("alpha", this),
 betta ("betta", this),
 gamma ("gamma", this),
 Input1("Input1", this),
 Input2("Input2", this),
 Input3("Input3", this),
 Output1("Output1", this),
 Output2("Output2", this),
 Output3("Output3", this)
{
 x11_prev=0;
 x12_prev=0;
 x21_prev=0;
 x22_prev=0;

 y11_prev=0;
 y12_prev=0;
 y21_prev=0;
 y22_prev=0;

 z11_prev=0;
 z12_prev=0;
 z21_prev=0;
 z22_prev=0;
}
NAstaticGyro::~NAstaticGyro(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NAstaticGyro* NAstaticGyro::New(void)
{
 return new NAstaticGyro;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NAstaticGyro::ADefault(void)
{
 natural_freq = 190.0;
 z_g = 0.707;

 Input1->Assign(1,1,0.0);
 Input2->Assign(1,1,0.0);
 Input3->Assign(1,1,0.0);

 Output1.Assign(1,1,0.0);
 Output2.Assign(1,1,0.0);
 Output3.Assign(1,1,0.0);
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NAstaticGyro::ABuild(void)
{
 return true;
}
// Сброс процесса счета без потери настроек
bool NAstaticGyro::AReset(void)
{
 alpha=0;
 betta=0;
 gamma=0;

 x11_prev=0;
 x12_prev=0;
 x21_prev=0;
 x22_prev=0;

 y11_prev=0;
 y12_prev=0;
 y21_prev=0;
 y22_prev=0;

 z11_prev=0;
 z12_prev=0;
 z21_prev=0;
 z22_prev=0;

 return true;
}
// Выполняет расчет этого объекта
bool NAstaticGyro::ACalculate(void)
{
 vector<double> input;

 if(!Input1.IsConnected())
  return true;

 if(!Input2.IsConnected())
  return true;

 if(!Input3.IsConnected())
  return true;

 input.resize(3);
 input[0]=(*Input1)(0,0);
 input[1]=(*Input2)(0,0);
 input[2]=(*Input3)(0,0);

// Нахождение угла alpha
double x11=x11_prev+(x12_prev)/TimeStep;
double x12=x12_prev+(natural_freq*natural_freq*(input[0]-x11_prev)-2*z_g*natural_freq*x12_prev)/TimeStep;
double x21=x21_prev+(x22_prev)/TimeStep;
double x22=x22_prev+(x11_prev)/TimeStep;
// Нахождение угла betta
double y11=y11_prev+(y12_prev)/TimeStep;
double y12=y12_prev+(natural_freq*natural_freq*(input[1]-y11_prev)-2*z_g*natural_freq*y12_prev)/TimeStep;
double y21=y21_prev+(y22_prev)/TimeStep;
double y22=y22_prev+(y11_prev)/TimeStep;
// Нахождение угла gamma
double z11=z11_prev+(z12_prev)/TimeStep;
double z12=z12_prev+(natural_freq*natural_freq*(input[2]-z11_prev)-2*z_g*natural_freq*z12_prev)/TimeStep;
double z21=z21_prev+(z22_prev)/TimeStep;
double z22=z22_prev+(z11_prev)/TimeStep;

 x11_prev=x11;
 x12_prev=x12;
 x21_prev=x21;
 x22_prev=x22;

 y11_prev=y11;
 y12_prev=y12;
 y21_prev=y21;
 y22_prev=y22;

 z11_prev=z11;
 z12_prev=z12;
 z21_prev=z21;
 z22_prev=z22;

 alpha=x21;
 betta=y21;
 gamma=z21;

 Output1(0,0)=alpha;  //угол alpha
 Output2(0,0)=betta;  //угол betta
 Output3(0,0)=gamma;  //угол gamma

 return true;
}
// --------------------------
}
#endif
