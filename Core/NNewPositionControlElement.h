/* ***********************************************************

E-mail:        
Url:           

This file is part of the project: 

File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NNewPositionControlElement_H
#define NNewPositionControlElement_H

#include "..\..\RDK\Core\Engine\UNet.h"
#include "NEngineMotionControl.h"
#include "NMotionElement.h"
#include "NPositionControlElement.h"

namespace NMSDK {

//class NEngineMotionControl;
class NNewPositionControlElement: public NPositionControlElement
{
public: // ��������
RDK::UPropertyInput<NEngineMotionControl,NNewPositionControlElement> MotionControl;
//RDK::ULProperty<MDMatrix<double>, NNewPositionControlElement, ptPubState> CurrentPosition;
//RDK::ULProperty<MDMatrix<double>, NNewPositionControlElement> TargetPosition;
//RDK::ULProperty<string, NNewPositionControlElement> InputNeuronType;
//RDK::ULProperty<string, NNewPositionControlElement> ControlNeuronType;
//RDK::ULProperty<bool, NNewPositionControlElement> ExternalControl;
//RDK::ULProperty<bool, NNewPositionControlElement, ptPubState> RememberState;
//RDK::ULProperty<MDMatrix<double>, NNewPositionControlElement, ptPubState> Delta;

public: // ���������� ���������

// ������� ��������� ������������ ��������� ����� �������� � �������
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - ������� ���� �� 0 �������
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - ������� ���� �� 1 �������
vector<vector<NNet*> > LeftPostInputNeurons;

// ������� ��������� ������������ ��������� ������ �������� � �������
// PIN(0,0) PIN(0,1), ..., PIN(0,M) - ������� ���� �� 0 �������
// PIN(1,0) PIN(1,1), ..., PIN(1,M) - ������� ���� �� 1 �������
vector<vector<NNet*> > RightPostInputNeurons;


vector<UNet*> Generators;

// ���������� ����� �������� � �������
// G(0,0) G(0,1), ..., G(0,M) - ���������� ���� �� 0 �������
// G(1,0) G(1,1), ..., G(1,M) - ���������� ���� �� 1 �������
vector<vector<UNet*> > LeftGenerators;

// ���������� ������ �������� � �������
// G(0,0) G(0,1), ..., G(0,M) - ���������� ���� �� 0 �������
// G(1,0) G(1,1), ..., G(1,M) - ���������� ���� �� 1 �������
vector<vector<UNet*> > RightGenerators;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NNewPositionControlElement(void);
virtual ~NNewPositionControlElement(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
bool SetInputNeuronType(const string &value);
bool SetControlNeuronType(const string &value);
// ---------------------

// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NNewPositionControlElement* New(void);
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
bool CreateNeurons(void);
bool CreateExternalControlElements(void);
bool LinkNeurons(vector <NNet*> start, vector <NNet*> finish);
bool UnlinkNeurons(vector <NNet*> start, vector <NNet*> finish);
bool LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link);
bool LinkNegative(vector <NNet*> start, vector <NNet*> finish);
public:
vector<NNet*> GetInputNeurons(void);
vector<NNet*> GetControlNeurons(void);
};

}

#endif



