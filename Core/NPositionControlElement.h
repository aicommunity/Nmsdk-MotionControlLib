/* ***********************************************************

E-mail:        
Url:           

This file is part of the project: 

File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NPositionControlElement_H
#define NPositionControlElement_H

#include "..\..\RDK\Core\Engine\UNet.h"
#include "NEngineMotionControl.h"
#include "NMotionElement.h"

namespace NMSDK {

//class NEngineMotionControl;
class NPositionControlElement: public UNet
{
public: // ��������
RDK::UPropertyInput<NEngineMotionControl,NPositionControlElement> MotionControl;
RDK::ULProperty<MDMatrix<double>, NPositionControlElement, ptPubState> CurrentPosition;
RDK::ULProperty<MDMatrix<double>, NPositionControlElement> TargetPosition;
RDK::ULProperty<string, NPositionControlElement> InputNeuronType;
RDK::ULProperty<string, NPositionControlElement> ControlNeuronType;
RDK::ULProperty<bool, NPositionControlElement> ExternalControl;
RDK::ULProperty<bool, NPositionControlElement> RememberState;
RDK::ULProperty<MDMatrix<double>, NPositionControlElement, ptPubState> Delta;

protected: // ���������� ���������
vector<NNet*> InputNeurons;
vector<NNet*> ControlNeurons;
vector<NNet*> PreControlNeurons;
vector<UNet*> Generators;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NPositionControlElement(void);
virtual ~NPositionControlElement(void);
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
virtual NPositionControlElement* New(void);
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
public:
vector<NNet*> GetInputNeurons(void);
vector<NNet*> GetControlNeurons(void);
};

}

#endif



