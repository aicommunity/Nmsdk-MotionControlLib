// ===========================================================
// Version:        1.0.0
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2009.
E-mail:		alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NSIGNUMSEPARATOR_CPP
#define NSIGNUMSEPARATOR_CPP

#include "NSignumSeparator.h"

namespace NMSDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NSignumSeparator::NSignumSeparator(void)
: Sign("Sign",this,&NSignumSeparator::SetSign),
  Gain("Gain",this,&NSignumSeparator::SetGain),
  Input("Input",this),
  Output("Output",this)
{
}

NSignumSeparator::~NSignumSeparator(void)
{
}
// --------------------------


// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
// Выделяемый знак
bool NSignumSeparator::SetSign(const double &value)
{
 return true;
}

// Множитель
bool NSignumSeparator::SetGain(const double &value)
{
 return true;
}
// --------------------------


// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NSignumSeparator* NSignumSeparator::New(void)
{
 return new NSignumSeparator;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NSignumSeparator::ADefault(void)
{
// AutoNumInputs=true;

 // Начальные значения всем параметрам
 // Амплитуда входных импульсов
 vector<double> values;

 values.assign(1,1);

 Sign=values;

 Gain=values;

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NSignumSeparator::ABuild(void)
{
 return true;
}

// Сброс процесса счета.
bool NSignumSeparator::AReset(void)
{
 // Сброс временных переменных
 Output.ToZero();
 return true;
}

// Выполняет расчет этого объекта
bool NSignumSeparator::ACalculate(void)
{
 double input(0.0);

 MDMatrix<double> &output_data_matrix=*Output;
 if(!Input.IsConnected())
 {
  output_data_matrix.Resize(0,0);
  return true;
 }

 int input_data_size=Input->GetCols();
 UEPtr<const MDMatrix<double> > input_data_matrix;
 if(Input.IsConnected())
  input_data_matrix=Input.operator->();

 output_data_matrix.Resize(1,int(input_data_size));

 if(input_data_size<int(Gain->size()))
  input_data_size=int(Gain->size());
 if(input_data_size<int(Sign->size()))
  input_data_size=int(Sign->size());

 for(int i=0;i<input_data_size;i++)
 {
   input=(*input_data_matrix)[i];

   if((input<0 && Sign[i] >0) || (input>0 && Sign[i] <0))
	output_data_matrix[i]=0;
   else
	output_data_matrix[i]=input*Sign[i]*Gain[i];
 }

 return true;
}
// --------------------------
}
#endif
