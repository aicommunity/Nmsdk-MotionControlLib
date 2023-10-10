// ===========================================================
// Version:        3.0.2
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2002.
E-mail:        alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NMANIPULATOR_INPUT_EMULATOR_H
#define NMANIPULATOR_INPUT_EMULATOR_H

#include "NManipulatorInput.h"


namespace NMSDK {

class RDK_LIB_TYPE NManipulatorInputEmulator: public NManipulatorInput
{
public: // ������������� ��������

protected: // �������� ��������


public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NManipulatorInputEmulator(void);
virtual ~NManipulatorInputEmulator(void);
// --------------------------

public:
// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NManipulatorInputEmulator* New(void);
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
protected:
// �������������� �������� �� ��������� � ����� �������� �����
virtual bool ADefault(void);


// ����� �������� �����.
virtual bool AReset(void);

// ��������� ������ ����� �������
virtual bool ACalculate(void);
// --------------------------
};

}
#endif

