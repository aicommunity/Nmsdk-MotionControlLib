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
// Конструкторы и деструкторы
// --------------------------
NMotionControlLibrary::NMotionControlLibrary(void)
 : ULibrary("MotionControlLibrary","1.0", GetGlobalVersion())
{
}
// --------------------------

// --------------------------
// Методы создания составных моделей
// --------------------------
// --------------------------

// --------------------------
// Методы заполенения бибилиотеки
// --------------------------
// Заполняет массив ClassSamples готовыми экземплярами образцов и их именами.
// Не требуется предварительная очистка массива и уборка памяти.
void NMotionControlLibrary::CreateClassSamples(UStorage *storage)
{
 UEPtr<RDK::UContainer> cont;

 cont=new NDCEngine;
 cont->SetName("DCEngine");
 cont->Default();
 UploadClass("NDCEngine",cont);

 cont=new NPendulumAndCart;
 cont->SetName("PendulumAndCart");
 cont->Default();
 UploadClass("NPendulumAndCart",cont);

{
 UEPtr<UContainer> generated_cont=new NAstaticGyro;
 generated_cont->SetName("NAstaticGyro");
 generated_cont->Default();
 UploadClass("NAstaticGyro",generated_cont);
}

{
 UEPtr<UContainer> generated_cont=new NManipulatorAndGyro;
 generated_cont->SetName("NManipulatorAndGyro");
 generated_cont->Default();
 UploadClass("NManipulatorAndGyro",generated_cont);
}



 cont=new NManipulator;
 cont->SetName("Manipulator");
 cont->Default();
 UploadClass("NManipulator",cont);

 cont=new NNavMousePrimitive;
 cont->SetName("NavMousePrimitive");
 cont->Default();
 UploadClass("NNavMousePrimitive",cont);



  cont=new NFrequencyReceiver;
 cont->SetName("FrequencyReceiver");
 cont->Default();
 UploadClass("NFrequencyReceiver",cont);

 cont=new NManipulatorInput;
 cont->SetName("ManipulatorInput");
 cont->Default();
 UploadClass("NManipulatorInput",cont);

 cont=new NManipulatorInputEmulator;
 cont->SetName("ManipulatorInputEmulator");
 cont->Default();
 UploadClass("NManipulatorInputEmulator",cont);

 cont=new NSimpleStatistic;
 cont->SetName("SimpleStatistic");
 cont->Default();
 UploadClass("NSimpleStatistic",cont);
/*
 cont=new NNetworkLinksStatistic;
 cont->SetName("NetworkLinksStatistic");
 cont->Default();
 UploadClass("NNetworkLinksStatistic",cont);
 */
 cont=new NPulseReceiver;
 cont->SetName("PulseReceiver");
 cont->Default();
 UploadClass("NPulseReceiver",cont);



 cont=new NSignumSeparator;
 cont->SetName("SignumSeparator");
 cont->Default();
 UploadClass("NSignumSeparator",cont);

 cont=new NIntervalSeparator;
 cont->SetName("IntervalSeparator");
 cont->Default();
 UploadClass("NIntervalSeparator",cont);


 // Разделители аналогового сигнала на положительную и отрицательную части
 cont=dynamic_pointer_cast<UContainer>(dynamic_cast<UStorage*>(storage)->TakeObject("NSignumSeparator"));
 cont->SetName("PosSignumSeparator");
 vector<double> signum;
 signum.assign(1,1.0);
// signum.assign(1,1.0/(2*M_PI));
 cont->Default();
 dynamic_pointer_cast<NSignumSeparator>(cont)->Sign=signum;
 UploadClass("NPosSignumSeparator",cont);

 cont=dynamic_pointer_cast<UContainer>(dynamic_cast<UStorage*>(storage)->TakeObject("NSignumSeparator"));
 cont->SetName("NegSignumSeparator");
 signum.assign(1,-1.0);
// signum.assign(1,-1.0/(2*M_PI));
 cont->Default();
 dynamic_pointer_cast<NSignumSeparator>(cont)->Sign=signum;
 UploadClass("NNegSignumSeparator",cont);

 cont=new NEyeRetina;
 cont->SetName("EyeRetina");
 cont->Default();
 UploadClass("NEyeRetina",cont);


  cont=new NManipulatorSourceEmulator;
 cont->SetName("ManipulatorSourceEmulator");
 cont->Default();
 UploadClass("NManipulatorSourceEmulator",cont);

 cont=new NManipulatorSource;
 cont->SetName("ManipulatorSource");
 cont->Default();
 UploadClass("NManipulatorSource",cont);

 cont=new NControlObjectSource;
 cont->SetName("ManipulatorSource");
 cont->Default();
 UploadClass("NControlObjectSource",cont);


 cont = new NSuppressionUnit;
 cont->SetName("SuppressionUnit");
 cont->Default();
 UploadClass("NSuppressionUnit", cont);

 cont = new NCounterNeuron;
 cont->SetName("NCounterNeuron");
 cont->Default();
 UploadClass("NCounterNeuron", cont);

 cont = new NObjInArea;
 cont->SetName("NObjInArea");
 cont->Default();
 UploadClass("NObjInArea", cont);

 cont = new NSeqComparison;
 cont->SetName("NSeqComparison");
 cont->Default();
 UploadClass("NSeqComparison", cont);

 cont = new NActuatorSignals;
 cont->SetName("NActuatorSignals");
 cont->Default();
 UploadClass("NActuatorSignals", cont);

 cont = new NSignalEstimation;
 cont->SetName("NSignalEstimation");
 cont->Default();
 UploadClass("NSignalEstimation", cont);


 // Создаем СУ двигательной единицей
 UEPtr<UNet> net=new NMotionElement;
 net->SetName("MotionElement");
 net->Default();
 UploadClass("NNewMotionElement",net);

 // Создаем прототип систем управления
 UEPtr<NEngineMotionControl> cs=0;
 cs=new NEngineMotionControl;
 cs->Default();
 cs->SetName("EngineMotionControl");
 UploadClass("NEngineMotionControl",cs);

 // Режим формирования сети

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
