/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2011.
E-mail:            alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */
//---------------------------------------------------------------------------
#ifndef NWPhysicalManipulatorCPP
#define NWPhysicalManipulatorCPP

#include "NWPhysicalManipulator.h"

namespace NMSDK {

/* *************************************************************************** */
// --------------------------
// Constructors & destructors
// --------------------------
NWPhysicalManipulator::NWPhysicalManipulator(void)
 :
 Input("Input",this),
 Output1("Output1",this),
 Output2("Output2",this),
 Output3("Output3",this)
{
 ManipulatorDLL=0;
 DMInit=0;
 DMDeInit=0;
 DMComOpen=0;
 DMComClose=0;
 DMComStatus=0;
 DMStart=0;
 DMStop=0;
 DMReset=0;
 DMMoveServo=0;
 DMGetServoData=0;
 DMSetExternalData=0;
 DMGetModelTime=0;

 DMGetState=0;
 DMNeiroPulse=0;
 DMNeiroPulseGroup=0;
 DMGetNeiroSumm=0;
 DMSetNeiroParam=0;
 DMSetWorkMode=0;

 WindowHandle=0;

 DMAngleServo=0;
 DMDiffServo=0;
 DMHoldServo=0;
 DMFreeServo=0;
 DMSetMinPWM=0;

// Диапазоны по току (моменту)
 AddLookupProperty("MinMoment",ptPubParameter,new UVProperty<double,NWPhysicalManipulator>(this,&MinMoment));
 AddLookupProperty("MaxMoment",ptPubParameter,new UVProperty<double,NWPhysicalManipulator>(this,&MaxMoment));

// Диапазоны по углу
 AddLookupProperty("MinAngle",ptPubParameter,new UVProperty<double,NWPhysicalManipulator>(this,&MinAngle));
 AddLookupProperty("MaxAngle",ptPubParameter,new UVProperty<double,NWPhysicalManipulator>(this,&MaxAngle));

//Дипазоны по входному напряжению
 AddLookupProperty("MinVoltage",ptPubParameter,new UVProperty<double,NWPhysicalManipulator>(this,&MinVoltage));
 AddLookupProperty("MaxVoltage",ptPubParameter,new UVProperty<double,NWPhysicalManipulator>(this,&MaxVoltage));

// Найденные диапазоны по току
 AddLookupProperty("FoundMinForce",ptPubState,new UVProperty<double,NWPhysicalManipulator>(this,&FoundMinForce));
 AddLookupProperty("FoundMaxForce",ptPubState,new UVProperty<double,NWPhysicalManipulator>(this,&FoundMaxForce));

// Найденные диапазоны по скорости
 AddLookupProperty("FoundMinSpeed",ptPubState,new UVProperty<double,NWPhysicalManipulator>(this,&FoundMinSpeed));
 AddLookupProperty("FoundMaxSpeed",ptPubState,new UVProperty<double,NWPhysicalManipulator>(this,&FoundMaxSpeed));

// Множитель выходного упрвляющего напряжения
 AddLookupProperty("OutputMul",ptPubParameter,new UVProperty<double,NWPhysicalManipulator>(this,&OutputMul));

// Длительность управляющего воздействия
 AddLookupProperty("TimeDuration",ptPubParameter,new UVProperty<double,NWPhysicalManipulator>(this,&TimeDuration));

// Минимальная величина управляющего воздействия
 AddLookupProperty("MinControlVoltage",ptPubParameter,new UVProperty<double,NWPhysicalManipulator>(this,&MinControlVoltage));

// Шаг по накоплению в контроллере за 1мс
 AddLookupProperty("AccumulationStep",ptPubParameter,new UVProperty<int,NWPhysicalManipulator>(this,&NWPhysicalManipulator::SetAccumulationStep,&NWPhysicalManipulator::GetAccumulationStep));

// Шаг по разряду в контроллере за 1мс
 AddLookupProperty("DissociationStep",ptPubParameter,new UVProperty<int,NWPhysicalManipulator>(this,&NWPhysicalManipulator::SetDissociationStep,&NWPhysicalManipulator::GetDissociationStep));

 AddLookupProperty("EmulatorMode",ptPubParameter,new UVProperty<bool,NWPhysicalManipulator>(this,&EmulatorMode));

 AddLookupProperty("ComPort",ptPubParameter,new UVProperty<int ,NWPhysicalManipulator>(this,&ComPort));

 AddLookupProperty("ServoNumber",ptPubParameter,new UVProperty<int ,NWPhysicalManipulator>(this,&ServoNumber));

// Режим работы dll манипулятора
 AddLookupProperty("DllManipulatorMode",ptPubParameter,new UVProperty<int ,NWPhysicalManipulator>(this,&DllManipulatorMode));

// Максимум счетчика пропусков задания управляющего воздействия
 AddLookupProperty("MaxSendCounter",ptPubParameter,new UVProperty<int ,NWPhysicalManipulator>(this,&MaxSendCounter));

// Максимум счетчика пропусков считывания данных
 AddLookupProperty("MaxReadCounter",ptPubParameter,new UVProperty<int ,NWPhysicalManipulator>(this,&MaxReadCounter));


// Состояния выходных нейронов
 AddLookupProperty("PulseState",ptPubState,new UVProperty<vector<int> ,NWPhysicalManipulator>(this,&PulseState));

 AddLookupProperty("PreviousAngles",ptPubState,new UVProperty<vector<double> ,NWPhysicalManipulator>(this,&PreviousAngles));

 AddLookupProperty("AnglesTimes",ptPubState,new UVProperty<vector<unsigned> ,NWPhysicalManipulator>(this,&AnglesTimes));

 AddLookupProperty("NumAnglesHistory",ptPubState,new UVProperty<size_t ,NWPhysicalManipulator>(this,&NumAnglesHistory));

 AddLookupProperty("AnglesHistoryTime",ptPubState,new UVProperty<double ,NWPhysicalManipulator>(this,&AnglesHistoryTime));

 AddLookupProperty("AngleSpeed",ptPubState,new UVProperty<double ,NWPhysicalManipulator>(this,&AngleSpeed));

// Выходное управляющее напряжение
 AddLookupProperty("OutputVoltage",ptPubState,new UVProperty<double ,NWPhysicalManipulator>(this,&OutputVoltage));

// Входное управляющее напряжение
 AddLookupProperty("InputVoltage",ptPubState,new UVProperty<double ,NWPhysicalManipulator>(this,&InputVoltage));
}

NWPhysicalManipulator::~NWPhysicalManipulator(void)
{
}
// --------------------------

// --------------------------
// Методы управления железом
// --------------------------
// Загрузка DLL
bool NWPhysicalManipulator::LoadManipulatorDll(void)
{
 if(!WindowHandle)
  return false;

 if(ManipulatorDLL)
  return true;

 // Идентификатор библиотеки
 ManipulatorDLL=LoadLibraryA("RoboLib.dll");

 if(!ManipulatorDLL)
  return false;

 DMInit=(PInit)GetProcAddress(ManipulatorDLL, "Init");
 DMDeInit=(PDeInit)GetProcAddress(ManipulatorDLL, "DeInit");
 DMComOpen=(PComOpen)GetProcAddress(ManipulatorDLL, "ComOpen");
 DMComClose=(PComClose)GetProcAddress(ManipulatorDLL, "ComClose");
 DMComStatus=(PComStatus)GetProcAddress(ManipulatorDLL, "ComStatus");
 DMStart=(PStart)GetProcAddress(ManipulatorDLL, "Start");
 DMStop=(PStop)GetProcAddress(ManipulatorDLL, "Stop");
 DMReset=(PReset)GetProcAddress(ManipulatorDLL, "Reset");
 DMMoveServo=(PMoveServo)GetProcAddress(ManipulatorDLL, "MoveServo");
 DMGetServoData=(PGetServoData)GetProcAddress(ManipulatorDLL, "GetServoData");
 DMSetExternalData=(PSetExternalData)GetProcAddress(ManipulatorDLL, "SetExternalData");
 DMGetModelTime=(PGetModelTime)GetProcAddress(ManipulatorDLL, "GetModelTime");

 DMGetState=(PGetState)GetProcAddress(ManipulatorDLL, "GetState");
 DMNeiroPulse=(PNeiroPulse)GetProcAddress(ManipulatorDLL, "NeiroPulse");
 DMNeiroPulseGroup=(PNeiroPulseGroup)GetProcAddress(ManipulatorDLL, "NeiroPulseGroup");

 DMGetNeiroSumm=(PGetNeiroSumm)GetProcAddress(ManipulatorDLL, "GetNeiroSumm");
 DMSetNeiroParam=(PSetNeiroParam)GetProcAddress(ManipulatorDLL, "SetNeiroParam");

 DMSetWorkMode=(PSetWorkMode)GetProcAddress(ManipulatorDLL, "SetWorkMode");

 DMAngleServo=(PAngleServo)GetProcAddress(ManipulatorDLL, "AngleServo");
 DMDiffServo=(PDiffServo)GetProcAddress(ManipulatorDLL, "DiffServo");
 DMHoldServo=(PHoldServo)GetProcAddress(ManipulatorDLL, "HoldServo");
 DMFreeServo=(PFreeServo)GetProcAddress(ManipulatorDLL, "FreeServo");
 DMSetMinPWM=(PSetMinPWM)GetProcAddress(ManipulatorDLL, "SetMinPWM");

 return true;
}

// Выгрузка DLL
bool NWPhysicalManipulator::UnLoadManipulatorDll(void)
{
 if(!ManipulatorDLL)
  return true;

 DMInit=0;
 DMDeInit=0;
 DMComOpen=0;
 DMComClose=0;
 DMComStatus=0;
 DMStart=0;
 DMStop=0;
 DMReset=0;
 DMMoveServo=0;
 DMGetServoData=0;
 DMSetExternalData=0;
 DMGetModelTime=0;

 DMGetState=0;
 DMNeiroPulse=0;
 DMNeiroPulseGroup=0;
 DMGetNeiroSumm=0;
 DMSetNeiroParam=0;
 DMSetWorkMode=0;

 DMAngleServo=0;
 DMDiffServo=0;
 DMHoldServo=0;
 DMFreeServo=0;
 DMSetMinPWM=0;

 FreeLibrary(ManipulatorDLL);
 ManipulatorDLL=0;

 return true;
}

// Инициализация манипулятора
bool NWPhysicalManipulator::InitManipulator(void)
{
 if(!ManipulatorDLL)
  return false;

 bool res=false;
 res=DMInit((void*)WindowHandle,EmulatorMode);
 res=DMComOpen(ComPort);
// res=DMReset();
 res=DMSetWorkMode(DllManipulatorMode);
// res=DMStart();
 return res;
}

// Деинициализация манипулятора
bool NWPhysicalManipulator::UnInitManipulator(void)
{
 if(!ManipulatorDLL)
  return true;

 DMStop();
 DMComClose();
 DMDeInit();
 return true;
}

// Шаг по накоплению в контроллере за 1мс
const int& NWPhysicalManipulator::GetAccumulationStep(void) const
{
 return AccumulationStep;
}

bool NWPhysicalManipulator::SetAccumulationStep(const int &value)
{
 AccumulationStep=value;
 // Отправка
 if(ManipulatorDLL && !EmulatorMode && DMSetNeiroParam)
 {
  DMSetNeiroParam(AccumulationStep,DissociationStep);
 }

 return true;
}

// Шаг по разряду в контроллере за 1мс
const int& NWPhysicalManipulator::GetDissociationStep(void) const
{
 return DissociationStep;
}

bool NWPhysicalManipulator::SetDissociationStep(const int &value)
{
 DissociationStep=value;
 // Отправка
 if(ManipulatorDLL && !EmulatorMode && DMSetNeiroParam)
 {
  DMSetNeiroParam(AccumulationStep,DissociationStep);
 }

 return true;
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NWPhysicalManipulator* NWPhysicalManipulator::New(void)
{
 return new NWPhysicalManipulator;
}
// --------------------------

// --------------------------
// Computation methods
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NWPhysicalManipulator::ADefault(void)
{
 //TimeStep=1000;
 MinControlVoltage=0;//150;

// Mode=0;

 // Шаг по накоплению в контроллере за 1мс
 AccumulationStep=100;

 // Шаг по разряду в контроллере за 1мс
 DissociationStep=5;

 NumAnglesHistory=2;
 AnglesHistoryTime=0.3;

 DllManipulatorMode=2;

 // Диапазоны по току (моменту)
 MinMoment=-0.7;
 MaxMoment=0.7;

 // Диапазоны по углу
 MinAngle=0;
 MaxAngle=190;

 //Дипазоны по входному напряжению
 MinVoltage=-5;
 MaxVoltage=5;


 OutputMul=2;
 ComPort=2;
 ServoNumber=1;

 MaxSendCounter=200;//20;
 MaxReadCounter=200;//50;

 TimeDuration=1;

 EmulatorMode=true;

 Input->Assign(1,1,0.0);
 Output1.Assign(1,1,0.0);
 Output2.Assign(1,1,0.0);
 Output3.Assign(1,1,0.0);

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NWPhysicalManipulator::ABuild(void)
{
 return true;
}

void NWPhysicalManipulator::AInit(void)
{
 InitManipulator();

 SendThreadTerminated=false;
 ReadThreadTerminated=false;
 SendThread=boost::thread(boost::bind(&NWPhysicalManipulator::SendCommand, boost::ref(*this)));
 ReadThread=boost::thread(boost::bind(&NWPhysicalManipulator::ReadData, boost::ref(*this)));
}

void NWPhysicalManipulator::AUnInit(void)
{
 SendThreadTerminated=true;
 SendThread.join();

 ReadThreadTerminated=true;
 ReadThread.join();

 UnInitManipulator();
 Sleep(1);
 UnLoadManipulatorDll();
 Sleep(100);
}

// Reset computation
bool NWPhysicalManipulator::AReset(void)
{
 Params.clear();
 FoundMinForce=10000;
 FoundMaxForce=-10000;

 FoundMinSpeed=10000;
 FoundMaxSpeed=-10000;


 SendCounter=MaxSendCounter;

 LastSentTime=GetCurrentStartupTime();
 LastReadTime=GetCurrentStartupTime();

 if(ManipulatorDLL && !EmulatorMode && DMSetWorkMode)
 {
  DMSetWorkMode(DllManipulatorMode);
 }

 return true;
}

// Execute math. computations of current object on current step
bool NWPhysicalManipulator::ACalculate(void)
{
 if(!ManipulatorDLL)
  return true;

 {
  boost::mutex::scoped_lock lock(ReadMutex);
  SafeOutput[0]=Output1(0,0);
  SafeOutput[1]=Output2(0,0);
  SafeOutput[2]=Output3(0,0);
 }

 {
  boost::mutex::scoped_lock lock(CommandMutex);
  SafeInput=(*Input)(0,0);
 }

 return true;
}

/// Отправляет камере команду на движение
void NWPhysicalManipulator::SendCommand(void)
{
 while (!SendThreadTerminated)
 {
  if(!ManipulatorDLL)
  {
   boost::this_thread::sleep(boost::posix_time::milliseconds(10));
   continue;
  }
  else
  {
   boost::mutex::scoped_lock lock(SendMutex);
   long long diff_time=CalcDiffTime(GetCurrentStartupTime(),LastSentTime);
   if(DllManipulatorMode == 2 /* командный режим */ || EmulatorMode)
   {
	if(diff_time>MaxSendCounter)
	{
	 boost::mutex::scoped_lock lock(CommandMutex);
	 MoveServoByCommand();
	 LastSentTime=GetCurrentStartupTime();
	}
	else
	 boost::this_thread::sleep(boost::posix_time::milliseconds(MaxSendCounter-int(diff_time)));
   }
/*   else
   if(DllManipulatorMode == 3) // Нейроуправление
   {
	if(NumInputs<1)
	 return true;

	PulseState.resize(NumInputs-1);
	for(size_t i=0;i<PulseState.size();i++)
	{
	 const UEPtr<const UItemData>& item=GetInputData(i+1);
	 if(item)
	 {
	  double curval=item->Double[0];

	  if(curval > 0 && !(i%2))
	   PulseState[i]+=1;
	  else
	  if(curval > 0 && (i%2))
	   PulseState[i]+=-1;
	 }
	}

	if(SendCounter == MaxSendCounter)
	{
		MoveServoByNeuro();
		SendCounter=0;
	}
	else
	 ++SendCounter;
   } */


//   long long diff_time=CalcDiffTime(GetCurrentStartupTime(),LastSentCommand);
//   if(diff_time>PtzInfo->Delay.Value)
//   {
//	ABasicSendCommand();
//	LastSentCommand=GetCurrentStartupTime();
//   }
//   else
  }
 }
}

void NWPhysicalManipulator::ABasicSendCommand(void)
{

}

void NWPhysicalManipulator::ReadData(void)
{
 while (!ReadThreadTerminated)
 {
  if(!ManipulatorDLL)
  {
   boost::this_thread::sleep(boost::posix_time::milliseconds(10));
   continue;
  }
  else
  {
   boost::mutex::scoped_lock lock(ReadMutex);
   long long diff_time=CalcDiffTime(GetCurrentStartupTime(),LastReadTime);

   if(diff_time>MaxReadCounter)
   {
	boost::mutex::scoped_lock lock(DataMutex);
	ReadManipulatorData();

	LastReadTime=GetCurrentStartupTime();
   }
   else
	boost::this_thread::sleep(boost::posix_time::milliseconds(MaxReadCounter-int(diff_time)));
  }
 }
}
// --------------------------


// --------------------------
// Вспомогательные методы
// --------------------------
// Производит считывание входных данных
bool NWPhysicalManipulator::ReadManipulatorData(void)
{
 unsigned char buffer[100];
 if(!DMGetServoData(ServoNumber,buffer))
 {
  SafeOutput[2]=0.001;
  SafeOutput[1]=0.001;
  SafeOutput[0]=0.001;
 }
//  memcpy(buffer,(void*)param.second,18);
  unsigned char N;
  unsigned char Dir;
  double speed;
  double angle;
  double force;
  memcpy(&N,&buffer[0],1);
  memcpy(&angle,&buffer[1],8);
  memcpy(&force,&buffer[9],8);
  memcpy(&Dir,&buffer[17],1);
  memcpy(&speed,&buffer[18],8);

  int n=N;
  if(n != ServoNumber)
  {
   return true;
  }

  if(Dir == 2)
  {
   //force=-force;
  }
  else
  if(Dir == 1)
  {
   force=-force;
  }
//  else
//   return false;

  if(FoundMinForce>force)
   FoundMinForce=force;
  if(FoundMaxForce<force)
   FoundMaxForce=force;

  MinMoment=-max(fabs(FoundMinForce),fabs(FoundMaxForce));
  MaxMoment=-MinMoment;


  angle=(angle*3.141)/180-3.14/2;
//  POutputData[1].Double[0]=((angle-MinAngle)*(3.14/2+3.14/2))/(MaxAngle-MinAngle)-3.14/2;  // Угол
  SafeOutput[1]=angle;//(angle*3.141)/180-3.14/2;  // Угол

  if(MaxMoment-MinMoment != 0)
   SafeOutput[0]=(((force-MinMoment)*(10+10))/(MaxMoment-MinMoment)-10);  // Момент

  PreviousAngles.push_back(SafeOutput[1]);
  AnglesTimes.push_back(GetTickCount());

  while((AnglesTimes.back()-AnglesTimes.front())/1000.0 >AnglesHistoryTime)
  {
   AnglesTimes.erase(AnglesTimes.begin());
   PreviousAngles.erase(PreviousAngles.begin());
  }

  if(PreviousAngles.size()<NumAnglesHistory)
   SafeOutput[2]=speed; // Угловая скорость
  else
  {
   if(!EmulatorMode)
   {
/*	if(PreviousAngles.size()>NumAnglesHistory)
	 PreviousAngles.erase(PreviousAngles.begin());

	if(AnglesTimes.size()>NumAnglesHistory)
	 AnglesTimes.erase(AnglesTimes.begin());
  */
	AngleSpeed=0;
	double moment_speed=0;
	for(size_t j=0;j<PreviousAngles.size()-1;j++)
	{
	 if(AnglesTimes[j+1]-AnglesTimes[j]>0)
	  moment_speed=(PreviousAngles[j+1]-PreviousAngles[j])/((AnglesTimes[j+1]-AnglesTimes[j])/1000.0);
	 else
	  moment_speed=0;
	 AngleSpeed+=moment_speed;
	}
	AngleSpeed/=(PreviousAngles.size()-1);
   }
   else
	AngleSpeed=speed;

   if(FoundMinSpeed>AngleSpeed)
	FoundMinSpeed=AngleSpeed;
   if(FoundMaxSpeed<AngleSpeed)
	FoundMaxSpeed=AngleSpeed;

   double minspeed=-max(fabs(FoundMinSpeed),fabs(FoundMaxSpeed));
   double maxspeed=-minspeed;
   // Масштабируем
   if(maxspeed-minspeed != 0)
	SafeOutput[2]=(((AngleSpeed-minspeed)*(2.0*M_PI+2.0*M_PI))/(maxspeed-minspeed)-2.0*M_PI);  // Момент

//   POutputData[2].Double[0]=AngleSpeed; // Угловая скорость
  }
 return true;
}


// Формирование управляющего сигнала манипулятором в командном режиме
bool NWPhysicalManipulator::MoveServoByCommand(void)
{
 bool res=true;
   CurrentTime=GetTickCount();

   LastDuration=CurrentTime-LastSentTime;
   double max_duration(2.0);
   int int_control_duration=int(((double(LastDuration)*255.0/1000.0))/max_duration);
   if(int_control_duration>255)
	int_control_duration=255;

//   if(NumInputs>0 && GetInputDataSize(0)>0)
   {
	InputVoltage=SafeInput;
	if(InputVoltage>=0)
	{
	 OutputVoltage=OutputMul*((InputVoltage-0)*(255-MinControlVoltage))/(100-0)+MinControlVoltage;
	 if(OutputVoltage>255)
	  OutputVoltage=255;
	 try {
  res&=DMMoveServo((unsigned char)ServoNumber,ServoRight,(unsigned char)OutputVoltage,/*TimeDuration*/int_control_duration);
	 }
	 catch(...)
	 {
	  return true;
	 }
	}
	else
	{
	 OutputVoltage=-OutputMul*((fabs(InputVoltage)-0)*(255-MinControlVoltage))/(100-0)+MinControlVoltage;
	 if(OutputVoltage<-255)
	  OutputVoltage=-255;
	 try {
  res&=DMMoveServo((unsigned char)ServoNumber,ServoLeft,(unsigned char)(-OutputVoltage),int_control_duration/*TimeDuration*/);
	 }
	 catch(...)
	 {
	  return true;
	 }
	}
   }
 return res;
}

// Формирование управляющего сигнала манипулятором в нейро режиме
bool NWPhysicalManipulator::MoveServoByNeuro(void)
{
 bool res=true;
   // Отправка
   for(int i=0;i<int(PulseState.size());i++)
   {
	if(PulseState[i] >= 1)
	{
	 res&=DMNeiroPulse(ServoNumber, i/2, 1);
	 Sleep(0);
	 PulseState[i]=0;
	}
	else
	if(PulseState[i] <= -1)
	{
	 res&=DMNeiroPulse(ServoNumber, i/2, 0);
	 Sleep(0);
	 PulseState[i]=0;
	}
   }
   /*
   unsigned char group=0;
   unsigned char table[8]={1,0,3,2,5,4,7,6};
//   unsigned char table[8]={0,1,2,3,4,5,6,7};
   for(size_t i=0;i<PulseState.size();i++)
   {
	if(PulseState[i] != 0)
	{
	 group+= (unsigned char)(1)<<table[i];
	 PulseState[i]=0;
	}
   }
   res=DMNeiroPulseGroup(ServoNumber, group);
	  */
 return res;
}
// --------------------------

/* *************************************************************************** */

}
#endif
