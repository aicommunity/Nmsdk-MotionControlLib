/* ***********************************************************

E-mail:        
Url:           

This file is part of the project: 

File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NPositionControlElement_CPP
#define NPositionControlElement_CPP

#include "NPositionControlElement.h"

namespace NMSDK {

// ������
// --------------------------
// ������������ � �����������
// --------------------------
NPositionControlElement::NPositionControlElement(void)
:   MotionControl("MotionControl",this,0)
{
}

NPositionControlElement::~NPositionControlElement(void)
{
}
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
NPositionControlElement* NPositionControlElement::New(void)
{
 return new NPositionControlElement;
}
// --------------------------


// --------------------------
// ������� ������ ���������� ������ 
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool NPositionControlElement::ADefault(void)
{           
 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NPositionControlElement::ABuild(void)
{
 return true;
}

// ����� �������� ����� ��� ������ ��������
bool NPositionControlElement::AReset(void)
{
 return true;
}

// ��������� ������ ����� �������
bool NPositionControlElement::ACalculate(void)
{
 MotionControl->NumMotionElements;
// vector<NNet*> Motions = MotionControl->GetMotion();
// NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[0]);
 return true;
}
// --------------------------

}
#endif



