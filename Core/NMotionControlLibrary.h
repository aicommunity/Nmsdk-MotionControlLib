/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2010.
E-mail:            alexab@ailab.ru
url:            http://ailab.ru
Version:        1.0.0

This file - part of the project: NMSDK

File License:        BSD License
Project License:    GNU GPLv3 License
See file license.txt for more information
*********************************************************** */

#ifndef NMotionControlLibraryH
#define NMotionControlLibraryH


#include "NDCEngine.h"
#include "NPendulumAndCart.h"
#include "NAstaticGyro.h"
#include "NManipulatorAndGyro.h"


#include "NManipulator.h"
#include "NNavMousePrimitive.h"

#include "NFrequencyReceiver.h"
#include "NManipulatorInput.h"
#include "NManipulatorInputEmulator.h"
#include "NSimpleStatistic.h"
//#include "NNetworkLinksStatistic.h"
#include "NPulseReceiver.h"

#include "NSignumSeparator.h"
#include "NIntervalSeparator.h"
#include "NEyeRetina.h"


#include "NManipulatorSourceEmulator.h"
#include "NManipulatorSource.h"
#include "NControlObjectSource.h"


#include "NEngineMotionControl.h"
#include "NMotionElement.h"
#include "NPositionControlElement.h"
#include "NNewPositionControlElement.h"
#include "NMultiPositionControl.h"
#include "NPCNElement.h"

#include "NSuppressionUnit.h"

namespace NMSDK {

class RDK_LIB_TYPE NMotionControlLibrary: public ULibrary
{
public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NMotionControlLibrary(void);
// --------------------------

// --------------------------
// Методы создания составных моделей
// --------------------------
// --------------------------


// --------------------------
// Методы заполенения бибилиотеки
// --------------------------
// Заполняет массив ClassSamples готовыми экземплярами образцов и их именами.
// Не требуется предварительная очистка массива и уборка памяти.
virtual void CreateClassSamples(UStorage *storage);
// --------------------------
};     

extern RDK_LIB_TYPE NMotionControlLibrary MotionControlLibrary;

}


#endif
