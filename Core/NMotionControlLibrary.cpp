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

//#include "NMotionElement.cpp"
//#include "NEngineMotionControl.cpp"
//#include "NPositionControlElement.cpp"
//#include "NNewPositionControlElement.cpp"
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

 // ����� ������������ ����

 // 1 - Range
 // 2 - Branched Range
 // 3 - Branched Range Crosslinks
 // 4 - Branched Ind. Range
 // 5 - Branched Ind. Range Crosslinks
 // 10 - Branched Ind. Range Continues LTZone neurons
 // 11 - Simplest 2 neuron model
 // 12 - As 11 but speed and force control added
 // 13 - As 12 but additional control contour added

 // 0 - Signum
 cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
 cs->NumMotionElements=1;
 cs->CreationMode=0;
 cs->MotionElementClassName="NNewMotionElement";
 cs->MCNeuroObjectName="NNewSPNeuron";
 cs->ObjectControlInterfaceClassName="NControlObjectSource";
 cs->Create();
 net=cs;
 net->SetName("EngineControlSignumAfferent");
 UploadClass("NEngineControlSignumAfferent",net);

 // 1 - Range
 cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
 cs->NumMotionElements=1;
 cs->CreationMode=0;
 cs->MotionElementClassName="NNewMotionElement";
 cs->MCNeuroObjectName="NNewSPNeuron";
 cs->ObjectControlInterfaceClassName="NControlObjectSource";
 cs->Create();
 net=cs;
 net->SetName("EngineControlRangeAfferent");
 UploadClass("NEngineControlRangeAfferent",net);

 // 14 - New net with parametric structure control
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
