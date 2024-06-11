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
  Input_u_top("Input_u_top",this),
  Input_u_tcn("Input_u_tcn",this),
  Input_y_pcn2("Input_y_pcn2",this),
  Output("Output",this)
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
 neurons.resize(2);
 for (int i = 0; i<2; i++)
 {
  neurons[i] = AddMissingComponent<NPulseNeuron>(std::string("Neuron")+sntoa(i+1), "NSPNeuronGen");
  if(!neurons[i])
   return false;
  neurons[i]->SetCoord(MVector<double,3>(4.0, y_base+i*y_shift, 0));
  neurons[i]->NumSomaMembraneParts = 4;
  neurons[i]->NumDendriteMembranePartsVec[0] = 3;
  neurons[i]->Reset();
 }

 //������ �����
 bool res(true);

 //����� 2�� ������� - �2 1�� �������
 UEPtr<NLTZone> ltzone2 = neurons[1]->GetComponentL<NLTZone>("LTZone", true);
 if(!ltzone2)
     return true;
 UEPtr<NPulseSynapse> M21_n1_d1_seg2 = neurons[0]->GetComponentL<NPulseSynapse>("Dendrite1_1.ExcSynapse1",true);
 res&=CreateLink(ltzone2->GetLongName(this),"Output",M21_n1_d1_seg2->GetLongName(this),"Input");
 if(!res)
  return true;

 //����� 1�� ������� - �2 2�� �������
 UEPtr<NLTZone> ltzone1 = neurons[0]->GetComponentL<NLTZone>("LTZone", true);
 if(!ltzone1)
     return true;
 UEPtr<NPulseSynapse> M22_n2_d1_seg2 = neurons[1]->GetComponentL<NPulseSynapse>("Dendrite1_1.ExcSynapse1",true);
 res&=CreateLink(ltzone1->GetLongName(this),"Output",M22_n2_d1_seg2->GetLongName(this),"Input");
 if(!res)
  return true;

 //������� ��� ����������� ������� ������ � ACalculate
 n1_d1_seg2_exc = neurons[0]->GetComponentL<NPulseSynapse>("Dendrite1_2.ExcSynapse1",true);
 n1_d1_seg3_exc = neurons[0]->GetComponentL<NPulseSynapse>("Dendrite1_3.ExcSynapse1",true);
 n1_s3_inh = neurons[0]->GetComponentL<NPulseSynapse>("Soma3.InhSynapse1",true);
 n1_s4_inh = neurons[0]->GetComponentL<NPulseSynapse>("Soma4.InhSynapse1",true);
 n1_d1_seg3_inh = neurons[0]->GetComponentL<NPulseSynapse>("Dendrite1_3.InhSynapse1",true);
 n1_d1_seg1_inh = neurons[0]->GetComponentL<NPulseSynapse>("Dendrite1_1.InhSynapse1",true);
 n2_d1_seg3_inh = neurons[1]->GetComponentL<NPulseSynapse>("Dendrite1_3.InhSynapse1",true);
 n2_d1_seg1_inh = neurons[1]->GetComponentL<NPulseSynapse>("Dendrite1_1.InhSynapse1",true);

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
  Output = neurons[0]->Output;

  //���� � u_top
  n1_d1_seg2_exc->Input = Input_u_top;

  //���� � ����������� �������� u_tcn
  n1_d1_seg3_exc->Input = Input_u_tcn;
  n1_s3_inh->Input = Input_u_tcn;
  n1_s4_inh->Input = Input_u_tcn;

  //���� � ����������� ������ ������� (������� PCN2, ����� MultiPositioncontrol) y_pcn2
  n1_d1_seg3_inh->Input = Input_y_pcn2;
  n1_d1_seg1_inh->Input = Input_y_pcn2;
  n2_d1_seg3_inh->Input = Input_y_pcn2;
  n2_d1_seg1_inh->Input = Input_y_pcn2;

 return true;
}




// --------------------------


}
//---------------------------------------------------------------------------
#endif
