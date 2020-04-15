/* ***********************************************************

E-mail:
Url:

This file is part of the project:

File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NNewPositionControlElement_CPP
#define NNewPositionControlElement_CPP

#include "NNewPositionControlElement.h"

namespace NMSDK {

// ������
// --------------------------
// ������������ � �����������
// --------------------------
NNewPositionControlElement::NNewPositionControlElement(void)
:   MotionControl("MotionControl",this),
	SimControl("SimControl", this)//,
	//CurrentPosition("CurrentPosition",this),
	//TargetPosition("TargetPosition",this),
	//InputNeuronType("InputNeuronType",this, &NNewPositionControlElement::SetInputNeuronType),
	//ControlNeuronType("ControlNeuronType",this, &NNewPositionControlElement::SetControlNeuronType),
	//ExternalControl("ExternalControl", this),
	//RememberState("RememberState", this),
	//Delta("Delta",this)
{
}

NNewPositionControlElement::~NNewPositionControlElement(void)
{
}
// --------------------------


// ---------------------
// ������ ���������� �����������
// ---------------------
bool NNewPositionControlElement::SetInputNeuronType(const string &value)
{
 Ready=false;
 return true;
}
bool NNewPositionControlElement::SetControlNeuronType(const string &value)
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
NNewPositionControlElement* NNewPositionControlElement::New(void)
{
 return new NNewPositionControlElement;
}
// --------------------------


// --------------------------
// ������� ������ ���������� ������
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool NNewPositionControlElement::ADefault(void)
{
 NPositionControlElement::ADefault();
 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NNewPositionControlElement::ABuild(void)
{
 CurrentPosition->Assign(2,1,0.0);
 TargetPosition->Assign(MotionControl->GetNumControlLoops(),2*MotionControl->NumMotionElements,0.0);
 InputNeurons.clear();
 ControlNeurons.clear();
 Generators.clear();
 LeftGenerators.clear();
 RightGenerators.clear();
 Delta->Assign(2,1,0.0);

 return true;
}

// ����� �������� ����� ��� ������ ��������
bool NNewPositionControlElement::AReset(void)
{
 RememberState = false;
 return true;
}

// ��������� ������ ����� �������
bool NNewPositionControlElement::ACalculate(void)
{

	 if(MotionControl->GetNumControlLoops() != CurrentPosition->GetRows() ||
		2*MotionControl->NumMotionElements != CurrentPosition->GetCols())
	 {
	  Ready=false;
	  Reset();
	 }


  if(InputNeurons.empty()||ControlNeurons.empty()||PreControlNeurons.empty()||PostInputNeurons.empty())
   CreateNeurons();
  CurrentPosition->Assign(1,1,0.0);
  vector<NNet*> Motions = MotionControl->GetMotion();
  CurrentPosition->Assign(MotionControl->GetNumControlLoops(),2*MotionControl->NumMotionElements,0.0);
  Delta->Assign(MotionControl->GetNumControlLoops(),2*MotionControl->NumMotionElements,0.0);
  //CurrentPosition Calculation
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
  //Delta Calculation
  for(int i=0;i<MotionControl->NumMotionElements;i++)
  {
   NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
   if(!melem)
	continue;
   for(int j=0;j<melem->NumControlLoops;j++)
   {
	UEPtr<UADItem> controlLTZoneL=dynamic_pointer_cast<UADItem>(GetComponentL("ControlNeuronL"+sntoa(i+1)+sntoa(j+1)+".LTZone"));
	UEPtr<UADItem> controlLTZoneR=dynamic_pointer_cast<UADItem>(GetComponentL("ControlNeuronR"+sntoa(i+1)+sntoa(j+1)+".LTZone"));
	UEPtr<UADItem> preControlLTZoneL=dynamic_pointer_cast<UADItem>(GetComponentL("PreControlNeuronL"+sntoa(i+1)+sntoa(j+1)+".LTZone"));
	UEPtr<UADItem> preControlLTZoneR=dynamic_pointer_cast<UADItem>(GetComponentL("PreControlNeuronR"+sntoa(i+1)+sntoa(j+1)+".LTZone"));

	(*Delta)(j,2*i)= controlLTZoneL->GetOutputData(2).Double[0]-preControlLTZoneL->GetOutputData(2).Double[0];
	(*Delta)(j,2*i+1)= controlLTZoneR->GetOutputData(2).Double[0]-preControlLTZoneR->GetOutputData(2).Double[0];
   }
  }

  if(RememberState)
  {
   RememberState = false;
   vector<NNet*> activeInputs, postInputs, preControls, activeControls;
   for(size_t i=0;i<InputNeurons.size();i++)
   {
	UEPtr<UADItem> ltzone=dynamic_pointer_cast<UADItem>(InputNeurons[i]->GetComponentL("LTZone"));
	if(ltzone->GetOutputData(2).Double[0]>0)
	{
	 activeInputs.push_back(InputNeurons[i]);
	 preControls.push_back(PreControlNeurons[i]);
	}
   }
   for(size_t c=0;c<PreControlNeurons.size();c++)
   {
	UEPtr<UADItem> ltzone=dynamic_pointer_cast<UADItem>(ControlNeurons[c]->GetComponentL("LTZone"));
	if(ltzone->GetOutputData(2).Double[0]>0)
	{
	 activeControls.push_back(ControlNeurons[c]);
	 postInputs.push_back(PostInputNeurons[c]);
	}
   }
   LinkNegative(activeInputs, postInputs);
   LinkNeurons(activeInputs, postInputs);
   LinkNeurons(preControls, activeControls);
  }
  if(ExternalControl)
  {
   //UnlinkNeurons(InputNeurons, ControlNeurons);
   LinkGenerators(Generators, PreControlNeurons, true, SimControl);
  }
  else
  {
   LinkGenerators(Generators, PreControlNeurons, false, SimControl);
  }

 return true;
}
// --------------------------
bool NNewPositionControlElement::CreateNeurons()
{
   UEPtr<UContainer> cont;
   UEPtr<UStorage> storage = GetStorage();
   bool res(true);
   vector<NNet*> Motions = MotionControl->GetMotion();
   //Creating InputNeurons
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
	NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	LeftInputNeurons.resize(melem->NumControlLoops);
	RightInputNeurons.resize(melem->NumControlLoops);
	for(int j=0;j<melem->NumControlLoops;j++)
	{
	 UEPtr<UADItem> ltzoneL=dynamic_pointer_cast<UADItem>(melem->GetComponentL("AfferentL"+sntoa(j+1)+".LTZone"));
	UEPtr<UADItem> ltzoneR=dynamic_pointer_cast<UADItem>(melem->GetComponentL("AfferentR"+sntoa(j+1)+".LTZone"));
	 UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
	 string ltzoneLName,ltzoneRName;
	 ltzoneL->GetLongName(owner, ltzoneLName);
	 ltzoneR->GetLongName(owner, ltzoneRName);
	 string inputNeuronLName = "InputNeuronL"+sntoa(i+1)+sntoa(j+1);
	 string inputNeuronRName = "InputNeuronR"+sntoa(i+1)+sntoa(j+1);
	 string inputLName,inputRName;

	 if(CheckComponentL(inputNeuronLName))
	 {
	  NNet *inputNeuron = static_pointer_cast<NNet>(GetComponent(inputNeuronLName));
	  InputNeurons.push_back(inputNeuron);
	  inputNeuron->GetLongName(owner, inputLName);
	  LeftInputNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(inputNeuronLName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(InputNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(inputNeuronLName);
      res&=(AddComponent(cont) != ForbiddenId);
	  InputNeurons.push_back(static_pointer_cast<NNet>(cont));
	  cont->GetLongName(owner, inputLName);
	  LeftInputNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(inputNeuronLName)));
	 }

	 if(CheckComponentL(inputNeuronRName))
	 {
	  NNet *inputNeuron = static_pointer_cast<NNet>(GetComponent(inputNeuronRName));
	  InputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(inputNeuronRName)));
	  inputNeuron->GetLongName(owner, inputRName);
	  RightInputNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(inputNeuronRName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(InputNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(inputNeuronRName);
      res&=(AddComponent(cont) != ForbiddenId);
	  InputNeurons.push_back(static_pointer_cast<NNet>(cont));
	  cont->GetLongName(owner, inputRName);
	  RightInputNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(inputNeuronRName)));
	 }

	 owner->CreateLink(ltzoneLName,0,inputLName+".PNeuronMembrane.PosChannel");
	 owner->CreateLink(ltzoneLName,0,inputRName+".PNeuronMembrane.NegChannel");
	 owner->CreateLink(ltzoneRName,0,inputLName+".PNeuronMembrane.NegChannel");
	 owner->CreateLink(ltzoneRName,0,inputRName+".PNeuronMembrane.PosChannel");
	}
   }
   //Creating ControlNeurons
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
	NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	LeftControlNeurons.resize(melem->NumControlLoops);
	RightControlNeurons.resize(melem->NumControlLoops);
	for(int j=0;j<melem->NumControlLoops;j++)
	{
	 UEPtr<UADItem> postAfferentL=dynamic_pointer_cast<UADItem>(melem->GetComponentL("PostAfferentL"+sntoa(j+1)));
	 UEPtr<UADItem> postAfferentR=dynamic_pointer_cast<UADItem>(melem->GetComponentL("PostAfferentR"+sntoa(j+1)));
	 UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
	 string postAfferentLName,postAfferentRName;
	 postAfferentL->GetLongName(owner, postAfferentLName);
	 postAfferentR->GetLongName(owner, postAfferentRName);
	 string controlNeuronLName = "ControlNeuronL"+sntoa(i+1)+sntoa(j+1);
	 string controlNeuronRName = "ControlNeuronR"+sntoa(i+1)+sntoa(j+1);
	 string controlLName,controlRName;

	 if(CheckComponentL(controlNeuronLName))
	 {
	  NNet *controlNeuron = static_pointer_cast<NNet>(GetComponent(controlNeuronLName));
	  ControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(controlNeuronLName)));
	  controlNeuron->GetLongName(owner, controlLName);
	  LeftControlNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(controlNeuronLName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return false;
	  cont->SetName(controlNeuronLName);
      res&=(AddComponent(cont) != ForbiddenId);
	  ControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	  cont->GetLongName(owner, controlLName);
      LeftControlNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(controlNeuronLName)));
	 }
	 if(CheckComponentL(controlNeuronRName))
	 {
	  NNet *controlNeuron = static_pointer_cast<NNet>(GetComponent(controlNeuronRName));
	  ControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(controlNeuronRName)));
	  controlNeuron->GetLongName(owner, controlRName);
	  RightControlNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(controlNeuronRName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return false;
	  cont->SetName(controlNeuronRName);
      res&=(AddComponent(cont) != ForbiddenId);
	  ControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	  cont->GetLongName(owner, controlRName);
	  RightControlNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(controlNeuronRName)));
	 }
	 owner->CreateLink(controlLName+".LTZone",0,postAfferentLName+".PNeuronMembrane.PosChannel");
	 owner->CreateLink(controlLName+".LTZone",0,postAfferentRName+".PNeuronMembrane.NegChannel");
	 owner->CreateLink(controlRName+".LTZone",0,postAfferentLName+".PNeuronMembrane.NegChannel");
	 owner->CreateLink(controlRName+".LTZone",0,postAfferentRName+".PNeuronMembrane.PosChannel");
	/*
	 owner->CreateLink(controlLName+".LTZone",0,postAfferentRName+".PNeuronMembrane.PosChannel");
	 owner->CreateLink(controlLName+".LTZone",0,postAfferentLName+".PNeuronMembrane.NegChannel");
	 owner->CreateLink(controlRName+".LTZone",0,postAfferentRName+".PNeuronMembrane.NegChannel");
	 owner->CreateLink(controlRName+".LTZone",0,postAfferentLName+".PNeuronMembrane.PosChannel");  */
	}
   }
   //Creating PreControlNeurons
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
	NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	LeftPreControlNeurons.resize(melem->NumControlLoops);
	RightPreControlNeurons.resize(melem->NumControlLoops);
	for(int j=0;j<melem->NumControlLoops;j++)
	{
	 string preControlNeuronLName = "PreControlNeuronL"+sntoa(i+1)+sntoa(j+1);
	 string preControlNeuronRName = "PreControlNeuronR"+sntoa(i+1)+sntoa(j+1);

	 if(CheckComponentL(preControlNeuronLName))
	 {
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronLName)));
	  LeftPreControlNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronLName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return false;
	  cont->SetName(preControlNeuronLName);
      res&=(AddComponent(cont) != ForbiddenId);
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	  LeftPreControlNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronLName)));
	 }
	 if(CheckComponentL(preControlNeuronRName))
	 {
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronRName)));
	  RightPreControlNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronRName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return false;
	  cont->SetName(preControlNeuronRName);
      res&=(AddComponent(cont) != ForbiddenId);
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	  RightPreControlNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronRName)));
	 }
	}
   }
   //Creating PostInputNeurons
   LeftPostInputNeurons.clear();
   RightPostInputNeurons.clear();
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
	NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	LeftPostInputNeurons.resize(melem->NumControlLoops);
	RightPostInputNeurons.resize(melem->NumControlLoops);
	for(int j=0;j<melem->NumControlLoops;j++)
	{
	 string postInputNeuronLName = "PostInputNeuronL"+sntoa(i+1)+sntoa(j+1);
	 string postInputNeuronRName = "PostInputNeuronR"+sntoa(i+1)+sntoa(j+1);

	 if(CheckComponentL(postInputNeuronLName))
	 {
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronLName)));
	  LeftPostInputNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronLName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return false;
	  cont->SetName(postInputNeuronLName);
      res&=(AddComponent(cont) != ForbiddenId);
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(cont));
	  LeftPostInputNeurons[j].push_back(static_pointer_cast<NNet>(cont));
	 }
	 if(CheckComponentL(postInputNeuronRName))
	 {
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronRName)));
	  RightPostInputNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronRName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return false;
	  cont->SetName(postInputNeuronRName);
      res&=(AddComponent(cont) != ForbiddenId);
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(cont));
	  RightPostInputNeurons[j].push_back(static_pointer_cast<NNet>(cont));
	 }
	}
   }
   //Creating downward links
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
	NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	for(int j=0;j<melem->NumControlLoops;j++)
	{
	 vector <NNet*> start, finish;
	 if(i==0)
	 {
	  start.push_back(LeftInputNeurons[j][0]);
	  finish.push_back(RightPostInputNeurons[j][0]);
	  LinkNeuronsNeg(start,finish);
	  start.clear();
	  finish.clear();
	  start.push_back(RightInputNeurons[j][0]);
	  finish.push_back(LeftPostInputNeurons[j][0]);
	  LinkNeuronsNeg(start,finish);
	  start.clear();
	  finish.clear();
	 }

	 start.push_back(LeftInputNeurons[j][i]);
	 finish.push_back(LeftPostInputNeurons[j][i]);
	 LinkNeurons(start,finish);
	 start.clear();
	 finish.clear();

	 start.push_back(RightInputNeurons[j][i]);
	 finish.push_back(RightPostInputNeurons[j][i]);
	 LinkNeurons(start,finish);
	 start.clear();
	 finish.clear();

	 start.push_back(LeftInputNeurons[j][i]);
	 for(int k=0;k<i;k++)
	 {
	  finish.push_back(LeftPostInputNeurons[j][k]);
	 }
	 LinkNeuronsNeg(start,finish);
	 start.clear();
	 finish.clear();

	 start.push_back(RightInputNeurons[j][i]);
	 for(int k=0;k<i;k++)
	 {
	  finish.push_back(RightPostInputNeurons[j][k]);
	 }
	 LinkNeuronsNeg(start,finish);
	 start.clear();
	 finish.clear();

	 start.push_back(RightInputNeurons[j][i]);
	 for(int k=0;k<i;k++)
	 {
	  finish.push_back(RightPostInputNeurons[j][k]);
	 }
	 LinkNeuronsNeg(start,finish);
	 start.clear();
	 finish.clear();

	 start.push_back(LeftPreControlNeurons[j][i]);
	 for(int k=0;k<=i;k++)
	 {
	  finish.push_back(LeftControlNeurons[j][k]);
	 }
	 LinkNeurons(start,finish);
	 start.clear();
	 finish.clear();

	 start.push_back(RightPreControlNeurons[j][i]);
	 for(int k=0;k<=i;k++)
	 {
	  finish.push_back(RightControlNeurons[j][k]);
	 }
	 LinkNeurons(start,finish);
	}
   }

   CreateExternalControlElements();
   return res;
}

