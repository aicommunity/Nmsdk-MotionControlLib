/* ***********************************************************

E-mail:        
Url:           

This file is part of the project: 

File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NMultiPositionControl_CPP
#define NMultiPositionControl_CPP

#include "NMultiPositionControl.h"

namespace NMSDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NMultiPositionControl::NMultiPositionControl(void)
:
 PositionControl("PositionControl", this),
 NumOfPositions("NumOfPositions",this)
{
}

NMultiPositionControl::~NMultiPositionControl(void)
{
}
// --------------------------    


// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------

// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NMultiPositionControl* NMultiPositionControl::New(void)
{
 return new NMultiPositionControl;
}
// --------------------------


// --------------------------
// Скрытые методы управления счетом 
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NMultiPositionControl::ADefault(void)
{
 NPositionControlElement::ADefault();
 NumOfPositions=0;
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NMultiPositionControl::ABuild(void)
{
 InputNeurons.clear();
 ControlNeurons.clear();
 return true;
}

// Сброс процесса счета без потери настроек
bool NMultiPositionControl::AReset(void)
{
 return true;
}

// Выполняет расчет этого объекта
bool NMultiPositionControl::ACalculate(void)
{
 ControlNeuronType = "NNewSynSPNeuron";
 UEPtr<UContainer> cont;
 UEPtr<UStorage> storage = GetStorage();

 if(InputNeurons.empty()||ControlNeurons.empty())
	CreateNeurons();

 if(*RememberState)
  {
   RememberState = false;
   vector<NNet*> activeInputs, postInputs, preControls, activeControls;
   size_t preControlSize = PreControlNeurons.size();
	 string preControlNeuronName = "PreControlNeuron"+sntoa(preControlSize+1);
	 if(CheckComponentL(preControlNeuronName))
	 {
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronName)));
	  preControls.push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(*ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(preControlNeuronName);
	  AddComponent(cont);
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	  preControls.push_back(static_pointer_cast<NNet>(cont));
	 }
	 size_t postInputSize = PostInputNeurons.size();
	 string postInputNeuronName = "PostInputNeuron"+sntoa(postInputSize+1);
	 if(CheckComponentL(postInputNeuronName))
	 {
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronName)));
	  postInputs.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(*ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(postInputNeuronName);
	  AddComponent(cont);
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(cont));
	  postInputs.push_back(static_pointer_cast<NNet>(cont));
	 }
	 size_t generatorSize = Generators.size();
	 string generatorName = "PGenerator"+sntoa(generatorSize+1);
	 if(CheckComponentL(generatorName))
	 {
	  Generators.push_back(static_pointer_cast<NNet>(GetComponent(generatorName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPGenerator"));
	  if(!cont)
	   return 0;
	  cont->SetName(generatorName);
	  AddComponent(cont);
	  Generators.push_back(static_pointer_cast<NNet>(cont));
	 }
   for(size_t i=0;i<InputNeurons.size();i++)
   {
	UEPtr<UDynamicMatNet> ltzone=dynamic_pointer_cast<UDynamicMatNet>(InputNeurons[i]->GetComponentL("LTZone"));
	if(ltzone->GetOutputData(2).Double[0]>0)
	{
	 activeInputs.push_back(InputNeurons[i]);
	 activeControls.push_back(ControlNeurons[i]);
	}
   }
/*   for(size_t c=0;c<ControlNeurons.size();c++)
   {
	UEPtr<UADItem> ltzone=dynamic_pointer_cast<UADItem>(ControlNeurons[c]->GetComponentL("LTZone"));
	if(ltzone->GetOutputData(2).Double[0]>0)
	{
	 activeControls.push_back(ControlNeurons[c]);
	 size_t postInputSize = PostInputNeurons.size();
	 string postInputNeuronName = "PostInputNeuron"+sntoa(postInputSize+1);
	 if(CheckComponentL(postInputNeuronName))
	 {
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronName)));
	  postInputs.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(postInputNeuronName);
	  AddComponent(cont);
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(cont));
	  postInputs.push_back(static_pointer_cast<NNet>(cont));
	 }
	}
   }
*/
   //LinkNegative(activeInputs, postInputs);
   LinkNeurons(activeInputs, postInputs);
   LinkNeurons(preControls, activeControls);
   if(*ExternalControl)
   {
	LinkGenerators(Generators,PreControlNeurons,true,false);
   }
   NumOfPositions=NumOfPositions+1;
  }

 return true;
}
// --------------------------

