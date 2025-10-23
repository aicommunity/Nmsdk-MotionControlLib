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
// ������������ � �����������
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
// ������ ���������� �����������
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
 if(value==0)//���� ������������ ����������� - ����� ����� � ����� 0 (��� �������������)
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
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
NMotionElement* NMotionElement::New(void)
{
 return new NMotionElement;
}
// --------------------------

// --------------------------
// Proctected computation methods
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
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


// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
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


// ������� ��������� � ������������ � �������� ���������� ����������
// ���� ��������� ����������, �� �������� �������������� �� � ������������ �����������
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

// ������� ���������� ����� � ������������ � �������� ���������� ����������
void NMotionElement::CreateInternalLinks(void)
{
   LinkMotoneurons();
   if(RenshowMode)
	 LinkRenshow();
   if(PacemakerMode)
     LinkPM();
}

// ��������� � ��������������� ������� �����
void NMotionElement::BackupExternalLinks(void)
{

}

void NMotionElement::RestoreExternalLinks(void)
{

}
// --------------------------

bool CreateNeuronBranchLink(std::shared_ptr<UNet> net,const string &source,
	const string &target_head, const string &target_tail)
{
 string tmpname;
 std::shared_ptr<NPulseNeuron> neuron=dynamic_pointer_cast<NPulseNeuron>(net->GetComponent(target_head));
 std::shared_ptr<NPulseMembraneCommon> branch;
 std::shared_ptr<NPulseChannel> channel;
 std::shared_ptr<UContainer> ltmembr;

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
  branch=std::shared_ptr<NPulseMembraneCommon>(neuron->BranchDendrite("Soma1",false), RDK::NonOwningDeleter());
 else
  branch=std::shared_ptr<NPulseMembraneCommon>(neuron->BranchDendrite("Soma1",true), RDK::NonOwningDeleter());
 channel=dynamic_pointer_cast<NPulseChannel>(branch->GetComponentL(target_tail));
 bool res=net->CreateLink(source,"Output",
                 channel->GetLongName(net,tmpname),"ChannelInput");
 return res;
}





bool CreateNeuronBranchLink(std::shared_ptr<UNet> net,const string &source,
	const string &target_head, const string &target_tail, string &branch_bame)
{
 string tmpname;
 std::shared_ptr<NPulseNeuron> neuron=dynamic_pointer_cast<NPulseNeuron>(net->GetComponent(target_head));
 std::shared_ptr<NPulseMembraneCommon> branch;
 std::shared_ptr<NPulseChannel> channel;
 std::shared_ptr<UContainer> ltmembr;

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
  branch=std::shared_ptr<NPulseMembraneCommon>(neuron->BranchDendrite("Soma1",false), RDK::NonOwningDeleter());
 else
  branch=std::shared_ptr<NPulseMembraneCommon>(neuron->BranchDendrite("Soma1",true), RDK::NonOwningDeleter());

 branch->GetLongName(neuron,branch_bame);
 channel=dynamic_pointer_cast<NPulseChannel>(branch->GetComponentL(target_tail));
 bool res=net->CreateLink(source,"Output",
                 channel->GetLongName(net,tmpname), "ChannelInput");
 return res;
}




