#include "NEngineMotionControl.h"
#include "../../Nmsdk-BasicLib/Core/NNet.h"
#include "../../Nmsdk-SourceLib/Core/NConstGenerator.h"
#include "../../Nmsdk-SourceLib/Core/NPulseGenerator.h"
#include "../../Nmsdk-SourceLib/Core/NManipulatorSource.h"
#include "../../Nmsdk-SourceLib/Core/NManipulatorSourceEmulator.h"
#include "../../Nmsdk-SensorLib/Core/NSignumSeparator.h"
#include "../../Nmsdk-SensorLib/Core/NIntervalSeparator.h"
#include "../../Nmsdk-PulseLib/Core/NPac.h"
#include "../../Nmsdk-PulseLib/Core/NReceptor.h"
#include "../../Nmsdk-ReceiverLib/Core/NManipulatorInputEmulator.h"
#include "../../Nmsdk-ReceiverLib/Core/NManipulatorInput.h"
#include "NMotionElement.h"

namespace NMSDK {

// --------------------------
// Конструкторы и деструкторы
// --------------------------
NEngineMotionControl::NEngineMotionControl(void)
 :
   NumControlLoops("NumControlLoops",this, &NEngineMotionControl::SetNumControlLoops),
   NumMotionElements("NumMotionElements",this,&NEngineMotionControl::SetNumMotionElements),
   CreationMode("CreationMode",this,&NEngineMotionControl::SetCreationMode),
   MotionElementClassName("MotionElementClassName",this),
   ObjectControlInterfaceClassName("ObjectControlInterfaceClassName",this,&NEngineMotionControl::SetObjectControlInterfaceClassName),
   AdaptiveStructureMode("AdaptiveStructureMode",this),
   InterneuronPresentMode("InterneuronPresentMode",this, &NEngineMotionControl::SetInterneuronPresentMode),
   LinkModes("LinkModes",this,&NEngineMotionControl::SetLinkModes),
   IaMin("IaMin",this),
   IaMax("IaMax",this),
   IbMin("IbMin",this),
   IbMax("IbMax",this),
   IIMin("IIMin",this),
   IIMax("IIMax",this),
   IcMin("IcMin",this),
   IcMax("IcMax",this),
   AfferentMin("AfferentMin",this, &NEngineMotionControl::SetAfferentMin),
   AfferentMax("AfferentMax",this, &NEngineMotionControl::SetAfferentMax),
   IntervalSeparatorMode("IntervalSeparatorMode",this, &NEngineMotionControl::SetIntervalSeparatorMode),
   PacGain("PacGain",this,&NEngineMotionControl::SetPacGain),
   PacSecretionTC("PacSecretionTC",this,&NEngineMotionControl::SetPacSecretionTC),
   PacDissociationTC("PacDissociationTC",this,&NEngineMotionControl::SetPacDissociationTC),
   AfferentRangeMode("AfferentRangeMode",this,&NEngineMotionControl::SetAfferentRangeMode),
   PacRangeMode("PacRangeMode",this,&NEngineMotionControl::SetPacRangeMode),

   MinAfferentRange("MinAfferentRange",this),

   CurrentContourAmplitude("CurrentContourAmplitude",this),
   CurrentContourAverage("CurrentContourAverage",this),
   CurrentTransientTime("CurrentTransientTime",this),
   InstantAvgSpeed("InstantAvgSpeed",this),
   CurrentTransientState("CurrentTransientState",this),
   DestContourMaxAmplitude("DestContourMaxAmplitude",this),
   DestContourMinAmplitude("DestContourMinAmplitude",this),
   DestTransientTime("DestTransientTime",this),
   ActiveContours("ActiveContours",this,&NEngineMotionControl::SetActiveContours),
   UseContourData("UseContourData",this),
   TransientHistoryTime("TransientHistoryTime",this),
   TransientObjectIndex("TransientObjectIndex",this),
   TransientAverageThreshold("TransientAverageThreshold",this),
   MCNeuroObjectName("MCNeuroObjectName",this,&NEngineMotionControl::SetMCNeuroObjectName),
   MCAfferentObjectName("MCAfferentObjectName",this,&NEngineMotionControl::SetMCAfferentObjectName),
   PacObjectName("PacObjectName",this,&NEngineMotionControl::SetPacObjectName),

