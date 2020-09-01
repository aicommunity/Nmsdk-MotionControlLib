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

#include "NMotionElement.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"
//---------------------------------------------------------------------------
namespace NMSDK {

// --------------------------
// Конструкторы и деструкторы
// --------------------------
NMotionElement::NMotionElement(void)
:
NumControlLoops("NumControlLoops",this, &NMotionElement::SetNumControlLoops),
EnableControlLoopFlags("EnableControlLoopFlags",this, &NMotionElement::SetEnableControlLoopFlags),
LinkModes("LinkModes",this,&NMotionElement::SetLinkModes),
InterneuronPresentMode("InterneuronPresentMode",this, &NMotionElement::SetInterneuronPresentMode),
RenshowMode("RenshowMode",this, &NMotionElement::SetRenshowMode),
RecurrentInhibitionMode("RecurrentInhibitionMode",this, &NMotionElement::SetRecurrentInhibitionMode),
RecurrentInhibitionBranchMode("RecurrentInhibitionBranchMode",this, &NMotionElement::SetRecurrentInhibitionBranchMode),
MotoneuronBranchMode("MotoneuronBranchMode",this, &NMotionElement::SetMotoneuronBranchMode),
ExternalControlMode("ExternalControlMode",this, &NMotionElement::SetExternalControlMode),

NeuroObjectName("NeuroObjectName",this, &NMotionElement::SetNeuroObjectName),
AfferentObjectName("AfferentObjectName",this, &NMotionElement::SetAfferentObjectName),
Afferents("Afferents",this),
ExternalControlGenerators("ExternalControlGenerators",this),
Motoneurons("Motoneurons",this)
{
 isNumControlLoopsInitialized=false;
}

NMotionElement::~NMotionElement(void)
{

}
// --------------------------

// --------------------------
// Методы управления параметрами
// --------------------------
bool NMotionElement::SetNumControlLoops(const int &value)
{
 if(value <=0)
  return false;
 Ready=false;
 LinkModes->resize(value, 0);
 return true;
}

bool NMotionElement::SetEnableControlLoopFlags(const std::vector<int> &value)
{
 return true;
}

bool NMotionElement::SetRenshowMode(const int &value)
{
 Ready=false;
 return true;
}

bool NMotionElement::SetLinkModes(const std::vector<int> &value)
{
 Ready=false;
 return true;
}

bool NMotionElement::SetInterneuronPresentMode(const int &value)
{
 if(value==0)//Если интернейроны отсутствуют - сброс связи в режим 0 (без интернейронов)
 {
  LinkModes->clear();
  LinkModes->resize(NumControlLoops, 0);
 }
 Ready=false;
 return true;
}

bool NMotionElement::SetRecurrentInhibitionMode(const int &value)
{
 if(value <0)
  return false;
 Ready=false;
 return true;
}

bool NMotionElement::SetRecurrentInhibitionBranchMode(const int &value)
{
 if(value <0)
  return false;
 Ready=false;
 return true;
}

bool NMotionElement::SetMotoneuronBranchMode(const int &value)
{
 if(value <0)
  return false;
 Ready=false;
 return true;
}

bool NMotionElement::SetExternalControlMode(const int &value)
{
 if(value <0)
  return false;
 Ready=false;
 return true;
}

bool NMotionElement::SetNeuroObjectName(const string &value)
{
 Ready=false;
 return true;
}

bool NMotionElement::SetAfferentObjectName(const string &value)
{
 Ready=false;
 return true;
}
// --------------------------



// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NMotionElement* NMotionElement::New(void)
{
 return new NMotionElement;
}
// --------------------------

// --------------------------
// Proctected computation methods
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NMotionElement::ADefault(void)
{
  if(!isNumControlLoopsInitialized)
   NumControlLoops = 1;
  InterneuronPresentMode = 1;
  LinkModes->assign(NumControlLoops,1);
  RenshowMode = 0;
  NeuroObjectName = "NNewSynSPNeuron";
  AfferentObjectName = "NSimpleAfferentNeuron";

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NMotionElement::ABuild(void)
{
 BackupExternalLinks();
 CreateStructure();
 CreateInternalLinks();
 RestoreExternalLinks();
 return true;
}

// Reset computation
bool NMotionElement::AReset(void)
{
 return true;
}

// Execute math. computations of current object on current step
bool NMotionElement::ACalculate(void)
{
 return true;
}

// Создает структуру в соответствии с текущими значениями параметров
// Если структура существует, то пытается модифицировать ее с минимальными изменениями
void NMotionElement::CreateStructure(void)
{
  DelAllComponents();
  if(!GetStorage())
   return;
  if(NeuroObjectName->empty()||AfferentObjectName->empty())
   return;
  CreateMotoneurons();
  CreateAfferents();
  CreateInterneurons();
}

// Создает внутренние связи в соответствии с текущими значениями параметров
void NMotionElement::CreateInternalLinks(void)
{
   for (int i=0; i<NumControlLoops; i++)
   {
	 LinkMotoneurons("AfferentL"+sntoa(i+1),"AfferentR"+sntoa(i+1),(*LinkModes)[i]);
   }
   if(RenshowMode)
	 LinkRenshow();
}

// Сохраняет и восстанавливает внешние связи
void NMotionElement::BackupExternalLinks(void)
{

}

void NMotionElement::RestoreExternalLinks(void)
{

}
// --------------------------

// Формирует СУ двигательной единицей
UEPtr<UNet> CreateSimplestMotionElement(UStorage *storage, const string &netclassname, int mode)
{
 UEPtr<UContainer> cont;
 bool res;

 UEPtr<UNet> net=RDK::dynamic_pointer_cast<UNet>(storage->TakeObject(netclassname));
 if(!net)
  return 0;

 // Клетка реншоу 1
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynRenshowCell"));
 if(!cont)
  return 0;
 cont->SetName("Renshow1");
 res=net->AddComponent(cont);

 // Клетка реншоу 2
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynRenshowCell"));
 if(!cont)
  return 0;
 cont->SetName("Renshow2");
 res=net->AddComponent(cont);

 // Мотонейрон 1
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynMotoneuron"));
 if(!cont)
  return 0;
 cont->SetName("Motoneuron1");
 res=net->AddComponent(cont);

 // Мотонейрон 2
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynMotoneuron"));
 if(!cont)
  return 0;
 cont->SetName("Motoneuron2");
 res=net->AddComponent(cont);

 // Постафферентные нейроны
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent11");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent12");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent13");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent14");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent21");
 res=net->AddComponent(cont);


 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent22");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent23");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent24");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSAfferentNeuron"));
 if(!cont)
  return 0;
 cont->SetName("Afferent_Ia1");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSAfferentNeuron"));
 if(!cont)
  return 0;
 cont->SetName("Afferent_Ia2");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSAfferentNeuron"));
 if(!cont)
  return 0;
 cont->SetName("Afferent_Ib1");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSAfferentNeuron"));
 if(!cont)
  return 0;
 cont->SetName("Afferent_Ib2");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSAfferentNeuron"));
 if(!cont)
  return 0;
 cont->SetName("Afferent_II1");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSAfferentNeuron"));
 if(!cont)
  return 0;
 cont->SetName("Afferent_II2");
 res=net->AddComponent(cont);

 // Установка связей
 ULongId item,conn;

 res=net->CreateLink("Afferent_II1.LTZone",0,
				 "Motoneuron1.Soma1.ExcChannel");

 res=net->CreateLink("Afferent_II1.LTZone",0,
				 "Motoneuron2.Soma1.InhChannel");

 res=net->CreateLink("Afferent_II1.LTZone",0,
				 "Motoneuron1.Soma1.InhChannel");

 res=net->CreateLink("Afferent_II1.LTZone",0,
				 "Motoneuron2.Soma1.ExcChannel");

 res=net->CreateLink("PostAfferent14.LTZone",0,
				 "Motoneuron1.Soma1.ExcChannel");
 res=net->CreateLink("PostAfferent14.LTZone",0,
				 "Motoneuron2.Soma1.InhChannel");
 res=net->CreateLink("PostAfferent24.LTZone",0,
				 "Motoneuron1.Soma1.InhChannel");
 res=net->CreateLink("PostAfferent24.LTZone",0,
				 "Motoneuron2.Soma1.ExcChannel");


 if(!res)
  res=true;

 return net;
}

