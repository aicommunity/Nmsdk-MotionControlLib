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
:   MotionControl("MotionControl",this),
    SimControl("SimControl", this),
    Output("Output", this)//,
    //RememberState("RememberState", this)//,
	//CurrentPosition("CurrentPosition",this),
	//TargetPosition("TargetPosition",this),
	//InputNeuronType("InputNeuronType",this, &NNewPositionControlElement::SetInputNeuronType),
	//ControlNeuronType("ControlNeuronType",this, &NNewPositionControlElement::SetControlNeuronType),
	//ExternalControl("ExternalControl", this),
	//Delta("Delta",this)
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
 NPositionControlElement::ADefault();
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

 //удаление старой структуры перед перестройкой, добавлено
 if (MotionControl)
 {
    size_t i_min = 2*(size_t(MotionControl->NumMotionElements));
    if (i_min>0)
    {
      size_t i_max = InputNeurons.size();
      for (size_t i=i_min; i<i_max; i++)
      {
        //NameT check_name = InputNeurons[i]->GetName();
        DelComponent(InputNeurons[i],true);
      }

      i_max = PostInputNeurons.size();
      for (size_t i=i_min; i<i_max; i++)
      {
        //NameT check_name = PostInputNeurons[i]->GetName();
        DelComponent(PostInputNeurons[i],true);
      }

      i_max = PreControlNeurons.size();
      for (size_t i=i_min; i<i_max; i++)
      {
        //NameT check_name = PreControlNeurons[i]->GetName();
        DelComponent(PreControlNeurons[i],true);
      }

      i_max = ControlNeurons.size();
      for (size_t i=i_min; i<i_max; i++)
      {
       //NameT check_name = ControlNeurons[i]->GetName();
        DelComponent(ControlNeurons[i],true);
      }

      i_max = Generators.size();
      for (size_t i=i_min; i<i_max; i++)
      {
        //NameT check_name = Generators[i]->GetName();
        DelComponent(Generators[i],true);
      }
    }
 }

 InputNeurons.clear();
 PostInputNeurons.clear();//добавлено
 PreControlNeurons.clear();//добавлено
 ControlNeurons.clear();

 Generators.clear();
 LeftGenerators.clear();
 RightGenerators.clear();
 Delta->Assign(2,1,0.0);

 return true;
}