   MaxContourAmplitude("MaxContourAmplitude",this),
   MotoneuronBranchMode("MotoneuronBranchMode",this,&NEngineMotionControl::SetMotoneuronBranchMode),
   RenshowMode("RenshowMode",this,&NEngineMotionControl::SetRenshowMode),
   Statistic("Statistic",this)


{
 LastAdaptiveTime=0.0;
 ControlMode=0;

 HistorySize=0;
 TransientHistorySize=0;
 TransientStartTime=0;
 OldTransientAverage=0;
 TempTransientState=0;
 InternalGeneratorDirection=-1;

}

NEngineMotionControl::~NEngineMotionControl(void)
{
}
// --------------------------

// --------------------------
// Методы управления параметрами
// --------------------------
// Число контуров управления
bool NEngineMotionControl::SetNumControlLoops(const int &value)
{
 if(value <=0)
  return false;

 if(ControlMode == 1)
 {
  for(size_t i=0; i<Motions.size(); i++)
  {
   //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
   NMotionElement *melem= Motions[i];
   melem->isNumControlLoopsInitialized=true;
   melem->NumControlLoops=value;
  }

  AfferentMin->resize(value,0);
  AfferentMax->resize(value,0);
  ActiveContours->resize(value,false);
  UseContourData->resize(value,false);
  CurrentContourAmplitude->resize(value,0);
  CurrentContourAverage->resize(value,0);
  MaxContourAmplitude->resize(value,0);

  DestContourMaxAmplitude->resize(value,0);
  DestContourMinAmplitude->resize(value,0);
  LinkModes->resize(value,1);
  Ready=false;
 }
 return true;
}

// Число управляющих элементов
bool NEngineMotionControl::SetNumMotionElements(const int &value)
{
// NumMotionElements.v=value;
// Create();
 if(Ready && AdaptiveStructureMode >0)
 {
  NumMotionElements.v=value;
  Create(false);
 }
 else
  Ready=false;

 return true;
}

// Режим формирования сети
bool NEngineMotionControl::SetCreationMode(const int &value)
{
 Ready=false;
 return true;
}

// Режим настройки диапазонов афферентных нейронов
bool NEngineMotionControl::SetAfferentRangeMode(const int &value)
{
 if(value < 0 || value > 3)
  return false;

if(Ready)
{

 bool crossranges=false;
 int real_ranges=0;

 real_ranges=CalcAfferentRange(NumMotionElements, crossranges, IaMin, IaMax,
			Ia_ranges_pos, Ia_ranges_neg,value);
 real_ranges=CalcAfferentRange(NumMotionElements, crossranges, IbMin, IbMax,
			Ib_ranges_pos, Ib_ranges_neg,value);
 real_ranges=CalcAfferentRange(NumMotionElements, crossranges, IIMin, IIMax,
			II_ranges_pos, II_ranges_neg,value);

 real_ranges=CalcAfferentRange(NumMotionElements, crossranges, IcMin, IcMax,
			Ic_ranges_pos, Ic_ranges_neg,value);

 AfferentRangesPos.resize(NumControlLoops);
 AfferentRangesNeg.resize(NumControlLoops);
 for(int i=0;i<NumControlLoops;i++)
 {
  if(int(AfferentMin->size())<=i)
  {
   LogMessageEx(RDK_EX_WARNING,__FUNCTION__,"AfferentMin size less than NumControlLoops");
   break;
  }
  if(int(AfferentMax->size())<=i)
  {
   LogMessageEx(RDK_EX_WARNING,__FUNCTION__,"AfferentMax size less than NumControlLoops");
   break;
  }
  real_ranges=CalcAfferentRange(NumMotionElements, crossranges, (*AfferentMin)[i], (*AfferentMax)[i],
			AfferentRangesPos[i], AfferentRangesNeg[i],value);
 }

 NewIntervalSeparatorsUpdate(IntervalSeparatorMode,6);
}
 return true;
}

// Режим настрйоки диапазонов pac
bool NEngineMotionControl::SetPacRangeMode(const int &value)
{
 if(value < 0 || value > 3)
  return false;

 PacRangeMode.v=value;
 if(Ready)
  SetupPacRange();

 return true;
}

// Процентная величина от соответствующей разницы *Max-*Min
bool NEngineMotionControl::SetMinAfferentRange(const double &value)
{
 if(value <=0 || value >1)
  return false;

 bool crossranges=false;
if(Ready)
{
 CalcAfferentRange(NumMotionElements, crossranges, IaMin, IaMax,
			Ia_ranges_pos, Ia_ranges_neg,AfferentRangeMode);
 CalcAfferentRange(NumMotionElements, crossranges, IbMin, IbMax,
			Ib_ranges_pos, Ib_ranges_neg,AfferentRangeMode);
 CalcAfferentRange(NumMotionElements, crossranges, IIMin, IIMax,
			II_ranges_pos, II_ranges_neg,AfferentRangeMode);

 CalcAfferentRange(NumMotionElements, crossranges, IcMin, IcMax,
            Ic_ranges_pos, Ic_ranges_neg,int(value));

 AfferentRangesPos.resize(NumControlLoops);
 AfferentRangesNeg.resize(NumControlLoops);
 for(int i=0;i<NumControlLoops;i++)
 {
     if(int(AfferentMin->size())<=i)
     {
      LogMessageEx(RDK_EX_WARNING,__FUNCTION__,"AfferentMin size less than NumControlLoops");
      break;
     }
     if(int(AfferentMax->size())<=i)
     {
      LogMessageEx(RDK_EX_WARNING,__FUNCTION__,"AfferentMax size less than NumControlLoops");
      break;
     }
  CalcAfferentRange(NumMotionElements, crossranges, (*AfferentMin)[i], (*AfferentMax)[i],
            AfferentRangesPos[i], AfferentRangesNeg[i],int(value));
 }

 NewIntervalSeparatorsUpdate(IntervalSeparatorMode,6);
}
 return true;
}

// Максимальное усиление управляющего воздействия
bool NEngineMotionControl::SetPacGain(const double &value)
{
 if(value <0)
  return false;

 PacGain.v=value;
 if(Ready)
  SetupPacRange();

 return true;
}

bool NEngineMotionControl::SetPacSecretionTC(const double &value)
{
 if(value <0)
  return false;

 PacSecretionTC.v=value;
 UpdatePacTCParameters();
	  /*
 if(Ready)
 {
  switch(ControlMode)
  {
  case 0:
   PACSetup(this, 1, PacSecretionTC, PacDissociationTC, PacGain,false);
   SetupPacRange();
  break;

  case 1:
   NewPACSetup(1, PacSecretionTC, PacDissociationTC, PacGain,false);
   SetupPacRange();
  break;
  }
 }          */

 return true;
}

bool NEngineMotionControl::SetPacDissociationTC(const double &value)
{
 if(value <0)
  return false;

 PacDissociationTC.v=value;
 UpdatePacTCParameters();
			 /*
 if(Ready)
 {
  switch(ControlMode)
  {
  case 0:
   PACSetup(this, 1, PacSecretionTC, PacDissociationTC, PacGain,false);
   SetupPacRange();
  break;

  case 1:
   NewPACSetup(1, PacSecretionTC, PacDissociationTC, PacGain,false);
   SetupPacRange();
  break;
  }
 }             */

 return true;
}

bool NEngineMotionControl::SetMCNeuroObjectName(const string &value)
{
 if(!Motions.empty())
 {
  for(int i=0;i<NumMotionElements;i++)
  {
   //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
   NMotionElement *melem=Motions[i];
   if(!melem)
	continue;
   melem->NeuroObjectName = value;
  }
 }
 Ready=false;
 return true;
}

bool NEngineMotionControl::SetMCAfferentObjectName(const string &value)
{
 if(!Motions.empty())
 {
  for(int i=0;i<NumMotionElements;i++)
  {
   //NNet* net=Motions[i];
   //NMotionElement *melem=dynamic_cast<NMotionElement *>(net);

   NMotionElement *melem = Motions[i];
   if(!melem)
    continue;
   melem->AfferentObjectName = value;
  }
 }
 Ready=false;
 return true;
}

bool NEngineMotionControl::SetPacObjectName(const string &value)
{
 Ready=false;
 return true;
}

bool NEngineMotionControl::SetObjectControlInterfaceClassName(const string &value)
{
 Ready=false;
 return true;
}

/// Диапазон афферентных нейронов по каналам
bool NEngineMotionControl::SetAfferentMin(const std::vector<double> &value)
{
 if(ControlMode == 0)
 {
  if(value.size()>0)
   IIMin=value[0];
  if(value.size()>1)
   IaMin=value[1];
  if(value.size()>2)
   IbMin=value[2];
  if(value.size()>3)
   IcMin=value[3];
 }

 if(Ready)
 {
  SetAfferentRangeMode(AfferentRangeMode.v);
/*  if(ControlMode == 0)
  {

  }
  else
  if(ControlMode == 1)
  {
   bool crossranges=false;
   AfferentRangesPos.resize(NumControlLoops);
   AfferentRangesNeg.resize(NumControlLoops);
   for(int i=0;i<NumControlLoops;i++)
	CalcAfferentRange(NumMotionElements, crossranges, value[i], (*AfferentMax)[i],
			AfferentRangesPos[i], AfferentRangesNeg[i],AfferentRangeMode);

   NewIntervalSeparatorsUpdate(IntervalSeparatorMode,6);
  }*/
 }

 return true;
}

bool NEngineMotionControl::SetAfferentMax(const std::vector<double> &value)
{
 if(ControlMode == 0)
 {
  if(value.size()>0)
   IIMax=value[0];
  if(value.size()>1)
   IaMax=value[1];
  if(value.size()>2)
   IbMax=value[2];
  if(value.size()>3)
   IcMax=value[3];
 }

 if(Ready)
 {
  SetAfferentRangeMode(AfferentRangeMode.v);
/*
  bool crossranges=false;
  AfferentRangesPos.resize(NumControlLoops);
  AfferentRangesNeg.resize(NumControlLoops);
  for(int i=0;i<NumControlLoops;i++)
   CalcAfferentRange(NumMotionElements, crossranges, (*AfferentMin)[i], value[i],
			AfferentRangesPos[i], AfferentRangesNeg[i],AfferentRangeMode);

  NewIntervalSeparatorsUpdate(IntervalSeparatorMode,6);
  */
 }

 return true;
}

bool NEngineMotionControl::SetIntervalSeparatorMode(const int &value)
{
 if(ControlMode == 1)
  Ready=false;
 return true;
}

bool NEngineMotionControl::SetMotoneuronBranchMode(const int &value)
{
 if(ControlMode == 1)
 {
  if(Ready && AdaptiveStructureMode >0)
  {
   MotoneuronBranchMode.v=value;
   Create(false);
  }
  else
   Ready=false;
 }
 return true;
}

bool NEngineMotionControl::SetRenshowMode(const int &value)
{
 if(ControlMode == 1)
 {
  if(Ready && AdaptiveStructureMode >0)
  {
   RenshowMode.v=value;
   Create(false);
  }
  else
   Ready=false;
 }
 return true;
}

/// Управление списком активных контуров
bool NEngineMotionControl::SetActiveContours(const std::vector<bool> &value)
{
 int num_controls=GetNumControlLoops();

 int count=(int(value.size())<num_controls)?int(value.size()):num_controls;

 for (int j = 0; j <count; j++)
 {
  SetIsAfferentLinked(j,value[j]);
 }

 return true;
}


/// Режим наличия интернейронов
bool NEngineMotionControl::SetInterneuronPresentMode(const int &value)
{
 for(size_t i=0; i<Motions.size(); i++)
 {
  //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
  NMotionElement *melem= Motions[i];
  if(!melem)
   continue;
  melem->InterneuronPresentMode=value;
//  if(value == 0)
//  {
//   std::vector<int> modes;
//   modes.assign(melem->NumControlLoops,0);
//   melem->LinkModes=modes;
//  }
//  else
//  if(value == 1)
//  {
//   std::vector<int> modes;
//   modes.assign(melem->NumControlLoops,1);
//   melem->LinkModes=modes;
//  }
 }

 Ready=false;
 return true;
}

bool NEngineMotionControl::SetLinkModes(const std::vector<int> &value)
{
 for(size_t i=0; i<Motions.size(); i++)
 {
  NMotionElement *melem = Motions[i];
  //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
  if(!melem)
   continue;
  melem->LinkModes=value;
 }

 Ready=false;
 return true;
}
// --------------------------



// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NEngineMotionControl* NEngineMotionControl::New(void)
{
 return new NEngineMotionControl;
}
// --------------------------

// --------------------------
// Proctected computation methods
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NEngineMotionControl::ADefault(void)
{
 NumMotionElements=1;
 NumControlLoops=1;
 MotoneuronBranchMode=0;
 IntervalSeparatorMode=6;
 RenshowMode=0;
 CreationMode=14;
 IaMin=-2.0*M_PI;
 IaMax=2.0*M_PI;
 IbMin=-1;
 IbMax=1;
 IIMin=-M_PI/2;
 IIMax=M_PI/2;
 IcMin=-10;
 IcMax=10;
 AfferentMin->assign(1,-M_PI/2);
 AfferentMax->assign(1,M_PI/2);
 PacGain=100;
 PacDissociationTC=0.001;
 PacSecretionTC=0.001;
 AfferentRangeMode=2;//0;
 PacRangeMode=2;//0;
 MinAfferentRange=0.1;
 MotionElementClassName="NNewMotionElement";
 ObjectControlInterfaceClassName="NControlObjectSource";
 AdaptiveStructureMode=2;
 InterneuronPresentMode=0;
 LinkModes->assign(1,1);

 DestTransientTime=0.1;
 TransientHistoryTime=1.0;
 TransientObjectIndex=0;
 TransientAverageThreshold=0.05;

 MCNeuroObjectName = "NNewSPNeuron";
 MCAfferentObjectName = "NSimpleAfferentNeuron";
 PacObjectName="NPac";

// StatisticDoubleMatrix.Default();
// StatisticDoubleMatrix.SetActivity(true);
// StatisticDoubleMatrix.Init();

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NEngineMotionControl::ABuild(void)
{
 if(!GetStorage())
  return false;

 if(AdaptiveStructureMode)
  Create(NumMotionElements);


 CurrentContourAmplitude->resize(NumControlLoops);
 CurrentContourAverage->resize(NumControlLoops);
 MaxContourAmplitude->resize(NumControlLoops);

 DestContourMaxAmplitude->resize(NumControlLoops);
 DestContourMinAmplitude->resize(NumControlLoops);
 UseContourData->resize(NumControlLoops);

// StatisticDoubleMatrix.Build();
 return true;
}


// Reset computation
bool NEngineMotionControl::AReset(void)
{
// StatisticDoubleMatrix.Reset();

 CurrentContourAmplitude->assign(NumControlLoops,0);
 CurrentContourAverage->assign(NumControlLoops,0);
 MaxContourAmplitude->assign(NumControlLoops,0);
 CurrentTransientTime=0;
 CurrentTransientState=false;
 TempTransientState=false;
 OldTransientAverage=0;
 InstantAvgSpeed=0;

// SetNumOutputs(6);
// for(int i=0;i<NumOutputs;i++)
//  SetOutputDataSize(i,MMatrixSize(1,1));

 receptors.resize(NumMotionElements);
 for(int n=0;n<NumMotionElements;n++)
 {
  receptors[n].resize(6);
  UContainer* cont=GetComponent(string("MotionElement")+RDK::sntoa(n),true);
  if(!cont)
   continue;

/*  try {
   receptors[n][4]=static_pointer_cast<NReceptor>(cont->GetComponentL("Afferent_II1.Receptor"));
   receptors[n][5]=static_pointer_cast<NReceptor>(cont->GetComponentL("Afferent_II2.Receptor"));
   receptors[n][0]=static_pointer_cast<NReceptor>(cont->GetComponentL("Afferent_Ia1.Receptor"));
   receptors[n][1]=static_pointer_cast<NReceptor>(cont->GetComponentL("Afferent_Ia2.Receptor"));
   receptors[n][2]=static_pointer_cast<NReceptor>(cont->GetComponentL("Afferent_Ib1.Receptor"));
   receptors[n][3]=static_pointer_cast<NReceptor>(cont->GetComponentL("Afferent_Ib2.Receptor"));
  }
  catch (EComponentNameNotExist &exc)
  {
   continue;
  }*/

//  LastAdaptiveTime=0.0;
/*
  try {
   cont->GetComponentL("Afferent_Ic1.Receptor");
   receptors[n].resize(8);

   receptors[n][6]=static_pointer_cast<NReceptor>(cont->GetComponentL("Afferent_Ic1.Receptor"));
   receptors[n][7]=static_pointer_cast<NReceptor>(cont->GetComponentL("Afferent_Ic2.Receptor"));
  }
  catch (EComponentNameNotExist &exc)
  {
   continue;
  }     */

  Statistic->Assign(1,11+NumControlLoops*2,0.0);
//  StatisticDoubleMatrix.Calculate();
 }



 return true;
}



// Execute math. computations of current object on current step
bool NEngineMotionControl::ACalculate(void)
{
 CreateLink("","Statistic","StatisticDoubleMatrix","InputMatrixData");
 // Считаем статистику
 // за ожидаемое время переходного процесса
 CurrentContourAmplitude->assign(NumControlLoops,0);
 CurrentContourAverage->assign(NumControlLoops,0);

 HistorySize=int(TransientHistoryTime*TimeStep);
 vector<double> measure;

 measure.resize(NumControlLoops);
 UEPtr<UNet> source;
 for(int i=0;i<NumControlLoops;i++)
 {
  source=dynamic_pointer_cast<UNet>(GetComponent("NManipulatorSource1"/*+sntoa(i+1)*/));
//  if(source->GetOutputDataSize(0)>MMatrixSize(1,0))
//   measure[i]=source->GetOutputData(0).Double[i];
//  else
   measure[i]=0;
 }

 History.push_back(measure);
 if(int(History.size())>HistorySize)
  History.erase(History.begin());

 double min_val, max_val,avg_val;
 for(size_t i=0;i<measure.size();i++)
 {
  min_val=10000;
  max_val=-10000;
  avg_val=0;
  for(size_t j=0;j<History.size();j++)
  {
   if(min_val>History[j][i])
	min_val=History[j][i];
   if(max_val<History[j][i])
	max_val=History[j][i];
   avg_val+=History[j][i];
  }
  if(CurrentContourAmplitude->size()>i)
  {
   (*CurrentContourAmplitude)[i]=max_val-min_val;
   (*CurrentContourAverage)[i]=avg_val/History.size();
   double max_min=max_val-min_val;
   if(((*MaxContourAmplitude)[i])<max_min)
    (*MaxContourAmplitude)[i]=max_min;
  }
 }

// bool old_transient_state=CurrentTransientState;
 InstantAvgSpeed=((*CurrentContourAverage)[TransientObjectIndex]-OldTransientAverage)*TimeStep;
 OldTransientAverage=(*CurrentContourAverage)[TransientObjectIndex];
// double transient_precentage(0.0);
// if((*CurrentContourAverage)[TransientObjectIndex]>1e-5)
// {
//  transient_precentage=delta/(*CurrentContourAverage)[TransientObjectIndex];
// }
 if(fabs(InstantAvgSpeed)>=TransientAverageThreshold && !CurrentTransientState)
 {
  CurrentTransientState=true;
  TempTransientState=true;
  TransientStartTime=Environment->GetTime().GetDoubleTime();
 }
 else
 if(fabs(InstantAvgSpeed)<TransientAverageThreshold && CurrentTransientState)
 {
  if(TempTransientState)
  {
   CurrentTransientTime=Environment->GetTime().GetDoubleTime()-TransientStartTime;
  }
  TempTransientState=false;
//  if(Environment->GetTime().GetDoubleTime()-TransientStartTime>TransientHistoryTime*3.0)
   CurrentTransientState=false;
 }

	/*
 TransientHistorySize=TransientHistoryTime*TimeStep;
 if(TransientObjectIndex>=0 && TransientObjectIndex<measure.size())
 {
  TransientHistory.push_back((*CurrentContourAverage)[TransientObjectIndex]);
  if(TransientHistory.size()>TransientHistorySize)
   TransientHistory.erase(TransientHistory.begin());
  min_val=10000;
  max_val=-10000;
  double delta=10000;
//  CurrentTransientTime=TransientHistoryTime;
  bool old_transient_state=CurrentTransientState;
  for(size_t j=1;j<TransientHistory.size()-1;j++)
  {
   delta=(TransientHistory[j]-TransientHistory[j-1])*TimeStep;
   if(fabs(delta)<TransientAverageThreshold)
   {
	CurrentTransientState=false;

	if(old_transient_state)
	{
	 CurrentTransientTime=GetDoubleTime()-TransientStartTime;
    }
	break;
   }
   else
   {
	if(!CurrentTransientState)
	{
	 TransientStartTime=GetDoubleTime();
	}
	CurrentTransientState=true;
   }
  }

 }     */

 if(AdaptiveStructureMode == 2)
  AdaptiveTuning();

// int pos_angle=0,pos_speed=0,pos_moment=0;
// int neg_angle=0,neg_speed=0,neg_moment=0;
 // Receptors
 for(int i=0;i<NumMotionElements;i++)
 {
  try {
/*  if(receptors[i][0] && receptors[i][0]->GetInputData(size_t(0))->Double[0] > 0)
   ++pos_speed;
  if(receptors[i][1] && receptors[i][1]->GetInputData(size_t(0))->Double[0] > 0)
   ++neg_speed;

  if(receptors[i][2] && receptors[i][2]->GetInputData(size_t(0))->Double[0] > 0)
   ++pos_moment;
  if(receptors[i][3] &&receptors[i][3]->GetInputData(size_t(0))->Double[0] > 0)
   ++neg_moment;

  if(receptors[i][4] && receptors[i][4]->GetInputData(size_t(0)) && receptors[i][4]->GetInputData(size_t(0))->Double[0] > 0)
   ++pos_angle;
  if(receptors[i][5] && receptors[i][5]->GetInputData(size_t(0)) && receptors[i][5]->GetInputData(size_t(0))->Double[0] > 0)
   ++neg_angle;*/
  }
  catch (UEPtr<NReceptor>::EUsingZeroPtr &)
  {
   continue;
  }

 }

// POutputData[0].Double[0]=pos_angle;
// POutputData[1].Double[0]=neg_angle;
// POutputData[2].Double[0]=pos_speed;
// POutputData[3].Double[0]=neg_speed;
// POutputData[4].Double[0]=pos_moment;
// POutputData[5].Double[0]=neg_moment;

 std::vector<double> stats;

 stats.push_back(NumMotionElements);
 stats.push_back(NumControlLoops);
 stats.push_back(AfferentRangeMode);
 stats.push_back(PacRangeMode);
 stats.push_back(PacGain);
 stats.push_back(TransientHistoryTime);
 stats.push_back(TransientAverageThreshold);
 stats.push_back(TransientAverageThreshold);
 for(int i=0;i<NumControlLoops;i++)
  stats.push_back((*CurrentContourAmplitude)[i]);
 for(int i=0;i<NumControlLoops;i++)
  stats.push_back((*CurrentContourAverage)[i]);
 stats.push_back(CurrentTransientTime);
 stats.push_back(CurrentTransientTime);
 stats.push_back(CurrentTransientState);

 Statistic->Assign(1,int(stats.size()),0.0);
 for(size_t i=0;i<stats.size();i++)
  (*Statistic)(0,int(i))=stats[i];


 return true;
}
// --------------------------

// --------------------------
// Методы управления счетом
// --------------------------
// Создает объект с желаемой структурой в соответствии с CreationMode
// Если full_recreate == true удаляет все существующие управляющие элементы
// Иначе стремится их сохранить
bool NEngineMotionControl::Create(bool full_recreate)
{
 // Удаляем все кроме интерфейсных компонент
 if(full_recreate)
  ClearStructure(0);
 else
  ClearStructure(NumMotionElements);

 //DelAllComponents();
 Motions.clear();
 receptors.clear();
 ControlMode=0;
 switch(CreationMode)
 {
 case 1:
  CreateNewEngineControl2NeuronsSimplest(false,true);
 break;

 case 2:
  CreateNewEngineControl2NeuronsSimplest(false,true);
 break;

 case 3:
  CreateNewEngineControl2NeuronsSimplest(true,true);
 break;

 case 4:
  CreateNewEngineControl2NeuronsSimplest(false,false);
 break;

 case 5:
  CreateNewEngineControl2NeuronsSimplest(true,false);
 break;

 case 10:
  CreateNewEngineControl2NeuronsSimplest();
 break;

 case 14://Этот режим
  CreateNewEngineControl2NeuronsSimplest();
 break;
 };

 SetupPacRange();

 SetActiveContours(*ActiveContours);


 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);
 UEPtr<UStatisticMatrix<double> > stats=AddMissingComponent<UStatisticMatrix<double> >("StatisticDoubleMatrix", "UStatisticDoubleMatrix");
 stats->ManualModeEnabled=true;
 return true;
}

/// Удаляет существующую структуру, не трогая вспомогательные компоненты
/// Сохраняет expected_num_motion_elements число управляющих элементов
bool NEngineMotionControl::ClearStructure(int expected_num_motion_elements)
{
 int i=0;
 int found_motion_elements=0;
 while(i<GetNumComponents())
 {
  if(PComponents[i]->GetName() == "IIPosAfferentGenerator" ||
	 PComponents[i]->GetName() == "IINegAfferentGenerator" ||
	 PComponents[i]->GetName() == "AfferentSource1" ||
	 (PComponents[i]->GetName() == "NManipulatorSource1" && PComponents[i]->GetCompClassName() == *ObjectControlInterfaceClassName) ||
	 PComponents[i]->GetName() == "NManipulatorInput1" ||
	 PComponents[i]->GetName() == "StatisticDoubleMatrix")// ||
//	 PComponents[i]->GetName() == "Pac")
  {
   ++i;
   continue;
  }
  else
  {
   if(PComponents[i]->GetName().substr(0,std::string("MotionElement").size())
		== "MotionElement" && found_motion_elements<expected_num_motion_elements)
	++found_motion_elements;
   else
	DelComponent(PComponents[i],true);
  }
 }
 return true;
}


/// Алгоритм адаптивной настройки
void NEngineMotionControl::AdaptiveTuning(void)
{
 std::vector<double> &current_contour_amplitude=*CurrentContourAmplitude;
 std::vector<bool> &use_contour_data=*UseContourData;
 double &current_transient_time=*CurrentTransientTime;
 std::vector<double> &dest_contour_max_amplitude=*DestContourMaxAmplitude;
 std::vector<double> &dest_contour_min_amplitude=*DestContourMinAmplitude;
 double &dest_transient_time=*DestTransientTime;

 int num_motion_elements=NumMotionElements;
 double control_grain=PacGain;

 AdaptiveTuningSimple(current_contour_amplitude,
		use_contour_data, current_transient_time, dest_contour_max_amplitude,
		dest_contour_min_amplitude, dest_transient_time, num_motion_elements, control_grain);

 // Применяем настройки регулятора
 NumMotionElements=num_motion_elements;
 PacGain=control_grain;
}

/// Реализация алгоритмов адаптивной настройки
/// current_contour_amplitude - текущая амплитуда по контурам управления
/// use_contour_data - флаги, определающие данные каких контуров можно использовать
/// current_transient_time - текущее время переходного процесса
/// dest_contour_amplitude - желаемая амплитуда по контурам управления
/// dest_transient_time - желаемое время переходного процесса
/// num_motion_elements - расчетное число управляющих элементов
/// control_gain - расчетное усиление сигнала управления
///
void NEngineMotionControl::AdaptiveTuningSimple(const std::vector<double> &current_contour_amplitude,
								  const std::vector<bool> &use_contour_data,
								  double current_transient_time,
								  const std::vector<double> &dest_contour_max_amplitude,
								  const std::vector<double> &dest_contour_min_amplitude,
								  double dest_transient_time,
								  int &num_motion_elements,
								  double &control_gain)
{
 if(CurrentTransientState)
  return;

 double current_double_time=Environment->GetTime().GetDoubleTime();
 double last_adaptive_time=LastAdaptiveTime;
 double transient_hist_time=*TransientHistoryTime;
 if(LastAdaptiveTime>0 && current_double_time-last_adaptive_time<transient_hist_time)
  return;

 LastAdaptiveTime=Environment->GetTime().GetDoubleTime();

 for(size_t i=0;i<use_contour_data.size();i++)
 {
  if(use_contour_data[i])
  {
   if(current_contour_amplitude[i]>dest_contour_max_amplitude[i])
   {
	++num_motion_elements;
	break; // Заглушка
   }
   else
   if(current_contour_amplitude[i]<dest_contour_min_amplitude[i])
   {
	if(num_motion_elements>1)
	 --num_motion_elements;
    break; // Заглушка
   }

  }
 }
}
// --------------------------

// --------------------------
// Вспомогательные методы
// --------------------------
// Вычисляет диапазоны действия афферентнов
int NEngineMotionControl::CalcAfferentRange(int num_motions, bool cross_ranges, double a_min, double a_max,
			vector<pair<double,double> > &pos_ranges, vector<pair<double,double> > &neg_ranges, int range_mode)
{
 int real_ranges=0;
 if(cross_ranges)
  real_ranges=num_motions/2+1;
 else
  real_ranges=num_motions;

 pos_ranges.resize(num_motions);
 neg_ranges.resize(num_motions);

 int rr_index=0;

 if(cross_ranges)
 {
 for(int i=0;i<num_motions;i++)
 {
  if(!(i % 2)) // четное
  {
   double left_range=a_min,right_range=a_max;
   double pos_range=(right_range-0)/real_ranges;
   double neg_range=(0-left_range)/real_ranges;

   pos_ranges[i].first=rr_index*pos_range;
   pos_ranges[i].second=(rr_index+1)*pos_range;
   neg_ranges[i].first=left_range+rr_index*neg_range;
   neg_ranges[i].second=left_range+(rr_index+1)*neg_range;
  }
  else // нечетное
  {
   double left_range=a_min,right_range=a_max;
   double pos_range=(right_range-0)/real_ranges;
   double neg_range=(0-left_range)/real_ranges;

   pos_ranges[i].first=(rr_index+0.5)*pos_range;
   pos_ranges[i].second=(rr_index+1.5)*pos_range;
   neg_ranges[i].first=left_range+(rr_index+0.5)*neg_range;
   neg_ranges[i].second=left_range+(rr_index+1.5)*neg_range;

   ++rr_index;
  }
 }
 }
 else
 {
  if(range_mode == 0)
  {
   for(int i=0;i<num_motions;i++)
   {
	double left_range=a_min,right_range=a_max;
	double pos_range=(right_range-0)/real_ranges;
	double neg_range=(0-left_range)/real_ranges;

	pos_ranges[i].first=rr_index*pos_range;
	pos_ranges[i].second=(rr_index+1)*pos_range;
	neg_ranges[num_motions-i-1].first=left_range+rr_index*neg_range;
	neg_ranges[num_motions-i-1].second=left_range+(rr_index+1)*neg_range;
	++rr_index;
   }
  }
  else
  if(range_mode == 1)
  {
   MinAfferentRange=1./real_ranges;
   double left_range=a_min*MinAfferentRange/real_ranges,right_range=a_max*MinAfferentRange/real_ranges;
   double pos_range=(right_range-0)*(1);
   double neg_range=(left_range-0)*(1);
   for(int i=0;i<num_motions;i++)
   {
	if(i==0)
	 pos_ranges[i].first=rr_index*pos_range;
	else
	 pos_ranges[i].first=pos_ranges[i-1].second;

	pos_range=(right_range-0)*(i+1);

	pos_ranges[i].second=(rr_index+1)*pos_range;
	++rr_index;
   }

   rr_index=0;
   neg_range=(left_range-0)*(rr_index+1);
   for(int i=0;i<num_motions;i++)
   {
	if(rr_index==0)
	 neg_ranges[i].second=rr_index*neg_range;
	else
	 neg_ranges[i].second=neg_ranges[i-1].first;

	neg_range=(left_range-0)*(i+1);

	neg_ranges[i].first=(rr_index+1)*neg_range;
	++rr_index;
   }
  }
  else
  if(range_mode == 2)
  {
   for(int i=0;i<num_motions;i++)
   {
	if(i==0)
	 pos_ranges[i].first=0;
	else
	 pos_ranges[i].first=pos_ranges[i-1].second;

	pos_ranges[i].second=a_max/powl(2.0,num_motions-i-1);
	++rr_index;
   }

   rr_index=0;
   for(int i=0;i<num_motions;i++)
   {
	if(rr_index==0)
	 neg_ranges[i].second=0;
	else
	 neg_ranges[i].second=neg_ranges[i-1].first;

	neg_ranges[i].first=a_min/powl(2.0,num_motions-i-1);
	++rr_index;
   }
  }
  else
  if(range_mode == 3)
  {
   for(int i=0;i<num_motions;i++)
   {
	if(i==0)
	 pos_ranges[i].first=0;
	else
	 pos_ranges[i].first=pos_ranges[i-1].second;

	pos_ranges[i].second=a_max/(num_motions-i);
	++rr_index;
   }

   rr_index=0;
   for(int i=0;i<num_motions;i++)
   {
	if(rr_index==0)
	 neg_ranges[i].second=0;
	else
	 neg_ranges[i].second=neg_ranges[i-1].first;

	neg_ranges[i].first=a_min/(num_motions-i);
	++rr_index;
   }
  }
 }
 return real_ranges;
}

void NEngineMotionControl::SetupPacRange(void)
{
 if(NumMotionElements <= 0)
  return;
 UEPtr<NPac> cont;
  cont=dynamic_pointer_cast<NPac>(GetComponentL("Pac",true));

 if(!cont)
  return;

 double a_min=-PacGain;
 double a_max=PacGain;
 int num_motions=NumMotionElements;
 vector<vector<double> > values=cont->Gain;

 if(num_motions*2 != int(values.size()))
  return; // TODO: Это аномалия, которую нужно разрешить

// values.resize(num_motions*2);
 int rr_index=0;
 if(PacRangeMode == 0)
 {
   for(int i=0;i<num_motions;i++)
   {
	double left_range=a_min,right_range=a_max;
	double pos_range=(right_range-0)/NumMotionElements;
	double neg_range=(0-left_range)/NumMotionElements;

	values[i+values.size()/2].assign(1,(rr_index+1)*pos_range);
//	pos_ranges[i].second=(rr_index+1)*pos_range;
	values[values.size()/2-i-1].assign(1,left_range+rr_index*neg_range);
//	neg_ranges[num_motions-i-1].second=left_range+(rr_index+1)*neg_range;
	++rr_index;
   }
 }
 else
 if(PacRangeMode == 1)
 {
   double min_afferent_range=1./NumMotionElements;
   double left_range=a_min*min_afferent_range/NumMotionElements,right_range=a_max*min_afferent_range/NumMotionElements;
   double pos_range=(right_range-0)*(1);
   double neg_range=(left_range-0)*(1);
   for(int i=0;i<num_motions;i++)
   {
	pos_range=(right_range-0)*(i+1);
	values[i+values.size()/2].assign(1,(rr_index+1)*pos_range);
	++rr_index;
   }

   rr_index=0;
   neg_range=(left_range-0)*(rr_index+1);
   for(int i=0;i<num_motions;i++)
   {
	neg_range=(left_range-0)*(i+1);
	values[i].assign(1,(rr_index+1)*neg_range);
	++rr_index;
   }
 }
 else
 if(PacRangeMode == 2)
 {
   for(int i=0;i<num_motions;i++)
   {
	values[i+values.size()/2].assign(1,a_max/powl(2.0,num_motions-i-1));
	++rr_index;
   }

   rr_index=0;
   for(int i=0;i<num_motions;i++)
   {
	values[i].assign(1,a_min/powl(2.0,num_motions-i-1));
	++rr_index;
   }
 }
 else
 if(PacRangeMode == 3)
 {
   for(int i=0;i<num_motions;i++)
   {
	values[i+values.size()/2].assign(1,a_max/(num_motions-i));
	++rr_index;
   }

   rr_index=0;
   for(int i=0;i<num_motions;i++)
   {
	values[i].assign(1,a_min/(num_motions-i));
	++rr_index;
   }
 }
 cont->Gain=values;
}

// Настройка преобразователя аналог-аналог
void NEngineMotionControl::AACSetup(UEPtr<UContainer> net, double gain_value)
{
 NPac* cont=0;
 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);

