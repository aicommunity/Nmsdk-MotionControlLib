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

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NPositionControlElement::NPositionControlElement(void)
:   MotionControl("MotionControl",this,0),
	CurrentPosition("CurrentPosition",this),
	TargetPosition("TargetPosition",this),
	InputNeuronType("InputNeuronType",this, &NPositionControlElement::SetInputNeuronType),
	ControlNeuronType("ControlNeuronType",this, &NPositionControlElement::SetControlNeuronType),
	ExternalControl("ExternalControl", this),
	RememberState("RememberState", this)
{
}

NPositionControlElement::~NPositionControlElement(void)
{
}
// --------------------------


// ---------------------
// Методы управления параметрами
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
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NPositionControlElement* NPositionControlElement::New(void)
{
 return new NPositionControlElement;
}
// --------------------------


// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NPositionControlElement::ADefault(void)
{
 InputNeuronType = "NNewSynSPNeuron";
 ControlNeuronType = "NNewSynSPNeuron";
 ExternalControl = false;
 RememberState = false;
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NPositionControlElement::ABuild(void)
{
 CurrentPosition->Assign(2,1,0.0);
 TargetPosition->Assign(MotionControl->GetNumControlLoops(),2*MotionControl->NumMotionElements,0.0);
 InputNeurons.clear();
 ControlNeurons.clear();
 Generators.clear();

 return true;
}

// Сброс процесса счета без потери настроек
bool NPositionControlElement::AReset(void)
{
 return true;
}

// Выполняет расчет этого объекта
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
  if(RememberState)
  {
   RememberState = false;
   vector<NNet*> activeInputs, activeControls;
   for(int i=0;i<InputNeurons.size();i++)
   {
	UEPtr<UADItem> ltzone=dynamic_pointer_cast<UADItem>(InputNeurons[i]->GetComponentL("LTZone"));
	if(ltzone->GetOutputData(2).Double[0]>0)
	 activeInputs.push_back(InputNeurons[i]);
   }
   for(int c=0;c<ControlNeurons.size();c++)
   {
	UEPtr<UADItem> ltzone=dynamic_pointer_cast<UADItem>(ControlNeurons[c]->GetComponentL("LTZone"));
	if(ltzone->GetOutputData(2).Double[0]>0)
	 activeControls.push_back(ControlNeurons[c]);
   }
   LinkNeurons(activeInputs, activeControls);
  }
  if(ExternalControl)
  {
   UnlinkNeurons(InputNeurons, ControlNeurons);
   LinkGenerators(Generators, ControlNeurons, true);
  }
  else
  {
   LinkGenerators(Generators, ControlNeurons, false);
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
	 UEPtr<UADItem> ltzoneL=dynamic_pointer_cast<UADItem>(melem->GetComponentL("AfferentL"+sntoa(j+1)+".LTZone"));
	UEPtr<UADItem> ltzoneR=dynamic_pointer_cast<UADItem>(melem->GetComponentL("AfferentR"+sntoa(j+1)+".LTZone"));
	 UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
	 string ltzoneLName,ltzoneRName;
	 ltzoneL->GetLongName(owner, ltzoneLName);
	 ltzoneR->GetLongName(owner, ltzoneRName);
	 string inputNeuronLName = "InputNeuronL"+sntoa(i+1)+sntoa(j+1);
	 string inputNeuronRName = "InputNeuronR"+sntoa(i+1)+sntoa(j+1);
	 string inputLName,inputRName;

	 if(CheckComponent(inputNeuronLName))
	 {
	  NNet *inputNeuron = static_pointer_cast<NNet>(GetComponent(inputNeuronLName));
	  InputNeurons.push_back(inputNeuron);
	  inputNeuron->GetLongName(owner, inputLName);
	 }
	 else
	 {
	 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(InputNeuronType));
	 if(!cont)
	  return 0;
	 cont->SetName(inputNeuronLName);
	 res=AddComponent(cont);
	 InputNeurons.push_back(static_pointer_cast<NNet>(cont));
	 cont->GetLongName(owner, inputLName);
	 }

	 if(CheckComponent(inputNeuronRName))
	 {
	  NNet *inputNeuron = static_pointer_cast<NNet>(GetComponent(inputNeuronRName));
	  InputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(inputNeuronRName)));
	  inputNeuron->GetLongName(owner, inputRName);
	 }
	 else
	 {
	 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(InputNeuronType));
	 if(!cont)
	  return 0;
	 cont->SetName(inputNeuronRName);
	 res=AddComponent(cont);
	 InputNeurons.push_back(static_pointer_cast<NNet>(cont));
	 cont->GetLongName(owner, inputRName);
	 }

	 owner->CreateLink(ltzoneLName,0,inputLName+".PNeuronMembrane.PosChannel");
	 owner->CreateLink(ltzoneLName,0,inputRName+".PNeuronMembrane.NegChannel");
	 owner->CreateLink(ltzoneRName,0,inputLName+".PNeuronMembrane.NegChannel");
	 owner->CreateLink(ltzoneRName,0,inputRName+".PNeuronMembrane.PosChannel");
	}
   }
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
	NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
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

	 if(CheckComponent(controlNeuronLName))
	 {
	  NNet *controlNeuron = static_pointer_cast<NNet>(GetComponent(controlNeuronLName));
	  ControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(controlNeuronLName)));
	  controlNeuron->GetLongName(owner, controlLName);
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(controlNeuronLName);
	  res=AddComponent(cont);
	  ControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	  cont->GetLongName(owner, controlLName);
	 }
	 if(CheckComponent(controlNeuronRName))
	 {
	  NNet *controlNeuron = static_pointer_cast<NNet>(GetComponent(controlNeuronRName));
	  ControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(controlNeuronRName)));
	  controlNeuron->GetLongName(owner, controlRName);
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(controlNeuronRName);
	  res=AddComponent(cont);
	  ControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	  cont->GetLongName(owner, controlRName);
	 }
