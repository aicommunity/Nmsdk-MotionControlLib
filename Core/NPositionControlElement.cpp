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
	TargetPosition("TargetPosition",this),
	InputNeuronType("InputNeuronType",this, &NPositionControlElement::SetInputNeuronType),
	ControlNeuronType("ControlNeuronType",this, &NPositionControlElement::SetControlNeuronType)
{
}

NPositionControlElement::~NPositionControlElement(void)
{
}
// --------------------------


// ---------------------
// ������ ���������� �����������
// ---------------------
bool NPositionControlElement::SetInputNeuronType(const string &value)
{
 Ready=false;
 return true;
}
bool NPositionControlElement::SetControlNeuronType(const string &value)
{
 Ready=false;
 return true;
}
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
 InputNeuronType = "NNewSynSPNeuron";
 ControlNeuronType = "NNewSynSPNeuron";
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
 InputNeurons.clear();
 ControlNeurons.clear();

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
 if(InputNeurons.empty())
  CreateNeurons();

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
bool NPositionControlElement::CreateNeurons()
{
   UEPtr<UContainer> cont;
   UEPtr<UStorage> storage = GetStorage();
   bool res;
   vector<NNet*> Motions = MotionControl->GetMotion();
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
	NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	for(int j=0;j<melem->NumControlLoops;j++)
	{
	 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(InputNeuronType));
	 if(!cont)
	  return 0;
	 cont->SetName("InputNeuronL"+sntoa(i+1)+sntoa(j+1));
	 res=AddComponent(cont);
	 InputNeurons.push_back(static_pointer_cast<NNet>(cont));

	 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(InputNeuronType));
	 if(!cont)
	  return 0;
	 cont->SetName("InputNeuronR"+sntoa(i+1)+sntoa(j+1));
	 res=AddComponent(cont);
	 InputNeurons.push_back(static_pointer_cast<NNet>(cont));
	}
   }
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
	NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	for(int j=0;j<melem->NumControlLoops;j++)
	{
	 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	 if(!cont)
	  return 0;
	 cont->SetName("ControlNeuronL"+sntoa(i+1)+sntoa(j+1));
	 res=AddComponent(cont);
	 ControlNeurons.push_back(static_pointer_cast<NNet>(cont));

	 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	 if(!cont)
	  return 0;
	 cont->SetName("ControlNeuronR"+sntoa(i+1)+sntoa(j+1));
	 res=AddComponent(cont);
	 ControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	}
   }
   LinkNeurons(InputNeurons, ControlNeurons);
   return true;
}

bool NPositionControlElement::LinkNeurons(vector <NNet*> start, vector <NNet*> finish)
{
	vector <NNet*>::iterator I,J;
	for(int j=0;j<finish.size();j++)
	{
	  NPulseNeuron* neuron=dynamic_cast<NPulseNeuron*>(finish[j]);
	  UEPtr<NPulseMembrane> branch;
	  bool hasEmptyMembrane=false;
	  for(int k=0;k<neuron->Membranes.size();k++)
	  {
	   NPulseMembrane* membr = neuron->Membranes[k];
	   if((neuron->GetNumOfConnectedSynToPosCh(membr)==0)&&(membr->GetName()!="LTMembrane"))
	   {
		 branch=neuron->Membranes[k];
		 hasEmptyMembrane=true;
		 break;
	   }
	  }
	  if(!hasEmptyMembrane)
	   branch=neuron->BranchDendrite(neuron->GetComponentId("PNeuronMembrane"),false);
	  NameT finishName = finish[j]->GetName()+"."+branch->GetName()+".PosChannel";
	  for(int i=0;i<start.size();i++)
	  {
	   NameT startName = start[i]->GetName()+".LTZone";
	   if(CheckLink(startName,finishName))
		CreateLink(startName, 0, finishName);
	  }
	}

	return true;
}

}
#endif