 if(!storage)
  return;

 cont=dynamic_pointer_cast<NPac>(storage->TakeObject(PacObjectName));
 if(!cont)
  return;
 cont->SetName("Pac");
 cont->Mode=0;
 cont->TCMode=0;
 net->AddComponent(cont);
// cont->SetOutputDataSize(0,MMatrixSize(1,int(Motions.size())*2));

 // Начальные значения всем параметрам
 // Амплитуда входных импульсов
 vector<vector<double> > values;
 values.resize(Motions.size()*2);

 // Усиление
 for(size_t i=0;i<values.size()/2;i++)
 {
  values[i].assign(1,gain_value/Motions.size());
 }

 for(size_t i=values.size()/2;i<values.size();i++)
 {
  values[i].assign(1,-gain_value/Motions.size());
 }

 cont->Gain=values;
}



// Задание вспомогательных компонент
void NEngineMotionControl::AdditionalComponentsSetup(UEPtr<UContainer> net)
{
 UContainer* cont=0;
 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);
 bool res=true;

 if(!storage)
  return;

 if(CheckName("IIPosAfferentGenerator"))
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPGenerator"));
  if(!cont)
   return;
  cont->SetName("IIPosAfferentGenerator");
  ((NPulseGenerator*)cont)->Amplitude=1;
  ((NPulseGenerator*)cont)->Frequency=0;
  res=net->AddComponent(cont);
 }

 if(CheckName("IINegAfferentGenerator"))
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPGenerator"));
  if(!cont)
   return;
  cont->SetName("IINegAfferentGenerator");
  ((NPulseGenerator*)cont)->Amplitude=1;
  ((NPulseGenerator*)cont)->Frequency=0;
  res=net->AddComponent(cont);
 }

 if(CheckName("AfferentSource1"))
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NCGenerator"));
  if(!cont)
   return;
  cont->SetName("AfferentSource1");
  ((NConstGenerator*)cont)->Amplitude=0;
  res=net->AddComponent(cont);
 }

 //for(int i=0; i<NumControlLoops; ++i)
 {
  std::string name="NManipulatorSource1";
  if(CheckName(name))
  {
   UEPtr<UItem> cont2=0;
   cont2=dynamic_pointer_cast<UItem>(storage->TakeObject(ObjectControlInterfaceClassName));
   if(!cont2)
	return;
   cont2->SetName(name);
   res=net->AddComponent(cont2);
  }
 }

 if(CheckName("NManipulatorInput1"))
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NManipulatorInput"));
  if(!cont)
   return;
  cont->SetName("NManipulatorInput1");
  res=net->AddComponent(cont);
 }

 if(res)
  return;
}

