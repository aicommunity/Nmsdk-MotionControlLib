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

#include "NNavMousePrimitive.h"

//---------------------------------------------------------------------------
namespace NMSDK{

/* *************************************************************************** */
// --------------------------
// Constructors & destructors
// --------------------------
NNavMousePrimitive::NNavMousePrimitive(void)
// : NADItem(name),
 : PoleSize("PoleSize",this,&NNavMousePrimitive::SetPoleSize),
   MouseSize("MouseSize",this,&NNavMousePrimitive::SetMouseSize),
   VibrissSize("VibrissSize",this,&NNavMousePrimitive::SetVibrissSize),
   Velocity("Velocity",this,&NNavMousePrimitive::SetVelocity),
   Frequency("Frequency",this),
   UseExternalInput("UseExternalInput",this),
   Delay("Delay",this),
   PainDelay("PainDelay",this),
   Amplitude("Amplitude",this),
   PulseLength("PulseLength",this),
   Input("Input",this),
   Output("Output",this),
   MotionControlSimple("MotionControlSimple",this),
   MotionControlSpikeForward("MotionControlSpikeForward",this),
   MotionControlSpikeBackward("MotionControlSpikeBackward",this),
   MotionControlSpikeStop("MotionControlSpikeStop",this),
   MotionControlState("MotionControlState",this),
   PainState("PainState",this),
   MousePosition("MousePosition",this),
   PainOutput("PainOutput",this),
   VibrissOutput("VibrissOutput",this)

{
OldFrequency=0.0;
}

NNavMousePrimitive::~NNavMousePrimitive(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NNavMousePrimitive* NNavMousePrimitive::New(void)
{
 return new NNavMousePrimitive;
}
// --------------------------

// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
bool NNavMousePrimitive::SetPoleSize(const double &value)
{
 if(value<=0)
  return false;
 //TODO: Обновление положения и выходов, но пока лучше размер во время работы не менять, и скидывать в ресет
 return true;
}

bool NNavMousePrimitive::SetVibrissSize(const double &value)
{
 if(value<=0)
  return false;
 //TODO: Обновление положения и выходов, но пока лучше размер во время работы не менять, и скидывать в ресет
 return true;
}

bool NNavMousePrimitive::SetMouseSize(const double &value)
{
 if(value<=0)
  return false;
 //TODO: Обновление положения и выходов, но пока лучше размер во время работы не менять, и скидывать в ресет
 return true;
}


bool NNavMousePrimitive::SetVelocity(const double &value)
{
 if(value<=0)
  return false;
 //TODO: Обновление положения и выходов, но пока лучше размер во время работы не менять, и скидывать в ресет
 return true;
}

// --------------------------

// --------------------------
// Computation methods
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NNavMousePrimitive::ADefault(void)
{
 //все в см
 PoleSize=100;
 MouseSize=20;
 VibrissSize=10;
 Velocity=2;

 //все как везде
 Frequency=1.5;
 Delay=0.1;//Я отнюдь не уверен, что именно тут задержка уместна в принципе...
 VibrissOutput.Resize(1,2);
 VibrissOutput.Assign(1,1,0.0);
 VibrissOutput.Assign(1,2,0.0);
 PainOutput.Resize(1,1);
 PainOutput.Assign(1,1,0.0);

 MotionControlState=0;

 //Стоим в нуле, отодвинувшись на целый радиус и еще на 1/10 радиуса, чтобы не пищать даже случайно
 MousePosition=0.0+MouseSize+MouseSize/10;

 //По дефолту принимаем вход со спайков
 UseExternalInput=true;

 //Это блин еще зачем ?!
 Input->Assign(1,1,0.0);
 Output.Assign(1,1,0.0);

 PainDelay=0.1;
 PainState=0;

 VibrDelays.resize(2, 0.0);
 VibrDelays[0]=0.0;
 VibrDelays[1]=0.0;
 ForwardSpikeTime=0.0;
 BackwardSpikeTime=0.0;
 StopSpikeTime=0.0;
 start_iter_time=0.0;

 PulseLength=0.001;

 Amplitude=1.0;

 ResetTime=0.0;
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NNavMousePrimitive::ABuild(void)
{

 return true;
}

// Reset computation
bool NNavMousePrimitive::AReset(void)
{
 MotionControlState=0;
 VibrDelays.resize(2, 0.0);
 VibrDelays[0]=0.0;
 VibrDelays[1]=0.0;
 ForwardSpikeTime=0.0;
 BackwardSpikeTime=0.0;
 StopSpikeTime=0.0;

 PainState=0;
 start_iter_time=0.0;
 //Это спорно, но логика подсказывает, что именно так и надо
 MousePosition=0.0+MouseSize+MouseSize/10;

 vibriss_counters.resize(2, 0.0);
 vibriss_counters[0]=0.0;
 vibriss_counters[1]=0.0;
 pain_counter=0;

 if(Environment)
  ResetTime = Environment->GetTime().GetDoubleTime();
 else
  ResetTime = 0.0;

 return true;
}

bool NNavMousePrimitive::UpdateState(void)
{
    //Определить свое состояние
    int state=100;
    bool updated=false;
    if(UseExternalInput)
    {

        double minimum=1000000000.0;
        if(fabs(StopSpikeTime)>0.001)
        {
            state=0;
            updated=true;
            minimum=StopSpikeTime;
        }
        if(fabs(ForwardSpikeTime)>0.001)
        {
            if(ForwardSpikeTime<minimum)
            {
                state=1;
                updated=true;
                minimum=ForwardSpikeTime;
            }
        }
        if(fabs(BackwardSpikeTime)>0.001)
        {
            if(BackwardSpikeTime<minimum)
            {
                state=-1;
                updated=true;
                minimum=BackwardSpikeTime;
            }
        }
    }
    else
    {
        state = MotionControlSimple;
        updated=true;
    }

    if(!updated)
        state = MotionControlState;

    //По умолчанию не болит, если вылезем за границы - заболит
    PainState=0;

    if(state==1)
    {
        MousePosition=MousePosition+Velocity;
        if((MousePosition+MouseSize)>=PoleSize)
        {
            MousePosition = PoleSize-MouseSize-MouseSize/10;
            PainState=1;
        }
    }

    if(state==-1)
    {
        MousePosition=MousePosition-Velocity;
        if((MousePosition-MouseSize)<=0)
        {
            MousePosition = MouseSize+MouseSize/10;
            PainState=1;
        }
    }

    //Рассчитать реакции вибрисс на итерации
    if((MousePosition+MouseSize+VibrissSize)>PoleSize)
    {
        double d = PoleSize-(MousePosition+MouseSize);// (MousePosition+MouseSize+VibrissSize)-PoleSize;
        double dd = ((VibrissSize-d)/VibrissSize)*0.2;
        VibrDelays[0] = Delay+dd;
    }
    else
    {
        VibrDelays[0] = Delay;
    }
    if((MousePosition-MouseSize-VibrissSize)<0)
    {
        double d = fabs(MousePosition-MouseSize); //проверить!
        double dd = ((VibrissSize-d)/VibrissSize)*0.2;
        VibrDelays[1] = Delay+dd;
    }
    else
    {
        VibrDelays[1] = Delay;
    }

    //Не забываем сохранить состояние на тот случай, если на будущем шаге команды не придут
    MotionControlState = state;

 return true;
}

// Execute math. computations of current object on current step
bool NNavMousePrimitive::ACalculate(void)
{
    if(Frequency<1e-8 || TimeStep/Frequency<1)
    {
     Output.ToZero();
     VibrissOutput.ToZero();
     PainOutput.ToZero();

     /*
     OutputPulseTimes.ToZero();
     AvgFrequencyCounter->clear();
     OldFrequency=Frequency;

     pulses.resize(PulseCounters.size(), false);
     for(int i=0; i<PulseCounters.size();i++)
     {
         PulseCounters[i]=0;
         pulses[i]=false;
     }
     */
     return true;
    }

    if(OldFrequency != Frequency.v)
    {
        for(int i=0; i<vibriss_counters.size();i++)
        {
            double Delay = VibrDelays[i];
            if(Environment->GetTime().GetDoubleTime() - ResetTime < Delay)
                continue;

            int &PulseCounter = vibriss_counters[i];
            //if(PulseCounter < 0 && PulseCounter < static_cast<int>(-int(TimeStep/Frequency)+PulseLength*TimeStep))
            PulseCounter = static_cast<int>(-int(TimeStep/Frequency)+PulseLength*TimeStep);
        }
        pain_counter=static_cast<int>(-int(TimeStep/Frequency)+PulseLength*TimeStep);

        OldFrequency=Frequency;
     }




 double iter_time = Environment->GetTime().GetDoubleTime() - start_iter_time; // Текущее время итерации
 double iter_length = (1.0 / Frequency) - (1.0 / double(TimeStep)); // Длина одной итерации

 //Оцениваем входы
 if(UseExternalInput)
 {
     if(MotionControlSpikeForward.IsConnected())
     {
         if(fabs((*MotionControlSpikeForward)(0,0))>0.001)
         {
             //Только первое ненулевое значение входа (=передний фронт 1-го спайка) имеет значение
             if(fabs(ForwardSpikeTime)<0.001)
             {
                 ForwardSpikeTime = iter_time; //Так же правильно - сколько времени прошло ?!
             }
         }
     }

     if(MotionControlSpikeBackward.IsConnected())
     {
         if(fabs((*MotionControlSpikeBackward)(0,0))>0.001)
         {
             //Только первое ненулевое значение входа (=передний фронт 1-го спайка) имеет значение
             if(fabs(BackwardSpikeTime)<0.001)
             {
                 BackwardSpikeTime = iter_time; //Так же правильно - сколько времени прошло ?!
             }
         }
     }

     if(MotionControlSpikeStop.IsConnected())
     {
         if(fabs((*MotionControlSpikeStop)(0,0))>0.001)
         {
             //Только первое ненулевое значение входа (=передний фронт 1-го спайка) имеет значение
             if(fabs(StopSpikeTime)<0.001)
             {
                 StopSpikeTime = iter_time; //Так же правильно - сколько времени прошло ?!
             }
         }
     }

 }

 //Формируем выходы

 //Обработать счетчики вибрисс
 for(int i=0; i<vibriss_counters.size();i++)
 {
  double Delay = VibrDelays[i];
  if(Environment->GetTime().GetDoubleTime() - ResetTime < Delay)
   continue;

  int &PulseCounter = vibriss_counters[i];
  if(PulseCounter>0) // Если импульс идет
  {
   --PulseCounter;
   if(PulseCounter <= 0) // Выключаем импульс и включаем ожидание
   {
    PulseCounter=static_cast<int>(-int(TimeStep/Frequency)+PulseLength*TimeStep);
    VibrissOutput(0, i) = 0.0;
   }
  }
  else
  {
   ++PulseCounter;
   if(PulseCounter >= 0) // Включаем импульс
   {
    PulseCounter=static_cast<RDK::UTime>(PulseLength*TimeStep);
    VibrissOutput(0, i) = Amplitude;
   }
  }
 }

 //Обработать счетчик "боли"
 double Delay = PainDelay;
 if((Environment->GetTime().GetDoubleTime() - ResetTime) > Delay)
 {
     int &PulseCounter = pain_counter;
     if(PulseCounter>0) // Если импульс идет
     {
      --PulseCounter;
      if(PulseCounter <= 0) // Выключаем импульс и включаем ожидание
      {
       //Включить ожидание на период, НО он никогда не дожидается, будет обновлено раньше в if(iter_time >= iter_length){...}
       PulseCounter=static_cast<int>(-int(TimeStep/Frequency)+PulseLength*TimeStep);
       //PainOutput(1,1)=0;
       PainOutput.Assign(1,1,0.0);
      }
     }
     else
     {
      ++PulseCounter;
      if(PulseCounter >= 0) // Включаем импульс
      {
       PulseCounter=static_cast<RDK::UTime>(PulseLength*TimeStep);
       PainOutput.Assign(1,1,Amplitude);
      }
     }
 }

 if(iter_time >= iter_length)
 {
     UpdateState();

     //Специфика такова, что задержки по факту меняются после каждой итерации и потому
     //после определения задержек надо что-то типа ресета, это и есть тут.
     for(int i=0; i<vibriss_counters.size();i++)
     {
         double Delay = VibrDelays[i];
         if(Environment->GetTime().GetDoubleTime() - ResetTime < Delay)
             continue;

         int &PulseCounter = vibriss_counters[i];
         //if(PulseCounter < 0 && PulseCounter < static_cast<int>(-int(TimeStep/Frequency)+PulseLength*TimeStep))

         PulseCounter = static_cast<int>(-int(Delay*TimeStep)); //static_cast<int>(-int(TimeStep/Frequency));
     }
     double Delay = PainDelay;
     if((Environment->GetTime().GetDoubleTime() - ResetTime) > Delay)
     {
      int &PulseCounter = pain_counter;
      if(PainState>0)
      {
          if(PulseCounter < 0 && PulseCounter < static_cast<int>(-int(Delay*TimeStep)+PulseLength*TimeStep))
           PulseCounter = static_cast<int>(-int(Delay*TimeStep));
      }
      else
      {
          PulseCounter = static_cast<int>(-int(TimeStep/Frequency)*2);
      }
     }

     //Сбросить маркеры спайков на входе после конца итерации
     ForwardSpikeTime=0.0;
     BackwardSpikeTime=0.0;
     StopSpikeTime=0.0;
     //Это пока того, на уровне идеи, в обучении все почему-то сложнее
     start_iter_time = Environment->GetTime().GetDoubleTime();
 }
 return true;
}
// --------------------------
/* *************************************************************************** */

}
//---------------------------------------------------------------------------

