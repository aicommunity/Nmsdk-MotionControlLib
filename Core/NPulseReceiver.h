/* *********************************************************** 
@Copyright Alexander V. Bakhshiev, 2012.
E-mail:        alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: RDK

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NPulseReceiverH
#define NPulseReceiverH

#include "../../Nmsdk-PulseLib/Core/NReceiver.h"

//---------------------------------------------------------------------------
namespace NMSDK {

/* *************************************************************************** */
class RDK_LIB_TYPE NPulseReceiver: public NReceiver
{
public: // Общедоступные свойства
// Рассматриваемый интервал времени
ULProperty<double, NPulseReceiver, ptPubParameter> MaxAccumulationRange;

protected: // Данные
UPropertyInputCData<MDMatrix<double>, NPulseReceiver, ptInput | ptPubState> Inputs;

UPropertyOutputData<MDMatrix<double>, NPulseReceiver, ptOutput | ptPubState> Output;

protected: // Временные переменные
std::vector<std::list<double> > Data;
std::vector<std::list<double> > MatrixData;

std::vector<bool> PulseFlag;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NPulseReceiver(void);
virtual ~NPulseReceiver(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NPulseReceiver* New(void);
// --------------------------

protected:
// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
// Рассматриваемый интервал времени
bool SetMaxAccumulationRange(const double &value);
// --------------------------

// --------------------------
// Computation methods
// --------------------------
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
// --------------------------
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
