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

 /// ������������ ��������� (�)
 ULProperty<double, NObjInArea, ptPubParameter> PulseLength;

 /// ��������� ���������
 ULProperty<double, NObjInArea, ptPubParameter> Amplitude;

 /// ������� ��������� (��) ���� �����������
 ULProperty<double, NObjInArea, ptPubParameter> Frequency;

 /// ������� ��������� (��) ������ ���������� ������� � ������������� ����������
 ULProperty<double, NObjInArea, ptPubParameter> HighFreq;


 /// ������ ������� t=t0, � �������� ���������� ���������� ������� ����� � ����� �������
 ULProperty<double, NObjInArea, ptPubParameter> Delay1_1;

 /// ������ ������� t=t1, �� �������� ���������� ���������� ������� ����� � ����� �������
 ULProperty<double, NObjInArea, ptPubParameter> Delay1_2;

 /// ������ ������� t=t2, � �������� ���������� ���������� ������� ����� � ������ �������
 ULProperty<double, NObjInArea, ptPubParameter> Delay2_1;

 /// ������ ������� t=T, �� �������� ���������� ���������� ������� ����� � ������ �������
 ULProperty<double, NObjInArea, ptPubParameter> Delay2_2;


 /// ����������� ��������� ������� � ���������� ������
 ULProperty<int, NObjInArea, ptPubParameter> PulseCount;

 /// ���������� �������� ��� �������������
 ULProperty<int, NObjInArea, ptPubParameter> NumObj;

 /// ��������, �������� ��������� ������ �������
 ULProperty<MDMatrix<double>, NObjInArea, ptPubParameter> Delays_ClsSpikeFr;


public: // ����� � ������
/// ������� ������ ������ 1
//std::vector<UPropertyInputData<MDMatrix<double>, NObjInArea, ptInput | ptPubState>> InputDelaysCls;

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

/// ��������� ������������ ���������
bool SetPulseLength(const double &value);

/// ��������� ��������� ���������
bool SetAmplitude(const double &value);

/// ��������� ������� ��������� (��) ���� �����������
bool SetFrequency(const double &value);

/// ��������� ������� ��������� ����������� � ������������� �����������
bool SetHighFreq(const double &value);

/// ��������� ������� ������� t=t0, � �������� ���������� ���������� ������� ����� � ����� �������
bool SetDelay1_1(const double &value);

/// ��������� ������� ������� t=t1, �� �������� ���������� ���������� ������� ����� � ����� �������
bool SetDelay1_2(const double &value);

/// ��������� ������� ������� t=t2, � �������� ���������� ���������� ������� ����� � ������ �������
bool SetDelay2_1(const double &value);

/// ��������� ������� ������� t=T, �� �������� ���������� ���������� ������� ����� � ������ �������
bool SetDelay2_2(const double &value);

/// ��������� ����������� ��������� �������
bool SetPulseCount(const int &value);

/// ��������� ���������� �������� ��� �������������
bool SetNumObj(const int &value);

/// ��������� ��������, �������� ��������� ������ �������
bool SetDelays_ClsSpikeFr(const MDMatrix<double> &value);

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
