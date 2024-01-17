/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   November, 2023
/// @brief  The component for sequences comparison

#ifndef NSeqComparisonH
#define NSeqComparisonH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGeneratorMulti.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NSeqComparison: public UNet
{
public: // ������������� ��������

 /// ��� ������ �������
 ULProperty<std::string, NSeqComparison, ptPubParameter> NeuronClassName;

 /// ��� ������ ���������� ������������������ ���������
 ULProperty<std::string, NSeqComparison, ptPubParameter> MultiGeneratorClassName;


 /// ������������ ��������� (�)
 ULProperty<double, NSeqComparison, ptPubParameter> PulseLength;

 /// ��������� ���������
 ULProperty<double, NSeqComparison, ptPubParameter> Amplitude;

 /// ������� ��������� (��) ���� �����������
 ULProperty<double, NSeqComparison, ptPubParameter> Frequency;

 /// ������������ ����� �������������� ���� �������
 ULProperty<double, NSeqComparison, ptPubParameter> LTZThreshold;


 /// ����������� ��������� ������� � ���������� ������
 ULProperty<int, NSeqComparison, ptPubParameter> PulseCount;

 /// ���������� �������� ��� ���������
 ULProperty<int, NSeqComparison, ptPubParameter> NumClasses;

 /// ��������, �������� ����������� ��������� ������ �������
 ULProperty<MDMatrix<double>, NSeqComparison, ptPubParameter> DelaysKFCls;

 /// ��������, �������� �������� ��������� ������ �������
 ULProperty<MDMatrix<double>, NSeqComparison, ptPubParameter> DelaysFrCls;


public: // ����� � ������

/// ����� ������
UPropertyOutputData<MDMatrix<double>, NSeqComparison, ptOutput | ptPubState> Output;


protected: // ������
 /// ���������� ����������� ��������� ������� �������
 std::vector<UEPtr <NPulseGeneratorMulti>> KFClsSpikes;

 /// ���������� �������� ��������� ������� �������
 std::vector<UEPtr <NPulseGeneratorMulti>> FrClsSpikes;

 /// ������������ ������
 UEPtr <NPulseNeuron> CompNeuron;

 /// ������ �������� ���������� ������� ��������, ����������� ��� �������������� ����������
 int OldNumClasses;

protected: // ��������� ����������


public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NSeqComparison(void);
virtual ~NSeqComparison(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NSeqComparison* New(void);
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

/// ��������� ����� ������ �������
bool SetNeuronClassName(const std::string &value);

/// ��������� ����� ������ ���������� ������������������ ���������
bool SetMultiGeneratorClassName(const std::string &value);

/// ��������� ������������ ���������
bool SetPulseLength(const double &value);

/// ��������� ��������� ���������
bool SetAmplitude(const double &value);

/// ��������� ������� ��������� (��) ���� �����������
bool SetFrequency(const double &value);

/// ��������� ������������� ������ �������������� ���� �������
bool SetLTZThreshold(const double &value);

/// ��������� ����������� ��������� �������
bool SetPulseCount(const int &value);

/// ��������� ���������� �������� ��� ���������
bool SetNumClasses(const int &value);

/// ��������� ��������, �������� ����������� ��������� ������ �������
bool SetDelaysKFCls(const MDMatrix<double> &value);

/// ��������� ��������, �������� �������� ��������� ������ �������
bool SetDelaysFrCls(const MDMatrix<double> &value);

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
