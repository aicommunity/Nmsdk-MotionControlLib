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
 MotionControlElement = safe_shared_cast<NEngineMotionControl>(dynamic_cast<NEngineMotionControl*>(MotionControl.GetItem()));
 CurrentPosition->Assign(2,1,0.0);

 //�������� ������ ��������� ����� ������������, ���������
 if (MotionControlElement)
 {
  TargetPosition->Assign(MotionControlElement->GetNumControlLoops(),2*MotionControlElement->NumMotionElements,0.0);
  size_t i_min = 2*(size_t(MotionControlElement->NumMotionElements));
  if (i_min>0)
  {
    size_t i_max = InputNeurons.size();
    for (size_t i=i_min; i<i_max; i++)
    {
      //NameT check_name = InputNeurons[i]->GetName();
      DelComponent(safe_shared_cast<UContainer>(InputNeurons[i]),true);
    }

    i_max = PostInputNeurons.size();
    for (size_t i=i_min; i<i_max; i++)
    {
      //NameT check_name = PostInputNeurons[i]->GetName();
      DelComponent(safe_shared_cast<UContainer>(PostInputNeurons[i]),true);
    }

    i_max = PreControlNeurons.size();
    for (size_t i=i_min; i<i_max; i++)
    {
      //NameT check_name = PreControlNeurons[i]->GetName();
      DelComponent(safe_shared_cast<UContainer>(PreControlNeurons[i]),true);
    }

    i_max = ControlNeurons.size();
    for (size_t i=i_min; i<i_max; i++)
    {
     //NameT check_name = ControlNeurons[i]->GetName();
      DelComponent(safe_shared_cast<UContainer>(ControlNeurons[i]),true);
    }

    i_max = Generators.size();
    for (size_t i=i_min; i<i_max; i++)
    {
      //NameT check_name = Generators[i]->GetName();
      DelComponent(safe_shared_cast<UContainer>(Generators[i]),true);
    }
  }
 }

 InputNeurons.clear();
 PostInputNeurons.clear();//���������
 PreControlNeurons.clear();//���������
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
 if(!MotionControlElement)
  return true;

  if(MotionControlElement->GetNumControlLoops() != CurrentPosition->GetRows() ||
  2*MotionControlElement->NumMotionElements != CurrentPosition->GetCols())
	 {
	  Ready=false;
	  Reset();
	 }


  if(InputNeurons.empty()||ControlNeurons.empty()||PreControlNeurons.empty()||PostInputNeurons.empty())
   CreateNeurons();
  CurrentPosition->Assign(1,1,0.0);
  vector<NMotionElement *> Motions = MotionControlElement->GetMotion();
  //vector<NNet*> Motions = MotionControlElement->GetMotion();
  CurrentPosition->Assign(MotionControlElement->GetNumControlLoops(),2*MotionControlElement->NumMotionElements,0.0);
  Delta->Assign(MotionControlElement->GetNumControlLoops(),2*MotionControlElement->NumMotionElements,0.0);


  //CurrentPosition Calculation
  for(int i=0;i<MotionControlElement->NumMotionElements;i++)
  {
   NMotionElement *melem=Motions[i];
   //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
   if(!melem)
    continue;
   for(int j=0;j<melem->NumControlLoops;j++)
   {
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> ltzoneL_weak = melem->GetComponentL("AfferentL"+sntoa(j+1)+".LTZone");
    std::shared_ptr<NPulseLTZoneCommon> ltzoneL;
    if(!ltzoneL_weak.expired())
     ltzoneL = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzoneL_weak.lock());
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> ltzoneR_weak = melem->GetComponentL("AfferentR"+sntoa(j+1)+".LTZone");
    std::shared_ptr<NPulseLTZoneCommon> ltzoneR;
    if(!ltzoneR_weak.expired())
     ltzoneR = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzoneR_weak.lock());

    double temp=0;
    temp=ltzoneL->OutputFrequency->As<double>(0);
    (*CurrentPosition)(j,2*i)=temp;

    temp=ltzoneL->OutputFrequency->As<double>(0);
    (*CurrentPosition)(j,2*i+1)= temp;
   }
  }


  //Delta Calculation
  for(int i=0;i<MotionControlElement->NumMotionElements;i++)
  {
   NMotionElement *melem=Motions[i];
   //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
   if(!melem)
    continue;
   for(int j=0;j<melem->NumControlLoops;j++)
   {
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> controlLTZoneL_weak = GetComponentL("ControlNeuronL"+sntoa(i+1)+sntoa(j+1)+".LTZone");
    std::shared_ptr<NPulseLTZoneCommon> controlLTZoneL;
    if(!controlLTZoneL_weak.expired())
     controlLTZoneL = std::dynamic_pointer_cast<NPulseLTZoneCommon>(controlLTZoneL_weak.lock());
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> controlLTZoneR_weak = GetComponentL("ControlNeuronR"+sntoa(i+1)+sntoa(j+1)+".LTZone");
    std::shared_ptr<NPulseLTZoneCommon> controlLTZoneR;
    if(!controlLTZoneR_weak.expired())
     controlLTZoneR = std::dynamic_pointer_cast<NPulseLTZoneCommon>(controlLTZoneR_weak.lock());
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> preControlLTZoneL_weak = GetComponentL("PreControlNeuronL"+sntoa(i+1)+sntoa(j+1)+".LTZone");
    std::shared_ptr<NPulseLTZoneCommon> preControlLTZoneL;
    if(!preControlLTZoneL_weak.expired())
     preControlLTZoneL = std::dynamic_pointer_cast<NPulseLTZoneCommon>(preControlLTZoneL_weak.lock());
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> preControlLTZoneR_weak = GetComponentL("PreControlNeuronR"+sntoa(i+1)+sntoa(j+1)+".LTZone");
    std::shared_ptr<NPulseLTZoneCommon> preControlLTZoneR;
    if(!preControlLTZoneR_weak.expired())
     preControlLTZoneR = std::dynamic_pointer_cast<NPulseLTZoneCommon>(preControlLTZoneR_weak.lock());
    if(!controlLTZoneL || !controlLTZoneR || !preControlLTZoneL || !preControlLTZoneR)
     continue;

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
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> ltzone_weak = InputNeurons[i]->GetComponentL("LTZone");
    std::shared_ptr<NPulseLTZoneCommon> ltzone;
    if(!ltzone_weak.expired())
     ltzone = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzone_weak.lock());
    if(!ltzone)
     continue;
    if(ltzone->OutputFrequency->As<double>(0)>0)
    {
        //NameT check_input_name = InputNeurons[i]->GetName();
     activeInputs.push_back(InputNeurons[i]);
     preControls.push_back(PreControlNeurons[i]);
    }
   }
   for(size_t c=0;c<PreControlNeurons.size();c++)
   {
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> ltzone_weak = ControlNeurons[c]->GetComponentL("LTZone");
    std::shared_ptr<NPulseLTZoneCommon> ltzone;
    if(!ltzone_weak.expired())
     ltzone = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzone_weak.lock());
    if(!ltzone)
     continue;
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
 if(!MotionControlElement)
  return false;

   std::shared_ptr<UContainer> cont;
   std::shared_ptr<UStorage> storage = GetStorage();
   if (!storage) {
     return false;
   }
   bool res(true);
   vector<NMotionElement *> Motions = MotionControlElement->GetMotion();

   if(Motions.empty())//���������
    return false;


   //Creating InputNeurons
   LeftInputNeurons.clear(); //���������
   RightInputNeurons.clear(); //���������
   for(int i=0;i<MotionControlElement->NumMotionElements;i++)
   {
    NMotionElement *melem=Motions[i];
    //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
    if(!melem)
     continue;
    LeftInputNeurons.resize(melem->NumControlLoops);
    RightInputNeurons.resize(melem->NumControlLoops);
    for(int j=0;j<melem->NumControlLoops;j++)
    {
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock
     std::weak_ptr<RDK::UContainer> ltzoneL_weak = melem->GetComponentL("AfferentL"+sntoa(j+1)+".LTZone");
     std::shared_ptr<UItem> ltzoneL;
     if(!ltzoneL_weak.expired())
      ltzoneL = std::dynamic_pointer_cast<UItem>(ltzoneL_weak.lock());
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock
     std::weak_ptr<RDK::UContainer> ltzoneR_weak = melem->GetComponentL("AfferentR"+sntoa(j+1)+".LTZone");
     std::shared_ptr<UItem> ltzoneR;
     if(!ltzoneR_weak.expired())
      ltzoneR = std::dynamic_pointer_cast<UItem>(ltzoneR_weak.lock());
     if(!ltzoneL || !ltzoneR)
      continue;
     // CRITICAL: GetOwner now returns weak_ptr, need to lock
     std::weak_ptr<RDK::UContainer> owner_weak = GetOwner();
     UNet *owner = nullptr;
     if(!owner_weak.expired())
     {
      std::shared_ptr<RDK::UContainer> owner_shared = owner_weak.lock();
      if(owner_shared)
       owner = std::dynamic_pointer_cast<UNet>(owner_shared).get();
     }
     if(!owner)
      continue;
     string ltzoneLName,ltzoneRName;
     ltzoneL->GetLongName(GetThisAsSharedContainer(), ltzoneLName);
     ltzoneR->GetLongName(GetThisAsSharedContainer(), ltzoneRName);
     string inputNeuronLName = "InputNeuronL"+sntoa(i+1)+sntoa(j+1);
     string inputNeuronRName = "InputNeuronR"+sntoa(i+1)+sntoa(j+1);
     string inputLName,inputRName;

     if(CheckComponentL(inputNeuronLName))
     {
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> inputNeuron_weak = GetComponent(inputNeuronLName);
      NNet *inputNeuron = nullptr;
      if(!inputNeuron_weak.expired())
      {
       std::shared_ptr<RDK::UContainer> inputNeuron_shared = inputNeuron_weak.lock();
       if(inputNeuron_shared)
        inputNeuron = std::dynamic_pointer_cast<NNet>(inputNeuron_shared).get();
      }
      if(!inputNeuron)
       continue;
      InputNeurons.push_back(inputNeuron);
      inputNeuron->GetLongName(GetThisAsSharedContainer(), inputLName);
      LeftInputNeurons[j].push_back(inputNeuron);
     }
     else
     {
      cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(InputNeuronType));
      if(!cont)
       return 0;
      cont->SetName(inputNeuronLName);
         res&=(AddComponent(cont) != ForbiddenId);
      InputNeurons.push_back(static_pointer_cast<NNet>(cont).get());
      cont->GetLongName(GetThisAsSharedContainer(), inputLName);
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> inputNeuron_weak_2 = GetComponent(inputNeuronLName);
      if(!inputNeuron_weak_2.expired())
      {
       std::shared_ptr<RDK::UContainer> inputNeuron_shared_2 = inputNeuron_weak_2.lock();
       if(inputNeuron_shared_2)
        LeftInputNeurons[j].push_back(std::dynamic_pointer_cast<NNet>(inputNeuron_shared_2).get());
      }
     }

     if(CheckComponentL(inputNeuronRName))
     {
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> inputNeuron_weak_r = GetComponent(inputNeuronRName);
      NNet *inputNeuron = nullptr;
      if(!inputNeuron_weak_r.expired())
      {
       std::shared_ptr<RDK::UContainer> inputNeuron_shared_r = inputNeuron_weak_r.lock();
       if(inputNeuron_shared_r)
        inputNeuron = std::dynamic_pointer_cast<NNet>(inputNeuron_shared_r).get();
      }
      if(!inputNeuron)
       continue;
      InputNeurons.push_back(inputNeuron);
      inputNeuron->GetLongName(GetThisAsSharedContainer(), inputRName);
      RightInputNeurons[j].push_back(inputNeuron);
     }
     else
     {
      cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(InputNeuronType));
      if(!cont)
       return 0;
      cont->SetName(inputNeuronRName);
         res&=(AddComponent(cont) != ForbiddenId);
      InputNeurons.push_back(static_pointer_cast<NNet>(cont).get());
      cont->GetLongName(GetThisAsSharedContainer(), inputRName);
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> inputNeuron_weak_r2 = GetComponent(inputNeuronRName);
      if(!inputNeuron_weak_r2.expired())
      {
       std::shared_ptr<RDK::UContainer> inputNeuron_shared_r2 = inputNeuron_weak_r2.lock();
       if(inputNeuron_shared_r2)
        RightInputNeurons[j].push_back(std::dynamic_pointer_cast<NNet>(inputNeuron_shared_r2).get());
      }
     }

        //���������� ������ �� ����������� �������� � EngineMotionControl->MotionElement
        //�� ������� �������� (InputNeuron) � NNewPositionControlElement
     res = owner->CreateLink(ltzoneLName,"Output",inputLName+".Soma1.ExcSynapse1","Input");
     res = owner->CreateLink(ltzoneLName,"Output",inputRName+".Soma1.InhSynapse1","Input");
     res = owner->CreateLink(ltzoneRName,"Output",inputLName+".Soma1.InhSynapse1","Input");
     res = owner->CreateLink(ltzoneRName,"Output",inputRName+".Soma1.ExcSynapse1","Input");
    }
   }

   //int num_motions = MotionControlElement->NumMotionElements;

   //Creating ControlNeurons
   LeftControlNeurons.clear(); //���������
   RightControlNeurons.clear(); //���������
   for(int i=0;i<MotionControlElement->NumMotionElements;i++)
   {
    NMotionElement *melem=Motions[i];
    //NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
    if(!melem)
     continue;
    LeftControlNeurons.resize(melem->NumControlLoops);
    RightControlNeurons.resize(melem->NumControlLoops);

    for(int j=0;j<melem->NumControlLoops;j++)
    {
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock
        std::weak_ptr<RDK::UContainer> postAfferentL_weak = melem->GetComponentL("PostAfferentL"+sntoa(j+1));
        std::shared_ptr<UItem> postAfferentL;
        if(!postAfferentL_weak.expired())
         postAfferentL = std::dynamic_pointer_cast<UItem>(postAfferentL_weak.lock());
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock
        std::weak_ptr<RDK::UContainer> postAfferentR_weak = melem->GetComponentL("PostAfferentR"+sntoa(j+1));
        std::shared_ptr<UItem> postAfferentR;
        if(!postAfferentR_weak.expired())
         postAfferentR = std::dynamic_pointer_cast<UItem>(postAfferentR_weak.lock());
        if(!postAfferentL || !postAfferentR)
         continue;
        // CRITICAL: GetOwner now returns weak_ptr, need to lock
        std::weak_ptr<RDK::UContainer> owner_weak = GetOwner();
        UNet *owner = nullptr;
        if(!owner_weak.expired())
        {
         std::shared_ptr<RDK::UContainer> owner_shared = owner_weak.lock();
         if(owner_shared)
          owner = std::dynamic_pointer_cast<UNet>(owner_shared).get();
        }
        if(!owner)
         continue;
     string postAfferentLName,postAfferentRName;
     postAfferentL->GetLongName(GetThisAsSharedContainer(), postAfferentLName);
     postAfferentR->GetLongName(GetThisAsSharedContainer(), postAfferentRName);
     string controlNeuronLName = "ControlNeuronL"+sntoa(i+1)+sntoa(j+1);
     string controlNeuronRName = "ControlNeuronR"+sntoa(i+1)+sntoa(j+1);
     string controlLName,controlRName;

     if(CheckComponentL(controlNeuronLName))
     {
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> controlNeuron_weak = GetComponent(controlNeuronLName);
      NNet *controlNeuron = nullptr;
      if(!controlNeuron_weak.expired())
      {
       std::shared_ptr<RDK::UContainer> controlNeuron_shared = controlNeuron_weak.lock();
       if(controlNeuron_shared)
        controlNeuron = std::dynamic_pointer_cast<NNet>(controlNeuron_shared).get();
      }
      if(!controlNeuron)
       continue;
         ControlNeurons.push_back(controlNeuron);
      controlNeuron->GetLongName(GetThisAsSharedContainer(), controlLName);
      LeftControlNeurons[j].push_back(controlNeuron);
         //Adding synapses for links to PrecontrolNeurons, ���������
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock
         std::weak_ptr<RDK::UContainer> soma_weak = controlNeuron->GetComponentL("Soma1",true);
         std::shared_ptr<NPulseMembrane> soma;
         if(!soma_weak.expired())
          soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak.lock());
         if(!soma)
          return true;
         soma->NumExcitatorySynapses = (MotionControlElement->NumMotionElements-i);
         soma->Build();
     }
     else
     {
      cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
      if(!cont)
       return false;
      cont->SetName(controlNeuronLName);
         res&=(AddComponent(cont) != ForbiddenId);
      ControlNeurons.push_back(static_pointer_cast<NNet>(cont).get());
      cont->GetLongName(GetThisAsSharedContainer(), controlLName);
         // CRITICAL: GetComponent now returns weak_ptr, need to lock
         std::weak_ptr<RDK::UContainer> controlNeuron_weak_2 = GetComponent(controlNeuronLName);
         if(!controlNeuron_weak_2.expired())
         {
          std::shared_ptr<RDK::UContainer> controlNeuron_shared_2 = controlNeuron_weak_2.lock();
          if(controlNeuron_shared_2)
           LeftControlNeurons[j].push_back(std::dynamic_pointer_cast<NNet>(controlNeuron_shared_2).get());
         }
         //Adding synapses for links to PrecontrolNeurons, ���������
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock
         std::weak_ptr<RDK::UContainer> soma_weak = cont->GetComponentL("Soma1",true);
         std::shared_ptr<NPulseMembrane> soma;
         if(!soma_weak.expired())
          soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak.lock());
         if(!soma)
          return true;
         soma->NumExcitatorySynapses = (MotionControlElement->NumMotionElements-i);
         soma->Build();
     }


     if(CheckComponentL(controlNeuronRName))
     {
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> controlNeuron_weak_r = GetComponent(controlNeuronRName);
      NNet *controlNeuron = nullptr;
      if(!controlNeuron_weak_r.expired())
      {
       std::shared_ptr<RDK::UContainer> controlNeuron_shared_r = controlNeuron_weak_r.lock();
       if(controlNeuron_shared_r)
        controlNeuron = std::dynamic_pointer_cast<NNet>(controlNeuron_shared_r).get();
      }
      if(!controlNeuron)
       continue;
      ControlNeurons.push_back(controlNeuron);
      controlNeuron->GetLongName(GetThisAsSharedContainer(), controlRName);
      RightControlNeurons[j].push_back(controlNeuron);
         //Adding synapses for links to PrecontrolNeurons, ���������
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock
         std::weak_ptr<RDK::UContainer> soma_weak_r = controlNeuron->GetComponentL("Soma1",true);
         std::shared_ptr<NPulseMembrane> soma;
         if(!soma_weak_r.expired())
          soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak_r.lock());
         if(!soma)
          return true;
         soma->NumExcitatorySynapses = (MotionControlElement->NumMotionElements-i);
         soma->Build();
     }
     else
     {
      cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
      if(!cont)
       return false;
      cont->SetName(controlNeuronRName);
         res&=(AddComponent(cont) != ForbiddenId);
      ControlNeurons.push_back(static_pointer_cast<NNet>(cont).get());
      cont->GetLongName(GetThisAsSharedContainer(), controlRName);
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> controlNeuron_weak_r2 = GetComponent(controlNeuronRName);
      if(!controlNeuron_weak_r2.expired())
      {
       std::shared_ptr<RDK::UContainer> controlNeuron_shared_r2 = controlNeuron_weak_r2.lock();
       if(controlNeuron_shared_r2)
        RightControlNeurons[j].push_back(std::dynamic_pointer_cast<NNet>(controlNeuron_shared_r2).get());
      }
         //Adding synapses for links to PrecontrolNeurons, ���������
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock
         std::weak_ptr<RDK::UContainer> soma_weak_r2 = cont->GetComponentL("Soma1",true);
         std::shared_ptr<NPulseMembrane> soma;
         if(!soma_weak_r2.expired())
          soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak_r2.lock());
         if(!soma)
          return true;
         soma->NumExcitatorySynapses = (MotionControlElement->NumMotionElements-i);
         soma->Build();
     }

        //���������� ������ �� ControlNeuron-�� � NNewPositionControlElement �
        //��������������� �������� (PostAfferent)� EngineMotionControl->MotionElement
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
   LeftPreControlNeurons.clear(); //���������
   RightPreControlNeurons.clear(); //���������
   for(int i=0;i<MotionControlElement->NumMotionElements;i++)
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

        //Left PreControl Neurons
     if(CheckComponentL(preControlNeuronLName))
     {
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> preControlNeuron_weak_l = GetComponent(preControlNeuronLName);
      NNet *preControlNeuron_l = nullptr;
      if(!preControlNeuron_weak_l.expired())
      {
       std::shared_ptr<RDK::UContainer> preControlNeuron_shared_l = preControlNeuron_weak_l.lock();
       if(preControlNeuron_shared_l)
        preControlNeuron_l = std::dynamic_pointer_cast<NNet>(preControlNeuron_shared_l).get();
      }
      if(!preControlNeuron_l)
       continue;
      PreControlNeurons.push_back(preControlNeuron_l);
      LeftPreControlNeurons[j].push_back(preControlNeuron_l);
         //Adding synapses for links from Control Neurons in NMultiPositionControl, ���������
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock
         std::weak_ptr<RDK::UContainer> soma_weak_pre_l = preControlNeuron_l->GetComponentL("Soma1",true);
         std::shared_ptr<NPulseMembrane> soma;
         if(!soma_weak_pre_l.expired())
          soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak_pre_l.lock());
         if(!soma)
          return true;
         soma->NumExcitatorySynapses = 2;
         soma->Build();
     }
     else
     {
      cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
      if(!cont)
       return false;
      cont->SetName(preControlNeuronLName);
         res&=(AddComponent(cont) != ForbiddenId);
      PreControlNeurons.push_back(static_pointer_cast<NNet>(cont).get());
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> preControlNeuron_weak_l2 = GetComponent(preControlNeuronLName);
      if(!preControlNeuron_weak_l2.expired())
      {
       std::shared_ptr<RDK::UContainer> preControlNeuron_shared_l2 = preControlNeuron_weak_l2.lock();
       if(preControlNeuron_shared_l2)
        LeftPreControlNeurons[j].push_back(std::dynamic_pointer_cast<NNet>(preControlNeuron_shared_l2).get());
      }
         //Adding synapses for links from Control Neurons in NMultiPositionControl, ���������
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock
         std::weak_ptr<RDK::UContainer> preControlNeuron_weak_l3 = GetComponent(preControlNeuronLName);
         if(!preControlNeuron_weak_l3.expired())
         {
          std::shared_ptr<RDK::UContainer> preControlNeuron_shared_l3 = preControlNeuron_weak_l3.lock();
          if(preControlNeuron_shared_l3)
          {
           std::weak_ptr<RDK::UContainer> soma_weak_pre_l2 = preControlNeuron_shared_l3->GetComponentL("Soma1",true);
           std::shared_ptr<NPulseMembrane> soma;
           if(!soma_weak_pre_l2.expired())
            soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak_pre_l2.lock());
           if(!soma)
            return true;
           soma->NumExcitatorySynapses = 2;
           soma->Build();
          }
         }
     }

        //Right PreControl Neurons
     if(CheckComponentL(preControlNeuronRName))
     {
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> preControlNeuron_weak_r = GetComponent(preControlNeuronRName);
      NNet *preControlNeuron_r = nullptr;
      if(!preControlNeuron_weak_r.expired())
      {
       std::shared_ptr<RDK::UContainer> preControlNeuron_shared_r = preControlNeuron_weak_r.lock();
       if(preControlNeuron_shared_r)
        preControlNeuron_r = std::dynamic_pointer_cast<NNet>(preControlNeuron_shared_r).get();
      }
      if(!preControlNeuron_r)
       continue;
      PreControlNeurons.push_back(preControlNeuron_r);
      RightPreControlNeurons[j].push_back(preControlNeuron_r);
         //Adding synapses for links from Control Neurons in NMultiPositionControl, ���������
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock
         std::weak_ptr<RDK::UContainer> soma_weak_pre_r = preControlNeuron_r->GetComponentL("Soma1",true);
         std::shared_ptr<NPulseMembrane> soma;
         if(!soma_weak_pre_r.expired())
          soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak_pre_r.lock());
         if(!soma)
          return true;
         soma->NumExcitatorySynapses = 2;
         soma->Build();
     }
     else
     {
      cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
      if(!cont)
       return false;
      cont->SetName(preControlNeuronRName);
         res&=(AddComponent(cont) != ForbiddenId);
      PreControlNeurons.push_back(static_pointer_cast<NNet>(cont).get());
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> preControlNeuron_weak_r2 = GetComponent(preControlNeuronRName);
      if(!preControlNeuron_weak_r2.expired())
      {
       std::shared_ptr<RDK::UContainer> preControlNeuron_shared_r2 = preControlNeuron_weak_r2.lock();
       if(preControlNeuron_shared_r2)
        RightPreControlNeurons[j].push_back(std::dynamic_pointer_cast<NNet>(preControlNeuron_shared_r2).get());
      }
         //Adding synapses for links from Control Neurons in NMultiPositionControl, ���������
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock
         std::weak_ptr<RDK::UContainer> preControlNeuron_weak_r3 = GetComponent(preControlNeuronRName);
         if(!preControlNeuron_weak_r3.expired())
         {
          std::shared_ptr<RDK::UContainer> preControlNeuron_shared_r3 = preControlNeuron_weak_r3.lock();
          if(preControlNeuron_shared_r3)
          {
           std::weak_ptr<RDK::UContainer> soma_weak_pre_r2 = preControlNeuron_shared_r3->GetComponentL("Soma1",true);
           std::shared_ptr<NPulseMembrane> soma;
           if(!soma_weak_pre_r2.expired())
            soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak_pre_r2.lock());
           if(!soma)
            return true;
           soma->NumExcitatorySynapses = 2;
           soma->Build();
          }
         }
     }
    }
   }


   //Creating PostInputNeurons
   LeftPostInputNeurons.clear();
   RightPostInputNeurons.clear();
   for(int i=0;i<MotionControlElement->NumMotionElements;i++)
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
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> postInputNeuron_weak_l = GetComponent(postInputNeuronLName);
      NNet *postInputNeuron_l = nullptr;
      if(!postInputNeuron_weak_l.expired())
      {
       std::shared_ptr<RDK::UContainer> postInputNeuron_shared_l = postInputNeuron_weak_l.lock();
       if(postInputNeuron_shared_l)
        postInputNeuron_l = std::dynamic_pointer_cast<NNet>(postInputNeuron_shared_l).get();
      }
      if(!postInputNeuron_l)
       continue;
      PostInputNeurons.push_back(postInputNeuron_l);
      LeftPostInputNeurons[j].push_back(postInputNeuron_l);
         //Adding synapses for links from InputNeurons, ���������
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock
         std::weak_ptr<RDK::UContainer> soma_weak_post_l = postInputNeuron_l->GetComponentL("Soma1",true);
         std::shared_ptr<NPulseMembrane> soma;
         if(!soma_weak_post_l.expired())
          soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak_post_l.lock());
         if(!soma)
          return true;
         soma->NumInhibitorySynapses = (MotionControlElement->NumMotionElements-i);
         soma->Build();
     }
     else
     {
      cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
      if(!cont)
       return false;
      cont->SetName(postInputNeuronLName);
         res&=(AddComponent(cont) != ForbiddenId);
      PostInputNeurons.push_back(static_pointer_cast<NNet>(cont).get());
      LeftPostInputNeurons[j].push_back(static_pointer_cast<NNet>(cont).get());
         //Adding synapses for links from InputNeurons, ���������
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock
         std::weak_ptr<RDK::UContainer> soma_weak_post_l2 = cont->GetComponentL("Soma1",true);
         std::shared_ptr<NPulseMembrane> soma;
         if(!soma_weak_post_l2.expired())
          soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak_post_l2.lock());
         if(!soma)
          return true;
         soma->NumInhibitorySynapses = (MotionControlElement->NumMotionElements-i);
         soma->Build();
     }


     if(CheckComponentL(postInputNeuronRName))
     {
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> postInputNeuron_weak_r = GetComponent(postInputNeuronRName);
      NNet *postInputNeuron_r = nullptr;
      if(!postInputNeuron_weak_r.expired())
      {
       std::shared_ptr<RDK::UContainer> postInputNeuron_shared_r = postInputNeuron_weak_r.lock();
       if(postInputNeuron_shared_r)
        postInputNeuron_r = std::dynamic_pointer_cast<NNet>(postInputNeuron_shared_r).get();
      }
      if(!postInputNeuron_r)
       continue;
      PostInputNeurons.push_back(postInputNeuron_r);
      RightPostInputNeurons[j].push_back(postInputNeuron_r);
         //Adding synapses for links from InputNeurons, ���������
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock
         std::weak_ptr<RDK::UContainer> soma_weak_post_r = postInputNeuron_r->GetComponentL("Soma1",true);
         std::shared_ptr<NPulseMembrane> soma;
         if(!soma_weak_post_r.expired())
          soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak_post_r.lock());
         if(!soma)
          return true;
         soma->NumInhibitorySynapses = (MotionControlElement->NumMotionElements-i);
         soma->Build();
     }
     else
     {
      cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(ControlNeuronType));
      if(!cont)
       return false;
      cont->SetName(postInputNeuronRName);
         res&=(AddComponent(cont) != ForbiddenId);
      PostInputNeurons.push_back(static_pointer_cast<NNet>(cont).get());
      RightPostInputNeurons[j].push_back(static_pointer_cast<NNet>(cont).get());

         //Adding synapses for links from InputNeurons, ���������
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock
         std::weak_ptr<RDK::UContainer> soma_weak_post_r3 = cont->GetComponentL("Soma1",true);
         std::shared_ptr<NPulseMembrane> soma;
         if(!soma_weak_post_r3.expired())
          soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak_post_r3.lock());
         if(!soma)
          return true;
         soma->NumInhibitorySynapses = (MotionControlElement->NumMotionElements-i);
         soma->Build();
     }
    }
   }

   //Creating downward links
   for(int i=0;i<MotionControlElement->NumMotionElements;i++)
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
 if(!MotionControlElement)
  return false;

   std::shared_ptr<UContainer> cont;
   std::shared_ptr<UStorage> storage = GetStorage();
   if (!storage) {
     return false;
   }
   bool res(true);
   vector<NMotionElement *> Motions = MotionControlElement->GetMotion();
   //vector<NNet*> Motions = MotionControlElement->GetMotion();
   if(Motions.empty())//���������
       return false;

   for(int i=0;i<MotionControlElement->NumMotionElements;i++)
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
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> generator_weak_l = GetComponent(generatorLName);
      NNet *generator_l = nullptr;
      if(!generator_weak_l.expired())
      {
       std::shared_ptr<RDK::UContainer> generator_shared_l = generator_weak_l.lock();
       if(generator_shared_l)
        generator_l = std::dynamic_pointer_cast<NNet>(generator_shared_l).get();
      }
      if(!generator_l)
       continue;
      Generators.push_back(generator_l);
      LeftGenerators[j].push_back(generator_l);
     }
     else
     {
      cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPGenerator"));
      if(!cont)
       return false;
      cont->SetName(generatorLName);
         res&=(AddComponent(cont) != ForbiddenId);
      Generators.push_back(static_pointer_cast<NNet>(cont).get());
      LeftGenerators[j].push_back(static_pointer_cast<NNet>(cont).get());
     }
     if(CheckComponentL(generatorRName))
     {
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> generator_weak_r = GetComponent(generatorRName);
      NNet *generator_r = nullptr;
      if(!generator_weak_r.expired())
      {
       std::shared_ptr<RDK::UContainer> generator_shared_r = generator_weak_r.lock();
       if(generator_shared_r)
        generator_r = std::dynamic_pointer_cast<NNet>(generator_shared_r).get();
      }
      if(!generator_r)
       continue;
      Generators.push_back(generator_r);
      RightGenerators[j].push_back(generator_r);
     }
     else
     {
      cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NPGenerator"));
      if(!cont)
       return false;
      cont->SetName(generatorRName);
         res&=(AddComponent(cont) != ForbiddenId);
      Generators.push_back(static_pointer_cast<NNet>(cont).get());
      RightGenerators[j].push_back(static_pointer_cast<NNet>(cont).get());
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
      std::shared_ptr<NPulseMembrane> branch;
	  bool hasEmptyMembrane=false;

	  for(size_t k=0;k<neuron->GetNumMembranes();k++)
	  {
	   NPulseMembrane* membr = neuron->GetMembrane(k);
	   if((neuron->GetNumOfConnectedSynToPosCh(membr)==0)&&(membr->GetName()!="LTMembrane"))
	   {
     branch=safe_shared_cast<NPulseMembrane>(neuron->GetMembrane(k));
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

   for(size_t i=0;i<start.size();i++) //���������
	  {
    startName = start[i]->GetName()+".LTZone";

    for (int m = 0; m<syns_max; m++)
    {
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock
     std::weak_ptr<RDK::UContainer> syn_weak = finish[j]->GetComponentL("Soma1.ExcSynapse"+sntoa(m+1),true);
     std::shared_ptr<NPulseSynapse> syn;
     if(!syn_weak.expired())
      syn = std::dynamic_pointer_cast<NPulseSynapse>(syn_weak.lock());
     if(!syn)
       return true;

     finishName = finish[j]->GetName()+"."+branch->GetName()+".ExcSynapse"+sntoa(m+1);

     if(CheckLink(startName,finishName))
       break;//������� � ���������� �������� � start

     if (syn->Input.IsConnected())
       continue; //������� � ���������� �������

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
      std::shared_ptr<NPulseMembrane> branch;
	  bool hasEmptyMembrane=false;
	  for(size_t k=0;k<neuron->GetNumMembranes();k++)
	  {
	   NPulseMembrane* membr = neuron->GetMembrane(k);
	   if((neuron->GetNumOfConnectedSynToPosCh(membr)==0)&&(membr->GetName()!="LTMembrane"))
	   {
     branch=safe_shared_cast<NPulseMembrane>(neuron->GetMembrane(k));
     hasEmptyMembrane=true;
     break;
    }
	  }

	  if(!hasEmptyMembrane)
   {
    //branch=neuron->BranchDendrite("Soma1",false);
    neuron->NumSomaMembraneParts = 1; //���������
    neuron->Build();
   }

  NameT startName;
  NameT finishName;
  int syns_max = branch->NumInhibitorySynapses;

  for(size_t i=0;i<start.size();i++) //���������
  {
   startName = start[i]->GetName()+".LTZone";

   for (int m = 0; m<syns_max; m++)
   {
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> syn_weak = finish[j]->GetComponentL("Soma1.InhSynapse"+sntoa(m+1),true);
    std::shared_ptr<NPulseSynapse> syn;
    if(!syn_weak.expired())
     syn = std::dynamic_pointer_cast<NPulseSynapse>(syn_weak.lock());
    if(!syn)
      return true;

    finishName = finish[j]->GetName()+"."+branch->GetName()+".InhSynapse"+sntoa(m+1);

    if(CheckLink(startName,finishName))
      break;//������� � ���������� �������� � start

    if (syn->Input.IsConnected())
      continue; //������� � ���������� �������

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
 if(!MotionControlElement)
  return false;
    vector<NMotionElement *> Motions = MotionControlElement->GetMotion();
  //vector<NNet*> Motions = MotionControlElement->GetMotion();

  //int check = MotionControlElement->NumMotionElements;
  //int i=0;
  for(int i=0; i<MotionControlElement->NumMotionElements;i++)
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
 if(!MotionControlElement)
  return false;
   vector<NMotionElement *> Motions = MotionControlElement->GetMotion();
   //vector<NNet*> Motions = MotionControlElement->GetMotion();
   vector<NPulseMembrane*> membrToConnectL;
   vector<NPulseMembrane*> membrToConnectR;

   for(int i=0;i<MotionControlElement->NumMotionElements;i++)
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
     // CRITICAL: GetComponent now returns weak_ptr, need to lock
     std::weak_ptr<RDK::UContainer> neuronL_weak = GetComponent(postInputNeuronLName);
     std::shared_ptr<NPulseNeuron> neuronL;
     if(!neuronL_weak.expired())
      neuronL = std::dynamic_pointer_cast<NPulseNeuron>(neuronL_weak.lock());
     std::weak_ptr<RDK::UContainer> neuronR_weak = GetComponent(postInputNeuronRName);
     std::shared_ptr<NPulseNeuron> neuronR;
     if(!neuronR_weak.expired())
      neuronR = std::dynamic_pointer_cast<NPulseNeuron>(neuronR_weak.lock());
     if(!neuronL || !neuronR)
      continue;

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
      membrToConnectL[l]->GetLongName(GetThisAsSharedContainer(),membrLName);
        //string check_startName = start[c]->GetName()+".LTZone";
        //string check_finishName = membrLName+".InhSynapse1";
     CreateLink(start[c]->GetName()+".LTZone","Output",membrLName+".InhSynapse1","Input");
    }

    for(size_t k=0;k<membrToConnectR.size();k++)
    {
     string membrRName;
      membrToConnectR[k]->GetLongName(GetThisAsSharedContainer(),membrRName);
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

} // namespace NMSDK

#endif



