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
#ifndef NTrajectoryElementH
#define NTrajectoryElementH

#include "../../Nmsdk-PulseLib/Core/NAfferentNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {

class RDK_LIB_TYPE NTrajectoryElement: public UNet
{
public: // Параметры
/// Имя класса нейрона
ULProperty<std::string, NTrajectoryElement, ptPubParameter> NeuronClassName;

/// Выходной сигнал на следующий элемент траектории
UPropertyOutputData<MDMatrix<double>, NTrajectoryElement, ptOutput | ptPubState> Output;

///Номер слоя, к которому принадлежит элемент траетории
/// (для использования в MazeMemory, по умолчанию = 0)
ULProperty<int, NTrajectoryElement, ptPubParameter> Layer;

//Input_u_top, Input_u_tcn и Input_y_pcn2 - перемычки от входа блока к синапсам соответсвующих сегментов дендритов (согласно схеме),
//заданы для упрощения покдлючения блока NTrajectoryElement к внешним источникам.
//Сейчас неактивны, т.к. нет возможности в коде подключить Input блока к Input синапса.
//Могут быть полезны, если такая возможность появится - тогда в код необходимо добавить построение связей между
//Input_u_top, Input_u_tcn и Input_y_pcn2 и соответствующими синапсами

/// Входной сигнал с высшего уровня управления (
//UPropertyInputData<MDMatrix<double>,NTrajectoryElement, ptInput | ptPubState> Input_u_top;

/// Входной сигнал c предыдущего элемента траектории
//UPropertyInputData<MDMatrix<double>,NTrajectoryElement, ptInput | ptPubState> Input_u_tcn;

/// Входной сигнал с предыдущего уровня системы управления - PCN2 (блоки NMultiPositionControl),
/// сигнализирует о выполнении текущего элемента траектории
//UPropertyInputData<MDMatrix<double>,NTrajectoryElement, ptInput | ptPubState> Input_y_pcn2;

///Возможные направления движения из данного элемента траектории
/// (возможные действия в данной ситуации)
/// параметр необходим для использования в MazeMemory
std::vector<UEPtr<NTrajectoryElement>> Forwards;

///Последнее выбранное направление движения
/// (возможное действие)
///параметр необходим для использования в MazeMemory
int CurrentForward;

///Обратные связи из данной точки
///параметр необходим для использования в MazeMemory
std::vector<UEPtr<NTrajectoryElement>> Backwards;

///Последняя выбранная обратная связь
///параметр необходим для использования в MazeMemory
int CurrentBackward;

///Все связи из данной точки
///параметр необходим для использования в MazeMemory
std::vector<UEPtr<NTrajectoryElement>> Paths;

///Последняя выбранная связь (прямая или обратная)
///параметр необходим для использования в MazeMemory
int CurrentPath;

///Связь, по которой попали из предыдущего TE в текущий
///параметр необходим для использования в MazeMemory
//int UsedPath;




protected:
///Внутренние нейроны элемента траектории
std::vector<UEPtr<NPulseNeuron>> Neurons;

///Размер сомы нейронов 1 и 2
int SomaSize;

///Размер дендритов нейрона 1
std::vector<int> DendSizes1;

///Размер дендритов нейрона 2
std::vector<int> DendSizes2;





public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NTrajectoryElement(void);
virtual ~NTrajectoryElement(void);
// --------------------------

// --------------------------
// Методы управления параметрами
// --------------------------
/// Имя класса, создающего нейрон
bool SetNeuronClassName(const std::string &value);

///Номер слоя, к которому принадлежит элемент траетории
/// (для использования в MazeMemory, по умолчанию = 0)
bool SetLayer(const int &value);


// --------------------------

// --------------------------
// Системные методы управления объектом

// --------------------------
public:
// Выделяет память для новой чистой копии объекта этого класса
virtual NTrajectoryElement* New(void);
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

};

}
//---------------------------------------------------------------------------
#endif