bool CreateNeuronExsitedBranchLink(std::shared_ptr<UNet> net,const string &source,
	const string &target_head, const string &target_tail, const string &branch_name)
{
 string tmpname;
 std::shared_ptr<NPulseNeuron> neuron=dynamic_pointer_cast<NPulseNeuron>(net->GetComponent(target_head));
 std::shared_ptr<NPulseMembrane> branch;
 std::shared_ptr<NPulseChannel> channel;
 std::shared_ptr<UContainer> ltmembr;

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




// ������� ���� ������������
 bool NMotionElement::CreateMotoneurons()
 {
  // ���������� 1
  std::shared_ptr<NPulseNeuron> mn1 = AddMissingComponent<NPulseNeuron>("MotoneuronL", NeuroObjectName);
  if(!mn1)
   return false;
  mn1->SetCoord(MVector<double,3>(22.0, 5.0, 0));
  mn1->NumSomaMembraneParts = NumControlLoops;

  // ���������� 2
  std::shared_ptr<NPulseNeuron> mn2 = AddMissingComponent<NPulseNeuron>("MotoneuronR", NeuroObjectName);
  if(!mn2)
   return false;
  mn2->SetCoord(MVector<double,3>(22.0, 8.0, 1.0));
  mn2->NumSomaMembraneParts = NumControlLoops;

  if(RenshowMode)
  {
   // ������ ������ 1
   std::shared_ptr<NPulseNeuron> ren1=AddMissingComponent<NPulseNeuron>("RenshowL", NeuroObjectName);
   if(!ren1)
    return false;
   ren1->SetCoord(MVector<double,3>(15.0, 3.0, 0.0));

   // ������ ������ 2
   std::shared_ptr<NPulseNeuron> ren2=AddMissingComponent<NPulseNeuron>("RenshowR", NeuroObjectName);
   if(!ren2)
    return false;
   ren2->SetCoord(MVector<double,3>(15.0, 10.0, 1.0));
  }

  if(PacemakerMode)
  {
   // ���������� 1
   std::shared_ptr<NPulseNeuron> pml=AddMissingComponent<NPulseNeuron>("PmL", NeuroObjectName);
   if(!pml)
    return false;
   pml->SetCoord(MVector<double,3>(15.0, 1.0, 4.0));

   // ���������� 2
   std::shared_ptr<NPulseNeuron> pmr=AddMissingComponent<NPulseNeuron>("PmR", NeuroObjectName);
   if(!pmr)
    return false;
   pmr->SetCoord(MVector<double,3>(15.0, 12.0, 5.0));
  }
  return true;
 }



 // ������ ������ ����������� ��������
 bool NMotionElement::CreateAfferents()
 {
   for (int i=0; i<NumControlLoops; i++)
   {
    std::shared_ptr<NAfferentNeuron> cont=AddMissingComponent<NAfferentNeuron>("AfferentR"+sntoa(i+1), AfferentObjectName);
    if(!cont)
     return false;
    cont->SetCoord(MVector<double,3>((5.0+i), 8.0, 2.0));

    cont=AddMissingComponent<NAfferentNeuron>("AfferentL"+sntoa(i+1), AfferentObjectName);
    if(!cont)
     return false;
    cont->SetCoord(MVector<double,3>((5.0+i), 5.0, 3.0));
   }

   return true;
 }




 bool NMotionElement::CreateInterneurons()
 {
   if(InterneuronPresentMode!=1)
    return false;

   for (int i=0; i<NumControlLoops; i++)
   {
    std::shared_ptr<NPulseNeuron> cont=AddMissingComponent<NPulseNeuron>("PostAfferentL"+sntoa(i+1), NeuroObjectName);
	if(!cont)
     return false;
    cont->SetCoord(MVector<double,3>((15.0+i), 5.0, 4));

    cont=AddMissingComponent<NPulseNeuron>("PostAfferentR"+sntoa(i+1), NeuroObjectName);
    if(!cont)
     return false;
    cont->SetCoord(MVector<double,3>((15.0+i), 8.0, 5));
   }

   return true;
 }




 // �������� ������
 bool NMotionElement::LinkMotoneurons()
 {
   std::shared_ptr<UContainer> cont;
   std::shared_ptr<UStorage> storage(GetStorage().get());
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
        case 0: //������ ����� (��� �������������)
            res&=LinkNeuron(afferentL,"MotoneuronL",0,"Soma"+sntoa(i+1)); // 0 - ������������ �����
			res&=LinkNeuron(afferentR,"MotoneuronR",0,"Soma"+sntoa(i+1));
            res&=LinkNeuron(afferentL,"MotoneuronR",1,"Soma"+sntoa(i+1)); // 1 - ��������� �����
			res&=LinkNeuron(afferentR,"MotoneuronL",1,"Soma"+sntoa(i+1));
			break;

         case 1: //����� ����� ������������ - ��� ����� �������
			if(!(CheckComponent("Post"+afferentL)&&CheckComponent("Post"+afferentR)))
			  return false;
            res&=LinkNeuron(afferentL,"Post"+afferentL,0); //������������ �����
            res&=LinkNeuron("Post"+afferentL,"MotoneuronR",1,"Soma"+sntoa(i+1)); //��������� �����
            res&=LinkNeuron("Post"+afferentL,"MotoneuronL",0,"Soma"+sntoa(i+1)); //������������ �����
            res&=LinkNeuron(afferentR,"Post"+afferentR,0); //������������ �����
            res&=LinkNeuron("Post"+afferentR,"MotoneuronL",1,"Soma"+sntoa(i+1)); //��������� �����
            res&=LinkNeuron("Post"+afferentR,"MotoneuronR",0,"Soma"+sntoa(i+1)); //������������ �����
			break;

         case 2: //����� ����� ������������(L-R) + ������ �����(L-L)
			if(!(CheckComponent("Post"+afferentL)&&CheckComponent("Post"+afferentR)))
			  return false;
			res&=LinkNeuron(afferentL,"MotoneuronL",0,"Soma"+sntoa(i+1));
			res&=LinkNeuron(afferentR,"MotoneuronR",0,"Soma"+sntoa(i+1));
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=LinkNeuron("Post"+afferentL,"MotoneuronR",1,"Soma"+sntoa(i+1));
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			res&=LinkNeuron("Post"+afferentR,"MotoneuronL",1,"Soma"+sntoa(i+1));
			break;

         case 3://����� ����� ������������(L-L) + ������ �����(L-R)
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
        case 0: //������ ����� (��� �������������)
		 res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentL+".LTZone","MotoneuronL", "ExcChannel",branch_name);
			res&=LinkNeuron(afferentL,"MotoneuronR",1,branch_name);
		 res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentR+".LTZone","MotoneuronR", "ExcChannel",branch_name);
			res&=LinkNeuron(afferentR,"MotoneuronL",1,branch_name);
		 break;

         case 1: //����� ����� ������������
			if(!(CheckComponent("Post"+afferentL) && CheckComponent("Post"+afferentR)))
			  return false;
