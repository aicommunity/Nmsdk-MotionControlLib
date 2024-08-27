// ===========================================================
// Version:        3.0.2
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2002.
E-mail:		alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NMAZEMEMORY_H
#define NMAZEMEMORY_H


#include "NTrajectoryElement.h"
#include "NMultiPositionControl.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"
#include "../../../Rdk/Deploy/Include/rdk.h"
#include <iostream>
#include <fstream>
#include <cstring>

namespace NMSDK {

//using namespace RDK;

// Класс, создающий группу нейронов для моделирования условного рефлекса
class RDK_LIB_TYPE NMazeMemory: public UNet
{
public: // Параметры
/// Количество возможных вариантов выбора в ситуации
/// (возможных напарвлений движения)
ULProperty<int, NMazeMemory, ptPubParameter> OptionsNum;

/// Флаг ситуации с множественным выбором (перекресток, развилка)
ULProperty<bool, NMazeMemory, ptPubParameter> NewSituation;

/// Флаг запоминания ситуации (положения)
ULProperty<bool, NMazeMemory, ptPubParameter> RememberSituation;

/// Количество признаков, описывающих запоминаемую ситуацию
ULProperty<int, NMazeMemory, ptPubParameter> FeaturesNum;

/// Флаг попадания в тупик
ULProperty<bool, NMazeMemory, ptPubParameter> IsDeadlock;




protected:
///Все элементы траектории на схеме
std::vector<UEPtr<NTrajectoryElement>> TrajectoryElements;

/// Активные элементы траектории в схеме
std::vector<UEPtr<NTrajectoryElement>> ActiveTE;

/// Блоки MultiPC (осуществляют запоминание признаков ситуации)
std::vector<UEPtr<NMultiPositionControl>> MultiPCs;

///Индекс текущего элемента траектории в массиве
int CurrentTE;

/// Номер текущего слоя
int CurrentLayer;

///Расстояние между слоями по оси X (нужно для растановки блоков)
double LayerShift;

///Расстояние между блоками по оси Y (нужно для растановки блоков)
double yShift;

///Вес дополнительных связей (на направления помимо основного)
double SideWeight;

///Пройденные элементы траектории
std::vector<UEPtr<NTrajectoryElement>> PassedTEs;



public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NMazeMemory(void);
virtual ~NMazeMemory(void);
// --------------------------

// --------------------------
// Методы упраления параметрами
// --------------------------
/// Количество возможных вариантов (направлений движения)
bool SetOptionsNum(const int &value);

/// Флаг ситуации с множественным выбором (перекресток, развилка)
bool SetNewSituation(const bool &value);

/// Флаг ситуации с множественным выбором (перекресток, развилка)
bool SetRememberSituation(const bool &value);

/// Количество признаков, описывающих запоминаемую ситуацию
bool SetFeaturesNum(const int &value);

/// Флаг попадания в тупик
bool SetIsDeadlock(const bool &value);


// --------------------------

// --------------------------
// Методы доступа к временным переменным
// --------------------------

// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NMazeMemory* New(void);
static UComponent* NewStatic(void);
// --------------------------

// --------------------------
// Скрытые методы управления компонентами
// --------------------------
protected:
// Выполняет завершающие пользовательские действия
// при добавлении дочернего компонента в этот объект
// Метод будет вызван только если comp был
// успешно добавлен в список компонент
virtual bool AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer=0);

// Выполняет предварительные пользовательские действия
// при удалении дочернего компонента из этого объекта
// Метод будет вызван только если comp
// существует в списке компонент
virtual bool ADelComponent(UEPtr<UContainer> comp);
// --------------------------


// --------------------------
// Скрытые методы управления счетом
// --------------------------
protected:

// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ADefault(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool ABuild(void);

// Сброс процесса счета.
virtual bool AReset(void);

// Выполняет расчет этого объекта
virtual bool ACalculate(void);


UEPtr<NTrajectoryElement> CreatePoint(MVector<double,3> coords);
// --------------------------
};

}
#endif
