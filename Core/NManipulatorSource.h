// ===========================================================
// Version:        3.0.2
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2002.
E-mail:        alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NMANIPULATOR_SOURCE_H
#define NMANIPULATOR_SOURCE_H

#include "../../Nmsdk-PulseLib/Core/NSource.h"


namespace NMSDK {

class RDK_LIB_TYPE NManipulatorSource: public NSource
{
public: // ��������������� ����������� �������� � ������
/// �������� � ���� ��������� ������
ULProperty<double,NManipulatorSource, ptPubParameter> Angle;

/// �������� � ������� ��������
ULProperty<double,NManipulatorSource, ptPubParameter> Speed;

/// �������� � ������� ���������
ULProperty<double,NManipulatorSource, ptPubParameter> Force;

/// �������� � �������� �������
ULProperty<double,NManipulatorSource, ptPubParameter> Movement;

public: // ����� � ������
/// ����: ���� ��������� ������
UPropertyInputData<MDMatrix<double>,NManipulatorSource> InputAngle;

/// ����: ������� ��������
UPropertyInputData<MDMatrix<double>,NManipulatorSource> InputSpeed;

/// ����: ������ ���������
UPropertyInputData<MDMatrix<double>,NManipulatorSource> InputForce;

/// ����: �������� �������
UPropertyInputData<MDMatrix<double>,NManipulatorSource> InputMovement;

/// ����: ���� ��������� ������
UPropertyOutputData<MDMatrix<double>,NManipulatorSource, ptOutput | ptPubState> OutputAngle;

/// ����: ������� ��������
UPropertyOutputData<MDMatrix<double>,NManipulatorSource, ptOutput | ptPubState> OutputSpeed;

/// ����: ������ ���������
UPropertyOutputData<MDMatrix<double>,NManipulatorSource, ptOutput | ptPubState> OutputForce;

/// ����: �������� �������
UPropertyOutputData<MDMatrix<double>,NManipulatorSource, ptOutput | ptPubState> OutputMovement;

public: // ������
bool UpdateOutputFlag;
// --------------------------
// ������������ � �����������
// --------------------------
NManipulatorSource(void);
virtual ~NManipulatorSource(void);
// --------------------------

protected:
// --------------------------
// ������ ���������� �������������� ����������
// --------------------------
// ������������� ����
bool SetAngle(const double &value);

// ������������� ��������
bool SetSpeed(const double &value);

// ������������� ������
bool SetForce(const double &value);
// --------------------------

public:
// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NManipulatorSource* New(void);
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
protected:
// �������������� �������� �� ��������� � ����� �������� �����
virtual bool ADefault(void);

// ����� �������� �����.
virtual bool AReset(void);

// ��������� ������ ����� �������
virtual bool ACalculate(void);
// --------------------------
};

}
#endif