// Формирует простейшую СУ двигательной единицей из двух мотонейронов
UEPtr<UNet> CreateMotionElement(UStorage *storage, const string &netclassname, int mode)
{
 UEPtr<UContainer> cont;
 bool res;

 UEPtr<UNet> net=RDK::dynamic_pointer_cast<UNet>(storage->TakeObject(netclassname));
 if(!net)
  return 0;

 // Клетка реншоу 1
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynRenshowCell"));
 if(!cont)
  return 0;
 cont->SetName("Renshow1");
 res=net->AddComponent(cont);

 // Клетка реншоу 2
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynRenshowCell"));
 if(!cont)
  return 0;
 cont->SetName("Renshow2");
 res=net->AddComponent(cont);

 // Мотонейрон 1
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynMotoneuron"));
 if(!cont)
  return 0;
 cont->SetName("Motoneuron1");
 res=net->AddComponent(cont);

 // Мотонейрон 2
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynMotoneuron"));
 if(!cont)
  return 0;
 cont->SetName("Motoneuron2");
 res=net->AddComponent(cont);

 // Постафферентные нейроны
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent11");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent12");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent13");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent14");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent21");
 res=net->AddComponent(cont);


 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent22");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent23");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSynSPNeuron"));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent24");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSAfferentNeuron"));
 if(!cont)
  return 0;
 cont->SetName("Afferent_Ia1");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSAfferentNeuron"));
 if(!cont)
  return 0;
 cont->SetName("Afferent_Ia2");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSAfferentNeuron"));
 if(!cont)
  return 0;
 cont->SetName("Afferent_Ib1");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSAfferentNeuron"));
 if(!cont)
  return 0;
 cont->SetName("Afferent_Ib2");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSAfferentNeuron"));
 if(!cont)
  return 0;
 cont->SetName("Afferent_II1");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NSAfferentNeuron"));
 if(!cont)
  return 0;
 cont->SetName("Afferent_II2");
 res=net->AddComponent(cont);

 // Установка связей
 ULongId item,conn;

 res=net->CreateLink("Afferent_Ia1.LTZone",0,
				 "Motoneuron1.Soma1.ExcChannel");

 res=net->CreateLink("Afferent_Ia1.LTZone",0,
				 "PostAfferent11.Soma1.ExcChannel");

 res=net->CreateLink("Afferent_Ia2.LTZone",0,
				 "Motoneuron2.Soma1.ExcChannel");

 res=net->CreateLink("Afferent_Ia2.LTZone",0,
				 "PostAfferent21.Soma1.ExcChannel");

 res=net->CreateLink("PostAfferent11.LTZone",0,
				 "Motoneuron2.Soma1.InhChannel");

 res=net->CreateLink("PostAfferent21.LTZone",0,
				 "Motoneuron1.Soma1.InhChannel");

 res=net->CreateLink("Motoneuron1.LTZone",0,
				 "Renshow1.Soma1.ExcChannel");

 res=net->CreateLink("Renshow1.LTZone",0,
				 "Motoneuron1.Soma1.InhChannel");

 res=net->CreateLink("Renshow1.LTZone",0,
				 "PostAfferent11.Soma1.InhChannel");

 res=net->CreateLink("Motoneuron2.LTZone",0,
				 "Renshow2.Soma1.ExcChannel");

 res=net->CreateLink("Renshow2.LTZone",0,
				 "Motoneuron2.Soma1.InhChannel");

 res=net->CreateLink("Renshow2.LTZone",0,
				 "PostAfferent21.Soma1.InhChannel");

 switch(mode)
 {
 case 0:
  // Канал Ib
  res=net->CreateLink("Afferent_Ib1.LTZone",0,
				 "PostAfferent12.Soma1.ExcChannel");

  res=net->CreateLink("Afferent_Ib1.LTZone",0,
				 "PostAfferent13.Soma1.ExcChannel");

  res=net->CreateLink("PostAfferent12.LTZone",0,
				 "Motoneuron2.Soma1.ExcChannel");

  res=net->CreateLink("PostAfferent13.LTZone",0,
				 "Motoneuron1.Soma1.InhChannel");

  res=net->CreateLink("Afferent_Ib2.LTZone",0,
				 "PostAfferent22.Soma1.ExcChannel");

  res=net->CreateLink("Afferent_Ib2.LTZone",0,
				 "PostAfferent23.Soma1.ExcChannel");

  res=net->CreateLink("PostAfferent22.LTZone",0,
				 "Motoneuron1.Soma1.ExcChannel");

  res=net->CreateLink("PostAfferent23.LTZone",0,
				 "Motoneuron2.Soma1.InhChannel");

  // Канал II
  res=net->CreateLink("Afferent_II1.LTZone",0,
				 "PostAfferent14.Soma1.ExcChannel");

  res=net->CreateLink("PostAfferent14.LTZone",0,
				 "Motoneuron1.Soma1.ExcChannel");
  res=net->CreateLink("PostAfferent14.LTZone",0,
				 "Motoneuron2.Soma1.InhChannel");

  res=net->CreateLink("Afferent_II2.LTZone",0,
				 "PostAfferent24.Soma1.ExcChannel");

  res=net->CreateLink("PostAfferent24.LTZone",0,
				 "Motoneuron2.Soma1.ExcChannel");
  res=net->CreateLink("PostAfferent24.LTZone",0,
				 "Motoneuron1.Soma1.InhChannel");
 break;

 case 1:
  // Варинат канала b без промежуточных нейронов
  res=net->CreateLink("Afferent_Ib1.LTZone",0,
				 "Motoneuron2.Soma1.ExcChannel");

  res=net->CreateLink("Afferent_Ib1.LTZone",0,
				 "Motoneuron1.Soma1.InhChannel");

  res=net->CreateLink("Afferent_Ib2.LTZone",0,
				 "Motoneuron1.Soma1.ExcChannel");

  res=net->CreateLink("Afferent_Ib2.LTZone",0,
				 "Motoneuron2.Soma1.InhChannel");

  // Варинат канала II без промежуточных нейронов
  res=net->CreateLink("Afferent_II1.LTZone",0,
				 "Motoneuron1.Soma1.ExcChannel");

  res=net->CreateLink("Afferent_II1.LTZone",0,
				 "Motoneuron2.Soma1.InhChannel");

  res=net->CreateLink("Afferent_II2.LTZone",0,
				 "Motoneuron2.Soma1.ExcChannel");

  res=net->CreateLink("Afferent_II2.LTZone",0,
				 "Motoneuron1.Soma1.InhChannel");
 break;
 }

 if(!res)
  res=true;

 return net;
}


