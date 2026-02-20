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
#ifndef NMotionElementH
#define NMotionElementH

#include "../../Nmsdk-PulseLib/Core/NAfferentNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {

class RDK_LIB_TYPE NMotionElement: public UNet
{
public: // 
//   
RDK::UProperty<int, NMotionElement, ptPubParameter> NumControlLoops;

//     
RDK::UProperty<std::vector<int>, NMotionElement, ptPubParameter> EnableControlLoopFlags;

//     
// 0 -   ( )
// 1 -   
// 2 -    (L-R) +  . (L-L)
// 3 - .   (L-L) +   (L-R)
// 4 -  1,      
RDK::UProperty<std::vector<int>, NMotionElement, ptPubParameter> LinkModes;

//    
// ,    ,     
//  
// 0 -   
// 1 -   
RDK::UProperty<int, NMotionElement, ptPubParameter> InterneuronPresentMode;

//    
// 0 -   
// 1 -   
RDK::UProperty<int, NMotionElement, ptPubParameter> RenshowMode;
//    
// 0 -   
// 1 -   
RDK::UProperty<int, NMotionElement, ptPubParameter> PacemakerMode;

//    
// 0 -   
// 1 -      ()
// 2 -       
RDK::UProperty<int, NMotionElement, ptPubParameter> RecurrentInhibitionMode;

//       
// 0 -       ,    
// (  N,       ).
// 1 -       
RDK::UProperty<int, NMotionElement, ptPubParameter> RecurrentInhibitionBranchMode;

//       
// 0 -       
// 1 -      
RDK::UProperty<int, NMotionElement, ptPubParameter> MotoneuronBranchMode;

//      
// 0 -          
//   .
// 1 -           
//     
RDK::UProperty<int, NMotionElement, ptPubParameter> ExternalControlMode;

//    
RDK::UProperty<string, NMotionElement, ptPubParameter> NeuroObjectName;
//     
RDK::UProperty<string, NMotionElement, ptPubParameter> AfferentObjectName;

public: //  

// Флаг первоначальной инициализации количества контуров управления
bool  isNumControlLoopsInitialized;

public: //  
// Указатели на афферентные нейроны
UCPointer<NAfferentNeuron, NMotionElement> Afferents;

// Указатели на генераторы внешнего управления
UCPointer<NPulseGenerator, NMotionElement> ExternalControlGenerators;

// Указатели на результирующие нейроны (мотонейроны)
UCPointer<NPulseNeuron, NMotionElement> Motoneurons;

string netclassname;
string neuron_class_name;

public: // 
// --------------------------
// --------------------------
// Конструкторы и деструкторы
// --------------------------
// --------------------------
NMotionElement(void);
virtual ~NMotionElement(void);
// --------------------------

// --------------------------
// --------------------------
// Методы управления параметрами
// --------------------------
// --------------------------
bool SetNumControlLoops(const int &value);
bool SetEnableControlLoopFlags(const std::vector<int> &value);
bool SetRenshowMode(const int &value);
bool SetPacemakerMode(const int &value);
bool SetLinkModes(const std::vector<int> &value);
bool SetInterneuronPresentMode(const int &value);
bool SetRecurrentInhibitionMode(const int &value);
bool SetRecurrentInhibitionBranchMode(const int &value);
bool SetMotoneuronBranchMode(const int &value);
bool SetExternalControlMode(const int &value);
bool SetNeuroObjectName(const string &value);
bool SetAfferentObjectName(const string &value);
// --------------------------

// --------------------------
// --------------------------

// --------------------------
public:
// Выделяет память для новой чистой копии объекта этого класса
virtual NMotionElement* New(void);
// --------------------------

// --------------------------
// --------------------------
// Proctected computation methods
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

// Reset computation
virtual bool AReset(void);

// Execute math. computations of current object on current step
virtual bool ACalculate(void);

// Создает структуру в соответствии с текущими значениями параметров
// Если структура существует, то пытается модифицировать ее с минимальными изменениями
void CreateStructure(void);

// Создает внутренние связи в соответствии с текущими значениями параметров
void CreateInternalLinks(void);

// Сохраняет и восстанавливает внешние связи
void BackupExternalLinks(void);
void RestoreExternalLinks(void);
// --------------------------
// --------------------------
// Методы создания элементов СУ
// --------------------------
// Создание пары мотонейронов
// --------------------------
bool CreateMotoneurons();

// Создание связки афферентных нейронов
bool CreateAfferents();

// Создание интернейронов
bool CreateInterneurons();

// Создание связей между афферентными нейронами и мотонейронами
// 0 - прямая связь (без интернейронов)
// 1 - связь через интернейроны
// 2 - связь через интернейроны(L-R) + прямая связь(L-L)
// 3 - связь через интернейроны(L-L) + прямая связь(L-R)
bool LinkMotoneurons();

// Создание связей с клетками Реншоу
bool LinkRenshow();

// Создание связей с пейсмейкерными нейронами
bool LinkPM();

// Создание связи между двумя нейронами
// 0 - возбуждающая связь
// 1 - тормозная связь
bool LinkNeuron(const string &source, const string &sink, int mode, const string &branch = "Soma1");
};


bool RDK_LIB_TYPE CreateNeuronBranchLink(UEPtr<UNet> net,const string &source,
	const string &target_head, const string &target_tail);

}
//---------------------------------------------------------------------------
#endif
