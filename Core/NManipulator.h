// ===========================================================
// Version:        1.0.0
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

#ifndef NManipulatorH
#define NManipulatorH
#include "../../../Rdk/Deploy/Include/rdk.h"

//---------------------------------------------------------------------------
namespace NMSDK {

using namespace RDK;

/* *************************************************************************** */
class RDK_LIB_TYPE NManipulator: public UNet
{
public: //  
/// Электромагнитный коэффициент
UProperty<double, NManipulator, ptPubParameter> EMFactor;

/// Индуктивность
UProperty<double, NManipulator, ptPubParameter> Inductance;

/// Сопротивление
UProperty<double, NManipulator, ptPubParameter> Resistance;

public: //   
/// Входной сигнал
UProperty<MDMatrix<double>, NManipulator, ptInput | ptPubState> Input;

/// Выходой сигнал
UProperty<MDMatrix<double>, NManipulator, ptOutput | ptPubState> Output;

protected: //  
// Ток якоря
double Current;

// Класс рисования
UGraphics Graphics;

public: // 
// --------------------------
// --------------------------
// Конструкторы и деструкторы
// --------------------------
// --------------------------
NManipulator(void);
virtual ~NManipulator(void);
// --------------------------

// --------------------------
// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
// --------------------------
virtual NManipulator* New(void);
// --------------------------

protected:
// --------------------------
// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
// Электромагнитный коэффициент
// --------------------------
bool SetEMFactor(const double &value);

// Индуктивность
bool SetInductance(const double &value);

// Сопротивление
bool SetResistance(const double &value);
// --------------------------

// --------------------------
// --------------------------
// Computation methods
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
// --------------------------
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
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
