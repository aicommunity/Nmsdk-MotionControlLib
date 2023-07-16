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
//#include "NPositionControlElement.cpp"
#include "NNewPositionControlElement.cpp"
//#include "NMultiPositionControl.cpp"

namespace NMSDK {

NMotionControlLibrary MotionControlLibrary;

// --------------------------
// ������������ � �����������
// --------------------------
NMotionControlLibrary::NMotionControlLibrary(void)
 : ULibrary("MotionControlLibrary","1.0", GetGlobalVersion())
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

 // ������� �������� ������ ����������
 UEPtr<NEngineMotionControl> cs=0;
 cs=new NEngineMotionControl;
 cs->Default();
 cs->SetName("EngineMotionControl");
 UploadClass("NEngineMotionControl",cs);

 cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
 cs->NumMotionElements=1;
 cs->CreationMode=14;
 cs->MotionElementClassName="NNewMotionElement";
 cs->MCNeuroObjectName="NNewSPNeuron";
 cs->ObjectControlInterfaceClassName="NControlObjectSource";
 cs->Create();
 net=cs;

 net->SetName("EngineControlRangeAfferent");
 UploadClass("N2AsfNewSimplestAfferentBranchedEngineControl",net);


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

//{
// UEPtr<UContainer> generated_cont=new NMultiPositionControl;
// generated_cont->SetName("NMultiPositionControl");
// generated_cont->Default();
// UploadClass("NMultiPositionControl",generated_cont);
//}

//{
// UEPtr<UContainer> generated_cont=new NPCNElement;
// generated_cont->SetName("NPCN");
// generated_cont->Default();
// UploadClass("NPCN",generated_cont);
//}
}
// --------------------------


}