// Формируем сеть управления новым способом на 2 импульсных нейронах
UNet* NEngineMotionControl::CreateNewEngineControl2NeuronsSimplest(bool crosslinks, bool crossranges)
{
 ControlMode=1; //новый режим, в котором число контуров может динамически изменяться

// // Отключаем ненужные параметры и состояния
// ChangeLookupPropertyType("IaMin",ptParameter);
// ChangeLookupPropertyType("IaMax",ptParameter);
//
// // Диапазон афферентных нейронов по каналу Ib
// ChangeLookupPropertyType("IbMin",ptParameter);
// ChangeLookupPropertyType("IbMax",ptParameter);
//
// // Диапазон афферентных нейронов по каналу II
// ChangeLookupPropertyType("IIMin",ptParameter);
// ChangeLookupPropertyType("IIMax",ptParameter);
//
// // Диапазон афферентных нейронов по каналу Ic
// ChangeLookupPropertyType("IcMin",ptParameter);
// ChangeLookupPropertyType("IcMax",ptParameter);

 ActiveContours->resize(4,0);

 ChangeLookupPropertyType("MCNeuroObjectName",ptPubParameter);
 ChangeLookupPropertyType("MCAfferentObjectName",ptPubParameter);

 ChangeLookupPropertyType("AfferentMin",ptPubParameter);
 ChangeLookupPropertyType("AfferentMax",ptPubParameter);

 ChangeLookupPropertyType("NumControlLoops", ptPubParameter);

 bool res(false);
 UEPtr<UStorage> storage=static_pointer_cast<UStorage>(Storage);
 size_t num_motions=NumMotionElements;

 // Число неперекрывающихся диапазонов
 int real_ranges=0;
// bool crossranges=false;
// bool crosslinks=false;


 real_ranges=CalcAfferentRange(int(num_motions), crossranges, IaMin, IaMax,
			Ia_ranges_pos, Ia_ranges_neg,AfferentRangeMode);
 real_ranges=CalcAfferentRange(int(num_motions), crossranges, IbMin, IbMax,
			Ib_ranges_pos, Ib_ranges_neg,AfferentRangeMode);
 real_ranges=CalcAfferentRange(int(num_motions), crossranges, IIMin, IIMax,
			II_ranges_pos, II_ranges_neg,AfferentRangeMode);

 AfferentRangesPos.resize(NumControlLoops);
 AfferentRangesNeg.resize(NumControlLoops);
 for(int i=0;i<NumControlLoops;i++)
 {
     if(int(AfferentMin->size())<=i)
     {
      LogMessageEx(RDK_EX_WARNING,__FUNCTION__,"AfferentMin size less than NumControlLoops");
      break;
     }
     if(int(AfferentMax->size())<=i)
     {
      LogMessageEx(RDK_EX_WARNING,__FUNCTION__,"AfferentMax size less than NumControlLoops");
      break;
     }
  real_ranges=CalcAfferentRange(NumMotionElements, crossranges, (*AfferentMin)[i], (*AfferentMax)[i],
			AfferentRangesPos[i], AfferentRangesNeg[i],AfferentRangeMode);
 }



 UNet *net=this;//dynamic_cast<UNet*>(storage->TakeObject(netclassname));
// net = dynamic_cast<NMotionElement*>(this);
 if(!net)
  return 0;

 int outmode=0;
 int inpmode=0;

 double exp_coeff=0.0001;//0.00001;

 // Двигательные элементы
 NewMotionElementsSetup(net, inpmode, outmode, exp_coeff, PacGain, 1, real_ranges);

 AdditionalComponentsSetup(net);
 if(CreationMode == 10)
 {
  AACSetup(net, 100);
 }
 else
 {
  NewPACSetup(1, PacSecretionTC, PacDissociationTC, PacGain,false);
 }
 NewIntervalSeparatorsSetup(IntervalSeparatorMode, 6, 1, -1);

 // Установка связей
 ULongId item,conn;

 NewStandardLinksSetup("Pac");

 NewIntervalSeparatorLinksSetup();

 if(crosslinks)
 {
  for(size_t k=1;k<Motions.size()-1;k++)
  {
   res=net->CreateLink(Motions[k]->GetName()+".RenshowL.LTZone","Output",Motions[k-1]->GetName()+".MotoneuronL.Soma1.InhSynapse1","Input");
   res=net->CreateLink(Motions[k]->GetName()+".RenshowL.LTZone","Output",Motions[k+1]->GetName()+".MotoneuronL.Soma1.InhSynapse1","Input");
   res=net->CreateLink(Motions[k]->GetName()+".RenshowR.LTZone","Output",Motions[k-1]->GetName()+".MotoneuronR.Soma1.InhSynapse1","Input");
   res=net->CreateLink(Motions[k]->GetName()+".RenshowR.LTZone","Output",Motions[k+1]->GetName()+".MotoneuronR.Soma1.InhSynapse1","Input");
  }
 }
 if(!res)
  return 0;

 return net;
}

