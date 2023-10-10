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
#include "../../Nmsdk-SourceLib/Core/NManipulatorSource.h"
#include "../../Nmsdk-SensorLib/Core/NIntervalSeparator.h"
#include "../../Nmsdk-SourceLib/Core/NControlObjectSource.h"


namespace NMSDK {

class RDK_LIB_TYPE NEngineMotionControl: public UNet
{
public: // Публичные свойства
// Число контуров управления
RDK::ULProperty<int, NEngineMotionControl> NumControlLoops;
// Число управляющих элементов
RDK::ULProperty<int, NEngineMotionControl> NumMotionElements;

// Режим формирования сети
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
RDK::ULProperty<int, NEngineMotionControl> CreationMode;

// Имя класса управляющего элемента
RDK::ULProperty<NameT, NEngineMotionControl> MotionElementClassName;

// Имя класса интерфейса с объектом управления
RDK::ULProperty<NameT, NEngineMotionControl> ObjectControlInterfaceClassName;

/// Режим включения адаптивной перестройки структуры
/// 0 - нет адаптивности
/// 1 - адаптивность включена с полным сбрсом управления
/// 2 - включена автоматическая адаптация c полным сбросом управления
RDK::ULProperty<int, NEngineMotionControl> AdaptiveStructureMode;

// Режим наличия вставочных нейронов
// 0 - вставочных нейронов нет
// 1 - вставочные нейроны есть
RDK::ULProperty<int, NEngineMotionControl> InterneuronPresentMode;

// Режим связывания мотонейронов (см. описание в управляющем элементе)
RDK::ULProperty<std::vector<int>, NEngineMotionControl> LinkModes;

// Диапазон афферентных нейронов по каналу Ia
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IaMin;
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IaMax;

// Диапазон афферентных нейронов по каналу Ib
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IbMin;
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IbMax;

// Диапазон афферентных нейронов по каналу II
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IIMin;
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IIMax;

// Диапазон афферентных нейронов по каналу Ic
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IcMin;
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IcMax;

/// Диапазон афферентных нейронов по каналам
RDK::ULProperty<std::vector<double>, NEngineMotionControl> AfferentMin;
RDK::ULProperty<std::vector<double>, NEngineMotionControl> AfferentMax;

/// Режим разбиения сепараторов
RDK::ULProperty<int, NEngineMotionControl> IntervalSeparatorMode;


// Максимальное усиление управляющего воздействия
// 0 - Разбиение на диапазоны [Max/(i+1)] где i - индекс диапазона
// 1 - Разбиение на диапазоны [Max/N^2]*(i+1)^2 где i - индекс диапазона
// 2 - по формуле (3.4)
// 3 - по формуле (3.3) Max/(N-i), где i - индекс диапазона, а N - число диапазонов
RDK::ULProperty<double, NEngineMotionControl> PacGain;

RDK::ULProperty<double, NEngineMotionControl> PacSecretionTC;
RDK::ULProperty<double, NEngineMotionControl> PacDissociationTC;

// Режим настройки диапазонов афферентных нейронов
// 0 - диапазоны одинаковой длины
// 1 - диапазоны с удвоением длины, и минимальной шириной в процентах
// MinAfferentRange от соответствующей разницы *Max-*Min
RDK::ULProperty<int, NEngineMotionControl> AfferentRangeMode;

// Режим настрйоки диапазонов pac
RDK::ULProperty<int, NEngineMotionControl> PacRangeMode;

// Процентная величина от соответствующей разницы *Max-*Min
// определяющая минимальную ширину диапазона афферетных нейронов
// изменяяется в интервале [0;1]
RDK::ULProperty<double, NEngineMotionControl,ptPubState> MinAfferentRange;

/// Текущая амплитуда колебаний
RDK::ULProperty<std::vector<double>, NEngineMotionControl,ptPubState> CurrentContourAmplitude;

/// Текущее среднее колебаний
RDK::ULProperty<std::vector<double>, NEngineMotionControl,ptPubState> CurrentContourAverage;

/// Текущее время переходного процесса
RDK::ULProperty<double, NEngineMotionControl,ptPubState> CurrentTransientTime;

/// Мгновенная скорость вычисленная по среднему положению выбранного контура
RDK::ULProperty<double, NEngineMotionControl,ptPubState> InstantAvgSpeed;

/// Текущее состояние переходного процесса
RDK::ULProperty<bool, NEngineMotionControl,ptPubState> CurrentTransientState;

/// Маскимально допустимая амплитуда колебаний
RDK::ULProperty<std::vector<double>, NEngineMotionControl> DestContourMaxAmplitude;

/// Минимально допустимая амплитуда колебаний
RDK::ULProperty<std::vector<double>, NEngineMotionControl> DestContourMinAmplitude;

/// Желаемое время переходного процесса
RDK::ULProperty<double, NEngineMotionControl> DestTransientTime;

/// Используемые в работе контуры управления
/// В случае старого MotionControl порядок таков:
/// 0=II
/// 1=Ia
/// 2=Ib
/// 3=Ic
RDK::ULProperty<std::vector<bool>, NEngineMotionControl> ActiveContours;

/// Используемые в адаптации контуры управления
RDK::ULProperty<std::vector<bool>, NEngineMotionControl> UseContourData;

/// Интервал времени на котором анализируем переходный процесс
RDK::ULProperty<double, NEngineMotionControl> TransientHistoryTime;

/// Индекс выхода источника сигнала (объекта) по которому анализируем переходный процесс
RDK::ULProperty<int, NEngineMotionControl> TransientObjectIndex;

