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
#include "../../../Rdk/Core/Engine/ModernSmartPointers.h"
#include "../../../Rdk/Core/Engine/ModernContainers.h"
#include "../../../Rdk/Core/System/ModernChrono.h"
#include "../../../Rdk/Core/System/ModernMutex.h"
#include "UEPtr.h"
#include <memory>
#include <mutex>
#include <chrono>
#include <thread>

//---------------------------------------------------------------------------
namespace NMSDK {

using namespace RDK;

/* *************************************************************************** */
class RDK_LIB_TYPE NManipulator: public UNet
{
public: // ������������� ��������
/// ���������������� �����������
ULProperty<double, NManipulator, ptPubParameter> EMFactor;

/// �������������
ULProperty<double, NManipulator, ptPubParameter> Inductance;

/// �������������
ULProperty<double, NManipulator, ptPubParameter> Resistance;

public: // ����� � ������
/// ������� ������
UPropertyInputData<MDMatrix<double>, NManipulator, ptInput | ptPubState> Input;

/// ������� ������
UPropertyOutputData<MDMatrix<double>, NManipulator, ptOutput | ptPubState> Output;

protected: // ��������� ����������
// ��� �����
double Current;

// ����� ���������
UGraphics Graphics;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NManipulator(void);
virtual ~NManipulator(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NManipulator* New(void);
// --------------------------

protected:
// --------------------------
// ������ ���������� �������������� ����������
// --------------------------
// ���������������� �����������
bool SetEMFactor(const double &value);

// �������������
bool SetInductance(const double &value);

// �������������
bool SetResistance(const double &value);
// --------------------------

// --------------------------
// Computation methods
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
virtual bool ADefault(void);

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
virtual bool ABuild(void);

// Reset computation
virtual bool AReset(void);

// Execute math. computations of current object on current step
virtual bool ACalculate(void);
// --------------------------

// --------------------------
// Modern C++20 methods
// --------------------------
// Move semantics
NManipulator(const NManipulator&) = default;
NManipulator(NManipulator&&) noexcept = default;
NManipulator& operator=(const NManipulator&) = default;
NManipulator& operator=(NManipulator&&) noexcept = default;

// Modern manipulator operations
void OptimizeManipulatorPerformance();
bool IsManipulatorValid() const;
void ReserveManipulatorMemory(size_t control_points_count);
TimePoint GetLastManipulatorUpdateTime() const;
void SetLastManipulatorUpdateTime(TimePoint time);

// Modern control operations
template<typename T>
void ReserveControlPoints(std::vector<T>& control_points, size_t count);

template<typename T>
void MoveControlPoints(std::vector<T>& target, std::vector<T>&& source);

// Advanced manipulator algorithms
void UpdateManipulatorParameters(double em_factor, double inductance, double resistance);
void CalculateManipulatorKinematics(const std::vector<double>& joint_angles);
void OptimizeManipulatorTrajectory(const std::vector<double>& target_positions);
void UpdateManipulatorDynamics(double torque, double velocity, double acceleration);
// --------------------------
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
