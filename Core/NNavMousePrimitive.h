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

#ifndef NNavMousePrimitiveH
#define NNavMousePrimitiveH
#include "../../../Rdk/Deploy/Include/rdk.h"

//---------------------------------------------------------------------------
namespace NMSDK {

using namespace RDK;

/* *************************************************************************** */
class RDK_LIB_TYPE NNavMousePrimitive: public UNet
{
public: // ������������� ��������
/// ������ ���� ����������� "����", �� ������ (0) �� ����� (PoleSize)
/// ����� ����� � ��, ��� ��� ����� ����������� ������
ULProperty<double, NNavMousePrimitive, ptPubParameter> PoleSize;

/// ������ "����" ("������" ��� �������)
ULProperty<double, NNavMousePrimitive, ptPubParameter> MouseSize;

/// ������ �������� "����", �������� �������� � "������" ����� �������� ������
/// ���������� ������ � ����� �� ���������
ULProperty<double, NNavMousePrimitive, ptPubParameter> VibrissSize;

/// �������� �������� �� 1 "�������" ���� (�.�. �� Freqiency)
ULProperty<double, NNavMousePrimitive, ptPubParameter> Velocity;

/// �������, � ������� ���� �������� ��� ��������� ������
ULProperty<double, NNavMousePrimitive, ptPubParameter> Frequency;

/// �������������� ��������, � ������� ���� �������� ��������
/// (0.1 �� ������� ��� ����� ����������� �������� � �������)
ULProperty<double, NNavMousePrimitive, ptPubParameter> Delay;

/// ��������, � ������� �������� ������ ����
/// (0.1 - �������� ������������ ������� ������������� 0.2 �� ���������)
ULProperty<double, NNavMousePrimitive, ptPubParameter> PainDelay;

/// ����������� �������� ��� ��������� - ����� ��������
/// �� ��������� - 0.001
ULProperty<double, NNavMousePrimitive, ptPubParameter> PulseLength;

/// ������������ ������� ���� �� ��������� (�� ��������� - ��)
ULProperty<bool, NNavMousePrimitive, ptPubParameter> UseExternalInput;

///��������� ���������
ULProperty<double, NNavMousePrimitive, ptPubParameter> Amplitude;

/// �� ����������� - ������� ���� ���������� ���������:
/// 0 - ����� �� �����
/// 1 - ���� ������ �� ��������� Velocity
/// -1 - ���� ����� �� ��������� Velocity
/// ��� ������ - ������ ���������
ULProperty<int, NNavMousePrimitive, ptPubParameter> MotionControlSimple;

public: // ����� � ������
/// ������� ������
UPropertyInputData<MDMatrix<double>, NNavMousePrimitive, ptInput | ptPubState> Input;

/// ����������� �������, ������ ���� �������������� ��� ��������� �� ������
/// ���� ����� ������ ����, ��
UPropertyInputData<MDMatrix<double>, NNavMousePrimitive, ptInput | ptPubState> MotionControlSpikeForward;
UPropertyInputData<MDMatrix<double>, NNavMousePrimitive, ptInput | ptPubState> MotionControlSpikeBackward;
UPropertyInputData<MDMatrix<double>, NNavMousePrimitive, ptInput | ptPubState> MotionControlSpikeStop;

/// ��������� �������� ����� ��������� ������
///  0 - ����� �� �����
///  1 - ���� ������ �� ��������� Velocity
/// -1 - ���� ����� �� ��������� Velocity
UPropertyOutputData<int, NNavMousePrimitive, ptPubState> MotionControlState;

///��������� ���� - ����� ������ ������ � ����� ���� ���������� � ����������
UPropertyOutputData<int, NNavMousePrimitive, ptPubState> PainState;

/// ��������� "����" � "��������"
UPropertyOutputData<double, NNavMousePrimitive, ptPubState> MousePosition;

/// ������� ������
UPropertyOutputData<MDMatrix<double>, NNavMousePrimitive, ptOutput | ptPubState> Output;

/// ������� ������ �������
/// ������ ����� �������� � �������� Frequency, ��������� �� ������ Delay
///  � �������� ��������� � ����������� �� ����, ����� ���������� �� �����������
UPropertyOutputData<MDMatrix<double>, NNavMousePrimitive, ptOutput | ptPubState> VibrissOutput;

/// ������� ������ ����
UPropertyOutputData<MDMatrix<double>, NNavMousePrimitive, ptOutput | ptPubState> PainOutput;

protected: // ��������� ����������

//�������� ��� �������, ����� ��������� ���������� ���������� ����������������
std::vector<double> VibrDelays;

//��������� �������� ������ ��� �������� ������� � ���, ����� �������
double ForwardSpikeTime;
double BackwardSpikeTime;
double StopSpikeTime;

//������� ��������, ���������� ���� ��� ��� �������� ��� �������� �������
double start_iter_time;

double OldFrequency;
double ResetTime;

//�������� ��������� ��� �������
std::vector<int> vibriss_counters;
int pain_counter;

// ����� ���������
// TODO: (�����-�� ����� ���� ����� �������� ������������)
UGraphics Graphics;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NNavMousePrimitive(void);
virtual ~NNavMousePrimitive(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NNavMousePrimitive* New(void);
// --------------------------

protected:
// --------------------------
// ������ ���������� �������������� ����������
// --------------------------

bool SetPoleSize(const double &value);
bool SetVibrissSize(const double &value);
bool SetMouseSize(const double &value);
bool SetVelocity(const double &value);

// --------------------------
// ������ �������
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

// ����� ���������� ��������� ����������. ������ �����:
// 1) �������������� ������ �� ������ ��������� ������
// 2) ����������� ��������, ����������� ������� �
// 3) ������������, ����� ������ ����� ��������������� �� ������ ��������� ������
virtual bool UpdateState(void);

// Execute math. computations of current object on current step
virtual bool ACalculate(void);
// --------------------------
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