// Настройка рецепторов
void NEngineMotionControl::NewMotionElementsSetup(UEPtr<UContainer> net, int inp_mode, int out_mode, double exp_coeff, double receptor_max_output, double receptor_gain, int real_ranges)
{
 UEPtr<UContainer> cont;
 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);
 Motions.clear();

 if(!storage)
  return;

 InternalGenerator=0;
 try
 {
  InternalGenerator=dynamic_pointer_cast<NPulseGenerator>(storage->TakeObject("NPGenerator"));
  InternalGenerator->SetName("InternalGenerator");
  net->AddComponent(InternalGenerator);
 }
 catch (EComponentNameNotExist &)
 {
 }


 for(int i=0;i<NumMotionElements;i++)
 {
  UEPtr<NReceptor> receptor=0;
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(MotionElementClassName));
  if(!cont)
   continue;
  cont->SetName(string("MotionElement")+RDK::sntoa(i));
  net->AddComponent(cont);
  Motions.push_back(dynamic_pointer_cast<NMotionElement>(cont));
  //Motions.push_back(static_pointer_cast<NNet>(cont));

  UEPtr<NMotionElement> melement=dynamic_pointer_cast<NMotionElement>(cont);
  if(melement)
  {
   melement->NeuroObjectName = MCNeuroObjectName;
   melement->AfferentObjectName = MCAfferentObjectName;
   melement->NumControlLoops=NumControlLoops;
   melement->InterneuronPresentMode=InterneuronPresentMode;
   melement->MotoneuronBranchMode=MotoneuronBranchMode;

   melement->RenshowMode=!RenshowMode; // TODO: Заглушка, чтобы обойти невозможность выполнить Build, т.к. Ready в melement уже взведен.
   melement->RenshowMode=RenshowMode;

   if(InterneuronPresentMode == 0)
   {
	std::vector<int> modes;
	modes.assign(melement->NumControlLoops,0);
	melement->LinkModes=modes;
   }
   else
   {
	std::vector<int> modes;
	modes.assign(melement->NumControlLoops,1);
	melement->LinkModes=modes;
   }

  //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
//  melement->SetNumControlLoops(2);
   melement->Build();
	for (int j = 0; j < melement->NumControlLoops; j++)
	{
	  try
	  {
	   receptor=dynamic_pointer_cast<NReceptor>(Motions[i]->GetComponentL("AfferentL"+RDK::sntoa(j+1)+".Receptor"));
	   receptor->MinInputRange=0;
	   receptor->MaxInputRange=fabs(IaMin)/real_ranges;
//	   receptor->InputAdaptationMode=inp_mode;
//	   receptor->OutputAdaptationMode=out_mode;
//	   receptor->MaxOutputRange=receptor_max_output;
//	   receptor->Gain=receptor_gain;
//	   receptor->ExpCoeff=exp_coeff;
	  }
      catch (EComponentNameNotExist &)
	  {
	  }

	  try
	  {
	   receptor=dynamic_pointer_cast<NReceptor>(Motions[i]->GetComponentL("AfferentR"+RDK::sntoa(j+1)+".Receptor"));
	   receptor->MinInputRange=0;
	   receptor->MaxInputRange=fabs(IaMax)/real_ranges;
//	   receptor->InputAdaptationMode=inp_mode;
//	   receptor->OutputAdaptationMode=out_mode;
//	   receptor->MaxOutputRange=receptor_max_output;
//	   receptor->Gain=receptor_gain;
//	   receptor->ExpCoeff=exp_coeff;
	  }
      catch (EComponentNameNotExist &)
	  {
	  }
	}
  }
 }

}

