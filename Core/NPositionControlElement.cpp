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
:
	CurrentPosition("CurrentPosition",this),
	TargetPosition("TargetPosition",this),
	InputNeuronType("InputNeuronType",this, &NPositionControlElement::SetInputNeuronType),
	ControlNeuronType("ControlNeuronType",this, &NPositionControlElement::SetControlNeuronType),
	ExternalControl("ExternalControl", this, &NPositionControlElement::SetExternalControl),
	RememberState("RememberState", this),
	Delta("Delta",this)
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
bool NPositionControlElement::SetExternalControl(const bool &value)
{
 LinkGenerators(value);
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
 ExternalControl = false;
 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NPositionControlElement::ABuild(void)
{
 CurrentPosition->Assign(2,1,0.0);
 TargetPosition->Assign(2,1,0.0);
 InputNeurons.clear();
 ControlNeurons.clear();
 Generators.clear();
 Delta->Assign(2,1,0.0);

 return true;
}

// ����� �������� ����� ��� ������ ��������
bool NPositionControlElement::AReset(void)
{
 RememberState = false;
 return true;
}

// ��������� ������ ����� �������
bool NPositionControlElement::ACalculate(void)
{
 return true;
}
// --------------------------
bool NPositionControlElement::CreateNeurons()
{
   return true;
}

bool NPositionControlElement::CreateExternalControlElements(void)
{
 return true;
}

bool NPositionControlElement::LinkNeurons(vector <NNet*> start, vector <NNet*> finish)
{
	//vector <NNet*>::iterator I,J;
	for(size_t j=0;j<finish.size();j++)
	{
	  NPulseNeuron* neuron=dynamic_cast<NPulseNeuron*>(finish[j]);
	  UEPtr<NPulseMembrane> branch;
	  bool hasEmptyMembrane=false;
	  for(size_t k=0;k<neuron->Membranes.size();k++)
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
	  for(size_t i=0;i<start.size();i++)
	  {
	   NameT startName = start[i]->GetName()+".LTZone";
	   if(!CheckLink(startName,finishName))
		CreateLink(startName, 0, finishName);
//	   ExternalControl=false;
	  }
	}

	return true;
}

bool NPositionControlElement::UnlinkNeurons(vector <NNet*> start, vector <NNet*> finish)
{
 for(size_t i=0;i<start.size();i++)
 {
  NameT startName = start[i]->GetName()+".LTZone";
  NPulseNeuron* neuron=dynamic_cast<NPulseNeuron*>(finish[i]);
  for(size_t j=0;j<finish.size();j++)
  {
   for(size_t k=0;k<neuron->Membranes.size();k++)
   {
	NameT finishName = finish[j]->GetName()+"."+neuron->Membranes[k]->GetName()+".PosChannel";
	if(CheckLink(startName,finishName))
	   BreakLink(startName,finishName);
   }
  }
 }
 return true;
}
bool NPositionControlElement::LinkGenerators(const bool &value)
{
 return true;
}
bool NPositionControlElement::LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link, bool is_sim)
{
 return true;
}
bool NPositionControlElement::LinkNegative(vector <NNet*> start, vector <NNet*> finish)
{
 return true;
}

vector<NNet*> NPositionControlElement::GetInputNeurons(void)
{
 return InputNeurons;
}

vector<NNet*> NPositionControlElement::GetControlNeurons(void)
{
 return ControlNeurons;
}

}
#endif



