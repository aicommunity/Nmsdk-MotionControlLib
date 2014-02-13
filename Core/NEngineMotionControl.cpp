#include "NEngineMotionControl.h"
//#include "../../Kernel/NStorage.h"
#include "../BCL/NNet.h"
#include "../SourceLib/NConstGenerator.h"
#include "../SourceLib/NPulseGenerator.h"
#include "../PulseLib/NPac.h"
#include "../SensorLib/NSignumSeparator.h"
#include "../SensorLib/NIntervalSeparator.h"
#include "../PulseLib/NReceptor.h"
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
   AdaptiveStructureMode("AdaptiveStructureMode",this),
   IaMin("IaMin",this),
   IaMax("IaMax",this),
   IbMin("IbMin",this),
   IbMax("IbMax",this),
   IIMin("IIMin",this),
   IIMax("IIMax",this),
   IcMin("IcMin",this),
   IcMax("IcMax",this),
   PacGain("PacGain",this,&NEngineMotionControl::SetPacGain),
   AfferentRangeMode("AfferentRangeMode",this,&NEngineMotionControl::SetAfferentRangeMode),
   PacRangeMode("PacRangeMode",this,&NEngineMotionControl::SetPacRangeMode),

   MinAfferentRange("MinAfferentRange",this),

   CurrentContourAmplitude("CurrentContourAmplitude",this),
   CurrentContourAverage("CurrentContourAverage",this),
   CurrentTransientTime("CurrentTransientTime",this),
   CurrentTransientState("CurrentTransientState",this),
   DestContourMaxAmplitude("DestContourMaxAmplitude",this),
   DestContourMinAmplitude("DestContourMinAmplitude",this),
   DestTransientTime("DestTransientTime",this),
   UseContourData("UseContourData",this),
   TransientHistoryTime("TransientHistoryTime",this),
   TransientObjectIndex("TransientObjectIndex",this),
   TransientAverageThreshold("TransientAverageThreshold",this)


{
  LastAdaptiveTime=0.0;
}

NEngineMotionControl::~NEngineMotionControl(void)
{
}
// --------------------------

// --------------------------
// Методы управления параметрами
// --------------------------
// Число управляющих элементов
bool NEngineMotionControl::SetNumMotionElements(const size_t &value)
{
// NumMotionElements.v=value;
// Create();
 if(AdaptiveStructureMode >0)
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

 IntervalSeparatorsUpdate(this, 5);

 return true;
}

// Режим настрйоки диапазонов pac
bool NEngineMotionControl::SetPacRangeMode(const int &value)
{
 if(value < 0 || value > 3)
  return false;

 PacRangeMode.v=value;
 SetupPacRange();

 return true;
}

// Процентная величина от соответствующей разницы *Max-*Min
bool NEngineMotionControl::SetMinAfferentRange(const double &value)
{
 if(value <=0 || value >1)
  return false;

 bool crossranges=false;

 CalcAfferentRange(NumMotionElements, crossranges, IaMin, IaMax,
			Ia_ranges_pos, Ia_ranges_neg,AfferentRangeMode);
 CalcAfferentRange(NumMotionElements, crossranges, IbMin, IbMax,
			Ib_ranges_pos, Ib_ranges_neg,AfferentRangeMode);
 CalcAfferentRange(NumMotionElements, crossranges, IIMin, IIMax,
			II_ranges_pos, II_ranges_neg,AfferentRangeMode);

 CalcAfferentRange(NumMotionElements, crossranges, IcMin, IcMax,
			Ic_ranges_pos, Ic_ranges_neg,value);

 IntervalSeparatorsUpdate(this, 5);

 return true;
}

