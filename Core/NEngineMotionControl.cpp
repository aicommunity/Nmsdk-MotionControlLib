#include "NEngineMotionControl.h"
#include "../../Kernel/NStorage.h"
#include "../BCL/NNet.h"
#include "../SourceLib/NConstGenerator.h"
#include "../SourceLib/NPulseGenerator.h"
#include "../ActLib/NPac.h"
#include "../SensorLib/NSignumSeparator.h"
#include "../SensorLib/NIntervalSeparator.h"
#include "../ReceiverLib/NReceptor.h"
#include "../SourceLib/NManipulatorSource.h"
#include "../SourceLib/NManipulatorSourceEmulator.h"
#include "../ReceiverLib/NManipulatorInputEmulator.h"
#include "../ReceiverLib/NManipulatorInput.h"

namespace NMSDK {

// --------------------------
// Конструкторы и деструкторы
// --------------------------
NEngineMotionControl::NEngineMotionControl(void)
 : NumMotionElements("NumMotionElements",this,&NEngineMotionControl::SetNumMotionElements),
   CreationMode("CreationMode",this,&NEngineMotionControl::SetCreationMode),
   MotionElementClassName("MotionElementClassName",this),
   IaMin("IaMin",this),
   IaMax("IaMax",this),
   IbMin("IbMin",this),
   IbMax("IbMax",this),
   IIMin("IIMin",this),
   IIMax("IIMax",this)
{
}

NEngineMotionControl::~NEngineMotionControl(void)
{
}
// --------------------------

// --------------------------
// Методы управления параметрами
// --------------------------
// Число управляющих элементов
bool NEngineMotionControl::SetNumMotionElements(size_t value)
{
 Ready=false;
 return true;
}

// Режим формирования сети
bool NEngineMotionControl::SetCreationMode(int value)
{
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
 CreationMode=5;
 IaMin=-2.0*M_PI;
 IaMax=2.0*M_PI;
 IbMin=-10;
 IbMax=10;
 IIMin=-M_PI/2;
 IIMax=M_PI/2;
 MotionElementClassName="NMotionElement";

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NEngineMotionControl::ABuild(void)
{
 return true;
}

// Reset computation
bool NEngineMotionControl::AReset(void)
{
 SetNumOutputs(6);
 for(int i=0;i<NumOutputs;i++)
  SetOutputDataSize(i,1);

 receptors.resize(NumMotionElements);
 for(size_t n=0;n<NumMotionElements;n++)
 {
  receptors[n].resize(6);
  NAContainer* cont=0;
  try {
   cont=GetComponent(string("MotionElement")+RDK::sntoa(n));
  }
  catch (EComponentNameNotExist &exc)
  {
   continue;
  }
  try {
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
  }
 }

 return true;
}

// Execute math. computations of current object on current step
bool NEngineMotionControl::ACalculate(void)
{
 int pos_angle=0,pos_speed=0,pos_moment=0;
 int neg_angle=0,neg_speed=0,neg_moment=0;
 // Receptors
 for(size_t i=0;i<NumMotionElements;i++)
 {
  try {
  if(receptors[i][0] && receptors[i][0]->GetInputData(size_t(0))->Double[0] > 0)
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
   ++neg_angle;
  }
  catch (UEPtr<NReceptor>::EUsingZeroPtr &exc)
  {
   continue;
  }

 }

 POutputData[0].Double[0]=pos_angle;
 POutputData[1].Double[0]=neg_angle;
 POutputData[2].Double[0]=pos_speed;
 POutputData[3].Double[0]=neg_speed;
 POutputData[4].Double[0]=pos_moment;
 POutputData[5].Double[0]=neg_moment;
 return true;
}
// --------------------------

// --------------------------
// Методы управления счетом
// --------------------------
// Создает объект с желаемой структурой в соответствии с CreationMode
bool NEngineMotionControl::Create(void)
{
 DelAllComponents();

 switch(CreationMode)
 {
 case 0:
  CreateEngineControlSignumAfferent();
 break;

 case 1:
  CreateEngineControlRangeAfferent();
 break;

 case 2:
  CreateEngineControlRangeAfferent();
 break;

 case 3:
  CreateEngineControlRangeAfferent(true);
 break;

 case 4:
  CreateEngineControlRangeAfferent(false,false);
 break;

 case 5:
  CreateEngineControlRangeAfferent(true,false);
 break;

 case 10:
  CreateEngineControlContinuesNeuronsSimple(false);
 break;

 case 11:
  CreateEngineControl2NeuronsSimplest();
 break;

 };

 return true;
}
// --------------------------

// --------------------------
// Вспомогательные методы
// --------------------------
// Вычисляет диапазоны действия афферентнов
int NEngineMotionControl::CalcAfferentRange(int num_motions, bool cross_ranges, double a_min, double a_max,
			vector<pair<double,double> > &pos_ranges, vector<pair<double,double> > &neg_ranges)
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
   real left_range=a_min,right_range=a_max;
   real pos_range=(right_range-0)/real_ranges;
   real neg_range=(0-left_range)/real_ranges;

   pos_ranges[i].first=rr_index*pos_range;
   pos_ranges[i].second=(rr_index+1)*pos_range;
   neg_ranges[i].first=left_range+rr_index*neg_range;
   neg_ranges[i].second=left_range+(rr_index+1)*neg_range;
  }
  else // нечетное
  {
   real left_range=a_min,right_range=a_max;
   real pos_range=(right_range-0)/real_ranges;
   real neg_range=(0-left_range)/real_ranges;

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
 for(int i=0;i<num_motions;i++)
 {
   real left_range=a_min,right_range=a_max;
   real pos_range=(right_range-0)/real_ranges;
   real neg_range=(0-left_range)/real_ranges;

   pos_ranges[i].first=rr_index*pos_range;
   pos_ranges[i].second=(rr_index+1)*pos_range;
   neg_ranges[i].first=left_range+rr_index*neg_range;
   neg_ranges[i].second=left_range+(rr_index+1)*neg_range;
   ++rr_index;
 }
 }
 return real_ranges;
}

