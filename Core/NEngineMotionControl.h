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
#ifndef NEngineMotionControlH
#define NEngineMotionControlH

#include "../Kernel/NBios.h"
#include "../Libraries/ReceiverLib/NReceptor.h"

namespace NMSDK {

class NEngineMotionControl: public NANet
{
public: // ��������� ��������
// ����� ����������� ���������
RDK::ULProperty<size_t, NEngineMotionControl> NumMotionElements;

// ����� ������������ ����
// 0 - Signum
// 1 - Range
// 2 - Branched Range
// 3 - Branched Range Crosslinks
// 4 - Branched Ind. Range
// 5 - Branched Ind. Range Crosslinks
RDK::ULProperty<int, NEngineMotionControl> CreationMode;

// ��� ������ ������������ ��������
RDK::ULProperty<NameT, NEngineMotionControl> MotionElementClassName;

// �������� ����������� �������� �� ������ Ia
RDK::ULProperty<real, NEngineMotionControl> IaMin;
RDK::ULProperty<real, NEngineMotionControl> IaMax;

// �������� ����������� �������� �� ������ Ib
RDK::ULProperty<real, NEngineMotionControl> IbMin;
RDK::ULProperty<real, NEngineMotionControl> IbMax;

// �������� ����������� �������� �� ������ II
RDK::ULProperty<real, NEngineMotionControl> IIMin;
RDK::ULProperty<real, NEngineMotionControl> IIMax;

protected: // ��������� ����������
vector<vector<NReceptor*> > receptor;


public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NEngineMotionControl(void);
virtual ~NEngineMotionControl(void);
// --------------------------

// --------------------------
// ������ ���������� �����������
// --------------------------
protected:
// ����� ����������� ���������
bool SetNumMotionElements(size_t value);

// ����� ������������ ����
bool SetCreationMode(int value);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
public:
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NEngineMotionControl* New(void);
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
// --------------------------

// --------------------------
// ������ ���������� ������
// --------------------------
public:
// ������� ������ � �������� ���������� � ������������ � CreationMode
virtual bool Create(void);
// --------------------------

// --------------------------
// ��������������� ������
// --------------------------
protected:
// ��������� ���� ���������� ���������� � ����������� ��������������� ������ � ��������
// �� ��� ������ �� �����
NANet* CreateEngineControlSignumAfferent(void);

// ��������� ���� ���������� ���������� c ����������� ��������������� ������ � ��������
// �� ������ �� ���������
NANet* CreateEngineControlRangeAfferent(bool crosslinks=false, bool crossranges=true);
// --------------------------

};

}
#endif