// Максимальное усиление управляющего воздействия
bool NEngineMotionControl::SetPacGain(const double &value)
{
 if(value <0)
  return false;

 PacGain.v=value;
 SetupPacRange();

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
 IbMin=-1;
 IbMax=1;
 IIMin=-M_PI/2;
 IIMax=M_PI/2;
 IcMin=-10;
 IcMax=10;
 PacGain=300;
 AfferentRangeMode=2;//0;
 PacRangeMode=2;//0;
 MinAfferentRange=0.1;
 MotionElementClassName="NMotionElement";
 AdaptiveStructureMode=0;

 DestTransientTime=0.1;
 TransientHistoryTime=1.0;
 TransientObjectIndex=0;
 TransientAverageThreshold=0.05;

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NEngineMotionControl::ABuild(void)
{
 if(AdaptiveStructureMode)
  Create(NumMotionElements);

 CurrentContourAmplitude->resize(4);
 CurrentContourAverage->resize(4);

 DestContourMaxAmplitude->resize(4);
 DestContourMinAmplitude->resize(4);
 UseContourData->resize(4);
 return true;
}

// Reset computation
bool NEngineMotionControl::AReset(void)
{
 CurrentContourAmplitude->assign(4,0);
 CurrentContourAverage->assign(4,0);
 CurrentTransientTime=0;
 CurrentTransientState=false;
 TempTransientState=false;
 OldTransientAverage=0;

 SetNumOutputs(6);
 for(int i=0;i<NumOutputs;i++)
  SetOutputDataSize(i,1);

 receptors.resize(NumMotionElements);
 for(size_t n=0;n<NumMotionElements;n++)
 {
  receptors[n].resize(6);
  UContainer* cont=0;
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
 }



 return true;
}

// Execute math. computations of current object on current step
bool NEngineMotionControl::ACalculate(void)
{
 // Считаем статистику
 // за ожидаемое время переходного процесса
 CurrentContourAmplitude->assign(4,0);
 CurrentContourAverage->assign(4,0);

 UEPtr<NManipulatorSource> source=dynamic_pointer_cast<NManipulatorSource>(GetComponent("NManipulatorSource1"));
 HistorySize=TransientHistoryTime*TimeStep;
 vector<double> measure;
 measure.resize(source->GetNumOutputs());
 for(size_t i=0;i<measure.size();i++)
 {
  measure[i]=source->GetOutputData(i).Double[0];
 }

 History.push_back(measure);
 if(History.size()>HistorySize)
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
  (*CurrentContourAmplitude)[i]=max_val-min_val;
  (*CurrentContourAverage)[i]=avg_val/History.size();
 }

 bool old_transient_state=CurrentTransientState;
 double delta=fabs((*CurrentContourAverage)[TransientObjectIndex]-OldTransientAverage)*TimeStep;
 OldTransientAverage=(*CurrentContourAverage)[TransientObjectIndex];
 if(delta>=TransientAverageThreshold && !CurrentTransientState)
 {
  CurrentTransientState=true;
  TempTransientState=true;
  TransientStartTime=Environment->GetTime().GetDoubleTime();
 }
 else
 if(delta<TransientAverageThreshold)
 {
  if(TempTransientState)
  {
   CurrentTransientTime=Environment->GetTime().GetDoubleTime()-TransientStartTime;
  }
  TempTransientState=false;
  if(Environment->GetTime().GetDoubleTime()-TransientStartTime>TransientHistoryTime*3.0)
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

 int pos_angle=0,pos_speed=0,pos_moment=0;
 int neg_angle=0,neg_speed=0,neg_moment=0;
 // Receptors
 for(size_t i=0;i<NumMotionElements;i++)
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

 case 12:
  CreateEngineControl2NeuronsSimplest(true);
 break;


 case 13:
  CreateEngineControl2NeuronsSimplest(true,true);
 break;

 case 14:
  CreateNewEngineControl2NeuronsSimplest(true);
 break;
 };

 SetupPacRange();
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
	 PComponents[i]->GetName() == "NManipulatorSource1" ||
	 PComponents[i]->GetName() == "NManipulatorInput1")
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

 if(LastAdaptiveTime>0 && Environment->GetTime().GetDoubleTime()-LastAdaptiveTime<*TransientHistoryTime)
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
 try
 {
  cont=dynamic_pointer_cast<NPac>(GetComponentL("Pac"));
 }
 catch (EComponentNameNotExist &exc)
 {
  return;
 }

 double a_min=-PacGain;
 double a_max=PacGain;
 int num_motions=NumMotionElements;
 vector<Real> values=cont->Gain;
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