// Настройка рецепторов
void NEngineMotionControl::MotionElementsSetup(UEPtr<NAContainer> net, int inp_mode, int out_mode, double exp_coeff, double receptor_max_output, double receptor_gain, int real_ranges)
{
 UEPtr<NAContainer> cont;
 UEPtr<NStorage> storage=dynamic_pointer_cast<NStorage>(Storage);

 for(size_t i=0;i<NumMotionElements;i++)
 {
  UEPtr<NReceptor> receptor=0;
  cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject(MotionElementClassName));
  if(!cont)
   continue;
  cont->SetName(string("MotionElement")+RDK::sntoa(i));
  net->AddComponent(cont);
  Motions.push_back(static_pointer_cast<NNet>(cont));

  try
  {
   receptor=dynamic_pointer_cast<NReceptor>(Motions[i]->GetComponentL("Afferent_Ia1.Receptor"));
   receptor->MinInputRange=0;
   receptor->MaxInputRange=fabs(IaMin)/real_ranges;
   receptor->InputAdaptationMode=inp_mode;
   receptor->OutputAdaptationMode=out_mode;
   receptor->MaxOutputRange=receptor_max_output;
   receptor->Gain=receptor_gain;
   receptor->ExpCoeff=exp_coeff;
  }
  catch (EComponentNameNotExist &exc)
  {
  }

  try
  {
   receptor=dynamic_pointer_cast<NReceptor>(Motions[i]->GetComponentL("Afferent_Ia2.Receptor"));
   receptor->MinInputRange=0;
   receptor->MaxInputRange=fabs(IaMax)/real_ranges;
   receptor->InputAdaptationMode=inp_mode;
   receptor->OutputAdaptationMode=out_mode;
   receptor->MaxOutputRange=receptor_max_output;
   receptor->Gain=receptor_gain;
   receptor->ExpCoeff=exp_coeff;
  }
  catch (EComponentNameNotExist &exc)
  {
  }

  try
  {
   receptor=dynamic_pointer_cast<NReceptor>(Motions[i]->GetComponentL("Afferent_Ib1.Receptor"));
   receptor->MinInputRange=0;
   receptor->MaxInputRange=fabs(IbMin)/real_ranges;
   receptor->InputAdaptationMode=inp_mode;
   receptor->OutputAdaptationMode=out_mode;
   receptor->MaxOutputRange=receptor_max_output;
   receptor->Gain=receptor_gain;
   receptor->ExpCoeff=exp_coeff;
  }
  catch (EComponentNameNotExist &exc)
  {
  }

  try
  {
   receptor=dynamic_pointer_cast<NReceptor>(Motions[i]->GetComponentL("Afferent_Ib2.Receptor"));
   receptor->MinInputRange=0;
   receptor->MaxInputRange=fabs(IbMax)/real_ranges;
   receptor->InputAdaptationMode=inp_mode;
   receptor->OutputAdaptationMode=out_mode;
   receptor->MaxOutputRange=receptor_max_output;
   receptor->Gain=receptor_gain;
   receptor->ExpCoeff=exp_coeff;
  }
  catch (EComponentNameNotExist &exc)
  {
  }

  try
  {
   receptor=dynamic_pointer_cast<NReceptor>(Motions[i]->GetComponentL("Afferent_II1.Receptor"));
   receptor->MinInputRange=0;
   receptor->MaxInputRange=fabs(IIMin)/real_ranges;
   receptor->InputAdaptationMode=inp_mode;
   receptor->OutputAdaptationMode=out_mode;
   receptor->MaxOutputRange=receptor_max_output;
   receptor->Gain=receptor_gain;
   receptor->ExpCoeff=exp_coeff;
  }
  catch (EComponentNameNotExist &exc)
  {
  }


  try
  {
   receptor=dynamic_pointer_cast<NReceptor>(Motions[i]->GetComponentL("Afferent_II2.Receptor"));
   receptor->MinInputRange=0;
   receptor->MaxInputRange=fabs(IIMax)/real_ranges;
   receptor->InputAdaptationMode=inp_mode;
   receptor->OutputAdaptationMode=out_mode;
   receptor->MaxOutputRange=receptor_max_output;
   receptor->Gain=receptor_gain;
   receptor->ExpCoeff=exp_coeff;
  }
  catch (EComponentNameNotExist &exc)
  {
  }

 }

}

