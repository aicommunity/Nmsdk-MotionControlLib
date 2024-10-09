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
#include "../../Nmsdk-PulseLib/Core/NNeuronTrainer.h"
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
/// Флаг ситуации с множественным выбором (перекресток, развилка)
ULProperty<bool, NMazeMemory, ptPubParameter> Situation;

/// Вектор возможных вариантов действий в ситуации (направлений движения)
/// Кол-во элементов = кол-ву возможных вариантов,
/// значения элеменов вектора:
/// 1 - есть возможность совершить это действие, 0 - нет возможности
/// [0] - поворот направо, [1] - поворот налево, [2] - ехать прямо, [3] - стоп
ULProperty<std::vector<int>,NMazeMemory, ptPubParameter> InputActions;

///Тип нейронов действия
RDK::ULProperty<string, NMazeMemory, ptPubParameter> ActionNeuronsType;

/// Количество признаков, описывающих запоминаемую ситуацию
ULProperty<int, NMazeMemory, ptPubParameter> FeaturesNum;

/// Флаг завершения работы (все возможные варианты исследованы)
ULProperty<bool, NMazeMemory, ptPubParameter> IsDone;

///Вектор параметров ситуации (сейчас = {x,y,alpha}, где alpha - ориентация робота
ULProperty<MDMatrix<double>, NMazeMemory, ptPubParameter> SituationCoords;

protected:
///Нейроны действий
std::vector<UEPtr<NPulseNeuron>> ActionNeurons;

///Все элементы траектории на схеме
std::vector<UEPtr<NTrajectoryElement>> TrajectoryElements;

/// Активные PostInput нейроны в схеме
std::vector<UEPtr<NTrajectoryElement>> ActivePIs;

/// Блоки MultiPC (осуществляют запоминание признаков ситуации)
std::vector<UEPtr<NMultiPositionControl>> MultiPCs;

///Индекс текущего элемента траектории в массиве
int CurrentTE;

/// Номер текущего слоя
int CurrentLayer;

///Расстояние по умолчанию между слоями по оси X (нужно для растановки блоков)
double LayerShift;

///Расстояние по умолчанию между блоками по оси Y (нужно для растановки блоков)
double yShift;

///Вес по умолчанию дополнительных связей (на направления помимо основного)
double SideWeight;

///Пройденные элементы траектории
std::vector<UEPtr<NTrajectoryElement>> PassedTEs;

///Текущий элемент траектории
UEPtr<NTrajectoryElement> BaseTE;

///MultiPC, соответствующий текущему элементу траектории
UEPtr<NMultiPositionControl> BaseMPC;

///Предыдущий элемент траектории
UEPtr<NTrajectoryElement> PrevTE;


MDMatrix<double> check_pattern;


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
/// Флаг ситуации с множественным выбором (перекресток, развилка)
bool SetSituation(const bool &value);

/// Вектор возможных вариантов действий в ситуации (направлений движения)
/// Кол-во элементов = кол-ву возможных вариантов,
/// значения элеменов вектора:
/// 1 - есть возможность совершить это действие, 0 - нет возможности
/// [0] - поворот направо, [1] - поворот налево, [2] - ехать прямо, [3] - стоп
bool SetInputActions(const std::vector<int> &value);

///Тип нейронов действия
bool SetActionNeuronsType(const string &value);

/// Количество признаков, описывающих запоминаемую ситуацию
bool SetFeaturesNum(const int &value);

/// Флаг завершения работы (все возможные варианты исследованы)
bool SetIsDone(const bool &value);

///Вектор параметров ситуации (сейчас = {x,y,alpha}, где alpha - ориентация робота
bool SetSituationCoords(const MDMatrix<double> &value);

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

//Создание новой точки маршрута (варианта действия), т.е.
//построение связки TrajectoryElement+MultiPC
UEPtr<NTrajectoryElement> CreatePoint(MVector<double,3> coords);

//Слияние совпадающих элементов траектории
//Принимает на вход порядковый номер ТЕ в векторе TrajectoryElements,
//которому соответствует активный PostInput нейрон
//т.е. номер TE, с которым совпадает текущая ситуация
bool MergingTEs(int active_num);

//Проверяет, есть ли активные PostInput нейроны в сети
std::vector<UEPtr<NTrajectoryElement>> CheckActivePIs();

//Проверяет, есть ли активные элементы траектории среди возможных вариантов действий
bool CheckActiveForwards(UEPtr<NTrajectoryElement> t_element);

//Вес связи, по которой попали в эту точку, w = 0,2
bool LastUsedLink();

//Обработка новой ситуации (развилки)
bool ProcessOptions();

//Обработка тупика
bool DeadlockProcessing();

// --------------------------
};

}
#endif