bool NMultiPositionControl::CreateNeurons(void)
{
 UEPtr<UContainer> cont;
 UEPtr<UStorage> storage = GetStorage();
 //Creating InputNeurons
 size_t positionControlSize = PositionControl.size();
 InputNeuronsByContours.resize(positionControlSize);
 for(size_t i=0; i<positionControlSize; i++)
 {
  for(size_t j=0; j<PositionControl[i]->PostInputNeurons.size(); j++)
  {
   UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
   string inputNeuronName = "InputNeuron"+sntoa(i+1)+"-"+sntoa(j+1);
   string ltzoneName, inputName;
   //UEPtr<UADItem> ltzone=dynamic_pointer_cast<UADItem>(PositionControl[i]->PostInputNeurons[j]->GetComponentL(".LTZone"));
   PositionControl[i]->PostInputNeurons[j]->GetLongName(owner, ltzoneName);
   
   if(CheckComponentL(inputNeuronName))
   {
	NNet *inputNeuron = static_pointer_cast<NNet>(GetComponent(inputNeuronName));
	InputNeurons.push_back(inputNeuron);
	InputNeuronsByContours[i].push_back(inputNeuron);
	inputNeuron->GetLongName(owner, inputName);
   }
   else
   {
	InputNeuronType = "NNewSynSPNeuron";
	cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(*InputNeuronType));
	if(!cont)
	 return 0;
	cont->SetName(inputNeuronName);
	AddComponent(cont);
	InputNeurons.push_back(static_pointer_cast<NNet>(cont));
	InputNeuronsByContours[i].push_back(static_pointer_cast<NNet>(cont));
	cont->GetLongName(owner, inputName);
   }
   owner->CreateLink(ltzoneName+".LTZone","DataOutput0",inputName+".PNeuronMembrane.PosChannel","");
  }
 }
 //Creating ControlNeurons 0
 ControlNeuronsByContours.resize(positionControlSize);
 for(size_t i=0; i<positionControlSize; i++)
 {
  for(size_t j=0; j<PositionControl[i]->PreControlNeurons.size(); j++)
  {
   UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
   string controlNeuronName = "ControlNeuron"+sntoa(i+1)+"-"+sntoa(j+1);
   string outputName, controlName;
   //UEPtr<UADItem> output=dynamic_pointer_cast<UADItem>(PositionControl[i]->PreControlNeurons[j]->GetComponentL(".PNeuronMembrane.PosChannel"));
   PositionControl[i]->PreControlNeurons[j]->GetLongName(owner, outputName);
   if(CheckComponentL(controlNeuronName))
   {
	NNet *controlNeuron = static_pointer_cast<NNet>(GetComponent(controlNeuronName));
	ControlNeurons.push_back(controlNeuron);
	ControlNeuronsByContours[i].push_back(controlNeuron);
	controlNeuron->GetLongName(owner, controlName);
   }
   else
   {
    ControlNeuronType = "NNewSynSPNeuron";
	cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(*ControlNeuronType));
	if(!cont)
	 return 0;
	cont->SetName(controlNeuronName);
	AddComponent(cont);
	ControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	ControlNeuronsByContours[i].push_back(static_pointer_cast<NNet>(cont));
	cont->GetLongName(owner, controlName);
   }
   owner->CreateLink(controlName+".LTZone","DataOutput0",outputName+".PNeuronMembrane.PosChannel","");
  }
 }
 for(int i=0;i<(*NumOfPositions);i++)
 {
  string preControlNeuronName = "PreControlNeuron"+sntoa(i+1);
	 if(CheckComponentL(preControlNeuronName))
	 {
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(*ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(preControlNeuronName);
	  AddComponent(cont);
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	 }
	 string postInputNeuronName = "PostInputNeuron"+sntoa(i+1);
	 if(CheckComponentL(postInputNeuronName))
	 {
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(*ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(postInputNeuronName);
	  AddComponent(cont);
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(cont));
	 }
	 string generatorName = "PGenerator"+sntoa(i+1);
	 if(CheckComponentL(generatorName))
	 {
	  Generators.push_back(static_pointer_cast<NNet>(GetComponent(generatorName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPGenerator"));
	  if(!cont)
	   return 0;
	  cont->SetName(generatorName);
	  AddComponent(cont);
	  Generators.push_back(static_pointer_cast<NNet>(cont));
	 }
 }
 return true;
}

bool NMultiPositionControl::LinkGenerators(const bool &value)
{
 LinkGenerators(Generators,PreControlNeurons,value,false);
 return true;
}

bool NMultiPositionControl::LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link, bool is_sim)
{

  for(size_t i=0;i<generators.size();i++)
  {
   for(size_t j=0;j<neurons.size();j++)
   {
	if(j==i)
	{
	 string generatorName = generators[i]->GetName();
	 string controlNeuronName = neurons[j]->GetName()+".PNeuronMembrane.PosChannel";

	 if(link)
	 {
	  if(!IsLinkExists(generatorName,controlNeuronName))
		 CreateLink(generatorName, "DataOutput0", controlNeuronName,"");
	 }
	 else
	 {
	  if(IsLinkExists(generatorName,controlNeuronName))
		BreakLink(generatorName,controlNeuronName);
	 }
	}
   }
  }
 return true;
}

}
#endif



