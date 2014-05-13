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

protected: // ���������� ���������

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
};


}

#endif



