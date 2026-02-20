// ===========================================================
// Version:        1.0.0
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

#ifndef NNavMousePrimitiveH
#define NNavMousePrimitiveH
#include "../../../Rdk/Deploy/Include/rdk.h"

//---------------------------------------------------------------------------
namespace NMSDK {

using namespace RDK;

/* *************************************************************************** */
class RDK_LIB_TYPE NNavMousePrimitive: public UNet
{
public: //  
/// Размер поля перемещения "мыши", от начала (0) до упора (PoleSize)
/// пусть будет в см, так как робот планируется мелкий
UProperty<double, NNavMousePrimitive, ptPubParameter> PoleSize;

/// Размер "мыши" ("радиус" так сказать)
UProperty<double, NNavMousePrimitive, ptPubParameter> MouseSize;

/// Размер вибриссы "мыши", упираясь которыми в "стенки" будет выдавать сигнал
/// направлены вперед и назад по умолчанию
UProperty<double, NNavMousePrimitive, ptPubParameter> VibrissSize;

/// Скорость движения за 1 "большой" такт (т.е. за Freqiency)
UProperty<double, NNavMousePrimitive, ptPubParameter> Velocity;

/// Частота, с которой идут импульсы для обработки логики
UProperty<double, NNavMousePrimitive, ptPubParameter> Frequency;

/// Дополнительная задержка, с которой идет передача сигналов
/// (0.1 по дефолту для учета переходного процесса в нейроне)
UProperty<double, NNavMousePrimitive, ptPubParameter> Delay;

/// Задержка, с которой выдается сигнал боли
/// (0.1 - середина стандартного периода классификации 0.2 по умолчанию)
UProperty<double, NNavMousePrimitive, ptPubParameter> PainDelay;

///     -  
/// Стандартный параметр для генерации - длина импульса
/// по умолчанию - 0.001
UProperty<double, NNavMousePrimitive, ptPubParameter> PulseLength;

/// Использовать внешний вход на актуаторы (по умолчанию - да)
UProperty<bool, NNavMousePrimitive, ptPubParameter> UseExternalInput;

///Амплитуда импульсов
UProperty<double, NNavMousePrimitive, ptPubParameter> Amplitude;

/// По возможности - простой вход управления движением:
/// 0 - стоим на месте
/// 1 - едем вперед со скоростью Velocity
/// -1 - едем назад со скоростью Velocity
/// для начала - просто свойством
///   -  
UProperty<int, NNavMousePrimitive, ptPubParameter> MotionControlSimple;

public: //   
/// Входной сигнал
UProperty<MDMatrix<double>, NNavMousePrimitive, ptInput | ptPubState> Input;

/// Аналогичные команды, только вход обрабатывается как спайковый за период
/// если спайк только один, то
UProperty<MDMatrix<double>, NNavMousePrimitive, ptInput | ptPubState> MotionControlSpikeForward;
UProperty<MDMatrix<double>, NNavMousePrimitive, ptInput | ptPubState> MotionControlSpikeBackward;
UProperty<MDMatrix<double>, NNavMousePrimitive, ptInput | ptPubState> MotionControlSpikeStop;

/// Состояние движения после обработки времен
///  0 - стоим на месте
///  1 - едем вперед со скоростью Velocity
/// -1 - едем назад со скоростью Velocity
UProperty<int, NNavMousePrimitive, ptPubState> MotionControlState;

///  -         
UProperty<int, NNavMousePrimitive, ptPubState> PainState;

/// Положение "мыши" в "коридоре"
UProperty<double, NNavMousePrimitive, ptPubState> MousePosition;

/// Выходой сигнал
UProperty<MDMatrix<double>, NNavMousePrimitive, ptOutput | ptPubState> Output;

/// Выходой сигнал вибрисс
/// Выдает сразу импульсы с частотой Frequency, задержкой от начала Delay
///  и основной задержкой в зависимости от того, какое расстояние до препятствия
UProperty<MDMatrix<double>, NNavMousePrimitive, ptOutput | ptPubState> VibrissOutput;

/// Выходой сигнал боли
UProperty<MDMatrix<double>, NNavMousePrimitive, ptOutput | ptPubState> PainOutput;

protected: //  

//Задержки для вибрисс, чтобы генератор сообразить аналогично мультигенератору
std::vector<double> VibrDelays;

//Расчетные задержки входов для принятия решений о том, какой победил
double ForwardSpikeTime;
double BackwardSpikeTime;
double StopSpikeTime;

//Отсчеты итераций, аналогично тому как это работает при обучении нейрона
double start_iter_time;

double OldFrequency;
double ResetTime;

//Счетчики импульсов для выходов
std::vector<int> vibriss_counters;
int pain_counter;

// Класс рисования
// TODO: (когда-то потом надо будет запилить визуализацию)
UGraphics Graphics;

public: // 
// --------------------------
// --------------------------
// Конструкторы и деструкторы
// --------------------------
// --------------------------
NNavMousePrimitive(void);
virtual ~NNavMousePrimitive(void);
// --------------------------

// --------------------------
// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
// --------------------------
virtual NNavMousePrimitive* New(void);
// --------------------------

protected:
// --------------------------
//    
// --------------------------

bool SetPoleSize(const double &value);
bool SetVibrissSize(const double &value);
bool SetMouseSize(const double &value);
bool SetVelocity(const double &value);

// --------------------------
// --------------------------
// Методы расчета
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
// --------------------------
virtual bool ADefault(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
//    Reset()   Ready  true
virtual bool ABuild(void);

// Reset computation
virtual bool AReset(void);

// Метод обновления состояния компонента. Именно здесь:
// 1) обрабатывается логика на основе собранных входов
// 2) применяется скорость, обновляется позиция и
// 3) определяется, какие данные будут транслироваться на выходе следующий период
virtual bool UpdateState(void);

// Execute math. computations of current object on current step
virtual bool ACalculate(void);
// --------------------------
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
