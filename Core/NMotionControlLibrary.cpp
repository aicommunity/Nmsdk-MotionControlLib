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
 std::shared_ptr<RDK::UContainer> cont;

 cont=std::make_shared<NDCEngine>();
 cont->SetName("DCEngine");
 cont->Default();
 UploadClass("NDCEngine",cont);

 cont=std::make_shared<NPendulumAndCart>();
 cont->SetName("PendulumAndCart");
 cont->Default();
 UploadClass("NPendulumAndCart",cont);

{
 std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NAstaticGyro>();
 generated_cont->SetName("NAstaticGyro");
 generated_cont->Default();
 UploadClass("NAstaticGyro",generated_cont);
}

{
 std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NManipulatorAndGyro>();
 generated_cont->SetName("NManipulatorAndGyro");
 generated_cont->Default();
 UploadClass("NManipulatorAndGyro",generated_cont);
}



 cont=std::make_shared<NManipulator>();
 cont->SetName("Manipulator");
 cont->Default();
 UploadClass("NManipulator",cont);

 cont=std::make_shared<NNavMousePrimitive>();
 cont->SetName("NavMousePrimitive");
 cont->Default();
 UploadClass("NNavMousePrimitive",cont);



  cont=std::make_shared<NFrequencyReceiver>();
 cont->SetName("FrequencyReceiver");
 cont->Default();
 UploadClass("NFrequencyReceiver",cont);

 cont=std::make_shared<NManipulatorInput>();
 cont->SetName("ManipulatorInput");
 cont->Default();
 UploadClass("NManipulatorInput",cont);

 cont=std::make_shared<NManipulatorInputEmulator>();
 cont->SetName("ManipulatorInputEmulator");
 cont->Default();
 UploadClass("NManipulatorInputEmulator",cont);

 cont=std::make_shared<NSimpleStatistic>();
 cont->SetName("SimpleStatistic");
 cont->Default();
 UploadClass("NSimpleStatistic",cont);
/*
 cont=std::make_shared<NNetworkLinksStatistic>();
 cont->SetName("NetworkLinksStatistic");
 cont->Default();
 UploadClass("NNetworkLinksStatistic",cont);
 */
 cont=std::make_shared<NPulseReceiver>();
 cont->SetName("PulseReceiver");
 cont->Default();
 UploadClass("NPulseReceiver",cont);



 cont=std::make_shared<NSignumSeparator>();
 cont->SetName("SignumSeparator");
 cont->Default();
 UploadClass("NSignumSeparator",cont);

 cont=std::make_shared<NIntervalSeparator>();
 cont->SetName("IntervalSeparator");
 cont->Default();
 UploadClass("NIntervalSeparator",cont);


 // ����������� ����������� ������� �� ������������� � ������������� �����
 cont=dynamic_pointer_cast<RDK::UContainer>(dynamic_cast<UStorage*>(storage)->TakeObject("NSignumSeparator"));
 cont->SetName("PosSignumSeparator");
 vector<double> signum;
 signum.assign(1,1.0);
// signum.assign(1,1.0/(2*M_PI));
 cont->Default();
 dynamic_pointer_cast<NSignumSeparator>(cont)->Sign=signum;
 UploadClass("NPosSignumSeparator",cont);

 cont=dynamic_pointer_cast<RDK::UContainer>(dynamic_cast<UStorage*>(storage)->TakeObject("NSignumSeparator"));
 cont->SetName("NegSignumSeparator");
 signum.assign(1,-1.0);
// signum.assign(1,-1.0/(2*M_PI));
 cont->Default();
 dynamic_pointer_cast<NSignumSeparator>(cont)->Sign=signum;
 UploadClass("NNegSignumSeparator",cont);

  cont=std::make_shared<NEyeRetina>();
 cont->SetName("EyeRetina");
 cont->Default();
 UploadClass("NEyeRetina",cont);


  cont=std::make_shared<NManipulatorSourceEmulator>();
 cont->SetName("ManipulatorSourceEmulator");
 cont->Default();
 UploadClass("NManipulatorSourceEmulator",cont);

 cont=std::make_shared<NManipulatorSource>();
 cont->SetName("ManipulatorSource");
 cont->Default();
 UploadClass("NManipulatorSource",cont);

 cont=std::make_shared<NControlObjectSource>();
 cont->SetName("ManipulatorSource");
 cont->Default();
 UploadClass("NControlObjectSource",cont);


  cont = std::make_shared<NSuppressionUnit>();
 cont->SetName("SuppressionUnit");
 cont->Default();
 UploadClass("NSuppressionUnit", cont);

  cont = std::make_shared<NCounterNeuron>();
 cont->SetName("NCounterNeuron");
 cont->Default();
 UploadClass("NCounterNeuron", cont);

  cont = std::make_shared<NObjInArea>();
 cont->SetName("NObjInArea");
 cont->Default();
 UploadClass("NObjInArea", cont);

  cont = std::make_shared<NSeqComparison>();
 cont->SetName("NSeqComparison");
 cont->Default();
 UploadClass("NSeqComparison", cont);

  cont = std::make_shared<NActuatorSignals>();
 cont->SetName("NActuatorSignals");
 cont->Default();
 UploadClass("NActuatorSignals", cont);

  cont = std::make_shared<NSignalEstimation>();
 cont->SetName("NSignalEstimation");
 cont->Default();
 UploadClass("NSignalEstimation", cont);


 // ������� �� ������������ ��������
  std::shared_ptr<UNet> net=std::make_shared<NMotionElement>();
 net->SetName("MotionElement");
 net->Default();
 UploadClass("NNewMotionElement",net);

 // ������� �������� ������ ����������
 std::shared_ptr<NEngineMotionControl> cs=0;
  cs=std::make_shared<NEngineMotionControl>();
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
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NPositionControlElement>();
 generated_cont->SetName("NPositionControlElement");
 generated_cont->Default();
 UploadClass("NPositionControlElement",generated_cont);
}

{
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NNewPositionControlElement>();
 generated_cont->SetName("NNewPositionControlElement");
 generated_cont->Default();
 UploadClass("NNewPositionControlElement",generated_cont);
}

{
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NMultiPositionControl>();
 generated_cont->SetName("NMultiPositionControl");
 generated_cont->Default();
 UploadClass("NMultiPositionControl",generated_cont);
}

{
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NTrajectoryElement>();
 generated_cont->SetName("NTrajectoryElement");
 generated_cont->Default();
 UploadClass("NTrajectoryElement",generated_cont);
}

{
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NMazeMemory>();
 generated_cont->SetName("NMazeMemory");
 generated_cont->Default();
 UploadClass("NMazeMemory",generated_cont);
}

{
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NMazeMemory_simplified>();
 generated_cont->SetName("NMazeMemory_simplified");
 generated_cont->Default();
 UploadClass("NMazeMemory_simplified",generated_cont);
}

{
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NPCNElement>();
 generated_cont->SetName("NPCN");
 generated_cont->Default();
 UploadClass("NPCN",generated_cont);
}
}
// --------------------------


}