bool CreateNeuronBranchLink(UEPtr<UNet> net,const string &source,
	const string &target_head, const string &target_tail)
{
 string tmpname;
 UEPtr<NPulseNeuron> neuron=dynamic_pointer_cast<NPulseNeuron>(net->GetComponent(target_head));
 UEPtr<NPulseMembraneCommon> branch;
 UEPtr<NPulseChannel> channel;
 UEPtr<UContainer> ltmembr;

 bool ltmembr_found=neuron->CheckComponent("LTMembrane");
 if(ltmembr_found)
 {
  try
  {
   ltmembr=neuron->GetComponent("LTMembrane");
  }
  catch(UContainer::EComponentNameNotExist &){}
 }
 if(ltmembr)
  branch=neuron->BranchDendrite("Soma1",false);
 else
  branch=neuron->BranchDendrite("Soma1",true);
 channel=dynamic_pointer_cast<NPulseChannel>(branch->GetComponentL(target_tail));
 bool res=net->CreateLink(source,0,
				 channel->GetLongName(net,tmpname));
 return res;
}

bool CreateNeuronBranchLink(UEPtr<UNet> net,const string &source,
	const string &target_head, const string &target_tail, string &branch_bame)
{
 string tmpname;
 UEPtr<NPulseNeuron> neuron=dynamic_pointer_cast<NPulseNeuron>(net->GetComponent(target_head));
 UEPtr<NPulseMembraneCommon> branch;
 UEPtr<NPulseChannel> channel;
 UEPtr<UContainer> ltmembr;

 bool ltmembr_found=neuron->CheckComponent("LTMembrane");
 if(ltmembr_found)
 {
  try
  {
   ltmembr=neuron->GetComponent("LTMembrane");
  }
  catch(UContainer::EComponentNameNotExist &){}
 }
 if(ltmembr)
  branch=neuron->BranchDendrite("Soma1",false);
 else
  branch=neuron->BranchDendrite("Soma1",true);

 branch->GetLongName(neuron,branch_bame);
 channel=dynamic_pointer_cast<NPulseChannel>(branch->GetComponentL(target_tail));
 bool res=net->CreateLink(source,0,
				 channel->GetLongName(net,tmpname));
 return res;
}

