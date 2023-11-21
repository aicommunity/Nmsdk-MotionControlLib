/* ***********************************************************

E-mail:
Url:           

This file is part of the project: 

File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NMultiPositionControl_H
#define NMultiPositionControl_H

#include "NPositionControlElement.h"

namespace NMSDK {

class RDK_LIB_TYPE NMultiPositionControl: public NPositionControlElement
{
protected: // Свойства
RDK::UPropertyInputC<NPositionControlElement,NMultiPositionControl> PositionControl;
RDK::ULProperty<int, NPositionControlElement> NumOfPositions;

public: // Переменные состояния
// Входные нейроны, разделённые по нижним уровням
// IN(0,0) IN(0,1), ..., IN(0,M) - нейроны, подключенные к первому элементу нижнего уровня
// IN(1,0) IN(1,1), ..., IN(1,M) - нейроны, подключенные ко второму элементу нижнего уровня
vector<vector<NNet*> > InputNeuronsByContours;
// Выходные нейроны, разделённые по нижним уровням
// Ctrl(0,0) Ctrl(0,1), ..., Ctrl(0,M) - нейроны, подключенные к первому элементу нижнего уровня
// Ctrl(1,0) Ctrl(1,1), ..., Ctrl(1,M) - нейроны, подключенные ко второму элементу нижнего уровня
vector<vector<NNet*> > ControlNeuronsByContours;

//Режим сборки
//true - сбока структуры в Build и автономная работа, без подключения NewPositionControl
//false - сбока структуры в Calculate и работа в системе с NewPositionControl
RDK::ULProperty<bool, NMultiPositionControl> BuildSolo;

//Число входов
//используется при автономной работе Multi (при BuildSolo = true),
//и заменяет число подключенных NNewPositionControls
//может быть интерпретировано как число звеньев объекта управления
RDK::ULProperty<int, NMultiPositionControl> PCsNum;

//Число InputNeurons и ControlNeurons
//используется при автономной работе Multi (при BuildSolo = true)
RDK::ULProperty<int, NMultiPositionControl> InputsNum;



public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NMultiPositionControl(void);
virtual ~NMultiPositionControl(void);
// --------------------------

// ---------------------
// Методы управления параметрами
// ---------------------
bool SetBuildSolo(const bool &value);
bool SetInputsNum(const int &value);
bool SetPCsNum(const int &value);
// ---------------------

// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NMultiPositionControl* New(void);
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

// Сброс процесса счета без потери настроек
virtual bool AReset(void);

// Выполняет расчет этого объекта
virtual bool ACalculate(void);
// --------------------------
public:
bool CreateNeurons(void);
bool LinkGenerators(const bool &value);
bool LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link, bool is_sim);

bool CreateNeuronsSolo(void);
};


}

#endif



