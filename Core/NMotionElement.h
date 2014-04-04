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

#include "../../Kernel/NSupport.h"
#include "../PulseLib/NAfferentNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {

class NMotionElement: public UNet
{
public: // ���������
// ����� �������� ����������
RDK::ULProperty<int, NMotionElement> NumControlLoops;

// ������ ������ ��������� �������� ����������
RDK::ULProperty<std::vector<int>, NMotionElement> EnableControlLoopFlags;

// ������ ������� ����� ����� ���������
// 0 - ������ ����� (��� �������������)
// 1 - ����� ����� ������������
// 2 - ����� ����� ������������(L-R) + ������ �����(L-L)
// 3 - ����� ����� ������������(L-L) + ������ �����(L-R)
RDK::ULProperty<std::vector<int>, NMotionElement> LinkModes;

// ����� ������� ���������� ��������
// ��������, ���� ����� ���������� ����, ��� ������� ������ ������ �����
// ���� ����������
// 0 - ���������� �������� ���
// 1 - ���������� ������� ����
RDK::ULProperty<int, NMotionElement> InterneuronPresentMode;

// ����� ������� ������ ������
// 0 - ������ ������ ���
// 1 - ������ ������ ����
RDK::ULProperty<int, NMotionElement> RenshowMode;

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

// ��� ������� ��� ��������
RDK::ULProperty<string, NMotionElement> NeuroObjectName;
// ��� ������� ��� ����������� ��������
RDK::ULProperty<string, NMotionElement> AfferentObjectName;

public: // ���������� ���������

protected: // ��������� ����������
// ��������� �� ����������� �������
UCPointer<NAfferentNeuron, NMotionElement> Afferents;

// ��������� �� ���������� �������� ����������
UCPointer<NPulseGenerator, NMotionElement> ExternalControlGenerators;

// ��������� �� �������������� ������� (�����������)
UCPointer<NPulseNeuron, NMotionElement> Motoneurons;

string netclassname;
string neuron_class_name;

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
bool SetNumControlLoops(const int &value);
bool SetEnableControlLoopFlags(const std::vector<int> &value);
bool SetRenshowMode(const int &value);
bool SetLinkModes(const std::vector<int> &value);
bool SetInterneuronPresentMode(const int &value);
bool SetRecurrentInhibitionMode(const int &value);
bool SetRecurrentInhibitionBranchMode(const int &value);
bool SetMotoneuronBranchMode(const int &value);
bool SetExternalControlMode(const int &value);
bool SetNeuroObjectName(const string &value);
bool SetAfferentObjectName(const string &value);
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
// ������ �������� ��������� ��
// --------------------------
// �������� ���� ������������
bool CreateMotoneurons();
// �������� ������ ����������� ��������
bool CreateAfferents();
// �������� �������������
bool CreateInterneurons();
// �������� ������ ����� ������������ ��������� � �������������
// 0 - ������ ����� (��� �������������)
// 1 - ����� ����� ������������
// 2 - ����� ����� ������������(L-R) + ������ �����(L-L)
// 3 - ����� ����� ������������(L-L) + ������ �����(L-R)
bool LinkMotoneurons(const string &afferentL, const string afferentR, int mode);

// �������� ������ � �������� ������
bool LinkRenshow();

};

// ��������� �� ������������ ��������
// ���� mode == 0 - ����������� ������ ������
// 	    mode == 1 - ������ ��� ���������� �������������
UEPtr<UNet> CreateMotionElement(UStorage *storage, const string &netclassname, int mode);

// ����������, �� � ��������� �� ���������
UEPtr<UNet> CreateBranchedMotionElement(UStorage *storage, const string &netclassname,
	const string &neuron_class_name, const string &afferent_neuron_class_name, int mode);

// ��������� ���������� �� ������������ �������� �� ���� ������������
UEPtr<UNet> CreateSimplestMotionElement(UStorage *storage, const string &netclassname, int mode);
UEPtr<UNet> CreateSimplestBranchedMotionElement(UStorage *storage, const string &netclassname, int mode, bool use_speed_force=false, bool use_add_contours=false);
UEPtr<UNet> CreateSimplestBranchedMotionElementPM(UStorage *storage, const string &netclassname, int mode, bool use_speed_force=false, bool use_add_contours=false);

bool CreateNeuronBranchLink(UEPtr<UNet> net,const string &source,
	const string &target_head, const string &target_tail);

}
//---------------------------------------------------------------------------
#endif
