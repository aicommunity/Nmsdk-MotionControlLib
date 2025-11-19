/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   December, 2023
/// @brief  The component for determining the direction of movement of the control object

#include "NActuatorSignals.h"

//---------------------------------------------------------------------------
namespace NMSDK {

/* *************************************************************************** */
// --------------------------
// Constructors & destructors
// --------------------------
NActuatorSignals::NActuatorSignals(void)
 : NeuronClassName("ClassNameNeuron", this, &NActuatorSignals::SetNeuronClassName),
   GeneratorClassName("ClassNameGenerator", this, &NActuatorSignals::SetGeneratorClassName),
   DelayClassName("ClassNameDelay", this, &NActuatorSignals::SetDelayClassName),
   PulseLength("PulseLength", this, &NActuatorSignals::SetPulseLength),
   Amplitude("Amplitude", this, &NActuatorSignals::SetAmplitude),
   Frequency("Frequency", this, &NActuatorSignals::SetFrequency),
   LeftDelay("DelayLeft", this, &NActuatorSignals::SetLeftDelay),
   RightDelay("DelayRight", this, &NActuatorSignals::SetRightDelay),
   Sinchro1Delay("DelaySinchro1", this, &NActuatorSignals::SetSinchro1Delay),
   Sinchro2Delay("DelaySinchro2", this, &NActuatorSignals::SetSinchro2Delay),
   UseTransitEngineSignal("UseTransitEngineSignal", this, &NActuatorSignals::SetUseTransitEngineSignal),
   UseTransitSinchroSignal("UseTransitSinchroSignal", this, &NActuatorSignals::SetUseTransitSinchroSignal),
   Stay("Stay", this),
   Forward("Forward", this),
   Back("Back", this),
   Left("Left", this),
   Right("Right", this)
{
 LeftEngine = NULL;
 RightEngine = NULL;
 Sinchro1 = NULL;
 Sinchro2 = NULL;

 NOTGenerator = NULL;
 Delay1To2 = NULL;
 ORNeuron = NULL;
 NOTNeuron = NULL;

 IsForwardNeuron = NULL;
 IsBackNeuron = NULL;
 IsLeftNeuron = NULL;
 IsRightNeuron = NULL;

 ForwardNeuron = NULL;
 BackNeuron = NULL;
 LeftNeuron = NULL;
 RightNeuron = NULL;
 StayNeuron = NULL;
}

NActuatorSignals::~NActuatorSignals(void)
{
}
// --------------------------


// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
NActuatorSignals* NActuatorSignals::New(void)
{
 return new NActuatorSignals;
}
// --------------------------


// --------------------------
// ������� ������ ���������� ������������
// --------------------------

/// ��������� ����������� ���������������� ��������
/// ��� ���������� ��������� ���������� � ���� ������
// ����� ����� ������ ������ ���� comp ��� ������� �������� � ������ ���������
bool NActuatorSignals::AAddComponent(std::shared_ptr<UContainer> comp, std::shared_ptr<UIPointer> pointer)
{
 return true;
}

/// ��������� ��������������� ���������������� ��������
/// ��� �������� ��������� ���������� �� ����� �������
// ����� ����� ������ ������ ���� comp ���������� � ������ ���������
bool NActuatorSignals::ADelComponent(std::shared_ptr<UContainer> comp)
{
 return true;
}

// --------------------------


// --------------------------
// ������ ���������� �������������� ����������
// --------------------------

/// ��������� ����� ������ �������
bool NActuatorSignals::SetNeuronClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// ��������� ����� ������ ����������� ���������
bool NActuatorSignals::SetGeneratorClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// ��������� ����� ������ ����� �������� �������
bool NActuatorSignals::SetDelayClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// ��������� ������������ ���������
bool NActuatorSignals::SetPulseLength(const double &value)
{
 if(value <= 0)
  return false;

 // ����������� ����������
 std::shared_ptr <NPulseGeneratorTransit> generators[5]{LeftEngine, RightEngine, Sinchro1, Sinchro2, NOTGenerator};

 for (int i = 0; i < 5; i++)
 {
  if(generators[i])
   generators[i]->PulseLength = value;
 }

 return true;
}

/// ��������� ��������� ���������
bool NActuatorSignals::SetAmplitude(const double &value)
{
 if(value <= 0)
  return false;

 // ����������� ����������
 std::shared_ptr <NPulseGeneratorTransit> generators[5]{LeftEngine, RightEngine, Sinchro1, Sinchro2, NOTGenerator};

 for (int i = 0; i < 5; i++)
 {
  if(generators[i])
   generators[i]->Amplitude = value;
 }

 return true;
}

/// ��������� ������� ��������� (��) ���� �����������
bool NActuatorSignals::SetFrequency(const double &value)
{
 if(value < 0)
  return false;

 // ����������� ����������
 std::shared_ptr <NPulseGeneratorTransit> generators[5]{LeftEngine, RightEngine, Sinchro1, Sinchro2, NOTGenerator};

 for (int i = 0; i < 5; i++)
 {
  if(generators[i] && !generators[i]->UseTransitSignal)
   generators[i]->Frequency = value;
 }

 return true;
}

/// ��������� �������� ��� ������ ���������
bool NActuatorSignals::SetLeftDelay(const double &value)
{
 if(value < 0)
  return false;

 if (LeftEngine)
  LeftEngine->Delay = value;

 return true;
}

/// ��������� �������� ��� ������� ���������
bool NActuatorSignals::SetRightDelay(const double &value)
{
 if(value < 0)
  return false;

 if (RightEngine)
  RightEngine->Delay = value;

 return true;
}

/// ��������� �������� ����������������� ��������, ������ ���� �������� �������� �����
bool NActuatorSignals::SetSinchro1Delay(const double &value)
{
 if(value < 0)
  return false;

 if (Sinchro1)
  Sinchro1->Delay = value;

 if (Delay1To2)
  Delay1To2->DelayTime = Sinchro2Delay - value;

 return true;
}

/// ��������� �������� ����������������� ��������, ������ ���� �������� �������� ������
bool NActuatorSignals::SetSinchro2Delay(const double &value)
{
 if(value < 0)
  return false;

 if (Sinchro2)
  Sinchro2->Delay = value;

 if (Delay1To2)
  Delay1To2->DelayTime = value - Sinchro1Delay;

 if (NOTGenerator)
  NOTGenerator->Delay = value;

 return true;
}


/// ��������� ������������� �������� ������� �� ��������� �� �������� ���������
bool NActuatorSignals::SetUseTransitEngineSignal(const bool &value)
{
 LeftEngine->UseTransitSignal = value;
 RightEngine->UseTransitSignal = value;

 if (value)
 {
  LeftEngine->Frequency = 0;
  RightEngine->Frequency = 0;
 }
 else
 {
  LeftEngine->Frequency = Frequency;
  RightEngine->Frequency = Frequency;
 }

 return true;
}

/// ��������� ������������� �������� ������� �� ���������������� ���������� �� �������� ���������
bool NActuatorSignals::SetUseTransitSinchroSignal(const bool &value)
{
 Sinchro1->UseTransitSignal = value;
 Sinchro2->UseTransitSignal = value;

 if (value)
 {
  Sinchro1->Frequency = 0;
  Sinchro2->Frequency = 0;
 }
 else
 {
  Sinchro1->Frequency = Frequency;
  Sinchro2->Frequency = Frequency;
 }

 return true;
}

// --------------------------


// --------------------------
// Computation methods
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool NActuatorSignals::ADefault(void)
{
 NeuronClassName = "NSPNeuronGen";
 GeneratorClassName = "NPulseGeneratorTransit";
 DelayClassName = "NPDelay";

 PulseLength = 0.001;
 Amplitude = 1.0;
 Frequency = 0.0;

 LeftDelay = 0;
 RightDelay = 0;
 Sinchro1Delay = 0.05;
 Sinchro2Delay = 0.15;

 Stay.Assign(1,1,0.0);
 Forward.Assign(1,1,0.0);
 Back.Assign(1,1,0.0);
 Left.Assign(1,1,0.0);
 Right.Assign(1,1,0.0);

 UseTransitEngineSignal = false;
 UseTransitSinchroSignal = false;

 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NActuatorSignals::ABuild(void)
{
 bool res = true;

 std::shared_ptr<NPulseMembrane> soma, dendrite;
 NPulseSynapseCommon *excsynapse, *inhsynapse;

 // �������������� ����������
 {
  // �������������� �������� ������� ������ ���������
  LeftEngine = AddMissingComponent<NPulseGeneratorTransit>(std::string("LeftEngine"), GeneratorClassName);
  if(!LeftEngine) return true;
  LeftEngine->SetCoord(MVector<double,3>(4, 2.33, 0));
  LeftEngine->DisconnectAll("Output");

  // �������������� �������� ������� ������� ���������
  RightEngine = AddMissingComponent<NPulseGeneratorTransit>(std::string("RightEngine"), GeneratorClassName);
  if(!RightEngine) return true;
  RightEngine->SetCoord(MVector<double,3>(4, 5, 0));
  RightEngine->DisconnectAll("Output");

  // �������������� �������� ����������������� ������� 1 - ������� �������� �����
  Sinchro1 = AddMissingComponent<NPulseGeneratorTransit>(std::string("Sinchro1"), GeneratorClassName);
  if(!Sinchro1) return true;
  Sinchro1->SetCoord(MVector<double,3>(4, 10.33, 0));
  Sinchro1->DisconnectAll("Output");

  // �������������� �������� ����������������� ������� 2 - ������� �������� ������
  Sinchro2 = AddMissingComponent<NPulseGeneratorTransit>(std::string("Sinchro2"), GeneratorClassName);
  if(!Sinchro2) return true;
  Sinchro2->SetCoord(MVector<double,3>(4, 13, 0));
  Sinchro2->DisconnectAll("Output");

  // �������������� ��������� ��� ������� NOTNeuron
  NOTGenerator = AddMissingComponent<NPulseGeneratorTransit>(std::string("NOTGenerator"), GeneratorClassName);
  if(!NOTGenerator) return true;
  NOTGenerator->SetCoord(MVector<double,3>(21.33, 2.33, 0));
  NOTGenerator->DisconnectAll("Output");


  // �������������� ���� ��������
  Delay1To2 = AddMissingComponent<NPulseDelay>(std::string("Delay1To2"), DelayClassName);
  if(!Delay1To2) return true;
  Delay1To2->SetCoord(MVector<double,3>(21.33, 11.66, 0));
  Delay1To2->DisconnectAll("Output");


  // �������������� ������ ���
  ORNeuron = AddMissingComponent<NPulseNeuron>(std::string("ORNeuron"), NeuronClassName);
  if(!ORNeuron) return true;
  ORNeuron->SetCoord(MVector<double,3>(21.33, 7.66, 0));
  ORNeuron->DisconnectAll("Output");
 // CRITICAL: GetComponentL now returns weak_ptr, need to lock
 std::weak_ptr<UContainer> soma_weak = ORNeuron->GetComponentL("Soma1", true);
 if(!soma_weak.expired())
  soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak.lock());
 else
  soma = nullptr;
 if (soma)
 {
  soma->NumExcitatorySynapses = 4;
  soma->Build();
 }

  // �������������� ������ ��
 NOTNeuron = AddMissingComponent<NPulseNeuron>(std::string("NOTNeuron"), NeuronClassName);
 if(!NOTNeuron) return true;
 NOTNeuron->SetCoord(MVector<double,3>(21.33, 5, 0));
 NOTNeuron->DisconnectAll("Output");
 if(NOTNeuron->StructureBuildMode != 2)
  NOTNeuron->NumDendriteMembraneParts = 2;
 else
 {
  std::vector<int> dend;
  dend.assign(1, 2);
  NOTNeuron->NumDendriteMembranePartsVec = dend;
 }
 NOTNeuron->Reset();


  // �������������� ������, ������������ �������� �����
 IsForwardNeuron = AddMissingComponent<NPulseNeuron>(std::string("IsForward"), NeuronClassName);
 if(!IsForwardNeuron) return true;
 IsForwardNeuron->SetCoord(MVector<double,3>(12.66, 3.66, 0));
 IsForwardNeuron->DisconnectAll("Output");
 IsForwardNeuron->NumSomaMembraneParts = 2;
 IsForwardNeuron->Build();

 // �������������� ������, ������������ �������� �����
 IsBackNeuron = AddMissingComponent<NPulseNeuron>(std::string("IsBack"), NeuronClassName);
 if(!IsBackNeuron) return true;
 IsBackNeuron->SetCoord(MVector<double,3>(12.66, 11.66, 0));
 IsBackNeuron->DisconnectAll("Output");
 IsBackNeuron->NumSomaMembraneParts = 2;
 IsBackNeuron->Build();

 // �������������� ������, ������������ ������� ������
 IsLeftNeuron = AddMissingComponent<NPulseNeuron>(std::string("IsLeft"), NeuronClassName);
 if(!IsLeftNeuron) return true;
 IsLeftNeuron->SetCoord(MVector<double,3>(12.66, 6.33, 0));
 IsLeftNeuron->DisconnectAll("Output");
 IsLeftNeuron->NumSomaMembraneParts = 2;
 if(IsLeftNeuron->StructureBuildMode != 2)
  IsLeftNeuron->NumDendriteMembraneParts = 2;
 else
 {
  std::vector<int> dend;
  dend.assign(2, 0);
  dend[0] = 10;
  IsLeftNeuron->NumDendriteMembranePartsVec = dend;
 }
 IsLeftNeuron->Build();

  // �������������� ������, ������������ ������� �������
 IsRightNeuron = AddMissingComponent<NPulseNeuron>(std::string("IsRight"), NeuronClassName);
 if(!IsRightNeuron) return true;
 IsRightNeuron->SetCoord(MVector<double,3>(12.66, 9, 0));
 IsRightNeuron->DisconnectAll("Output");
 IsRightNeuron->NumSomaMembraneParts = 2;
 if(IsRightNeuron->StructureBuildMode != 2)
  IsRightNeuron->NumDendriteMembraneParts = 2;
 else
 {
  std::vector<int> dend;
  dend.assign(2, 0);
  dend[1] = 10;
  IsRightNeuron->NumDendriteMembranePartsVec = dend;
 }
 IsRightNeuron->Build();


  // �������������� �������� ������, ��������������� �� ���������
 StayNeuron = AddMissingComponent<NPulseNeuron>(std::string("Stay"), NeuronClassName);
 if(!StayNeuron) return true;
 StayNeuron->SetCoord(MVector<double,3>(30, 2.33, 0));
 StayNeuron->DisconnectAll("Output");

 // �������������� �������� ������, ��������������� � �������� �����
 ForwardNeuron = AddMissingComponent<NPulseNeuron>(std::string("Forward"), NeuronClassName);
 if(!ForwardNeuron) return true;
 ForwardNeuron->SetCoord(MVector<double,3>(30, 5, 0));
 ForwardNeuron->DisconnectAll("Output");

 // �������������� �������� ������, ��������������� � �������� ������
 LeftNeuron = AddMissingComponent<NPulseNeuron>(std::string("Left"), NeuronClassName);
 if(!LeftNeuron) return true;
 LeftNeuron->SetCoord(MVector<double,3>(30, 7.66, 0));
 LeftNeuron->DisconnectAll("Output");

 // �������������� �������� ������, ��������������� � �������� �������
 RightNeuron = AddMissingComponent<NPulseNeuron>(std::string("Right"), NeuronClassName);
 if(!RightNeuron) return true;
 RightNeuron->SetCoord(MVector<double,3>(30, 10.33, 0));
 RightNeuron->DisconnectAll("Output");

 // �������������� �������� ������, ��������������� � �������� �����
 BackNeuron = AddMissingComponent<NPulseNeuron>(std::string("Back"), NeuronClassName);
 if(!BackNeuron) return true;
 BackNeuron->SetCoord(MVector<double,3>(30, 13, 0));
 BackNeuron->DisconnectAll("Output");
}


 // ������ ����� ����� ������������
 {
 // ����� �� IsForward
 {
  // CRITICAL: GetComponent now returns weak_ptr, need to lock
  std::weak_ptr<UContainer> soma_weak = IsForwardNeuron->GetComponent("Soma1", true);
  if(!soma_weak.expired())
   soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak.lock());
  else
   soma = nullptr;
  
  // SAFETY: Check if soma is valid before accessing it
  if(!soma)
  {
   LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because Soma1 component doesn't exist in IsForwardNeuron"));
   // Skip this section
  }
  else
  {
   excsynapse = soma->GetExcitatorySynapses(0);
   inhsynapse = soma->GetInhibitorySynapses(0);

   // ������ ����� c LeftEngine
   if(excsynapse && !CheckLink("LeftEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("LeftEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");

   //   c Sinchro1
   if(inhsynapse && !CheckLink("Sinchro1", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("Sinchro1", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
  }


  // CRITICAL: GetComponent now returns weak_ptr, need to lock
  std::weak_ptr<UContainer> soma_weak_soma2 = IsForwardNeuron->GetComponent("Soma2", true);
  if(!soma_weak_soma2.expired())
   soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak_soma2.lock());
  else
   soma = nullptr;
  
  // SAFETY: Check if soma is valid before accessing it
  if(!soma)
  {
   LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because Soma2 component doesn't exist in IsForwardNeuron"));
   // Skip this section
  }
  else
  {
   excsynapse = soma->GetExcitatorySynapses(0);
   inhsynapse = soma->GetInhibitorySynapses(0);

   // ������ ����� c RightEngine
   if(excsynapse && !CheckLink("RightEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("RightEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");

   //   c Sinchro1
   if(inhsynapse && !CheckLink("Sinchro1", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("Sinchro1", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
  }
 }

 //   IsLeft
 {
  // CRITICAL: GetComponent now returns weak_ptr, need to lock
  std::weak_ptr<UContainer> dendrite_weak = IsLeftNeuron->GetComponent("Dendrite1_10", true);
  if(!dendrite_weak.expired())
   dendrite = std::dynamic_pointer_cast<NPulseMembrane>(dendrite_weak.lock());
  else
   dendrite = nullptr;
  
  // SAFETY: Check if dendrite is valid before accessing it
  if(!dendrite)
  {
   LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because Dendrite1_10 component doesn't exist in IsLeftNeuron"));
   // Skip this section
  }
  else
  {
   excsynapse = dendrite->GetExcitatorySynapses(0);
   inhsynapse = dendrite ->GetInhibitorySynapses(0);

   // ������ ����� c LeftEngine
   if(excsynapse && !CheckLink("LeftEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("LeftEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");

   //   c Sinchro2
   if(inhsynapse && !CheckLink("Sinchro2", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("Sinchro2", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
  }


  // CRITICAL: GetComponent now returns weak_ptr, need to lock
  std::weak_ptr<UContainer> soma_weak2 = IsLeftNeuron->GetComponent("Soma2", true);
  if(!soma_weak2.expired())
   soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak2.lock());
  else
   soma = nullptr;
  
  // SAFETY: Check if soma is valid before accessing it
  if(!soma)
  {
   LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because Soma2 component doesn't exist in IsLeftNeuron"));
   // Skip this section
  }
  else
  {
   excsynapse = soma->GetExcitatorySynapses(0);
   inhsynapse = soma->GetInhibitorySynapses(0);

   // ������ ����� c RightEngine
   if(excsynapse && !CheckLink("RightEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("RightEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");

   //   c Sinchro1
   if(inhsynapse && !CheckLink("Sinchro1", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("Sinchro1", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
  }
 }

 //   IsRight
 {
  // CRITICAL: GetComponent now returns weak_ptr, need to lock
  std::weak_ptr<UContainer> dendrite_weak2 = IsRightNeuron->GetComponent("Dendrite2_10", true);
  if(!dendrite_weak2.expired())
   dendrite = std::dynamic_pointer_cast<NPulseMembrane>(dendrite_weak2.lock());
  else
   dendrite = nullptr;
  
  // SAFETY: Check if dendrite is valid before accessing it
  if(!dendrite)
  {
   LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because Dendrite2_10 component doesn't exist in IsRightNeuron"));
   // Skip this section
  }
  else
  {
   excsynapse = dendrite->GetExcitatorySynapses(0);
   inhsynapse = dendrite ->GetInhibitorySynapses(0);

   // ������ ����� c RightEngine
   if(excsynapse && !CheckLink("RightEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("RightEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");

   //   c Sinchro2
   if(inhsynapse && !CheckLink("Sinchro2", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("Sinchro2", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
  }


  // CRITICAL: GetComponent now returns weak_ptr, need to lock
  std::weak_ptr<UContainer> soma_weak3 = IsRightNeuron->GetComponent("Soma1", true);
  if(!soma_weak3.expired())
   soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak3.lock());
  else
   soma = nullptr;
  
  // SAFETY: Check if soma is valid before accessing it
  if(!soma)
  {
   LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because Soma1 component doesn't exist in IsRightNeuron"));
   // Skip this section - we're in a block, not a loop, so we just skip the code below
  }
  else
  {
   excsynapse = soma->GetExcitatorySynapses(0);
   inhsynapse = soma->GetInhibitorySynapses(0);

   //   c LeftEngine
   if(excsynapse && !CheckLink("LeftEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("LeftEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");

   //   c Sinchro1
   if(inhsynapse && !CheckLink("Sinchro1", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("Sinchro1", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
  }
 }

 //   IsBack
 {
  // CRITICAL: GetComponent now returns weak_ptr, need to lock
  std::weak_ptr<UContainer> soma_weak4 = IsBackNeuron->GetComponent("Soma1", true);
  if(!soma_weak4.expired())
   soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak4.lock());
  else
   soma = nullptr;
  
  // SAFETY: Check if soma is valid before accessing it
  if(soma)
  {
   excsynapse = soma->GetExcitatorySynapses(0);
   inhsynapse = soma->GetInhibitorySynapses(0);

   //   c LeftEngine
   if(excsynapse && !CheckLink("LeftEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("LeftEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");

   //   c Sinchro2
   if(inhsynapse && !CheckLink("Sinchro2", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("Sinchro2", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
  }

  // CRITICAL: GetComponent now returns weak_ptr, need to lock
  std::weak_ptr<UContainer> soma_weak5 = IsBackNeuron->GetComponent("Soma2", true);
  if(!soma_weak5.expired())
   soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak5.lock());
  else
   soma = nullptr;
  
  // SAFETY: Check if soma is valid before accessing it
  if(soma)
  {
   excsynapse = soma->GetExcitatorySynapses(0);
   inhsynapse = soma->GetInhibitorySynapses(0);

   //   c RightEngine
   if(excsynapse && !CheckLink("RightEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("RightEngine", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");

   //   c Sinchro2
   if(inhsynapse && !CheckLink("Sinchro2", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("Sinchro2", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
  }
 }

 //   ORNeuron
 {
  // CRITICAL: GetComponent now returns weak_ptr, need to lock
  std::weak_ptr<UContainer> soma_weak6 = ORNeuron->GetComponent("Soma1", true);
  if(!soma_weak6.expired())
   soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak6.lock());
  else
   soma = nullptr;

  // SAFETY: Check if soma is valid before accessing it
  if(soma)
  {
   //   c IsForward
   excsynapse = soma->GetExcitatorySynapses(0);
   if(excsynapse && !CheckLink("IsForward", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("IsForward", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");

   //   c IsLeft
   excsynapse = soma->GetExcitatorySynapses(1);
   if(excsynapse && !CheckLink("IsLeft", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("IsLeft", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");

   //   c IsRight
   excsynapse = soma->GetExcitatorySynapses(2);
   if(excsynapse && !CheckLink("IsRight", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("IsRight", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");

   //   c Delay1To2
   excsynapse = soma->GetExcitatorySynapses(3);
   if(excsynapse && !CheckLink("Delay1To2", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("Delay1To2", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
  }
 }

 //   NOTNeuron
 {
  //   c ORNeuron
  // CRITICAL: GetComponent now returns weak_ptr, need to lock
  std::weak_ptr<UContainer> soma_weak7 = NOTNeuron->GetComponent("Soma1", true);
  if(!soma_weak7.expired())
   soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak7.lock());
  else
   soma = nullptr;
  
  // SAFETY: Check if soma is valid before accessing it
  if(soma)
  {
   inhsynapse = soma->GetInhibitorySynapses(0);
   if(inhsynapse && !CheckLink("ORNeuron", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("ORNeuron", "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
  }

  //   c NOTGenerator
  // CRITICAL: GetComponent now returns weak_ptr, need to lock
  std::weak_ptr<UContainer> dendrite_weak3 = NOTNeuron->GetComponent("Dendrite1_2", true);
  if(!dendrite_weak3.expired())
   dendrite = std::dynamic_pointer_cast<NPulseMembrane>(dendrite_weak3.lock());
  else
   dendrite = nullptr;
  
  // SAFETY: Check if dendrite is valid before accessing it
  if(dendrite)
  {
   excsynapse = dendrite->GetExcitatorySynapses(0);
   if(excsynapse && !CheckLink("NOTGenerator", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink("NOTGenerator", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
  }
 }

 //  c IsBack  Delay1To2
 if (!CheckLink("IsBack", "Output", "Delay1To2", "Input"))
  res &= CreateLink("IsBack", "Output", "Delay1To2", "Input");

 //  NOTNeuron  Stay
 // CRITICAL: GetComponent now returns weak_ptr, need to lock
 std::weak_ptr<UContainer> soma_weak8 = StayNeuron->GetComponent("Soma1", true);
 if(!soma_weak8.expired())
  soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak8.lock());
 else
  soma = nullptr;
 if(soma)
 {
  excsynapse = soma->GetExcitatorySynapses(0);
  if(excsynapse && !CheckLink("NOTNeuron", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
   res &= CreateLink("NOTNeuron", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
 }

 //  IsForward  Forward
 // CRITICAL: GetComponent now returns weak_ptr, need to lock
 std::weak_ptr<UContainer> soma_weak9 = ForwardNeuron->GetComponent("Soma1", true);
 if(!soma_weak9.expired())
  soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak9.lock());
 else
  soma = nullptr;
 if(soma)
 {
  excsynapse = soma->GetExcitatorySynapses(0);
  if(excsynapse && !CheckLink("IsForward", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
   res &= CreateLink("IsForward", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
 }

 //  IsLeft  Left
 // CRITICAL: GetComponent now returns weak_ptr, need to lock
 std::weak_ptr<UContainer> soma_weak10 = LeftNeuron->GetComponent("Soma1", true);
 if(!soma_weak10.expired())
  soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak10.lock());
 else
  soma = nullptr;
 if(soma)
 {
  excsynapse = soma->GetExcitatorySynapses(0);
  if(excsynapse && !CheckLink("IsLeft", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
   res &= CreateLink("IsLeft", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
 }

 //  IsRight  Right
 // CRITICAL: GetComponent now returns weak_ptr, need to lock
 std::weak_ptr<UContainer> soma_weak11 = RightNeuron->GetComponent("Soma1", true);
 if(!soma_weak11.expired())
  soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak11.lock());
 else
  soma = nullptr;
 if(soma)
 {
  excsynapse = soma->GetExcitatorySynapses(0);
  if(excsynapse && !CheckLink("IsRight", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
   res &= CreateLink("IsRight", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
 }

 //  Delay1To2  Back
 // CRITICAL: GetComponent now returns weak_ptr, need to lock
 std::weak_ptr<UContainer> soma_weak12 = BackNeuron->GetComponent("Soma1", true);
 if(!soma_weak12.expired())
  soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak12.lock());
 else
  soma = nullptr;
 if(soma)
 {
  excsynapse = soma->GetExcitatorySynapses(0);
  if(excsynapse && !CheckLink("Delay1To2", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
   res &= CreateLink("Delay1To2", "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
 }

 } // Close block opened at line 469

 return res;
}


// Reset computation
bool NActuatorSignals::AReset(void)
{
 bool res = true;

 //   
 Stay.ToZero();
 Forward.ToZero();
 Back.ToZero();
 Left.ToZero();
 Right.ToZero();

 //  
 std::shared_ptr <NPulseGeneratorTransit> generators[5]{LeftEngine, RightEngine, Sinchro1, Sinchro2, NOTGenerator};
 for (int i = 0; i < 5; i++)
 {
  generators[i]->Amplitude = Amplitude;
  generators[i]->PulseLength = PulseLength;
  generators[i]->Frequency = Frequency;
 }
 LeftEngine->Delay = LeftDelay;
 RightEngine->Delay = RightDelay;
 Sinchro1->Delay = Sinchro1Delay;
 Sinchro2->Delay = Sinchro2Delay;
 NOTGenerator->Delay = Sinchro2Delay;

 // ����������� ���� ��������
 Delay1To2->DelayTime = Sinchro2Delay - Sinchro1Delay;

 // ����������� ������ ��������
 // CRITICAL: GetComponentL now returns weak_ptr, need to lock
 std::weak_ptr<UContainer> ltzone_weak = NOTNeuron->GetComponentL("LTZone", true);
 std::shared_ptr<NLTZone> ltzone;
 if(!ltzone_weak.expired())
  ltzone = std::dynamic_pointer_cast<NLTZone>(ltzone_weak.lock());
 if(ltzone)
  ltzone->Threshold = 0.01;

 std::shared_ptr <NPulseNeuron> is_neurons[4]{IsForwardNeuron, IsLeftNeuron, IsRightNeuron, IsBackNeuron};
 for (int i = 0; i < 4; i++)
 {
  // CRITICAL: GetComponentL now returns weak_ptr, need to lock
  std::weak_ptr<UContainer> ltzone_weak = is_neurons[i]->GetComponentL("LTZone", true);
  if(!ltzone_weak.expired())
   ltzone = std::dynamic_pointer_cast<NLTZone>(ltzone_weak.lock());
  else
   ltzone = nullptr;
  if(ltzone)
   ltzone->Threshold = 0.013;
 }

 return res;
}

// Execute math. computations of current object on current step
bool NActuatorSignals::ACalculate(void)
{
 bool res = true;

 // ������� ���������� �� ������
 Stay = StayNeuron->Output;
 Forward = ForwardNeuron->Output;
 Back = BackNeuron->Output;
 Left = LeftNeuron->Output;
 Right = RightNeuron->Output;


 return res;
}
// --------------------------
/* *************************************************************************** */

}
//---------------------------------------------------------------------------


