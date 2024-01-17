//---------------------------------------------------------------------------

//#pragma hdrstop

#include "NPCNElement.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)


namespace NMSDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NPCNElement::NPCNElement(void)
:   PMotionControl("MotionControl",this),
	SimControl("SimControl", this)
{
}

NPCNElement::~NPCNElement(void)
{
}
// --------------------------


// ---------------------
// Методы управления параметрами
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
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NPCNElement* NPCNElement::New(void)
{
 return new NPCNElement;
}
// --------------------------


// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NPCNElement::ADefault(void)
{
 NPositionControlElement::ADefault();
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NPCNElement::ABuild(void)
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

// Сброс процесса счета без потери настроек
bool NPCNElement::AReset(void)
{
 RememberState = false;
 return true;
}

// Выполняет расчет этого объекта
bool NPCNElement::ACalculate(void)
{

	 if(MotionControl->GetNumControlLoops() != CurrentPosition->GetRows() ||
		2*MotionControl->NumMotionElements != CurrentPosition->GetCols())
	 {
	  Ready=false;
	  Reset();
	 }


  if(InputNeurons.empty()||ControlNeurons.empty())
   CreateNeurons();
  CurrentPosition->Assign(1,1,0.0);
  vector<NMotionElement*> Motions = MotionControl->GetMotion();
  CurrentPosition->Assign(MotionControl->GetNumControlLoops(),2*MotionControl->NumMotionElements,0.0);
  //CurrentPosition Calculation
  for(int i=0;i<MotionControl->NumMotionElements;i++)
  {
   NMotionElement *melem=dynamic_cast<NMotionElement *>(Motions[i]);
   if(!melem)
	continue;
   for(int j=0;j<melem->NumControlLoops;j++)
   {
    UEPtr<NPulseLTZoneCommon> ltzoneL=dynamic_pointer_cast<NPulseLTZoneCommon>(melem->GetComponentL("AfferentL"+sntoa(j+1)+".LTZone"));
    UEPtr<NPulseLTZoneCommon> ltzoneR=dynamic_pointer_cast<NPulseLTZoneCommon>(melem->GetComponentL("AfferentR"+sntoa(j+1)+".LTZone"));
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
   UEPtr<UContainer> cont;
   UEPtr<UStorage> storage = GetStorage();
   bool res(true);
    vector<NMotionElement*> Motions = MotionControl->GetMotion();
   //Creating InputNeurons
   for(int i=0;i<MotionControl->NumMotionElements;i++)
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
     UEPtr<UItem> ltzoneL=dynamic_pointer_cast<UItem>(melem->GetComponentL("AfferentL"+sntoa(j+1)+".LTZone"));
     UEPtr<UItem> ltzoneR=dynamic_pointer_cast<UItem>(melem->GetComponentL("AfferentR"+sntoa(j+1)+".LTZone"));
	 //
	 UEPtr<NNet> afferentL=dynamic_pointer_cast<NNet>(melem->GetComponentL("AfferentL"+sntoa(j+1)));
	 UEPtr<NNet> afferentR=dynamic_pointer_cast<NNet>(melem->GetComponentL("AfferentR"+sntoa(j+1)));
	 //
	 UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
	 string ltzoneLName,ltzoneRName;
	 ltzoneL->GetLongName(owner, ltzoneLName);
	 LeftAfferentNeurons[j].push_back(static_pointer_cast<NNet>(afferentL));
	 ltzoneR->GetLongName(owner, ltzoneRName);
	 RightAfferentNeurons[j].push_back(static_pointer_cast<NNet>(afferentR));
	 string inputNeuronLName = "InputNeuronL"+sntoa(i+1)+sntoa(j+1);
	 string inputNeuronRName = "InputNeuronR"+sntoa(i+1)+sntoa(j+1);
	 string inputLName,inputRName;

	 if(CheckComponentL(inputNeuronLName))
	 {
	  NNet *inputNeuron = static_pointer_cast<NNet>(GetComponent(inputNeuronLName));
	  InputNeurons.push_back(inputNeuron);
	  inputNeuron->GetLongName(owner, inputLName);
	  LeftInputNeurons[j].push_back(inputNeuron);
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(InputNeuronType));
	  if(!cont)
	   return false;
	  cont->SetName(inputNeuronLName);
      res&=(AddComponent(cont) != ForbiddenId);
	  InputNeurons.push_back(static_pointer_cast<NNet>(cont));
	  cont->GetLongName(owner, inputLName);
	  LeftInputNeurons[j].push_back(static_pointer_cast<NNet>(cont));
	 }

	 if(CheckComponentL(inputNeuronRName))
	 {
	  NNet *inputNeuron = static_pointer_cast<NNet>(GetComponent(inputNeuronRName));
	  InputNeurons.push_back(static_pointer_cast<NNet>(GetComponent(inputNeuronRName)));
	  inputNeuron->GetLongName(owner, inputRName);
	  RightInputNeurons[j].push_back(inputNeuron);
	 }
	 else
	 {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(InputNeuronType));
	  if(!cont)
	   return false;
	  cont->SetName(inputNeuronRName);
      res&=(AddComponent(cont) != ForbiddenId);
	  InputNeurons.push_back(static_pointer_cast<NNet>(cont));
	  cont->GetLongName(owner, inputRName);
	  RightInputNeurons[j].push_back(static_pointer_cast<NNet>(cont));
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
   for(int i=0;i<MotionControl->NumMotionElements;i++)
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
     UEPtr<UItem> postAfferentL=dynamic_pointer_cast<UItem>(melem->GetComponentL("PostAfferentL"+sntoa(j+1)));
     UEPtr<UItem> postAfferentR=dynamic_pointer_cast<UItem>(melem->GetComponentL("PostAfferentR"+sntoa(j+1)));
	 //
	 UEPtr<NNet> postafferentL=dynamic_pointer_cast<NNet>(melem->GetComponentL("PostAfferentL"+sntoa(j+1)));
	 UEPtr<NNet> postafferentR=dynamic_pointer_cast<NNet>(melem->GetComponentL("PostAfferentR"+sntoa(j+1)));
	 //
	 UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
	 string postAfferentLName,postAfferentRName;

	 postAfferentL->GetLongName(owner, postAfferentLName);
	 LeftPostAfferentNeurons[j].push_back(static_pointer_cast<NNet>(postafferentL));
	 postAfferentR->GetLongName(owner, postAfferentRName);
	 RightPostAfferentNeurons[j].push_back(static_pointer_cast<NNet>(postafferentR));

	 string controlNeuronLName = "ControlNeuronL"+sntoa(i+1)+sntoa(j+1);
	 string controlNeuronRName = "ControlNeuronR"+sntoa(i+1)+sntoa(j+1);
	 string controlLName,controlRName;

	 if(CheckComponentL(controlNeuronLName))
	 {
	  NNet *controlNeuron = static_pointer_cast<NNet>(GetComponent(controlNeuronLName));
	  ControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(controlNeuronLName)));
	  controlNeuron->GetLongName(owner, controlLName);
	  LeftControlNeurons[j].push_back(controlNeuron);
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
	  LeftControlNeurons[j].push_back(static_pointer_cast<NNet>(cont));
	 }
	 if(CheckComponentL(controlNeuronRName))
	 {
	  NNet *controlNeuron = static_pointer_cast<NNet>(GetComponent(controlNeuronRName));
	  ControlNeurons.push_back(static_pointer_cast<NNet>(GetComponent(controlNeuronRName)));
	  controlNeuron->GetLongName(owner, controlRName);
	  RightControlNeurons[j].push_back(controlNeuron);
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
	  RightControlNeurons[j].push_back(static_pointer_cast<NNet>(cont));
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
   for(int i=0;i<MotionControl->NumMotionElements;i++)
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
   UEPtr<UContainer> cont;
   UEPtr<UStorage> storage = GetStorage();
   bool res(true);
   vector<NMotionElement*> Motions = MotionControl->GetMotion();

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

bool NPCNElement::LinkNeurons(vector <NNet*> start, vector <NNet*> finish)
{
	//vector <NNet*>::iterator I,J;

	UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
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
	   branch=neuron->BranchDendrite("Soma1",false);
	  NameT finishName;
	  branch->GetLongName(owner, finishName);
	  finishName += ".ExcChannel";
	  for(size_t i=0;i<start.size();i++)
	  {
	   NameT startName;
	   start[i]->GetLongName(owner, startName);
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

	UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
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
	   branch=neuron->BranchDendrite("Soma1",false);
	  NameT finishName;
	  branch->GetLongName(owner, finishName);
	  finishName += ".ExcChannel";
	  for(size_t i=0;i<start.size();i++)
	  {
	   NameT startName;
	   start[i]->GetLongName(owner, startName);
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
 UNet *owner=dynamic_pointer_cast<UNet>(GetOwner());
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
  vector<NMotionElement*> Motions = MotionControl->GetMotion();

  for(int i=0;i<MotionControl->NumMotionElements;i++)
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
   vector<NMotionElement*> Motions = MotionControl->GetMotion();
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
	 membrToConnectL[l]->GetLongName(this,membrLName);
     CreateLink(start[c]->GetName()+".LTZone","Output",membrLName+".InhChannel", "SynapticInputs");
	}
	for(size_t k=0;k<membrToConnectR.size();k++)
	{
	 string membrRName;
	 membrToConnectR[k]->GetLongName(this,membrRName);
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
