/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   December, 2023
/// @brief  The component for signal zoning

#include "NSignalEstimation.h"

//---------------------------------------------------------------------------
namespace NMSDK {

/* *************************************************************************** */
// --------------------------
// Constructors & destructors
// --------------------------
NSignalEstimation::NSignalEstimation(void)
 : NeuronClassName("NeuronClassName", this, &NSignalEstimation::SetNeuronClassName),
   GeneratorClassName("GeneratorClassName", this, &NSignalEstimation::SetGeneratorClassName),
   PulseLength("PulseLength", this, &NSignalEstimation::SetPulseLength),
   Amplitude("Amplitude", this, &NSignalEstimation::SetAmplitude),
   Frequency("Frequency", this, &NSignalEstimation::SetFrequency),
   LTZThreshold("LTZThreshold", this, &NSignalEstimation::SetLTZThreshold),
   DelaySignal("DelaysSignal", this, &NSignalEstimation::SetDelaySignal),
   DelaySinchro("DelaysSinchro", this, &NSignalEstimation::SetDelaySinchro),
   NumZones("NumZones", this, &NSignalEstimation::SetNumZones),
   UpperLimitsOfZones("UpperLimitsOfZones", this, &NSignalEstimation::SetUpperLimitsOfZones),
   UseTransitSignal("UseTransitSignal", this, &NSignalEstimation::SetUseTransitSignal),
   UseTransitSinchro("UseTransitSinchro", this, &NSignalEstimation::SetUseTransitSinchro),
   Sinchro("Sinchro", this),
   Input("Input", this)
{
 OldNumZones = 0;
 ZoneNeurons.clear();
 SignalGen = NULL;
 SinchroGen = NULL;
}

NSignalEstimation::~NSignalEstimation(void) {}
// --------------------------


// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
NSignalEstimation* NSignalEstimation::New(void)
{
 return new NSignalEstimation;
}
// --------------------------


// --------------------------
// ������� ������ ���������� ������������
// --------------------------

/// ��������� ����������� ���������������� ��������
/// ��� ���������� ��������� ���������� � ���� ������
// ����� ����� ������ ������ ���� comp ��� ������� �������� � ������ ���������
bool NSignalEstimation::AAddComponent(std::shared_ptr<UContainer> comp, std::shared_ptr<UIPointer> pointer)
{
 return true;
}

/// ��������� ��������������� ���������������� ��������
/// ��� �������� ��������� ���������� �� ����� �������
// ����� ����� ������ ������ ���� comp ���������� � ������ ���������
bool NSignalEstimation::ADelComponent(std::shared_ptr<UContainer> comp)
{
 return true;
}

// --------------------------


// --------------------------
// ������ ���������� �������������� ����������
// --------------------------

/// ��������� ����� ������ ��������
bool NSignalEstimation::SetNeuronClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// ��������� ����� ������ ����������� ���������
bool NSignalEstimation::SetGeneratorClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// ��������� ������������ ���������
bool NSignalEstimation::SetPulseLength(const double &value)
{
 if(value <= 0)
  return false;

 if (SignalGen)
  SignalGen->PulseLength = value;

 if (SinchroGen)
  SinchroGen->PulseLength = value;

 return true;
}

/// ��������� ��������� ���������
bool NSignalEstimation::SetAmplitude(const double &value)
{
 if(value <= 0)
  return false;

 if (SignalGen)
  SignalGen->Amplitude = value;

 if (SinchroGen)
  SinchroGen->Amplitude = value;

 return true;
}

/// ��������� ������� ��������� (��) ���� �����������
bool NSignalEstimation::SetFrequency(const double &value)
{
 if(value < 0)
  return false;

 if (SignalGen)
  SignalGen->Frequency = value;

 if (SinchroGen)
  SinchroGen->Frequency = value;

 return true;
}

/// ��������� ������������� ������ �������������� ���� ��������
bool NSignalEstimation::SetLTZThreshold(const double &value)
{
 if(value <= 0)
  return false;

 for (int i = 0; i < ZoneNeurons.size(); i++)
 {
  if (!ZoneNeurons[i])
   continue;

  std::weak_ptr<RDK::UContainer> ltzone_weak = ZoneNeurons[i]->GetComponentL("LTZone");  // GetLTZone();
  std::shared_ptr<NLTZone> ltzone;
  if(!ltzone_weak.expired())
   ltzone = std::dynamic_pointer_cast<NLTZone>(ltzone_weak.lock());
  else
   ltzone = nullptr;
  if(!ltzone)
   continue;

  ltzone->Threshold = value;
 }

 return true;
}

/// ��������� �������� �������, ������� ����� ������������
bool NSignalEstimation::SetDelaySignal(const double &value)
{
 if(value < 0.0)
  return false;

 if (SignalGen)
  SignalGen->Delay = value;

 return true;
}

/// ��������� �������� ����������������� ��������
bool NSignalEstimation::SetDelaySinchro(const double &value)
{
 if(value < 0.0)
  return false;

 if (SinchroGen)
  SinchroGen->Delay = value;

 return true;
}

/// ��������� ���������� ��������, �� ������� ����� ������������ ������
bool NSignalEstimation::SetNumZones(const int &value)
{
 if(value < 2)
  return false;

 OldNumZones = NumZones;

 UpperLimitsOfZones.Resize(1, value, 0.0);

 Ready = false;
 return true;
}

/// ��������� ������� ����������� ��� ������ �� �������� �����������
bool NSignalEstimation::SetUpperLimitsOfZones(const MDMatrix<double> &value)
{
 bool res = true;

 Ready = false;
 return res;
}

/// ��������� ������������� �������� ����������� ������� �� �������� ���������
bool NSignalEstimation::SetUseTransitSignal(const bool &value)
{
 SignalGen->UseTransitSignal = value;

 if (value)
 {
  SignalGen->Frequency = 0;
  SignalGen->Delay = 0;
 }
 else
 {
  SignalGen->Frequency = Frequency;
  SignalGen->Delay = DelaySignal;
 }

 return true;
}

/// ��������� ������������� �������� ������� �� ���������������� ��������� �� �������� ���������
bool NSignalEstimation::SetUseTransitSinchro(const bool &value)
{
 SinchroGen->UseTransitSignal = value;

 if (value)
 {
  SinchroGen->Frequency = 0;
  SinchroGen->Delay = 0;
 }
 else
 {
  SinchroGen->Frequency = Frequency;
  SinchroGen->Delay = DelaySinchro;
 }

 return true;
}
// --------------------------


// --------------------------
// Computation methods
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool NSignalEstimation::ADefault(void)
{
 NeuronClassName = "NSPNeuronGen";
 GeneratorClassName = "NPulseGeneratorTransit";

 PulseLength = 0.001;
 Amplitude = 1.0;
 Frequency = 0.0;
 LTZThreshold = 0.016;

 DelaySignal = 0.0;
 DelaySinchro = 0.2;

 NumZones = 4;

 UpperLimitsOfZones.Assign(1, NumZones, (0.0));
 UpperLimitsOfZones[0] = 0.05;
 UpperLimitsOfZones[1] = 0.10;
 UpperLimitsOfZones[2] = 0.15;
 UpperLimitsOfZones[3] = 0.20;

 UseTransitSignal = false;
 UseTransitSinchro = false;

 Sinchro->Assign(1,1,0.0);
 Input->Assign(1,1,0.0);

 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NSignalEstimation::ABuild(void)
{
 bool res = true;

 // �������������� ��������� ����������� �������
 SignalGen = AddMissingComponent<NPulseGeneratorTransit>(std::string("Signal"), GeneratorClassName);
 if(!SignalGen) return true;
 SignalGen->SetCoord(MVector<double,3>(4, 1.67, 0));
 SignalGen->DisconnectAll("Output");

 // �������������� ��������� ����������������� �������
 SinchroGen = AddMissingComponent<NPulseGeneratorTransit>(std::string("Sinchro"), GeneratorClassName);
 if(!SinchroGen) return true;
 SinchroGen->SetCoord(MVector<double,3>(4, 4, 0));
 SinchroGen->DisconnectAll("Output");


 ZoneNeurons.resize(NumZones);

 // ������� ���������� �������
 for(int i = 0; i < OldNumZones; i++)
  DelComponent(std::string("Zone_")+sntoa(i+1));

 // �������������� ���������� �������
 for (int i = 0; i < NumZones; i++)
 {
  ZoneNeurons[i] = AddMissingComponent<NPulseNeuron>(std::string("Zone_")+sntoa(i+1), NeuronClassName);
  if(!ZoneNeurons[i]) return true;
  ZoneNeurons[i]->SetCoord(MVector<double,3>(13, 1.67 + 2.33 * i, 0));
  ZoneNeurons[i]->DisconnectAll("Output");
  ZoneNeurons[i]->NumSomaMembraneParts = 2;
  ZoneNeurons[i]->Reset();

  // �� ������ ���� ������������� �������������� ����. ������� ��� �������� ������ �/� ���������
  for (int j = 1; j <= 2; j++)
  {
   std::weak_ptr<RDK::UContainer> soma_weak = ZoneNeurons[i]->GetComponentL("Soma" + sntoa(j), true);
   std::shared_ptr<NPulseMembrane> soma;
   if(!soma_weak.expired())
    soma = std::dynamic_pointer_cast<NPulseMembrane>(soma_weak.lock());
   else
    soma = nullptr;
   if (soma)
   {
    soma->NumInhibitorySynapses = NumZones - 1;
    soma->Build();
   }
  }

  // ������������� ����� ��������� = �������� ��� ������ ����
  if(ZoneNeurons[i]->StructureBuildMode != 2)
   ZoneNeurons[i]->NumDendriteMembraneParts = 2;
  else
  {
   std::vector<int> dend;
   dend.assign(2, 1);
   dend[0] = int(UpperLimitsOfZones[NumZones - i - 1] / 0.01 - 1);
   ZoneNeurons[i]->NumDendriteMembranePartsVec = dend;
  }

  ZoneNeurons[i]->Reset();
 }

 // ������ ����� � ����������� �� �������
 for (int i = 0; i < NumZones; i++)
 {
  int dendrite_num = int(UpperLimitsOfZones[NumZones - i - 1] / 0.01 - 1);
  std::weak_ptr<RDK::UContainer> dendrite_weak = ZoneNeurons[i]->GetComponentL("Dendrite1_"+sntoa(dendrite_num), true);
  std::shared_ptr<NPulseMembrane> dendrite;
  if(!dendrite_weak.expired())
   dendrite = std::dynamic_pointer_cast<NPulseMembrane>(dendrite_weak.lock());
  else
   dendrite = nullptr;
  if (!dendrite)
   continue;
  NPulseSynapseCommon *synapse = dendrite->GetExcitatorySynapses(0);
  if (!synapse)
   continue;

  // ������ ����� c ���������� ������� SignalGen �� ������ �������
  if (!CheckLink(SignalGen->GetLongName(GetThisAsSharedContainer()), "Output", synapse->GetLongName(GetThisAsSharedContainer()), "Input"))
   res &= CreateLink(SignalGen->GetLongName(GetThisAsSharedContainer()), "Output", synapse->GetLongName(GetThisAsSharedContainer()), "Input");


  std::weak_ptr<RDK::UContainer> dendrite_weak2 = ZoneNeurons[i]->GetComponentL("Dendrite2_1", true);
  if(!dendrite_weak2.expired())
   dendrite = std::dynamic_pointer_cast<NPulseMembrane>(dendrite_weak2.lock());
  else
   dendrite = nullptr;
  if (!dendrite)
   continue;
  synapse = dendrite->GetExcitatorySynapses(0);
  if (!synapse)
   continue;

  // ������ ����� c ���������� ����������������� �������� SinchroGen �� ������ �������
  if (!CheckLink(SinchroGen->GetLongName(GetThisAsSharedContainer()), "Output", synapse->GetLongName(GetThisAsSharedContainer()), "Input"))
   res &= CreateLink(SinchroGen->GetLongName(GetThisAsSharedContainer()), "Output", synapse->GetLongName(GetThisAsSharedContainer()), "Input");
 }


 // ������ �������� ��������� ����� ����� ���������
 for (int i = 0; i < NumZones; i++)
 {
  std::weak_ptr<RDK::UContainer> soma1_weak = ZoneNeurons[i]->GetComponentL(std::string("Soma1"), true);
  std::shared_ptr<NPulseMembrane> soma1;
  if(!soma1_weak.expired())
   soma1 = std::dynamic_pointer_cast<NPulseMembrane>(soma1_weak.lock());
  else
   soma1 = nullptr;
  std::weak_ptr<RDK::UContainer> soma2_weak = ZoneNeurons[i]->GetComponentL(std::string("Soma2"), true);
  std::shared_ptr<NPulseMembrane> soma2;
  if(!soma2_weak.expired())
   soma2 = std::dynamic_pointer_cast<NPulseMembrane>(soma2_weak.lock());
  else
   soma2 = nullptr;
  if(!soma1 || !soma2)
   continue;

  int index_synapse = 1;

  for (int j = 0; j < NumZones; j++)
  {
   if (i == j)
    continue;

   std::weak_ptr<RDK::UContainer> soma1_synapse_weak = soma1->GetComponentL(std::string("InhSynapse" + sntoa(index_synapse)), true);
   std::shared_ptr<NPulseSynapse> soma1_synapse;
   if(!soma1_synapse_weak.expired())
    soma1_synapse = std::dynamic_pointer_cast<NPulseSynapse>(soma1_synapse_weak.lock());
   else
    soma1_synapse = nullptr;
   std::weak_ptr<RDK::UContainer> soma2_synapse_weak = soma2->GetComponentL(std::string("InhSynapse" + sntoa(index_synapse)), true);
   std::shared_ptr<NPulseSynapse> soma2_synapse;
   if(!soma2_synapse_weak.expired())
    soma2_synapse = std::dynamic_pointer_cast<NPulseSynapse>(soma2_synapse_weak.lock());
   else
    soma2_synapse = nullptr;
   if(!soma1_synapse || !soma2_synapse)
       return true;

   if (!CheckLink(ZoneNeurons[j]->GetLongName(GetThisAsSharedContainer()), "Output", soma1_synapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink(ZoneNeurons[j]->GetLongName(GetThisAsSharedContainer()), "Output", soma1_synapse->GetLongName(GetThisAsSharedContainer()), "Input");

   if (!CheckLink(ZoneNeurons[j]->GetLongName(GetThisAsSharedContainer()), "Output", soma2_synapse->GetLongName(GetThisAsSharedContainer()), "Input"))
    res &= CreateLink(ZoneNeurons[j]->GetLongName(GetThisAsSharedContainer()), "Output", soma2_synapse->GetLongName(GetThisAsSharedContainer()), "Input");

   index_synapse++;
  }
 }

 return res;
}


// Reset computation
bool NSignalEstimation::AReset(void)
{
 bool res = true;

 // ����������� ���������� - ����� ���������
 std::shared_ptr <NPulseGeneratorTransit> generators[2]{SignalGen, SinchroGen};
 for (int i = 0; i < 2; i++)
 {
  if(generators[i])
  {
   generators[i]->Amplitude = Amplitude;
   generators[i]->PulseLength = PulseLength;
  }
 }
 // �������� ��������� ������������ �������
 if (UseTransitSignal)
 {
  SignalGen->Frequency = 0;
  SignalGen->Delay = 0;
 }
 else
 {
  SignalGen->Frequency = Frequency;
  SignalGen->Delay = DelaySignal;
 }
 // �������� ��������� ���������������� ���������
 if (UseTransitSinchro)
 {
  SinchroGen->Frequency = 0;
  SinchroGen->Delay = 0;
 }
 else
 {
  SinchroGen->Frequency = Frequency;
  SinchroGen->Delay = DelaySinchro;
 }


 // ����������� �������
 for (int i = 0; i < NumZones; i++)
 {
  if(!ZoneNeurons[i])
   continue;
  std::weak_ptr<RDK::UContainer> ltzone_weak = ZoneNeurons[i]->GetComponentL("LTZone");  // GetLTZone();
  std::shared_ptr<NLTZone> ltzone;
  if(!ltzone_weak.expired())
   ltzone = std::dynamic_pointer_cast<NLTZone>(ltzone_weak.lock());
  else
   ltzone = nullptr;
  if(!ltzone)
   continue;
  ltzone->Threshold = LTZThreshold;
 }


 // ����������� ������������� ��� �������� �� ��������� �� 1 �� 31
 // ��� ������� �� ��������� 86�, �� ����� ����������� �������� �����. ����. 50�
 vector<double> synapse_resistance = {50.0, 55.0, 43.0, 37.0, 32.5, 29.5, 27.0, 25.0,
                                      23.7, 22.5, 21.5, 20.5, 19.6, 18.9, 18.2, 17.6,
                                      17.0, 16.5, 16.1, 15.7, 15.3, 14.9, 14.6, 14.3,
                                      14.0, 13.7, 13.4, 13.2, 13.0, 12.8, 12.5};

 // ����������� ������� �� ���������
 for (int i = 0; i < NumZones; i++)
 {
  // ������ �������
  int dendrite_num = int(UpperLimitsOfZones[NumZones - i - 1] / 0.01 - 1);
  std::weak_ptr<RDK::UContainer> dendrite_weak = ZoneNeurons[i]->GetComponentL("Dendrite1_"+sntoa(dendrite_num), true);
  std::shared_ptr<NPulseMembrane> dendrite;
  if(!dendrite_weak.expired())
   dendrite = std::dynamic_pointer_cast<NPulseMembrane>(dendrite_weak.lock());
  else
   dendrite = nullptr;
  if (!dendrite)
   continue;
  NPulseSynapseCommon *synapse = dendrite->GetExcitatorySynapses(0);
  if (!synapse)
   continue;
  synapse->Resistance = synapse_resistance[dendrite_num - 1] * 1000000;

  // ������ �������
  std::weak_ptr<RDK::UContainer> dendrite_weak2 = ZoneNeurons[i]->GetComponentL("Dendrite2_1", true);
  if(!dendrite_weak2.expired())
   dendrite = std::dynamic_pointer_cast<NPulseMembrane>(dendrite_weak2.lock());
  else
   dendrite = nullptr;
  if (!dendrite)
   continue;
  synapse = dendrite->GetExcitatorySynapses(0);
  if (!synapse)
   continue;
  synapse->Resistance = synapse_resistance[0] * 1000000;
 }

 return res;
}

// Execute math. computations of current object on current step
bool NSignalEstimation::ACalculate(void)
{
 bool res = true;

 // ������� ������� ���������� �� ������ ���������� �� ���������� ���������,
 // ���� ������� ��������������� �����
 if (UseTransitSignal)
  *SignalGen->Input = *Input;

 if (UseTransitSinchro)
  *SinchroGen->Input = *Sinchro;

 return res;
}
// --------------------------
/* *************************************************************************** */

}
//---------------------------------------------------------------------------