// Сброс процесса счета без потери настроек
bool NNewPositionControlElement::AReset(void)
{
 RememberState = false;
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
  vector<NMotionElement *> Motions = MotionControl->GetMotion();
  //vector<NNet*> Motions = MotionControl->GetMotion();
  CurrentPosition->Assign(MotionControl->GetNumControlLoops(),2*MotionControl->NumMotionElements,0.0);
  Delta->Assign(MotionControl->GetNumControlLoops(),2*MotionControl->NumMotionElements,0.0);


  //CurrentPosition Calculation
  for(int i=0;i<MotionControl->NumMotionElements;i++)
  {
   NMotionElement *melem=Motions[i];
   //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
   if(!melem)
	continue;
   for(int j=0;j<melem->NumControlLoops;j++)
   {
	UEPtr<NPulseLTZoneCommon> ltzoneL=dynamic_pointer_cast<NPulseLTZoneCommon>(melem->GetComponentL("AfferentL"+sntoa(j+1)+".LTZone"));
	UEPtr<NPulseLTZoneCommon> ltzoneR=dynamic_pointer_cast<NPulseLTZoneCommon>(melem->GetComponentL("AfferentR"+sntoa(j+1)+".LTZone"));

	double temp=0;
	temp=ltzoneL->OutputFrequency->As<double>(0);
	(*CurrentPosition)(j,2*i)=temp;

	temp=ltzoneL->OutputFrequency->As<double>(0);
	(*CurrentPosition)(j,2*i+1)= temp;
   }
  }


  //Delta Calculation
  for(int i=0;i<MotionControl->NumMotionElements;i++)
  {
   NMotionElement *melem=Motions[i];
   //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
   if(!melem)
	continue;
   for(int j=0;j<melem->NumControlLoops;j++)
   {
	UEPtr<NPulseLTZoneCommon> controlLTZoneL=dynamic_pointer_cast<NPulseLTZoneCommon>(GetComponentL("ControlNeuronL"+sntoa(i+1)+sntoa(j+1)+".LTZone"));
	UEPtr<NPulseLTZoneCommon> controlLTZoneR=dynamic_pointer_cast<NPulseLTZoneCommon>(GetComponentL("ControlNeuronR"+sntoa(i+1)+sntoa(j+1)+".LTZone"));
	UEPtr<NPulseLTZoneCommon> preControlLTZoneL=dynamic_pointer_cast<NPulseLTZoneCommon>(GetComponentL("PreControlNeuronL"+sntoa(i+1)+sntoa(j+1)+".LTZone"));
	UEPtr<NPulseLTZoneCommon> preControlLTZoneR=dynamic_pointer_cast<NPulseLTZoneCommon>(GetComponentL("PreControlNeuronR"+sntoa(i+1)+sntoa(j+1)+".LTZone"));

	(*Delta)(j,2*i)= controlLTZoneL->OutputFrequency->As<double>(0)-preControlLTZoneL->OutputFrequency->As<double>(0);
	(*Delta)(j,2*i+1)= controlLTZoneR->OutputFrequency->As<double>(0)-preControlLTZoneR->OutputFrequency->As<double>(0);
   }
  }

  if(RememberState)
  {
   RememberState = false;
   vector<NNet*> activeInputs, postInputs, preControls, activeControls;

   for(size_t i=0;i<InputNeurons.size();i++)
   {
	UEPtr<NPulseLTZoneCommon> ltzone=dynamic_pointer_cast<NPulseLTZoneCommon>(InputNeurons[i]->GetComponentL("LTZone"));
    //double check = ltzone->OutputFrequency->As<double>(0);
	if(ltzone->OutputFrequency->As<double>(0)>0)
	{
     //NameT check_input_name = InputNeurons[i]->GetName();
	 activeInputs.push_back(InputNeurons[i]);
	 preControls.push_back(PreControlNeurons[i]);
	}
   }
   for(size_t c=0;c<PreControlNeurons.size();c++)
   {
	UEPtr<NPulseLTZoneCommon> ltzone=dynamic_pointer_cast<NPulseLTZoneCommon>(ControlNeurons[c]->GetComponentL("LTZone"));
	if(ltzone->OutputFrequency->As<double>(0)>0)
	{
     //NameT check_postInputs_name = PostInputNeurons[c]->GetName();
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
   vector<NMotionElement *> Motions = MotionControl->GetMotion();

   if(Motions.empty())//добавлено
       return false;


   //Creating InputNeurons
   LeftInputNeurons.clear(); //добавлено
   RightInputNeurons.clear(); //добавлено
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
    NMotionElement *melem=Motions[i];
    //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	LeftInputNeurons.resize(melem->NumControlLoops);
	RightInputNeurons.resize(melem->NumControlLoops);
	for(int j=0;j<melem->NumControlLoops;j++)
	{
     UEPtr<UItem> ltzoneL=dynamic_pointer_cast<UItem>(melem->GetComponentL("AfferentL"+sntoa(j+1)+".LTZone"));
     UEPtr<UItem> ltzoneR=dynamic_pointer_cast<UItem>(melem->GetComponentL("AfferentR"+sntoa(j+1)+".LTZone"));
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

     //Построение связей от афферентных нейронов в EngineMotionControl->MotionElement
     //ко входным нейронам (InputNeuron) в NNewPositionControlElement
	 res = owner->CreateLink(ltzoneLName,"Output",inputLName+".Soma1.ExcSynapse1","Input");
	 res = owner->CreateLink(ltzoneLName,"Output",inputRName+".Soma1.InhSynapse1","Input");
	 res = owner->CreateLink(ltzoneRName,"Output",inputLName+".Soma1.InhSynapse1","Input");
	 res = owner->CreateLink(ltzoneRName,"Output",inputRName+".Soma1.ExcSynapse1","Input");
	}
   }

   int num_motions = MotionControl->NumMotionElements;

   //Creating ControlNeurons
   LeftControlNeurons.clear(); //добавлено
   RightControlNeurons.clear(); //добавлено
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
    NMotionElement *melem=Motions[i];
    //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	LeftControlNeurons.resize(melem->NumControlLoops);
	RightControlNeurons.resize(melem->NumControlLoops);

	for(int j=0;j<melem->NumControlLoops;j++)
	{
     UEPtr<UItem> postAfferentL=dynamic_pointer_cast<UItem>(melem->GetComponentL("PostAfferentL"+sntoa(j+1)));
     UEPtr<UItem> postAfferentR=dynamic_pointer_cast<UItem>(melem->GetComponentL("PostAfferentR"+sntoa(j+1)));
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
      //Adding synapses for links to PrecontrolNeurons
      UEPtr<NPulseMembrane> soma = controlNeuron->GetComponentL<NPulseMembrane>("Soma1",true);
      if(!soma)
       return true;
      soma->NumExcitatorySynapses = (MotionControl->NumMotionElements-i);
      soma->Build();
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
      //Adding synapses for links to PrecontrolNeurons
      UEPtr<NPulseMembrane> soma = cont->GetComponentL<NPulseMembrane>("Soma1",true);
      if(!soma)
       return true;
      soma->NumExcitatorySynapses = (MotionControl->NumMotionElements-i);
      soma->Build();
	 }


	 if(CheckComponentL(controlNeuronRName))
	 {
	  NNet *controlNeuron = static_pointer_cast<NNet>(GetComponent(controlNeuronRName));
	  ControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(controlNeuronRName)));
	  controlNeuron->GetLongName(owner, controlRName);
	  RightControlNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(controlNeuronRName)));
      //Adding synapses for links to PrecontrolNeurons
      UEPtr<NPulseMembrane> soma = controlNeuron->GetComponentL<NPulseMembrane>("Soma1",true);
      if(!soma)
       return true;
      soma->NumExcitatorySynapses = (MotionControl->NumMotionElements-i);
      soma->Build();
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
      //Adding synapses for links to PrecontrolNeurons
      UEPtr<NPulseMembrane> soma = cont->GetComponentL<NPulseMembrane>("Soma1",true);
      if(!soma)
       return true;
      soma->NumExcitatorySynapses = (MotionControl->NumMotionElements-i);
      soma->Build();
	 }

     //Построение связей от ControlNeuron-ов в NNewPositionControlElement к!!!
     //постафферентным нейронам (PostAfferent)в EngineMotionControl->MotionElement
     res = owner->CreateLink(controlLName+".LTZone","Output",postAfferentLName+".Soma1.ExcSynapse2","Input");
	 res = owner->CreateLink(controlLName+".LTZone","Output",postAfferentRName+".Soma1.InhSynapse1","Input");
	 res = owner->CreateLink(controlRName+".LTZone","Output",postAfferentLName+".Soma1.InhSynapse1","Input");
     res = owner->CreateLink(controlRName+".LTZone","Output",postAfferentRName+".Soma1.ExcSynapse2","Input");
	/*
	 owner->CreateLink(controlLName+".LTZone",0,postAfferentRName+".Soma1.ExcChannel");
	 owner->CreateLink(controlLName+".LTZone",0,postAfferentLName+".Soma1.InhChannel");
	 owner->CreateLink(controlRName+".LTZone",0,postAfferentRName+".Soma1.InhChannel");
	 owner->CreateLink(controlRName+".LTZone",0,postAfferentLName+".Soma1.ExcChannel");  */
	}
   }


   //Creating PreControlNeurons
   LeftPreControlNeurons.clear(); //добавлено
   RightPreControlNeurons.clear(); //добавлено
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
    NMotionElement *melem=Motions[i];
    //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
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
    NMotionElement *melem=Motions[i];
    //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
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
      //Adding synapses for links from InputNeurons, добавлено
      UEPtr<NPulseMembrane> soma = GetComponent(postInputNeuronLName)->GetComponentL<NPulseMembrane>("Soma1",true);
      if(!soma)
       return true;
      soma->NumInhibitorySynapses = (MotionControl->NumMotionElements-i);
      soma->Build();
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
      //Adding synapses for links from InputNeurons, добавлено
      UEPtr<NPulseMembrane> soma = cont->GetComponentL<NPulseMembrane>("Soma1",true);
      if(!soma)
       return true;
      soma->NumInhibitorySynapses = (MotionControl->NumMotionElements-i);
      soma->Build();
	 }


	 if(CheckComponentL(postInputNeuronRName))
	 {
	  PostInputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronRName)));
	  RightPostInputNeurons[j].push_back(static_pointer_cast<NNet>(GetComponent(postInputNeuronRName)));
      //Adding synapses for links from InputNeurons, добавлено
      UEPtr<NPulseMembrane> soma = GetComponent(postInputNeuronRName)->GetComponentL<NPulseMembrane>("Soma1",true);
      if(!soma)
       return true;
      soma->NumInhibitorySynapses = (MotionControl->NumMotionElements-i);
      soma->Build();
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

      //Adding synapses for links from InputNeurons, добавлено
      UEPtr<NPulseMembrane> soma = cont->GetComponentL<NPulseMembrane>("Soma1",true);
      if(!soma)
       return true;
      soma->NumInhibitorySynapses = (MotionControl->NumMotionElements-i);
      soma->Build();
	 }
	}
   }


   //Creating downward links
   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
    NMotionElement *melem=Motions[i];
    //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
	if(!melem)
	 continue;
	for(int j=0;j<melem->NumControlLoops;j++)
	{
	 vector <NNet*> start, finish;
     vector <int> syn_num;

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
     //NameT check_startName = LeftInputNeurons[j][i]->GetName();
     //NameT check_finisName = LeftPostInputNeurons[j][i]->GetName();
     LinkNeurons(start,finish);
	 start.clear();
	 finish.clear();

	 start.push_back(RightInputNeurons[j][i]);
	 finish.push_back(RightPostInputNeurons[j][i]);
     //check_startName = RightInputNeurons[j][i]->GetName();
     //check_finisName = RightPostInputNeurons[j][i]->GetName();
	 LinkNeurons(start,finish);
	 start.clear();
	 finish.clear();

     //check_startName = LeftInputNeurons[j][i]->GetName();
	 start.push_back(LeftInputNeurons[j][i]);
	 for(int k=0;k<i;k++)
	 {
      //check_finisName = LeftPostInputNeurons[j][k]->GetName();
	  finish.push_back(LeftPostInputNeurons[j][k]);
	 }
	 LinkNeuronsNeg(start,finish);
	 start.clear();
	 finish.clear();

	 start.push_back(RightInputNeurons[j][i]);
     //check_startName = RightInputNeurons[j][i]->GetName();
	 for(int k=0;k<i;k++)
	 {
      //check_finisName = RightPostInputNeurons[j][k]->GetName();
	  finish.push_back(RightPostInputNeurons[j][k]);  
	 }
	 LinkNeuronsNeg(start,finish);
	 start.clear();
	 finish.clear();

	 start.push_back(RightInputNeurons[j][i]);
     //check_startName = RightInputNeurons[j][i]->GetName();
	 for(int k=0;k<i;k++)
	 {
      //check_finisName = RightPostInputNeurons[j][k]->GetName();
	  finish.push_back(RightPostInputNeurons[j][k]);
	 }
	 LinkNeuronsNeg(start,finish);
	 start.clear();
	 finish.clear();

	 start.push_back(LeftPreControlNeurons[j][i]);
     //check_startName = LeftPreControlNeurons[j][i]->GetName();
	 for(int k=0;k<=i;k++)
	 {
      //check_finisName = LeftControlNeurons[j][k]->GetName();
      finish.push_back(LeftControlNeurons[j][k]);
	 }
	 LinkNeurons(start,finish);
	 start.clear();
	 finish.clear();

	 start.push_back(RightPreControlNeurons[j][i]);
     //check_startName = RightPreControlNeurons[j][i]->GetName();
	 for(int k=0;k<=i;k++)
	 {
      //check_finisName = RightControlNeurons[j][k]->GetName();
	  finish.push_back(RightControlNeurons[j][k]);
	 }
	 LinkNeurons(start,finish);
	}
   }

   CreateExternalControlElements();
    //Setting coordinates
   PositionNeurons();
   return res;
}

