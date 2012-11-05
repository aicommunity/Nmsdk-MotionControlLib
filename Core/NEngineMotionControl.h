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

#include "../../Kernel/NBios.h"
#include "../ReceiverLib/NReceptor.h"

namespace NMSDK {

class NEngineMotionControl: public NANet
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
RDK::ULProperty<int, NEngineMotionControl> CreationMode;

// ��� ������ ������������ ��������
RDK::ULProperty<NameT, NEngineMotionControl> MotionElementClassName;

// �������� ����������� �������� �� ������ Ia
RDK::ULProperty<real, NEngineMotionControl> IaMin;
RDK::ULProperty<real, NEngineMotionControl> IaMax;

// �������� ����������� �������� �� ������ Ib
RDK::ULProperty<real, NEngineMotionControl> IbMin;
RDK::ULProperty<real, NEngineMotionControl> IbMax;

// �������� ����������� �������� �� ������ II
RDK::ULProperty<real, NEngineMotionControl> IIMin;
RDK::ULProperty<real, NEngineMotionControl> IIMax;

// ����� ��������� ���������� ����������� ��������
// 0 - ��������� ���������� �����
// 1 - ��������� � ��������� �����, � ����������� ������� � ���������
// MinAfferentRange �� ��������������� ������� *Max-*Min
RDK::ULProperty<int, NEngineMotionControl> AfferentRangeMode;

// ���������� �������� �� ��������������� ������� *Max-*Min
// ������������ ����������� ������ ��������� ���������� ��������
// ����������� � ��������� [0;1]
RDK::ULProperty<double, NEngineMotionControl,ptPubState> MinAfferentRange;

protected: // ��������� ����������
vector<vector<UEPtr<NReceptor> > > receptors;

vector<pair<double,double> > Ia_ranges_pos,Ia_ranges_neg;
vector<pair<double,double> > Ib_ranges_pos,Ib_ranges_neg;
vector<pair<double,double> > II_ranges_pos,II_ranges_neg;

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
bool SetNumMotionElements(size_t value);

// ����� ������������ ����
bool SetCreationMode(int value);

// ����� ��������� ���������� ����������� ��������
bool SetAfferentRangeMode(int value);

// ���������� �������� �� ��������������� ������� *Max-*Min
bool SetMinAfferentRange(double value);
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
virtual bool Create(void);
// --------------------------

// --------------------------
// ��������������� ������
// --------------------------
protected:
// ��������� ��������� �������� �����������
// ���������� ����� ������������ ����������
int CalcAfferentRange(int num_motions, bool cross_ranges, double a_min, double a_max,
			vector<pair<double,double> > &pos_ranges, vector<pair<double,double> > &neg_ranges, int range_mode);

// ��������� ����������
void MotionElementsSetup(UEPtr<NAContainer> net, int inp_mode, int out_mode, double exp_coeff, double receptor_max_output, double receptor_gain, int real_ranges);

// ��������� ��������������� �������-������
void PACSetup(UEPtr<NAContainer> net,
		double pulse_amplitude, double secretion_tc, double dissociaton_tc, double gain_value, bool gain_div_mode=true);

// ��������� ��������������� ������-������
void AACSetup(UEPtr<NAContainer> net, double gain_value);

// ��������� ������������ ����������
void IntervalSeparatorsSetup(UEPtr<NAContainer> net, int mode_value, double pos_gain_value, double neg_gain_value, bool II=true, bool Ia=true, bool Ib=true);

// ��������� ������������ ����������
void IntervalSeparatorsUpdate(UEPtr<NAContainer> net, int mode_value);

// ������� ��������������� ���������
void AdditionalComponentsSetup(UEPtr<NAContainer> net);

// ��������� ����������� ������
void StandardLinksSetup(UEPtr<NANet> net, const string &engine_integrator_name);

// ��������� ������ ������������ ����������
void IntervalSeparatorLinksSetup(UEPtr<NANet> net, bool II=true, bool Ia=true, bool Ib=true);

// ��������� ���� ���������� ���������� � ����������� ��������������� ������ � ��������
// �� ��� ������ �� �����
NANet* CreateEngineControlSignumAfferent(void);

// ��������� ���� ���������� ���������� c ����������� ��������������� ������ � ��������
// �� ������ �� ���������
NANet* CreateEngineControlRangeAfferent(bool crosslinks=false, bool crossranges=true);

// ��������� ���� ���������� �� �������� � ����������� ������������ �������� ��������
NANet* CreateEngineControlContinuesNeuronsSimple(bool crossranges=true);

// ��������� ���� ���������� �� 2 ���������� ��������
NANet* CreateEngineControl2NeuronsSimplest(void);
// --------------------------

};

}
#endif
