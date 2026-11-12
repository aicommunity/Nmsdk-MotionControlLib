/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2010.
E-mail:			alexab@ailab.ru
url:			http://ailab.ru
Version:		1.0.0

This file - part of the project: NMSDK

File License:		BSD License
Project License:	GNU GPLv3 License
See file license.txt for more information
*********************************************************** */
#ifndef NEngineMotionControlH
#define NEngineMotionControlH

#include "../../Nmsdk-PulseLib/Core/NReceptor.h"
#include "../../Nmsdk-PulseLib/Core/NNet.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../Rdk-BasicLib/Core/UBCLLibrary.h"
#include "NMotionElement.h"
#include "NManipulatorSource.h"
#include "NIntervalSeparator.h"
#include "NControlObjectSource.h"



namespace NMSDK {

class RDK_LIB_TYPE NEngineMotionControl: public UNet
{
public: //  
//   
RDK::UProperty<int, NEngineMotionControl, ptPubParameter> NumControlLoops;
//   
RDK::UProperty<int, NEngineMotionControl, ptPubParameter> NumMotionElements;

//   
// 0 - Signum
// 1 - Range
// 2 - Branched Range
// 3 - Branched Range Crosslinks
// 4 - Branched Ind. Range
// 5 - Branched Ind. Range Crosslinks
// 10 - Branched Ind. Range Continues LTZone neurons
// 11 - Simplest 2 neuron model
// 12 - As 11 but speed and force control added
// 13 - As 12 but additional control contour added
// 14 - New net with parametric structure control
RDK::UProperty<int, NEngineMotionControl, ptPubParameter> CreationMode;

//    
RDK::UProperty<NameT, NEngineMotionControl, ptPubParameter> MotionElementClassName;

//      
RDK::UProperty<NameT, NEngineMotionControl, ptPubParameter> ObjectControlInterfaceClassName;

///     
/// 0 -  
/// 1 -      
/// 2 -    c   
RDK::UProperty<int, NEngineMotionControl, ptPubParameter> AdaptiveStructureMode;

//    
// 0 -   
// 1 -   
RDK::UProperty<int, NEngineMotionControl, ptPubParameter> InterneuronPresentMode;

// Link modes (vector)
RDK::UProperty<std::vector<int>, NEngineMotionControl, ptPubParameter> LinkModes;

// Диапазон афферентных нейронов по каналу Ia
RDK::UProperty<double, NEngineMotionControl,ptParameter> IaMin;
RDK::UProperty<double, NEngineMotionControl,ptParameter> IaMax;

// Диапазон афферентных нейронов по каналу Ib
RDK::UProperty<double, NEngineMotionControl,ptParameter> IbMin;
RDK::UProperty<double, NEngineMotionControl,ptParameter> IbMax;

// Диапазон афферентных нейронов по каналу II
RDK::UProperty<double, NEngineMotionControl,ptParameter> IIMin;
RDK::UProperty<double, NEngineMotionControl,ptParameter> IIMax;

// Диапазон афферентных нейронов по каналу Ic
RDK::UProperty<double, NEngineMotionControl,ptParameter> IcMin;
RDK::UProperty<double, NEngineMotionControl,ptParameter> IcMax;

/// Afferent minimum values
RDK::UProperty<std::vector<double>, NEngineMotionControl, ptPubParameter> AfferentMin;
/// Afferent maximum values
RDK::UProperty<std::vector<double>, NEngineMotionControl, ptPubParameter> AfferentMax;

///   
RDK::UProperty<int, NEngineMotionControl, ptPubParameter> IntervalSeparatorMode;


//    
// 0 -    [Max/(i+1)]  i -  
// 1 -    [Max/N^2]*(i+1)^2  i -  
// 2 -   (3.4)
// 3 -   (3.3) Max/(N-i),  i -  ,  N -  
RDK::UProperty<double, NEngineMotionControl, ptPubParameter> PacGain;

RDK::UProperty<double, NEngineMotionControl, ptPubParameter> PacSecretionTC;
RDK::UProperty<double, NEngineMotionControl, ptPubParameter> PacDissociationTC;

//     
// 0 -   
// 1 -    ,     
// MinAfferentRange    *Max-*Min
RDK::UProperty<int, NEngineMotionControl, ptPubParameter> AfferentRangeMode;

//    pac
RDK::UProperty<int, NEngineMotionControl, ptPubParameter> PacRangeMode;

// Процентная величина от соответствующей разницы *Max-*Min
// определяющая минимальную ширину диапазона афферетных нейронов
// изменяяется в интервале [0;1]
//    [0;1]
RDK::UProperty<double, NEngineMotionControl,ptPubState> MinAfferentRange;

/// Текущая амплитуда колебаний
RDK::UProperty<std::vector<double>, NEngineMotionControl,ptPubState> CurrentContourAmplitude;

/// Текущее среднее колебаний
RDK::UProperty<std::vector<double>, NEngineMotionControl,ptPubState> CurrentContourAverage;

/// Текущее время переходного процесса
RDK::UProperty<double, NEngineMotionControl,ptPubState> CurrentTransientTime;

/// Мгновенная скорость вычисленная по среднему положению выбранного контура
RDK::UProperty<double, NEngineMotionControl,ptPubState> InstantAvgSpeed;

/// Текущее состояние переходного процесса
RDK::UProperty<bool, NEngineMotionControl,ptPubState> CurrentTransientState;

/// Destination contour maximum amplitude
RDK::UProperty<std::vector<double>, NEngineMotionControl, ptPubParameter> DestContourMaxAmplitude;

/// Destination contour minimum amplitude
RDK::UProperty<std::vector<double>, NEngineMotionControl, ptPubParameter> DestContourMinAmplitude;

///    
RDK::UProperty<double, NEngineMotionControl, ptPubParameter> DestTransientTime;

///     
///    MotionControl  :
/// 0=II
/// 1=Ia
/// 2=Ib
/// 3=Ic
RDK::UProperty<std::vector<bool>, NEngineMotionControl, ptPubParameter> ActiveContours;

/// Use contour data flag
RDK::UProperty<std::vector<bool>, NEngineMotionControl, ptPubParameter> UseContourData;

///       
RDK::UProperty<double, NEngineMotionControl, ptPubParameter> TransientHistoryTime;

///     ()     
RDK::UProperty<int, NEngineMotionControl, ptPubParameter> TransientObjectIndex;

