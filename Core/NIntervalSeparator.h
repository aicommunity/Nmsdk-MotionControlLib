// ===========================================================
// Version:        1.0.0
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2009.
E-mail:		alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NINTERVAL_SEPARATOR_H
#define NINTERVAL_SEPARATOR_H

#include "../../../Rdk/Deploy/Include/rdk.h"

namespace NMSDK {

using namespace RDK;

class RDK_LIB_TYPE NIntervalSeparator: public UNet
{
public: // ������������� ��������
/// ������ ������� ����������� ���������
UCLProperty<vector<double> ,NIntervalSeparator> MinRange;

/// ������� ������� ����������� ���������
UCLProperty<vector<double> ,NIntervalSeparator> MaxRange;

/// ����� ����������.
/// 0 - ���� �������� �������� � �������� ��������, �� ��� � ������������, ����� 0
/// 1 - ���� �������� �������� � �������� ��������, �� ������������ �������� �����
/// ������ ������� ���������, ����� 0
/// 2 - ���� �������� ��������� ������ �������, �� ��� � ������������, ����� 0
/// 3 - ���� �������� ���� ������� �������, �� ��� � ������������, ����� 0
/// 4 - ���� �������� ��������� ������ �������, �� ������������ �������� �����
/// 5 - ���� �������� ��������� ������ �������, �� ������������ �������� �����
/// ������ ������� ���������;���� �������� ��������� ������� �������, �� ������������
/// ������� ������� ����� ������ ������� (��� �������� �� ������� ������������ �������� �������)
/// 6 -  ���� �������� ��������� ������ �������, �� ������������ �������� �����
/// ������ ������� ���������;���� �������� ��������� ������� �������, �� ������������
/// ������� ������� ����� ������ �������
UCLProperty<vector<int>,NIntervalSeparator> Mode;

/// ��������� ��������� ��������
UCLProperty<vector<double> ,NIntervalSeparator> Gain;

/// ������� ������
UPropertyInputData<MDMatrix<double>,NIntervalSeparator, ptInput | ptPubState> Input;

/// ����������� ������
UPropertyOutputData<MDMatrix<double>,NIntervalSeparator, ptOutput | ptPubState> Output;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NIntervalSeparator(void);
virtual ~NIntervalSeparator(void);
// --------------------------

protected:
// --------------------------
// ������ ���������� �������������� ����������
// --------------------------
/// ������ ������� ����������� ���������
bool SetMinRange(const double &value);

/// ������� ������� ����������� ���������
bool SetMaxRange(const double &value);

/// ����� ����������
bool SetMode(const int &value);

/// ��������� ��������� ��������
bool SetGain(const double &value);
// --------------------------

public:
// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
/// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NIntervalSeparator* New(void);
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
protected:
/// �������������� �������� �� ��������� � ����� �������� �����
virtual bool ADefault(void);

/// ������������ ������ ���������� ��������� �������
/// ����� ��������� ����������
/// ������������� �������� ����� Reset() � ���������� Ready � true
/// � ������ �������� ������
virtual bool ABuild(void);

/// ����� �������� �����.
virtual bool AReset(void);

/// ��������� ������ ����� �������
virtual bool ACalculate(void);
// --------------------------
};

}
#endif
