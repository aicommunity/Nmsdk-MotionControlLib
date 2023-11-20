/* *********************************************************** 
@Copyright Alexander V. Bakhshiev, 2012.
E-mail:        alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: RDK

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NSuppressionUnitH
#define NSuppressionUnitH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NSuppressionUnit: public UNet
{
public: // ������������� ��������

 /// ��� ������ ���������� ���������
 ULProperty<std::string, NSuppressionUnit, ptPubParameter> PulseGeneratorClassName;

 /// ��� ������ �������
 ULProperty<std::string, NSuppressionUnit, ptPubParameter> NeuronClassName;

 /// ��� ������ ��������
 ULProperty<std::string, NSuppressionUnit, ptPubParameter> SynapseClassName;


 /// ������������ ��������� (�)
 ULProperty<double, NSuppressionUnit, ptPubParameter> PulseLength;

 /// ��������� ���������
 ULProperty<double, NSuppressionUnit, ptPubParameter> Amplitude;

 /// ������� ��������� (��) ����� ���������� �������
 ULProperty<double, NSuppressionUnit, ptPubParameter> SuppressionFreq;

 /// ������������ ����� �������������� ���� �������
 ULProperty<double, NSuppressionUnit, ptPubParameter> LTZThreshold;


 /// ������ ������� �1, � �������� ���������� ���������� �������
 ULProperty<double, NSuppressionUnit, ptPubParameter> Delay1;

 /// ������ ������� �2, �� �������� ���������� ���������� �������
 ULProperty<double, NSuppressionUnit, ptPubParameter> Delay2;


 /// ������� ��������� (��) ��������� ������������ �������
 ULProperty<double, NSuppressionUnit, ptPubParameter> SourceFreq;

 /// �������� ��������� ������������ �������
 ULProperty<double, NSuppressionUnit, ptPubParameter> SourceDelay;

 /// ��� �������� ����� ������������ ������� ������ �� ���������,
 /// ����� ���������� �������� ���������� ���������� ������ � ������������ � �����������
 ULProperty<bool, NSuppressionUnit, ptPubParameter> TransitInput;

 /// ��� �������� ����� ��������� �� ���������� �������� ������� � ������
 /// ����������� ������ ���������� �������� ������ ����������� ������
 ULProperty<bool, NSuppressionUnit, ptPubParameter> OnlyInhibition;

 /// ��� �������� ����� ��������� ������� ���������� ������ �� ����� ��������
 ULProperty<bool, NSuppressionUnit, ptPubParameter> SingleUse;

public: // ����� � ������
/// ������� ������ �� ���������
UPropertyInputData<MDMatrix<double>, NSuppressionUnit, ptInput | ptPubState> Input;

/// ����� ������
UPropertyOutputData<MDMatrix<double>, NSuppressionUnit, ptOutput | ptPubState> Output;


protected: // ������
 /// ���������� ������� ���������
 UEPtr <NPulseGeneratorTransit> SourceGenerator;

 /// ���������� ��������
 std::vector<UEPtr<NPulseGeneratorTransit>> DelayGenerators;

 /// ������ ���
 UEPtr <NPulseNeuron> ORNeuron;

 /// ��������� ����������� ���������
 UEPtr <NPulseGeneratorTransit> ControlledGenerator;

 /// ������
 UEPtr <NPulseNeuron> Neuron;

protected: // ��������� ����������


public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NSuppressionUnit(void);
virtual ~NSuppressionUnit(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NSuppressionUnit* New(void);
// --------------------------

protected:
 // --------------------------
 // ������� ������ ���������� ������������
 // --------------------------

 /// ��������� ����������� ���������������� ��������
 /// ��� ���������� ��������� ���������� � ���� ������
 // ����� ����� ������ ������ ���� comp ��� ������� �������� � ������ ���������
 virtual bool AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer = 0);

 /// ��������� ��������������� ���������������� ��������
 /// ��� �������� ��������� ���������� �� ����� �������
 // ����� ����� ������ ������ ���� comp ���������� � ������ ���������
 virtual bool ADelComponent(UEPtr<UContainer> comp);

 // --------------------------

protected:
// --------------------------
// ������ ���������� �������������� ����������
// --------------------------

/// ��������� ����� ������ ����������� ���������
bool SetPulseGeneratorClassName(const std::string &value);

/// ��������� ����� ������ �������
bool SetNeuronClassName(const std::string &value);

/// ��������� ����� ������ ��������
bool SetSynapseClassName(const std::string &value);

/// ��������� ������� ��������� ������������ ����������
bool SetSuppressionFreq(const double &value);

/// ��������� ������������ ���������
bool SetPulseLength(const double &value);

/// ��������� ��������� ���������
bool SetAmplitude(const double &value);

/// ��������� ������������� ������ �������������� ���� �������
bool SetLTZThreshold(const double &value);

/// ��������� ������� ������� �1, � �������� ���������� ���������� �������
bool SetDelay1(const double &value);

/// ��������� ������� ������� �2, �� �������� ���������� ���������� �������
bool SetDelay2(const double &value);

/// ��������� ������� ��������� ������������ ������� (� ����������� ��������)
bool SetSourceFreq(const double &value);

/// ��������� �������� ��������� ������������ �������
bool SetSourceDelay(const double &value);

/// ��������� �������� ����� �������� ������� �������� ���������
bool SetTransitInput(const bool &value);

/// ��������� �������� ����� ������������� ������ ������������ �������
bool SetOnlyInhibition(const bool &value);

/// ��������� �������� ����� ������������� ���������� �������
bool SetSingleUse(const bool &value);

// --------------------------

// --------------------------
// Computation methods
// --------------------------
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
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
