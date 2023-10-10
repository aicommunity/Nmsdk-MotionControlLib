// ===========================================================
// Version:        1.0.1
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2009.
E-mail:		alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NSIGNUMSEPARATOR_H
#define NSIGNUMSEPARATOR_H

#include "../../../Rdk/Deploy/Include/rdk.h"


namespace NMSDK {

using namespace RDK;

class RDK_LIB_TYPE NSignumSeparator: public UNet
{
public: // Общедоступные свойства
/// Выделяемый знак
/// может принимать значения +-1
UCLProperty<vector<double>,NSignumSeparator> Sign;

/// Множитель выходного значения
UCLProperty<vector<double>,NSignumSeparator> Gain;

/// Входные данные
UPropertyInputData<MDMatrix<double>,NSignumSeparator, ptInput | ptPubState> Input;

/// Разделенные данные
UPropertyOutputData<MDMatrix<double>,NSignumSeparator, ptOutput | ptPubState> Output;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NSignumSeparator(void);
virtual ~NSignumSeparator(void);
// --------------------------

protected:
// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
/// Выделяемый знак
bool SetSign(const double &value);

/// Множитель выходного значения
bool SetGain(const double &value);
// --------------------------

public:
// --------------------------
// Системные методы управления объектом
// --------------------------
/// Выделяет память для новой чистой копии объекта этого класса
virtual NSignumSeparator* New(void);
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
protected:
/// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ADefault(void);

/// Обеспечивает сборку внутренней структуры объекта
/// после настройки параметров
/// Автоматически вызывает метод Reset() и выставляет Ready в true
/// в случае успешной сборки
virtual bool ABuild(void);

/// Сброс процесса счета.
virtual bool AReset(void);

/// Выполняет расчет этого объекта
virtual bool ACalculate(void);
// --------------------------
};

}
#endif