 ///          ,  
///    ,    
RDK::UProperty<double, NEngineMotionControl, ptPubParameter> TransientAverageThreshold;

//    
RDK::UProperty<string, NEngineMotionControl, ptPubParameter> MCNeuroObjectName;

//     
RDK::UProperty<string, NEngineMotionControl, ptPubParameter> MCAfferentObjectName;

//    PAC
RDK::UProperty<string, NEngineMotionControl, ptPubParameter> PacObjectName;

// Максимальная зафиксированная амплитуда колебаний
RDK::UProperty<std::vector<double>, NEngineMotionControl,ptPubState> MaxContourAmplitude;

//       
// 0 -       
// 1 -      
RDK::UProperty<int, NEngineMotionControl, ptPubParameter> MotoneuronBranchMode;

//    
// 0 -   
// 1 -   
RDK::UProperty<int, NEngineMotionControl, ptPubParameter> RenshowMode;

///    .        :
//
//UProperty<MDMatrix<double>, NEngineMotionControl, ptPubState> Statistic;
UProperty<MDMatrix<double>, NEngineMotionControl, ptPubState> Statistic;

protected: //  
vector<vector<UEPtr<NReceptor> > > receptors;

vector<pair<double,double> > Ia_ranges_pos,Ia_ranges_neg;
vector<pair<double,double> > Ib_ranges_pos,Ib_ranges_neg;
vector<pair<double,double> > II_ranges_pos,II_ranges_neg;
vector<pair<double,double> > Ic_ranges_pos,Ic_ranges_neg;

vector<vector<pair<double,double> > > AfferentRangesPos, AfferentRangesNeg;

vector<NMotionElement *> Motions;

/// Генераторы принудительной коррекции регулятора
UEPtr<NPulseGenerator> InternalGenerator;

vector<vector<double> > History;

vector<double> TransientHistory;

int HistorySize,TransientHistorySize;

double LastAdaptiveTime;

double TransientStartTime;

double OldTransientAverage;

double TempTransientState;

/// Признак, индицирующий, какой режим системы управления сейчас ипользуется
/// 0 - старый режим, в котором число контуров фиксировано при создании
/// 1 - новый режим, в котором число контуров может динамически изменяться
int ControlMode;

/// Временная переменная, отражающая текущий режим внутреннего генератора
/// -1 - не опредлено
/// 0 -
/// 1 -
int InternalGeneratorDirection;

//UStatisticMatrix<double> StatisticDoubleMatrix;

public: // 
// --------------------------
// --------------------------
// Конструкторы и деструкторы
// --------------------------
// --------------------------
NEngineMotionControl(void);
virtual ~NEngineMotionControl(void);

// --------------------------
// --------------------------
// --------------------------
// Вспомогательные методы
// --------------------------
// --------------------------
protected:
//UEPtr<NManipulatorSource> ManipulatorSource1;
//UEPtr<NIntervalSeparator> PosIntervalSeparator;

// Число контуров управления
bool SetNumControlLoops(const int &value);

bool SetIsAfferentLinked(const int &index, const bool &value);

bool GetIsAfferentLinked(const int &index);

// Число управляющих элементов
bool SetNumMotionElements(const int &value);

// Режим формирования сети
bool SetCreationMode(const int &value);

// Режим настройки диапазонов афферентных нейронов
bool SetAfferentRangeMode(const int &value);

// Режим настрйоки диапазонов pac
bool SetPacRangeMode(const int &value);

// Процентная величина от соответствующей разницы *Max-*Min
bool SetMinAfferentRange(const double &value);

// Максимальное усиление управляющего воздействия
bool SetPacGain(const double &value);
bool SetPacSecretionTC(const double &value);
bool SetPacDissociationTC(const double &value);

bool SetMCNeuroObjectName(const string &value);
bool SetMCAfferentObjectName(const string &value);
bool SetPacObjectName(const string &value);
bool SetObjectControlInterfaceClassName(const string &value);


/// Диапазон афферентных нейронов по каналам
bool SetAfferentMin(const std::vector<double> &value);
bool SetAfferentMax(const std::vector<double> &value);

bool SetIntervalSeparatorMode(const int &value);

bool SetMotoneuronBranchMode(const int &value);
bool SetRenshowMode(const int &value);

/// Управление списком активных контуров
bool SetActiveContours(const std::vector<bool> &value);

/// Режим наличия интернейронов
bool SetInterneuronPresentMode(const int &value);

bool SetLinkModes(const std::vector<int> &value);
// --------------------------

// --------------------------
// --------------------------
// Методы управления счетом
// --------------------------
// --------------------------
public:
// Выделяет память для новой чистой копии объекта этого класса
virtual NEngineMotionControl* New(void);
// --------------------------

// --------------------------
// --------------------------
// Вспомогательные методы
// --------------------------
// --------------------------
protected:
// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ADefault(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
//    Reset()   Ready  true
virtual bool ABuild(void);

// Reset computation
virtual bool AReset(void);

// Execute math. computations of current object on current step
virtual bool ACalculate(void);
// --------------------------

// --------------------------
// --------------------------
// Методы управления счетом
// --------------------------
// --------------------------
public:
// Создает объект с желаемой структурой в соответствии с CreationMode
// Если full_recreate == true удаляет все существующие управляющие элементы
// Иначе стремится их сохранить
virtual bool Create(bool full_recreate=true);

/// Удаляет существующую структуру, не трогая вспомогательные компоненты
/// Сохраняет expected_num_motion_elements число управляющих элементов
virtual bool ClearStructure(int expected_num_motion_elements);

/// Алгоритм адаптивной настройки
virtual void AdaptiveTuning(void);

/// Реализация алгоритмов адаптивной настройки
/// current_contour_amplitude - текущая амплитуда по контурам управления
/// use_contour_data - флаги, определающие данные каких контуров можно использовать
/// current_transient_time - текущее время переходного процесса
/// dest_contour_amplitude - желаемая амплитуда по контурам управления
/// dest_transient_time - желаемое время переходного процесса
/// num_motion_elements - расчетное число управляющих элементов
/// control_gain - расчетное усиление сигнала управления
///
virtual void AdaptiveTuningSimple(const std::vector<double> &current_contour_amplitude,
								  const std::vector<bool> &use_contour_data,
								  double current_transient_time,
								  const std::vector<double> &dest_contour_max_amplitude,
								  const std::vector<double> &dest_contour_min_amplitude,
								  double dest_transient_time,
								  int &num_motion_elements,
								  double &control_gain);
// --------------------------
int GetNumControlLoops(void);
// --------------------------
// --------------------------
// Вспомогательные методы
// --------------------------
// --------------------------
protected:
// Вычисляет диапазоны действия афферентнов
// Возвращает число получившихся диапазонов
int CalcAfferentRange(int num_motions, bool cross_ranges, double a_min, double a_max,
			vector<pair<double,double> > &pos_ranges, vector<pair<double,double> > &neg_ranges, int range_mode);

void SetupPacRange(void);

//   -
void AACSetup(UEPtr<UNet> net, double gain_value);

//   
void AdditionalComponentsSetup(UEPtr<UNet> net);

//------------------------------
// Формируем сеть управления новым способом на 2 импульсных нейронах

UNet* CreateNewEngineControl2NeuronsSimplest(bool crosslinks = false, bool crossranges=false);

//  
void NewMotionElementsSetup(UEPtr<UNet> net);

//   -
void NewPACSetup(double pulse_amplitude, double secretion_tc, double dissociaton_tc, double gain_value, bool gain_div_mode);

/// Обновляет параметры постоянных времени Pac
void UpdatePacTCParameters(void);

// Установка стандартных связей
void NewStandardLinksSetup(const string &engine_integrator_name);

public:
// Настройка разделителей интервалов
void NewIntervalSeparatorsSetup(int mode_value, int last_mode_value, double pos_gain_value, double neg_gain_value);

// Настройка разделителей интервалов
void NewIntervalSeparatorsUpdate(int mode_value, int last_mode_value);

// Установка связей разделителей интервалов
void NewIntervalSeparatorLinksSetup(void);

/// Подключает внутренние генераторы к вставочным нейронам нужного числа управляющих элементов
/// direction 0 - налево, direction 1 - направо
void ConnectInternalGenerators(int direction, int num_motion_elements, int control_loop_index);

/// Задает частоту работы внутреннего генератора
void SetInternalGeneratorFrequency(int direction, int num_motion_elements, int control_loop_index, double value);
// --------------------------

vector<NMotionElement *> GetMotion(void);

};

}
#endif
