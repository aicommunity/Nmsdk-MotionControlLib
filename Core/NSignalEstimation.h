/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   December, 2023
/// @brief  The component for signal zoning

#ifndef NSignalEstimationH
#define NSignalEstimationH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NSignalEstimation: public UNet
{
public: // Общедоступные свойства

 /// Имя класса нейронов
 ULProperty<std::string, NSignalEstimation, ptPubParameter> NeuronClassName;

 /// Имя класса генераторов импульсов
 ULProperty<std::string, NSignalEstimation, ptPubParameter> GeneratorClassName;


 /// Длительность импульсов (с)
 ULProperty<double, NSignalEstimation, ptPubParameter> PulseLength;

 /// Амплитуда импульсов
 ULProperty<double, NSignalEstimation, ptPubParameter> Amplitude;

 /// Частота генерации (Гц) всех компонентов
 ULProperty<double, NSignalEstimation, ptPubParameter> Frequency;

 /// Используемый порог низкопороговой зоны нейронов
 ULProperty<double, NSignalEstimation, ptPubParameter> LTZThreshold;


 /// Задержка сигнала, который будет зонироваться
 ULProperty<double, NSignalEstimation, ptPubParameter> DelaySignal;

 /// Задержка синхронизирующего импульса
 ULProperty<double, NSignalEstimation, ptPubParameter> DelaySinchro;

 /// Количество областей, на которые будет зонироваться сигнал
 ULProperty<int, NSignalEstimation, ptPubParameter> NumZones;

 /// Верхние ограничения для каждой из областей зонирования
 ULProperty<MDMatrix<double>, NSignalEstimation, ptPubParameter> UpperLimitsOfZones;

 /// Признак необходимости транзита зонируемого сигнала от внешнего источника
 ULProperty<bool, NSignalEstimation, ptPubParameter> UseTransitSignal;

 /// Признак необходимости транзита сигнала на синхронизирующий генератор от внешнего источника
 ULProperty<bool, NSignalEstimation, ptPubParameter> UseTransitSinchro;


public: // Входы и выходы

 /// Входной синхронизирующий сигнал на компонент
 UPropertyInputData<MDMatrix<double>, NSignalEstimation, ptInput | ptPubState> Sinchro;

 /// Входной сигнал на компонент, который будет зонироваться
 UPropertyInputData<MDMatrix<double>, NSignalEstimation, ptInput | ptPubState> Input;


protected: // Данные
 /// Генератор зонируемого сигнала
 UEPtr <NPulseGeneratorTransit> SignalGen;

 /// Генератор синхронизирующего сигнала
 UEPtr <NPulseGeneratorTransit> SinchroGen;

 ///Зонирующие нейроны
 std::vector<UEPtr <NPulseNeuron>> ZoneNeurons;

 /// Старое значение количества зон, необходимое для перестраивания компонента
 int OldNumZones;

protected: // Временные переменные


public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NSignalEstimation(void);
virtual ~NSignalEstimation(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NSignalEstimation* New(void);
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

/// Установка имени класса нейронов
bool SetNeuronClassName(const std::string &value);

/// Установка имени класса генераторов импульсов
bool SetGeneratorClassName(const std::string &value);

/// Установка длительности импульсов
bool SetPulseLength(const double &value);

/// Установка амплитуды импульсов
bool SetAmplitude(const double &value);

/// Установка частоты генерации (Гц) всех компонентов
bool SetFrequency(const double &value);

/// Установка используемого порога низкопороговой зоны нейронов
bool SetLTZThreshold(const double &value);

/// Установка задержки сигнала, который будет зонироваться
bool SetDelaySignal(const double &value);

/// Установка задержки синхронизирующего импульса
bool SetDelaySinchro(const double &value);

/// Установка количества областей, на которые будет зонироваться сигнал
bool SetNumZones(const int &value);

/// Установка верхних ограничений для каждой из областей зонирования
bool SetUpperLimitsOfZones(const MDMatrix<double> &value);

/// Установка необходимости транзита зонируемого сигнала от внешнего источника
bool SetUseTransitSignal(const bool &value);

/// Установка необходимости транзита сигнала на синхронизирующий генератор от внешнего источника
bool SetUseTransitSinchro(const bool &value);

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
