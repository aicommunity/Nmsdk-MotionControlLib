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
:   MotionControl("MotionControl",this,0),
	CurrentPosition("CurrentPosition",this),
	TargetPosition("TargetPosition",this)
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
 CurrentPosition->Assign(2,1,0.0);
 TargetPosition->Assign(MotionControl->GetNumControlLoops(),2*MotionControl->NumMotionElements,0.0);

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
 CurrentPosition->Assign(1,1,0.0);
 vector<NNet*> Motions = MotionControl->GetMotion();
 CurrentPosition->Assign(MotionControl->GetNumControlLoops(),2*MotionControl->NumMotionElements,0.0);
 for(int i=0;i<MotionControl->NumMotionElements;i++)
 {
  NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
  if(!melem)
   continue;
  for(int j=0;j<melem->NumControlLoops;j++)
  {
   UEPtr<UADItem> ltzoneL=dynamic_pointer_cast<UADItem>(melem->GetComponentL("AfferentL"+sntoa(j+1)+".LTZone"));
   UEPtr<UADItem> ltzoneR=dynamic_pointer_cast<UADItem>(melem->GetComponentL("AfferentR"+sntoa(j+1)+".LTZone"));
   double temp=0;
   if(ltzoneL->GetNumOutputs()>2)
   {
	temp=ltzoneL->GetOutputData(2).Double[0];
	(*CurrentPosition)(j,2*i)=temp;
   }
   if(ltzoneR->GetNumOutputs()>2)
   {
	temp=ltzoneR->GetOutputData(2).Double[0];
	(*CurrentPosition)(j,2*i+1)= temp;
   }
  }
 }
 return true;
}
// --------------------------

}
#endif



