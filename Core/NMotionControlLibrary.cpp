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
#include <glog/logging.h>

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
 try {
  std::shared_ptr<RDK::UContainer> cont;

 cont=std::make_shared<NDCEngine>();
 cont->SetStorage(storage);
 cont->SetName("DCEngine");
 cont->Default();
 UploadClass("NDCEngine",cont);

 cont=std::make_shared<NPendulumAndCart>();
 cont->SetStorage(storage);
 cont->SetName("PendulumAndCart");
 cont->Default();
 UploadClass("NPendulumAndCart",cont);

{
 std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NAstaticGyro>();
 generated_cont->SetStorage(storage);
 cont->SetStorage(storage);
 generated_cont->SetName("NAstaticGyro");
 generated_cont->Default();
 UploadClass("NAstaticGyro",generated_cont);
}

{
 std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NManipulatorAndGyro>();
 generated_cont->SetStorage(storage);
 cont->SetStorage(storage);
 generated_cont->SetName("NManipulatorAndGyro");
 generated_cont->Default();
 UploadClass("NManipulatorAndGyro",generated_cont);
}



 cont=std::make_shared<NManipulator>();
 cont->SetStorage(storage);
 cont->SetName("Manipulator");
 cont->Default();
 UploadClass("NManipulator",cont);

 cont=std::make_shared<NNavMousePrimitive>();
 cont->SetStorage(storage);
 cont->SetName("NavMousePrimitive");
 cont->Default();
 UploadClass("NNavMousePrimitive",cont);



  cont=std::make_shared<NFrequencyReceiver>();
 cont->SetStorage(storage);
 cont->SetName("FrequencyReceiver");
 cont->Default();
 UploadClass("NFrequencyReceiver",cont);

 cont=std::make_shared<NManipulatorInput>();
 cont->SetStorage(storage);
 cont->SetName("ManipulatorInput");
 cont->Default();
 UploadClass("NManipulatorInput",cont);

 cont=std::make_shared<NManipulatorInputEmulator>();
 cont->SetStorage(storage);
 cont->SetName("ManipulatorInputEmulator");
 cont->Default();
 UploadClass("NManipulatorInputEmulator",cont);

 cont=std::make_shared<NSimpleStatistic>();
 cont->SetStorage(storage);
 cont->SetName("SimpleStatistic");
 cont->Default();
 UploadClass("NSimpleStatistic",cont);
/*
 cont=std::make_shared<NNetworkLinksStatistic>();
 cont->SetStorage(storage);
 cont->SetName("NetworkLinksStatistic");
 cont->Default();
 UploadClass("NNetworkLinksStatistic",cont);
 */
 cont=std::make_shared<NPulseReceiver>();
 cont->SetStorage(storage);
 cont->SetName("PulseReceiver");
 cont->Default();
 UploadClass("NPulseReceiver",cont);



 cont=std::make_shared<NSignumSeparator>();
 cont->SetStorage(storage);
 cont->SetName("SignumSeparator");
 cont->Default();
 UploadClass("NSignumSeparator",cont);

 cont=std::make_shared<NIntervalSeparator>();
 cont->SetStorage(storage);
 cont->SetName("IntervalSeparator");
 cont->Default();
 UploadClass("NIntervalSeparator",cont);


 // ����������� ����������� ������� �� ������������� � ������������� �����
 cont=dynamic_pointer_cast<RDK::UContainer>(dynamic_cast<UStorage*>(storage)->TakeObject("NSignumSeparator"));
 if(cont)
 {
  cont->SetName("PosSignumSeparator");
  vector<double> signum;
  signum.assign(1,1.0);
// signum.assign(1,1.0/(2*M_PI));
  cont->Default();
  dynamic_pointer_cast<NSignumSeparator>(cont)->Sign=signum;
  // Remove from ObjectsStorage before using as prototype - PopObject removes object from Storage
  storage->PopObject(cont);
  UploadClass("NPosSignumSeparator",cont);
  
  // IMPORTANT: After UploadClass, factory holds shared_ptr to cont
  // We need to keep cont alive until function ends, or reset it explicitly
  // to avoid destroying object that factory still uses
  // Reset cont to release local reference - factory still holds the object
  LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - after UploadClass NPosSignumSeparator, cont use_count=" << cont.use_count();
  cont.reset(); // Release local reference - factory still holds the object
  LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - after reset cont for NPosSignumSeparator";
 }

 cont=dynamic_pointer_cast<RDK::UContainer>(dynamic_cast<UStorage*>(storage)->TakeObject("NSignumSeparator"));
 if(cont)
 {
  cont->SetName("NegSignumSeparator");
  vector<double> signum;
  signum.assign(1,-1.0);
// signum.assign(1,-1.0/(2*M_PI));
  cont->Default();
  dynamic_pointer_cast<NSignumSeparator>(cont)->Sign=signum;
  storage->PopObject(cont);
  UploadClass("NNegSignumSeparator",cont);
  
  // IMPORTANT: After UploadClass, factory holds shared_ptr to cont
  // Reset cont to release local reference - factory still holds the object
  LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - after UploadClass NNegSignumSeparator, cont use_count=" << cont.use_count();
  cont.reset(); // Release local reference - factory still holds the object
  LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - after reset cont for NNegSignumSeparator";
 }

  cont=std::make_shared<NEyeRetina>();
 cont->SetStorage(storage);
 cont->SetName("EyeRetina");
 cont->Default();
 UploadClass("NEyeRetina",cont);


  cont=std::make_shared<NManipulatorSourceEmulator>();
 cont->SetStorage(storage);
 cont->SetName("ManipulatorSourceEmulator");
 cont->Default();
 UploadClass("NManipulatorSourceEmulator",cont);

 cont=std::make_shared<NManipulatorSource>();
 cont->SetStorage(storage);
 cont->SetName("ManipulatorSource");
 cont->Default();
 UploadClass("NManipulatorSource",cont);

 cont=std::make_shared<NControlObjectSource>();
 cont->SetStorage(storage);
 cont->SetName("ManipulatorSource");
 cont->Default();
 UploadClass("NControlObjectSource",cont);


  cont = std::make_shared<NSuppressionUnit>();
 cont->SetName("SuppressionUnit");
 // IMPORTANT: Set Storage BEFORE Default() because Default() calls Build() which calls ABuild()
 // which calls AddMissingComponent() which needs Storage
 cont->SetStorage(storage);
 cont->Default();
 LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - before UploadClass NSuppressionUnit";
 UploadClass("NSuppressionUnit", cont);
 LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - after UploadClass NSuppressionUnit";

  LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - before creating NCounterNeuron";
  try {
   cont = std::make_shared<NCounterNeuron>();
  } catch (const std::bad_weak_ptr& e) {
   LOG(ERROR) << "NMotionControlLibrary::CreateClassSamples - bad_weak_ptr in make_shared<NCounterNeuron>: " << e.what();
   throw; // Re-throw to be caught by outer catch
  }
 cont->SetName("NCounterNeuron");
 cont->SetStorage(storage);
 cont->Default();
 UploadClass("NCounterNeuron", cont);

  cont = std::make_shared<NObjInArea>();
 cont->SetName("NObjInArea");
 cont->SetStorage(storage);
 cont->Default();
 UploadClass("NObjInArea", cont);

  cont = std::make_shared<NSeqComparison>();
 cont->SetName("NSeqComparison");
 cont->SetStorage(storage);
 cont->Default();
 UploadClass("NSeqComparison", cont);

  cont = std::make_shared<NActuatorSignals>();
 cont->SetName("NActuatorSignals");
 cont->SetStorage(storage);
 cont->Default();
 UploadClass("NActuatorSignals", cont);

  cont = std::make_shared<NSignalEstimation>();
 cont->SetName("NSignalEstimation");
 cont->SetStorage(storage);
 cont->Default();
 UploadClass("NSignalEstimation", cont);


 // ������� �� ������������ ��������
  std::shared_ptr<UNet> net=std::make_shared<NMotionElement>();
 net->SetName("MotionElement");
 net->SetStorage(storage);
 net->Default();
 UploadClass("NNewMotionElement",net);

 // ������� �������� ������ ����������
 LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - before creating NEngineMotionControl";
 std::shared_ptr<NEngineMotionControl> cs=0;
  cs=std::make_shared<NEngineMotionControl>();
 cs->SetStorage(storage);
 cs->SetName("EngineMotionControl");
 cs->Default();
 LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - before UploadClass NEngineMotionControl";
 UploadClass("NEngineMotionControl",cs);
 LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - after UploadClass NEngineMotionControl";

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
 LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - creating NEngineControlSignumAfferent";
 try {
  cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
  if(!cs) {
   LOG(ERROR) << "NMotionControlLibrary::CreateClassSamples - TakeObject returned nullptr for NEngineMotionControl";
   throw std::runtime_error("TakeObject returned nullptr");
  }
  cs->NumMotionElements=1;
  cs->CreationMode=0;
  cs->MotionElementClassName="NNewMotionElement";
  cs->MCNeuroObjectName="NNewSPNeuron";
  cs->ObjectControlInterfaceClassName="NControlObjectSource";
  LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - calling cs->Create() for NEngineControlSignumAfferent";
  try {
   cs->Create();
  } catch (const std::bad_weak_ptr& e) {
   LOG(ERROR) << "NMotionControlLibrary::CreateClassSamples - bad_weak_ptr in cs->Create() for NEngineControlSignumAfferent: " << e.what();
   throw; // Re-throw to be caught by outer catch
  }
 } catch (const std::bad_weak_ptr& e) {
  LOG(ERROR) << "NMotionControlLibrary::CreateClassSamples - bad_weak_ptr exception in NEngineControlSignumAfferent creation: " << e.what();
  throw; // Re-throw to be caught by outer catch
 }
 net=cs;
 net->SetName("EngineControlSignumAfferent");
 // Remove from ObjectsStorage before using as prototype
 // Child components created via Create() remain in Storage, which is fine
 storage->PopObject(net);
 LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - calling UploadClass for NEngineControlSignumAfferent";
 UploadClass("NEngineControlSignumAfferent",net);

 // 1 - Range
 cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
 if(cs)
 {
  cs->NumMotionElements=1;
  cs->CreationMode=0;
  cs->MotionElementClassName="NNewMotionElement";
  cs->MCNeuroObjectName="NNewSPNeuron";
  cs->ObjectControlInterfaceClassName="NControlObjectSource";
  cs->Create();
  net=cs;
  net->SetName("EngineControlRangeAfferent");
  storage->PopObject(net);
  UploadClass("NEngineControlRangeAfferent",net);
 }

 // 14 - New net with parametric structure control
 cs=dynamic_pointer_cast<NEngineMotionControl>(dynamic_cast<UStorage*>(storage)->TakeObject("NEngineMotionControl"));
 if(cs)
 {
  cs->NumMotionElements=1;
  cs->CreationMode=14;
  cs->MotionElementClassName="NNewMotionElement";
  cs->MCNeuroObjectName="NNewSPNeuron";
  cs->ObjectControlInterfaceClassName="NControlObjectSource";
  cs->Create();
  net=cs;
  net->SetName("EngineControlRangeAfferent");
  storage->PopObject(net);
  UploadClass("N2AsfNewSimplestAfferentBranchedEngineControl",net);
 }


{
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NPositionControlElement>();
 generated_cont->SetStorage(storage);
 cont->SetStorage(storage);
 generated_cont->SetName("NPositionControlElement");
 generated_cont->Default();
 UploadClass("NPositionControlElement",generated_cont);
}

{
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NNewPositionControlElement>();
 generated_cont->SetStorage(storage);
 cont->SetStorage(storage);
 generated_cont->SetName("NNewPositionControlElement");
 generated_cont->Default();
 UploadClass("NNewPositionControlElement",generated_cont);
}

{
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NMultiPositionControl>();
 generated_cont->SetStorage(storage);
 cont->SetStorage(storage);
 generated_cont->SetName("NMultiPositionControl");
 generated_cont->Default();
 UploadClass("NMultiPositionControl",generated_cont);
}

{
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NTrajectoryElement>();
 generated_cont->SetStorage(storage);
 cont->SetStorage(storage);
 generated_cont->SetName("NTrajectoryElement");
 generated_cont->Default();
 UploadClass("NTrajectoryElement",generated_cont);
}

{
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NMazeMemory>();
 generated_cont->SetStorage(storage);
 cont->SetStorage(storage);
 generated_cont->SetName("NMazeMemory");
 generated_cont->Default();
 UploadClass("NMazeMemory",generated_cont);
}

{
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NMazeMemory_simplified>();
 generated_cont->SetStorage(storage);
 cont->SetStorage(storage);
 generated_cont->SetName("NMazeMemory_simplified");
 generated_cont->Default();
 UploadClass("NMazeMemory_simplified",generated_cont);
}

{
  std::shared_ptr<RDK::UContainer> generated_cont=std::make_shared<NPCNElement>();
 generated_cont->SetStorage(storage);
 cont->SetStorage(storage);
 generated_cont->SetName("NPCN");
 generated_cont->Default();
 UploadClass("NPCN",generated_cont);
}

// Log completion of CreateClassSamples for debugging segfault
LOG(INFO) << "NMotionControlLibrary::CreateClassSamples - completed successfully";
 } catch (const std::bad_weak_ptr& e) {
  LOG(ERROR) << "NMotionControlLibrary::CreateClassSamples - bad_weak_ptr exception: " << e.what();
  throw; // Re-throw to be caught by test
 } catch (const std::exception& e) {
  LOG(ERROR) << "NMotionControlLibrary::CreateClassSamples - exception: " << e.what();
  throw; // Re-throw to be caught by test
 }
}
// --------------------------


}
