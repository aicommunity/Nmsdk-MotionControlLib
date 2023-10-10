// ===========================================================
// Version:        3.0.3
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2002.
E-mail:		alexab@ailab.ru
url:		http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

//#pragma hdrstop

#include "NFrequencyReceiver.h"

//---------------------------------------------------------------------------
namespace NMSDK{

/* *************************************************************************** */
// --------------------------
// Constructors & destructors
// --------------------------
NFrequencyReceiver::NFrequencyReceiver(void)
// : NReceiver(name),
 : LeftRange("LeftRange",this,&NFrequencyReceiver::SetLeftRange),
   RightRange("RightRange",this,&NFrequencyReceiver::SetRightRange),
   FrequencyStep("FrequencyStep",this,&NFrequencyReceiver::SetFrequencyStep),

   OutputFreq("OutputFreq",this),
   PulseCounter("PulseCounter",this),
   Input("Input",this),
   Results("Results",this)
{
 PrevPulseStartTime=0;
 PrevPulseStopTime=0;
}

NFrequencyReceiver::~NFrequencyReceiver(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NFrequencyReceiver* NFrequencyReceiver::New(void)
{
 return new NFrequencyReceiver;
}
// --------------------------

// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
// Устанавливает левую границу наблюдаемого интервала частот
bool NFrequencyReceiver::SetLeftRange(const double &range)
{
 if(range<0)
  return false;

 Ready=false;

 return true;
}

// Устанавливает правую границу наблюдаемого интервала частот
bool NFrequencyReceiver::SetRightRange(const double &range)
{
 if(range<0)
  return false;

 Ready=false;

 return true;
}

// Устанавливает шаг по частоте
bool NFrequencyReceiver::SetFrequencyStep(const double &step)
{
 if(step<0)
  return false;

 Ready=false;

 return true;
}
// --------------------------

// --------------------------
// Watch results control methods
// --------------------------
// Return number of results points
size_t NFrequencyReceiver::GetResultsSize(void) const
{
 return Results->GetCols();
}
// --------------------------

// --------------------------
// Computation methods
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NFrequencyReceiver::ADefault(void)
{
 LeftRange=0;
 RightRange=500;
 FrequencyStep=1;

 Input->Assign(1,1,0.0);
 OutputFreq.Assign(2,1,0.0);
 Results.Assign(1,1,0.0);;

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NFrequencyReceiver::ABuild(void)
{

 return true;
}

// Reset computation
bool NFrequencyReceiver::AReset(void)
{
 Results.Resize(2,0);

 OutputFreq.Assign(2,1,0.0);

 Results.Resize(2,int((RightRange-LeftRange)/FrequencyStep+1));
 int i(0);
 for(double f=LeftRange;f<RightRange;f+=FrequencyStep,++i)
  {
   Results(0,i)=f;
   Results(1,i)=0;
  }

 PrevPulseStartTime=-1;
 PrevPulseStopTime=-1;
 PulseCounter=0;

 return true;
}

// Execute math. computations of current object on current step
bool NFrequencyReceiver::ACalculate(void)
{
 double cs,frequency;
 list<double>::iterator I,J,K,L;

 cs=(*Input)(0,0);

   if(cs>0)
   {
	if(PrevPulseStartTime == -1)
	{
	 if(PrevPulseStopTime != -1)
	 {
	  frequency=(double)1.0/PrevPulseStopTime;
	  if(frequency >= LeftRange && frequency <= RightRange)
	  {
	   for(int ii=1;ii<Results.GetCols();ii++)
		if(frequency>=Results(0,ii-1) && frequency<Results(0,ii))
		 ++Results(1,ii-1);

	   ++PulseCounter;
	  }


	  PrevPulseStartTime=0;
	  PrevPulseStopTime=-1;
	 }
	 else
	 {
	  PrevPulseStartTime+=1.0/TimeStep;
	 }

	 OutputFreq.Resize(2,Results.GetCols());
	 for(int i=0;i<Results.GetCols();i++)
	 {
	  OutputFreq(1,i)=Results(1,i);
	 }
	 for(int i=0;i<Results.GetCols();i++)
	 {
	  OutputFreq(0,i)=Results(0,i);
	 }
	 return true;
	}
	else
	{
	 PrevPulseStartTime+=1.0/TimeStep;
	}
   }
   else
   {
	if(PrevPulseStartTime != -1)
	{
	 PrevPulseStopTime=PrevPulseStartTime;
	 PrevPulseStartTime=-1;
	}
	else
	if(PrevPulseStopTime>=0)
	 PrevPulseStopTime+=1.0/TimeStep;
   }



 OutputFreq.Resize(2,Results.GetCols());

	 for(int i=0;i<Results.GetCols();i++)
	 {
	  OutputFreq(1,i)=Results(1,i);
	 }
	 for(int i=0;i<Results.GetCols();i++)
	 {
	  OutputFreq(0,i)=Results(0,i);
	 }

 return true;
}
// --------------------------
/* *************************************************************************** */

}
//---------------------------------------------------------------------------

