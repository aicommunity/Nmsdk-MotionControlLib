/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   November, 2023
/// @brief  The component for a detection of an objects in restricted area of image

#ifndef NObjInAreaH
#define NObjInAreaH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-MotionControlLib/Core/NSuppressionUnit.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGeneratorMulti.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NObjInArea: public UNet
{
public: // Общедоступные свойства

 /// Имя класса нейрона
 ULProperty<std::string, NObjInArea, ptPubParameter> NeuronClassName;

 /// Длительность импульсов (с)
 ULProperty<double, NObjInArea, ptPubParameter> PulseLength;

 /// Амплитуда импульсов
 ULProperty<double, NObjInArea, ptPubParameter> Amplitude;

 /// Частота генерации (Гц) всех компонентов
 ULProperty<double, NObjInArea, ptPubParameter> Frequency;

 /// Частота генерации (Гц) блоков подавления сигнала и возбуждающего генератора
 ULProperty<double, NObjInArea, ptPubParameter> HighFreq;


 /// Момент времени t=t0, с которого начинается подавление области кадра с левой стороны
 ULProperty<double, NObjInArea, ptPubParameter> Delay1_1;

 /// Момент времени t=t1, до которого происходит подавление области кадра с левой стороны
 ULProperty<double, NObjInArea, ptPubParameter> Delay1_2;

 /// Момент времени t=t2, с которого начинается подавление области кадра с правой стороны
 ULProperty<double, NObjInArea, ptPubParameter> Delay2_1;

 /// Момент времени t=T, до которого происходит подавление области кадра с правой стороны
 ULProperty<double, NObjInArea, ptPubParameter> Delay2_2;


 /// Размерность спайковых образов в одномерном случае
 ULProperty<int, NObjInArea, ptPubParameter> PulseCount;

 /// Количество объектов для распознавания
 ULProperty<int, NObjInArea, ptPubParameter> NumObj;

 /// Задержки, задающие спайковый образы классов
 ULProperty<MDMatrix<double>, NObjInArea, ptPubParameter> Delays_ClsSpikeFr;


public: // Входы и выходы
/// Входные сигнал класса 1
//std::vector<UPropertyInputData<MDMatrix<double>, NObjInArea, ptInput | ptPubState>> InputDelaysCls;

/// Выход модели
UPropertyOutputData<MDMatrix<double>, NObjInArea, ptOutput | ptPubState> Output;


protected: // Данные
 /// Генераторы спайковых образов классов
 std::vector<UEPtr <NPulseGeneratorMulti>> ClsSpikeFr;

 /// Старое значение количества классов объектов, необходимое для перестраивания компонента
 int OldNumObj;

 /// Блок подавления 1
 UEPtr <NSuppressionUnit> SuppressUnit1;

 /// Блок подавления 2
 UEPtr <NSuppressionUnit> SuppressUnit2;

 /// Нейрон ИЛИ
 UEPtr <NPulseNeuron> ORNeuron;

 /// Нейрон И
 UEPtr <NPulseNeuron> ANDNeuron;

 /// Решающий нейрон
 UEPtr <NPulseNeuron> DecidingNeuron;

 /// Возбуждающий генератор
 UEPtr <NPulseGeneratorTransit> ExcitatoryGen;

 // Флаг для изменения входных связей ExcitatoryGen
 bool Relinked;


protected: // Временные переменные


public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NObjInArea(void);
virtual ~NObjInArea(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NObjInArea* New(void);
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

/// Установка длительности импульсов
bool SetPulseLength(const double &value);

/// Установка амплитуды импульсов
bool SetAmplitude(const double &value);

/// Установка частоты генерации (Гц) всех компонентов
bool SetFrequency(const double &value);

/// Установка частоты импульсов подавляющих и возбуждающего генераторов
bool SetHighFreq(const double &value);

/// Установка момента времени t=t0, с которого начинается подавление области кадра с левой стороны
bool SetDelay1_1(const double &value);

/// Установка момента времени t=t1, до которого происходит подавление области кадра с левой стороны
bool SetDelay1_2(const double &value);

/// Установка момента времени t=t2, с которого начинается подавление области кадра с правой стороны
bool SetDelay2_1(const double &value);

/// Установка момента времени t=T, до которого происходит подавление области кадра с правой стороны
bool SetDelay2_2(const double &value);

/// Установка размерности спайковых образов
bool SetPulseCount(const int &value);

/// Установка количества объектов для распознавания
bool SetNumObj(const int &value);

/// Установка задержек, задающих спайковый образы классов
bool SetDelays_ClsSpikeFr(const MDMatrix<double> &value);

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