bool CreateNeuronExsitedBranchLink(UEPtr<UNet> net,const string &source,
	const string &target_head, const string &target_tail, const string &branch_name)
{
 string tmpname;
 UEPtr<NPulseNeuron> neuron=dynamic_pointer_cast<NPulseNeuron>(net->GetComponent(target_head));
 UEPtr<NPulseMembrane> branch;
 UEPtr<NPulseChannel> channel;
 UEPtr<UContainer> ltmembr;

 bool ltmembr_found=neuron->CheckComponent("LTMembrane");
 if(ltmembr_found)
 {
  try
  {
   ltmembr=neuron->GetComponent("LTMembrane");
  }
  catch(UContainer::EComponentNameNotExist &){}
 }
 try
 {
  branch=dynamic_pointer_cast<NPulseMembrane>(neuron->GetComponent(branch_name));
 }
 catch(UContainer::EComponentNameNotExist &)
 {
  return false;
 }
 channel=dynamic_pointer_cast<NPulseChannel>(branch->GetComponentL(target_tail));
 bool res=net->CreateLink(source,0,
				 channel->GetLongName(net,tmpname));
 return res;
}

// Формирует СУ двигательной единицей
// Аналогично, но с развязкой по дендритам
UEPtr<UNet> CreateBranchedMotionElement(UStorage *storage,
	const string &netclassname, const string &neuron_class_name, const string &afferent_neuron_class_name, int mode)
{
 UEPtr<UContainer> cont;
 bool res;

 UEPtr<UNet> net=dynamic_pointer_cast<UNet>(storage->TakeObject(netclassname));
 if(!net)
  return 0;

 // Клетка реншоу 1
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Renshow1");
 res=net->AddComponent(cont);

 // Клетка реншоу 2
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Renshow2");
 res=net->AddComponent(cont);

 // Мотонейрон 1
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Motoneuron1");
 res=net->AddComponent(cont);

 // Мотонейрон 2
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Motoneuron2");
 res=net->AddComponent(cont);

 // Постафферентные нейроны
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent11");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent12");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent13");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent14");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent21");
 res=net->AddComponent(cont);


 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent22");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent23");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent24");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Afferent_Ia1");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Afferent_Ia2");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Afferent_Ib1");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Afferent_Ib2");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Afferent_II1");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Afferent_II2");
 res=net->AddComponent(cont);

 // Установка связей
 ULongId item,conn;
 NameT tmpname;
 res=true;

 res=CreateNeuronBranchLink(net,"Afferent_Ia1.LTZone","Motoneuron1", "ExcChannel");
 res=CreateNeuronBranchLink(net,"Afferent_Ia1.LTZone","PostAfferent11", "ExcChannel");

 res=CreateNeuronBranchLink(net,"Afferent_Ia2.LTZone","Motoneuron2", "ExcChannel");
 res=CreateNeuronBranchLink(net,"Afferent_Ia2.LTZone","PostAfferent21", "ExcChannel");
 res=CreateNeuronBranchLink(net,"PostAfferent11.LTZone","Motoneuron2", "InhChannel");
 res=CreateNeuronBranchLink(net,"PostAfferent21.LTZone","Motoneuron1", "InhChannel");
 res=CreateNeuronBranchLink(net,"Motoneuron1.LTZone","Renshow1", "ExcChannel");
 res=CreateNeuronBranchLink(net,"Renshow1.LTZone","Motoneuron1", "InhChannel");
 res=CreateNeuronBranchLink(net,"Renshow1.LTZone","PostAfferent11", "InhChannel");
 res=CreateNeuronBranchLink(net,"Motoneuron2.LTZone","Renshow2", "ExcChannel");
 res=CreateNeuronBranchLink(net,"Renshow2.LTZone","Motoneuron2", "InhChannel");
 res=CreateNeuronBranchLink(net,"Renshow2.LTZone","PostAfferent21", "InhChannel");

 string name;
 switch(mode)
 {
 case 0:
  // Канал Ib
  res=CreateNeuronBranchLink(net,"Afferent_Ib1.LTZone","PostAfferent12", "ExcChannel");
  res=CreateNeuronBranchLink(net,"PostAfferent12.LTZone","Motoneuron2", "ExcChannel");
  res=CreateNeuronBranchLink(net,"PostAfferent12.LTZone","Motoneuron1", "InhChannel");
  res=CreateNeuronBranchLink(net,"Afferent_Ib2.LTZone","PostAfferent22", "ExcChannel");
  res=CreateNeuronBranchLink(net,"PostAfferent22.LTZone","Motoneuron1", "ExcChannel");
  res=CreateNeuronBranchLink(net,"PostAfferent22.LTZone","Motoneuron2", "InhChannel");

  // Канал II
  res=CreateNeuronBranchLink(net,"Afferent_II1.LTZone","PostAfferent14", "ExcChannel");
  res=CreateNeuronBranchLink(net,"PostAfferent14.LTZone","Motoneuron1", "ExcChannel");
  res=CreateNeuronBranchLink(net,"PostAfferent14.LTZone","Motoneuron2", "InhChannel");
  res=CreateNeuronBranchLink(net,"Afferent_II2.LTZone","PostAfferent24", "ExcChannel");
  res=CreateNeuronBranchLink(net,"PostAfferent24.LTZone","Motoneuron2", "ExcChannel");
  res=CreateNeuronBranchLink(net,"PostAfferent24.LTZone","Motoneuron1", "InhChannel");
 break;

 case 1:
  // Варинат канала b без промежуточных нейронов
  res=CreateNeuronBranchLink(net,"Afferent_Ib1.LTZone","Motoneuron2", "ExcChannel");
  res=CreateNeuronBranchLink(net,"Afferent_Ib1.LTZone","Motoneuron1", "InhChannel");
  res=CreateNeuronBranchLink(net,"Afferent_Ib2.LTZone","Motoneuron1", "ExcChannel");
  res=CreateNeuronBranchLink(net,"Afferent_Ib2.LTZone","Motoneuron2", "InhChannel");

  // Варинат канала II без промежуточных нейронов
  res=CreateNeuronBranchLink(net,"Afferent_II1.LTZone","Motoneuron1", "ExcChannel");
  res=CreateNeuronBranchLink(net,"Afferent_II1.LTZone","Motoneuron2", "InhChannel");
  res=CreateNeuronBranchLink(net,"Afferent_II2.LTZone","Motoneuron2", "ExcChannel");
  res=CreateNeuronBranchLink(net,"Afferent_II2.LTZone","Motoneuron1", "InhChannel");
 break;
 }

 if(!res)
  res=true;

 return net;
}

