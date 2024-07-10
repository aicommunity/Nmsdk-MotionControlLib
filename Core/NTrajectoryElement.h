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
#ifndef NTrajectoryElementH
#define NTrajectoryElementH

#include "../../Nmsdk-PulseLib/Core/NAfferentNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {

class RDK_LIB_TYPE NTrajectoryElement: public UNet
{
public: // ���������
/// ��� ������ �������
ULProperty<std::string, NTrajectoryElement, ptPubParameter> NeuronClassName;

/// �������� ������ �� ��������� ������� ����������
UPropertyOutputData<MDMatrix<double>, NTrajectoryElement, ptOutput | ptPubState> Output;

//Input_u_top, Input_u_tcn � Input_y_pcn2 - ��������� �� ����� ����� � �������� �������������� ��������� ��������� (�������� �����),
//������ ��� ��������� ����������� ����� NTrajectoryElement � ������� ����������.
//������ ���������, �.�. ��� ����������� � ���� ���������� Input ����� � Input �������.
//����� ���� �������, ���� ����� ����������� �������� - ����� � ��� ���������� �������� ���������� ������ �����
//Input_u_top, Input_u_tcn � Input_y_pcn2 � ���������������� ���������

/// ������� ������ � ������� ������ ���������� (
//UPropertyInputData<MDMatrix<double>,NTrajectoryElement, ptInput | ptPubState> Input_u_top;

/// ������� ������ c ����������� �������� ����������
//UPropertyInputData<MDMatrix<double>,NTrajectoryElement, ptInput | ptPubState> Input_u_tcn;

/// ������� ������ � ����������� ������ ������� ���������� - PCN2 (����� NMultiPositionControl),
/// ������������� � ���������� �������� �������� ����������
//UPropertyInputData<MDMatrix<double>,NTrajectoryElement, ptInput | ptPubState> Input_y_pcn2;


protected:
///���������� ������� �������� ����������
std::vector<UEPtr<NPulseNeuron>> Neurons;

///������ ���� �������� 1 � 2
int SomaSize;

///������ ��������� �������� 1 � 2
std::vector<int> DendSizes;

///������� ��� ����������� ������� ������
/// n - � �������, d - � ��������, seg - � �������� ��������,
/// s - � ����, exc ��� inh - ��� �������
//UEPtr<NPulseSynapse> n1_d1_seg2_exc;
//UEPtr<NPulseSynapse> n1_d1_seg3_exc;
//UEPtr<NPulseSynapse> n1_s3_inh;
//UEPtr<NPulseSynapse> n1_s4_inh;
//UEPtr<NPulseSynapse> n1_d1_seg3_inh;
//UEPtr<NPulseSynapse> n1_d1_seg1_inh;
//UEPtr<NPulseSynapse> n2_d1_seg3_inh;
//UEPtr<NPulseSynapse> n2_d1_seg1_inh;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NTrajectoryElement(void);
virtual ~NTrajectoryElement(void);
// --------------------------

// --------------------------
// ������ ���������� �����������
// --------------------------
/// ��� ������, ���������� ������
bool SetNeuronClassName(const std::string &value);


// --------------------------

// --------------------------
// ��������� ������ ���������� ��������

// --------------------------
public:
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NTrajectoryElement* New(void);
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

};

}
//---------------------------------------------------------------------------
#endif
