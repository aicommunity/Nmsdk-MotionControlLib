/* ***********************************************************

E-mail:        
Url:           

This file is part of the project: 

File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NNewPositionControlElement_H
#define NNewPositionControlElement_H

#include "..\..\RDK\Core\Engine\UNet.h"
#include "NEngineMotionControl.h"
#include "NMotionElement.h"
#include "NPositionControlElement.h"

namespace NMSDK {

//class NEngineMotionControl;
class NNewPositionControlElement: public NPositionControlElement
{
public: // Свойства
RDK::UPropertyInput<NEngineMotionControl,NNewPositionControlElement> MotionControl;
//RDK::ULProperty<MDMatrix<double>, NNewPositionControlElement, ptPubState> CurrentPosition;
//RDK::ULProperty<MDMatrix<double>, NNewPositionControlElement> TargetPosition;
//RDK::ULProperty<string, NNewPositionControlElement> InputNeuronType;
//RDK::ULProperty<string, NNewPositionControlElement> ControlNeuronType;
//RDK::ULProperty<bool, NNewPositionControlElement> ExternalControl;
//RDK::ULProperty<bool, NNewPositionControlElement, ptPubState> RememberState;
//RDK::ULProperty<MDMatrix<double>, NNewPositionControlElement, ptPubState> Delta;

public: // Переменные состояния

// Нейроны обученные распознавать положения левых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
vector<vector<NNet*> > LeftPostInputNeurons;

// Нейроны обученные распознавать положения правых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
vector<vector<NNet*> > RightPostInputNeurons;


vector<UNet*> Generators;

// Генераторы левых контуров в порядке
// G(0,0) G(0,1), ..., G(0,M) - генераторы всех УЭ 0 контура
// G(1,0) G(1,1), ..., G(1,M) - генераторы всех УЭ 1 контура
vector<vector<UNet*> > LeftGenerators;

// Генераторы правых контуров в порядке
// G(0,0) G(0,1), ..., G(0,M) - генераторы всех УЭ 0 контура
// G(1,0) G(1,1), ..., G(1,M) - генераторы всех УЭ 1 контура
vector<vector<UNet*> > RightGenerators;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NNewPositionControlElement(void);
virtual ~NNewPositionControlElement(void);
// --------------------------

// ---------------------
// Методы управления параметрами
// ---------------------
bool SetInputNeuronType(const string &value);
bool SetControlNeuronType(const string &value);
// ---------------------

// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NNewPositionControlElement* New(void);
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
bool CreateNeurons(void);
bool CreateExternalControlElements(void);
bool LinkNeurons(vector <NNet*> start, vector <NNet*> finish);
bool UnlinkNeurons(vector <NNet*> start, vector <NNet*> finish);
bool LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link);
bool LinkNegative(vector <NNet*> start, vector <NNet*> finish);
public:
vector<NNet*> GetInputNeurons(void);
vector<NNet*> GetControlNeurons(void);
};

}

#endif



