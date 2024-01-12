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
 NumOfPositions("NumOfPositions",this),
 BuildSolo("BuildSolo",this, &NMultiPositionControl::SetBuildSolo),
 InputsNum("InputsNum",this, &NMultiPositionControl::SetInputsNum),
 PCsNum("PCsNum",this, &NMultiPositionControl::SetPCsNum),
 IsNeedToRebuild("IsNeedToRebuild",this, &NMultiPositionControl::SetIsNeedToRebuild)
{
}

NMultiPositionControl::~NMultiPositionControl(void)
{
}
// --------------------------    


// ---------------------
// Методы управления параметрами
// ---------------------
bool NMultiPositionControl::SetBuildSolo(const bool &value)
{
 return true;
}

bool NMultiPositionControl::SetInputsNum(const int &value)
{
 Ready=false;
 //IsNeedToRebuild = true;
 return true;
}

bool NMultiPositionControl::SetPCsNum(const int &value)
{
 Ready=false;
 //IsNeedToRebuild = true;
 return true;
}

bool NMultiPositionControl::SetIsNeedToRebuild(const bool &value)
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
 BuildSolo = true;
 PCsNum = 1;
 InputsNum = 0;
 ExternalControl = true;
 IsNeedToRebuild = false;
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NMultiPositionControl::ABuild(void)
{
 bool a_check = IsNeedToRebuild;
 bool b_check = InputNeurons.empty();
 bool c_check = ControlNeurons.empty();

 if (IsNeedToRebuild)
 {
     InputNeurons.clear();
     ControlNeurons.clear();

     if (BuildSolo)//test
      CreateNeuronsSolo();
     else
      CreateNeurons();

     IsNeedToRebuild = false;
 }


 // if(InputNeurons.empty()||ControlNeurons.empty())//test
 // {
//        if (BuildSolo)//test
//         CreateNeuronsSolo();
//        else
//         CreateNeurons();
 // }

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
 ControlNeuronType = "NNewSPNeuron";
 UEPtr<UContainer> cont;
 UEPtr<UStorage> storage = GetStorage();

// if(InputNeurons.empty()||ControlNeurons.empty())//test
// {
//     if (BuildSolo)
//      CreateNeuronsSolo();
//     else
//      CreateNeurons();
// }

 if(RememberState)
  {
     RememberState = false;
     vector<NNet*> activeInputs, postInputs, preControls, activeControls;

     size_t preControlSize = PreControlNeurons.size();
	 string preControlNeuronName = "PreControlNeuron"+sntoa(preControlSize+1);

     //Adding PreControlNeuron
     if (IsNeedToRebuild)
     {
      if(CheckComponentL(preControlNeuronName))
      {
       PreControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronName)));
       preControls.push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronName)));
      }
     }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(preControlNeuronName);
	  AddComponent(cont);
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	  preControls.push_back(static_pointer_cast<NNet>(cont));
     }

     //Adding PostInputNeuron
	 size_t postInputSize = PostInputNeurons.size();
	 string postInputNeuronName = "PostInputNeuron"+sntoa(postInputSize+1);

     if (IsNeedToRebuild)
     {
       if(CheckComponentL(postInputNeuronName))
       {
        PostInputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronName)));
        postInputs.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronName)));
       }
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

     //Adding Generator
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
    UEPtr<NPulseLTZoneCommon> ltzone=dynamic_pointer_cast<NPulseLTZoneCommon>(InputNeurons[i]->GetComponentL("LTZone"));

    if(ltzone->OutputFrequency->As<double>(0) >0)
	{
     //string check_actInp = InputNeurons[i]->GetName();
	 activeInputs.push_back(InputNeurons[i]);
	 activeControls.push_back(ControlNeurons[i]);
	}
   }
