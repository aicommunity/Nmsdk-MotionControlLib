/* ***********************************************************

E-mail:        
Url:           

This file is part of the project: 

File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NPositionControlElement_H
#define NPositionControlElement_H

#include "NEngineMotionControl.h"
#include "NMotionElement.h"

namespace NMSDK {

//class NEngineMotionControl;
class RDK_LIB_TYPE NPositionControlElement: public UNet
{
public: // Свойства
RDK::ULProperty<MDMatrix<double>, NPositionControlElement, ptPubState> CurrentPosition;
RDK::ULProperty<MDMatrix<double>, NPositionControlElement> TargetPosition;
RDK::ULProperty<string, NPositionControlElement> InputNeuronType;
RDK::ULProperty<string, NPositionControlElement> ControlNeuronType;
RDK::ULProperty<bool, NPositionControlElement> ExternalControl;
RDK::ULProperty<bool, NPositionControlElement, ptPubState> RememberState;
RDK::ULProperty<MDMatrix<double>, NPositionControlElement, ptPubState> Delta;

public: // Переменные состояния
vector<NNet*> InputNeurons;
vector<NNet*> ControlNeurons;
vector<NNet*> PreControlNeurons;

vector<NNet*> PostInputNeurons;

vector<UNet*> Generators;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NPositionControlElement(void);
virtual ~NPositionControlElement(void);
// --------------------------

// ---------------------
// Методы управления параметрами
// ---------------------
bool SetInputNeuronType(const string &value);
bool SetControlNeuronType(const string &value);
bool SetExternalControl(const bool &value);
// ---------------------

// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NPositionControlElement* New(void);
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
bool CreateExternalControlElements(void);
bool LinkNeurons(vector <NNet*> start, vector <NNet*> finish);
bool UnlinkNeurons(vector <NNet*> start, vector <NNet*> finish);
virtual bool LinkGenerators(const bool &value);
bool LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link);
bool LinkNegative(vector <NNet*> start, vector <NNet*> finish);
vector<NNet*> GetInputNeurons(void);
vector<NNet*> GetControlNeurons(void);
};

}

#endif



