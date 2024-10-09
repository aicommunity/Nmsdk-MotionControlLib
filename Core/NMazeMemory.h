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
#include "../../Nmsdk-PulseLib/Core/NNeuronTrainer.h"
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
/// ���� �������� � ������������� ������� (�����������, ��������)
ULProperty<bool, NMazeMemory, ptPubParameter> Situation;

/// ������ ��������� ��������� �������� � �������� (����������� ��������)
/// ���-�� ��������� = ���-�� ��������� ���������,
/// �������� �������� �������:
/// 1 - ���� ����������� ��������� ��� ��������, 0 - ��� �����������
/// [0] - ������� �������, [1] - ������� ������, [2] - ����� �����, [3] - ����
ULProperty<std::vector<int>,NMazeMemory, ptPubParameter> InputActions;

///��� �������� ��������
RDK::ULProperty<string, NMazeMemory, ptPubParameter> ActionNeuronsType;

/// ���������� ���������, ����������� ������������ ��������
ULProperty<int, NMazeMemory, ptPubParameter> FeaturesNum;

/// ���� ���������� ������ (��� ��������� �������� �����������)
ULProperty<bool, NMazeMemory, ptPubParameter> IsDone;

///������ ���������� �������� (������ = {x,y,alpha}, ��� alpha - ���������� ������
ULProperty<MDMatrix<double>, NMazeMemory, ptPubParameter> SituationCoords;

protected:
///������� ��������
std::vector<UEPtr<NPulseNeuron>> ActionNeurons;

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
UEPtr<NTrajectoryElement> BaseTE;

///MultiPC, ��������������� �������� �������� ����������
UEPtr<NMultiPositionControl> BaseMPC;

///���������� ������� ����������
UEPtr<NTrajectoryElement> PrevTE;


MDMatrix<double> check_pattern;


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
/// ���� �������� � ������������� ������� (�����������, ��������)
bool SetSituation(const bool &value);

/// ������ ��������� ��������� �������� � �������� (����������� ��������)
/// ���-�� ��������� = ���-�� ��������� ���������,
/// �������� �������� �������:
/// 1 - ���� ����������� ��������� ��� ��������, 0 - ��� �����������
/// [0] - ������� �������, [1] - ������� ������, [2] - ����� �����, [3] - ����
bool SetInputActions(const std::vector<int> &value);

///��� �������� ��������
bool SetActionNeuronsType(const string &value);

/// ���������� ���������, ����������� ������������ ��������
bool SetFeaturesNum(const int &value);

/// ���� ���������� ������ (��� ��������� �������� �����������)
bool SetIsDone(const bool &value);

///������ ���������� �������� (������ = {x,y,alpha}, ��� alpha - ���������� ������
bool SetSituationCoords(const MDMatrix<double> &value);

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

//���������, ���� �� �������� �������� ���������� ����� ��������� ��������� ��������
bool CheckActiveForwards(UEPtr<NTrajectoryElement> t_element);

//��� �����, �� ������� ������ � ��� �����, w = 0,2
bool LastUsedLink();

//��������� ����� �������� (��������)
bool ProcessOptions();

//��������� ������
bool DeadlockProcessing();

// --------------------------
};

}
#endif
