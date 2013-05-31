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
RDK::ULProperty<bool, NEngineMotionControl> AdaptiveStructureMode;

// �������� ����������� �������� �� ������ Ia
RDK::ULProperty<real, NEngineMotionControl> IaMin;
RDK::ULProperty<real, NEngineMotionControl> IaMax;

// �������� ����������� �������� �� ������ Ib
RDK::ULProperty<real, NEngineMotionControl> IbMin;
RDK::ULProperty<real, NEngineMotionControl> IbMax;

// �������� ����������� �������� �� ������ II
RDK::ULProperty<real, NEngineMotionControl> IIMin;
RDK::ULProperty<real, NEngineMotionControl> IIMax;

// �������� ����������� �������� �� ������ Ic
RDK::ULProperty<real, NEngineMotionControl> IcMin;
RDK::ULProperty<real, NEngineMotionControl> IcMax;

// ������������ �������� ������������ �����������
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



protected: // ��������� ����������
vector<vector<UEPtr<NReceptor> > > receptors;

vector<pair<double,double> > Ia_ranges_pos,Ia_ranges_neg;
vector<pair<double,double> > Ib_ranges_pos,Ib_ranges_neg;
vector<pair<double,double> > II_ranges_pos,II_ranges_neg;
vector<pair<double,double> > Ic_ranges_pos,Ic_ranges_neg;

vector<NNet*> Motions;


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
/// control_grain - ��������� �������� ������� ����������
///
virtual void AdaptiveTuningSimple(const std::vector<double> &current_contour_amplitude,
								  const std::vector<bool> &use_contour_data,
								  double current_transient_time,
								  const std::vector<double> &dest_contour_amplitude,
								  double dest_transient_time,
								  int &num_motion_elements,
								  double &control_grain);
// --------------------------

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
		double pulse_amplitude, double secretion_tc, double dissociaton_tc, double gain_value, bool gain_div_mode=true);

// ��������� ��������������� ������-������
void AACSetup(UEPtr<UContainer> net, double gain_value);

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
// --------------------------

};

}
#endif
