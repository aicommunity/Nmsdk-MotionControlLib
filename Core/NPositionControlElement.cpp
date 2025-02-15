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
 InputNeuronType = "NNewSPNeuron";
 ControlNeuronType = "NNewSPNeuron";
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
template <typename T>
void NPositionControlElement::SetPositions(vector<T*>& values, int x_base, int x_shift, int y_base, int y_shift)
{
 for(int i=0;i<int(values.size());++i)
 {
  values[i]->SetCoord(MVector<double,3>(x_base + i*x_shift,y_base + i*y_shift,0));
 }
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
	  for(size_t k=0;k<neuron->GetNumMembranes();k++)
	  {
	   NPulseMembrane* membr = neuron->GetMembrane(k);
	   if((neuron->GetNumOfConnectedSynToPosCh(membr)==0)&&(membr->GetName()!="LTMembrane"))
	   {
		 branch=neuron->GetMembrane(k);
		 hasEmptyMembrane=true;
		 break;
	   }
	  }

      if(!hasEmptyMembrane)
      {
       //branch=neuron->BranchDendrite("Soma1",false);
       neuron->NumSomaMembraneParts = 1;
       neuron->Build();
      }

      NameT startName;
      //NameT finishName = finish[j]->GetName()+"."+branch->GetName()+".ExcChannel";
      NameT finishName;

      int syns_max = branch->NumExcitatorySynapses;

      for(size_t i=0;i<start.size();i++)//���������
      {
          startName = start[i]->GetName()+".LTZone";

          for (int m = 0; m<syns_max; m++)
          {
              UEPtr<NPulseSynapse> syn = finish[j]->GetComponentL<NPulseSynapse>("Soma1.ExcSynapse"+sntoa(m+1),true);
              if(!syn)
                return true;

              finishName = finish[j]->GetName()+"."+branch->GetName()+".ExcSynapse"+sntoa(m+1);

              if(CheckLink(startName,finishName))
                break;//������� � ���������� �������� � start

              if (syn->Input.IsConnected())
              {
                  branch->NumExcitatorySynapses++;
                  branch->Build();
                  syns_max = branch->NumExcitatorySynapses;
                  continue; //������� � ���������� �������
              }

              CreateLink(startName, "Output", finishName,"Input");
              break;
          }
      }

      //	   ExternalControl=false;
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
	NameT finishName = finish[j]->GetName()+"."+neuron->Membranes[k]->GetName()+".ExcChannel";
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

bool NPositionControlElement::PositionNeurons()
{
   int x_base=4; //5
   int x_shift=0;
   int y_base=2; //6
   int y_shift = 2;
   SetPositions<NNet>(InputNeurons, x_base, x_shift, y_base, y_shift);
   x_base+=7; //10
   SetPositions<NNet>(PostInputNeurons, x_base, x_shift, y_base, y_shift);
   x_base+=7; //10
   SetPositions<NNet>(PreControlNeurons, x_base, x_shift, y_base, y_shift);
   x_base+=7; //10
   SetPositions<NNet>(ControlNeurons, x_base, x_shift, y_base, y_shift);
   x_base+=7; //10
   SetPositions<UNet>(Generators, x_base, x_shift, y_base, y_shift);
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



