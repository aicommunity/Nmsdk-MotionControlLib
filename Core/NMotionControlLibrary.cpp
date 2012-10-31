/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2010.
E-mail:            alexab@ailab.ru
url:            http://ailab.ru
Version:        1.0.0

This file - part of the project: NMSDK

File License:        BSD License
Project License:    GNU GPLv3 License
See file license.txt for more information
*********************************************************** */

#include "NMotionControlLibrary.h"

#include "NMotionElement.cpp"
#include "NEngineMotionControl.cpp"

namespace NMSDK {

NMotionControlLibrary MotionControlLibrary;

// --------------------------
// ������������ � �����������
// --------------------------
NMotionControlLibrary::NMotionControlLibrary(void)
 : NLibrary("MotionControlLibrary","1.0")
{
}
// --------------------------

// --------------------------
// ������ �������� ��������� �������
// --------------------------
// --------------------------

// --------------------------
// ������ ����������� �����������
// --------------------------
// ��������� ������ ClassSamples �������� ������������ �������� � �� �������.
// �� ��������� ��������������� ������� ������� � ������ ������.
void NMotionControlLibrary::ACreateClassSamples(NStorage *storage)
{
 // ������� �� ������������ ��������
 UEPtr<NANet> net=CreateMotionElement(storage, "NNet",0);
 net->SetName("MotionElement");
 UploadClass("NMotionElement",net);

 // ������� �� ������������ �������� ��� ���������� �������������
 net=CreateMotionElement(storage, "NNet",1);
 net->SetName("SimpleMotionElement");
 UploadClass("NSimpleMotionElement",net);

 // ������� �� � ����������
 net=CreateBranchedMotionElement(storage, "NNet","NSynSPNeuron","NSAfferentNeuron",0);
 net->SetName("MotionElement");
 UploadClass("NBranchedMotionElement",net);

 // ������� �� � ���������� ��� ���������� �������������
 net=CreateBranchedMotionElement(storage, "NNet","NSynSPNeuron","NSAfferentNeuron",1);
 net->SetName("MotionElement");
 UploadClass("NSimpleBranchedMotionElement",net);

 // ������� �� � ���������� � ������������ ���������
 net=CreateBranchedMotionElement(storage, "NNet","NContinuesSynSPNeuron","NContinuesSAfferentNeuron",0);
 net->SetName("MotionElement");
 UploadClass("NContinuesBranchedMotionElement",net);

 // ������� �� � ���������� � ������������ ��������� ��� ���������� �������������
 net=CreateBranchedMotionElement(storage, "NNet","NContinuesSynSPNeuron","NContinuesSAfferentNeuron",1);
 net->SetName("MotionElement");
 UploadClass("NContinuesSimpleBranchedMotionElement",net);

 // ������� ���������� ��
 net=CreateSimplestMotionElement(storage, "NNet",0);
 net->SetName("MotionElement");
 UploadClass("NSimplestMotionElement",net);

 // ������� ���������� �� � ����������
 net=CreateSimplestBranchedMotionElement(storage, "NNet","NSynSPNeuron","NSAfferentNeuron",0);
 net->SetName("MotionElement");
 UploadClass("NSimplestBranchedMotionElement",net);

 // ������� ���������� �� � ���������� � ������� ����������� ��������
 net=CreateSimplestBranchedMotionElement(storage, "NNet","NSynSPNeuron","NSimpleAfferentNeuron",0);
 net->SetName("MotionElement");
 UploadClass("NSimplestAfferentBranchedMotionElement",net);

 // ������� �������� ������ ����������
 UEPtr<NEngineMotionControl> cs=0;
 cs=new NEngineMotionControl;
 cs->Default();
 cs->SetName("EngineMotionControl");
 UploadClass("NEngineMotionControl",cs);


// pair<real,real> Ia(-2.0*M_PI,2.0*M_PI);
// pair<real,real> Ib(-10,10);
// pair<real,real> II(-M_PI/2,M_PI/2);
 size_t max_number_of_mc=1;

 // ��������� ���� ���������� ���������� � ����������� ��������������� ������ � ��������
 // �� ��� ������ �� �����
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=0;
  cs->MotionElementClassName="NMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlSignumAfferent(storage, "NNet", "NMotionElement",i+1);
  net->SetName("EngineControlSignumAfferent");
  if(i>0)
   UploadClass(string("NEngineControlSignumAfferent")+RDK::sntoa(i+1),net);
  else
   UploadClass(string("NEngineControlSignumAfferent"),net);
 }

 // ��������� ���� ���������� ���������� c ����������� ��������������� ������ � ��������
 // �� ������ �� ���������
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=1;
  cs->MotionElementClassName="NMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(storage, "NNet", "NMotionElement",
//   Ia,Ib,II,i+1);
  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NEngineControlRangeAfferent")+RDK::sntoa(i+1),net);
  else
   UploadClass("NEngineControlRangeAfferent",net);
 }

 // ��������� ���� ���������� ���������� � ����������� ��������������� ������ � ��������
 // �� ��� ������ �� �����
 // ������������ ������������ ������� ��� ���������� �������������
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=0;
  cs->MotionElementClassName="NSimpleMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlSignumAfferent(storage, "NNet", "NSimpleMotionElement",i+1);
  net->SetName("SimpleEngineControlSignumAfferent");
  if(i>0)
   UploadClass(string("NSimpleEngineControlSignumAfferent")+RDK::sntoa(i+1),net);
  else
   UploadClass("NSimpleEngineControlSignumAfferent",net);
 }

 // ��������� ���� ���������� ���������� c ����������� ��������������� ������ � ��������
 // �� ������ �� ���������
 // ������������ ������������ ������� ��� ���������� �������������
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=1;
  cs->MotionElementClassName="NSimpleMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(storage, "NNet", "NSimpleMotionElement",
