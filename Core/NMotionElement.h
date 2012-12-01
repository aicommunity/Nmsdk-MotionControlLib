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
#ifndef NMotionElementH
#define NMotionElementH

#include "../../Kernel/NBios.h"
#include "../PulseLib/NAfferentNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {

class NMotionElement: public NANet
{
public: // ���������
// ����� �������� ����������
RDK::ULProperty<int, NMotionElement> NumControlLoops;

// ������ ������ ��������� �������� ����������
RDK::ULProperty<std::vector<int>, NMotionElement> EnableControlLoopFlags;

// ����� ������� ���������� ��������
// ��������, ���� ����� ���������� ����, ��� ������� ������ ������ �����
// ���� ����������
// 0 - ���������� �������� ���
// 1 - ���������� ������� ����
RDK::ULProperty<int, NMotionElement> InterneuronPresentMode;

// ����� ������� ����������� ����������
// 0 - ����������� ���������� ���
// 1 - ���������� ���������� ������ �� �������� (�����������)
// 2 - ���������� ���������� ������������ �� ������ ������� �������
RDK::ULProperty<int, NMotionElement> RecurrentInhibitionMode;

// ����� ��������� ����������� ��� ����������� ����������� ����������
// 0 - ���������� ��������� ���� �� ��� �� ��������, ��� � ������ ����������
// (���� �������� N, �� �� ������ ������ ���� ��������� ����).
// 1 - ���������� ��������� ���� �� ��������� �������� �������
RDK::ULProperty<int, NMotionElement> RecurrentInhibitionBranchMode;

// ����� ��������� ����������� ��� ����������� �������� ����������
// 0 - ��� ������� ������������ � ������ ������� ��������
// 1 - ������ ������ ����� ���� ������� ��������
RDK::ULProperty<int, NMotionElement> MotoneuronBranchMode;

// ����� ����������� ���������� � ������� �������
// 0 - ���������� �� ������� ������������ �������� �������� ���� �� ���� ����
// ����� � ���������.
// 1 - ���������� �� ������� ������������ �� ��������� ������� � ���� �� �������
// ������� ��������� ����� � ���������
RDK::ULProperty<int, NMotionElement> ExternalControlMode;


public: // ���������� ���������

protected: // ��������� ����������
// ��������� �� ����������� �������
UCPointer<NAfferentNeuron, NMotionElement> Afferents;

// ��������� �� ���������� �������� ����������
UCPointer<NPulseGenerator, NMotionElement> ExternalControlGenerators;

// ��������� �� �������������� ������� (�����������)
UCPointer<NPulseNeuron, NMotionElement> Motoneurons;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NMotionElement(void);
virtual ~NMotionElement(void);
// --------------------------

// --------------------------
// ������ ���������� �����������
// --------------------------
bool SetNumControlLoops(int value);
bool SetEnableControlLoopFlags(const std::vector<int> &value);
bool SetInterneuronPresentMode(int value);
bool SetRecurrentInhibitionMode(int value);
bool SetRecurrentInhibitionBranchMode(int value);
bool SetMotoneuronBranchMode(int value);
bool SetExternalControlMode(int value);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
public:
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NMotionElement* New(void);
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

// ������� ��������� � ������������ � �������� ���������� ����������
// ���� ��������� ����������, �� �������� �������������� �� � ������������ �����������
void CreateStructure(void);

// ������� ���������� ����� � ������������ � �������� ���������� ����������
void CreateInternalLinks(void);

// ��������� � ��������������� ������� �����
void BackupExternalLinks(void);
void RestoreExternalLinks(void);
// --------------------------
};

// ��������� �� ������������ ��������
// ���� mode == 0 - ����������� ������ ������
// 	    mode == 1 - ������ ��� ���������� �������������
UEPtr<NANet> CreateMotionElement(NStorage *storage, const string &netclassname, int mode);

// ����������, �� � ��������� �� ���������
UEPtr<NANet> CreateBranchedMotionElement(NStorage *storage, const string &netclassname,
	const string &neuron_class_name, const string &afferent_neuron_class_name, int mode);

// ��������� ���������� �� ������������ �������� �� ���� ������������
UEPtr<NANet> CreateSimplestMotionElement(NStorage *storage, const string &netclassname, int mode);
UEPtr<NANet> CreateSimplestBranchedMotionElement(NStorage *storage, const string &netclassname, int mode, bool use_speed_force=false, bool use_add_contours=false);

bool CreateNeuronBranchLink(UEPtr<NANet> net,const string &source,
	const string &target_head, const string &target_tail);

}
//---------------------------------------------------------------------------
#endif
