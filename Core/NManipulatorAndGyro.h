/* ***********************************************************
File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NManipulatorAndGyro_H
#define NManipulatorAndGyro_H

#include "../../../Rdk/Deploy/Include/rdk.h"

namespace NMSDK {

using namespace RDK;

class RDK_LIB_TYPE NManipulatorAndGyro: public UNet
{
public: // ��������
ULProperty<double, NManipulatorAndGyro, ptPubParameter> Mass;

ULProperty<double, NManipulatorAndGyro, ptPubParameter> Length;

public: // ����� � ������
/// ������� ������
UPropertyInputData<MDMatrix<double>, NManipulatorAndGyro, ptInput | ptPubState> InputMomentumExt;

/// ������ �� ���� ���������
UPropertyInputData<MDMatrix<double>, NManipulatorAndGyro, ptInput | ptPubState> InputMomentum;

/// ������� ���� �������� ����
UPropertyInputData<MDMatrix<double>, NManipulatorAndGyro, ptInput | ptPubState> InputAngle;

UPropertyOutputData<MDMatrix<double>, NManipulatorAndGyro, ptOutput | ptPubState> Output;


protected: // ���������� ���������
 double gravity_constant;

 public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NManipulatorAndGyro(void);
virtual ~NManipulatorAndGyro(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NManipulatorAndGyro* New(void);
// --------------------------

// --------------------------
// ������� ������ ���������� ������ 
// --------------------------
protected:
// �������������� �������� �� ��������� � ����� �������� �����
virtual bool ADefault(void);

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
virtual bool ABuild(void);

// ����� �������� ����� ��� ������ ��������
virtual bool AReset(void);

// ��������� ������ ����� �������
virtual bool ACalculate(void);
// --------------------------
};
}
#endif