bool NNewPositionControlElement::CreateExternalControlElements(void)
{
   UEPtr<UContainer> cont;
   UEPtr<UStorage> storage = GetStorage();
   bool res(true);
   vector<NMotionElement *> Motions = MotionControl->GetMotion();
   //vector<NNet*> Motions = MotionControl->GetMotion();
   if(Motions.empty())//добавлено
       return false;

   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
    NMotionElement *melem=Motions[i];
    //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
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
      NameT finishName;
      int syns_max = branch->NumExcitatorySynapses;

	  for(size_t i=0;i<start.size();i++)
	  {
          startName = start[i]->GetName()+".LTZone";

          for (int m = 0; m<syns_max; m++)
          {
              UEPtr<NPulseSynapse> syn = finish[j]->GetComponentL<NPulseSynapse>("Soma1.ExcSynapse"+sntoa(m+1),true);
              if(!syn)
                return true;

              finishName = finish[j]->GetName()+"."+branch->GetName()+".ExcSynapse"+sntoa(m+1);

              if(CheckLink(startName,finishName))
                break;//перейти к следующему элементу в start

              if (syn->Input.IsConnected())
                continue; //перейти к следующему синапсу

              CreateLink(startName, "Output", finishName,"Input");
              break;
          }
      }
//	   ExternalControl=false;
    }
    return true;
}


