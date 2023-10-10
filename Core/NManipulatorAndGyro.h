/* ***********************************************************
File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NManipulatorAndGyro_H
#define NManipulatorAndGyro_H

#include "../../../Rdk/Deploy/Include/rdk.h"

namespace NMSDK {

using namespace RDK;

class RDK_LIB_TYPE NManipulatorAndGyro: public UNet
{
public: // Свойства
ULProperty<double, NManipulatorAndGyro, ptPubParameter> Mass;

ULProperty<double, NManipulatorAndGyro, ptPubParameter> Length;

public: // Входы и выходы
/// Внешний момент
UPropertyInputData<MDMatrix<double>, NManipulatorAndGyro, ptInput | ptPubState> InputMomentumExt;

/// Момент на валу двигателя
UPropertyInputData<MDMatrix<double>, NManipulatorAndGyro, ptInput | ptPubState> InputMomentum;

/// Текущий угол поворота вала
UPropertyInputData<MDMatrix<double>, NManipulatorAndGyro, ptInput | ptPubState> InputAngle;

UPropertyOutputData<MDMatrix<double>, NManipulatorAndGyro, ptOutput | ptPubState> Output;


protected: // Переменные состояния
 double gravity_constant;

 public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NManipulatorAndGyro(void);
virtual ~NManipulatorAndGyro(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NManipulatorAndGyro* New(void);
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
};
}
#endif
