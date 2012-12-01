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

#include "../../Kernel/NBios.h"
#include "../PulseLib/NAfferentNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {

class NMotionElement: public NANet
{
public: // Параметры
// Число контуров управления
RDK::ULProperty<int, NMotionElement> NumControlLoops;

// Массив флагов включения контуров управления
RDK::ULProperty<std::vector<int>, NMotionElement> EnableControlLoopFlags;

// Режим наличия вставочных нейронов
// Внимание, этот режим аналогичен тому, при котором каждый контур имеет
// свой мотонейрон
// 0 - вставочных нейронов нет
// 1 - вставочные нейроны есть
RDK::ULProperty<int, NMotionElement> InterneuronPresentMode;

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


public: // Переменные состояния

protected: // Временные переменные
// Указатели на афферентные нейроны
UCPointer<NAfferentNeuron, NMotionElement> Afferents;

// Указатели на генераторы внешнего управления
UCPointer<NPulseGenerator, NMotionElement> ExternalControlGenerators;

// Указатели на результирующие нейроны (мотонейроны)
UCPointer<NPulseNeuron, NMotionElement> Motoneurons;

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
bool SetNumControlLoops(int value);
bool SetEnableControlLoopFlags(const std::vector<int> &value);
bool SetInterneuronPresentMode(int value);
bool SetRecurrentInhibitionMode(int value);
bool SetRecurrentInhibitionBranchMode(int value);
bool SetMotoneuronBranchMode(int value);
bool SetExternalControlMode(int value);
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
};

// Формирует СУ двигательной единицей
// Если mode == 0 - формируется полная модель
// 	    mode == 1 - модель без вставочных интернейронов
UEPtr<NANet> CreateMotionElement(NStorage *storage, const string &netclassname, int mode);

// Аналогично, но с развязкой по дендритам
UEPtr<NANet> CreateBranchedMotionElement(NStorage *storage, const string &netclassname,
	const string &neuron_class_name, const string &afferent_neuron_class_name, int mode);

// Формирует простейшую СУ двигательной единицей из двух мотонейронов
UEPtr<NANet> CreateSimplestMotionElement(NStorage *storage, const string &netclassname, int mode);
UEPtr<NANet> CreateSimplestBranchedMotionElement(NStorage *storage, const string &netclassname, int mode, bool use_speed_force=false, bool use_add_contours=false);

bool CreateNeuronBranchLink(UEPtr<NANet> net,const string &source,
	const string &target_head, const string &target_tail);

}
//---------------------------------------------------------------------------
#endif