// Настройка преобразователя импульс-аналог
void NEngineMotionControl::PACSetup(UEPtr<NAContainer> net,
		double pulse_amplitude, double secretion_tc, double dissociaton_tc, double gain_value)
{
 NAContainer* cont=0;
 UEPtr<NStorage> storage=dynamic_pointer_cast<NStorage>(Storage);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NPac"));
 if(!cont)
  return;
 cont->SetName("Pac");
 net->AddComponent(cont);
 ((NPac*)cont)->SetOutputDataSize(0,Motions.size()*2);

 // Начальные значения всем параметрам
 // Амплитуда входных импульсов
 vector<Real> values;

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

 // Усиление
 for(size_t i=0;i<values.size()/2;i++)
 {
//  values[i].assign(1,100.0);
  values[i].assign(1,gain_value/Motions.size());
 }

 for(size_t i=values.size()/2;i<values.size();i++)
 {
//  values[i].assign(1,-100.0);
  values[i].assign(1,-gain_value/Motions.size());
 }

 ((NPac*)cont)->Gain=values;

}

// Настройка преобразователя аналог-аналог
void NEngineMotionControl::AACSetup(UEPtr<NAContainer> net, double gain_value)
{
 NSum* cont=0;
 UEPtr<NStorage> storage=dynamic_pointer_cast<NStorage>(Storage);

 cont=dynamic_pointer_cast<NSum>(storage->TakeObject("NSum"));
 if(!cont)
  return;
 cont->SetName("Sum");
 cont->Mode=1;
 net->AddComponent(cont);
 cont->SetOutputDataSize(0,Motions.size()*2);

 // Начальные значения всем параметрам
 // Амплитуда входных импульсов
 vector<Real> values;
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

// Настройка разделителей интервалов
void NEngineMotionControl::IntervalSeparatorsSetup(UEPtr<NAContainer> net, int mode_value, double pos_gain_value, double neg_gain_value)
{
 NAContainer* cont=0;
 UEPtr<NStorage> storage=dynamic_pointer_cast<NStorage>(Storage);
 bool res=true;
 Real left_value,right_value;
 Real pos_gain,neg_gain;

 vector<int> mode;
 mode.assign(1,mode_value);

 pos_gain.assign(1,pos_gain_value);
 neg_gain.assign(1,neg_gain_value);

 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("Ib_NegIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Gain=neg_gain;
  ((NIntervalSeparator*)cont)->Mode=mode;

  left_value.assign(1,Ib_ranges_neg[i].first);
  right_value.assign(1,Ib_ranges_neg[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("Ib_PosIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Mode=mode;
  ((NIntervalSeparator*)cont)->Gain=pos_gain;

  left_value.assign(1,Ib_ranges_pos[i].first);
  right_value.assign(1,Ib_ranges_pos[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("II_NegIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Mode=mode;
  ((NIntervalSeparator*)cont)->Gain=neg_gain;

  left_value.assign(1,II_ranges_neg[i].first);
  right_value.assign(1,II_ranges_neg[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("II_PosIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Mode=mode;
  ((NIntervalSeparator*)cont)->Gain=pos_gain;

  left_value.assign(1,II_ranges_pos[i].first);
  right_value.assign(1,II_ranges_pos[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("Ia_NegIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Mode=mode;
  ((NIntervalSeparator*)cont)->Gain=neg_gain;

  left_value.assign(1,Ia_ranges_neg[i].first);
  right_value.assign(1,Ia_ranges_neg[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("Ia_PosIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Mode=mode;
  ((NIntervalSeparator*)cont)->Gain=pos_gain;

  left_value.assign(1,Ia_ranges_pos[i].first);
  right_value.assign(1,Ia_ranges_pos[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }
 if(res)
  return;
}

// Задание вспомогательных компонент
void NEngineMotionControl::AdditionalComponentsSetup(UEPtr<NAContainer> net)
{
 NAContainer* cont=0;
 UEPtr<NStorage> storage=dynamic_pointer_cast<NStorage>(Storage);
 bool res=true;

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NPGenerator"));
 if(!cont)
  return;
 cont->SetName("IIPosAfferentGenerator");
 ((NPulseGenerator*)cont)->Amplitude=1;
 ((NPulseGenerator*)cont)->Frequency=0;
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NPGenerator"));
 if(!cont)
  return;
 cont->SetName("IINegAfferentGenerator");
 ((NPulseGenerator*)cont)->Amplitude=1;
 ((NPulseGenerator*)cont)->Frequency=0;
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NCGenerator"));
 if(!cont)
  return;
 cont->SetName("AfferentSource1");
 ((NConstGenerator*)cont)->Amplitude=0;
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NCGenerator"));
 if(!cont)
  return;
 cont->SetName("AfferentSource2");
 ((NConstGenerator*)cont)->Amplitude=0;
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NCGenerator"));
 if(!cont)
  return;
 cont->SetName("LengthSource1");
 ((NConstGenerator*)cont)->Amplitude=0;
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NPGenerator"));
 if(!cont)
  return;
 cont->SetName("Renshow1ActivatorGenerator");
 ((NPulseGenerator*)cont)->Frequency=0;
 ((NPulseGenerator*)cont)->Amplitude=1;
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NPGenerator"));
 if(!cont)
  return;
 cont->SetName("Renshow2ActivatorGenerator");
 ((NPulseGenerator*)cont)->Frequency=0;
 ((NPulseGenerator*)cont)->Amplitude=1;
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NPGenerator"));
 if(!cont)
  return;
 cont->SetName("Renshow1DeactivatorGenerator");
 ((NPulseGenerator*)cont)->Frequency=0;
 ((NPulseGenerator*)cont)->Amplitude=1;
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NPGenerator"));
 if(!cont)
  return;
 cont->SetName("Renshow2DeactivatorGenerator");
 ((NPulseGenerator*)cont)->Frequency=0;
 ((NPulseGenerator*)cont)->Amplitude=1;
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NFrequencyReceiver"));
 if(!cont)
  return;
 cont->SetName("Motoneuron1FrequencyReceiver");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NFrequencyReceiver"));
 if(!cont)
  return;
 cont->SetName("Motoneuron2FrequencyReceiver");
 res=net->AddComponent(cont);

	   /*
 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NCGenerator"));
 if(!cont)
  return;
 cont->SetName("EngineMoment");
 ((NConstGenerator*)cont)->Amplitude=0;
 res=net->AddComponent(cont);
                         */
 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NManipulatorSource"));
 if(!cont)
  return;
 cont->SetName("NManipulatorSource1");
 res=net->AddComponent(cont);
  /*
 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NManipulatorSourceEmulator"));
 if(!cont)
  return;
 cont->SetName("NManipulatorSourceEmulator1");
 res=net->AddComponent(cont);
    */
 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NManipulatorInput"));
 if(!cont)
  return;
 cont->SetName("NManipulatorInput1");
 res=net->AddComponent(cont);
	/*
 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NManipulatorInputEmulator"));
 if(!cont)
  return;
 cont->SetName("NManipulatorInputEmulator1");
 res=net->AddComponent(cont);

 // Двигатель
 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NDCEngine"));
 if(!cont)
  return;
 cont->SetName("Engine");
 res=net->AddComponent(cont);
          */
 if(res)
  return;
}

// Установка стандартных связей
void NEngineMotionControl::StandardLinksSetup(UEPtr<NANet> net,
		const string &engine_integrator_name)
{
 bool res=true;

 for(size_t k=0;k<Motions.size();k++)
  res=net->CreateLink(Motions[k]->GetName()+".Motoneuron1.LTZone",0,
				 engine_integrator_name);

 for(size_t k=0;k<Motions.size();k++)
  res=net->CreateLink(Motions[k]->GetName()+".Motoneuron2.LTZone",0,
				 engine_integrator_name);

 res=net->CreateLink(engine_integrator_name,0,"NManipulatorInput1");

 try {
  for(size_t i=0;i<Motions.size();i++)
  {
   res&=net->CreateLink("NManipulatorSource1",2,
				 std::string("Ia_PosIntervalSeparator")+RDK::sntoa(i+1),0);
   res&=net->CreateLink("NManipulatorSource1",2,
				 std::string("Ia_NegIntervalSeparator")+RDK::sntoa(i+1),0);
  }
 }
 catch (EComponentNameNotExist &exc)
 {
 }

 try {
  for(size_t i=0;i<Motions.size();i++)
  {
   res&=net->CreateLink("NManipulatorSource1",1,
				 std::string("II_PosIntervalSeparator")+RDK::sntoa(i+1),0);
   res&=net->CreateLink("NManipulatorSource1",1,
				 std::string("II_NegIntervalSeparator")+RDK::sntoa(i+1),0);
  }
 }
 catch (EComponentNameNotExist &exc)
 {
 }

 try {
  for(size_t i=0;i<Motions.size();i++)
  {
   res&=net->CreateLink("NManipulatorSource1",0,
				 std::string("Ib_PosIntervalSeparator")+RDK::sntoa(i+1),0);
   res&=net->CreateLink("NManipulatorSource1",0,
				 std::string("Ib_NegIntervalSeparator")+RDK::sntoa(i+1),0);
  }
 }
 catch (EComponentNameNotExist &exc)
 {
 }


// res=net->CreateLink(engine_integrator_name,0,"NManipulatorInputEmulator1");

// res=net->CreateLink("NManipulatorInputEmulator1",0,"Engine",0);

// res=net->CreateLink("Engine",0,"NManipulatorSourceEmulator1",0);
// res=net->CreateLink("Engine",1,"NManipulatorSourceEmulator1",1);
// res=net->CreateLink("Engine",2,"NManipulatorSourceEmulator1",2);


// res=net->CreateLink("EngineMoment",0,
//				 "Engine",1);

 if(res)
  return;
}

// Установка связей разделителей интервалов
void NEngineMotionControl::IntervalSeparatorLinksSetup(UEPtr<NANet> net)
{
 bool res=true;

 // Связи с моделью манипулятора (по умолчанию)
 for(size_t i=0;i<NumMotionElements;i++)
 {
  res=net->CreateLink("NManipulatorSource1",2,
				 string("Ia_PosIntervalSeparator")+RDK::sntoa(i+1));
  res=net->CreateLink("NManipulatorSource1",2,
				 string("Ia_NegIntervalSeparator")+RDK::sntoa(i+1));
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  res=net->CreateLink("NManipulatorSource1",1,
				 string("II_PosIntervalSeparator")+RDK::sntoa(i+1));
  res=net->CreateLink("NManipulatorSource1",1,
				 string("II_NegIntervalSeparator")+RDK::sntoa(i+1));
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  res=net->CreateLink("NManipulatorSource1",0,
				 string("Ib_PosIntervalSeparator")+RDK::sntoa(i+1));
  res=net->CreateLink("NManipulatorSource1",0,
				 string("Ib_NegIntervalSeparator")+RDK::sntoa(i+1));
 }

 for(size_t k=0;k<Motions.size();k++)
 {
  try{
   res=net->CreateLink(string("Ia_PosIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_Ia2.Receptor");
   res=net->CreateLink(string("Ia_NegIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_Ia1.Receptor");
  }
  catch (EComponentNameNotExist &exc) {  }
  try{
   res=net->CreateLink(string("II_PosIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_II1.Receptor");
   res=net->CreateLink(string("II_NegIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_II2.Receptor");
  }
  catch (EComponentNameNotExist &exc) {  }

  try{
   res=net->CreateLink(string("Ib_NegIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_Ib2.Receptor");
   res=net->CreateLink(string("Ib_PosIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_Ib1.Receptor");
  }
  catch (EComponentNameNotExist &exc) {  }
 }

 if(res)
  return;
}



// Формируем сеть управления двигателем с разделением информационного потока с датчиков
// на две полосы по знаку
NANet* NEngineMotionControl::CreateEngineControlSignumAfferent(void)
{
 UEPtr<NAContainer> cont;
 bool res;

 vector<UEPtr<NNet> > Motions;

 NANet *net=this;
 UEPtr<NStorage> storage=dynamic_pointer_cast<NStorage>(Storage);
 size_t num_motions=NumMotionElements;
 if(!net)
  return 0;

 // Двигательный элемент
// size_t num_motions=1;

 double exp_coeff=0.1;
 int inp_mode=1;  // Заглушка!! что здесь, надо восстанавливать по старому коду
 int out_mode=0;

 MotionElementsSetup(net, inp_mode, out_mode, exp_coeff, 0.01, 1, 1);

 AdditionalComponentsSetup(net);

 PACSetup(net, 1, 0.05, 0.5, 10);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NPosSignumSeparator"));
 if(!cont)
  return 0;
 cont->SetName("Ib_PosSignumSeparator");
 static_pointer_cast<NSignumSeparator>(cont)->SetNumOutputs(1);
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NNegSignumSeparator"));
 if(!cont)
  return 0;
 cont->SetName("Ib_NegSignumSeparator");
 static_pointer_cast<NSignumSeparator>(cont)->SetNumOutputs(1);
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NPosSignumSeparator"));
 if(!cont)
  return 0;
 cont->SetName("II_PosSignumSeparator");
 static_pointer_cast<NSignumSeparator>(cont)->SetNumOutputs(1);
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NNegSignumSeparator"));
 if(!cont)
  return 0;
 cont->SetName("II_NegSignumSeparator");
 static_pointer_cast<NSignumSeparator>(cont)->SetNumOutputs(1);
 res=net->AddComponent(cont);

 Real gain_value;

 cont=dynamic_pointer_cast<NSignumSeparator>(storage->TakeObject("NPosSignumSeparator"));
 if(!cont)
  return 0;
 cont->SetName("Ia_PosSignumSeparator");
 static_pointer_cast<NSignumSeparator>(cont)->SetNumOutputs(1);
 gain_value.assign(1,1.0/num_motions);
 static_pointer_cast<NSignumSeparator>(cont)->Gain=gain_value;
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NNegSignumSeparator"));
 if(!cont)
  return 0;
 cont->SetName("Ia_NegSignumSeparator");
 static_pointer_cast<NSignumSeparator>(cont)->SetNumOutputs(1);
 gain_value.assign(1,1.0/num_motions);
 static_pointer_cast<NSignumSeparator>(cont)->Gain=gain_value;
 res=net->AddComponent(cont);

// return net; // Заглушка!!!

 // Установка связей
 ULongId item,conn;

 StandardLinksSetup(net, "Pac");
/*
 res=net->CreateLink("Engine",2,
				 "Ia_PosSignumSeparator");
 if(!res)
 {
  delete net;
  return 0;
 }


 res=net->CreateLink("Engine",2,
				 "Ia_NegSignumSeparator");
 if(!res)
 {
  delete net;
  return 0;
 }


 res=net->CreateLink("Engine",1,
				 "II_PosSignumSeparator");
 if(!res)
 {
  delete net;
  return 0;
 }


 res=net->CreateLink("Engine",1,
				 "II_NegSignumSeparator");
 if(!res)
 {
  delete net;
  return 0;
 }


 res=net->CreateLink("Engine",0,
				 "Ib_PosSignumSeparator");
 if(!res)
 {
  delete net;
  return 0;
 }


 res=net->CreateLink("Engine",0,
				 "Ib_NegSignumSeparator");
 if(!res)
 {
  delete net;
  return 0;
 }
		  */

 for(size_t k=0;k<Motions.size();k++)
 {
  res=net->CreateLink("Ia_PosSignumSeparator",0,Motions[k]->GetName()+".Afferent_Ia2.Receptor");
  if(!res)
  {
   delete net;
   return 0;
  }
  res=net->CreateLink("Ia_NegSignumSeparator",0,Motions[k]->GetName()+".Afferent_Ia1.Receptor");
  if(!res)
  {
   delete net;
   return 0;
  }

  res=net->CreateLink("II_PosSignumSeparator",0,Motions[k]->GetName()+".Afferent_II1.Receptor");
  if(!res)
  {
   delete net;
   return 0;
  }
  res=net->CreateLink("II_NegSignumSeparator",0,Motions[k]->GetName()+".Afferent_II2.Receptor");
  if(!res)
  {
   delete net;
   return 0;
  }

  res=net->CreateLink("Ib_NegSignumSeparator",0,Motions[k]->GetName()+".Afferent_Ib1.Receptor");
  if(!res)
  {
   delete net;
   return 0;
  }
  res=net->CreateLink("Ib_PosSignumSeparator",0,Motions[k]->GetName()+".Afferent_Ib2.Receptor");
  if(!res)
  {
   delete net;
   return 0;
  }

  res=net->CreateLink("Renshow1ActivatorGenerator",0,Motions[k]->GetName()+".Motoneuron1.PNeuronMembrane2.PosChannel");
  if(!res)
  {
   delete net;
   return 0;
  }

  res=net->CreateLink("Renshow1DeactivatorGenerator",0,Motions[k]->GetName()+".Motoneuron1.PNeuronMembrane2.NegChannel");
  if(!res)
  {
   delete net;
   return 0;
  }

  res=net->CreateLink("Renshow1ActivatorGenerator",0,Motions[k]->GetName()+".Motoneuron2.PNeuronMembrane2.NegChannel");
  if(!res)
  {
   delete net;
   return 0;
  }

  res=net->CreateLink("Renshow1DeactivatorGenerator",0,Motions[k]->GetName()+".Motoneuron2.PNeuronMembrane2.PosChannel");
  if(!res)
  {
   delete net;
   return 0;
  }

 }

 return net;
}

// Формируем сеть управления двигателем c разделением информационного потока с датчиков
// на полосы по амплитуде
NANet* NEngineMotionControl::CreateEngineControlRangeAfferent(bool crosslinks, bool crossranges)

{
 NAContainer *cont;
 bool res;
 UEPtr<NStorage> storage=static_pointer_cast<NStorage>(Storage);
 size_t num_motions=NumMotionElements;

 // Число неперекрывающихся диапазонов
 int real_ranges=0;

 real_ranges=CalcAfferentRange(num_motions, crossranges, IaMin, IaMax,
			Ia_ranges_pos, Ia_ranges_neg);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IbMin, IbMax,
			Ib_ranges_pos, Ib_ranges_neg);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IIMin, IIMax,
			II_ranges_pos, II_ranges_neg);


 NANet *net=this;//dynamic_cast<NANet*>(storage->TakeObject(netclassname));
 if(!net)
  return 0;

 int outmode=1;
 int inpmode=0;

 double exp_coeff=0.0001;//0.00001;

 // Двигательные элементы
 MotionElementsSetup(net, inpmode, outmode, exp_coeff, 0.01, 1, real_ranges);

 AdditionalComponentsSetup(net);

 PACSetup(net, 1, 0.001, 0.01, 100);

 IntervalSeparatorsSetup(net, 5, 1, -1);

 // Компоненты вычисления статистики
 for(size_t i=0;i<num_motions;i++)
 {
  cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NFrequencyReceiver"));
  if(!cont)
   return 0;
  cont->SetName("PosMNFrequencyReceiver"+RDK::sntoa(i+1));
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NFrequencyReceiver"));
  if(!cont)
   return 0;
  cont->SetName("NegMNFrequencyReceiver"+RDK::sntoa(i+1));
  res=net->AddComponent(cont);
 }


 // Установка связей
 ULongId item,conn;

 StandardLinksSetup(net, "Pac");

 IntervalSeparatorLinksSetup(net);

 // Устанавливаем перекрестные связи, если они нужны
 if(crosslinks)
 {
  for(size_t k=1;k<Motions.size()-1;k++)
  {
   res=net->CreateLink(Motions[k]->GetName()+".Renshow1.LTZone",0,Motions[k-1]->GetName()+".Motoneuron1.PNeuronMembrane.NegChannel");
   res=net->CreateLink(Motions[k]->GetName()+".Renshow1.LTZone",0,Motions[k+1]->GetName()+".Motoneuron1.PNeuronMembrane.NegChannel");
   res=net->CreateLink(Motions[k]->GetName()+".Renshow2.LTZone",0,Motions[k-1]->GetName()+".Motoneuron2.PNeuronMembrane.NegChannel");
   res=net->CreateLink(Motions[k]->GetName()+".Renshow2.LTZone",0,Motions[k+1]->GetName()+".Motoneuron2.PNeuronMembrane.NegChannel");

   res=net->CreateLink(Motions[k]->GetName()+".Renshow1.LTZone",0,Motions[k-1]->GetName()+".Motoneuron1.PNeuronMembrane.NegChannel");
   res=net->CreateLink(Motions[k]->GetName()+".Renshow1.LTZone",0,Motions[k+1]->GetName()+".Motoneuron1.PNeuronMembrane.NegChannel");
   res=net->CreateLink(Motions[k]->GetName()+".Renshow2.LTZone",0,Motions[k-1]->GetName()+".Motoneuron2.PNeuronMembrane.NegChannel");
   res=net->CreateLink(Motions[k]->GetName()+".Renshow2.LTZone",0,Motions[k+1]->GetName()+".Motoneuron2.PNeuronMembrane.NegChannel");
  }

 }

 for(size_t k=0;k<Motions.size();k++)
 {
  res=net->CreateLink(Motions[k]->GetName()+".Motoneuron1.LTZone",0,string("PosMNFrequencyReceiver")+RDK::sntoa(k+1));
  res=net->CreateLink(Motions[k]->GetName()+".Motoneuron2.LTZone",0,string("NegMNFrequencyReceiver")+RDK::sntoa(k+1));
 }

 if(!res)
  return 0;

 return net;
}

// Формируем сеть управления на нейронах с непрерывной генераторной функцией нейронов
NANet* NEngineMotionControl::CreateEngineControlContinuesNeuronsSimple(bool crossranges)
{
 NAContainer *cont;
 bool res;
 UEPtr<NStorage> storage=static_pointer_cast<NStorage>(Storage);
 size_t num_motions=NumMotionElements;

 // Число неперекрывающихся диапазонов
 int real_ranges=0;

 real_ranges=CalcAfferentRange(num_motions, crossranges, IaMin, IaMax,
			Ia_ranges_pos, Ia_ranges_neg);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IbMin, IbMax,
			Ib_ranges_pos, Ib_ranges_neg);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IIMin, IIMax,
			II_ranges_pos, II_ranges_neg);

 NANet *net=this;
 if(!net)
  return 0;

 int outmode=4; //1;
 int inpmode=0;

 double exp_coeff=100;

 MotionElementsSetup(net, inpmode, outmode, exp_coeff, 1, 2, real_ranges);

 AdditionalComponentsSetup(net);

 AACSetup(net, 100);

 IntervalSeparatorsSetup(net, 5, 1, -1);

 // Установка связей
 ULongId item,conn;

 StandardLinksSetup(net, "Sum");

 IntervalSeparatorLinksSetup(net);

 if(!res)
  return 0;

 return net;
}


// Формируем сеть управления на 2 импульсных нейронах
NANet* NEngineMotionControl::CreateEngineControl2NeuronsSimplest(void)
{
 NAContainer *cont;
 bool res;
 UEPtr<NStorage> storage=static_pointer_cast<NStorage>(Storage);
 size_t num_motions=NumMotionElements;

 // Число неперекрывающихся диапазонов
 int real_ranges=0;
 bool crossranges=false;
 bool crosslinks=false;


 real_ranges=CalcAfferentRange(num_motions, crossranges, IaMin, IaMax,
			Ia_ranges_pos, Ia_ranges_neg);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IbMin, IbMax,
			Ib_ranges_pos, Ib_ranges_neg);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IIMin, IIMax,
			II_ranges_pos, II_ranges_neg);


 NANet *net=this;//dynamic_cast<NANet*>(storage->TakeObject(netclassname));
 if(!net)
  return 0;

 int outmode=1;
 int inpmode=0;

 double exp_coeff=0.0001;//0.00001;

 // Двигательные элементы
 MotionElementsSetup(net, inpmode, outmode, exp_coeff, 0.01, 1, real_ranges);

 AdditionalComponentsSetup(net);

 PACSetup(net, 1, 0.001, 0.01, 100);

 IntervalSeparatorsSetup(net, 5, 1, -1);

 // Компоненты вычисления статистики
 for(size_t i=0;i<num_motions;i++)
 {
  cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NFrequencyReceiver"));
  if(!cont)
   return 0;
  cont->SetName("PosMNFrequencyReceiver"+RDK::sntoa(i+1));
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<NAContainer>(storage->TakeObject("NFrequencyReceiver"));
  if(!cont)
   return 0;
  cont->SetName("NegMNFrequencyReceiver"+RDK::sntoa(i+1));
  res=net->AddComponent(cont);
 }


 // Установка связей
 ULongId item,conn;

 StandardLinksSetup(net, "Pac");

 IntervalSeparatorLinksSetup(net);
	/*
 for(size_t k=0;k<Motions.size();k++)
 {
  res=net->CreateLink(Motions[k]->GetName()+".Motoneuron1.LTZone",0,string("PosMNFrequencyReceiver")+RDK::sntoa(k+1));
  res=net->CreateLink(Motions[k]->GetName()+".Motoneuron2.LTZone",0,string("NegMNFrequencyReceiver")+RDK::sntoa(k+1));
 }    */

 if(!res)
  return 0;

 return net;
}
// --------------------------

}