// Настройка рецепторов
void NEngineMotionControl::MotionElementsSetup(UEPtr<UContainer> net, int inp_mode, int out_mode, double exp_coeff, double receptor_max_output, double receptor_gain, int real_ranges)
{
 UEPtr<UContainer> cont;
 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);

 if(!storage)
  return;

 for(size_t i=0;i<NumMotionElements;i++)
 {
  UEPtr<NReceptor> receptor=0;
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(MotionElementClassName));
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

  try
  {
   receptor=dynamic_pointer_cast<NReceptor>(Motions[i]->GetComponentL("Afferent_Ic1.Receptor"));
   receptor->MinInputRange=0;
   receptor->MaxInputRange=fabs(IcMin)/real_ranges;
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
   receptor=dynamic_pointer_cast<NReceptor>(Motions[i]->GetComponentL("Afferent_Ic2.Receptor"));
   receptor->MinInputRange=0;
   receptor->MaxInputRange=fabs(IcMax)/real_ranges;
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
void NEngineMotionControl::PACSetup(UEPtr<UContainer> net,
		double pulse_amplitude, double secretion_tc, double dissociaton_tc, double gain_value, bool gain_div_mode)
{
 UContainer* cont=0;
 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);

 if(!storage)
  return;

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPac"));
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

// Настройка преобразователя аналог-аналог
void NEngineMotionControl::AACSetup(UEPtr<UContainer> net, double gain_value)
{
 NSum* cont=0;
 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);

 if(!storage)
  return;

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
void NEngineMotionControl::IntervalSeparatorsSetup(UEPtr<UContainer> net, int mode_value, double pos_gain_value, double neg_gain_value, bool II, bool Ia, bool Ib, bool Ic)
{
 UContainer* cont=0;
 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);
 bool res=true;
 Real left_value,right_value;
 Real pos_gain,neg_gain;

 if(!storage)
  return;

 vector<int> mode;
 mode.assign(1,mode_value);

 pos_gain.assign(1,pos_gain_value);
 neg_gain.assign(1,neg_gain_value);

if(Ib)
{
 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("Ib_NegIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Gain=neg_gain;

  left_value.assign(1,Ib_ranges_neg[i].first);
  right_value.assign(1,Ib_ranges_neg[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("Ib_PosIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Gain=pos_gain;

  left_value.assign(1,Ib_ranges_pos[i].first);
  right_value.assign(1,Ib_ranges_pos[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }
}

if(II)
{
 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("II_NegIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Gain=neg_gain;

  left_value.assign(1,II_ranges_neg[i].first);
  right_value.assign(1,II_ranges_neg[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("II_PosIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Gain=pos_gain;

  left_value.assign(1,II_ranges_pos[i].first);
  right_value.assign(1,II_ranges_pos[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }
}

if(Ia)
{
 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("Ia_NegIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Gain=neg_gain;

  left_value.assign(1,Ia_ranges_neg[i].first);
  right_value.assign(1,Ia_ranges_neg[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("Ia_PosIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Gain=pos_gain;

  left_value.assign(1,Ia_ranges_pos[i].first);
  right_value.assign(1,Ia_ranges_pos[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }
}

if(Ic)
{
 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("Ic_NegIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Gain=neg_gain;

  left_value.assign(1,Ic_ranges_neg[i].first);
  right_value.assign(1,Ic_ranges_neg[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("Ic_PosIntervalSeparator")+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Gain=pos_gain;

  left_value.assign(1,Ic_ranges_pos[i].first);
  right_value.assign(1,Ic_ranges_pos[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }
}

 IntervalSeparatorsUpdate(net, mode_value);
 if(res)
  return;
}

// Настройка разделителей интервалов
void NEngineMotionControl::IntervalSeparatorsUpdate(UEPtr<UContainer> net, int mode_value)
{
 UContainer* cont=0;
 bool res=true;
 Real left_value,right_value;

 vector<int> mode;
 mode.assign(1,mode_value);

 for(size_t i=0;i<NumMotionElements;i++)
 {
  try
  {
   cont=net->GetComponent(string("Ib_NegIntervalSeparator")+RDK::sntoa(i+1));
  }
  catch (EComponentNameNotExist &exc)
  {
   continue;
  }
  ((NIntervalSeparator*)cont)->Mode=mode;

  left_value.assign(1,Ib_ranges_neg[i].first);
  right_value.assign(1,Ib_ranges_neg[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  try
  {
   cont=net->GetComponent(string("Ib_PosIntervalSeparator")+RDK::sntoa(i+1));
  }
  catch (EComponentNameNotExist &exc)
  {
   continue;
  }

  ((NIntervalSeparator*)cont)->Mode=mode;

  left_value.assign(1,Ib_ranges_pos[i].first);
  right_value.assign(1,Ib_ranges_pos[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  try
  {
   cont=net->GetComponent(string("II_NegIntervalSeparator")+RDK::sntoa(i+1));
  }
  catch (EComponentNameNotExist &exc)
  {
   continue;
  }

  ((NIntervalSeparator*)cont)->Mode=mode;

  left_value.assign(1,II_ranges_neg[i].first);
  right_value.assign(1,II_ranges_neg[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  try
  {
   cont=net->GetComponent(string("II_PosIntervalSeparator")+RDK::sntoa(i+1));
  }
  catch (EComponentNameNotExist &exc)
  {
   continue;
  }

  ((NIntervalSeparator*)cont)->Mode=mode;

  left_value.assign(1,II_ranges_pos[i].first);
  right_value.assign(1,II_ranges_pos[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  try
  {
   cont=net->GetComponent(string("Ia_NegIntervalSeparator")+RDK::sntoa(i+1));
  }
  catch (EComponentNameNotExist &exc)
  {
   continue;
  }

  ((NIntervalSeparator*)cont)->Mode=mode;

  left_value.assign(1,Ia_ranges_neg[i].first);
  right_value.assign(1,Ia_ranges_neg[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  try
  {
   cont=net->GetComponent(string("Ia_PosIntervalSeparator")+RDK::sntoa(i+1));
  }
  catch (EComponentNameNotExist &exc)
  {
   continue;
  }

  ((NIntervalSeparator*)cont)->Mode=mode;

  left_value.assign(1,Ia_ranges_pos[i].first);
  right_value.assign(1,Ia_ranges_pos[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  try
  {
   cont=net->GetComponent(string("Ic_NegIntervalSeparator")+RDK::sntoa(i+1));
  }
  catch (EComponentNameNotExist &exc)
  {
   continue;
  }
  ((NIntervalSeparator*)cont)->Mode=mode;

  left_value.assign(1,Ic_ranges_neg[i].first);
  right_value.assign(1,Ic_ranges_neg[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
 }

 for(size_t i=0;i<NumMotionElements;i++)
 {
  try
  {
   cont=net->GetComponent(string("Ic_PosIntervalSeparator")+RDK::sntoa(i+1));
  }
  catch (EComponentNameNotExist &exc)
  {
   continue;
  }

  ((NIntervalSeparator*)cont)->Mode=mode;

  left_value.assign(1,Ic_ranges_pos[i].first);
  right_value.assign(1,Ic_ranges_pos[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
 }


 if(res)
  return;
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

 if(CheckName("NManipulatorSource"))
 {
  UEPtr<UADItem> cont2=0;
  cont2=dynamic_pointer_cast<UADItem>(storage->TakeObject("NManipulatorSource"));
  if(!cont2)
   return;
  cont2->SetName("NManipulatorSource1");
  res=net->AddComponent(cont2);
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

// Установка стандартных связей
void NEngineMotionControl::StandardLinksSetup(UEPtr<UNet> net,
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
  /*
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

 try {
  for(size_t i=0;i<Motions.size();i++)
  {
   res&=net->CreateLink("NManipulatorSource1",3,
				 std::string("Ic_PosIntervalSeparator")+RDK::sntoa(i+1),0);
   res&=net->CreateLink("NManipulatorSource1",3,
				 std::string("Ic_NegIntervalSeparator")+RDK::sntoa(i+1),0);
  }
 }
 catch (EComponentNameNotExist &exc)
 {
 }
     */
 if(res)
  return;
}

// Установка связей разделителей интервалов
void NEngineMotionControl::IntervalSeparatorLinksSetup(UEPtr<UNet> net, bool II, bool Ia, bool Ib, bool Ic)
{
 bool res=true;

if(Ia)
{
 try{
  // Связи с моделью манипулятора (по умолчанию)
  for(size_t i=0;i<NumMotionElements;i++)
  {
   res=net->CreateLink("NManipulatorSource1",2,
				 string("Ia_PosIntervalSeparator")+RDK::sntoa(i+1));
   res=net->CreateLink("NManipulatorSource1",2,
				 string("Ia_NegIntervalSeparator")+RDK::sntoa(i+1));
  }
 }
 catch (EComponentNameNotExist &exc) {  }
}

if(II)
{
 try{
  for(size_t i=0;i<NumMotionElements;i++)
  {
   res=net->CreateLink("NManipulatorSource1",1,
				 string("II_PosIntervalSeparator")+RDK::sntoa(i+1));
   res=net->CreateLink("NManipulatorSource1",1,
				 string("II_NegIntervalSeparator")+RDK::sntoa(i+1));
  }
 }
 catch (EComponentNameNotExist &exc) {  }
}

if(Ib)
{
 try{
  for(size_t i=0;i<NumMotionElements;i++)
  {
   res=net->CreateLink("NManipulatorSource1",0,
				 string("Ib_PosIntervalSeparator")+RDK::sntoa(i+1));
   res=net->CreateLink("NManipulatorSource1",0,
				 string("Ib_NegIntervalSeparator")+RDK::sntoa(i+1));
  }
 }
 catch (EComponentNameNotExist &exc) {  }
}

if(Ic)
{
 try{
  for(size_t i=0;i<NumMotionElements;i++)
  {
   res=net->CreateLink("NManipulatorSource1",3,
				 string("Ic_PosIntervalSeparator")+RDK::sntoa(i+1));
   res=net->CreateLink("NManipulatorSource1",3,
				 string("Ic_NegIntervalSeparator")+RDK::sntoa(i+1));
  }
 }
 catch (EComponentNameNotExist &exc) {  }
}



 for(size_t k=0;k<Motions.size();k++)
 {
  try{
   res=net->CreateLink(string("Ia_PosIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_Ia2.Receptor",0);
   res=net->CreateLink(string("Ia_NegIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_Ia1.Receptor",0);
  }
  catch (EComponentNameNotExist &exc) {  }
  try{
   res=net->CreateLink(string("II_PosIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_II1.Receptor",0);
   res=net->CreateLink(string("II_NegIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_II2.Receptor",0);
  }
  catch (EComponentNameNotExist &exc) {  }

  try{
   res=net->BreakLink(string("Ib_NegIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_Ib2.Receptor",0);
   res=net->BreakLink(string("Ib_PosIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_Ib1.Receptor",0);
  }
  catch (EComponentNameNotExist &exc) {  }

  try{
   res=net->CreateLink(string("Ic_NegIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_Ic2.Receptor",0);
   res=net->CreateLink(string("Ic_PosIntervalSeparator")+RDK::sntoa(k+1),0,Motions[k]->GetName()+".Afferent_Ic1.Receptor",0);
  }
  catch (EComponentNameNotExist &exc) {  }
 }

 if(res)
  return;
}



// Формируем сеть управления двигателем с разделением информационного потока с датчиков
// на две полосы по знаку
UNet* NEngineMotionControl::CreateEngineControlSignumAfferent(void)
{
 UEPtr<UContainer> cont;
 bool res;

 vector<UEPtr<NNet> > Motions;

 UNet *net=this;
 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);
 size_t num_motions=NumMotionElements;
 if(!net)
  return 0;

 if(!storage)
  return 0;

 // Двигательный элемент
// size_t num_motions=1;

 double exp_coeff=0.1;
 int inp_mode=1;  // Заглушка!! что здесь, надо восстанавливать по старому коду
 int out_mode=0;

 MotionElementsSetup(net, inp_mode, out_mode, exp_coeff, 0.01, 1, 1);

 AdditionalComponentsSetup(net);

 PACSetup(net, 1, 0.05, 0.5, 10);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPosSignumSeparator"));
 if(!cont)
  return 0;
 cont->SetName("Ib_PosSignumSeparator");
 static_pointer_cast<NSignumSeparator>(cont)->SetNumOutputs(1);
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NNegSignumSeparator"));
 if(!cont)
  return 0;
 cont->SetName("Ib_NegSignumSeparator");
 static_pointer_cast<NSignumSeparator>(cont)->SetNumOutputs(1);
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPosSignumSeparator"));
 if(!cont)
  return 0;
 cont->SetName("II_PosSignumSeparator");
 static_pointer_cast<NSignumSeparator>(cont)->SetNumOutputs(1);
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NNegSignumSeparator"));
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

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NNegSignumSeparator"));
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
UNet* NEngineMotionControl::CreateEngineControlRangeAfferent(bool crosslinks, bool crossranges)

{
 UContainer *cont;
 bool res;
 UEPtr<UStorage> storage=static_pointer_cast<UStorage>(Storage);
 size_t num_motions=NumMotionElements;

 if(!storage)
  return 0;

 // Число неперекрывающихся диапазонов
 int real_ranges=0;

 real_ranges=CalcAfferentRange(num_motions, crossranges, IaMin, IaMax,
			Ia_ranges_pos, Ia_ranges_neg,AfferentRangeMode);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IbMin, IbMax,
			Ib_ranges_pos, Ib_ranges_neg,AfferentRangeMode);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IIMin, IIMax,
			II_ranges_pos, II_ranges_neg,AfferentRangeMode);


 UNet *net=this;//dynamic_cast<UNet*>(storage->TakeObject(netclassname));
 if(!net)
  return 0;

 int outmode=1;
 int inpmode=0;

 double exp_coeff=0.0001;//0.00001;

 // Двигательные элементы
 MotionElementsSetup(net, inpmode, outmode, exp_coeff, 0.01, 1, real_ranges);

 AdditionalComponentsSetup(net);

 PACSetup(net, 1, 0.001, 0.001, 100);

 IntervalSeparatorsSetup(net, 5, 1, -1);

 // Компоненты вычисления статистики
 for(size_t i=0;i<num_motions;i++)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NFrequencyReceiver"));
  if(!cont)
   return 0;
  cont->SetName("PosMNFrequencyReceiver"+RDK::sntoa(i+1));
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NFrequencyReceiver"));
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
UNet* NEngineMotionControl::CreateEngineControlContinuesNeuronsSimple(bool crossranges)
{
 UContainer *cont;
 bool res;
 UEPtr<UStorage> storage=static_pointer_cast<UStorage>(Storage);
 size_t num_motions=NumMotionElements;

 // Число неперекрывающихся диапазонов
 int real_ranges=0;

 real_ranges=CalcAfferentRange(num_motions, crossranges, IaMin, IaMax,
			Ia_ranges_pos, Ia_ranges_neg,AfferentRangeMode);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IbMin, IbMax,
			Ib_ranges_pos, Ib_ranges_neg,AfferentRangeMode);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IIMin, IIMax,
			II_ranges_pos, II_ranges_neg,AfferentRangeMode);

 UNet *net=this;
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
UNet* NEngineMotionControl::CreateEngineControl2NeuronsSimplest(bool use_speed_force, bool use_add_contours)
{
 UContainer *cont;
 bool res;
 UEPtr<UStorage> storage=static_pointer_cast<UStorage>(Storage);
 size_t num_motions=NumMotionElements;

 // Число неперекрывающихся диапазонов
 int real_ranges=0;
 bool crossranges=false;
 bool crosslinks=false;


 real_ranges=CalcAfferentRange(num_motions, crossranges, IaMin, IaMax,
			Ia_ranges_pos, Ia_ranges_neg,AfferentRangeMode);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IbMin, IbMax,
			Ib_ranges_pos, Ib_ranges_neg,AfferentRangeMode);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IIMin, IIMax,
			II_ranges_pos, II_ranges_neg,AfferentRangeMode);

 if(use_add_contours)
 {
  real_ranges=CalcAfferentRange(num_motions, crossranges, IcMin, IcMax,
			Ic_ranges_pos, Ic_ranges_neg,AfferentRangeMode);
 }


 UNet *net=this;//dynamic_cast<UNet*>(storage->TakeObject(netclassname));
 if(!net)
  return 0;

 int outmode=0;
 int inpmode=0;

 double exp_coeff=0.0001;//0.00001;

 // Двигательные элементы
 MotionElementsSetup(net, inpmode, outmode, exp_coeff, 100, 1, real_ranges);

 AdditionalComponentsSetup(net);

 PACSetup(net, 1, 0.001, 0.001, 100,false);

 if(use_speed_force)
 {
  if(use_add_contours)
   IntervalSeparatorsSetup(net, 5, 1, -1,true,true,true,true);
  else
   IntervalSeparatorsSetup(net, 5, 1, -1,true,true,true);
 }
 else
  IntervalSeparatorsSetup(net, 5, 1, -1,true,false,false);
		 /*
 // Компоненты вычисления статистики
 for(size_t i=0;i<num_motions;i++)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NFrequencyReceiver"));
  if(!cont)
   continue;
  cont->SetName("PosMNFrequencyReceiver"+RDK::sntoa(i+1));
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NFrequencyReceiver"));
  if(!cont)
   continue;
  cont->SetName("NegMNFrequencyReceiver"+RDK::sntoa(i+1));
  res=net->AddComponent(cont);
 }
                  */

 // Установка связей
 ULongId item,conn;

 StandardLinksSetup(net, "Pac");

 if(use_speed_force)
 {
  if(use_add_contours)
   IntervalSeparatorLinksSetup(net,true,true,true,true);
  else
   IntervalSeparatorLinksSetup(net,true,true,true);
 }
 else
  IntervalSeparatorLinksSetup(net,true,false,false);

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


// Формируем сеть управления новым способом на 2 импульсных нейронах
UNet* NEngineMotionControl::CreateNewEngineControl2NeuronsSimplest(bool use_speed_force)
{
 UContainer *cont;
 bool res;
 UEPtr<UStorage> storage=static_pointer_cast<UStorage>(Storage);
 size_t num_motions=NumMotionElements;

 // Число неперекрывающихся диапазонов
 int real_ranges=0;
 bool crossranges=false;
 bool crosslinks=false;


 real_ranges=CalcAfferentRange(num_motions, crossranges, IaMin, IaMax,
			Ia_ranges_pos, Ia_ranges_neg,AfferentRangeMode);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IbMin, IbMax,
			Ib_ranges_pos, Ib_ranges_neg,AfferentRangeMode);
 real_ranges=CalcAfferentRange(num_motions, crossranges, IIMin, IIMax,
			II_ranges_pos, II_ranges_neg,AfferentRangeMode);



 UNet *net=this;//dynamic_cast<UNet*>(storage->TakeObject(netclassname));
// net = dynamic_cast<NMotionElement*>(this);
 if(!net)
  return 0;

 int outmode=0;
 int inpmode=0;

 double exp_coeff=0.0001;//0.00001;

 // Двигательные элементы
 NewMotionElementsSetup(net, inpmode, outmode, exp_coeff, 100, 1, real_ranges);

 AdditionalComponentsSetup(net);

 NewPACSetup(net, 1, 0.001, 0.001, 100,false);

 if(use_speed_force)
 {
   IntervalSeparatorsSetup(net, 5, 1, -1);
 }
 else
  IntervalSeparatorsSetup(net, 5, 1, -1);
		 /*
 // Компоненты вычисления статистики
 for(size_t i=0;i<num_motions;i++)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NFrequencyReceiver"));
  if(!cont)
   continue;
  cont->SetName("PosMNFrequencyReceiver"+RDK::sntoa(i+1));
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NFrequencyReceiver"));
  if(!cont)
   continue;
  cont->SetName("NegMNFrequencyReceiver"+RDK::sntoa(i+1));
  res=net->AddComponent(cont);
 }
                  */

 // Установка связей
 ULongId item,conn;

 NewStandardLinksSetup(net, "Pac");

 if(use_speed_force)
 {
   IntervalSeparatorLinksSetup(net);
 }
 else
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

// Настройка рецепторов
void NEngineMotionControl::NewMotionElementsSetup(UEPtr<UContainer> net, int inp_mode, int out_mode, double exp_coeff, double receptor_max_output, double receptor_gain, int real_ranges)
{
 UEPtr<UContainer> cont;
 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);


 if(!storage)
  return;

 for(size_t i=0;i<NumMotionElements;i++)
 {
  UEPtr<NReceptor> receptor=0;
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(MotionElementClassName));
  if(!cont)
   continue;
  cont->SetName(string("MotionElement")+RDK::sntoa(i));
  net->AddComponent(cont);
  Motions.push_back(static_pointer_cast<NNet>(cont));
  NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
  melem->SetNumControlLoops(2);
  melem->Build();
	for (int j = 0; j < melem->NumControlLoops; j++)
	{
	  try
	  {
	   receptor=dynamic_pointer_cast<NReceptor>(Motions[i]->GetComponentL("AfferentL"+RDK::sntoa(j+1)+".Receptor"));
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
	   receptor=dynamic_pointer_cast<NReceptor>(Motions[i]->GetComponentL("AfferentR"+RDK::sntoa(j+1)+".Receptor"));
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
	}

 }

}

// Настройка преобразователя импульс-аналог
void NEngineMotionControl::NewPACSetup(UEPtr<UContainer> net,
		double pulse_amplitude, double secretion_tc, double dissociaton_tc, double gain_value, bool gain_div_mode)
{
 UContainer* cont=0;
 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);

 if(!storage)
  return;

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPac"));
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

// Установка стандартных связей
void NEngineMotionControl::NewStandardLinksSetup(UEPtr<UNet> net,
		const string &engine_integrator_name)
{
 bool res=true;

 for(size_t k=0;k<Motions.size();k++)
  res=net->CreateLink(Motions[k]->GetName()+".MotoneuronL.LTZone",0,
				 engine_integrator_name);

 for(size_t k=0;k<Motions.size();k++)
  res=net->CreateLink(Motions[k]->GetName()+".MotoneuronR.LTZone",0,
				 engine_integrator_name);

 res=net->CreateLink(engine_integrator_name,0,"NManipulatorInput1");
  /*
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

 try {
  for(size_t i=0;i<Motions.size();i++)
  {
   res&=net->CreateLink("NManipulatorSource1",3,
				 std::string("Ic_PosIntervalSeparator")+RDK::sntoa(i+1),0);
   res&=net->CreateLink("NManipulatorSource1",3,
				 std::string("Ic_NegIntervalSeparator")+RDK::sntoa(i+1),0);
  }
 }
 catch (EComponentNameNotExist &exc)
 {
 }
     */
 if(res)
  return;
}

// Настройка разделителей интервалов
void NEngineMotionControl::NewIntervalSeparatorsSetup(UEPtr<UContainer> net, int mode_value, double pos_gain_value, double neg_gain_value)
{
 UContainer* cont=0;
 UEPtr<UStorage> storage=dynamic_pointer_cast<UStorage>(Storage);
 bool res=true;
 Real left_value,right_value;
 Real pos_gain,neg_gain;

 if(!storage)
  return;

 vector<int> mode;
 mode.assign(1,mode_value);

 pos_gain.assign(1,pos_gain_value);
 neg_gain.assign(1,neg_gain_value);

for (size_t j=0; j < NumMotionElements ; j++)
{
 NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[j]);
 for(int i=0; i < melem->NumControlLoops ; i++)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("NegIntervalSeparator")+RDK::sntoa(j+1)+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Gain=neg_gain;

  left_value.assign(1,Ib_ranges_neg[i].first);
  right_value.assign(1,Ib_ranges_neg[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NIntervalSeparator"));
  if(!cont)
   continue;
  cont->SetName(string("PosIntervalSeparator")+RDK::sntoa(j+1)+RDK::sntoa(i+1));
  ((NIntervalSeparator*)cont)->SetNumOutputs(1);
  ((NIntervalSeparator*)cont)->Gain=pos_gain;

  left_value.assign(1,Ib_ranges_pos[i].first);
  right_value.assign(1,Ib_ranges_pos[i].second);
  ((NIntervalSeparator*)cont)->MinRange=left_value;
  ((NIntervalSeparator*)cont)->MaxRange=right_value;
  res=net->AddComponent(cont);
 }
}


 NewIntervalSeparatorsUpdate(net, mode_value);
 if(res)
  return;
}

// Настройка разделителей интервалов
void NEngineMotionControl::NewIntervalSeparatorsUpdate(UEPtr<UContainer> net, int mode_value)
{
 UContainer* cont=0;
 bool res=true;
 Real left_value,right_value;

 vector<int> mode;
 mode.assign(1,mode_value);

  for (int j = 0; j < NumMotionElements; j++)
  {
	 NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[j]);
	 for(size_t i=0;i<melem->NumControlLoops;i++)
	  {
	   try
       {
		cont=net->GetComponent(string("NegIntervalSeparator")+RDK::sntoa(j+1)+RDK::sntoa(i+1));
       }
       catch (EComponentNameNotExist &exc)
       {
        continue;
       }
       ((NIntervalSeparator*)cont)->Mode=mode;

       left_value.assign(1,Ib_ranges_neg[i].first);
       right_value.assign(1,Ib_ranges_neg[i].second);
       ((NIntervalSeparator*)cont)->MinRange=left_value;
       ((NIntervalSeparator*)cont)->MaxRange=right_value;

       try
       {
		cont=net->GetComponent(string("PosIntervalSeparator")+RDK::sntoa(j+1)+RDK::sntoa(i+1));
       }
       catch (EComponentNameNotExist &exc)
       {
        continue;
       }

       ((NIntervalSeparator*)cont)->Mode=mode;

       left_value.assign(1,Ib_ranges_pos[i].first);
       right_value.assign(1,Ib_ranges_pos[i].second);
       ((NIntervalSeparator*)cont)->MinRange=left_value;
	   ((NIntervalSeparator*)cont)->MaxRange=right_value;
	  }
	}

 if(res)
  return;
}

// Установка связей разделителей интервалов
void NEngineMotionControl::NewIntervalSeparatorLinksSetup(UEPtr<UNet> net)
{
 bool res=true;

for(size_t i=0;i<NumMotionElements;i++)
{
 NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
 try{
  // Связи с моделью манипулятора (по умолчанию)
  for(int j=0;j<melem->NumControlLoops;j++)
  {
   res=net->CreateLink("NManipulatorSource1",j,
				 string("PosIntervalSeparator")+RDK::sntoa(i+1)+RDK::sntoa(j+1));
   res=net->CreateLink("NManipulatorSource1",j,
				 string("NegIntervalSeparator")+RDK::sntoa(i+1)+RDK::sntoa(j+1));
  }
 }
 catch (EComponentNameNotExist &exc) {  }
}


 for(size_t k=0;k<Motions.size();k++)
 {
 	NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[k]);
	for (int l = 0; l < melem->NumControlLoops; l++) {
      try{
		 res=net->CreateLink(string("PosIntervalSeparator")+RDK::sntoa(k+1)+RDK::sntoa(l+1),0,Motions[k]->GetName()+".AfferentR"+sntoa(l+1)+".Receptor",0);
		 res=net->CreateLink(string("NegIntervalSeparator")+RDK::sntoa(k+1)+RDK::sntoa(l+1),0,Motions[k]->GetName()+".AfferentL"+sntoa(l+1)+".Receptor",0);
		}
        catch (EComponentNameNotExist &exc) {  }
  }

 }

 if(res)
  return;
}


// --------------------------

}
