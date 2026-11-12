// ===========================================================
// Version:        1.0.0
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

#ifndef NINTERVAL_SEPARATOR_H
#define NINTERVAL_SEPARATOR_H

#include "../../../Rdk/Deploy/Include/rdk.h"

namespace NMSDK {

using namespace RDK;

class RDK_LIB_TYPE NIntervalSeparator: public UNet
{
public: //  
///    
UProperty<vector<double> ,NIntervalSeparator, ptPubParameter> MinRange;

///    
UProperty<vector<double> ,NIntervalSeparator, ptPubParameter> MaxRange;

///  .
/// 0 -      ,    ,  0
/// 1 -      ,    
///   ,  0
/// 2 -     ,    ,  0
/// 3 -     ,    ,  0
/// 4 -     ,    
/// 5 -     ,    
///   ;    ,  
///      (      )
/// 6 -      ,    
///   ;    ,  
///     
UProperty<vector<int>,NIntervalSeparator, ptPubParameter> Mode;

///   
UProperty<vector<double> ,NIntervalSeparator, ptPubParameter> Gain;

/// Входные данные
UProperty<MDMatrix<double>,NIntervalSeparator, ptInput | ptPubState> Input;

/// Разделенные данные
UProperty<MDMatrix<double>,NIntervalSeparator, ptOutput | ptPubState> Output;

public: // 
// --------------------------
// --------------------------
// Конструкторы и деструкторы
// --------------------------
// --------------------------
NIntervalSeparator(void);
virtual ~NIntervalSeparator(void);
// --------------------------

protected:
// --------------------------
// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
/// Нижняя граница выделяемого интервала
// --------------------------
bool SetMinRange(const double &value);

/// Верхняя граница выделяемого интервала
bool SetMaxRange(const double &value);

/// Режим разделения
bool SetMode(const int &value);

/// Множитель выходного значения
bool SetGain(const double &value);
// --------------------------

public:
// --------------------------
// --------------------------
// Системные методы управления объектом
// --------------------------
/// Выделяет память для новой чистой копии объекта этого класса
// --------------------------
virtual NIntervalSeparator* New(void);
// --------------------------

// --------------------------
// --------------------------
// Скрытые методы управления счетом
// --------------------------
// --------------------------
protected:
/// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ADefault(void);

/// Обеспечивает сборку внутренней структуры объекта
/// после настройки параметров
/// Автоматически вызывает метод Reset() и выставляет Ready в true
/// в случае успешной сборки
///    Reset()   Ready  true
virtual bool ABuild(void);

/// Сброс процесса счета.
virtual bool AReset(void);

/// Выполняет расчет этого объекта
virtual bool ACalculate(void);
// --------------------------
};

}
#endif
