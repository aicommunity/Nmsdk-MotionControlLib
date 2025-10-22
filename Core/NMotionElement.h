/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2010.
E-mail:			alexab@ailab.ru
url:			http://ailab.ru
Version:		1.0.0

This file - part of the project: NMSDK

File License:		BSD License
Project License:	GNU GPLv3 License
See file license.txt for more information
*********************************************************** */
#ifndef NMotionElementH
#define NMotionElementH

#include "../../Nmsdk-PulseLib/Core/NAfferentNeuron.h"
#include "../../../Rdk/Core/Engine/ModernSmartPointers.h"
#include "../../../Rdk/Core/Engine/ModernContainers.h"
#include "../../../Rdk/Core/System/ModernChrono.h"
#include "../../../Rdk/Core/System/ModernMutex.h"

//---------------------------------------------------------------------------
namespace NMSDK {

class RDK_LIB_TYPE NMotionElement: public UNet
{
public: // ���������
// ����� �������� ����������
RDK::ULProperty<int, NMotionElement> NumControlLoops;

// ������ ������ ��������� �������� ����������
RDK::ULProperty<std::vector<int>, NMotionElement> EnableControlLoopFlags;

// ������ ������� ����� ����� ���������
// 0 - ������ ����� (��� �������������)
// 1 - ����� ����� ������������
// 2 - ��������� ����� ����� ������������(L-R) + ������ ����. �����(L-L)
// 3 - ����. ����� ����� ������������(L-L) + ������ ��������� �����(L-R)
// 4 - ��� 1, �� ��� ������������� ������� �� �����������
RDK::ULProperty<std::vector<int>, NMotionElement> LinkModes;

// ����� ������� ���������� ��������
// ��������, ���� ����� ���������� ����, ��� ������� ������ ������ �����
// ���� ����������
// 0 - ���������� �������� ���
// 1 - ���������� ������� ����
RDK::ULProperty<int, NMotionElement> InterneuronPresentMode;

// ����� ������� ������ ������
// 0 - ������ ������ ���
// 1 - ������ ������ ����
RDK::ULProperty<int, NMotionElement> RenshowMode;
// ����� ������� ������������� ��������
// 0 - ������������� �������� ���
// 1 - ������������� ������� ����
RDK::ULProperty<int, NMotionElement> PacemakerMode;

// ����� ������� ����������� ����������
// 0 - ����������� ���������� ���
// 1 - ���������� ���������� ������ �� �������� (�����������)
// 2 - ���������� ���������� ������������ �� ������ ������� �������
RDK::ULProperty<int, NMotionElement> RecurrentInhibitionMode;

// ����� ��������� ����������� ��� ����������� ����������� ����������
// 0 - ���������� ��������� ���� �� ��� �� ��������, ��� � ������ ����������
// (���� �������� N, �� �� ������ ������ ���� ��������� ����).
// 1 - ���������� ��������� ���� �� ��������� �������� �������
RDK::ULProperty<int, NMotionElement> RecurrentInhibitionBranchMode;

// ����� ��������� ����������� ��� ����������� �������� ����������
// 0 - ��� ������� ������������ � ������ ������� ��������
// 1 - ������ ������ ����� ���� ������� ��������
RDK::ULProperty<int, NMotionElement> MotoneuronBranchMode;

// ����� ����������� ���������� � ������� �������
// 0 - ���������� �� ������� ������������ �������� �������� ���� �� ���� ����
// ����� � ���������.
// 1 - ���������� �� ������� ������������ �� ��������� ������� � ���� �� �������
// ������� ��������� ����� � ���������
RDK::ULProperty<int, NMotionElement> ExternalControlMode;

// ��� ������� ��� ��������
RDK::ULProperty<string, NMotionElement> NeuroObjectName;
// ��� ������� ��� ����������� ��������
RDK::ULProperty<string, NMotionElement> AfferentObjectName;

public: // ���������� ���������

// ���� �������������� ������������� ���������� �������� ����������
bool  isNumControlLoopsInitialized;

public: // ��������� ����������
// ��������� �� ����������� �������
UCPointer<NAfferentNeuron, NMotionElement> Afferents;

// ��������� �� ���������� �������� ����������
UCPointer<NPulseGenerator, NMotionElement> ExternalControlGenerators;

// ��������� �� �������������� ������� (�����������)
UCPointer<NPulseNeuron, NMotionElement> Motoneurons;

string netclassname;
string neuron_class_name;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NMotionElement(void);
virtual ~NMotionElement(void);
// --------------------------

// --------------------------
// ������ ���������� �����������
// --------------------------
bool SetNumControlLoops(const int &value);
bool SetEnableControlLoopFlags(const std::vector<int> &value);
bool SetRenshowMode(const int &value);
bool SetPacemakerMode(const int &value);
bool SetLinkModes(const std::vector<int> &value);
bool SetInterneuronPresentMode(const int &value);
bool SetRecurrentInhibitionMode(const int &value);
bool SetRecurrentInhibitionBranchMode(const int &value);
bool SetMotoneuronBranchMode(const int &value);
bool SetExternalControlMode(const int &value);
bool SetNeuroObjectName(const string &value);
bool SetAfferentObjectName(const string &value);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������

// --------------------------
public:
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NMotionElement* New(void);
// --------------------------

// --------------------------
// Proctected computation methods
// --------------------------
protected:
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

// ������� ��������� � ������������ � �������� ���������� ����������
// ���� ��������� ����������, �� �������� �������������� �� � ������������ �����������
void CreateStructure(void);

// ������� ���������� ����� � ������������ � �������� ���������� ����������
void CreateInternalLinks(void);

// ��������� � ��������������� ������� �����
void BackupExternalLinks(void);
void RestoreExternalLinks(void);
// --------------------------
// ������ �������� ��������� ��
// --------------------------
// �������� ���� ������������
bool CreateMotoneurons();

// �������� ������ ����������� ��������
bool CreateAfferents();

// �������� �������������
bool CreateInterneurons();

// �������� ������ ����� ������������ ��������� � �������������
// 0 - ������ ����� (��� �������������)
// 1 - ����� ����� ������������
// 2 - ����� ����� ������������(L-R) + ������ �����(L-L)
// 3 - ����� ����� ������������(L-L) + ������ �����(L-R)
bool LinkMotoneurons();

// �������� ������ � �������� ������
bool LinkRenshow();

// �������� ������ � �������������� ���������
bool LinkPM();

// �������� ����� ����� ����� ���������
// 0 - ������������ �����
// 1 - ��������� �����
bool LinkNeuron(const string &source, const string &sink, int mode, const string &branch = "Soma1");

// --------------------------
// Modern C++20 methods
// --------------------------
// Move semantics
NMotionElement(const NMotionElement&) = default;
NMotionElement(NMotionElement&&) noexcept = default;
NMotionElement& operator=(const NMotionElement&) = default;
NMotionElement& operator=(NMotionElement&&) noexcept = default;

// Modern motion element operations
void OptimizeMotionElementPerformance();
bool IsMotionElementValid() const;
void ReserveMotionElementMemory(size_t control_loops_count);
TimePoint GetLastMotionUpdateTime() const;
void SetLastMotionUpdateTime(TimePoint time);

// Modern control loop operations
template<typename T>
void ReserveControlLoops(std::vector<T>& control_loops, size_t count);

template<typename T>
void MoveControlLoops(std::vector<T>& target, std::vector<T>&& source);

// Advanced motion control algorithms
void UpdateMotionParameters(double position, double velocity, double acceleration);
void CalculateMotionTrajectory(const std::vector<double>& waypoints);
void OptimizeMotionPath(const std::vector<double>& obstacles);
void UpdateMotionFeedback(double position_error, double velocity_error);
// --------------------------
};


bool RDK_LIB_TYPE CreateNeuronBranchLink(std::shared_ptr<UNet> net,const string &source,
	const string &target_head, const string &target_tail);

}
//---------------------------------------------------------------------------
#endif