bool NNewPositionControlElement::CreateExternalControlElements(void)
{
   UEPtr<UContainer> cont;
   UEPtr<UStorage> storage = GetStorage();
   bool res(true);
   vector<NNet*> Motions = MotionControl->GetMotion();

   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
	NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	LeftGenerators.resize(melem->NumControlLoops);
	RightGenerators.resize(melem->NumControlLoops);
	for(int j=0;j<melem->NumControlLoops;j++)
	{
	 string generatorLName = "NPGeneratorL"+sntoa(i+1)+sntoa(j+1);
	 string generatorRName = "NPGeneratorR"+sntoa(i+1)+sntoa(j+1);

	 if(CheckComponentL(generatorLName))
	 {
	  Generators.push_back(static_pointer_cast<NNet>(GetComponent(generatorLName)));
	  LeftGenerators[j].push_back(static_pointer_cast<NNet>(GetComponent(generatorLName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPGenerator"));
	  if(!cont)
	   return false;
	  cont->SetName(generatorLName);
      res&=(AddComponent(cont) != ForbiddenId);
	  Generators.push_back(static_pointer_cast<NNet>(cont));
	  LeftGenerators[j].push_back(static_pointer_cast<NNet>(cont));
	 }
	 if(CheckComponentL(generatorRName))
	 {
	  Generators.push_back(static_pointer_cast<NNet>(GetComponent(generatorRName)));
	  RightGenerators[j].push_back(static_pointer_cast<NNet>(GetComponent(generatorRName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPGenerator"));
	  if(!cont)
	   return false;
	  cont->SetName(generatorRName);
      res&=(AddComponent(cont) != ForbiddenId);
	  Generators.push_back(static_pointer_cast<NNet>(cont));
	  RightGenerators[j].push_back(static_pointer_cast<NNet>(cont));
	 }
	}
   }
 return res;
}

bool NNewPositionControlElement::LinkNeurons(vector <NNet*> start, vector <NNet*> finish)
{
	//vector <NNet*>::iterator I,J;
	for(size_t j=0;j<finish.size();j++)
	{
	  NPulseNeuron* neuron=dynamic_cast<NPulseNeuron*>(finish[j]);
	  UEPtr<NPulseMembraneCommon> branch;
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
	   branch=neuron->BranchDendrite("PNeuronMembrane",false);
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
bool NNewPositionControlElement::LinkNeuronsNeg(vector <NNet*> start, vector <NNet*> finish)
{
	//vector <NNet*>::iterator I,J;
	for(size_t j=0;j<finish.size();j++)
	{
	  NPulseNeuron* neuron=dynamic_cast<NPulseNeuron*>(finish[j]);
	  UEPtr<NPulseMembraneCommon> branch;
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
	   branch=neuron->BranchDendrite("PNeuronMembrane",false);
	  NameT finishName = finish[j]->GetName()+"."+branch->GetName()+".NegChannel";
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

bool NNewPositionControlElement::UnlinkNeurons(vector <NNet*> start, vector <NNet*> finish)
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

bool NNewPositionControlElement::LinkGenerators(const bool &value)
{
 return NPositionControlElement::LinkGenerators(value);
}

bool NNewPositionControlElement::LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link, bool is_sim)
{
  vector<NNet*> Motions = MotionControl->GetMotion();

  for(int i=0;i<MotionControl->NumMotionElements;i++)
  {
   NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
   if(!melem)
	continue;
   for(int j=0;j<melem->NumControlLoops;j++)
   {
	string generatorLName = "NPGeneratorL"+sntoa(i+1)+sntoa(j+1);
	string generatorRName = "NPGeneratorR"+sntoa(i+1)+sntoa(j+1);
	string controlNeuronLName = "ControlNeuronL"+sntoa(i+1)+sntoa(j+1)+".PNeuronMembrane.PosChannel";
	string controlNeuronRName = "ControlNeuronR"+sntoa(i+1)+sntoa(j+1)+".PNeuronMembrane.PosChannel";
	string preControlNeuronLName = "PreControlNeuronL"+sntoa(i+1)+sntoa(j+1)+".PNeuronMembrane.PosChannel";
	string preControlNeuronRName = "PreControlNeuronR"+sntoa(i+1)+sntoa(j+1)+".PNeuronMembrane.PosChannel";

	if(link)
	{
	 if(is_sim)
	 {
	  if(CheckLink(generatorLName,controlNeuronLName))
		 BreakLink(generatorLName,controlNeuronLName);
	  if(CheckLink(generatorRName,controlNeuronRName))
		 BreakLink(generatorRName,controlNeuronRName);

	  if(!CheckLink(generatorLName,preControlNeuronLName))
		 CreateLink(generatorLName, 0, preControlNeuronLName);
	  if(!CheckLink(generatorRName,preControlNeuronRName))
		 CreateLink(generatorRName, 0, preControlNeuronRName);
	 }
	 else
	 {
	  if(CheckLink(generatorLName,preControlNeuronLName))
		 BreakLink(generatorLName,preControlNeuronLName);
	  if(CheckLink(generatorRName,preControlNeuronRName))
		 BreakLink(generatorRName,preControlNeuronRName);

	  if(!CheckLink(generatorLName,controlNeuronLName))
		 CreateLink(generatorLName, 0, controlNeuronLName);
	  if(!CheckLink(generatorRName,controlNeuronRName))
		 CreateLink(generatorRName, 0, controlNeuronRName);
	 }
	}
	else
	{
	 if(CheckLink(generatorLName,controlNeuronLName))
		BreakLink(generatorLName,controlNeuronLName);
	 if(CheckLink(generatorRName,controlNeuronRName))
		BreakLink(generatorRName,controlNeuronRName);
	 if(CheckLink(generatorLName,preControlNeuronLName))
		BreakLink(generatorLName,preControlNeuronLName);
	 if(CheckLink(generatorRName,preControlNeuronRName))
		BreakLink(generatorRName,preControlNeuronRName);
	}
   }
  }
 return true;
}
bool NNewPositionControlElement::LinkNegative(vector <NNet*> start, vector <NNet*> finish)
{
   vector<NNet*> Motions = MotionControl->GetMotion();
   vector<NPulseMembrane*> membrToConnectL;
   vector<NPulseMembrane*> membrToConnectR;
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
	NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	for(int j=0;j<melem->NumControlLoops;j++)
	{
	 string inputNeuronLName = "InputNeuronL"+sntoa(i+1)+sntoa(j+1)+".LTZone";
	 string inputNeuronRName = "InputNeuronR"+sntoa(i+1)+sntoa(j+1)+".LTZone";
	 string postInputNeuronLName = "PostInputNeuronL"+sntoa(i+1)+sntoa(j+1);
	 string postInputNeuronRName = "PostInputNeuronR"+sntoa(i+1)+sntoa(j+1);
	 UEPtr<NPulseNeuron> neuronL=static_pointer_cast<NPulseNeuron>(GetComponent(postInputNeuronLName));
	 UEPtr<NPulseNeuron> neuronR=static_pointer_cast<NPulseNeuron>(GetComponent(postInputNeuronRName));
	 for(size_t m=0;m<neuronL->GetNumMembranes();m++)
	 {
	  NPulseMembrane* membr = neuronL->GetMembrane(m);
	  if(CheckLink(inputNeuronLName,postInputNeuronLName+"."+membr->GetName()+".PosChannel"))
		membrToConnectL.push_back(membr);
	 }
	 for(size_t n=0;n<neuronR->GetNumMembranes();n++)
	 {
	  NPulseMembrane* membr = neuronR->GetMembrane(n);
	  if(CheckLink(inputNeuronRName,postInputNeuronRName+"."+membr->GetName()+".PosChannel"))
		membrToConnectR.push_back(membr);
	 }
	}
   }
   for(size_t c=0;c<start.size();c++)
   {
	for(size_t l=0;l<membrToConnectL.size();l++)
	{
	 string membrLName;
	 membrToConnectL[l]->GetLongName(this,membrLName);
	 CreateLink(start[c]->GetName()+".LTZone",0,membrLName+".NegChannel");
	}
	for(size_t k=0;k<membrToConnectR.size();k++)
	{
	 string membrRName;
	 membrToConnectR[k]->GetLongName(this,membrRName);
	 CreateLink(start[c]->GetName()+".LTZone",0,membrRName+".NegChannel");
	}
   }
   return true;
}

vector<NNet*> NNewPositionControlElement::GetInputNeurons(void)
{
 return InputNeurons;
}

vector<NNet*> NNewPositionControlElement::GetControlNeurons(void)
{
 return ControlNeurons;
}

}
#endif



