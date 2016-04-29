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

#include "../../Nmsdk-BasicLib/Core/NSupport.h"
#include "../../Nmsdk-PulseLib/Core/NReceptor.h"
#include "../../Nmsdk-BasicLib/Core/NNet.h"
#include "../../Nmsdk-SourceLib/Core/NPulseGenerator.h"

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

// Вектор диапазонов афферентных нейронов
//RDK::ULProperty<std::vector<double>, NEngineMotionControl> IMin;
//RDK::ULProperty<std::vector<double>, NEngineMotionControl> IMax;

// Диапазон афферентных нейронов по каналу Ia
RDK::ULProperty<double, NEngineMotionControl> IaMin;
RDK::ULProperty<double, NEngineMotionControl> IaMax;

// Диапазон афферентных нейронов по каналу Ib
RDK::ULProperty<double, NEngineMotionControl> IbMin;
RDK::ULProperty<double, NEngineMotionControl> IbMax;

// Диапазон афферентных нейронов по каналу II
RDK::ULProperty<double, NEngineMotionControl> IIMin;
RDK::ULProperty<double, NEngineMotionControl> IIMax;

// Диапазон афферентных нейронов по каналу Ic
RDK::ULProperty<double, NEngineMotionControl> IcMin;
RDK::ULProperty<double, NEngineMotionControl> IcMax;

/// Диапазон афферентных нейронов по каналам
RDK::ULProperty<std::vector<double>, NEngineMotionControl> AfferentMin;
RDK::ULProperty<std::vector<double>, NEngineMotionControl> AfferentMax;



// Максимальное усиление управляющего воздействия
// 0 - Разбиение на диапазоны [Max/(i+1)] где i - индекс диапазона
// 1 - Разбиение на диапазоны [Max/(i+1)] где i - индекс диапазона
// 2 -
// 3 - Max/(N-i), где i - индекс диапазона, а N - число диапазонов
RDK::ULProperty<double, NEngineMotionControl> PacGain;

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

/// Текущее состояние переходного процесса
RDK::ULProperty<bool, NEngineMotionControl,ptPubState> CurrentTransientState;

/// Маскимально допустимая амплитуда колебаний
RDK::ULProperty<std::vector<double>, NEngineMotionControl> DestContourMaxAmplitude;

/// Минимально допустимая амплитуда колебаний
RDK::ULProperty<std::vector<double>, NEngineMotionControl> DestContourMinAmplitude;

/// Желаемое время переходного процесса
RDK::ULProperty<double, NEngineMotionControl> DestTransientTime;

/// Используемые в работе контуры управления
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

protected: // Временные переменные
vector<vector<UEPtr<NReceptor> > > receptors;

vector<pair<double,double> > Ia_ranges_pos,Ia_ranges_neg;
vector<pair<double,double> > Ib_ranges_pos,Ib_ranges_neg;
vector<pair<double,double> > II_ranges_pos,II_ranges_neg;
vector<pair<double,double> > Ic_ranges_pos,Ic_ranges_neg;

vector<vector<pair<double,double> > > AfferentRangesPos, AfferentRangesNeg;

vector<NNet*> Motions;

/// Генераторы принудительной коррекции регулятора
UEPtr<NPulseGenerator> InternalGenerator;

vector<vector<double> > History;

vector<double> TransientHistory;

//vector<bool> isAfferentLinked;

int HistorySize,TransientHistorySize;

double LastAdaptiveTime;

double TransientStartTime;

double OldTransientAverage;

double TempTransientState;


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

bool SetMCNeuroObjectName(const string &value);
bool SetMCAfferentObjectName(const string &value);
bool SetPacObjectName(const string &value);
bool SetObjectControlInterfaceClassName(const string &value);


/// Диапазон афферентных нейронов по каналам
bool SetAfferentMin(const std::vector<double> &value);
bool SetAfferentMax(const std::vector<double> &value);

/// Управление списком активных контуров
bool SetActiveContours(const std::vector<bool> &value);

/// Режим наличия интернейронов
bool SetInterneuronPresentMode(const int &value);
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

// Настройка рецепторов
void MotionElementsSetup(UEPtr<UContainer> net, int inp_mode, int out_mode, double exp_coeff, double receptor_max_output, double receptor_gain, int real_ranges);

// Настройка преобразователя импульс-аналог
void PACSetup(UEPtr<UContainer> net,
		double pulse_amplitude, double secretion_tc, double dissociaton_tc, double gain_value, bool gain_div_mode=true);

// Настройка преобразователя аналог-аналог
void AACSetup(UEPtr<UContainer> net, double gain_value);

// Настройка разделителей интервалов
void IntervalSeparatorsSetup(UEPtr<UContainer> net, int mode_value, double pos_gain_value, double neg_gain_value, bool II=true, bool Ia=true, bool Ib=true, bool Ic=false);

// Настройка разделителей интервалов
void IntervalSeparatorsUpdate(UEPtr<UContainer> net, int mode_value);

// Задание вспомогательных компонент
void AdditionalComponentsSetup(UEPtr<UContainer> net);

// Установка стандартных связей
void StandardLinksSetup(UEPtr<UNet> net, const string &engine_integrator_name);

// Установка связей разделителей интервалов
void IntervalSeparatorLinksSetup(UEPtr<UNet> net, bool II=true, bool Ia=true, bool Ib=true, bool Ic=false);

// Формируем сеть управления двигателем с разделением информационного потока с датчиков
// на две полосы по знаку
UNet* CreateEngineControlSignumAfferent(void);

// Формируем сеть управления двигателем c разделением информационного потока с датчиков
// на полосы по амплитуде
UNet* CreateEngineControlRangeAfferent(bool crosslinks=false, bool crossranges=true);

// Формируем сеть управления на нейронах с непрерывной генераторной функцией нейронов
UNet* CreateEngineControlContinuesNeuronsSimple(bool crossranges=true);

// Формируем сеть управления на 2 импульсных нейронах
UNet* CreateEngineControl2NeuronsSimplest(bool use_speed_force=false, bool use_add_contours=false);

//------------------------------
//Новый способ формирования сети

// Формируем сеть управления новым способом на 2 импульсных нейронах
UNet* CreateNewEngineControl2NeuronsSimplest(bool use_speed_force);

// Настройка рецепторов
void NewMotionElementsSetup(UEPtr<UContainer> net, int inp_mode, int out_mode, double exp_coeff, double receptor_max_output, double receptor_gain, int real_ranges);

// Настройка преобразователя импульс-аналог
void NewPACSetup(double pulse_amplitude, double secretion_tc, double dissociaton_tc, double gain_value, bool gain_div_mode);

// Установка стандартных связей
void NewStandardLinksSetup(const string &engine_integrator_name);

public:
// Настройка разделителей интервалов
void NewIntervalSeparatorsSetup(int mode_value, double pos_gain_value, double neg_gain_value);

// Настройка разделителей интервалов
void NewIntervalSeparatorsUpdate(int mode_value);

// Установка связей разделителей интервалов
void NewIntervalSeparatorLinksSetup(void);

/// Подключает внутренние генераторы к вставочным нейронам нужного числа управляющих эдементов
/// direction 0 - налево, direction 1 - направо
void ConnectInternalGenerators(int direction, int num_motion_elements, int control_loop_index);

/// Задает частоту работы внутреннего генератора
void SetInternalGeneratorFrequency(int control_loop_index, double value);
// --------------------------

vector<NNet*> GetMotion(void);

};

}
#endif