/*   for(size_t c=0;c<ControlNeurons.size();c++)
   {
    UEPtr<UItem> ltzone=dynamic_pointer_cast<UItem>(ControlNeurons[c]->GetComponentL("LTZone"));
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
   if(ExternalControl)
   {
	LinkGenerators(Generators,PreControlNeurons,true,false);
   }
   (*NumOfPositions)++;
  }

 PositionNeurons();//добавлено
 return true;
}
// --------------------------

bool NMultiPositionControl::CreateNeurons(void)
{
 UEPtr<UContainer> cont;
 UEPtr<UStorage> storage = GetStorage();

 //Creating InputNeurons
 int positionControlSize = int(PositionControl->size());
 InputNeuronsByContours.resize(positionControlSize);

 for(int i=0; i<positionControlSize; i++)
 {
  for(int j=0; j<int(PositionControl[i]->PostInputNeurons.size()); j++)
  {
   UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
   string inputNeuronName = "InputNeuron"+sntoa(i+1)+"-"+sntoa(j+1);
   string ltzoneName, inputName;
   //UEPtr<UItem> ltzone=dynamic_pointer_cast<UItem>(PositionControl[i]->PostInputNeurons[j]->GetComponentL(".LTZone"));
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
    InputNeuronType = "NNewSPNeuron";
	cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(InputNeuronType));
	if(!cont)
	 return 0;
	cont->SetName(inputNeuronName);
	AddComponent(cont);
	InputNeurons.push_back(static_pointer_cast<NNet>(cont));
	InputNeuronsByContours[i].push_back(static_pointer_cast<NNet>(cont));
	cont->GetLongName(owner, inputName);
   }
   //Построение связей между PostInput Neurons в NNewPositionControl и InputNeurons в MultiPositionControl
   owner->CreateLink(ltzoneName+".LTZone","Output",inputName+".Soma1.ExcSynapse1", "Input"); //добавлено
  }
 }


 //Creating ControlNeurons 0
 ControlNeuronsByContours.resize(positionControlSize);
 for(int i=0; i<positionControlSize; i++)
 {
  for(int j=0; j<int(PositionControl[i]->PreControlNeurons.size()); j++)
  {
   UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
   string controlNeuronName = "ControlNeuron"+sntoa(i+1)+"-"+sntoa(j+1);
   string outputName, controlName;
   //UEPtr<UItem> output=dynamic_pointer_cast<UItem>(PositionControl[i]->PreControlNeurons[j]->GetComponentL(".Soma1.ExcChannel"));
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
    ControlNeuronType = "NNewSPNeuron";
	cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	if(!cont)
	 return 0;
	cont->SetName(controlNeuronName);
	AddComponent(cont);
	ControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	ControlNeuronsByContours[i].push_back(static_pointer_cast<NNet>(cont));
	cont->GetLongName(owner, controlName);
   }
   //Построение связей между Control Neurons в MultiPositionControl и PreControl Neurons в NewPositionControl
   owner->CreateLink(controlName+".LTZone","Output",outputName+".Soma1.ExcSynapse2", "Input"); //добавлено
  }
 }


 for(int i=0;i<(*NumOfPositions);i++)
 {
     //Creating PreControlNeurons
     string preControlNeuronName = "PreControlNeuron"+sntoa(i+1);
	 if(CheckComponentL(preControlNeuronName))
	 {
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(preControlNeuronName);
	  AddComponent(cont);
	  PreControlNeurons.push_back(static_pointer_cast<NNet>(cont));
	 }

     //Creating PostInputNeurons
	 string postInputNeuronName = "PostInputNeuron"+sntoa(i+1);
	 if(CheckComponentL(postInputNeuronName))
	 {
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronName)));
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
	  if(!cont)
	   return 0;
	  cont->SetName(postInputNeuronName);
	  AddComponent(cont);
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(cont));
	 }

     //Creating Generators
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
  PositionNeurons();
 return true;
}


bool NMultiPositionControl::CreateNeuronsSolo(void)
{
 UEPtr<UContainer> cont;
 UEPtr<UStorage> storage = GetStorage();

 //Creating InputNeurons
 int positionControlSize = PCsNum; //int(PositionControl->size());
 InputNeuronsByContours.resize(positionControlSize);

 for(int i=0; i<positionControlSize; i++)
 {
  for(int j=0; j<InputsNum; j++)//int(PositionControl[i]->PostInputNeurons.size())
  {
   UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
   string inputNeuronName = "InputNeuron"+sntoa(i+1)+"-"+sntoa(j+1);
   string ltzoneName, inputName;
   //PositionControl[i]->PostInputNeurons[j]->GetLongName(owner, ltzoneName);

   if(CheckComponentL(inputNeuronName))
   {
    NNet *inputNeuron = static_pointer_cast<NNet>(GetComponent(inputNeuronName));
    InputNeurons.push_back(inputNeuron);
    InputNeuronsByContours[i].push_back(inputNeuron);
    inputNeuron->GetLongName(owner, inputName);
   }
   else
   {
    InputNeuronType = "NNewSPNeuron";
    cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(InputNeuronType));
    if(!cont)
     return 0;
    cont->SetName(inputNeuronName);
    AddComponent(cont);
    InputNeurons.push_back(static_pointer_cast<NNet>(cont));
    InputNeuronsByContours[i].push_back(static_pointer_cast<NNet>(cont));
    cont->GetLongName(owner, inputName);
   }
   //Построение связей между PostInput Neurons в NNewPositionControl и InputNeurons в MultiPositionControl
   owner->CreateLink(ltzoneName+".LTZone","Output",inputName+".Soma1.ExcSynapse1", "Input"); //добавлено
  }
 }


 //Creating ControlNeurons 0
 ControlNeuronsByContours.resize(positionControlSize);
 for(int i=0; i<positionControlSize; i++)
 {
  for(int j=0; j<InputsNum; j++)//int(PositionControl[i]->PreControlNeurons.size()
  {
   UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
   string controlNeuronName = "ControlNeuron"+sntoa(i+1)+"-"+sntoa(j+1);
   string outputName, controlName;
   //UEPtr<UItem> output=dynamic_pointer_cast<UItem>(PositionControl[i]->PreControlNeurons[j]->GetComponentL(".Soma1.ExcChannel"));
   //PositionControl[i]->PreControlNeurons[j]->GetLongName(owner, outputName);
   if(CheckComponentL(controlNeuronName))
   {
    NNet *controlNeuron = static_pointer_cast<NNet>(GetComponent(controlNeuronName));
    ControlNeurons.push_back(controlNeuron);
    ControlNeuronsByContours[i].push_back(controlNeuron);
    controlNeuron->GetLongName(owner, controlName);
   }
   else
   {
    ControlNeuronType = "NNewSPNeuron";
    cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
    if(!cont)
     return 0;
    cont->SetName(controlNeuronName);
    AddComponent(cont);
    ControlNeurons.push_back(static_pointer_cast<NNet>(cont));
    ControlNeuronsByContours[i].push_back(static_pointer_cast<NNet>(cont));
    cont->GetLongName(owner, controlName);
   }
   //Построение связей между Control Neurons в MultiPositionControl и PreControl Neurons в NewPositionControl
   owner->CreateLink(controlName+".LTZone","Output",outputName+".Soma1.ExcSynapse2", "Input"); //добавлено
  }
 }


 for(int i=0;i<(*NumOfPositions);i++)
 {
     //Creating PreControlNeurons
     string preControlNeuronName = "PreControlNeuron"+sntoa(i+1);
     if(CheckComponentL(preControlNeuronName))
     {
      PreControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(preControlNeuronName)));
     }
     else
     {
      cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
      if(!cont)
       return 0;
      cont->SetName(preControlNeuronName);
      AddComponent(cont);
      PreControlNeurons.push_back(static_pointer_cast<NNet>(cont));
     }

     //Creating PostInputNeurons
     string postInputNeuronName = "PostInputNeuron"+sntoa(i+1);
     if(CheckComponentL(postInputNeuronName))
     {
      PostInputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronName)));
     }
     else
     {
      cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
      if(!cont)
       return 0;
      cont->SetName(postInputNeuronName);
      AddComponent(cont);
      PostInputNeurons.push_back(static_pointer_cast<NNet>(cont));
     }

     //Creating Generators
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
 PositionNeurons();
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
     string controlNeuronName = neurons[j]->GetName()+".Soma1.ExcSynapse1";

	 if(link)
	 {
	  if(!CheckLink(generatorName,controlNeuronName))
      {
          CreateLink(generatorName, "Output", controlNeuronName, "Input");
      }
	 }
	 else
	 {
	  if(CheckLink(generatorName,controlNeuronName))
		BreakLink(generatorName,controlNeuronName);
	 }
	}
   }
  }
 return true;
}

}
#endif



