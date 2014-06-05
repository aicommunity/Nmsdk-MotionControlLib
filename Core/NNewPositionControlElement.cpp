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

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NNewPositionControlElement::NNewPositionControlElement(void)
:   MotionControl("MotionControl",this,0),
	CurrentPosition("CurrentPosition",this),
	TargetPosition("TargetPosition",this),
	InputNeuronType("InputNeuronType",this, &NNewPositionControlElement::SetInputNeuronType),
	ControlNeuronType("ControlNeuronType",this, &NNewPositionControlElement::SetControlNeuronType),
	ExternalControl("ExternalControl", this),
	RememberState("RememberState", this),
	Delta("Delta",this)
{
}

NNewPositionControlElement::~NNewPositionControlElement(void)
{
}
// --------------------------


// ---------------------
// Методы управления параметрами
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
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NNewPositionControlElement* NNewPositionControlElement::New(void)
{
 return new NNewPositionControlElement;
}
// --------------------------


// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NNewPositionControlElement::ADefault(void)
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
bool NNewPositionControlElement::ABuild(void)
{
 CurrentPosition->Assign(2,1,0.0);
 TargetPosition->Assign(MotionControl->GetNumControlLoops(),2*MotionControl->NumMotionElements,0.0);
 InputNeurons.clear();
 ControlNeurons.clear();
 Generators.clear();
 Delta->Assign(2,1,0.0);

 return true;
}

// Сброс процесса счета без потери настроек
bool NNewPositionControlElement::AReset(void)
{
 return true;
}

// Выполняет расчет этого объекта
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
   LinkGenerators(Generators, PreControlNeurons, true);
  }
  else
  {
   LinkGenerators(Generators, PreControlNeurons, false);
  }

 return true;
}
// --------------------------
bool NNewPositionControlElement::CreateNeurons()
{
   UEPtr<UContainer> cont;
   UEPtr<UStorage> storage = GetStorage();
   bool res;
   vector<NNet*> Motions = MotionControl->GetMotion();
   //Creating InputNeurons
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
   //Creating ControlNeurons
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
   //Creating PreControlNeurons
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
	NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	for(int j=0;j<melem->NumControlLoops;j++)
	{
	 string preControlNeuronLName = "PreControlNeuronL"+sntoa(i+1)+sntoa(j+1);
	 string preControlNeuronRName = "PreControlNeuronR"+sntoa(i+1)+sntoa(j+1);

	 if(CheckComponent(preControlNeuronLName))
	 {
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronLName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(preControlNeuronLName);
	  res=AddComponent(cont);
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	 }
	 if(CheckComponent(preControlNeuronRName))
	 {
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronRName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(preControlNeuronRName);
	  res=AddComponent(cont);
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	 }
	}
   }
   //Creating PostInputNeurons
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
	NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	for(int j=0;j<melem->NumControlLoops;j++)
	{
	 string postInputNeuronLName = "PostInputNeuronL"+sntoa(i+1)+sntoa(j+1);
	 string postInputNeuronRName = "PostInputNeuronR"+sntoa(i+1)+sntoa(j+1);

	 if(CheckComponent(postInputNeuronLName))
	 {
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronLName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(postInputNeuronLName);
	  res=AddComponent(cont);
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(cont));
	 }
	 if(CheckComponent(postInputNeuronRName))
	 {
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronRName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(postInputNeuronRName);
	  res=AddComponent(cont);
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(cont));
	 }
	}
   }

   CreateExternalControlElements();
   return true;
}

bool NNewPositionControlElement::CreateExternalControlElements(void)
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
 return true;
}

bool NNewPositionControlElement::LinkNeurons(vector <NNet*> start, vector <NNet*> finish)
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
	   ExternalControl=false;
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

bool NNewPositionControlElement::LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link)
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
	 for(size_t m=0;m<neuronL->Membranes.size();m++)
	 {
	  NPulseMembrane* membr = neuronL->Membranes[m];
	  if(CheckLink(inputNeuronLName,postInputNeuronLName+"."+membr->GetName()+".PosChannel"))
		membrToConnectL.push_back(membr);
	 }
	 for(size_t n=0;n<neuronR->Membranes.size();n++)
	 {
	  NPulseMembrane* membr = neuronR->Membranes[n];
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