// Формирует СУ двигательной единицей
// Аналогично, но с развязкой по дендритам
UEPtr<UNet> CreateSimplestBranchedMotionElement(UStorage *storage,
	const string &netclassname, const string &neuron_class_name,
	const string &afferent_neuron_class_name, int mode, bool use_speed_force, bool use_add_contours)
{
 UEPtr<UContainer> cont;
 bool res;

 UEPtr<UNet> net=dynamic_pointer_cast<UNet>(storage->TakeObject(netclassname));
 if(!net)
  return 0;
/*
 // Клетка реншоу 1
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Renshow1");
 res=net->AddComponent(cont);

 // Клетка реншоу 2
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Renshow2");
 res=net->AddComponent(cont);
				*/
 // Мотонейрон 1
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Motoneuron1");
 res=net->AddComponent(cont);

 // Мотонейрон 2
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Motoneuron2");
 res=net->AddComponent(cont);
  /*
 // Постафферентные нейроны
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent11");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent12");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent13");
 res=net->AddComponent(cont);
			  */
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent14");
 res=net->AddComponent(cont);
	  /*
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent21");
 res=net->AddComponent(cont);


 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent22");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent23");
 res=net->AddComponent(cont);
              */
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent24");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Afferent_II1");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Afferent_II2");
 res=net->AddComponent(cont);

 if(use_speed_force)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
  if(!cont)
   return 0;
  cont->SetName("Afferent_Ia1");
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
  if(!cont)
   return 0;
  cont->SetName("Afferent_Ia2");
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
  if(!cont)
   return 0;
  cont->SetName("Afferent_Ib1");
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
  if(!cont)
   return 0;
  cont->SetName("Afferent_Ib2");
  res=net->AddComponent(cont);
 }

 if(use_add_contours)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
  if(!cont)
   return 0;
  cont->SetName("Afferent_Ic1");
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
  if(!cont)
   return 0;
  cont->SetName("Afferent_Ic2");
  res=net->AddComponent(cont);
 }

 // Установка связей
 ULongId item,conn;
 NameT tmpname;
 res=true;

 string branch_name1, branch_name2;
 res=CreateNeuronBranchLink(net,"Afferent_II1.LTZone","Motoneuron1", "ExcChannel",branch_name1);
 res=CreateNeuronBranchLink(net,"Afferent_II2.LTZone","Motoneuron2", "ExcChannel",branch_name2);
 res=CreateNeuronExsitedBranchLink(net,"Afferent_II2.LTZone","Motoneuron1", "InhChannel",branch_name1);
 res=CreateNeuronExsitedBranchLink(net,"Afferent_II1.LTZone","Motoneuron2", "InhChannel",branch_name2);

 res=CreateNeuronBranchLink(net,"PostAfferent14.LTZone","Motoneuron1", "ExcChannel",branch_name1);
 res=CreateNeuronBranchLink(net,"PostAfferent24.LTZone","Motoneuron2", "ExcChannel",branch_name2);
 res=CreateNeuronExsitedBranchLink(net,"PostAfferent14.LTZone","Motoneuron2", "InhChannel",branch_name2);
 res=CreateNeuronExsitedBranchLink(net,"PostAfferent24.LTZone","Motoneuron1", "InhChannel",branch_name1);

// res=CreateNeuronExsitedBranchLink(net,"Motoneuron1.LTZone","Motoneuron2", "InhChannel",branch_name2);
// res=CreateNeuronExsitedBranchLink(net,"Motoneuron2.LTZone","Motoneuron1", "InhChannel",branch_name1);

 if(use_speed_force)
 {
  res=net->CreateLink("Afferent_Ia2.LTZone",0,"Motoneuron1.Soma1.ExcChannel");
  res=net->CreateLink("Afferent_Ia1.LTZone",0,"Motoneuron2.Soma1.ExcChannel");
  res=net->CreateLink("Afferent_Ia1.LTZone",0,"Motoneuron1.Soma1.InhChannel");
  res=net->CreateLink("Afferent_Ia2.LTZone",0,"Motoneuron2.Soma1.InhChannel");

  res=net->CreateLink("Afferent_Ib1.LTZone",0,"Motoneuron1.Soma1.ExcChannel");
  res=net->CreateLink("Afferent_Ib2.LTZone",0,"Motoneuron2.Soma1.ExcChannel");
  res=net->CreateLink("Afferent_Ib2.LTZone",0,"Motoneuron1.Soma1.InhChannel");
  res=net->CreateLink("Afferent_Ib1.LTZone",0,"Motoneuron2.Soma1.InhChannel");
 }

 if(use_add_contours)
 {
  res=net->CreateLink("Afferent_Ic1.LTZone",0,"Motoneuron1.Soma1.ExcChannel");
  res=net->CreateLink("Afferent_Ic1.LTZone",0,"Motoneuron2.Soma1.InhChannel");
  res=net->CreateLink("Afferent_Ic2.LTZone",0,"Motoneuron2.Soma1.ExcChannel");
  res=net->CreateLink("Afferent_Ic2.LTZone",0,"Motoneuron1.Soma1.InhChannel");
 }


 if(!res)
  res=true;

 return net;
}