//  Ia,Ib,II,i+1);
  net->SetName("SimpleEngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NSimpleEngineControlRangeAfferent")+RDK::sntoa(i+1),net);
  else
   UploadClass("NSimpleEngineControlRangeAfferent",net);
 }


 // ��������� ���� ���������� ���������� c ����������� ��������������� ������ � ��������
 // �� ������ �� ���������
 // �������������� ������������ ��������
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=2;
  cs->MotionElementClassName="NBranchedMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(storage, "NNet", "NBranchedMotionElement",
//   Ia,Ib,II,i+1);
  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NBranchedEngineControlRangeAfferent")+RDK::sntoa(i+1),net);
  else
   UploadClass("NBranchedEngineControlRangeAfferent",net);
 }

 // ��������� ���� ���������� ���������� c ����������� ��������������� ������ � ��������
 // �� ������ �� ���������
 // ������������ ������������ ������� ��� ���������� �������������
 // �������������� ������������ ��������
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=2;
  cs->MotionElementClassName="NSimpleBranchedMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(storage, "NNet", "NSimpleBranchedMotionElement",
//  Ia,Ib,II,i+1);
  net->SetName("SimpleEngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NSimpleBranchedEngineControlRangeAfferent")+RDK::sntoa(i+1),net);
  else
   UploadClass("NSimpleBranchedEngineControlRangeAfferent",net);
 }

 // ��������� ���� ���������� ���������� c ����������� ��������������� ������ � ��������
 // �� ������ �� ���������
 // �������������� ������������ ��������
 // ������������ �����
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=3;
  cs->MotionElementClassName="NBranchedMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(storage, "NNet", "NBranchedMotionElement",
//   Ia,Ib,II,i+1,true);
  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NBranchedCrosslinksEngineControlRangeAfferent")+RDK::sntoa(i+1),net);
  else
   UploadClass("NBranchedCrosslinksEngineControlRangeAfferent",net);
 }

 // ��������� ���� ���������� ���������� c ����������� ��������������� ������ � ��������
 // �� ������ �� ���������
 // ������������ ������������ ������� ��� ���������� �������������
 // �������������� ������������ ��������
 // ������������ �����
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=3;
  cs->MotionElementClassName="NSimpleBranchedMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(storage, "NNet", "NSimpleBranchedMotionElement",
//  Ia,Ib,II,i+1,true);
  net->SetName("SimpleEngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NSimpleBranchedCrosslinksEngineControlRangeAfferent")+RDK::sntoa(i+1),net);
  else
   UploadClass("NSimpleBranchedCrosslinksEngineControlRangeAfferent",net);
 }


 // ��������� ���� ���������� ���������� c ����������� ��������������� ������ � ��������
 // �� ������ �� ���������
 // �������������� ������������ ��������
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=4;
  cs->MotionElementClassName="NBranchedMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(storage, "NNet", "NBranchedMotionElement",
//   Ia,Ib,II,i+1,false,false);
  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NIndRangeBranchedEngineControlRangeAfferent")+RDK::sntoa(i+1),net);
  else
   UploadClass("NIndRangeBranchedEngineControlRangeAfferent",net);
 }

 // ��������� ���� ���������� ���������� c ����������� ��������������� ������ � ��������
 // �� ������ �� ��������� ��� �������������� ����������
 // �������������� ������������ ��������
 // ������������ �����
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=5;
  cs->MotionElementClassName="NBranchedMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(storage, "NNet", "NBranchedMotionElement",
//   Ia,Ib,II,i+1,true,false);
  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NIndRangeBranchedCrosslinksEngineControlRangeAfferent")+RDK::sntoa(i+1),net);
  else
   UploadClass("NIndRangeBranchedCrosslinksEngineControlRangeAfferent",net);
 }

 // ��������� ���� ���������� ���������� c ����������� ��������������� ������ � ��������
 // �� ������ �� ��������� ��� �������������� ����������
 // �������������� ������������ ��������
 // ������������ �����
 // ������� � ����������� ���������� ��������
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=10;
  cs->MotionElementClassName="NContinuesBranchedMotionElement";
  cs->Create();
  net=cs;

  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NContinuesIndRangeBranchedCrosslinksEngineControlRangeAfferent")+RDK::sntoa(i+1),net);
  else
   UploadClass("NContinuesIndRangeBranchedCrosslinksEngineControlRangeAfferent",net);
 }

 // ��������� ���� ���������� ���������� c ���������� ������� ��
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=11;
  cs->MotionElementClassName="NSimplestMotionElement";
  cs->Create();
  net=cs;

  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NSimplestEngineControl")+RDK::sntoa(i+1),net);
  else
   UploadClass("NSimplestEngineControl",net);
 }


 // ��������� ���� ���������� ���������� c ���������� ������� ��
 // ��������� ���������
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=11;
  cs->MotionElementClassName="NSimplestBranchedMotionElement";
  cs->Create();
  net=cs;

  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NSimplestBranchedEngineControl")+RDK::sntoa(i+1),net);
  else
   UploadClass("NSimplestBranchedEngineControl",net);
 }

 // ��������� ���� ���������� ���������� c ���������� ������� ��
 // � ����������� ����������
 // ��������� ���������
 max_number_of_mc=10;
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(storage->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=11;
  cs->MotionElementClassName="NSimplestAfferentBranchedMotionElement";
  cs->Create();
  net=cs;

  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NSimplestAfferentBranchedEngineControl")+RDK::sntoa(i+1),net);
  else
   UploadClass("NSimplestAfferentBranchedEngineControl",net);
 }




}
// --------------------------


}
