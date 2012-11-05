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

#include "../../Kernel/NBios.h"
#include "../ReceiverLib/NReceptor.h"

namespace NMSDK {

class NEngineMotionControl: public NANet
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

// Режим настройки диапазонов афферентных нейронов
// 0 - диапазоны одинаковой длины
// 1 - диапазоны с удвоением длины, и минимальной шириной в процентах
// MinAfferentRange от соответствующей разницы *Max-*Min
RDK::ULProperty<int, NEngineMotionControl> AfferentRangeMode;

// Процентная величина от соответствующей разницы *Max-*Min
// определяющая минимальную ширину диапазона афферетных нейронов
// изменяяется в интервале [0;1]
RDK::ULProperty<double, NEngineMotionControl,ptPubState> MinAfferentRange;

protected: // Временные переменные
vector<vector<UEPtr<NReceptor> > > receptors;

vector<pair<double,double> > Ia_ranges_pos,Ia_ranges_neg;
vector<pair<double,double> > Ib_ranges_pos,Ib_ranges_neg;
vector<pair<double,double> > II_ranges_pos,II_ranges_neg;

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

// Процентная величина от соответствующей разницы *Max-*Min
bool SetMinAfferentRange(double value);
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

// Настройка рецепторов
void MotionElementsSetup(UEPtr<NAContainer> net, int inp_mode, int out_mode, double exp_coeff, double receptor_max_output, double receptor_gain, int real_ranges);

// Настройка преобразователя импульс-аналог
void PACSetup(UEPtr<NAContainer> net,
		double pulse_amplitude, double secretion_tc, double dissociaton_tc, double gain_value, bool gain_div_mode=true);

// Настройка преобразователя аналог-аналог
void AACSetup(UEPtr<NAContainer> net, double gain_value);

// Настройка разделителей интервалов
void IntervalSeparatorsSetup(UEPtr<NAContainer> net, int mode_value, double pos_gain_value, double neg_gain_value, bool II=true, bool Ia=true, bool Ib=true);

// Настройка разделителей интервалов
void IntervalSeparatorsUpdate(UEPtr<NAContainer> net, int mode_value);

// Задание вспомогательных компонент
void AdditionalComponentsSetup(UEPtr<NAContainer> net);

// Установка стандартных связей
void StandardLinksSetup(UEPtr<NANet> net, const string &engine_integrator_name);

// Установка связей разделителей интервалов
void IntervalSeparatorLinksSetup(UEPtr<NANet> net, bool II=true, bool Ia=true, bool Ib=true);

// Формируем сеть управления двигателем с разделением информационного потока с датчиков
// на две полосы по знаку
NANet* CreateEngineControlSignumAfferent(void);

// Формируем сеть управления двигателем c разделением информационного потока с датчиков
// на полосы по амплитуде
NANet* CreateEngineControlRangeAfferent(bool crosslinks=false, bool crossranges=true);

// Формируем сеть управления на нейронах с непрерывной генераторной функцией нейронов
NANet* CreateEngineControlContinuesNeuronsSimple(bool crossranges=true);

// Формируем сеть управления на 2 импульсных нейронах
NANet* CreateEngineControl2NeuronsSimplest(void);
// --------------------------

};

}
#endif