/*	 owner->CreateLink(controlLName+".LTZone",0,postAfferentLName+".PNeuronMembrane.PosChannel");
	 owner->CreateLink(controlLName+".LTZone",0,postAfferentRName+".PNeuronMembrane.NegChannel");
	 owner->CreateLink(controlRName+".LTZone",0,postAfferentLName+".PNeuronMembrane.NegChannel");
	 owner->CreateLink(controlRName+".LTZone",0,postAfferentRName+".PNeuronMembrane.PosChannel");
  */
	 owner->CreateLink(controlLName+".LTZone",0,postAfferentRName+".PNeuronMembrane.PosChannel");
	 owner->CreateLink(controlLName+".LTZone",0,postAfferentLName+".PNeuronMembrane.NegChannel");
	 owner->CreateLink(controlRName+".LTZone",0,postAfferentRName+".PNeuronMembrane.NegChannel");
	 owner->CreateLink(controlRName+".LTZone",0,postAfferentLName+".PNeuronMembrane.PosChannel");
	}
   }
   CreateExternalControlElements();
   return true;
}

bool NPositionControlElement::CreateExternalControlElements(void)
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
	 string generatorLName = "NPGeneratorL"+sntoa(i+1)+sntoa(j+1);
	 string generatorRName = "NPGeneratorR"+sntoa(i+1)+sntoa(j+1);

	 if(CheckComponent(generatorLName))
	 {
	  Generators.push_back(static_pointer_cast<NNet>(GetComponent(generatorLName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPGenerator"));
	  if(!cont)
	   return 0;
	  cont->SetName(generatorLName);
	  res=AddComponent(cont);
	  Generators.push_back(static_pointer_cast<NNet>(cont));
	 }
	 if(CheckComponent(generatorRName))
	 {
	  Generators.push_back(static_pointer_cast<NNet>(GetComponent(generatorRName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPGenerator"));
	  if(!cont)
	   return 0;
	  cont->SetName(generatorRName);
	  res=AddComponent(cont);
	  Generators.push_back(static_pointer_cast<NNet>(cont));
	 }
	}
   }
}

bool NPositionControlElement::LinkNeurons(vector <NNet*> start, vector <NNet*> finish)
{
	//vector <NNet*>::iterator I,J;
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
	   if(!CheckLink(startName,finishName))
		CreateLink(startName, 0, finishName);
	  }
	}

	return true;
}

bool NPositionControlElement::UnlinkNeurons(vector <NNet*> start, vector <NNet*> finish)
{
 for(int i=0;i<start.size();i++)
 {
  NameT startName = start[i]->GetName()+".LTZone";
  NPulseNeuron* neuron=dynamic_cast<NPulseNeuron*>(finish[i]);
  for(int j=0;j<finish.size();j++)
  {
   for(int k=0;k<neuron->Membranes.size();k++)
   {
	NameT finishName = finish[j]->GetName()+"."+neuron->Membranes[k]->GetName()+".PosChannel";
	if(CheckLink(startName,finishName))
	   BreakLink(startName,finishName);
   }
  }
 }
 return true;
}

bool NPositionControlElement::LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link)
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

	if(link)
	{
	 if(!CheckLink(generatorLName,controlNeuronRName))
		CreateLink(generatorLName, 0, controlNeuronRName);
	 if(!CheckLink(generatorRName,controlNeuronLName))
		CreateLink(generatorRName, 0, controlNeuronLName);
	}
	else
	{
	 if(CheckLink(generatorLName,controlNeuronRName))
	   BreakLink(generatorLName,controlNeuronRName);
	 if(CheckLink(generatorRName,controlNeuronLName))
	   BreakLink(generatorRName,controlNeuronLName);
	}
   }
  }
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



