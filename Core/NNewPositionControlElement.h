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

namespace NMSDK {

//class NEngineMotionControl;
class NNewPositionControlElement: public UNet
{
public: // ��������
RDK::UPropertyInput<NEngineMotionControl,NNewPositionControlElement> MotionControl;
RDK::ULProperty<MDMatrix<double>, NNewPositionControlElement, ptPubState> CurrentPosition;
RDK::ULProperty<MDMatrix<double>, NNewPositionControlElement> TargetPosition;
RDK::ULProperty<string, NNewPositionControlElement> InputNeuronType;
RDK::ULProperty<string, NNewPositionControlElement> ControlNeuronType;
RDK::ULProperty<bool, NNewPositionControlElement> ExternalControl;
RDK::ULProperty<bool, NNewPositionControlElement> RememberState;
RDK::ULProperty<MDMatrix<double>, NNewPositionControlElement, ptPubState> Delta;

protected: // ���������� ���������
vector<NNet*> InputNeurons;
vector<NNet*> ControlNeurons;
vector<NNet*> PreControlNeurons;
vector<NNet*> PostInputNeurons;
vector<UNet*> Generators;

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



