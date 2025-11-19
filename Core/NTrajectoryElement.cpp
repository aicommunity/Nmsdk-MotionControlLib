/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2010.
E-mail:			alexab@ailab.ru
url:			http://ailab.ru
Version:		1.0.0

This file - part of the project: NMSDK

File License:		BSD License
Project License:	GNU GPLv3 License
See file license.txt for more information
*********************************************************** */
#ifndef NMotionElementCPP
#define NMotionElementCPP

#include "NTrajectoryElement.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"
//---------------------------------------------------------------------------
namespace NMSDK {

// --------------------------
// ������������ � �����������
// --------------------------
NTrajectoryElement::NTrajectoryElement(void)
: NeuronClassName("NeuronClassName",this, &NTrajectoryElement::SetNeuronClassName),
  Layer("Layer", this, &NTrajectoryElement::SetLayer),
  Output("Output",this)
  //Input_u_top("Input_u_top",this),
  //Input_u_tcn("Input_u_tcn",this),
  //Input_y_pcn2("Input_y_pcn2",this),
{
}

NTrajectoryElement::~NTrajectoryElement(void)
{

}
// --------------------------

// --------------------------
// ������ ���������� �����������
// --------------------------
bool NTrajectoryElement::SetNeuronClassName(const std::string &value)
{
 Ready=false;
 return true;
}

bool NTrajectoryElement::SetLayer(const int &value)
{
 Ready=false;
 return true;
}

// --------------------------



// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
NTrajectoryElement* NTrajectoryElement::New(void)
{
 return new NTrajectoryElement;
}
// --------------------------

// --------------------------
// Proctected computation methods
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool NTrajectoryElement::ADefault(void)
{
 NeuronClassName = "NSPNeuronGen"; //NNewSPNeuron
 SomaSize = 1;
 DendSizes1.resize(SomaSize);
 DendSizes1[0] = 5;
 DendSizes2.resize(SomaSize);
 DendSizes2[0] = 3;

 //CurrentForward = -1;
 //CurrentBackward = -1;

 return true;
}


// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NTrajectoryElement::ABuild(void)
{
 //���������� ��� ���������� ��������
 int y_base = 2;
 int y_shift = 2;

 //������� �������
 Neurons.resize(2);
 for (int i = 0; i<2; i++)
 {
  Neurons[i] = AddMissingComponent<NPulseNeuron>(std::string("Neuron")+sntoa(i+1), NeuronClassName);
  if(!Neurons[i])
   return true;
  Neurons[i]->SetCoord(MVector<double,3>(4.0, y_base+i*y_shift, 0));
  Neurons[i]->NumSomaMembraneParts = SomaSize;
  Neurons[i]->Reset();
 }

   Neurons[0]->NumDendriteMembranePartsVec = DendSizes1;
   Neurons[0]->Reset();  
   Neurons[1]->NumDendriteMembranePartsVec = DendSizes2;
   Neurons[1]->Reset();


 //������ �����
 bool res(true);

 //����� ������������ (��������� ��� ����������� NNewSPNeuron)
 //�� N1 � N2
 std::weak_ptr<RDK::UContainer> ltzone1_weak = Neurons[0]->GetComponentL("LTZone", true);
 std::shared_ptr<NPulseLTZoneCommon> ltzone1;
 if(!ltzone1_weak.expired())
  ltzone1 = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzone1_weak.lock());
 else
  ltzone1 = nullptr;
 if(!ltzone1)
     return true;

 std::weak_ptr<RDK::UContainer> n2_s1_d1_weak = Neurons[1]->GetComponentL("Dendrite1_1.ExcSynapse1",true);
 std::shared_ptr<NPulseSynapse> n2_s1_d1;
 if(!n2_s1_d1_weak.expired())
  n2_s1_d1 = std::dynamic_pointer_cast<NPulseSynapse>(n2_s1_d1_weak.lock());
 else
  n2_s1_d1 = nullptr;
 if(!n2_s1_d1)
  return true;
 res&=CreateLink(ltzone1->GetLongName(GetThisAsSharedContainer()),"Output",n2_s1_d1->GetLongName(GetThisAsSharedContainer()),"Input");
 if(!res)
  return true;

 std::weak_ptr<RDK::UContainer> n2_s1_d3_weak = Neurons[1]->GetComponentL("Dendrite1_3.ExcSynapse1",true);
 std::shared_ptr<NPulseSynapse> n2_s1_d3;
 if(!n2_s1_d3_weak.expired())
  n2_s1_d3 = std::dynamic_pointer_cast<NPulseSynapse>(n2_s1_d3_weak.lock());
 else
  n2_s1_d3 = nullptr;
 if(!n2_s1_d3)
  return true;
 res&=CreateLink(ltzone1->GetLongName(GetThisAsSharedContainer()),"Output",n2_s1_d3->GetLongName(GetThisAsSharedContainer()),"Input");
 if(!res)
  return true;

 //�� N2 � N1
 std::weak_ptr<RDK::UContainer> ltzone2_weak = Neurons[1]->GetComponentL("LTZone", true);
 std::shared_ptr<NLTZone> ltzone2;
 if(!ltzone2_weak.expired())
  ltzone2 = std::dynamic_pointer_cast<NLTZone>(ltzone2_weak.lock());
 else
  ltzone2 = nullptr;
 if(!ltzone2)
     return true;

 std::weak_ptr<RDK::UContainer> n1_s1_d1_weak = Neurons[0]->GetComponentL("Dendrite1_1.ExcSynapse1",true);
 std::shared_ptr<NPulseSynapse> n1_s1_d1;
 if(!n1_s1_d1_weak.expired())
  n1_s1_d1 = std::dynamic_pointer_cast<NPulseSynapse>(n1_s1_d1_weak.lock());
 else
  n1_s1_d1 = nullptr;
 if(!n1_s1_d1)
  return true;
 res&=CreateLink(ltzone2->GetLongName(GetThisAsSharedContainer()),"Output",n1_s1_d1->GetLongName(GetThisAsSharedContainer()),"Input");
 if(!res)
  return true;

 std::weak_ptr<RDK::UContainer> n1_s1_d3_weak = Neurons[0]->GetComponentL("Dendrite1_3.ExcSynapse1",true);
 std::shared_ptr<NPulseSynapse> n1_s1_d3;
 if(!n1_s1_d3_weak.expired())
  n1_s1_d3 = std::dynamic_pointer_cast<NPulseSynapse>(n1_s1_d3_weak.lock());
 else
  n1_s1_d3 = nullptr;
 if(!n1_s1_d3)
  return true;
 res&=CreateLink(ltzone2->GetLongName(GetThisAsSharedContainer()),"Output",n1_s1_d3->GetLongName(GetThisAsSharedContainer()),"Input");
 if(!res)
  return true;

 return true;
}


// Reset computation
bool NTrajectoryElement::AReset(void)
{
 return true;
}


// Execute math. computations of current object on current step
bool NTrajectoryElement::ACalculate(void)
{
 //�������� ������� � ������ �����
  Output = Neurons[0]->Output;

 return true;
}




// --------------------------


}
//---------------------------------------------------------------------------
#endif
