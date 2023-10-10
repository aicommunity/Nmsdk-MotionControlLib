// ===========================================================
// Version:        1.0.1
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

#ifndef NDCEngineH
#define NDCEngineH

#include "../../../Rdk/Deploy/Include/rdk.h"

//---------------------------------------------------------------------------
namespace NMSDK {

using namespace RDK;

/* *************************************************************************** */
class RDK_LIB_TYPE NDCEngine: public UNet
{
public: // ������������� ��������
/// ���������������� �����������
ULProperty<double, NDCEngine, ptPubParameter> EMFactor;

/// �������������
ULProperty<double, NDCEngine, ptPubParameter> Inductance;

/// �������������
ULProperty<double, NDCEngine, ptPubParameter> Resistance;

/// ������������������� ���������� �������
ULProperty<double, NDCEngine, ptPubParameter> Tm;

/// ������������ �����
ULProperty<double, NDCEngine, ptPubParameter> ReductionRate;

/// ������� ������
ULProperty<double, NDCEngine, ptPubParameter> OutMoment;

public: // ����� � ������
/// ������� ����������
UPropertyInputData<MDMatrix<double>, NDCEngine, ptInput | ptPubState> InputVoltage;

/// ������� ������ �� ����
UPropertyInputData<MDMatrix<double>, NDCEngine, ptInput | ptPubState> InputMomentum;

/// �������� ������
UPropertyOutputData<MDMatrix<double>, NDCEngine, ptOutput | ptPubState> OutputMomentum;

/// �������� ���� �������� ����
UPropertyOutputData<MDMatrix<double>, NDCEngine, ptOutput | ptPubState> OutputAngle;

/// �������� ������� �������� ��������
UPropertyOutputData<MDMatrix<double>, NDCEngine, ptOutput | ptPubState> OutputAngleSpeed;

protected: // ������
// ��� �����
double Current;

// ��� �����
double EMF;

// ���� ��������
double Angle;

// ������ ����������� ����������
double Moment;

// ������� ��������
double AngleSpeed;

// ����������� �������
double DiffMoment;

protected: // ��������� ����������

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NDCEngine(void);
virtual ~NDCEngine(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NDCEngine* New(void);
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

// ������������������� ���������� �������
bool SetTm(const double &value);

// ������������ �����
bool SetReductionRate(const double &value);
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
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
