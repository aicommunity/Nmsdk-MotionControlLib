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

#include "../../Kernel/NSupport.h"
#include "../PulseLib/NReceptor.h"

namespace NMSDK {

class NEngineMotionControl: public UNet
{
public: // ��������� ��������
// ����� ����������� ���������
RDK::ULProperty<size_t, NEngineMotionControl> NumMotionElements;

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
RDK::ULProperty<int, NEngineMotionControl> CreationMode;

// ��� ������ ������������ ��������
RDK::ULProperty<NameT, NEngineMotionControl> MotionElementClassName;

/// ����� ��������� ���������� ����������� ���������
/// 0 - ��� ������������
/// 1 - ������������ �������� � ������ ������ ����������
/// 2 - �������� �������������� ��������� c ������ ������� ����������
RDK::ULProperty<int, NEngineMotionControl> AdaptiveStructureMode;

// ������ ���������� ����������� ��������
//RDK::ULProperty<std::vector<double>, NEngineMotionControl> IMin;
//RDK::ULProperty<std::vector<double>, NEngineMotionControl> IMax;

// �������� ����������� �������� �� ������ Ia
RDK::ULProperty<double, NEngineMotionControl> IaMin;
RDK::ULProperty<double, NEngineMotionControl> IaMax;

// �������� ����������� �������� �� ������ Ib
RDK::ULProperty<double, NEngineMotionControl> IbMin;
RDK::ULProperty<double, NEngineMotionControl> IbMax;

// �������� ����������� �������� �� ������ II
RDK::ULProperty<double, NEngineMotionControl> IIMin;
RDK::ULProperty<double, NEngineMotionControl> IIMax;

// �������� ����������� �������� �� ������ Ic
RDK::ULProperty<double, NEngineMotionControl> IcMin;
RDK::ULProperty<double, NEngineMotionControl> IcMax;

// ������������ �������� ������������ �����������
// 0 - ��������� �� ��������� [Max/(i+1)] ��� i - ������ ���������
// 1 - ��������� �� ��������� [Max/(i+1)] ��� i - ������ ���������
// 2 -
// 3 - Max/(N-i), ��� i - ������ ���������, � N - ����� ����������
RDK::ULProperty<double, NEngineMotionControl> PacGain;

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

/// ������� ��������� ����������� ��������
RDK::ULProperty<bool, NEngineMotionControl,ptPubState> CurrentTransientState;

/// ����������� ���������� ��������� ���������
RDK::ULProperty<std::vector<double>, NEngineMotionControl> DestContourMaxAmplitude;

/// ���������� ���������� ��������� ���������
RDK::ULProperty<std::vector<double>, NEngineMotionControl> DestContourMinAmplitude;

/// �������� ����� ����������� ��������
RDK::ULProperty<double, NEngineMotionControl> DestTransientTime;

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

protected: // ��������� ����������
vector<vector<UEPtr<NReceptor> > > receptors;

vector<pair<double,double> > Ia_ranges_pos,Ia_ranges_neg;
vector<pair<double,double> > Ib_ranges_pos,Ib_ranges_neg;
vector<pair<double,double> > II_ranges_pos,II_ranges_neg;
vector<pair<double,double> > Ic_ranges_pos,Ic_ranges_neg;

vector<NNet*> Motions;

vector<vector<double> > History;

vector<double> TransientHistory;

vector<bool> isAfferentLinked;

int HistorySize,TransientHistorySize;

double LastAdaptiveTime;

double TransientStartTime;

double OldTransientAverage;

double TempTransientState;


public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NEngineMotionControl(void);
virtual ~NEngineMotionControl(void);
// --------------------------
bool SetIsAfferentLinked(const int &index, const bool &value);

bool GetIsAfferentLinked(const int &index);
// --------------------------
// ������ ���������� �����������
// --------------------------
protected:
// ����� ����������� ���������
bool SetNumMotionElements(const size_t &value);

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

bool SetMCNeuroObjectName(const string &value);
bool SetMCAfferentObjectName(const string &value);
bool SetPacObjectName(const string &value);
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

// ��������� ����������
void MotionElementsSetup(UEPtr<UContainer> net, int inp_mode, int out_mode, double exp_coeff, double receptor_max_output, double receptor_gain, int real_ranges);

// ��������� ��������������� �������-������
void PACSetup(UEPtr<UContainer> net,
		double pulse_amplitude, double secretion_tc, double dissociaton_tc, double gain_value, bool gain_div_mode, int tc_mode);

// ��������� ��������������� ������-������
void AACSetup(UEPtr<UContainer> net, double gain_value, bool gain_div_mode);

// ��������� ������������ ����������
void IntervalSeparatorsSetup(UEPtr<UContainer> net, int mode_value, double pos_gain_value, double neg_gain_value, bool II=true, bool Ia=true, bool Ib=true, bool Ic=false);

// ��������� ������������ ����������
void IntervalSeparatorsUpdate(UEPtr<UContainer> net, int mode_value);

// ������� ��������������� ���������
void AdditionalComponentsSetup(UEPtr<UContainer> net);

// ��������� ����������� ������
void StandardLinksSetup(UEPtr<UNet> net, const string &engine_integrator_name);

// ��������� ������ ������������ ����������
void IntervalSeparatorLinksSetup(UEPtr<UNet> net, bool II=true, bool Ia=true, bool Ib=true, bool Ic=false);

// ��������� ���� ���������� ���������� � ����������� ��������������� ������ � ��������
// �� ��� ������ �� �����
UNet* CreateEngineControlSignumAfferent(void);

// ��������� ���� ���������� ���������� c ����������� ��������������� ������ � ��������
// �� ������ �� ���������
UNet* CreateEngineControlRangeAfferent(bool crosslinks=false, bool crossranges=true);

// ��������� ���� ���������� �� �������� � ����������� ������������ �������� ��������
UNet* CreateEngineControlContinuesNeuronsSimple(bool crossranges=true);

// ��������� ���� ���������� �� 2 ���������� ��������
UNet* CreateEngineControl2NeuronsSimplest(bool use_speed_force=false, bool use_add_contours=false);

//------------------------------
//����� ������ ������������ ����

// ��������� ���� ���������� ����� �������� �� 2 ���������� ��������
UNet* CreateNewEngineControl2NeuronsSimplest(bool use_speed_force);

// ��������� ����������
void NewMotionElementsSetup(UEPtr<UContainer> net, int inp_mode, int out_mode, double exp_coeff, double receptor_max_output, double receptor_gain, int real_ranges);

// ��������� ��������������� �������-������
void NewPACSetup(double pulse_amplitude, double secretion_tc, double dissociaton_tc, double gain_value, bool gain_div_mode, int tc_mode);

// ��������� ����������� ������
void NewStandardLinksSetup(const string &engine_integrator_name);

public:
// ��������� ������������ ����������
void NewIntervalSeparatorsSetup(int mode_value, double pos_gain_value, double neg_gain_value);

// ��������� ������������ ����������
void NewIntervalSeparatorsUpdate(int mode_value);

// ��������� ������ ������������ ����������
void NewIntervalSeparatorLinksSetup(void);
// --------------------------

};

}
#endif
