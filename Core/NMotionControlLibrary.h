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
#include "../../../Rdk/Core/Engine/ModernSmartPointers.h"
#include "../../../Rdk/Core/Engine/ModernContainers.h"
#include "../../../Rdk/Core/System/ModernChrono.h"
#include "../../../Rdk/Core/System/ModernMutex.h"


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
#include "NTrajectoryElement.h"
#include "NMazeMemory.h"
#include "NMazeMemory_simplified.h"
#include "NPCNElement.h"

#include "NSuppressionUnit.h"
#include "NCounterNeuron.h"
#include "NObjInArea.h"
#include "NSeqComparison.h"
#include "NActuatorSignals.h"
#include "NSignalEstimation.h"


namespace NMSDK {

class RDK_LIB_TYPE NMotionControlLibrary: public ULibrary
{
public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NMotionControlLibrary(void);
// --------------------------

// --------------------------
// ������ �������� ��������� �������
// --------------------------
// --------------------------


// --------------------------
// ������ ����������� �����������
// --------------------------
// ��������� ������ ClassSamples �������� ������������ �������� � �� �������.
// �� ��������� ��������������� ������� ������� � ������ ������.
virtual void CreateClassSamples(UStorage *storage);
// --------------------------
};     

extern RDK_LIB_TYPE NMotionControlLibrary MotionControlLibrary;

}


#endif