// Формирует СУ двигательной единицей
// Аналогично, но с развязкой по дендритам
UEPtr<UNet> CreateSimplestBranchedMotionElementPM(UStorage *storage,
	const string &netclassname, const string &neuron_class_name,
	const string &afferent_neuron_class_name, int mode, bool use_speed_force, bool use_add_contours)
{
 UEPtr<UContainer> cont;
 bool res;

 UEPtr<UNet> net=dynamic_pointer_cast<UNet>(storage->TakeObject(netclassname));
 if(!net)
  return 0;

 // Пейсмекерный нейрон 1
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PM1");
 res=net->AddComponent(cont);
 UEPtr<NConstGenerator> gen=dynamic_pointer_cast<NConstGenerator>(cont->GetComponentL("NegGenerator"));
 gen->Amplitude=0.95;

 // Пейсмекерный нейрон 2
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PM2");
 res=net->AddComponent(cont);
 gen=dynamic_pointer_cast<NConstGenerator>(cont->GetComponentL("NegGenerator"));
 gen->Amplitude=0.95;
 /*
 // Клетка реншоу 1
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Renshow1");
 res=net->AddComponent(cont);

 // Клетка реншоу 2
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Renshow2");
 res=net->AddComponent(cont);
				*/
 // Мотонейрон 1
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Motoneuron1");
 res=net->AddComponent(cont);

 // Мотонейрон 2
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Motoneuron2");
 res=net->AddComponent(cont);
  /*
 // Постафферентные нейроны
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent11");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent12");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent13");
 res=net->AddComponent(cont);
			  */
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent14");
 res=net->AddComponent(cont);
	  /*
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent21");
 res=net->AddComponent(cont);


 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent22");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent23");
 res=net->AddComponent(cont);
			  */
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("PostAfferent24");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Afferent_II1");
 res=net->AddComponent(cont);

 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
 if(!cont)
  return 0;
 cont->SetName("Afferent_II2");
 res=net->AddComponent(cont);

 if(use_speed_force)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
  if(!cont)
   return 0;
  cont->SetName("Afferent_Ia1");
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
  if(!cont)
   return 0;
  cont->SetName("Afferent_Ia2");
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
  if(!cont)
   return 0;
  cont->SetName("Afferent_Ib1");
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
  if(!cont)
   return 0;
  cont->SetName("Afferent_Ib2");
  res=net->AddComponent(cont);
 }

 if(use_add_contours)
 {
  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
  if(!cont)
   return 0;
  cont->SetName("Afferent_Ic1");
  res=net->AddComponent(cont);

  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(afferent_neuron_class_name));
  if(!cont)
   return 0;
  cont->SetName("Afferent_Ic2");
  res=net->AddComponent(cont);
 }

 // Установка связей
 ULongId item,conn;
 NameT tmpname;
 res=true;

 string branch_name1, branch_name2;
 res=CreateNeuronBranchLink(net,"Afferent_II1.LTZone","Motoneuron1", "ExcChannel",branch_name1);
 res=CreateNeuronBranchLink(net,"Afferent_II2.LTZone","Motoneuron2", "ExcChannel",branch_name2);
 res=CreateNeuronExsitedBranchLink(net,"Afferent_II2.LTZone","Motoneuron1", "InhChannel",branch_name1);
 res=CreateNeuronExsitedBranchLink(net,"Afferent_II1.LTZone","Motoneuron2", "InhChannel",branch_name2);

 res=CreateNeuronExsitedBranchLink(net,"Afferent_II1.LTZone","PM1", "InhChannel","Soma1");
 res=CreateNeuronExsitedBranchLink(net,"Afferent_II2.LTZone","PM2", "InhChannel","Soma1");

 res=CreateNeuronBranchLink(net,"PostAfferent14.LTZone","Motoneuron1", "ExcChannel",branch_name1);
 res=CreateNeuronBranchLink(net,"PostAfferent24.LTZone","Motoneuron2", "ExcChannel",branch_name2);
 res=CreateNeuronExsitedBranchLink(net,"PostAfferent14.LTZone","Motoneuron2", "InhChannel",branch_name2);
 res=CreateNeuronExsitedBranchLink(net,"PostAfferent24.LTZone","Motoneuron1", "InhChannel",branch_name1);

 res=CreateNeuronExsitedBranchLink(net,"PM1.LTZone","Motoneuron1", "InhChannel",branch_name2);
 res=CreateNeuronExsitedBranchLink(net,"PM2.LTZone","Motoneuron2", "InhChannel",branch_name1);