 /// Максимально допустимая скорость изменения среднего значения сигнала за интервал времени, по которой
/// принимаем решение о том, что переходный процесс завершен
RDK::ULProperty<double, NEngineMotionControl> TransientAverageThreshold;

// Имя объекта для нейронов
RDK::ULProperty<string, NEngineMotionControl> MCNeuroObjectName;

// Имя объекта для афферентных нейронов
RDK::ULProperty<string, NEngineMotionControl> MCAfferentObjectName;

// Имя объекта для PAC
RDK::ULProperty<string, NEngineMotionControl> PacObjectName;

// Максимальная зафиксированная амплитуда колебаний
RDK::ULProperty<std::vector<double>, NEngineMotionControl,ptPubState> MaxContourAmplitude;

// Режим ветвления мотонейрона при подключении контуров управления
// 0 - все контура подключаются к одному участку мембраны
// 1 - каждый контур имеет свой участок мембраны
RDK::ULProperty<int, NEngineMotionControl> MotoneuronBranchMode;

// Режим наличия клеток Реншоу
// 0 - клеток Реншоу нет
// 1 - клетки Реншоу есть
RDK::ULProperty<int, NEngineMotionControl> RenshowMode;

/// Выход для сбора статистики. В каждой строке содержатся данные в следующем порядке:
//
//UPropertyOutputData<MDMatrix<double>, NEngineMotionControl, ptPubState> Statistic;
UPropertyOutputData<MDMatrix<double>, NEngineMotionControl> Statistic;

protected: // Временные переменные
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

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NEngineMotionControl(void);
virtual ~NEngineMotionControl(void);

// --------------------------
// --------------------------
// Методы управления параметрами
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
// Системные методы управления объектом
// --------------------------
public:
// Выделяет память для новой чистой копии объекта этого класса
virtual NEngineMotionControl* New(void);
// --------------------------

// --------------------------
// Proctected computation methods
// --------------------------
protected:
// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ADefault(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool ABuild(void);

// Reset computation
virtual bool AReset(void);

// Execute math. computations of current object on current step
virtual bool ACalculate(void);
// --------------------------

// --------------------------
// Методы управления счетом
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
// Вспомогательные методы
// --------------------------
protected:
// Вычисляет диапазоны действия афферентнов
// Возвращает число получившихся диапазонов
int CalcAfferentRange(int num_motions, bool cross_ranges, double a_min, double a_max,
			vector<pair<double,double> > &pos_ranges, vector<pair<double,double> > &neg_ranges, int range_mode);

void SetupPacRange(void);

// Настройка преобразователя аналог-аналог
void AACSetup(UEPtr<UContainer> net, double gain_value);

// Задание вспомогательных компонент
void AdditionalComponentsSetup(UEPtr<UContainer> net);

//------------------------------
//Новый способ формирования сети

// Формируем сеть управления новым способом на 2 импульсных нейронах
UNet* CreateNewEngineControl2NeuronsSimplest(bool crosslinks = false, bool crossranges=false);

// Настройка рецепторов
void NewMotionElementsSetup(UEPtr<UContainer> net, int inp_mode, int out_mode, double exp_coeff, double receptor_max_output, double receptor_gain, int real_ranges);

// Настройка преобразователя импульс-аналог
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
