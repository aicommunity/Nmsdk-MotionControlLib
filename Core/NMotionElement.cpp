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
PacemakerMode("PacemakerMode",this, &NMotionElement::SetPacemakerMode),
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
bool NMotionElement::SetPacemakerMode(const int &value)
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
  NeuroObjectName = "NNewSPNeuron";
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
   LinkMotoneurons();
   if(RenshowMode)
	 LinkRenshow();
   if(PacemakerMode)
     LinkPM();
}

// Сохраняет и восстанавливает внешние связи
void NMotionElement::BackupExternalLinks(void)
{

}

void NMotionElement::RestoreExternalLinks(void)
{

}
// --------------------------

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
 bool res=net->CreateLink(source,"Output",
                 channel->GetLongName(net,tmpname),"ChannelInput");
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
 bool res=net->CreateLink(source,"Output",
                 channel->GetLongName(net,tmpname), "ChannelInput");
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
 bool res=net->CreateLink(source,"Output",
                 channel->GetLongName(net,tmpname), "ChannelInput");
 return res;
}




// Создает пару мотонейронов
 bool NMotionElement::CreateMotoneurons()
 {

   UEPtr<UContainer> cont;
   UEPtr<NPulseNeuron> neuron;
   UEPtr<UStorage> storage = GetStorage();
   bool res(true);


 // Мотонейрон 1
   cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(NeuroObjectName));
   if(!cont)
	return 0;
   cont->SetName("MotoneuronL");
   res&=(AddComponent(cont)!=ForbiddenId);
   cont->SetCoord(MVector<double,3>(22.0, 5.0, 0));
   neuron = dynamic_pointer_cast<NPulseNeuron> (cont);
   if(!neuron)
	return 0;
   neuron->NumSomaMembraneParts = NumControlLoops;


  // Мотонейрон 2
   cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(NeuroObjectName));
   if(!cont)
	return false;
   cont->SetName("MotoneuronR");
   res&=(AddComponent(cont)!=ForbiddenId);
   cont->SetCoord(MVector<double,3>(22.0, 8.0, 1.0));
   neuron = dynamic_pointer_cast<NPulseNeuron> (cont);
   if(!neuron)
	return 0;
   neuron->NumSomaMembraneParts = NumControlLoops;

   if(RenshowMode)
   {
	// Клетка реншоу 1
	cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(NeuroObjectName));
	if(!cont)
	 return false;
	cont->SetName("RenshowL");
    res&=(AddComponent(cont)!=ForbiddenId);
    cont->SetCoord(MVector<double,3>(15.0, 3.0, 0.0));

	// Клетка реншоу 2
	cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(NeuroObjectName));
	if(!cont)
	 return false;
	cont->SetName("RenshowR");
    res&=(AddComponent(cont)!=ForbiddenId);
    cont->SetCoord(MVector<double,3>(15.0, 10.0, 1.0));
   }

   if(PacemakerMode)
   {
	// пейсмейкер 1
	cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(NeuroObjectName));
	if(!cont)
	 return false;
	cont->SetName("PmL");
    res&=(AddComponent(cont)!=ForbiddenId);
    cont->SetCoord(MVector<double,3>(15.0, 1.0, 4.0));

	// пейсмейкер 2
	cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(NeuroObjectName));
	if(!cont)
	 return false;
	cont->SetName("PmR");
    res&=(AddComponent(cont)!=ForbiddenId);
    cont->SetCoord(MVector<double,3>(15.0, 12.0, 5.0));
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
      cont->SetCoord(MVector<double,3>((5.0+i), 8.0, 2.0));

	  cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(AfferentObjectName));
	  if(!cont)
	   return 0;
	  cont->SetName("AfferentL"+sntoa(i+1));
      res&=(AddComponent(cont)!=ForbiddenId);
      cont->SetCoord(MVector<double,3>((5.0+i), 5.0, 3.0));
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
    cont->SetCoord(MVector<double,3>((15.0+i), 5.0, 4));

	cont=dynamic_pointer_cast<UContainer>(storage->TakeObject(NeuroObjectName));
	if(!cont)
	 return 0;
	cont->SetName("PostAfferentR"+sntoa(i+1));
    res&=(AddComponent(cont)!=ForbiddenId);
    cont->SetCoord(MVector<double,3>((15.0+i), 8.0, 5));
   }

   return res;
 }




 // Создание связей
 bool NMotionElement::LinkMotoneurons()
 {
   UEPtr<UContainer> cont;
   UEPtr<UStorage> storage = GetStorage();
   bool res = true;

   ULongId item,conn;
   NameT tmpname;
   res=true;
   for (int i=0; i<NumControlLoops; i++)
   {
	 string afferentL = "AfferentL"+sntoa(i+1);
	 string afferentR = "AfferentR"+sntoa(i+1);
	 int mode = (*LinkModes)[i];

     if(!(CheckComponent(afferentL) && CheckComponent(afferentR) &&
        CheckComponent("MotoneuronL") && CheckComponent("MotoneuronR")))
		return false;

   if (!MotoneuronBranchMode)
   {
	   switch(mode)
	   {
        case 0: //прямая связь (без интернейронов)
            res&=LinkNeuron(afferentL,"MotoneuronL",0,"Soma"+sntoa(i+1)); // 0 - возбуждающая связь
			res&=LinkNeuron(afferentR,"MotoneuronR",0,"Soma"+sntoa(i+1));
            res&=LinkNeuron(afferentL,"MotoneuronR",1,"Soma"+sntoa(i+1)); // 1 - тормозная связь
			res&=LinkNeuron(afferentR,"MotoneuronL",1,"Soma"+sntoa(i+1));
			break;

         case 1: //связь через интернейроны - все связи рабочие
			if(!(CheckComponent("Post"+afferentL)&&CheckComponent("Post"+afferentR)))
			  return false;
            res&=LinkNeuron(afferentL,"Post"+afferentL,0); //возбуждающая связь
            res&=LinkNeuron("Post"+afferentL,"MotoneuronR",1,"Soma"+sntoa(i+1)); //тормозная связь
            res&=LinkNeuron("Post"+afferentL,"MotoneuronL",0,"Soma"+sntoa(i+1)); //возбуждающая связь
            res&=LinkNeuron(afferentR,"Post"+afferentR,0); //возбуждающая связь
            res&=LinkNeuron("Post"+afferentR,"MotoneuronL",1,"Soma"+sntoa(i+1)); //тормозная связь
            res&=LinkNeuron("Post"+afferentR,"MotoneuronR",0,"Soma"+sntoa(i+1)); //возбуждающая связь
			break;

         case 2: //связь через интернейроны(L-R) + прямая связь(L-L)
			if(!(CheckComponent("Post"+afferentL)&&CheckComponent("Post"+afferentR)))
			  return false;
			res&=LinkNeuron(afferentL,"MotoneuronL",0,"Soma"+sntoa(i+1));
			res&=LinkNeuron(afferentR,"MotoneuronR",0,"Soma"+sntoa(i+1));
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=LinkNeuron("Post"+afferentL,"MotoneuronR",1,"Soma"+sntoa(i+1));
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			res&=LinkNeuron("Post"+afferentR,"MotoneuronL",1,"Soma"+sntoa(i+1));
			break;

         case 3://связь через интернейроны(L-L) + прямая связь(L-R)
			if(!(CheckComponent("Post"+afferentL)&&CheckComponent("Post"+afferentR)))
			  return false;
			res&=LinkNeuron(afferentL,"MotoneuronR",1,"Soma"+sntoa(i+1));
			res&=LinkNeuron(afferentR,"MotoneuronL",1,"Soma"+sntoa(i+1));
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			res&=LinkNeuron("Post"+afferentL,"MotoneuronL",0,"Soma"+sntoa(i+1));
			res&=LinkNeuron("Post"+afferentR,"MotoneuronR",0,"Soma"+sntoa(i+1));
			break;

		 case 4:
			if(!(CheckComponent("Post"+afferentL)&&CheckComponent("Post"+afferentR)))
			  return false;
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
//			res&=CreateLink("Post"+afferentL+".LTZone",0,"MotoneuronR.Soma1.InhChannel");
			res&=LinkNeuron("Post"+afferentL,"MotoneuronL",0,"Soma"+sntoa(i+1));
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
 //			res&=CreateLink("Post"+afferentR+".LTZone",0,"MotoneuronL.Soma1.InhChannel");
			res&=LinkNeuron("Post"+afferentR,"MotoneuronR",0,"Soma"+sntoa(i+1));
			break;

	   }
   }

   else
   {
    std::string branch_name;
	   switch(mode)
	   {
        case 0: //прямая связь (без интернейронов)
		 res&=CreateNeuronBranchLink(this,afferentL+".LTZone","MotoneuronL", "ExcChannel",branch_name);
			res&=LinkNeuron(afferentL,"MotoneuronR",1,branch_name);
		 res&=CreateNeuronBranchLink(this,afferentR+".LTZone","MotoneuronR", "ExcChannel",branch_name);
			res&=LinkNeuron(afferentR,"MotoneuronL",1,branch_name);
		 break;

         case 1: //связь через интернейроны
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

         case 2: //связь через интернейроны(L-R) + прямая связь(L-L)
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

         case 3: //связь через интернейроны(L-L) + прямая связь(L-R)
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




bool NMotionElement::LinkPM()
 {
   bool res = true;
   if(!(CheckComponent("PmL")&&CheckComponent("PmR")))
	 return false;
   res&=LinkNeuron("AfferentL1","PmL",1);
   res&=LinkNeuron("AfferentR1","PmR",1);

   res&=LinkNeuron("PmL","MotoneuronL",1);
   res&=LinkNeuron("PmR","MotoneuronR",1);

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
