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
/*
#include "NMotionElement.cpp"
#include "NEngineMotionControl.cpp"
#include "NPositionControlElement.cpp"
#include "NNewPositionControlElement.cpp"
#include "NMultiPositionControl.cpp"
*/
namespace NMSDK {

NMotionControlLibrary MotionControlLibrary;

// --------------------------
// ������������ � �����������
// --------------------------
NMotionControlLibrary::NMotionControlLibrary(void)
 : ULibrary("MotionControlLibrary","1.0")
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
void NMotionControlLibrary::CreateClassSamples(UStorage *storage)
{
 // ������� �� ������������ ��������
 UEPtr<UNet> net=new NMotionElement;
 net->SetName("MotionElement");
 net->Default();
 UploadClass("NNewMotionElement",net);

 // ������� �� ������������ ��������
 net=CreateMotionElement(dynamic_cast<UStorage*>(storage), "NNet",0);
 net->SetName("MotionElement");
 UploadClass("NMotionElement",net);

 // ������� �� ������������ �������� ��� ���������� �������������
 net=CreateMotionElement(dynamic_cast<UStorage*>(storage), "NNet",1);
 net->SetName("SimpleMotionElement");
 UploadClass("NSimpleMotionElement",net);

 // ������� �� � ����������
 net=CreateBranchedMotionElement(dynamic_cast<UStorage*>(storage), "NNet","NSynSPNeuron","NSAfferentNeuron",0);
 net->SetName("MotionElement");
 UploadClass("NBranchedMotionElement",net);

 // ������� �� � ���������� ��� ���������� �������������
 net=CreateBranchedMotionElement(dynamic_cast<UStorage*>(storage), "NNet","NSynSPNeuron","NSAfferentNeuron",1);
 net->SetName("MotionElement");
 UploadClass("NSimpleBranchedMotionElement",net);

 // ������� �� � ���������� � ������������ ���������
 net=CreateBranchedMotionElement(dynamic_cast<UStorage*>(storage), "NNet","NContinuesSynSPNeuron","NContinuesSAfferentNeuron",0);
 net->SetName("MotionElement");
 UploadClass("NContinuesBranchedMotionElement",net);

 // ������� �� � ���������� � ������������ ��������� ��� ���������� �������������
 net=CreateBranchedMotionElement(dynamic_cast<UStorage*>(storage), "NNet","NContinuesSynSPNeuron","NContinuesSAfferentNeuron",1);
 net->SetName("MotionElement");
 UploadClass("NContinuesSimpleBranchedMotionElement",net);

 // ������� ���������� ��
 net=CreateSimplestMotionElement(dynamic_cast<UStorage*>(storage), "NNet",0);
 net->SetName("MotionElement");
 UploadClass("NSimplestMotionElement",net);

 // ������� ���������� �� � ����������
 net=CreateSimplestBranchedMotionElement(dynamic_cast<UStorage*>(storage), "NNet","NSynSPNeuron","NSAfferentNeuron",0,false,false);
 net->SetName("MotionElement");
 UploadClass("NSimplestBranchedMotionElement",net);

 // ������� ���������� �� � ���������� � ������� ����������� ��������
 net=CreateSimplestBranchedMotionElement(dynamic_cast<UStorage*>(storage), "NNet","NSynSPNeuron","NSimpleAfferentNeuron",0,false,false);
 net->SetName("MotionElement");
 UploadClass("NSimplestAfferentBranchedMotionElement",net);

 // ������� ���������� �� � ����������, ������� ����������� �������� � ����������� ������� ������� � ���������� �������� �������� �� ����
 net=CreateSimplestBranchedMotionElement(dynamic_cast<UStorage*>(storage), "NNet","NNewSynSPNeuron","NSimpleAfferentNeuron",0,false,false);
 net->SetName("MotionElement");
 UploadClass("NNewSimplestAfferentBranchedMotionElement",net);

 // ������� ���������� �� � ����������, ������� ����������� �������� � ����������� ������� ������� � ���������� �������� �������� �� ����
 // � � ������������ ��������
 net=CreateSimplestBranchedMotionElementPM(dynamic_cast<UStorage*>(storage), "NNet","NNewSynSPNeuron","NSimpleAfferentNeuron",0,false,false);
 net->SetName("MotionElement");
 UploadClass("NNewSimplestAfferentBranchedMotionElementPM",net);

 // ������� ���������� �� � ����������, ������� ����������� �������� � ������� ������� �������, �� 3 ����������� ������
 net=CreateSimplestBranchedMotionElement(dynamic_cast<UStorage*>(storage), "NNet","NSynSPNeuron","NSimpleAfferentNeuron",0,true,false);
 net->SetName("MotionElement");
 UploadClass("NAsfSimplestAfferentBranchedMotionElement",net);

 // ������� ���������� �� � ����������, ������� ����������� �������� � ����������� ������� ������� � ���������� �������� �������� �� ����, �� 3 ����������� ������
 net=CreateSimplestBranchedMotionElement(dynamic_cast<UStorage*>(storage), "NNet","NNewSynSPNeuron","NSimpleAfferentNeuron",0,true,false);
 net->SetName("MotionElement");
 UploadClass("NAsfNewSimplestAfferentBranchedMotionElement",net);

 // ������� ���������� �� � ����������, ������� ����������� �������� � ������� ������� �������, �� 3 ����������� ������
 // � � ������������ ��������
 net=CreateSimplestBranchedMotionElementPM(dynamic_cast<UStorage*>(storage), "NNet","NSynSPNeuron","NSimpleAfferentNeuron",0,true,false);
 net->SetName("MotionElement");
 UploadClass("NAsfSimplestAfferentBranchedMotionElementPM",net);

 // ������� ���������� �� � ����������, ������� ����������� �������� � ����������� ������� ������� � ���������� �������� �������� �� ����, �� 3 ����������� ������
 // � � ������������ ��������
 net=CreateSimplestBranchedMotionElementPM(dynamic_cast<UStorage*>(storage), "NNet","NNewSynSPNeuron","NSimpleAfferentNeuron",0,true,false);
 net->SetName("MotionElement");
 UploadClass("NAsfNewSimplestAfferentBranchedMotionElementPM",net);

 // ������� ���������� �� � ����������, ������� ����������� �������� � ������� ������� �������, �� 4 ����������� ������
 net=CreateSimplestBranchedMotionElement(dynamic_cast<UStorage*>(storage), "NNet","NSynSPNeuron","NSimpleAfferentNeuron",0,true,true);
 net->SetName("MotionElement");
 UploadClass("NAslsSimplestAfferentBranchedMotionElement",net);

 // ������� ���������� �� � ����������, ������� ����������� �������� � ����������� ������� ������� � ���������� �������� �������� �� ����, �� 4 ����������� ������
 net=CreateSimplestBranchedMotionElement(dynamic_cast<UStorage*>(storage), "NNet","NNewSynSPNeuron","NSimpleAfferentNeuron",0,true,true);
 net->SetName("MotionElement");
 UploadClass("NAslsNewSimplestAfferentBranchedMotionElement",net);

 // ������� ���������� �� � ����������, ������� ����������� �������� � ������� ������� �������, �� 4 ����������� ������
 // � � ������������ ��������
 net=CreateSimplestBranchedMotionElementPM(dynamic_cast<UStorage*>(storage), "NNet","NSynSPNeuron","NSimpleAfferentNeuron",0,true,true);
 net->SetName("MotionElement");
 UploadClass("NAslsSimplestAfferentBranchedMotionElementPM",net);

 // ������� ���������� �� � ����������, ������� ����������� �������� � ����������� ������� ������� � ���������� �������� �������� �� ����, �� 4 ����������� ������
 // � � ������������ ��������
 net=CreateSimplestBranchedMotionElementPM(dynamic_cast<UStorage*>(storage), "NNet","NNewSynSPNeuron","NSimpleAfferentNeuron",0,true,true);
 net->SetName("MotionElement");
 UploadClass("NAslsNewSimplestAfferentBranchedMotionElementPM",net);

 // ������� �������� ������ ����������
 UEPtr<NEngineMotionControl> cs=0;
 cs=new NEngineMotionControl;
 cs->Default();
 cs->SetName("EngineMotionControl");
 UploadClass("NEngineMotionControl",cs);


// pair<double,double> Ia(-2.0*M_PI,2.0*M_PI);
// pair<double,double> Ib(-10,10);
// pair<double,double> II(-M_PI/2,M_PI/2);
 size_t max_number_of_mc=1;

 // ��������� ���� ���������� ���������� � ����������� ��������������� ������ � ��������
 // �� ��� ������ �� �����
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=0;
  cs->MotionElementClassName="NMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlSignumAfferent(dynamic_cast<UStorage*>(storage), "NNet", "NMotionElement",i+1);
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
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=1;
  cs->MotionElementClassName="NMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(dynamic_cast<UStorage*>(storage), "NNet", "NMotionElement",
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
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=0;
  cs->MotionElementClassName="NSimpleMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlSignumAfferent(dynamic_cast<UStorage*>(storage), "NNet", "NSimpleMotionElement",i+1);
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
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=1;
  cs->MotionElementClassName="NSimpleMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(dynamic_cast<UStorage*>(storage), "NNet", "NSimpleMotionElement",
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
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=2;
  cs->MotionElementClassName="NBranchedMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(dynamic_cast<UStorage*>(storage), "NNet", "NBranchedMotionElement",
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
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=2;
  cs->MotionElementClassName="NSimpleBranchedMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(dynamic_cast<UStorage*>(storage), "NNet", "NSimpleBranchedMotionElement",
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
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=3;
  cs->MotionElementClassName="NBranchedMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(dynamic_cast<UStorage*>(storage), "NNet", "NBranchedMotionElement",
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
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=3;
  cs->MotionElementClassName="NSimpleBranchedMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(dynamic_cast<UStorage*>(storage), "NNet", "NSimpleBranchedMotionElement",
//  Ia,Ib,II,i+1,true);
  net->SetName("SimpleEngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NSimpleBranchedCrosslinksEngineControlRangeAfferent")+RDK::sntoa(i+1),net);
  else
   UploadClass("NSimpleBranchedCrosslinksEngineControlRangeAfferent",net);
 }


 // ��������� ���� ���������� ���������� c ����������� ��������������� ������ � ��������
 // �� ������ �� ��������� ��� ����������� ����������
 // �������������� ������������ ��������
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=4;
  cs->MotionElementClassName="NBranchedMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(dynamic_cast<UStorage*>(storage), "NNet", "NBranchedMotionElement",
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
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=5;
  cs->MotionElementClassName="NBranchedMotionElement";
  cs->Create();
  net=cs;

