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

#include "../../Kernel/NSupport.h"
#include "../PulseLib/NReceptor.h"

namespace NMSDK {

class NEngineMotionControl: public UNet
{
public: // Публичные свойства
// Число управляющих элементов
RDK::ULProperty<size_t, NEngineMotionControl> NumMotionElements;

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

// Диапазон афферентных нейронов по каналу Ia
RDK::ULProperty<real, NEngineMotionControl> IaMin;
RDK::ULProperty<real, NEngineMotionControl> IaMax;

// Диапазон афферентных нейронов по каналу Ib
RDK::ULProperty<real, NEngineMotionControl> IbMin;
RDK::ULProperty<real, NEngineMotionControl> IbMax;

// Диапазон афферентных нейронов по каналу II
RDK::ULProperty<real, NEngineMotionControl> IIMin;
RDK::ULProperty<real, NEngineMotionControl> IIMax;

// Диапазон афферентных нейронов по каналу Ic
RDK::ULProperty<real, NEngineMotionControl> IcMin;
RDK::ULProperty<real, NEngineMotionControl> IcMax;

// Максимальное усиление управляющего воздействия
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


protected: // Временные переменные
vector<vector<UEPtr<NReceptor> > > receptors;

vector<pair<double,double> > Ia_ranges_pos,Ia_ranges_neg;
vector<pair<double,double> > Ib_ranges_pos,Ib_ranges_neg;
vector<pair<double,double> > II_ranges_pos,II_ranges_neg;
vector<pair<double,double> > Ic_ranges_pos,Ic_ranges_neg;

vector<NNet*> Motions;


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
// Число управляющих элементов
bool SetNumMotionElements(size_t value);

// Режим формирования сети
bool SetCreationMode(int value);

// Режим настройки диапазонов афферентных нейронов
bool SetAfferentRangeMode(int value);

// Режим настрйоки диапазонов pac
bool SetPacRangeMode(int value);

// Процентная величина от соответствующей разницы *Max-*Min
bool SetMinAfferentRange(double value);

// Максимальное усиление управляющего воздействия
bool SetPacGain(double value);
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
virtual bool Create(void);
// --------------------------

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
// --------------------------

};

}
#endif