// res=CreateNeuronExsitedBranchLink(net,"Motoneuron1.LTZone","Motoneuron2", "InhChannel",branch_name2);
// res=CreateNeuronExsitedBranchLink(net,"Motoneuron2.LTZone","Motoneuron1", "InhChannel",branch_name1);

 if(use_speed_force)
 {
  res=net->CreateLink("Afferent_Ia2.LTZone",0,"Motoneuron1.Soma1.ExcChannel");
  res=net->CreateLink("Afferent_Ia1.LTZone",0,"Motoneuron2.Soma1.ExcChannel");
  res=net->CreateLink("Afferent_Ia1.LTZone",0,"Motoneuron1.Soma1.InhChannel");
  res=net->CreateLink("Afferent_Ia2.LTZone",0,"Motoneuron2.Soma1.InhChannel");

  res=net->CreateLink("Afferent_Ib1.LTZone",0,"Motoneuron1.Soma1.ExcChannel");
  res=net->CreateLink("Afferent_Ib2.LTZone",0,"Motoneuron2.Soma1.ExcChannel");
  res=net->CreateLink("Afferent_Ib2.LTZone",0,"Motoneuron1.Soma1.InhChannel");
  res=net->CreateLink("Afferent_Ib1.LTZone",0,"Motoneuron2.Soma1.InhChannel");
 }

 if(use_add_contours)
 {
  res=net->CreateLink("Afferent_Ic1.LTZone",0,"Motoneuron1.Soma1.ExcChannel");
  res=net->CreateLink("Afferent_Ic1.LTZone",0,"Motoneuron2.Soma1.InhChannel");
  res=net->CreateLink("Afferent_Ic2.LTZone",0,"Motoneuron2.Soma1.ExcChannel");
  res=net->CreateLink("Afferent_Ic2.LTZone",0,"Motoneuron1.Soma1.InhChannel");
 }


 if(!res)
  res=true;

 return net;
}


