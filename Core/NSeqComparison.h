/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   November, 2023
/// @brief  The component for sequences comparison

#ifndef NSeqComparisonH
#define NSeqComparisonH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGeneratorMulti.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NSeqComparison: public UNet
{
public: // Общедоступные свойства

 /// Имя класса нейрона
 ULProperty<std::string, NSeqComparison, ptPubParameter> NeuronClassName;

 /// Имя класса генератора последовательности импульсов
 ULProperty<std::string, NSeqComparison, ptPubParameter> MultiGeneratorClassName;


 /// Длительность импульсов (с)
 ULProperty<double, NSeqComparison, ptPubParameter> PulseLength;

 /// Амплитуда импульсов
 ULProperty<double, NSeqComparison, ptPubParameter> Amplitude;

 /// Частота генерации (Гц) всех компонентов
 ULProperty<double, NSeqComparison, ptPubParameter> Frequency;

 /// Используемый порог низкопороговой зоны нейрона
 ULProperty<double, NSeqComparison, ptPubParameter> LTZThreshold;


 /// Размерность спайковых образов в одномерном случае
 ULProperty<int, NSeqComparison, ptPubParameter> PulseCount;

 /// Количество объектов для сравнения
 ULProperty<int, NSeqComparison, ptPubParameter> NumClasses;

 /// Задержки, задающие проверенные спайковые образы классов
 ULProperty<MDMatrix<double>, NSeqComparison, ptPubParameter> DelaysKFCls;

 /// Задержки, задающие тестовые спайковые образы классов
 ULProperty<MDMatrix<double>, NSeqComparison, ptPubParameter> DelaysFrCls;


public: // Входы и выходы

/// Выход модели
UPropertyOutputData<MDMatrix<double>, NSeqComparison, ptOutput | ptPubState> Output;


protected: // Данные
 /// Генераторы проверенных спайковых образов классов
 std::vector<UEPtr <NPulseGeneratorMulti>> KFClsSpikes;

 /// Генераторы тестовых спайковых образов классов
 std::vector<UEPtr <NPulseGeneratorMulti>> FrClsSpikes;

 /// Сравнивающий нейрон
 UEPtr <NPulseNeuron> CompNeuron;

 /// Старое значение количества классов объектов, необходимое для перестраивания компонента
 int OldNumClasses;

protected: // Временные переменные


public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NSeqComparison(void);
virtual ~NSeqComparison(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NSeqComparison* New(void);
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

/// Установка имени класса генератора последовательности импульсов
bool SetMultiGeneratorClassName(const std::string &value);

/// Установка длительности импульсов
bool SetPulseLength(const double &value);

/// Установка амплитуды импульсов
bool SetAmplitude(const double &value);

/// Установка частоты генерации (Гц) всех компонентов
bool SetFrequency(const double &value);

/// Установка используемого порога низкопороговой зоны нейрона
bool SetLTZThreshold(const double &value);

/// Установка размерности спайковых образов
bool SetPulseCount(const int &value);

/// Установка количества объектов для сравнения
bool SetNumClasses(const int &value);

/// Установка задержек, задающих проверенные спайковые образы классов
bool SetDelaysKFCls(const MDMatrix<double> &value);

/// Установка задержек, задающих тестовые спайковые образы классов
bool SetDelaysFrCls(const MDMatrix<double> &value);

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
