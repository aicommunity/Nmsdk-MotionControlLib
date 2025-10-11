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
#ifndef NEngineMotionControlH
#define NEngineMotionControlH

#include "../../Nmsdk-PulseLib/Core/NReceptor.h"
#include "../../Nmsdk-PulseLib/Core/NNet.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../Rdk-BasicLib/Core/UBCLLibrary.h"
#include "NMotionElement.h"
#include "NManipulatorSource.h"
#include "NIntervalSeparator.h"
#include "NControlObjectSource.h"
#include "../../../Rdk/Core/Engine/ModernSmartPointers.h"
#include "../../../Rdk/Core/Engine/ModernContainers.h"
#include "../../../Rdk/Core/System/ModernChrono.h"
#include "../../../Rdk/Core/System/ModernMutex.h"



namespace NMSDK {

class RDK_LIB_TYPE NEngineMotionControl: public UNet
{
public: // ��������� ��������
// ����� �������� ����������
RDK::ULProperty<int, NEngineMotionControl> NumControlLoops;
// ����� ����������� ���������
RDK::ULProperty<int, NEngineMotionControl> NumMotionElements;

// ����� ������������ ����
// 0 - Signum
// 1 - Range
// 2 - Branched Range
// 3 - Branched Range Crosslinks
// 4 - Branched Ind. Range
// 5 - Branched Ind. Range Crosslinks
// 10 - Branched Ind. Range Continues LTZone neurons
// 11 - Simplest 2 neuron model
// 12 - As 11 but speed and force control added
// 13 - As 12 but additional control contour added
// 14 - New net with parametric structure control
RDK::ULProperty<int, NEngineMotionControl> CreationMode;

// ��� ������ ������������ ��������
RDK::ULProperty<NameT, NEngineMotionControl> MotionElementClassName;

// ��� ������ ���������� � �������� ����������
RDK::ULProperty<NameT, NEngineMotionControl> ObjectControlInterfaceClassName;

/// ����� ��������� ���������� ����������� ���������
/// 0 - ��� ������������
/// 1 - ������������ �������� � ������ ������ ����������
/// 2 - �������� �������������� ��������� c ������ ������� ����������
RDK::ULProperty<int, NEngineMotionControl> AdaptiveStructureMode;

// ����� ������� ���������� ��������
// 0 - ���������� �������� ���
// 1 - ���������� ������� ����
RDK::ULProperty<int, NEngineMotionControl> InterneuronPresentMode;

// ����� ���������� ������������ (��. �������� � ����������� ��������)
RDK::ULProperty<std::vector<int>, NEngineMotionControl> LinkModes;

// �������� ����������� �������� �� ������ Ia
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IaMin;
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IaMax;

// �������� ����������� �������� �� ������ Ib
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IbMin;
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IbMax;

// �������� ����������� �������� �� ������ II
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IIMin;
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IIMax;

// �������� ����������� �������� �� ������ Ic
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IcMin;
RDK::ULProperty<double, NEngineMotionControl,ptParameter> IcMax;

/// �������� ����������� �������� �� �������
RDK::ULProperty<std::vector<double>, NEngineMotionControl> AfferentMin;
RDK::ULProperty<std::vector<double>, NEngineMotionControl> AfferentMax;

/// ����� ��������� �����������
RDK::ULProperty<int, NEngineMotionControl> IntervalSeparatorMode;


// ������������ �������� ������������ �����������
// 0 - ��������� �� ��������� [Max/(i+1)] ��� i - ������ ���������
// 1 - ��������� �� ��������� [Max/N^2]*(i+1)^2 ��� i - ������ ���������
// 2 - �� ������� (3.4)
// 3 - �� ������� (3.3) Max/(N-i), ��� i - ������ ���������, � N - ����� ����������
RDK::ULProperty<double, NEngineMotionControl> PacGain;

RDK::ULProperty<double, NEngineMotionControl> PacSecretionTC;
RDK::ULProperty<double, NEngineMotionControl> PacDissociationTC;

// ����� ��������� ���������� ����������� ��������
// 0 - ��������� ���������� �����
// 1 - ��������� � ��������� �����, � ����������� ������� � ���������
// MinAfferentRange �� ��������������� ������� *Max-*Min
RDK::ULProperty<int, NEngineMotionControl> AfferentRangeMode;

// ����� ��������� ���������� pac
RDK::ULProperty<int, NEngineMotionControl> PacRangeMode;

// ���������� �������� �� ��������������� ������� *Max-*Min
// ������������ ����������� ������ ��������� ���������� ��������
// ����������� � ��������� [0;1]
RDK::ULProperty<double, NEngineMotionControl,ptPubState> MinAfferentRange;

/// ������� ��������� ���������
RDK::ULProperty<std::vector<double>, NEngineMotionControl,ptPubState> CurrentContourAmplitude;

/// ������� ������� ���������
RDK::ULProperty<std::vector<double>, NEngineMotionControl,ptPubState> CurrentContourAverage;

/// ������� ����� ����������� ��������
RDK::ULProperty<double, NEngineMotionControl,ptPubState> CurrentTransientTime;

/// ���������� �������� ����������� �� �������� ��������� ���������� �������
RDK::ULProperty<double, NEngineMotionControl,ptPubState> InstantAvgSpeed;

/// ������� ��������� ����������� ��������
RDK::ULProperty<bool, NEngineMotionControl,ptPubState> CurrentTransientState;

/// ����������� ���������� ��������� ���������
RDK::ULProperty<std::vector<double>, NEngineMotionControl> DestContourMaxAmplitude;

/// ���������� ���������� ��������� ���������
RDK::ULProperty<std::vector<double>, NEngineMotionControl> DestContourMinAmplitude;

/// �������� ����� ����������� ��������
RDK::ULProperty<double, NEngineMotionControl> DestTransientTime;

/// ������������ � ������ ������� ����������
/// � ������ ������� MotionControl ������� �����:
/// 0=II
/// 1=Ia
/// 2=Ib
/// 3=Ic
RDK::ULProperty<std::vector<bool>, NEngineMotionControl> ActiveContours;

/// ������������ � ��������� ������� ����������
RDK::ULProperty<std::vector<bool>, NEngineMotionControl> UseContourData;

/// �������� ������� �� ������� ����������� ���������� �������
RDK::ULProperty<double, NEngineMotionControl> TransientHistoryTime;

/// ������ ������ ��������� ������� (�������) �� �������� ����������� ���������� �������
RDK::ULProperty<int, NEngineMotionControl> TransientObjectIndex;

