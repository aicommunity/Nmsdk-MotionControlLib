// ===========================================================
// Version:        1.0.1
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2009.
E-mail:			alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NDCEngineH
#define NDCEngineH

#include "../../../Rdk/Deploy/Include/rdk.h"

//---------------------------------------------------------------------------
namespace NMSDK {

using namespace RDK;

/* *************************************************************************** */
class RDK_LIB_TYPE NDCEngine: public UNet
{
public: // Общедоступные свойства
/// Электромагнитный коэффициент
ULProperty<double, NDCEngine, ptPubParameter> EMFactor;

/// Индуктивность
ULProperty<double, NDCEngine, ptPubParameter> Inductance;

/// Сопротивление
ULProperty<double, NDCEngine, ptPubParameter> Resistance;

/// Электромеханическая постоянная времени
ULProperty<double, NDCEngine, ptPubParameter> Tm;

/// Передаточное число
ULProperty<double, NDCEngine, ptPubParameter> ReductionRate;

/// Внешний момент
ULProperty<double, NDCEngine, ptPubParameter> OutMoment;

public: // Входы и выходы
/// Входное напряжение
UPropertyInputData<MDMatrix<double>, NDCEngine, ptInput | ptPubState> InputVoltage;

/// Внешний момент на валу
UPropertyInputData<MDMatrix<double>, NDCEngine, ptInput | ptPubState> InputMomentum;

/// Выходной момент
UPropertyOutputData<MDMatrix<double>, NDCEngine, ptOutput | ptPubState> OutputMomentum;

/// Выходной угол поворота вала
UPropertyOutputData<MDMatrix<double>, NDCEngine, ptOutput | ptPubState> OutputAngle;

/// Выходная угловая скорость вращения
UPropertyOutputData<MDMatrix<double>, NDCEngine, ptOutput | ptPubState> OutputAngleSpeed;

protected: // Данные
// Ток якоря
double Current;

// ЭДС якоря
double EMF;

// Угол поворота
double Angle;

// Момент развиваемый двигателем
double Moment;

// Угловая скорость
double AngleSpeed;

// Производная момента
double DiffMoment;

protected: // Временные переменные

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NDCEngine(void);
virtual ~NDCEngine(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NDCEngine* New(void);
// --------------------------

protected:
// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
// Электромагнитный коэффициент
bool SetEMFactor(const double &value);

// Индуктивность
bool SetInductance(const double &value);

// Сопротивление
bool SetResistance(const double &value);

// Электромеханическая постоянная времени
bool SetTm(const double &value);

// Передаточное число
bool SetReductionRate(const double &value);
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
