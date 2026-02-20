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

#include "NEngineMotionControl.h"
#include "NMotionElement.h"
#include "NPositionControlElement.h"

namespace NMSDK {

class NEngineMotionControl;
class RDK_LIB_TYPE NNewPositionControlElement: public NPositionControlElement
{
public: // 
RDK::UProperty<MDMatrix<double>,NNewPositionControlElement, ptPubParameter> MotionControl;
RDK::UProperty<bool, NNewPositionControlElement, ptPubParameter> SimControl;
//RDK::UProperty<bool, NNewPositionControlElement, ptPubState> RememberState;
//RDK::UProperty<MDMatrix<double>, NNewPositionControlElement, ptPubState> CurrentPosition;
//RDK::UProperty<MDMatrix<double>, NNewPositionControlElement> TargetPosition;
//RDK::UProperty<string, NNewPositionControlElement, ptPubParameter> InputNeuronType;
//RDK::UProperty<string, NNewPositionControlElement, ptPubParameter> ControlNeuronType;
//RDK::UProperty<bool, NNewPositionControlElement, ptPubParameter> ExternalControl;
//RDK::UProperty<MDMatrix<double>, NNewPositionControlElement, ptPubState> Delta;

public: //  
// Нейроны обученные распознавать положения левых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > LeftInputNeurons;

// Нейроны обученные распознавать положения правых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > RightInputNeurons;

// Нейроны обученные распознавать положения левых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > LeftControlNeurons;

// Нейроны обученные распознавать положения правых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > RightControlNeurons;

// Нейроны обученные распознавать положения левых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > LeftPostInputNeurons;

// Нейроны обученные распознавать положения правых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > RightPostInputNeurons;

// Нейроны обученные распознавать положения левых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > LeftPreControlNeurons;

// Нейроны обученные распознавать положения правых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > RightPreControlNeurons;


//vector<UNet*> Generators;

// Генераторы левых контуров в порядке
// G(0,0) G(0,1), ..., G(0,M) - генераторы всех УЭ 0 контура
// G(1,0) G(1,1), ..., G(1,M) - генераторы всех УЭ 1 контура
// G(0,0) G(0,1), ..., G(0,M) -    0 
// G(1,0) G(1,1), ..., G(1,M) -    1 
vector<vector<UNet*> > LeftGenerators;

// Генераторы правых контуров в порядке
// G(0,0) G(0,1), ..., G(0,M) - генераторы всех УЭ 0 контура
// G(1,0) G(1,1), ..., G(1,M) - генераторы всех УЭ 1 контура
// G(0,0) G(0,1), ..., G(0,M) -    0 
// G(1,0) G(1,1), ..., G(1,M) -    1 
vector<vector<UNet*> > RightGenerators;

//
//    MultiPositionControl
UProperty<MDMatrix<double>, NNewPositionControlElement, ptPubOutput> Output;

UEPtr<NEngineMotionControl> MotionControlElement;

public: // 
// --------------------------
// --------------------------
// Конструкторы и деструкторы
// --------------------------
// --------------------------
NNewPositionControlElement(void);
virtual ~NNewPositionControlElement(void);
// --------------------------

// ---------------------
// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------
bool SetInputNeuronType(const string &value);
bool SetControlNeuronType(const string &value);
// ---------------------

// ---------------------
// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
// ---------------------
// ---------------------

// --------------------------
// --------------------------
virtual NNewPositionControlElement* New(void);
// --------------------------

// --------------------------
// --------------------------
// Скрытые методы управления счетом 
// --------------------------
// --------------------------
protected:
// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ADefault(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
//    Reset()   Ready  true
virtual bool ABuild(void);

// Сброс процесса счета без потери настроек
virtual bool AReset(void);

// Выполняет расчет этого объекта
virtual bool ACalculate(void);
// --------------------------
bool CreateNeurons(void);
bool CreateExternalControlElements(void);
bool LinkNeurons(vector <NNet*> start, vector <NNet*> finish);
bool LinkNeuronsNeg(vector <NNet*> start, vector <NNet*> finish);
bool UnlinkNeurons(vector <NNet*> start, vector <NNet*> finish);
virtual bool LinkGenerators(const bool &value);
bool LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link, bool is_sim);
bool LinkNegative(vector <NNet*> start, vector <NNet*> finish);
public:
vector<NNet*> GetInputNeurons(void);
vector<NNet*> GetControlNeurons(void);
};

}

#endif



