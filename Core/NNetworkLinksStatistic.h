/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2011.
E-mail:            alexab@ailab.ru
url:            http://ailab.ru
Version:        1.0.0

This file - part of the project: NMSDK

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NNetworkLinksStatisticH
#define NNetworkLinksStatisticH

#include "../../../Rdk/Deploy/Include/rdk.h"

namespace NMSDK {

using namespace RDK;

class RDK_LIB_TYPE NNetworkLinksStatistic: public UNet
{
public: // 
// Интервал времени через который обновляется статистика
UProperty<double, NNetworkLinksStatistic, ptPubParameter> StatsInterval;

// Режим формирования статистики
UProperty<int, NNetworkLinksStatistic, ptPubParameter>  Mode;

public: //   
UPropertyInputC<UNet, NNetworkLinksStatistic, ptPubInput> Inputs;

protected: // 
// Номер статистики
int StatsNumber;

protected: //  
// Файлы данных
RDK::UIniFile<char> StatsFile;

// Момент начала очередного интервала накопления статистики
RDK::ULongTime StatsStartTime;


public: // 
// --------------------------
// --------------------------
// Конструкторы и деструкторы
// --------------------------
// --------------------------
NNetworkLinksStatistic(void);
virtual ~NNetworkLinksStatistic(void);
// --------------------------

// --------------------------
// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
// --------------------------
virtual NNetworkLinksStatistic* New(void);
// --------------------------

// --------------------------
// --------------------------
// Computation methods
// --------------------------
// Открывает новый файл, сохраняя предыдущий
// --------------------------
bool ReCreateFile(void);
// --------------------------

// --------------------------
// --------------------------
// Вспомогательные методы счета статистики
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
// --------------------------

// --------------------------
// --------------------------
// Вспомогательные методы счета статистики
// --------------------------
// --------------------------
protected:
// Сбрасывает текущую статистику
void ClearStats(void);

// Задает размеры векторов данных текущей статистики
void ResizeStats(void);
// --------------------------

};

}

#endif
