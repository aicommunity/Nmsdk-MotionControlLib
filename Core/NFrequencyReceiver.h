// ===========================================================
// Version:        3.0.2
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2002.
E-mail:		alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NFrequencyReceiverH
#define NFrequencyReceiverH
#include "../../Nmsdk-PulseLib/Core/NReceiver.h"

//---------------------------------------------------------------------------
namespace NMSDK {

/* *************************************************************************** */
class RDK_LIB_TYPE NFrequencyReceiver: public NReceiver
{
public: //  
/// Наблюдаемый интервал частот
UProperty<double, NFrequencyReceiver, ptPubParameter> LeftRange,RightRange;

/// Шаг по частоте
UProperty<double, NFrequencyReceiver, ptPubParameter> FrequencyStep;

protected: // 
UProperty<MDMatrix<double>, NFrequencyReceiver, ptPubOutput | ptState> OutputFreq;

/// Суммарное число импульсов
UProperty<size_t, NFrequencyReceiver, ptPubParameter> PulseCounter;

/// Множество входных данных
/// Каждая строка - последовательность данных
///   -  
UProperty<MDMatrix<double>, NFrequencyReceiver, ptPubParameter> Input;

/// Результаты в строках
/// Каждая строка - последовательность данных
///   -  
UProperty<MDMatrix<double>, NFrequencyReceiver, ptPubParameter> Results;

protected: //  
double PrevPulseStartTime;
double PrevPulseStopTime;

public: // 
// --------------------------
// --------------------------
// Конструкторы и деструкторы
// --------------------------
// --------------------------
NFrequencyReceiver(void);
virtual ~NFrequencyReceiver(void);
// --------------------------

// --------------------------
// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
// --------------------------
virtual NFrequencyReceiver* New(void);
// --------------------------

protected:
// --------------------------
// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
/// Устанавливает левую границу наблюдаемого интервала частот
// --------------------------
bool SetLeftRange(const double &range);

/// Устанавливает правую границу наблюдаемого интервала частот
bool SetRightRange(const double &range);

/// Устанавливает шаг по частоте
bool SetFrequencyStep(const double &step);
// --------------------------

// --------------------------
// --------------------------
// Watch results control methods
// --------------------------
// --------------------------
public:
/// Return number of results points
size_t GetResultsSize(void) const;
// --------------------------

// --------------------------
// --------------------------
// Computation methods
// --------------------------
/// Восстановление настроек по умолчанию и сброс процесса счета
// --------------------------
virtual bool ADefault(void);

/// Обеспечивает сборку внутренней структуры объекта
/// после настройки параметров
/// Автоматически вызывает метод Reset() и выставляет Ready в true
/// в случае успешной сборки
///    Reset()   Ready  true
virtual bool ABuild(void);

/// Reset computation
virtual bool AReset(void);

/// Execute math. computations of current object on current step
virtual bool ACalculate(void);
// --------------------------
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