// Настройка преобразователя импульс-аналог
void NEngineMotionControl::NewPACSetup(double pulse_amplitude, double secretion_tc, double dissociaton_tc, double gain_value, bool gain_div_mode)
{
 UEPtr<NPac> cont;
 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);

 if(!storage)
  return;

 if(PacObjectName->empty())
  return;

 cont=dynamic_pointer_cast<NPac>(storage->TakeObject(PacObjectName));
//try
//{
// cont=AddMissingComponent<NPac>(PacObjectName,"Pac");
//}
//catch(UException &ex)
//{
// return;
//}
 if(!cont)
  return;
 cont->SetName("Pac");
 AddComponent(cont);
 //cont->SetOutputDataSize(0,MMatrixSize(1,int(Motions.size())*2));

 // Начальные значения всем параметрам
 // Амплитуда входных импульсов
 vector<vector<double> > values;

 values.resize(Motions.size()*2);
 for(size_t i=0;i<values.size();i++)
//  values[i].assign(2,1);
  values[i].assign(1,pulse_amplitude);
 ((NPac*)cont)->PulseAmplitude=values;

 // Постоянная времени выделения медиатора
 for(size_t i=0;i<values.size();i++)
//  values[i].assign(2,0.05);
  values[i].assign(1,secretion_tc);
 ((NPac*)cont)->SecretionTC=values;

 // Постоянная времени распада медиатора
 for(size_t i=0;i<values.size();i++)
  values[i].assign(1,dissociaton_tc);
