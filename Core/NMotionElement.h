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

#include "../../Kernel/NSupport.h"
#include "../PulseLib/NAfferentNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {

class NMotionElement: public UNet
{
public: // Параметры
// Число контуров управления
RDK::ULProperty<int, NMotionElement> NumControlLoops;

// Массив флагов включения контуров управления
RDK::ULProperty<std::vector<int>, NMotionElement> EnableControlLoopFlags;

// Массив режимов связи между нейронами
// 0 - прямая связь (без интернейронов)
// 1 - связь через интернейроны
// 2 - связь через интернейроны(L-R) + прямая связь(L-L)
// 3 - связь через интернейроны(L-L) + прямая связь(L-R)
RDK::ULProperty<std::vector<int>, NMotionElement> LinkModes;

// Режим наличия вставочных нейронов
// Внимание, этот режим аналогичен тому, при котором каждый контур имеет
// свой мотонейрон
// 0 - вставочных нейронов нет
// 1 - вставочные нейроны есть
RDK::ULProperty<int, NMotionElement> InterneuronPresentMode;

// Режим наличия клеток Реншоу
// 0 - клеток Реншоу нет
// 1 - клетки Реншоу есть
RDK::ULProperty<int, NMotionElement> RenshowMode;

// Режим наличия возвратного торможения
// 0 - возвратного торможения нет
// 1 - возвратное торможение только на выходном (мотонейроне)
// 2 - возвратное торможение индвидуально на каждом нейроне контура
RDK::ULProperty<int, NMotionElement> RecurrentInhibitionMode;

// Режим ветвления мотонейрона при подключении возвратного торможения
// 0 - возвратный тормозный вход на том же дендрите, что и контур управления
// (если контуров N, то на каждый контур свой тормозный вход).
// 1 - возвратный тормозный вход на отдельном дендрите нейрона
RDK::ULProperty<int, NMotionElement> RecurrentInhibitionBranchMode;

// Режим ветвления мотонейрона при подключении контуров управления
// 0 - все контура подключаются к одному участку мембраны
// 1 - каждый контур имеет свой участок мембраны
RDK::ULProperty<int, NMotionElement> MotoneuronBranchMode;

// Режим подключения управления с верхних уровней
// 0 - управление по контуру подключается соседним синапсом туда же куда идет
// выход с афферента.
// 1 - управление по контуру подключается на отдельный дендрит к тому же нейрону
// который принимает выход с афферента
RDK::ULProperty<int, NMotionElement> ExternalControlMode;

// Имя объекта для нейронов
RDK::ULProperty<string, NMotionElement> NeuroObjectName;
// Имя объекта для афферентных нейронов
RDK::ULProperty<string, NMotionElement> AfferentObjectName;

public: // Переменные состояния

protected: // Временные переменные
// Указатели на афферентные нейроны
UCPointer<NAfferentNeuron, NMotionElement> Afferents;

// Указатели на генераторы внешнего управления
UCPointer<NPulseGenerator, NMotionElement> ExternalControlGenerators;

// Указатели на результирующие нейроны (мотонейроны)
UCPointer<NPulseNeuron, NMotionElement> Motoneurons;

string netclassname;
string neuron_class_name;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NMotionElement(void);
virtual ~NMotionElement(void);
// --------------------------

// --------------------------
// Методы управления параметрами
// --------------------------
bool SetNumControlLoops(const int &value);
bool SetEnableControlLoopFlags(const std::vector<int> &value);
bool SetRenshowMode(const int &value);
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
// Системные методы управления объектом

// --------------------------
public:
// Выделяет память для новой чистой копии объекта этого класса
virtual NMotionElement* New(void);
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

// Создает структуру в соответствии с текущими значениями параметров
// Если структура существует, то пытается модифицировать ее с минимальными изменениями
void CreateStructure(void);

// Создает внутренние связи в соответствии с текущими значениями параметров
void CreateInternalLinks(void);

// Сохраняет и восстанавливает внешние связи
void BackupExternalLinks(void);
void RestoreExternalLinks(void);
// --------------------------
// Методы создания элементов СУ
// --------------------------
// Создание пары мотонейронов
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
bool LinkMotoneurons(const string &afferentL, const string afferentR, int mode);

// Создание связей с клетками Реншоу
bool LinkRenshow();

};

// Формирует СУ двигательной единицей
// Если mode == 0 - формируется полная модель
// 	    mode == 1 - модель без вставочных интернейронов
UEPtr<UNet> CreateMotionElement(UStorage *storage, const string &netclassname, int mode);

// Аналогично, но с развязкой по дендритам
UEPtr<UNet> CreateBranchedMotionElement(UStorage *storage, const string &netclassname,
	const string &neuron_class_name, const string &afferent_neuron_class_name, int mode);

// Формирует простейшую СУ двигательной единицей из двух мотонейронов
UEPtr<UNet> CreateSimplestMotionElement(UStorage *storage, const string &netclassname, int mode);
UEPtr<UNet> CreateSimplestBranchedMotionElement(UStorage *storage, const string &netclassname, int mode, bool use_speed_force=false, bool use_add_contours=false);
UEPtr<UNet> CreateSimplestBranchedMotionElementPM(UStorage *storage, const string &netclassname, int mode, bool use_speed_force=false, bool use_add_contours=false);

bool CreateNeuronBranchLink(UEPtr<UNet> net,const string &source,
	const string &target_head, const string &target_tail);

}
//---------------------------------------------------------------------------
#endif
