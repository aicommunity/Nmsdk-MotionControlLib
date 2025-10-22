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

#include "NEngineMotionControl.h"
#include "NMotionElement.h"
#include <memory>
#include "ModernSmartPointers.h"
#include <memory>
#include <mutex>
#include <chrono>
#include <thread>

namespace NMSDK {

//class NEngineMotionControl;
class RDK_LIB_TYPE NPositionControlElement: public UNet
{
public: // ��������
RDK::ULProperty<MDMatrix<double>, NPositionControlElement, ptPubState> CurrentPosition;
RDK::ULProperty<MDMatrix<double>, NPositionControlElement> TargetPosition;
RDK::ULProperty<string, NPositionControlElement> InputNeuronType;
RDK::ULProperty<string, NPositionControlElement> ControlNeuronType;
RDK::ULProperty<bool, NPositionControlElement> ExternalControl;
RDK::ULProperty<bool, NPositionControlElement, ptPubState> RememberState;
RDK::ULProperty<MDMatrix<double>, NPositionControlElement, ptPubState> Delta;

public: // ���������� ���������
vector<NNet*> InputNeurons;
vector<NNet*> ControlNeurons;
vector<NNet*> PreControlNeurons;

vector<NNet*> PostInputNeurons;

vector<UNet*> Generators;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NPositionControlElement(void);
virtual ~NPositionControlElement(void);

// Modern C++20 thread-safe position control
// Thread-safe position operations
void SetPositionSafe(const MDMatrix<double>& position);
MDMatrix<double> GetPositionSafe(void) const;
void SetTargetPositionSafe(const MDMatrix<double>& target);

// Modern move semantics
NPositionControlElement(NPositionControlElement&& other) noexcept;
NPositionControlElement& operator=(NPositionControlElement&& other) noexcept;

// Modern smart pointer factory
static std::shared_ptr<NPositionControlElement> Create(void);

// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
bool SetInputNeuronType(const string &value);
bool SetControlNeuronType(const string &value);
bool SetExternalControl(const bool &value);
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
template <typename T>
void SetPositions(vector<T*>& values, int x_base, int x_shift, int y_base, int y_shift);
public:
virtual bool CreateNeurons(void);
virtual bool CreateExternalControlElements(void);
virtual bool LinkNeurons(vector <NNet*> start, vector <NNet*> finish);
virtual bool UnlinkNeurons(vector <NNet*> start, vector <NNet*> finish);
virtual bool LinkGenerators(const bool &value);
virtual bool LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link, bool is_sim);
virtual bool LinkNegative(vector <NNet*> start, vector <NNet*> finish);
virtual bool PositionNeurons();
vector<NNet*> GetInputNeurons(void);
vector<NNet*> GetControlNeurons(void);
};

}

#endif