bool NNewPositionControlElement::LinkNeuronsNeg(vector <NNet*> start, vector <NNet*> finish)
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

      //NameT finishName = finish[j]->GetName()+"."+branch->GetName()+".InhSynapse1";
      NameT startName;
      NameT finishName;
      int syns_max = branch->NumInhibitorySynapses;

	  for(size_t i=0;i<start.size();i++)
	  {
       startName = start[i]->GetName()+".LTZone";

       for (int m = 0; m<syns_max; m++)
       {
           UEPtr<NPulseSynapse> syn = finish[j]->GetComponentL<NPulseSynapse>("Soma1.InhSynapse"+sntoa(m+1),true);
           if(!syn)
             return true;

           finishName = finish[j]->GetName()+"."+branch->GetName()+".InhSynapse"+sntoa(m+1);

           if(CheckLink(startName,finishName))
             break;//перейти к следующему элементу в start

           if (syn->Input.IsConnected())
             continue; //перейти к следующему синапсу

           CreateLink(startName, "Output", finishName,"Input");
           break;
       }
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
	NameT finishName = finish[j]->GetName()+"."+neuron->Membranes[k]->GetName()+".ExcSynapse1";
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
    vector<NMotionElement *> Motions = MotionControl->GetMotion();
  //vector<NNet*> Motions = MotionControl->GetMotion();

  //int check = MotionControl->NumMotionElements;
  //int i=0;
  for(int i=0; i<MotionControl->NumMotionElements;i++)
  {
   NMotionElement *melem=Motions[i];
   //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
   if(!melem)
	continue;
   for(int j=0;j<melem->NumControlLoops;j++)
   {
	string generatorLName = "NPGeneratorL"+sntoa(i+1)+sntoa(j+1);
	string generatorRName = "NPGeneratorR"+sntoa(i+1)+sntoa(j+1);
	string controlNeuronLName = "ControlNeuronL"+sntoa(i+1)+sntoa(j+1)+".Soma1.ExcSynapse1";
	string controlNeuronRName = "ControlNeuronR"+sntoa(i+1)+sntoa(j+1)+".Soma1.ExcSynapse1";
	string preControlNeuronLName = "PreControlNeuronL"+sntoa(i+1)+sntoa(j+1)+".Soma1.ExcSynapse1";
	string preControlNeuronRName = "PreControlNeuronR"+sntoa(i+1)+sntoa(j+1)+".Soma1.ExcSynapse1";

	if(link)
	{
	 if(is_sim)
	 {
	  if(CheckLink(generatorLName,controlNeuronLName))
		 BreakLink(generatorLName,controlNeuronLName);
	  if(CheckLink(generatorRName,controlNeuronRName))
		 BreakLink(generatorRName,controlNeuronRName);

	  if(!CheckLink(generatorLName,preControlNeuronLName))
		 CreateLink(generatorLName, "Output", preControlNeuronLName,"Input");
	  if(!CheckLink(generatorRName,preControlNeuronRName))
		 CreateLink(generatorRName, "Output", preControlNeuronRName,"Input");
	 }
	 else
	 {
	  if(CheckLink(generatorLName,preControlNeuronLName))
		 BreakLink(generatorLName,preControlNeuronLName);
	  if(CheckLink(generatorRName,preControlNeuronRName))
		 BreakLink(generatorRName,preControlNeuronRName);

	  if(!CheckLink(generatorLName,controlNeuronLName))
		 CreateLink(generatorLName, "Output", controlNeuronLName, "Input");
	  if(!CheckLink(generatorRName,controlNeuronRName))
		 CreateLink(generatorRName, "Output", controlNeuronRName, "Input");
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
  PositionNeurons();
 return true;
}
bool NNewPositionControlElement::LinkNegative(vector <NNet*> start, vector <NNet*> finish)
{
   vector<NMotionElement *> Motions = MotionControl->GetMotion();
   //vector<NNet*> Motions = MotionControl->GetMotion();
   vector<NPulseMembrane*> membrToConnectL;
   vector<NPulseMembrane*> membrToConnectR;

   for(int i=0;i<MotionControl->NumMotionElements;i++)
   {
    NMotionElement *melem=Motions[i];
    //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
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
      //string check_startName = inputNeuronLName;
      //string check_finishName = postInputNeuronLName+"."+membr->GetName()+".ExcSynapse1";
	  if(CheckLink(inputNeuronLName,postInputNeuronLName+"."+membr->GetName()+".ExcSynapse1"))
		membrToConnectL.push_back(membr);
	 }
	 for(size_t n=0;n<neuronR->GetNumMembranes();n++)
	 {
	  NPulseMembrane* membr = neuronR->GetMembrane(n);
      //string check_startName = inputNeuronRName;
      //string check_finishName = postInputNeuronRName+"."+membr->GetName()+".ExcSynapse1";
	  if(CheckLink(inputNeuronRName,postInputNeuronRName+"."+membr->GetName()+".ExcSynapse1"))
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
     //string check_startName = start[c]->GetName()+".LTZone";
     //string check_finishName = membrLName+".InhSynapse1";
	 CreateLink(start[c]->GetName()+".LTZone","Output",membrLName+".InhSynapse1","Input");
	}

	for(size_t k=0;k<membrToConnectR.size();k++)
	{
	 string membrRName;
	 membrToConnectR[k]->GetLongName(this,membrRName);
     //string check_startName = start[c]->GetName()+".LTZone";
     //string check_finishName = membrRName+".InhSynapse1";
	 CreateLink(start[c]->GetName()+".LTZone","Output",membrRName+".InhSynapse1","Input");
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