//			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentL+".LTZone","Post"+afferentL,"ExcChannel");
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),"Post"+afferentL+".LTZone","MotoneuronR","InhChannel",branch_name);
			res&=LinkNeuron("Post"+afferentR,"MotoneuronR",0,branch_name);
//			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentR+".LTZone","Post"+afferentR,"ExcChannel");
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),"Post"+afferentR+".LTZone","MotoneuronL","InhChannel",branch_name);
			res&=LinkNeuron("Post"+afferentL,"MotoneuronL",0,branch_name);
			break;

         case 2: //����� ����� ������������(L-R) + ������ �����(L-L)
			if(!(CheckComponent("Post"+afferentL) && CheckComponent("Post"+afferentR)))
			  return false;
			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentL+".LTZone","MotoneuronL","ExcChannel",branch_name);
			res&=LinkNeuron("Post"+afferentR,"MotoneuronL",1,branch_name);
//			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentL+".LTZone","Post"+afferentL,"ExcChannel");
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=LinkNeuron("Post"+afferentL,"MotoneuronR",1,branch_name);
			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentR+".LTZone","MotoneuronR","ExcChannel",branch_name);
//			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentR+".LTZone","Post"+afferentR,"ExcChannel");
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			break;

         case 3: //����� ����� ������������(L-L) + ������ �����(L-R)
			if(!(CheckComponent("Post"+afferentL) && CheckComponent("Post"+afferentR)))
			  return false;
			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentL+".LTZone","MotoneuronR","InhChannel",branch_name);
			res&=LinkNeuron("Post"+afferentR,"MotoneuronR",0,branch_name);
//			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentL+".LTZone","Post"+afferentL,"ExcChannel");
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=LinkNeuron("Post"+afferentL,"MotoneuronL",0,branch_name);
			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentR+".LTZone","MotoneuronL","InhChannel",branch_name);
//			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentR+".LTZone","Post"+afferentR,"ExcChannel");
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			break;

		 case 4:
			if(!(CheckComponent("Post"+afferentL) && CheckComponent("Post"+afferentR)))
			  return false;
//			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentL+".LTZone","Post"+afferentL,"ExcChannel");
//			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),afferentR+".LTZone","Post"+afferentR,"ExcChannel");
			res&=LinkNeuron(afferentL,"Post"+afferentL,0);
			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),"Post"+afferentL+".LTZone","MotoneuronL","ExcChannel");
			res&=LinkNeuron(afferentR,"Post"+afferentR,0);
			res&=CreateNeuronBranchLink(std::static_pointer_cast<UNet>(GetThisAsSharedContainer()),"Post"+afferentR+".LTZone","MotoneuronR","ExcChannel");
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
