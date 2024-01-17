/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   December, 2023
/// @brief  The component for determining the direction of movement of the control object

#ifndef NActuatorSignalsH
#define NActuatorSignalsH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../Nmsdk-PulseLib/Core/NPulseDelay.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NActuatorSignals: public UNet
{
public: // Общедоступные свойства

 /// Имя класса нейронов
 ULProperty<std::string, NActuatorSignals, ptPubParameter> NeuronClassName;

 /// Имя класса генераторов импульсов
 ULProperty<std::string, NActuatorSignals, ptPubParameter> GeneratorClassName;

 /// Имя класса блока задержки сигнала
 ULProperty<std::string, NActuatorSignals, ptPubParameter> DelayClassName;

 /// Длительность импульсов (с)
 ULProperty<double, NActuatorSignals, ptPubParameter> PulseLength;

 /// Амплитуда импульсов
 ULProperty<double, NActuatorSignals, ptPubParameter> Amplitude;

 /// Частота генерации (Гц) всех компонентов
 ULProperty<double, NActuatorSignals, ptPubParameter> Frequency;

 /// Задержка для левого двигателя
 ULProperty<double, NActuatorSignals, ptPubParameter> LeftDelay;

 /// Задержка для правого двигателя
 ULProperty<double, NActuatorSignals, ptPubParameter> RightDelay;

 /// Задержка синхронизирующего импульса, должен быть аналогом вращения назад
 ULProperty<double, NActuatorSignals, ptPubParameter> Sinchro1Delay;

 /// Задержка синхронизирующего импульса, должен быть аналогом вращения вперед
 ULProperty<double, NActuatorSignals, ptPubParameter> Sinchro2Delay;

 /// Признак необходимости транзита сигнала на двигатели от внешнего источника
 ULProperty<bool, NActuatorSignals, ptPubParameter> UseTransitEngineSignal;

 /// Признак необходимости транзита сигнала на синхронизирующие генераторы от внешнего источника
 ULProperty<bool, NActuatorSignals, ptPubParameter> UseTransitSinchroSignal;


public: // Входы и выходы

/// Выход модели, сигнализирующий об остановке
UPropertyOutputData<MDMatrix<double>, NActuatorSignals, ptOutput | ptPubState> Stay;

/// Выход модели, сигнализирующий о движении вперед
UPropertyOutputData<MDMatrix<double>, NActuatorSignals, ptOutput | ptPubState> Forward;

/// Выход модели, сигнализирующий о движении назад
UPropertyOutputData<MDMatrix<double>, NActuatorSignals, ptOutput | ptPubState> Back;

/// Выход модели, сигнализирующий о повороте налево
UPropertyOutputData<MDMatrix<double>, NActuatorSignals, ptOutput | ptPubState> Left;

/// Выход модели, сигнализирующий о повороте направо
UPropertyOutputData<MDMatrix<double>, NActuatorSignals, ptOutput | ptPubState> Right;


protected: // Данные

 /// Источник сигнала левого двигателя
 UEPtr <NPulseGeneratorTransit> LeftEngine;

 /// Источник сигнала правого двигателя
 UEPtr <NPulseGeneratorTransit> RightEngine;

 /// Источник синхронизирующего сигнала 1 - аналога вращения назад
 UEPtr <NPulseGeneratorTransit> Sinchro1;

 /// Источник синхронизирующего сигнала 2 - аналога вращения вперед
 UEPtr <NPulseGeneratorTransit> Sinchro2;

 /// Генератор для нейрона NOTNeuron
 UEPtr <NPulseGeneratorTransit> NOTGenerator;


 /// Блок задержки
 UEPtr <NPulseDelay> Delay1To2;

 /// Нейрон ИЛИ
 UEPtr <NPulseNeuron> ORNeuron;

 /// Нейрон НЕ
 UEPtr <NPulseNeuron> NOTNeuron;


 /// Нейрон, определяющий движение вперёд
 UEPtr <NPulseNeuron> IsForwardNeuron;

 /// Нейрон, определяющий поворот налево
 UEPtr <NPulseNeuron> IsLeftNeuron;

 /// Нейрон, определяющий поворот направо
 UEPtr <NPulseNeuron> IsRightNeuron;

 /// Нейрон, определяющий движение назад
 UEPtr <NPulseNeuron> IsBackNeuron;

 /// Выходной нейрон, сигнализирующий об остановке
 UEPtr <NPulseNeuron> StayNeuron;

 /// Выходной нейрон, сигнализирующий о движении вперёд
 UEPtr <NPulseNeuron> ForwardNeuron;

 /// Выходной нейрон, сигнализирующий о повороте налево
 UEPtr <NPulseNeuron> LeftNeuron;

 /// Выходной нейрон, сигнализирующий о повороте направо
 UEPtr <NPulseNeuron> RightNeuron;

 /// Выходной нейрон, сигнализирующий о движении назад
 UEPtr <NPulseNeuron> BackNeuron;


protected: // Временные переменные


public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NActuatorSignals(void);
virtual ~NActuatorSignals(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NActuatorSignals* New(void);
// --------------------------

protected:
 // --------------------------
 // Скрытые методы управления компонентами
 // --------------------------

 /// Выполняет завершающие пользовательские действия
 /// при добавлении дочернего компонента в этот объект
 // Метод будет вызван только если comp был успешно добавлен в список компонент
 virtual bool AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer = 0);

 /// Выполняет предварительные пользовательские действия
 /// при удалении дочернего компонента из этого объекта
 // Метод будет вызван только если comp существует в списке компонент
 virtual bool ADelComponent(UEPtr<UContainer> comp);

 // --------------------------

protected:
// --------------------------
// Методы управления общедоступными свойствами
// --------------------------

/// Установка имени класса нейрона
bool SetNeuronClassName(const std::string &value);

/// Установка имени класса генераторов импульсов
bool SetGeneratorClassName(const std::string &value);

/// Установка имени класса блока задержки сигнала
bool SetDelayClassName(const std::string &value);

/// Установка длительности импульсов
bool SetPulseLength(const double &value);

/// Установка амплитуды импульсов
bool SetAmplitude(const double &value);

/// Установка частоты генерации (Гц) всех компонентов
bool SetFrequency(const double &value);

/// Установка задержки для левого двигателя
bool SetLeftDelay(const double &value);

/// Установка задержки для правого двигателя
bool SetRightDelay(const double &value);

/// Установка задержки синхронизирующего импульса, должен быть аналогом вращения назад
bool SetSinchro1Delay(const double &value);

/// Установка задержки синхронизирующего импульса, должен быть аналогом вращения вперед
bool SetSinchro2Delay(const double &value);

/// Установка необходимости транзита сигнала на двигатели от внешнего источника
bool SetUseTransitEngineSignal(const bool &value);

/// Установка необходимости транзита сигнала на синхронизирующие генераторы от внешнего источника
bool SetUseTransitSinchroSignal(const bool &value);

// --------------------------

// --------------------------
// Computation methods
// --------------------------
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
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
