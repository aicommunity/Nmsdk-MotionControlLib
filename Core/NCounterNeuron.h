/* *********************************************************** 
@Copyright Alexander V. Bakhshiev, 2012.
E-mail:        alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: RDK

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NCounterNeuronH
#define NCounterNeuronH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../Nmsdk-PulseLib/Core/NPulseMembrane.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuronCommon.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NCounterNeuron: public NPulseNeuronCommon
{
public: // Общедоступные свойства

 /// Имя класса участка мембраны
 ULProperty<std::string, NCounterNeuron, ptPubParameter> MembraneClassName;

 /// Имя класса генераторной зоны
 ULProperty<std::string, NCounterNeuron, ptPubParameter> LTZoneClassName;

 /// Имя класса источника, задающего сигнал для возбуждающего ионного механизма
 ULProperty<std::string, NCounterNeuron, ptPubParameter> ExcGeneratorClassName;

 /// Имя класса источника, задающего сигнал для тормозного ионного механизма
 ULProperty<std::string, NCounterNeuron, ptPubParameter> InhGeneratorClassName;


 /// До скольки будет считать счётчик
 /// и в то же время число участков мембраны, которое будет у тела нейрона
 ULProperty<int, NCounterNeuron, ptPubParameter> MaxCount;

 /// До скольки уже досчитал счётчик
 /// и в то же время число участков мембраны тела нейрона уже подключено
 ULProperty<int, NCounterNeuron, ptPubParameter> CurCount;


public: // Входы и выходы
 /// Входной сигнал на компонент
 UPropertyInputData<MDMatrix<double>, NCounterNeuron, ptInput | ptPubState> Input;

protected: // Данные

/// Участки мембраны нейрона
vector<UEPtr<NPulseMembrane> > Soma;

/// Старое значение количества участков мембраны, необходимое для перестраивания компонента
int OldNumSoma;

protected: // Временные переменные
/// Метка, что на входе фиксируется тот же импульс, что и на предыдущем такте
/// За время импульса происходит несколько тактов, и поэтому входное значение > 0
/// на очередном такте не должно воприниматься как новый импульс, если TheSamePulse == true
bool TheSamePulse = false;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NCounterNeuron(void);
virtual ~NCounterNeuron(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NCounterNeuron* New(void);
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

/// Установка имени класса участка мембраны
bool SetMembraneClassName(const std::string &value);

/// Установка имени класса генераторной зоны
bool SetLTZoneClassName(const std::string &value);

/// Установка имени класса источника, задающего сигнал для возбуждающего ионного механизма
bool SetExcGeneratorClassName(const std::string &value);

/// Установка имени класса источника, задающего сигнал для тормозного ионного механизма
bool SetInhGeneratorClassName(const std::string &value);

/// Установка ограничения для счётчика
bool SetMaxCount(const int &value);

/// Установка начала отсчёта для счётчика
bool SetCurCount(const int &value);

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

/// Создание необходимых связей в нейроне для переданной сомы
bool CreateSomaLinks(UEPtr<NPulseMembrane> soma);

// --------------------------
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
