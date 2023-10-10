/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2012.
E-mail:            alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NPendulumAndCartH
#define NPendulumAndCartH

#include "../../../Rdk/Deploy/Include/rdk.h"

//---------------------------------------------------------------------------
namespace NMSDK {

using namespace RDK;

/* *************************************************************************** */
class RDK_LIB_TYPE NPendulumAndCart: public UNet
{
public: // Общедоступные свойства
/// Масса тележки
ULProperty<double, NPendulumAndCart> CartMass;

/// Масса стержня
ULProperty<double, NPendulumAndCart> RodMass;

/// Длина стержня
ULProperty<double, NPendulumAndCart> RodLength;

/// Перемещение по X
ULProperty<double, NPendulumAndCart> OutXMovement;

/// Режим работы
/// 0 - Управление внешней СУ
/// 1 - Управление встроенным PID
ULProperty<double, NPendulumAndCart> Mode;

/// Ki
ULProperty<double, NPendulumAndCart> Ki;

/// Kd
ULProperty<double, NPendulumAndCart> Kd;

/// Kp
ULProperty<double, NPendulumAndCart> Kp;

/// Ki
ULProperty<double, NPendulumAndCart> MovementKi;

/// Kd
ULProperty<double, NPendulumAndCart> MovementKd;

/// Kp
ULProperty<double, NPendulumAndCart> MovementKp;

/// Вес управления по углу
ULProperty<double, NPendulumAndCart> AngleWeight;

/// Вес управления по перемещению
ULProperty<double, NPendulumAndCart> MovementWeight;

public: // Состояния

/// Внешний момент
ULProperty<double, NPendulumAndCart, ptPubState> ExtrenalMoment;

public: // Входы и выходы
UPropertyInputData<MDMatrix<double>, NPendulumAndCart, ptInput | ptPubState> Input1;

UPropertyInputData<MDMatrix<double>, NPendulumAndCart, ptInput | ptPubState> Input2;

/// Выходное ускорение тележки
UPropertyOutputData<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> Acceleration;

/// Угол поворота стержня
UPropertyOutputData<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> Angle;

/// Выходная скорость тележки
UPropertyOutputData<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> Speed;

/// Перемещение тележки
UPropertyOutputData<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> Movement;

/// Перемещение тележки
UPropertyOutputData<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> MovementSpeed;

protected: // Данные
double theta0;
double x0;
double y,x,movement;
double y0;

protected: // Временные переменные

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NPendulumAndCart(void);
//NPendulumAndCart(const string &name);
virtual ~NPendulumAndCart(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NPendulumAndCart* New(void);
// --------------------------

protected:
// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
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
