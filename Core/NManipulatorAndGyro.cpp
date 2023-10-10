/* ***********************************************************
File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NManipulatorAndGyro_CPP
#define NManipulatorAndGyro_CPP
#include "NManipulatorAndGyro.h"
namespace NMSDK {
// ������
// --------------------------
// ������������ � �����������
// --------------------------
NManipulatorAndGyro::NManipulatorAndGyro(void)
 : Mass("Mass",this),
   Length ("Length", this),
   InputMomentumExt("InputMomentumExt",this),
   InputMomentum("InputMomentum",this),
   InputAngle("InputAngle",this),
   Output("Output",this)
{
 gravity_constant=0;
}
NManipulatorAndGyro::~NManipulatorAndGyro(void)
{
}
// --------------------------    

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
NManipulatorAndGyro* NManipulatorAndGyro::New(void)
{
 return new NManipulatorAndGyro;
}
// --------------------------

// --------------------------
// ������� ������ ���������� ������ 
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool NManipulatorAndGyro::ADefault(void)
{
 Mass = 1;
 Length = 1;

 InputMomentumExt->Assign(1,1,0.0);
 InputMomentum->Assign(1,1,0.0);
 InputAngle->Assign(1,1,0.0);

 Output.Assign(1,1,0.0);

 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NManipulatorAndGyro::ABuild(void)
{
 return true;
}

// ����� �������� ����� ��� ������ ��������
bool NManipulatorAndGyro::AReset(void)
{
 gravity_constant = 9.8;
 return true;
}

// ��������� ������ ����� �������
bool NManipulatorAndGyro::ACalculate(void)
{
 vector<double> input;

 input.resize(3);

 input[0]=(*InputMomentumExt)(0,0);
 input[1]=(*InputMomentum)(0,0);
 input[2]=(*InputAngle)(0,0);

 double external_moment = input[0]; //������� ������
 double engine_moment = input[1]; //������ �� ���� ���������
 double angle = input[2]; //������� ���� �������� ���������

 Output(0,0) = - external_moment + engine_moment - Mass*gravity_constant*Length*sin(angle);

 return true;
}
// --------------------------
}
#endif