//  values[i].assign(2,0.5);
 ((NPac*)cont)->DissociationTC=values;

  if(gain_div_mode)
  {
   // Усиление
   for(size_t i=0;i<values.size()/2;i++)
	values[i].assign(1,-gain_value/Motions.size());

   for(size_t i=values.size()/2;i<values.size();i++)
	values[i].assign(1,gain_value/Motions.size());
  }
  else
  {
   // Усиление
   for(size_t i=0;i<values.size()/2;i++)
	values[i].assign(1,-gain_value);

   for(size_t i=values.size()/2;i<values.size();i++)
	values[i].assign(1,gain_value);
  }

 ((NPac*)cont)->Gain=values;

}

/// Обновляет параметры Pac
void NEngineMotionControl::UpdatePacTCParameters(void)
{
 UEPtr<NPac> pac=GetComponentL<NPac>("Pac",true);
 if(!pac)
  return;

  // Начальные значения всем параметрам
 // Амплитуда входных импульсов
 vector<vector<double> > values;

 // Постоянная времени выделения медиатора
 values.resize(pac->SecretionTC->size());
 for(size_t i=0;i<values.size();i++)
  values[i].assign(1,PacSecretionTC);
 pac->SecretionTC=values;

 // Постоянная времени распада медиатора
 values.resize(pac->DissociationTC->size());
 for(size_t i=0;i<values.size();i++)
  values[i].assign(1,PacDissociationTC);
 pac->DissociationTC=values;
}



// Установка стандартных связей
void NEngineMotionControl::NewStandardLinksSetup(const string &engine_integrator_name)
{
 bool res=true;

 for(size_t k=0;k<Motions.size();k++)
  res&=CreateLink(Motions[k]->GetName()+".MotoneuronL.LTZone","Output",
				 engine_integrator_name,"Inputs");

 for(size_t k=0;k<Motions.size();k++)
  res&=CreateLink(Motions[k]->GetName()+".MotoneuronR.LTZone","Output",
				 engine_integrator_name,"Inputs");

 res&=CreateLink(engine_integrator_name,"Output","NManipulatorInput1","Input");

 if(res)
  return;
}

// Настройка разделителей интервалов
void NEngineMotionControl::NewIntervalSeparatorsSetup(int mode_value, int last_mode_value, double pos_gain_value, double neg_gain_value)
{
 UContainer* cont=0;
 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);
 bool res=true;
 vector<double>  left_value,right_value;
 vector<double>  pos_gain,neg_gain;

 if(!storage)
  return;

 vector<int> mode;
 mode.assign(1,mode_value);

 pos_gain.assign(1,pos_gain_value);
 neg_gain.assign(1,neg_gain_value);

for (int j=0; j < NumMotionElements ; j++)
{
 NMotionElement *melem = Motions[j];
 //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[j]);
 if(!melem)
	 continue;
 for(int i=0; i < melem->NumControlLoops ; i++)
 {
  if (!CheckComponentL((std::string("NegIntervalSeparator")+sntoa(j+1)+sntoa(i+1)).c_str()))
  {
	   cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NIntervalSeparator"));
	   if(!cont)
		continue;
	   cont->SetName(string("NegIntervalSeparator")+RDK::sntoa(j+1)+RDK::sntoa(i+1));
//	   ((NIntervalSeparator*)cont)->SetNumOutputs(1);
	   ((NIntervalSeparator*)cont)->Gain=neg_gain;

	   left_value.assign(1,AfferentRangesNeg[i][j].first);
	   right_value.assign(1,AfferentRangesNeg[i][j].second);
	   ((NIntervalSeparator*)cont)->MinRange=left_value;
	   ((NIntervalSeparator*)cont)->MaxRange=right_value;
	   res=AddComponent(cont);
  }
  if (!CheckComponentL((std::string("PosIntervalSeparator")+sntoa(j+1)+sntoa(i+1)).c_str()))
  {
	   cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NIntervalSeparator"));
	   if(!cont)
		continue;
	   cont->SetName(string("PosIntervalSeparator")+RDK::sntoa(j+1)+RDK::sntoa(i+1));
//	   ((NIntervalSeparator*)cont)->SetNumOutputs(1);
	   ((NIntervalSeparator*)cont)->Gain=pos_gain;

	   left_value.assign(1,AfferentRangesPos[i][j].first);
	   right_value.assign(1,AfferentRangesPos[i][j].second);
	   ((NIntervalSeparator*)cont)->MinRange=left_value;
	   ((NIntervalSeparator*)cont)->MaxRange=right_value;
	   res=AddComponent(cont);
  }
 }
}


 NewIntervalSeparatorsUpdate(mode_value, last_mode_value);
 if(res)
  return;
}

// Настройка разделителей интервалов
void NEngineMotionControl::NewIntervalSeparatorsUpdate(int mode_value, int last_mode_value)
{
 UContainer* cont=0;
 bool res=true;
 vector<double>  left_value,right_value;

 vector<int> mode;
 mode.assign(1,mode_value);

  for (int j = 0; j < NumMotionElements; j++)
  {
   if(j == NumMotionElements-1)
   {
    mode.assign(1,last_mode_value);
   }

   if(int(Motions.size())<=j)
   {
	LogMessageEx(RDK_EX_WARNING,__FUNCTION__, std::string("Motion element not found, #")+sntoa(j));
    break;
   }

   NMotionElement *melem=Motions[j];
   //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[j]);
	 if(melem)
	 for(int i=0;i<melem->NumControlLoops;i++)
	  {
	   try
       {
		cont=GetComponent(string("NegIntervalSeparator")+RDK::sntoa(j+1)+RDK::sntoa(i+1));
       }
       catch (EComponentNameNotExist &)
       {
        continue;
       }
       ((NIntervalSeparator*)cont)->Mode=mode;

	   left_value.assign(1,AfferentRangesNeg[i][j].first);
	   right_value.assign(1,AfferentRangesNeg[i][j].second);
       ((NIntervalSeparator*)cont)->MinRange=left_value;
       ((NIntervalSeparator*)cont)->MaxRange=right_value;

       try
       {
		cont=GetComponent(string("PosIntervalSeparator")+RDK::sntoa(j+1)+RDK::sntoa(i+1));
       }
       catch (EComponentNameNotExist &)
	   {
        continue;
       }

       ((NIntervalSeparator*)cont)->Mode=mode;

	   left_value.assign(1,AfferentRangesPos[i][j].first);
       right_value.assign(1,AfferentRangesPos[i][j].second);
       ((NIntervalSeparator*)cont)->MinRange=left_value;
	   ((NIntervalSeparator*)cont)->MaxRange=right_value;
	  }
	}

 if(res)
  return;
}

