//---------------------------------------------------------------------------

//#pragma hdrstop

#include "NPCNElement.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)


namespace NMSDK {

// ������
// --------------------------
// ������������ � �����������
// --------------------------
NPCNElement::NPCNElement(void)
:   MotionControl("MotionControl",this),
	SimControl("SimControl", this)
{
}

NPCNElement::~NPCNElement(void)
{
}
// --------------------------


// ---------------------
// ������ ���������� �����������
// ---------------------
bool NPCNElement::SetInputNeuronType(const string &value)
{
 Ready=false;
 return true;
}
bool NPCNElement::SetControlNeuronType(const string &value)
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
NPCNElement* NPCNElement::New(void)
{
 return new NPCNElement;
}
// --------------------------


// --------------------------
// ������� ������ ���������� ������
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool NPCNElement::ADefault(void)
{
 NPositionControlElement::ADefault();
 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NPCNElement::ABuild(void)
{
 MotionControlElement = safe_shared_cast<NEngineMotionControl>(dynamic_cast<NEngineMotionControl*>(MotionControl.GetItem()));
 CurrentPosition->Assign(2,1,0.0);
 if(MotionControlElement)
  TargetPosition->Assign(MotionControlElement->GetNumControlLoops(),2*MotionControlElement->NumMotionElements,0.0);
 InputNeurons.clear();
 ControlNeurons.clear();
 Generators.clear();
 LeftGenerators.clear();
 RightGenerators.clear();
 Delta->Assign(2,1,0.0);

 return true;
}

// ����� �������� ����� ��� ������ ��������
bool NPCNElement::AReset(void)
{
 RememberState = false;
 return true;
}

// ��������� ������ ����� �������
bool NPCNElement::ACalculate(void)
{
 if(!MotionControlElement)
  return true;

 if(MotionControlElement->GetNumControlLoops() != CurrentPosition->GetRows() ||
  2*MotionControlElement->NumMotionElements != CurrentPosition->GetCols())
	 {
	  Ready=false;
	  Reset();
	 }


  if(InputNeurons.empty()||ControlNeurons.empty())
   CreateNeurons();
  CurrentPosition->Assign(1,1,0.0);
  vector<NMotionElement*> Motions = MotionControlElement->GetMotion();
  CurrentPosition->Assign(MotionControlElement->GetNumControlLoops(),2*MotionControlElement->NumMotionElements,0.0);
  //CurrentPosition Calculation
  for(int i=0;i<MotionControlElement->NumMotionElements;i++)
  {
   NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
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
    if(!ltzoneL || !ltzoneR)
     continue;
    double temp=0;
    temp=ltzoneL->OutputFrequency->Double[0];
    (*CurrentPosition)(j,2*i)=temp;
     temp=ltzoneR->OutputFrequency->Double[0];
    (*CurrentPosition)(j,2*i+1)= temp;
   }
  }

  if(ExternalControl)
  {
   //UnlinkNeurons(InputNeurons, ControlNeurons);
   LinkGenerators(Generators, ControlNeurons, true, SimControl);
  }
  else
  {
   LinkGenerators(Generators, ControlNeurons, false, SimControl);
  }

 return true;
}
// --------------------------
bool NPCNElement::CreateNeurons()
{
 if(!MotionControlElement)
  return false;
   std::shared_ptr<UContainer> cont;
   std::shared_ptr<UStorage> storage = GetStorage();
   if (!storage) {
     return false;
   }
   bool res(true);
    vector<NMotionElement*> Motions = MotionControlElement->GetMotion();
   //Creating InputNeurons
   for(int i=0;i<MotionControlElement->NumMotionElements;i++)
   {
    NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
    if(!melem)
     continue;
    LeftInputNeurons.resize(melem->NumControlLoops);
    RightInputNeurons.resize(melem->NumControlLoops);
    LeftAfferentNeurons.resize(melem->NumControlLoops);
    RightAfferentNeurons.resize(melem->NumControlLoops);
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
     //
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock
     std::weak_ptr<RDK::UContainer> afferentL_weak = melem->GetComponentL("AfferentL"+sntoa(j+1));
     std::shared_ptr<NNet> afferentL;
     if(!afferentL_weak.expired())
      afferentL = std::dynamic_pointer_cast<NNet>(afferentL_weak.lock());
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock
     std::weak_ptr<RDK::UContainer> afferentR_weak = melem->GetComponentL("AfferentR"+sntoa(j+1));
     std::shared_ptr<NNet> afferentR;
     if(!afferentR_weak.expired())
      afferentR = std::dynamic_pointer_cast<NNet>(afferentR_weak.lock());
     if(!afferentL || !afferentR)
      continue;
     //
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
     LeftAfferentNeurons[j].push_back(static_pointer_cast<NNet>(afferentL).get());
     ltzoneR->GetLongName(GetThisAsSharedContainer(), ltzoneRName);
     RightAfferentNeurons[j].push_back(static_pointer_cast<NNet>(afferentR).get());
     string inputNeuronLName = "InputNeuronL"+sntoa(i+1)+sntoa(j+1);
     string inputNeuronRName = "InputNeuronR"+sntoa(i+1)+sntoa(j+1);
     string inputLName,inputRName;

     if(CheckComponentL(inputNeuronLName))
     {
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> inputNeuron_weak_l = GetComponent(inputNeuronLName);
      NNet *inputNeuron = nullptr;
      if(!inputNeuron_weak_l.expired())
      {
       std::shared_ptr<RDK::UContainer> inputNeuron_shared_l = inputNeuron_weak_l.lock();
       if(inputNeuron_shared_l)
        inputNeuron = std::dynamic_pointer_cast<NNet>(inputNeuron_shared_l).get();
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
       return false;
      cont->SetName(inputNeuronLName);
         res&=(AddComponent(cont) != ForbiddenId);
      InputNeurons.push_back(static_pointer_cast<NNet>(cont).get());
      cont->GetLongName(GetThisAsSharedContainer(), inputLName);
      LeftInputNeurons[j].push_back(static_pointer_cast<NNet>(cont).get());
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
       return false;
      cont->SetName(inputNeuronRName);
         res&=(AddComponent(cont) != ForbiddenId);
      InputNeurons.push_back(static_pointer_cast<NNet>(cont).get());
      cont->GetLongName(GetThisAsSharedContainer(), inputRName);
      RightInputNeurons[j].push_back(static_pointer_cast<NNet>(cont).get());
     }

        owner->CreateLink(ltzoneLName,"Output",inputLName+".Soma1.ExcChannel", "SynapticInputs");
        owner->CreateLink(ltzoneRName,"Output",inputRName+".Soma1.ExcChannel", "SynapticInputs");
     if(i==0)
     {
         owner->CreateLink(ltzoneLName,"Output",inputRName+".Soma1.InhChannel", "SynapticInputs");
         owner->CreateLink(ltzoneRName,"Output",inputLName+".Soma1.InhChannel", "SynapticInputs");
     }
    }
   }


   //Creating ControlNeurons
   for(int i=0;i<MotionControlElement->NumMotionElements;i++)
   {
    NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
    if(!melem)
     continue;
    LeftControlNeurons.resize(melem->NumControlLoops);
    RightControlNeurons.resize(melem->NumControlLoops);
    LeftPostAfferentNeurons.resize(melem->NumControlLoops);
    RightPostAfferentNeurons.resize(melem->NumControlLoops);
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
     //
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock
     std::weak_ptr<RDK::UContainer> postafferentL_weak = melem->GetComponentL("PostAfferentL"+sntoa(j+1));
     std::shared_ptr<NNet> postafferentL;
     if(!postafferentL_weak.expired())
      postafferentL = std::dynamic_pointer_cast<NNet>(postafferentL_weak.lock());
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock
     std::weak_ptr<RDK::UContainer> postafferentR_weak = melem->GetComponentL("PostAfferentR"+sntoa(j+1));
     std::shared_ptr<NNet> postafferentR;
     if(!postafferentR_weak.expired())
      postafferentR = std::dynamic_pointer_cast<NNet>(postafferentR_weak.lock());
     if(!postafferentL || !postafferentR)
      continue;
     //
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
     LeftPostAfferentNeurons[j].push_back(static_pointer_cast<NNet>(postafferentL).get());
     postAfferentR->GetLongName(GetThisAsSharedContainer(), postAfferentRName);
     RightPostAfferentNeurons[j].push_back(static_pointer_cast<NNet>(postafferentR).get());

     string controlNeuronLName = "ControlNeuronL"+sntoa(i+1)+sntoa(j+1);
     string controlNeuronRName = "ControlNeuronR"+sntoa(i+1)+sntoa(j+1);
     string controlLName,controlRName;

     if(CheckComponentL(controlNeuronLName))
     {
      // CRITICAL: GetComponent now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> controlNeuron_weak_l = GetComponent(controlNeuronLName);
      NNet *controlNeuron = nullptr;
      if(!controlNeuron_weak_l.expired())
      {
       std::shared_ptr<RDK::UContainer> controlNeuron_shared_l = controlNeuron_weak_l.lock();
       if(controlNeuron_shared_l)
        controlNeuron = std::dynamic_pointer_cast<NNet>(controlNeuron_shared_l).get();
      }
      if(!controlNeuron)
       continue;
      ControlNeurons.push_back(controlNeuron);
      controlNeuron->GetLongName(GetThisAsSharedContainer(), controlLName);
      LeftControlNeurons[j].push_back(controlNeuron);
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
      LeftControlNeurons[j].push_back(static_pointer_cast<NNet>(cont).get());
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
      RightControlNeurons[j].push_back(static_pointer_cast<NNet>(cont).get());
     }
        owner->CreateLink(controlLName+".LTZone","Output",postAfferentLName+".Soma1.ExcChannel", "SynapticInputs");
        owner->CreateLink(controlLName+".LTZone","Output",postAfferentRName+".Soma1.InhChannel", "SynapticInputs");
        owner->CreateLink(controlRName+".LTZone","Output",postAfferentLName+".Soma1.InhChannel", "SynapticInputs");
        owner->CreateLink(controlRName+".LTZone","Output",postAfferentRName+".Soma1.ExcChannel", "SynapticInputs");
     /*
     owner->CreateLink(controlLName+".LTZone",0,postAfferentRName+".Soma1.ExcChannel");
     owner->CreateLink(controlLName+".LTZone",0,postAfferentLName+".Soma1.InhChannel");
     owner->CreateLink(controlRName+".LTZone",0,postAfferentRName+".Soma1.InhChannel");
     owner->CreateLink(controlRName+".LTZone",0,postAfferentLName+".Soma1.ExcChannel");     */
    }
   }
   //Creating downward links
   for(int i=0;i<MotionControlElement->NumMotionElements;i++)
   {
    NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
    if(!melem)
     continue;
    for(int j=0;j<melem->NumControlLoops;j++)
    {
     vector <NNet*> start, finish;

     start.push_back(LeftAfferentNeurons[j][i]);
     finish.push_back(LeftInputNeurons[j][i]);
     LinkNeurons(start,finish);
     start.clear();
     finish.clear();

     start.push_back(RightAfferentNeurons[j][i]);
     finish.push_back(RightInputNeurons[j][i]);
     LinkNeurons(start,finish);
     start.clear();
     finish.clear();

     start.push_back(LeftAfferentNeurons[j][i]);
     for(int k=0;k<i;k++)
     {
      finish.push_back(LeftInputNeurons[j][k]);
     }
     LinkNeuronsNeg(start,finish);
     start.clear();
     finish.clear();

     start.push_back(RightAfferentNeurons[j][i]);
     for(int k=0;k<i;k++)
     {
      finish.push_back(RightInputNeurons[j][k]);
     }
     LinkNeuronsNeg(start,finish);
     start.clear();
     finish.clear();

     start.push_back(RightAfferentNeurons[j][i]);
     for(int k=0;k<i;k++)
     {
      finish.push_back(RightInputNeurons[j][k]);
     }
     LinkNeuronsNeg(start,finish);
     start.clear();
     finish.clear();

     start.push_back(LeftControlNeurons[j][i]);
     for(int k=0;k<=i;k++)
     {
      finish.push_back(LeftPostAfferentNeurons[j][k]);
     }
     LinkNeurons(start,finish);
     start.clear();
     finish.clear();

     start.push_back(RightControlNeurons[j][i]);
     for(int k=0;k<=i;k++)
     {
      finish.push_back(RightPostAfferentNeurons[j][k]);
     }
     LinkNeurons(start,finish);
    }
   }

