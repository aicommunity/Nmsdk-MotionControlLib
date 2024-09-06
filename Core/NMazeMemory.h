// ===========================================================
// Version:        3.0.2
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2002.
E-mail:		alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NMAZEMEMORY_H
#define NMAZEMEMORY_H


#include "NTrajectoryElement.h"
#include "NMultiPositionControl.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"
#include "../../../Rdk/Deploy/Include/rdk.h"
#include <iostream>
#include <fstream>
#include <cstring>

namespace NMSDK {

//using namespace RDK;

// �����, ��������� ������ �������� ��� ������������� ��������� ��������
class RDK_LIB_TYPE NMazeMemory: public UNet
{
public: // ���������
/// ���������� ��������� ��������� ������ � ��������
/// (��������� ����������� ��������)
ULProperty<int, NMazeMemory, ptPubParameter> OptionsNum;

/// ���� �������� � ������������� ������� (�����������, ��������)
ULProperty<bool, NMazeMemory, ptPubParameter> Situation;

/// ���� ����������� �������� (���������)
//ULProperty<bool, NMazeMemory, ptPubParameter> RememberSituation;

/// ���������� ���������, ����������� ������������ ��������
ULProperty<int, NMazeMemory, ptPubParameter> FeaturesNum;

/// ���� ��������� � �����
ULProperty<bool, NMazeMemory, ptPubParameter> IsDeadlock;


protected:
///��� �������� ���������� �� �����
std::vector<UEPtr<NTrajectoryElement>> TrajectoryElements;

/// �������� PostInput ������� � �����
std::vector<UEPtr<NTrajectoryElement>> ActivePIs;

/// ����� MultiPC (������������ ����������� ��������� ��������)
std::vector<UEPtr<NMultiPositionControl>> MultiPCs;

///������ �������� �������� ���������� � �������
int CurrentTE;

/// ����� �������� ����
int CurrentLayer;

///���������� �� ��������� ����� ������ �� ��� X (����� ��� ���������� ������)
double LayerShift;

///���������� �� ��������� ����� ������� �� ��� Y (����� ��� ���������� ������)
double yShift;

///��� �� ��������� �������������� ������ (�� ����������� ������ ���������)
double SideWeight;

///���������� �������� ����������
std::vector<UEPtr<NTrajectoryElement>> PassedTEs;

///������� ������� ����������
UEPtr<NTrajectoryElement> base_TE;

///MultiPC, ��������������� �������� �������� ����������
UEPtr<NMultiPositionControl> base_MPC;



public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NMazeMemory(void);
virtual ~NMazeMemory(void);
// --------------------------

// --------------------------
// ������ ��������� �����������
// --------------------------
/// ���������� ��������� ��������� (����������� ��������)
bool SetOptionsNum(const int &value);

/// ���� �������� � ������������� ������� (�����������, ��������)
bool SetNewSituation(const bool &value);

/// ���� �������� � ������������� ������� (�����������, ��������)
//bool SetRememberSituation(const bool &value);

/// ���������� ���������, ����������� ������������ ��������
bool SetFeaturesNum(const int &value);

/// ���� ��������� � �����
bool SetIsDeadlock(const bool &value);


// --------------------------

// --------------------------
// ������ ������� � ��������� ����������
// --------------------------

// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NMazeMemory* New(void);
static UComponent* NewStatic(void);
// --------------------------

// --------------------------
// ������� ������ ���������� ������������
// --------------------------
protected:
// ��������� ����������� ���������������� ��������
// ��� ���������� ��������� ���������� � ���� ������
// ����� ����� ������ ������ ���� comp ���
// ������� �������� � ������ ���������
virtual bool AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer=0);

// ��������� ��������������� ���������������� ��������
// ��� �������� ��������� ���������� �� ����� �������
// ����� ����� ������ ������ ���� comp
// ���������� � ������ ���������
virtual bool ADelComponent(UEPtr<UContainer> comp);
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

// ����� �������� �����.
virtual bool AReset(void);

// ��������� ������ ����� �������
virtual bool ACalculate(void);

//�������� ����� ����� �������� (�������� ��������), �.�.
//���������� ������ TrajectoryElement+MultiPC
UEPtr<NTrajectoryElement> CreatePoint(MVector<double,3> coords);

//������� ����������� ��������� ����������
//��������� �� ���� ���������� ����� �� � ������� TrajectoryElements,
//�������� ������������� �������� PostInput ������
//�.�. ����� TE, � ������� ��������� ������� ��������
bool MergingTEs(int active_num);

//���������, ���� �� �������� PostInput ������� � ����
std::vector<UEPtr<NTrajectoryElement>> CheckActivePIs();

//��������� ����� �������� (��������)
bool ProcessOptions();

//��������� ������
bool DeadlockProcessing();

// --------------------------
};

}
#endif
