/* *********************************************************** 
@Copyright Alexander V. Bakhshiev, 2012.
E-mail:        alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: RDK

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NSuppressionUnitH
#define NSuppressionUnitH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NSuppressionUnit: public UNet
{
public: // Общедоступные свойства

 /// Имя класса генератора импульсов
 ULProperty<std::string, NSuppressionUnit, ptPubParameter> PulseGeneratorClassName;

 /// Имя класса нейрона
 ULProperty<std::string, NSuppressionUnit, ptPubParameter> NeuronClassName;

 /// Имя класса синапсов
 ULProperty<std::string, NSuppressionUnit, ptPubParameter> SynapseClassName;


 /// Длительность импульсов (с)
 ULProperty<double, NSuppressionUnit, ptPubParameter> PulseLength;

 /// Амплитуда импульсов
 ULProperty<double, NSuppressionUnit, ptPubParameter> Amplitude;

 /// Частота генерации (Гц) блока подавления сигнала
 ULProperty<double, NSuppressionUnit, ptPubParameter> SuppressionFreq;

 /// Используемый порог низкопороговой зоны нейрона
 ULProperty<double, NSuppressionUnit, ptPubParameter> LTZThreshold;


 /// Момент времени Т1, с которого начинается подавление сигнала
 ULProperty<double, NSuppressionUnit, ptPubParameter> Delay1;

 /// Момент времени Т2, до которого происходит подавление сигнала
 ULProperty<double, NSuppressionUnit, ptPubParameter> Delay2;


 /// Частота генерации (Гц) источника подавляемого сигнала
 ULProperty<double, NSuppressionUnit, ptPubParameter> SourceFreq;

 /// Задержка источника подавляемого сигнала
 ULProperty<double, NSuppressionUnit, ptPubParameter> SourceDelay;

 /// При поднятом флаге используется входной сигнал на компонент,
 /// иначе внутренний источник компонента генерирует сигнал в соответствии с настройками
 ULProperty<bool, NSuppressionUnit, ptPubParameter> TransitInput;

 /// При поднятом флаге компонент не использует источник сигнала и нейрон
 /// Результатом работы компонента является только подавляющий сигнал
 ULProperty<bool, NSuppressionUnit, ptPubParameter> OnlyInhibition;

 /// При поднятом флаге подвление сигнала происходит только на одной итерации
 ULProperty<bool, NSuppressionUnit, ptPubParameter> SingleUse;

public: // Входы и выходы
/// Входной сигнал на компонент
UPropertyInputData<MDMatrix<double>, NSuppressionUnit, ptInput | ptPubState> Input;

/// Выход модели
UPropertyOutputData<MDMatrix<double>, NSuppressionUnit, ptOutput | ptPubState> Output;


protected: // Данные
 /// Генераторы входных импульсов
 UEPtr <NPulseGeneratorTransit> SourceGenerator;

 /// Генераторы задержек
 std::vector<UEPtr<NPulseGeneratorTransit>> DelayGenerators;

 /// Нейрон ИЛИ
 UEPtr <NPulseNeuron> ORNeuron;

 /// Генератор подавляющих импульсов
 UEPtr <NPulseGeneratorTransit> ControlledGenerator;

 /// Нейрон
 UEPtr <NPulseNeuron> Neuron;

protected: // Временные переменные


public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NSuppressionUnit(void);
virtual ~NSuppressionUnit(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NSuppressionUnit* New(void);
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

/// Установка имени класса генераторов импульсов
bool SetPulseGeneratorClassName(const std::string &value);

/// Установка имени класса нейрона
bool SetNeuronClassName(const std::string &value);

/// Установка имени класса синапсов
bool SetSynapseClassName(const std::string &value);

/// Установка частоты импульсов подавляющего генератора
bool SetSuppressionFreq(const double &value);

/// Установка длительности импульсов
bool SetPulseLength(const double &value);

/// Установка амплитуды импульсов
bool SetAmplitude(const double &value);

/// Установка используемого порога низкопороговой зоны нейрона
bool SetLTZThreshold(const double &value);

/// Установка момента времени Т1, с которого начинается подавление сигнала
bool SetDelay1(const double &value);

/// Установка момента времени Т2, до которого происходит подавление сигнала
bool SetDelay2(const double &value);

/// Установка частоты источника подавляемого сигнала (и генераторов задержек)
bool SetSourceFreq(const double &value);

/// Установка задержки источника подавляемого сигнала
bool SetSourceDelay(const double &value);

/// Установка значения флага транзита сигнала внешнего источника
bool SetTransitInput(const bool &value);

/// Установка значения флага использования только подавляющего сигнала
bool SetOnlyInhibition(const bool &value);

/// Установка значения флага единоразового подавления сигнала
bool SetSingleUse(const bool &value);

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