   CreateExternalControlElements();
   return res;
}

bool NPCNElement::CreateExternalControlElements(void)
{
 if(!MotionControlElement)
  return false;
 std::shared_ptr<UContainer> cont;
 std::shared_ptr<UStorage> storage(GetStorage().get());
 bool res(true);
 vector<NMotionElement*> Motions = MotionControlElement->GetMotion();

 for(int i=0;i<MotionControlElement->NumMotionElements;i++)
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

bool NPCNElement::LinkNeurons(vector <NNet*> start, vector <NNet*> finish)
{
	//vector <NNet*>::iterator I,J;

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
	 return false;
	for(size_t j=0;j<finish.size();j++)
	{
	  NPulseNeuron* neuron=dynamic_cast<NPulseNeuron*>(finish[j]);
	  std::shared_ptr<NPulseMembraneCommon> branch;
	  bool hasEmptyMembrane=false;
	  for(size_t k=0;k<neuron->GetNumMembranes();k++)
	  {
	   NPulseMembrane* membr = neuron->GetMembrane(k);
	   if((neuron->GetNumOfConnectedSynToPosCh(membr)==0)&&(membr->GetName()!="LTMembrane"))
	   {
     branch=safe_shared_cast<NPulseMembraneCommon>(neuron->GetMembrane(k));
     hasEmptyMembrane=true;
     break;
	   }
	  }
	  if(!hasEmptyMembrane)
	   branch=safe_shared_cast<NPulseMembraneCommon>(neuron->BranchDendrite("Soma1",false));
	  NameT finishName;
	  branch->GetLongName(GetThisAsSharedContainer(), finishName);
	  finishName += ".ExcChannel";
	  for(size_t i=0;i<start.size();i++)
	  {
	   NameT startName;
	   start[i]->GetLongName(GetThisAsSharedContainer(), startName);
	   startName += ".LTZone";
	   if(!owner->CheckLink(startName,finishName))
        owner->CreateLink(startName, "Output", finishName, "SynapticInputs");
//	   ExternalControl=false;
	  }
	}

	return true;
}

bool NPCNElement::LinkNeuronsNeg(vector <NNet*> start, vector <NNet*> finish)
{
	//vector <NNet*>::iterator I,J;

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
	 return false;
	for(size_t j=0;j<finish.size();j++)
	{
	  NPulseNeuron* neuron=dynamic_cast<NPulseNeuron*>(finish[j]);
	  std::shared_ptr<NPulseMembraneCommon> branch;
	  bool hasEmptyMembrane=false;
	  for(size_t k=0;k<neuron->GetNumMembranes();k++)
	  {
	   NPulseMembrane* membr = neuron->GetMembrane(k);
	   if((neuron->GetNumOfConnectedSynToPosCh(membr)==0)&&(membr->GetName()!="LTMembrane"))
	   {
		 branch=safe_shared_cast<NPulseMembraneCommon>(neuron->GetMembrane(k));
		 hasEmptyMembrane=true;
		 break;
	   }
	  }
	  if(!hasEmptyMembrane)
	   branch=safe_shared_cast<NPulseMembraneCommon>(neuron->BranchDendrite("Soma1",false));
	  NameT finishName;
	  branch->GetLongName(GetThisAsSharedContainer(), finishName);
	  finishName += ".ExcChannel";
	  for(size_t i=0;i<start.size();i++)
	  {
	   NameT startName;
	   start[i]->GetLongName(GetThisAsSharedContainer(), startName);
	   startName += ".LTZone";
	   if(!owner->CheckLink(startName,finishName))
        owner->CreateLink(startName, "Output", finishName, "SynapticInputs");
//	   ExternalControl=false;
	  }
	}

	return true;
}

bool NPCNElement::UnlinkNeurons(vector <NNet*> start, vector <NNet*> finish)
{
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
  return false;
 for(size_t i=0;i<start.size();i++)
 {
  NameT startName = start[i]->GetName()+".LTZone";
  NPulseNeuron* neuron=dynamic_cast<NPulseNeuron*>(finish[i]);
  for(size_t j=0;j<finish.size();j++)
  {
   for(size_t k=0;k<neuron->Membranes.size();k++)
   {
    NameT finishName = finish[j]->GetName()+"."+neuron->Membranes[k]->GetName()+".ExcChannel";
    if(owner->CheckLink(startName,finishName))
     owner->BreakLink(startName,finishName);
   }
  }
 }
 return true;
}

bool NPCNElement::LinkGenerators(const bool &value)
{
 return NPositionControlElement::LinkGenerators(value);
}

bool NPCNElement::LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link, bool is_sim)
{
 if(!MotionControlElement)
  return false;
  vector<NMotionElement*> Motions = MotionControlElement->GetMotion();

  for(int i=0;i<MotionControlElement->NumMotionElements;i++)
  {
   NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
   if(!melem)
    continue;
   for(int j=0;j<melem->NumControlLoops;j++)
   {
    string generatorLName = "NPGeneratorL"+sntoa(i+1)+sntoa(j+1);
    string generatorRName = "NPGeneratorR"+sntoa(i+1)+sntoa(j+1);
    string controlNeuronLName = "ControlNeuronL"+sntoa(i+1)+sntoa(j+1)+".Soma1.ExcChannel";
    string controlNeuronRName = "ControlNeuronR"+sntoa(i+1)+sntoa(j+1)+".Soma1.ExcChannel";

    if(link)
    {
     if(!CheckLink(generatorLName,controlNeuronLName))
      CreateLink(generatorLName, "Output", controlNeuronLName, "SynapticInputs");
     if(!CheckLink(generatorRName,controlNeuronRName))
      CreateLink(generatorRName, "Output", controlNeuronRName, "SynapticInputs");
    }
    else
    {
     if(CheckLink(generatorLName,controlNeuronLName))
     BreakLink(generatorLName,controlNeuronLName);
     if(CheckLink(generatorRName,controlNeuronRName))
     BreakLink(generatorRName,controlNeuronRName);
    }
   }
  }
 return true;
}
bool NPCNElement::LinkNegative(vector <NNet*> start, vector <NNet*> finish)
{
 if(!MotionControlElement)
  return false;
   vector<NMotionElement*> Motions = MotionControlElement->GetMotion();
   vector<NPulseMembrane*> membrToConnectL;
   vector<NPulseMembrane*> membrToConnectR;
   for(int i=0;i<MotionControlElement->NumMotionElements;i++)
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
      if(CheckLink(inputNeuronLName,postInputNeuronLName+"."+membr->GetName()+".ExcChannel"))
     membrToConnectL.push_back(membr);
     }
     for(size_t n=0;n<neuronR->GetNumMembranes();n++)
     {
      NPulseMembrane* membr = neuronR->GetMembrane(n);
      if(CheckLink(inputNeuronRName,postInputNeuronRName+"."+membr->GetName()+".ExcChannel"))
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
        CreateLink(start[c]->GetName()+".LTZone","Output",membrLName+".InhChannel", "SynapticInputs");
    }
    for(size_t k=0;k<membrToConnectR.size();k++)
    {
     string membrRName;
     membrToConnectR[k]->GetLongName(GetThisAsSharedContainer(),membrRName);
        CreateLink(start[c]->GetName()+".LTZone","Output",membrRName+".InhChannel", "SynapticInputs");
    }
   }
   return true;
}

vector<NNet*> NPCNElement::GetInputNeurons(void)
{
 return InputNeurons;
}

vector<NNet*> NPCNElement::GetControlNeurons(void)
{
 return ControlNeurons;
}

}
