/* ***********************************************************

E-mail:
Url:           

This file is part of the project: 

File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NMultiPositionControl_H
#define NMultiPositionControl_H

#include "NPositionControlElement.h"

namespace NMSDK {

class RDK_LIB_TYPE NMultiPositionControl: public NPositionControlElement
{
protected: // ��������
RDK::UPropertyInputC<NPositionControlElement,NMultiPositionControl> PositionControl;
RDK::ULProperty<int, NPositionControlElement> NumOfPositions;

public: // ���������� ���������
// ������� �������, ���������� �� ������ �������
// IN(0,0) IN(0,1), ..., IN(0,M) - �������, ������������ � ������� �������� ������� ������
// IN(1,0) IN(1,1), ..., IN(1,M) - �������, ������������ �� ������� �������� ������� ������
vector<vector<NNet*> > InputNeuronsByContours;
// �������� �������, ���������� �� ������ �������
// Ctrl(0,0) Ctrl(0,1), ..., Ctrl(0,M) - �������, ������������ � ������� �������� ������� ������
// Ctrl(1,0) Ctrl(1,1), ..., Ctrl(1,M) - �������, ������������ �� ������� �������� ������� ������
vector<vector<NNet*> > ControlNeuronsByContours;

//����� ������
//true - ����� ��������� � Build � ���������� ������, ��� ����������� NewPositionControl
//false - ����� ��������� � Calculate � ������ � ������� � NewPositionControl
RDK::ULProperty<bool, NMultiPositionControl> BuildSolo;

//����� ������
//������������ ��� ���������� ������ Multi (��� BuildSolo = true),
//� �������� ����� ������������ NNewPositionControls
//����� ���� ���������������� ��� ����� ������� ������� ����������
RDK::ULProperty<int, NMultiPositionControl> PCsNum;

//����� InputNeurons � ControlNeurons
//������������ ��� ���������� ������ Multi (��� BuildSolo = true)
RDK::ULProperty<int, NMultiPositionControl> InputsNum;



public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NMultiPositionControl(void);
virtual ~NMultiPositionControl(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
bool SetBuildSolo(const bool &value);
bool SetInputsNum(const int &value);
bool SetPCsNum(const int &value);
// ---------------------

// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NMultiPositionControl* New(void);
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
public:
bool CreateNeurons(void);
bool LinkGenerators(const bool &value);
bool LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link, bool is_sim);

bool CreateNeuronsSolo(void);
};


}

#endif