 /// ����������� ���������� �������� ��������� �������� �������� ������� �� �������� �������, �� �������
/// ��������� ������� � ���, ��� ���������� ������� ��������
RDK::ULProperty<double, NEngineMotionControl> TransientAverageThreshold;

// ��� ������� ��� ��������
RDK::ULProperty<string, NEngineMotionControl> MCNeuroObjectName;

// ��� ������� ��� ����������� ��������
RDK::ULProperty<string, NEngineMotionControl> MCAfferentObjectName;

// ��� ������� ��� PAC
RDK::ULProperty<string, NEngineMotionControl> PacObjectName;

// ������������ ��������������� ��������� ���������
RDK::ULProperty<std::vector<double>, NEngineMotionControl,ptPubState> MaxContourAmplitude;

// ����� ��������� ����������� ��� ����������� �������� ����������
// 0 - ��� ������� ������������ � ������ ������� ��������
// 1 - ������ ������ ����� ���� ������� ��������
RDK::ULProperty<int, NEngineMotionControl> MotoneuronBranchMode;

// ����� ������� ������ ������
// 0 - ������ ������ ���
// 1 - ������ ������ ����
RDK::ULProperty<int, NEngineMotionControl> RenshowMode;

/// ����� ��� ����� ����������. � ������ ������ ���������� ������ � ��������� �������:
//
//UPropertyOutputData<MDMatrix<double>, NEngineMotionControl, ptPubState> Statistic;
UPropertyOutputData<MDMatrix<double>, NEngineMotionControl> Statistic;

protected: // ��������� ����������
vector<vector<UEPtr<NReceptor> > > receptors;

vector<pair<double,double> > Ia_ranges_pos,Ia_ranges_neg;
vector<pair<double,double> > Ib_ranges_pos,Ib_ranges_neg;
vector<pair<double,double> > II_ranges_pos,II_ranges_neg;
vector<pair<double,double> > Ic_ranges_pos,Ic_ranges_neg;

vector<vector<pair<double,double> > > AfferentRangesPos, AfferentRangesNeg;

vector<NMotionElement *> Motions;

/// ���������� �������������� ��������� ����������
UEPtr<NPulseGenerator> InternalGenerator;

vector<vector<double> > History;

vector<double> TransientHistory;

int HistorySize,TransientHistorySize;

double LastAdaptiveTime;

double TransientStartTime;

double OldTransientAverage;

double TempTransientState;

/// �������, ������������, ����� ����� ������� ���������� ������ �����������
/// 0 - ������ �����, � ������� ����� �������� ����������� ��� ��������
/// 1 - ����� �����, � ������� ����� �������� ����� ����������� ����������
int ControlMode;

/// ��������� ����������, ���������� ������� ����� ����������� ����������
/// -1 - �� ���������
/// 0 -
/// 1 -
int InternalGeneratorDirection;

//UStatisticMatrix<double> StatisticDoubleMatrix;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NEngineMotionControl(void);
virtual ~NEngineMotionControl(void);

// --------------------------
// --------------------------
// ������ ���������� �����������
// --------------------------
protected:
//UEPtr<NManipulatorSource> ManipulatorSource1;
//UEPtr<NIntervalSeparator> PosIntervalSeparator;

// ����� �������� ����������
bool SetNumControlLoops(const int &value);

bool SetIsAfferentLinked(const int &index, const bool &value);

bool GetIsAfferentLinked(const int &index);

// ����� ����������� ���������
bool SetNumMotionElements(const int &value);

// ����� ������������ ����
bool SetCreationMode(const int &value);

// ����� ��������� ���������� ����������� ��������
bool SetAfferentRangeMode(const int &value);

// ����� ��������� ���������� pac
bool SetPacRangeMode(const int &value);

// ���������� �������� �� ��������������� ������� *Max-*Min
bool SetMinAfferentRange(const double &value);

// ������������ �������� ������������ �����������
bool SetPacGain(const double &value);
bool SetPacSecretionTC(const double &value);
bool SetPacDissociationTC(const double &value);

bool SetMCNeuroObjectName(const string &value);
bool SetMCAfferentObjectName(const string &value);
bool SetPacObjectName(const string &value);
bool SetObjectControlInterfaceClassName(const string &value);


/// �������� ����������� �������� �� �������
bool SetAfferentMin(const std::vector<double> &value);
bool SetAfferentMax(const std::vector<double> &value);

bool SetIntervalSeparatorMode(const int &value);

bool SetMotoneuronBranchMode(const int &value);
bool SetRenshowMode(const int &value);

/// ���������� ������� �������� ��������
bool SetActiveContours(const std::vector<bool> &value);

/// ����� ������� �������������
bool SetInterneuronPresentMode(const int &value);

bool SetLinkModes(const std::vector<int> &value);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
public:
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NEngineMotionControl* New(void);
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
// --------------------------

// --------------------------
// ������ ���������� ������
// --------------------------
public:
// ������� ������ � �������� ���������� � ������������ � CreationMode
// ���� full_recreate == true ������� ��� ������������ ����������� ��������
// ����� ��������� �� ���������
virtual bool Create(bool full_recreate=true);

/// ������� ������������ ���������, �� ������ ��������������� ����������
/// ��������� expected_num_motion_elements ����� ����������� ���������
virtual bool ClearStructure(int expected_num_motion_elements);

/// �������� ���������� ���������
virtual void AdaptiveTuning(void);

/// ���������� ���������� ���������� ���������
/// current_contour_amplitude - ������� ��������� �� �������� ����������
/// use_contour_data - �����, ������������ ������ ����� �������� ����� ������������
/// current_transient_time - ������� ����� ����������� ��������
/// dest_contour_amplitude - �������� ��������� �� �������� ����������
/// dest_transient_time - �������� ����� ����������� ��������
/// num_motion_elements - ��������� ����� ����������� ���������
/// control_gain - ��������� �������� ������� ����������
///
virtual void AdaptiveTuningSimple(const std::vector<double> &current_contour_amplitude,
								  const std::vector<bool> &use_contour_data,
								  double current_transient_time,
								  const std::vector<double> &dest_contour_max_amplitude,
								  const std::vector<double> &dest_contour_min_amplitude,
								  double dest_transient_time,
								  int &num_motion_elements,
								  double &control_gain);
// --------------------------
int GetNumControlLoops(void);
// --------------------------
// ��������������� ������
// --------------------------
protected:
// ��������� ��������� �������� �����������
// ���������� ����� ������������ ����������
int CalcAfferentRange(int num_motions, bool cross_ranges, double a_min, double a_max,
			vector<pair<double,double> > &pos_ranges, vector<pair<double,double> > &neg_ranges, int range_mode);

void SetupPacRange(void);

// ��������� ��������������� ������-������
void AACSetup(UEPtr<UNet> net, double gain_value);

// ������� ��������������� ���������
void AdditionalComponentsSetup(UEPtr<UNet> net);

//------------------------------
//����� ������ ������������ ����

// ��������� ���� ���������� ����� �������� �� 2 ���������� ��������
UNet* CreateNewEngineControl2NeuronsSimplest(bool crosslinks = false, bool crossranges=false);

// ��������� ����������
void NewMotionElementsSetup(UEPtr<UNet> net);

// ��������� ��������������� �������-������
void NewPACSetup(double pulse_amplitude, double secretion_tc, double dissociaton_tc, double gain_value, bool gain_div_mode);

/// ��������� ��������� ���������� ������� Pac
void UpdatePacTCParameters(void);

// ��������� ����������� ������
void NewStandardLinksSetup(const string &engine_integrator_name);

public:
// ��������� ������������ ����������
void NewIntervalSeparatorsSetup(int mode_value, int last_mode_value, double pos_gain_value, double neg_gain_value);

// ��������� ������������ ����������
void NewIntervalSeparatorsUpdate(int mode_value, int last_mode_value);

// ��������� ������ ������������ ����������
void NewIntervalSeparatorLinksSetup(void);

/// ���������� ���������� ���������� � ���������� �������� ������� ����� ����������� ���������
/// direction 0 - ������, direction 1 - �������
void ConnectInternalGenerators(int direction, int num_motion_elements, int control_loop_index);

/// ������ ������� ������ ����������� ����������
void SetInternalGeneratorFrequency(int direction, int num_motion_elements, int control_loop_index, double value);
// --------------------------

vector<NMotionElement *> GetMotion(void);

// --------------------------
// Modern C++20 methods
// --------------------------
// Move semantics
NEngineMotionControl(const NEngineMotionControl&) = default;
NEngineMotionControl(NEngineMotionControl&&) noexcept = default;
NEngineMotionControl& operator=(const NEngineMotionControl&) = default;
NEngineMotionControl& operator=(NEngineMotionControl&&) noexcept = default;

// Modern motion control operations
void OptimizeMotionControlPerformance();
bool IsMotionControlValid() const;
void ReserveMotionControlMemory(size_t motion_elements_count);
TimePoint GetLastControlUpdateTime() const;
void SetLastControlUpdateTime(TimePoint time);

// Modern control loop operations
template<typename T>
void ReserveControlLoops(std::vector<T>& control_loops, size_t count);

template<typename T>
void MoveControlLoops(std::vector<T>& target, std::vector<T>&& source);

// Advanced motion control algorithms
void UpdateControlParameters(double position_gain, double velocity_gain, double acceleration_gain);
void OptimizeControlTrajectory(const std::vector<double>& target_positions);
void CalculateOptimalControlInput(const std::vector<double>& current_state, const std::vector<double>& target_state);
void UpdatePIDController(double kp, double ki, double kd);
// --------------------------

};

}
#endif
