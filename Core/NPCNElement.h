//---------------------------------------------------------------------------

#ifndef NPCNElementH
#define NPCNElementH
//---------------------------------------------------------------------------
#include "NEngineMotionControl.h"
#include "NMotionElement.h"
#include "NPositionControlElement.h"

namespace NMSDK {

//class NEngineMotionControl;
class RDK_LIB_TYPE NPCNElement: public NPositionControlElement
{
public: // Свойства
RDK::UPropertyInput<NEngineMotionControl,NPCNElement> MotionControl;
RDK::ULProperty<bool, NPCNElement> SimControl;

public: // Переменные состояния

vector<UNet*> Generators;
// Нейроны обученные распознавать положения левых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
vector<vector<NNet*> > LeftInputNeurons;

// Нейроны обученные распознавать положения правых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
vector<vector<NNet*> > RightInputNeurons;

// Нейроны обученные распознавать положения левых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
vector<vector<NNet*> > LeftControlNeurons;

// Нейроны обученные распознавать положения правых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
vector<vector<NNet*> > RightControlNeurons;

// Нейроны обученные распознавать положения левых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
vector<vector<NNet*> > LeftAfferentNeurons;

// Нейроны обученные распознавать положения правых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
vector<vector<NNet*> > RightAfferentNeurons;

// Нейроны обученные распознавать положения левых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
vector<vector<NNet*> > LeftPostAfferentNeurons;

// Нейроны обученные распознавать положения правых контуров в порядке
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - нейроны всех УЭ 0 контура
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - нейроны всех УЭ 1 контура
vector<vector<NNet*> > RightPostAfferentNeurons;

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
NPCNElement(void);
virtual ~NPCNElement(void);
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
virtual NPCNElement* New(void);
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



