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
//#include <string>

namespace NMSDK {

class RDK_LIB_TYPE NMultiPositionControl: public NPositionControlElement
{
protected: // 
RDK::UProperty<std::vector<MDMatrix<double>>,NMultiPositionControl, ptPubInput> PositionControl;
RDK::UProperty<int, NPositionControlElement, ptPubParameter> NumOfPositions;

public: //  
// Входные нейроны, разделённые по нижним уровням
// IN(0,0) IN(0,1), ..., IN(0,M) - нейроны, подключенные к первому элементу нижнего уровня
// IN(1,0) IN(1,1), ..., IN(1,M) - нейроны, подключенные ко второму элементу нижнего уровня
// IN(0,0) IN(0,1), ..., IN(0,M) - ,      
// IN(1,0) IN(1,1), ..., IN(1,M) - ,      
vector<vector<NNet*> > InputNeuronsByContours;
// Выходные нейроны, разделённые по нижним уровням
// Ctrl(0,0) Ctrl(0,1), ..., Ctrl(0,M) - нейроны, подключенные к первому элементу нижнего уровня
// Ctrl(1,0) Ctrl(1,1), ..., Ctrl(1,M) - нейроны, подключенные ко второму элементу нижнего уровня
// Ctrl(0,0) Ctrl(0,1), ..., Ctrl(0,M) - ,      
// Ctrl(1,0) Ctrl(1,1), ..., Ctrl(1,M) - ,      
vector<vector<NNet*> > ControlNeuronsByContours;

// 
//true -    Build   ,   NewPositionControl
//false -    Calculate      NewPositionControl
RDK::UProperty<bool, NMultiPositionControl, ptPubParameter> BuildSolo;

//  
//    Multi ( BuildSolo = true),
//   NNewPositionControls
//       ,    
RDK::UProperty<int, NMultiPositionControl, ptPubParameter> PCsNum;

// InputNeurons  ControlNeurons
//    Multi ( BuildSolo = true)
//        
RDK::UProperty<int, NMultiPositionControl, ptPubParameter> InputsNum;

//     
//    Multi ( BuildSolo = true)
//true -    InputsNum  PCsNum, false - 
//!         ,   !
RDK::UProperty<bool, NMultiPositionControl, ptPubParameter> IsNeedToRebuild;

//   PostInput
//   PostInput  ,
//    Multi ( BuildSolo = true)
RDK::UProperty<double, NMultiPositionControl, ptPubParameter> PostInputThreshold;

//   PostInput  PreControl 
//    (true    MazeMemory,  false)
RDK::UProperty<bool, NMultiPositionControl, ptPubParameter> PrebuildStructure;

protected:
std::vector<UEPtr<NPositionControlElement>> PositionControlElement;

public: // 
// --------------------------
// --------------------------
// Конструкторы и деструкторы
// --------------------------
// --------------------------
NMultiPositionControl(void);
virtual ~NMultiPositionControl(void);
// --------------------------

// ---------------------
//   
// ---------------------
bool SetBuildSolo(const bool &value);
bool SetInputsNum(const int &value);
bool SetPCsNum(const int &value);
bool SetFeaturesNum(const int &value);
bool SetIsNeedToRebuild(const bool &value);
bool SetPostInputTreshold(const double &value);
bool SetPrebuildStructure(const bool &value);

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
virtual NMultiPositionControl* New(void);
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
public:
bool CreateNeurons(void);
bool LinkGenerators(const bool &value);
bool LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link, bool is_sim);

bool CreateNeuronsSolo(void);
bool LinkNeuronsWithNorm(vector <NNet*> start, vector <NNet*> finish);
};


}

#endif



