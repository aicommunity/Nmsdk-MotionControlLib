/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   November, 2023
/// @brief  The component for a detection of an objects in restricted area of image

#ifndef NObjInAreaH
#define NObjInAreaH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-MotionControlLib/Core/NSuppressionUnit.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGeneratorMulti.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NObjInArea: public UNet
{
public: // ������������� ��������

 /// ��� ������ �������
 ULProperty<std::string, NObjInArea, ptPubParameter> NeuronClassName;

 /// ��� ������ ���������� ������������������ ���������
 ULProperty<std::string, NObjInArea, ptPubParameter> MultiGeneratorClassName;

 /// ������������ ��������� (�)
 ULProperty<double, NObjInArea, ptPubParameter> PulseLength;

 /// ��������� ���������
 ULProperty<double, NObjInArea, ptPubParameter> Amplitude;

 /// ������� ��������� (��) ���� �����������
 ULProperty<double, NObjInArea, ptPubParameter> Frequency;

 /// ������� ��������� (��) ������ ���������� ������� � ������������� ����������
 ULProperty<double, NObjInArea, ptPubParameter> HighFreq;


 /// ������ ������� t=t0, � �������� ���������� ���������� ������� ����� � ����� �������
 ULProperty<double, NObjInArea, ptPubParameter> Delay11;

 /// ������ ������� t=t1, �� �������� ���������� ���������� ������� ����� � ����� �������
 ULProperty<double, NObjInArea, ptPubParameter> Delay12;

 /// ������ ������� t=t2, � �������� ���������� ���������� ������� ����� � ������ �������
 ULProperty<double, NObjInArea, ptPubParameter> Delay21;

 /// ������ ������� t=T, �� �������� ���������� ���������� ������� ����� � ������ �������
 ULProperty<double, NObjInArea, ptPubParameter> Delay22;


 /// ����������� ��������� ������� � ���������� ������
 ULProperty<int, NObjInArea, ptPubParameter> PulseCount;

 /// ���������� �������� ��� �������������
 ULProperty<int, NObjInArea, ptPubParameter> NumObj;

 /// ��������, �������� ��������� ������ �������
 ULProperty<MDMatrix<double>, NObjInArea, ptPubParameter> DelaysClsSpikeFr;


public: // ����� � ������

/// ����� ������
UPropertyOutputData<MDMatrix<double>, NObjInArea, ptOutput | ptPubState> Output;


protected: // ������
 /// ���������� ��������� ������� �������
 std::vector<UEPtr <NPulseGeneratorMulti>> ClsSpikeFr;

 /// ������ �������� ���������� ������� ��������, ����������� ��� �������������� ����������
 int OldNumObj;

 /// ���� ���������� 1
 UEPtr <NSuppressionUnit> SuppressUnit1;

 /// ���� ���������� 2
 UEPtr <NSuppressionUnit> SuppressUnit2;

 /// ������ ���
 UEPtr <NPulseNeuron> ORNeuron;

 /// ������ �
 UEPtr <NPulseNeuron> ANDNeuron;

 /// �������� ������
 UEPtr <NPulseNeuron> DecidingNeuron;

 /// ������������ ���������
 UEPtr <NPulseGeneratorTransit> ExcitatoryGen;

 // ���� ��� ��������� ������� ������ ExcitatoryGen
 bool Relinked;


protected: // ��������� ����������


public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NObjInArea(void);
virtual ~NObjInArea(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NObjInArea* New(void);
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

/// ��������� ������� ��������� ����������� � ������������� �����������
bool SetHighFreq(const double &value);

/// ��������� ������� ������� t=t0, � �������� ���������� ���������� ������� ����� � ����� �������
bool SetDelay11(const double &value);

/// ��������� ������� ������� t=t1, �� �������� ���������� ���������� ������� ����� � ����� �������
bool SetDelay12(const double &value);

/// ��������� ������� ������� t=t2, � �������� ���������� ���������� ������� ����� � ������ �������
bool SetDelay21(const double &value);

/// ��������� ������� ������� t=T, �� �������� ���������� ���������� ������� ����� � ������ �������
bool SetDelay22(const double &value);

/// ��������� ����������� ��������� �������
bool SetPulseCount(const int &value);

/// ��������� ���������� �������� ��� �������������
bool SetNumObj(const int &value);

/// ��������� ��������, �������� ��������� ������ �������
bool SetDelaysClsSpikeFr(const MDMatrix<double> &value);

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
