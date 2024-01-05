/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   December, 2023
/// @brief  The component for signal zoning

#ifndef NSignalEstimationH
#define NSignalEstimationH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NSignalEstimation: public UNet
{
public: // ������������� ��������

 /// ��� ������ ��������
 ULProperty<std::string, NSignalEstimation, ptPubParameter> NeuronClassName;

 /// ��� ������ ����������� ���������
 ULProperty<std::string, NSignalEstimation, ptPubParameter> GeneratorClassName;


 /// ������������ ��������� (�)
 ULProperty<double, NSignalEstimation, ptPubParameter> PulseLength;

 /// ��������� ���������
 ULProperty<double, NSignalEstimation, ptPubParameter> Amplitude;

 /// ������� ��������� (��) ���� �����������
 ULProperty<double, NSignalEstimation, ptPubParameter> Frequency;

 /// ������������ ����� �������������� ���� ��������
 ULProperty<double, NSignalEstimation, ptPubParameter> LTZThreshold;


 /// �������� �������, ������� ����� ������������
 ULProperty<double, NSignalEstimation, ptPubParameter> DelaySignal;

 /// �������� ����������������� ��������
 ULProperty<double, NSignalEstimation, ptPubParameter> DelaySinchro;

 /// ���������� ��������, �� ������� ����� ������������ ������
 ULProperty<int, NSignalEstimation, ptPubParameter> NumZones;

 /// ������� ����������� ��� ������ �� �������� �����������
 ULProperty<MDMatrix<double>, NSignalEstimation, ptPubParameter> UpperLimitsOfZones;

 /// ������� ������������� �������� ����������� ������� �� �������� ���������
 ULProperty<bool, NSignalEstimation, ptPubParameter> UseTransitSignal;

 /// ������� ������������� �������� ������� �� ���������������� ��������� �� �������� ���������
 ULProperty<bool, NSignalEstimation, ptPubParameter> UseTransitSinchro;


public: // ����� � ������

 /// ������� ���������������� ������ �� ���������
 UPropertyInputData<MDMatrix<double>, NSignalEstimation, ptInput | ptPubState> Sinchro;

 /// ������� ������ �� ���������, ������� ����� ������������
 UPropertyInputData<MDMatrix<double>, NSignalEstimation, ptInput | ptPubState> Input;


protected: // ������
 /// ��������� ����������� �������
 UEPtr <NPulseGeneratorTransit> SignalGen;

 /// ��������� ����������������� �������
 UEPtr <NPulseGeneratorTransit> SinchroGen;

 ///���������� �������
 std::vector<UEPtr <NPulseNeuron>> ZoneNeurons;

 /// ������ �������� ���������� ���, ����������� ��� �������������� ����������
 int OldNumZones;

protected: // ��������� ����������


public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NSignalEstimation(void);
virtual ~NSignalEstimation(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NSignalEstimation* New(void);
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

/// ��������� ����� ������ ��������
bool SetNeuronClassName(const std::string &value);

/// ��������� ����� ������ ����������� ���������
bool SetGeneratorClassName(const std::string &value);

/// ��������� ������������ ���������
bool SetPulseLength(const double &value);

/// ��������� ��������� ���������
bool SetAmplitude(const double &value);

/// ��������� ������� ��������� (��) ���� �����������
bool SetFrequency(const double &value);

/// ��������� ������������� ������ �������������� ���� ��������
bool SetLTZThreshold(const double &value);

/// ��������� �������� �������, ������� ����� ������������
bool SetDelaySignal(const double &value);

/// ��������� �������� ����������������� ��������
bool SetDelaySinchro(const double &value);

/// ��������� ���������� ��������, �� ������� ����� ������������ ������
bool SetNumZones(const int &value);

/// ��������� ������� ����������� ��� ������ �� �������� �����������
bool SetUpperLimitsOfZones(const MDMatrix<double> &value);

/// ��������� ������������� �������� ����������� ������� �� �������� ���������
bool SetUseTransitSignal(const bool &value);

/// ��������� ������������� �������� ������� �� ���������������� ��������� �� �������� ���������
bool SetUseTransitSinchro(const bool &value);

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
