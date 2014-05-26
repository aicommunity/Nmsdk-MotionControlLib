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

#include "..\..\RDK\Core\Engine\UNet.h"
#include "NEngineMotionControl.h"
#include "NMotionElement.h"

namespace NMSDK {

//class NEngineMotionControl;
class NPositionControlElement: public UNet
{
public: // Свойства
RDK::UPropertyInput<NEngineMotionControl,NPositionControlElement> MotionControl;
RDK::ULProperty<MDMatrix<double>, NPositionControlElement, ptPubState> CurrentPosition;
RDK::ULProperty<MDMatrix<double>, NPositionControlElement> TargetPosition;
RDK::ULProperty<string, NPositionControlElement> InputNeuronType;
RDK::ULProperty<string, NPositionControlElement> ControlNeuronType;
RDK::ULProperty<bool, NPositionControlElement> ExternalControl;
RDK::ULProperty<bool, NPositionControlElement> RememberState;
RDK::ULProperty<MDMatrix<double>, NPositionControlElement, ptPubState> Delta;

protected: // Переменные состояния
vector<NNet*> InputNeurons;
vector<NNet*> ControlNeurons;
vector<NNet*> PreControlNeurons;
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
bool CreateNeurons(void);
bool CreateExternalControlElements(void);
bool LinkNeurons(vector <NNet*> start, vector <NNet*> finish);
bool UnlinkNeurons(vector <NNet*> start, vector <NNet*> finish);
bool LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link);
public:
vector<NNet*> GetInputNeurons(void);
vector<NNet*> GetControlNeurons(void);
};

}

#endif