// Установка связей разделителей интервалов
void NEngineMotionControl::NewIntervalSeparatorLinksSetup()
{
 bool res=true;

for(int i=0;i<NumMotionElements;i++)
{
 NMotionElement *melem=Motions[i];
 //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
 if(!melem)
	 continue;
 try{
  // Связи с моделью манипулятора (по умолчанию)
  for(int j=0;j<melem->NumControlLoops;j++)
  {
//    if (!CheckLink("NManipulatorSource1","PosIntervalSeparator"+RDK::sntoa(i+1)+RDK::sntoa(j+1)))
	{
       res=CreateLink("NManipulatorSource1.OutputProxy"+RDK::sntoa(j+1),"Output",//j
                        string("PosIntervalSeparator")+RDK::sntoa(i+1)+RDK::sntoa(j+1),"Input");
       res=CreateLink("NManipulatorSource1.OutputProxy"+RDK::sntoa(j+1),"Output",//j
					 string("PosIntervalSeparator")+RDK::sntoa(i+1)+RDK::sntoa(j+1),"Input");
	}
//	if (!CheckLink("NManipulatorSource1",string("NegIntervalSeparator")+RDK::sntoa(i+1)+RDK::sntoa(j+1)))
	{
       res=CreateLink("NManipulatorSource1.OutputProxy"+RDK::sntoa(j+1),"Output",   //j
					 string("NegIntervalSeparator")+RDK::sntoa(i+1)+RDK::sntoa(j+1),"Input");

	}
  }
 }
 catch (EComponentNameNotExist &) {  }
}


 for(size_t k=0;k<Motions.size();k++)
 {
    NMotionElement *melem=Motions[k];
    //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[k]);
	if(!melem)
	 continue;
	for (int l = 0; l < melem->NumControlLoops; l++) {
      try{
         res=CreateLink(string("PosIntervalSeparator")+RDK::sntoa(k+1)+RDK::sntoa(l+1),"Output",Motions[k]->GetName()+".AfferentL"+sntoa(l+1)+".Receptor","Input");
         res=CreateLink(string("NegIntervalSeparator")+RDK::sntoa(k+1)+RDK::sntoa(l+1),"Output",Motions[k]->GetName()+".AfferentR"+sntoa(l+1)+".Receptor","Input");
		}
        catch (EComponentNameNotExist &) {  }
  }

 }

 if(res)
  return;
}

int NEngineMotionControl::GetNumControlLoops(void)
{
 if(Motions.empty())
  return 0;

 NMotionElement *melem=Motions[0];
 //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[0]);
 if(!melem) // старый вариант управляющего элемента
 {
  if(Motions[0])
  {
   if(Motions[0]->CheckComponentL("Afferent_Ic1"))
	return 4;
   else
    return 3;
  }
  else
   return 0;
 }

 return melem->NumControlLoops;
}
// --------------------------

bool NEngineMotionControl::SetIsAfferentLinked(const int &index, const bool &value)
{
  int NumControlLoops = GetNumControlLoops();
  if((index >=NumControlLoops)||(index <0))
   return false;

  for(int i=0;i<NumMotionElements;i++)
  {
   std::string motion=Motions[i]->GetName();
   if(ControlMode == 1)
   {
    ActiveContours->resize(NumControlLoops,0);
	std::string pos_separator=std::string("PosIntervalSeparator")+RDK::sntoa(i+1)+RDK::sntoa(index+1);
	std::string neg_separator=std::string("NegIntervalSeparator")+RDK::sntoa(i+1)+RDK::sntoa(index+1);

	if (!CheckComponentL(pos_separator.c_str()) ||
	  !CheckComponentL(neg_separator.c_str()))
	 continue;
	std::string motion_name=std::string("MotionElement")+RDK::sntoa(i);
	if(!CheckComponentL((motion_name+".AfferentL"+RDK::sntoa(index+1)+".Receptor").c_str()) ||
	 !CheckComponentL((motion_name+".AfferentR"+RDK::sntoa(index+1)+".Receptor").c_str()))
	 continue;

	bool res=true;

	if(value == true)
	{
	 res&=BreakLink("AfferentSource1","Output",motion_name+".AfferentL"+RDK::sntoa(index+1)+".Receptor","Input");
	 res&=BreakLink("AfferentSource1","Output",motion_name+".AfferentR"+RDK::sntoa(index+1)+".Receptor","Input");
	 res&=CreateLink(pos_separator,"Output",motion_name+".AfferentL"+RDK::sntoa(index+1)+".Receptor","Input");
	 res&=CreateLink(neg_separator,"Output",motion_name+".AfferentR"+RDK::sntoa(index+1)+".Receptor","Input");
	}
	else
	{
	 res&=BreakLink(pos_separator,"Output",motion_name+".AfferentL"+RDK::sntoa(index+1)+".Receptor","Input");
	 res&=BreakLink(neg_separator,"Output",motion_name+".AfferentR"+RDK::sntoa(index+1)+".Receptor","Input");
	 res&=CreateLink("AfferentSource1","Output",motion_name+".AfferentL"+RDK::sntoa(index+1)+".Receptor","Input");
	 res&=CreateLink("AfferentSource1","Output",motion_name+".AfferentR"+RDK::sntoa(index+1)+".Receptor","Input");
	}
    (*ActiveContours)[index] = value;
   }
  }
  return true;
}

bool NEngineMotionControl::GetIsAfferentLinked(const int &index)
{
 if(ControlMode == 1)
 {
  if((index >=GetNumControlLoops())||(index <0))
   return false;
  return (*ActiveContours)[index];
 }
 return false;
}


/// Подключает внутренние генераторы к вставочным нейронам нужного числа управляющих эдементов
/// direction 0 - налево, direction 1 - направо
void NEngineMotionControl::ConnectInternalGenerators(int direction, int num_motion_elements, int control_loop_index)
{
 if(control_loop_index<0 || control_loop_index>=NumControlLoops)
  return;

 if(!InternalGenerator)
  return;

 InternalGenerator->DisconnectAll();
 bool res=true;

 int num_elements=(num_motion_elements<NumMotionElements)?num_motion_elements:NumMotionElements;
 for(int i=0;i<num_elements;i++)
 {
  std::string post_afferent_R_name="PostAfferentL"+sntoa(control_loop_index+1);
  std::string post_afferent_L_name="PostAfferentR"+sntoa(control_loop_index+1);
  if(direction == 0)
  {
   res&=CreateLink("InternalGenerator","Output",std::string("MotionElement")+RDK::sntoa(i)+std::string(".")+post_afferent_R_name+".Soma1.ExcChannel","ChannelInputs");
//   res&=CreateLink("InternalGenerator",0,std::string("MotionElement")+RDK::sntoa(i)+std::string(".")+post_afferent_L_name+".PNeuronMembrane.NegChannel");
   InternalGeneratorDirection=0;
  }
  else
  {
   res&=CreateLink("InternalGenerator","Output",std::string("MotionElement")+RDK::sntoa(i)+std::string(".")+post_afferent_L_name+".Soma1.ExcChannel","ChannelInputs");
//   res&=CreateLink("InternalGenerator",0,std::string("MotionElement")+RDK::sntoa(i)+std::string(".")+post_afferent_R_name+".PNeuronMembrane.NegChannel");
   InternalGeneratorDirection=1;
  }
 }

}

/// Задает частоту работы внутреннего генератора
void NEngineMotionControl::SetInternalGeneratorFrequency(int direction, int num_motion_elements, int control_loop_index, double value)
{
 if(!InternalGenerator)
  return;

 if(InternalGeneratorDirection != direction)
  ConnectInternalGenerators(direction, num_motion_elements, control_loop_index);

 InternalGenerator->Frequency=value;
}

vector<NMotionElement *> NEngineMotionControl::GetMotion(void)
{
  return Motions;
}

}
