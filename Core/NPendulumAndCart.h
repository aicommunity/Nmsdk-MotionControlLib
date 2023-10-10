/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2012.
E-mail:            alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NPendulumAndCartH
#define NPendulumAndCartH

#include "../../../Rdk/Deploy/Include/rdk.h"

//---------------------------------------------------------------------------
namespace NMSDK {

using namespace RDK;

/* *************************************************************************** */
class RDK_LIB_TYPE NPendulumAndCart: public UNet
{
public: // ������������� ��������
/// ����� �������
ULProperty<double, NPendulumAndCart> CartMass;

/// ����� �������
ULProperty<double, NPendulumAndCart> RodMass;

/// ����� �������
ULProperty<double, NPendulumAndCart> RodLength;

/// ����������� �� X
ULProperty<double, NPendulumAndCart> OutXMovement;

/// ����� ������
/// 0 - ���������� ������� ��
/// 1 - ���������� ���������� PID
ULProperty<double, NPendulumAndCart> Mode;

/// Ki
ULProperty<double, NPendulumAndCart> Ki;

/// Kd
ULProperty<double, NPendulumAndCart> Kd;

/// Kp
ULProperty<double, NPendulumAndCart> Kp;

/// Ki
ULProperty<double, NPendulumAndCart> MovementKi;

/// Kd
ULProperty<double, NPendulumAndCart> MovementKd;

/// Kp
ULProperty<double, NPendulumAndCart> MovementKp;

/// ��� ���������� �� ����
ULProperty<double, NPendulumAndCart> AngleWeight;

/// ��� ���������� �� �����������
ULProperty<double, NPendulumAndCart> MovementWeight;

public: // ���������

/// ������� ������
ULProperty<double, NPendulumAndCart, ptPubState> ExtrenalMoment;

public: // ����� � ������
UPropertyInputData<MDMatrix<double>, NPendulumAndCart, ptInput | ptPubState> Input1;

UPropertyInputData<MDMatrix<double>, NPendulumAndCart, ptInput | ptPubState> Input2;

/// �������� ��������� �������
UPropertyOutputData<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> Acceleration;

/// ���� �������� �������
UPropertyOutputData<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> Angle;

/// �������� �������� �������
UPropertyOutputData<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> Speed;

/// ����������� �������
UPropertyOutputData<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> Movement;

/// ����������� �������
UPropertyOutputData<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> MovementSpeed;

protected: // ������
double theta0;
double x0;
double y,x,movement;
double y0;

protected: // ��������� ����������

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NPendulumAndCart(void);
//NPendulumAndCart(const string &name);
virtual ~NPendulumAndCart(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NPendulumAndCart* New(void);
// --------------------------

protected:
// --------------------------
// ������ ���������� �������������� ����������
// --------------------------
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