// Создает пару мотонейронов
 bool NMotionElement::CreateMotoneurons()
 {

   UEPtr<UContainer> cont;
   UEPtr<UStorage> storage = GetStorage();
   bool res(true);


 // Мотонейрон 1
   cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(NeuroObjectName));
   if(!cont)
	return 0;
   cont->SetName("MotoneuronL");
   res&=(AddComponent(cont)!=ForbiddenId);

  // Мотонейрон 2
   cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(NeuroObjectName));
   if(!cont)
	return false;
   cont->SetName("MotoneuronR");
   res&=(AddComponent(cont)!=ForbiddenId);

   if(RenshowMode)
   {
	// Клетка реншоу 1
	cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(NeuroObjectName));
	if(!cont)
	 return false;
	cont->SetName("RenshowL");
    res&=(AddComponent(cont)!=ForbiddenId);

	// Клетка реншоу 2
	cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(NeuroObjectName));
	if(!cont)
	 return false;
	cont->SetName("RenshowR");
    res&=(AddComponent(cont)!=ForbiddenId);
   }

   return res;
 }
 // Создаёт связку афферентных нейронов
 bool NMotionElement::CreateAfferents()
 {
   UEPtr<UContainer> cont;
   UEPtr<UStorage> storage = GetStorage();
   bool res(true);


   for (int i=0; i<NumControlLoops; i++)
   {
	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(AfferentObjectName));
	  if(!cont)
	   return 0;
	  cont->SetName("AfferentR"+sntoa(i+1));
      res&=(AddComponent(cont)!=ForbiddenId);

	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(AfferentObjectName));
	  if(!cont)
	   return 0;
	  cont->SetName("AfferentL"+sntoa(i+1));
      res&=(AddComponent(cont)!=ForbiddenId);
   }

   return res;
 }
 bool NMotionElement::CreateInterneurons()
 {
   if(InterneuronPresentMode!=1)
    return 0;
   UEPtr<UContainer> cont;
   UEPtr<UStorage> storage = GetStorage();
   bool res(true);

   for (int i=0; i<NumControlLoops; i++)
   {
	cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(NeuroObjectName));
	if(!cont)
	 return 0;
	cont->SetName("PostAfferentL"+sntoa(i+1));
    res&=(AddComponent(cont)!=ForbiddenId);

	cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(NeuroObjectName));
	if(!cont)
	 return 0;
	cont->SetName("PostAfferentR"+sntoa(i+1));
    res&=(AddComponent(cont)!=ForbiddenId);
   }

   return res;
 }

 // Создание связей
 bool NMotionElement::LinkMotoneurons(const string &afferentL, const string &afferentR, int mode)
 {
   if(!(CheckComponent(afferentL) && CheckComponent(afferentR) && CheckComponent("MotoneuronL") &&
		CheckComponent("MotoneuronR")))
		return false;
   UEPtr<UContainer> cont;
   UEPtr<UStorage> storage = GetStorage();
   bool res = true;

   ULongId item,conn;
   NameT tmpname;
   res=true;

   if (!MotoneuronBranchMode)
   {
	   switch(mode)
	   {
		case 0:
			res&=LinkNeuron(afferentL,"MotoneuronL",0);
			res&=LinkNeuron(afferentR,"MotoneuronR",0);
			res&=LinkNeuron(afferentL,"MotoneuronR",1);
			res&=LinkNeuron(afferentR,"MotoneuronL",1);
			break;

		 case 1:
			if(!(CheckComponent("Post"+afferentL)&&CheckComponent("Post"+afferentR)))
			  return false;
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=LinkNeuron("Post"+afferentL,"MotoneuronR",1);
			res&=LinkNeuron("Post"+afferentL,"MotoneuronL",0);
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			res&=LinkNeuron("Post"+afferentR,"MotoneuronL",1);
			res&=LinkNeuron("Post"+afferentR,"MotoneuronR",0);
			break;

		 case 2:
			if(!(CheckComponent("Post"+afferentL)&&CheckComponent("Post"+afferentR)))
			  return false;
			res&=LinkNeuron(afferentL,"MotoneuronL",0);
			res&=LinkNeuron(afferentR,"MotoneuronR",0);
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=LinkNeuron("Post"+afferentL,"MotoneuronR",1);
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			res&=LinkNeuron("Post"+afferentR,"MotoneuronL",1);
			break;

		 case 3:
			if(!(CheckComponent("Post"+afferentL)&&CheckComponent("Post"+afferentR)))
			  return false;
			res&=LinkNeuron(afferentL,"MotoneuronR",1);
			res&=LinkNeuron(afferentR,"MotoneuronL",1);
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			res&=LinkNeuron("Post"+afferentL,"MotoneuronL",0);
			res&=LinkNeuron("Post"+afferentR,"MotoneuronR",0);
			break;

		 case 4:
			if(!(CheckComponent("Post"+afferentL)&&CheckComponent("Post"+afferentR)))
			  return false;
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
//			res&=CreateLink("Post"+afferentL+".LTZone",0,"MotoneuronR.Soma1.InhChannel");
			res&=LinkNeuron("Post"+afferentL,"MotoneuronL",0);
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
 //			res&=CreateLink("Post"+afferentR+".LTZone",0,"MotoneuronL.Soma1.InhChannel");
			res&=LinkNeuron("Post"+afferentR,"MotoneuronR",0);
			break;

	   }
   }

   else
   {
    std::string branch_name;
	   switch(mode)
	   {
		case 0:
		 res&=CreateNeuronBranchLink(this,afferentL+".LTZone","MotoneuronL", "ExcChannel",branch_name);
			res&=LinkNeuron(afferentL,"MotoneuronR",1,branch_name);
		 res&=CreateNeuronBranchLink(this,afferentR+".LTZone","MotoneuronR", "ExcChannel",branch_name);
			res&=LinkNeuron(afferentR,"MotoneuronL",1,branch_name);
		 break;

		 case 1:
			if(!(CheckComponent("Post"+afferentL) && CheckComponent("Post"+afferentR)))
			  return false;
//			res&=CreateNeuronBranchLink(this,afferentL+".LTZone","Post"+afferentL,"ExcChannel");
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=CreateNeuronBranchLink(this,"Post"+afferentL+".LTZone","MotoneuronR","InhChannel",branch_name);
			res&=LinkNeuron("Post"+afferentR,"MotoneuronR",0,branch_name);
//			res&=CreateNeuronBranchLink(this,afferentR+".LTZone","Post"+afferentR,"ExcChannel");
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			res&=CreateNeuronBranchLink(this,"Post"+afferentR+".LTZone","MotoneuronL","InhChannel",branch_name);
			res&=LinkNeuron("Post"+afferentL,"MotoneuronL",0,branch_name);
			break;

		 case 2:
			if(!(CheckComponent("Post"+afferentL) && CheckComponent("Post"+afferentR)))
			  return false;
			res&=CreateNeuronBranchLink(this,afferentL+".LTZone","MotoneuronL","ExcChannel",branch_name);
			res&=LinkNeuron("Post"+afferentR,"MotoneuronL",1,branch_name);
//			res&=CreateNeuronBranchLink(this,afferentL+".LTZone","Post"+afferentL,"ExcChannel");
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=LinkNeuron("Post"+afferentL,"MotoneuronR",1,branch_name);
			res&=CreateNeuronBranchLink(this,afferentR+".LTZone","MotoneuronR","ExcChannel",branch_name);
//			res&=CreateNeuronBranchLink(this,afferentR+".LTZone","Post"+afferentR,"ExcChannel");
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			break;

		 case 3:
			if(!(CheckComponent("Post"+afferentL) && CheckComponent("Post"+afferentR)))
			  return false;
			res&=CreateNeuronBranchLink(this,afferentL+".LTZone","MotoneuronR","InhChannel",branch_name);
			res&=LinkNeuron("Post"+afferentR,"MotoneuronR",0,branch_name);
//			res&=CreateNeuronBranchLink(this,afferentL+".LTZone","Post"+afferentL,"ExcChannel");
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=LinkNeuron("Post"+afferentL,"MotoneuronL",0,branch_name);
			res&=CreateNeuronBranchLink(this,afferentR+".LTZone","MotoneuronL","InhChannel",branch_name);
//			res&=CreateNeuronBranchLink(this,afferentR+".LTZone","Post"+afferentR,"ExcChannel");
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			break;

		 case 4:
			if(!(CheckComponent("Post"+afferentL) && CheckComponent("Post"+afferentR)))
			  return false;
//			res&=CreateNeuronBranchLink(this,afferentL+".LTZone","Post"+afferentL,"ExcChannel");
//			res&=CreateNeuronBranchLink(this,afferentR+".LTZone","Post"+afferentR,"ExcChannel");
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=CreateNeuronBranchLink(this,"Post"+afferentL+".LTZone","MotoneuronL","ExcChannel");
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			res&=CreateNeuronBranchLink(this,"Post"+afferentR+".LTZone","MotoneuronR","ExcChannel");
			break;

	   }
   }

   return res;
 }
 bool NMotionElement::LinkRenshow()
 {
   bool res = true;
   if(!(CheckComponent("RenshowL")&&CheckComponent("RenshowR")))
	 return false;
   res&=LinkNeuron("MotoneuronL","RenshowL",0);
   res&=LinkNeuron("RenshowL","MotoneuronL",1);

   res&=LinkNeuron("MotoneuronR","RenshowR",0);
   res&=LinkNeuron("RenshowR","MotoneuronR",1);

   return res;
 }
 bool NMotionElement::LinkNeuron(const string &source, const string &sink, int mode, const string &branch)
 {
   bool res = true;
   bool isSyn(NeuroObjectName->find("Syn")!=std::string::npos);

   string effect = (mode==0)?"Exc":"Inh";
   string input_element = (isSyn)?"Channel":"Synapse1";
   string input = (isSyn)?"SynapticInputs":"Input";

   res&=CreateLink(source+".LTZone","Output",sink+"."+branch+"."+effect+input_element,input);

   return res;
 }
}
//---------------------------------------------------------------------------
#endif
