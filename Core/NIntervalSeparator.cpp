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

#ifndef NINTERVAL_SEPARATOR_CPP
#define NINTERVAL_SEPARATOR_CPP

#include "NIntervalSeparator.h"

namespace NMSDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NIntervalSeparator::NIntervalSeparator(void)
: MinRange("MinRange",this,&NIntervalSeparator::SetMinRange),
MaxRange("MaxRange",this,&NIntervalSeparator::SetMaxRange),
Mode("Mode",this,&NIntervalSeparator::SetMode),
Gain("Gain",this,&NIntervalSeparator::SetGain),
Input("Input",this),
Output("Output",this)
{
}

NIntervalSeparator::~NIntervalSeparator(void)
{
}
// --------------------------


// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
// Нижняя граница выделяемого интервала
bool NIntervalSeparator::SetMinRange(const double &value)
{
 return true;
}

// Верхняя граница выделяемого интервала
bool NIntervalSeparator::SetMaxRange(const double &value)
{
 return true;
}

// Режим разделения
bool NIntervalSeparator::SetMode(const int &value)
{
 if(value < 0 && value >3)
  return false;

 return true;
}

// Множитель выходного значения
bool NIntervalSeparator::SetGain(const double &value)
{
 return true;
}
// --------------------------


// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NIntervalSeparator* NIntervalSeparator::New(void)
{
 return new NIntervalSeparator;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NIntervalSeparator::ADefault(void)
{
 // Начальные значения всем параметрам
 // Амплитуда входных импульсов
 vector<double> values;

 values.assign(1,0);
 MinRange=values;

 values.assign(1,1);
 MaxRange=values;

 vector<int> mode;
 mode.assign(1,2);
 Mode=mode;

 values.assign(1,1);
 Gain=values;

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NIntervalSeparator::ABuild(void)
{
 return true;
}

// Сброс процесса счета.
bool NIntervalSeparator::AReset(void)
{
 // Сброс временных переменных
 Output->ToZero();
 return true;
}

// Выполняет расчет этого объекта
bool NIntervalSeparator::ACalculate(void)
{
 double input(0.0);
 int num_inputs;
 if(Input.IsConnected())
  num_inputs=Input->GetCols();
 else
  num_inputs=0;

 UEPtr<const MDMatrix<double> > input_data_matrix;
 if(Input.IsConnected())
  input_data_matrix=Input.operator->();

 MDMatrix<double> &output_data_matrix=*Output;
 output_data_matrix.Resize(1,num_inputs);

 for(int j=0;j<num_inputs;j++)
  {
   input=(*input_data_matrix)[j];
   switch(Mode[j])
   {
   case 0:
	if(input<MinRange[j] || input>MaxRange[j])
	 output_data_matrix[j]=0;
	else
	 output_data_matrix[j]=input;
   break;

   case 1:
	if(input<MinRange[j] || input>MaxRange[j])
	 output_data_matrix[j]=0;
	else
	 output_data_matrix[j]=input-MinRange[j];
   break;

   case 2:
	if(input>MinRange[j])
	 output_data_matrix[j]=input;
   break;

   case 3:
	if(input<MaxRange[j])
	 output_data_matrix[j]=input;
	else
	 output_data_matrix[j]=0;
   break;

   case 4:
	if(input>MinRange[j])
	 output_data_matrix[j]=input-MinRange[j];
	else
	 output_data_matrix[j]=0;
   break;

   case 5:
	if(input>=0)
	{
	 if(input>MinRange[j] && input<MaxRange[j])
	  output_data_matrix[j]=input-MinRange[j];
	 else
	 if(input>MaxRange[j] && MaxRange[j]>0)
	  output_data_matrix[j]=MaxRange[j]-MinRange[j];
	 else
	  output_data_matrix[j]=0;
	}
	else
	{
	 if(input>MinRange[j] && input<MaxRange[j])
	  output_data_matrix[j]=input-MaxRange[j];
	 else
	 if(input<MinRange[j] && MinRange[j]<0)
	  output_data_matrix[j]=MinRange[j]-MaxRange[j];
	 else
	  output_data_matrix[j]=0;
	}
   break;

   case 6:
	if(input>=0)
	{
	 if(input>MinRange[j] && input<MaxRange[j])
	  output_data_matrix[j]=input-MinRange[j];
	 else
	 if(input>MaxRange[j] && MaxRange[j]>0)
	  output_data_matrix[j]=input-MinRange[j];
	 else
	  output_data_matrix[j]=0;
	}
	else
	{
	 if(input>MinRange[j] && input<MaxRange[j])
	  output_data_matrix[j]=input-MaxRange[j];
	 else
	 if(input<MinRange[j] && MinRange[j]<0)
	  output_data_matrix[j]=input-MaxRange[j];
	 else
	  output_data_matrix[j]=0;
	}
   break;
   }
   output_data_matrix[j]*=Gain[j];
//   POutputData[0].Double[j]=fabs(POutputData[0].Double[j]);
  }
 return true;
}
// --------------------------
}
#endif