//  net=CreateEngineControlRangeAfferent(dynamic_cast<UStorage*>(storage), "NNet", "NBranchedMotionElement",
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
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
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
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
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
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
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
 // � ����������� �����������
 // ��������� ���������
// max_number_of_mc=1;
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
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

 // ��������� ���� ���������� ���������� c ���������� ������� ��
 // � ����������� �����������
 // ��������� ���������
 // ����� ������ ������� � ���������� �����
// max_number_of_mc=1;
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=11;
  cs->MotionElementClassName="NNewSimplestAfferentBranchedMotionElement";
  cs->Create();
  net=cs;

  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NNewSimplestAfferentBranchedEngineControl")+RDK::sntoa(i+1),net);
  else
   UploadClass("NNewSimplestAfferentBranchedEngineControl",net);
 }


 // ��������� ���� ���������� ���������� c ���������� ������� ��
 // � ����������� �����������
 // ��������� ���������
 // ����� ������ ������� � ���������� �����
 // 3 ����������� ������
 int temp=max_number_of_mc;
 max_number_of_mc=3;
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=12;
  cs->MotionElementClassName="NAsfNewSimplestAfferentBranchedMotionElement";
  cs->Create();
  net=cs;

  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NAsfNewSimplestAfferentBranchedEngineControl")+RDK::sntoa(i+1),net);
  else
   UploadClass("NAsfNewSimplestAfferentBranchedEngineControl",net);
 }
 max_number_of_mc=temp;

 // ��������� ���� ���������� ���������� c ���������� ������� ��
 // � ����������� �����������
 // ��������� ���������
 // 3 ����������� ������
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=12;
  cs->MotionElementClassName="NAsfSimplestAfferentBranchedMotionElement";
  cs->Create();
  net=cs;

  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NAsfSimplestAfferentBranchedEngineControl")+RDK::sntoa(i+1),net);
  else
   UploadClass("NAsfSimplestAfferentBranchedEngineControl",net);
 }

 // ��������� ���� ���������� ���������� c ���������� ������� ��
 // � ����������� �����������
 // ��������� ���������
 // ����� ������ ������� � ���������� �����
 // 3 ����������� ������
 // ������������ ������� � ��
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=12;
  cs->MotionElementClassName="NAsfNewSimplestAfferentBranchedMotionElementPM";
  cs->Create();
  net=cs;

  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NAsfNewSimplestAfferentBranchedEngineControlPM")+RDK::sntoa(i+1),net);
  else
   UploadClass("NAsfNewSimplestAfferentBranchedEngineControlPM",net);
 }

 // ��������� ���� ���������� ���������� c ���������� ������� ��
 // � ����������� �����������
 // ��������� ���������
 // 3 ����������� ������
 // ������������ ������� � ��
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=12;
  cs->MotionElementClassName="NAsfSimplestAfferentBranchedMotionElementPM";
  cs->Create();
  net=cs;

  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NAsfSimplestAfferentBranchedEngineControlPM")+RDK::sntoa(i+1),net);
  else
   UploadClass("NAsfSimplestAfferentBranchedEngineControlPM",net);
 }

 // ��������� ���� ���������� ���������� c ���������� ������� ��
 // � ����������� �����������
 // ��������� ���������
 // ����� ������ ������� � ���������� �����
 // 4 ����������� ������
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=13;
  cs->MotionElementClassName="NAslsNewSimplestAfferentBranchedMotionElement";
  cs->Create();
  net=cs;

  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NAslsNewSimplestAfferentBranchedEngineControl")+RDK::sntoa(i+1),net);
  else
   UploadClass("NAslsNewSimplestAfferentBranchedEngineControl",net);
 }

  // ��������� ���� ���������� ���������� c ���������� ������� ��
 // � ����������� �����������
 // ��������� ���������
 // 4 ����������� ������
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=13;
  cs->MotionElementClassName="NAslsSimplestAfferentBranchedMotionElement";
  cs->Create();
  net=cs;

  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NAslsSimplestAfferentBranchedEngineControl")+RDK::sntoa(i+1),net);
  else
   UploadClass("NAslsSimplestAfferentBranchedEngineControl",net);
 }

 // ��������� ���� ���������� ���������� c ���������� ������� ��
 // � ����������� �����������
 // ��������� ���������
 // ����� ������ ������� � ���������� �����
 // 4 ����������� ������
 // ������������ ������� � ��
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=13;
  cs->MotionElementClassName="NAslsNewSimplestAfferentBranchedMotionElementPM";
  cs->Create();
  net=cs;

  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NAslsNewSimplestAfferentBranchedEngineControlPM")+RDK::sntoa(i+1),net);
  else
   UploadClass("NAslsNewSimplestAfferentBranchedEngineControlPM",net);
 }

 // ��������� ���� ���������� ���������� c ���������� ������� ��
 // � ����������� �����������
 // ��������� ���������
 // 4 ����������� ������
 // ������������ ������� � ��
 for(size_t i=0;i<max_number_of_mc;i++)
 {
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  cs->NumMotionElements=i+1;
  cs->CreationMode=13;
  cs->MotionElementClassName="NAslsSimplestAfferentBranchedMotionElementPM";
  cs->Create();
  net=cs;

  net->SetName("EngineControlRangeAfferent");
  if(i>0)
   UploadClass(string("NAslsSimplestAfferentBranchedEngineControlPM")+RDK::sntoa(i+1),net);
  else
   UploadClass("NAslsSimplestAfferentBranchedEngineControlPM",net);
 }

		 for(size_t i=0;i<max_number_of_mc;i++)
		 {
		  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
		  cs->NumMotionElements=i+1;
		  cs->CreationMode=14;
		  cs->MotionElementClassName="NNewMotionElement";
		  cs->ObjectControlInterfaceClassName="NControlObjectSource";
		  cs->Create();
		  net=cs;

		  net->SetName("EngineControlRangeAfferent");
		  if(i>0)
		   UploadClass(string("N2AsfNewSimplestAfferentBranchedEngineControl")+RDK::sntoa(i+1),net);
		  else
		   UploadClass("N2AsfNewSimplestAfferentBranchedEngineControl",net);
		 }

		 for(size_t i=0;i<max_number_of_mc;i++)
		 {
		  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
		  cs->NumMotionElements=i+1;
		  cs->CreationMode=14;
		  cs->MotionElementClassName="NNewMotionElement";
		  cs->MCNeuroObjectName="NSynSPNeuron";
		  cs->ObjectControlInterfaceClassName="NControlObjectSource";
		  cs->Create();
		  net=cs;

		  net->SetName("EngineControlRangeAfferent");
		  if(i>0)
		   UploadClass(string("N2AsfSimplestAfferentBranchedEngineControl")+RDK::sntoa(i+1),net);
		  else
		   UploadClass("N2AsfSimplestAfferentBranchedEngineControl",net);
		 }

{
 UEPtr<UContainer> generated_cont=new NPositionControlElement;
 generated_cont->SetName("NPositionControlElement");
 generated_cont->Default();
 UploadClass("NPositionControlElement",generated_cont);
}
{
 UEPtr<UContainer> generated_cont=new NNewPositionControlElement;
 generated_cont->SetName("NNewPositionControlElement");
 generated_cont->Default();
 UploadClass("NNewPositionControlElement",generated_cont);
}

{
 UEPtr<UContainer> generated_cont=new NMultiPositionControl;
 generated_cont->SetName("NMultiPositionControl");
 generated_cont->Default();
 UploadClass("NMultiPositionControl",generated_cont);
}

{
 UEPtr<UContainer> generated_cont=new NPCNElement;
 generated_cont->SetName("NPCN");
 generated_cont->Default();
 UploadClass("NPCN",generated_cont);
}
}
// --------------------------


}
