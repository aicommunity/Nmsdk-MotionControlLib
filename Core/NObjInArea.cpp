/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   November, 2023
/// @brief  The component for a detection of an objects in restricted area of image

#include "NObjInArea.h"

//---------------------------------------------------------------------------
namespace NMSDK {

/* *************************************************************************** */
// --------------------------
// Constructors & destructors
// --------------------------
NObjInArea::NObjInArea(void)
 : NeuronClassName("NeuronClassName", this, &NObjInArea::SetNeuronClassName),
   MultiGeneratorClassName("MultiGeneratorClassName", this, &NObjInArea::SetMultiGeneratorClassName),
   PulseLength("PulseLength", this, &NObjInArea::SetPulseLength),
   Amplitude("Amplitude", this, &NObjInArea::SetAmplitude),
   Frequency("Frequency", this, &NObjInArea::SetFrequency),
   HighFreq("HighFreq", this, &NObjInArea::SetHighFreq),
   Delay11("Delay11", this, &NObjInArea::SetDelay11),
   Delay12("Delay12", this, &NObjInArea::SetDelay12),
   Delay21("Delay21", this, &NObjInArea::SetDelay21),
   Delay22("Delay22", this, &NObjInArea::SetDelay22),
   PulseCount("PulseCount", this, &NObjInArea::SetPulseCount),
   NumObj("NumObj", this, &NObjInArea::SetNumObj),
   DelaysClsSpikeFr("DelaysClsSpikeFr", this, &NObjInArea::SetDelaysClsSpikeFr),
   Output("Output", this)
{
 OldNumObj = 0;
 ClsSpikeFr.clear();
 SuppressUnit1 = NULL;
 SuppressUnit2 = NULL;
 ORNeuron = NULL;
 ANDNeuron = NULL;
 DecidingNeuron = NULL;
 ExcitatoryGen = NULL;
 Relinked = false;
}

NObjInArea::~NObjInArea(void)
{
}
// --------------------------


// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
NObjInArea* NObjInArea::New(void)
{
 return new NObjInArea;
}
// --------------------------


// --------------------------
// ������� ������ ���������� ������������
// --------------------------

/// ��������� ����������� ���������������� ��������
/// ��� ���������� ��������� ���������� � ���� ������
// ����� ����� ������ ������ ���� comp ��� ������� �������� � ������ ���������
bool NObjInArea::AAddComponent(std::shared_ptr<UContainer> comp, std::shared_ptr<UIPointer> pointer)
{
 return true;
}

/// ��������� ��������������� ���������������� ��������
/// ��� �������� ��������� ���������� �� ����� �������
// ����� ����� ������ ������ ���� comp ���������� � ������ ���������
bool NObjInArea::ADelComponent(std::shared_ptr<UContainer> comp)
{
 return true;
}

// --------------------------


// --------------------------
// ������ ���������� �������������� ����������
// --------------------------

/// ��������� ����� ������ �������
bool NObjInArea::SetNeuronClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// ��������� ����� ������ ���������� ������������������ ���������
bool NObjInArea::SetMultiGeneratorClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// ��������� ������������ ���������
bool NObjInArea::SetPulseLength(const double &value)
{
 if(value <= 0)
  return false;

 // ����������� ���������� �������
 for (size_t i = 0; i < ClsSpikeFr.size(); i++)
 {
  if (!ClsSpikeFr[i])
   continue;
  ClsSpikeFr[i]->PulseLength = value;
 }


 // ����������� ����� ����������
 if (!SuppressUnit1 || !SuppressUnit2)
  return true;
 SuppressUnit1->PulseLength = value;
 SuppressUnit1->Reset();
 SuppressUnit2->PulseLength = value;
 SuppressUnit2->Reset();

 // ����������� ������������ ���������
 if (!ExcitatoryGen)
  return true;
 ExcitatoryGen->PulseLength = value;

 return true;
}

/// ��������� ��������� ���������
bool NObjInArea::SetAmplitude(const double &value)
{
 if(value <= 0)
  return false;

 // ����������� ���������� �������
 for (int i = 0; i < ClsSpikeFr.size(); i++)
 {
  if (!ClsSpikeFr[i])
   continue;
  ClsSpikeFr[i]->Amplitude = value;
 }


 // ����������� ����� ����������
 if (!SuppressUnit1 || !SuppressUnit2)
  return true;
 SuppressUnit1->Amplitude = value;
 SuppressUnit1->Reset();
 SuppressUnit2->Amplitude = value;
 SuppressUnit2->Reset();

 // ����������� ������������ ���������
 if (!ExcitatoryGen)
  return true;
 ExcitatoryGen->Amplitude = value;

 return true;
}

/// ��������� ������� ��������� (��) ���� �����������
bool NObjInArea::SetFrequency(const double &value)
{
 if(value < 0)
  return false;

 // ����������� ���������� �������
 for (int i = 0; i < ClsSpikeFr.size(); i++)
 {
  if (!ClsSpikeFr[i])
   continue;
  ClsSpikeFr[i]->Frequency = value;
 }


 // ����������� ����� ����������
 if (!SuppressUnit1 || !SuppressUnit2)
  return true;
 SuppressUnit1->SourceFreq = value;
 SuppressUnit2->SourceFreq = value;
 SuppressUnit1->Reset();
 SuppressUnit2->Reset();

 // ����������� ������������ ���������
 if (!ExcitatoryGen)
  return true;
 ExcitatoryGen->Frequency = value;

 return true;
}

/// ��������� ������� ��������� ����������� � ������������� �����������
bool NObjInArea::SetHighFreq(const double &value)
{
 if(value < 0)
  return false;

  // ����������� ����� ����������
 if (!SuppressUnit1 || !SuppressUnit2 || !ExcitatoryGen)
  return true;

 SuppressUnit1->SuppressionFreq = value;
 SuppressUnit1->Reset();
 SuppressUnit2->SuppressionFreq = value;
 SuppressUnit2->Reset();
 ExcitatoryGen->PatternFrequency = value;

 return true;
}

/// ��������� ������� ������� t=t0, � �������� ���������� ���������� ������� ����� � ����� �������
bool NObjInArea::SetDelay11(const double &value)
{
 if(value < 0)
  return false;

 if (!SuppressUnit1)
  return true;

 SuppressUnit1->Delay1 = value;
 SuppressUnit1->Reset();

 return true;
}

/// ��������� ������� ������� t=t1, �� �������� ���������� ���������� ������� ����� � ����� �������
bool NObjInArea::SetDelay12(const double &value)
{
 if(value < 0)
  return false;

 if (!SuppressUnit1)
  return true;

 SuppressUnit1->Delay2 = value;
 SuppressUnit1->Reset();

 return true;
}

/// ��������� ������� ������� t=t2, � �������� ���������� ���������� ������� ����� � ������ �������
bool NObjInArea::SetDelay21(const double &value)
{
 if(value < 0)
  return false;

 if (!SuppressUnit2)
  return true;

 SuppressUnit2->Delay1 = value;
 SuppressUnit2->Reset();

 return true;
}

/// ��������� ������� ������� t=T, �� �������� ���������� ���������� ������� ����� � ������ �������
bool NObjInArea::SetDelay22(const double &value)
{
 if(value < 0)
  return false;

 if (!SuppressUnit2)
  return true;

 SuppressUnit2->Delay2 = value;
 SuppressUnit2->Reset();

 return true;
}

/// ��������� ����������� ��������� �������
bool NObjInArea::SetPulseCount(const int &value)
{
 if(value < 1)
  return false;

 for (int i = 0; i < ClsSpikeFr.size(); i++)
 {
  if (!ClsSpikeFr[i])
   continue;
  ClsSpikeFr[i]->PulseCount = value;
  ClsSpikeFr[i]->Reset();
 }

 DelaysClsSpikeFr.Resize(NumObj, value, 0.0);

 return true;
}

/// ��������� ���������� �������� ��� �������������
bool NObjInArea::SetNumObj(const int &value)
{
 if(value < 1)
  return false;

 OldNumObj = NumObj;

 Ready = false;
 return true;
}

/// ��������� ��������, �������� ��������� ������ �������
bool NObjInArea::SetDelaysClsSpikeFr(const MDMatrix<double> &value)
{
 bool res = true;

 for(int i = 0; i < int(ClsSpikeFr.size()); i++)
 {
  for (int j = 0; j < PulseCount; j++)
   ClsSpikeFr[i]->Delays[j] = value(i, j);

  ClsSpikeFr[i]->Reset();
 }

 return res;
}

// --------------------------


// --------------------------
// Computation methods
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool NObjInArea::ADefault(void)
{
 NeuronClassName = "NSPNeuronGen";
 MultiGeneratorClassName = "NPulseGeneratorMulti";

 PulseLength = 0.001;
 Amplitude = 1.0;
 Frequency = 0.0;
 HighFreq = 500.0;

 Delay11 = 0;
 Delay12 = 0;
 Delay21 = 0;
 Delay22 = 0;

 PulseCount = 1;
 NumObj = 1;

 Output.Assign(1,1,0.0);
 DelaysClsSpikeFr.Assign(NumObj, PulseCount, 0.0);

 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NObjInArea::ABuild(void)
{
 bool res = true;

 DelaysClsSpikeFr.Resize(NumObj, PulseCount, 0.0);

 // ������� ������ ���������� ��������� ������� �������
 for(int i = NumObj; i < OldNumObj; i++)
  DelComponent(std::string("Cls")+sntoa(i+1)+std::string("SpikeFr"));

 // �������������� ���������� ��������� ������� �������
 ClsSpikeFr.resize(NumObj);
 for (int i = 0; i < NumObj; i++)
 {
  ClsSpikeFr[i] = AddMissingComponent<NPulseGeneratorMulti>(std::string("Cls")+sntoa(i+1)+std::string("SpikeFr"), MultiGeneratorClassName);
  ClsSpikeFr[i]->SetCoord(MVector<double,3>(4, 2 + 2.33 * i, 0));
  ClsSpikeFr[i]->DisconnectAll("Output");
 }


 // �������������� ����������� �����
 SuppressUnit1 = AddMissingComponent<NSuppressionUnit>(std::string("SuppressUnit1"), "NSuppressionUnit");
 SuppressUnit1->SetCoord(MVector<double,3>(4, 5.33 + 2.33 * (NumObj - 1), 0));
 SuppressUnit1->DisconnectAll("Output");

 SuppressUnit2 = AddMissingComponent<NSuppressionUnit>(std::string("SuppressUnit2"), "NSuppressionUnit");
 SuppressUnit2->SetCoord(MVector<double,3>(4, 5.33 + 2.33 * NumObj, 0));
 SuppressUnit2->DisconnectAll("Output");


 // �������������� ������ ���
 ORNeuron = AddMissingComponent<NPulseNeuron>(std::string("ORNeuron"), NeuronClassName);
 ORNeuron->SetCoord(MVector<double,3>(10.33, 2 + 2.33 * (NumObj - 1) / 2, 0));
 ORNeuron->DisconnectAll("Output");
 std::shared_ptr<NPulseMembrane> or_soma = ORNeuron->GetComponentL<NPulseMembrane>("Soma1", true);
 if (or_soma)
 {
  or_soma->NumExcitatorySynapses = NumObj;
  or_soma->Build();
 }

 // �������������� �������� ������
 DecidingNeuron = AddMissingComponent<NPulseNeuron>(std::string("DecidingNeuron"), NeuronClassName);
 DecidingNeuron->SetCoord(MVector<double,3>(16.67, 4.83 + 2.33 * (NumObj - 1) / 2, 0));
 DecidingNeuron->DisconnectAll("Output");
 if(DecidingNeuron->StructureBuildMode != 2)
  DecidingNeuron->NumDendriteMembraneParts = 1;
 else
 {
  std::vector<int> dend;
  dend.assign(1, 1);
  DecidingNeuron->NumDendriteMembranePartsVec = dend;
 }
 DecidingNeuron->Reset();

 std::shared_ptr<NPulseMembrane> decidning_dend = DecidingNeuron->GetComponentL<NPulseMembrane>("Dendrite1_1", true);
 std::shared_ptr<NPulseMembrane> deciding_soma = DecidingNeuron->GetComponentL<NPulseMembrane>("Soma1", true);
 if (deciding_soma)
 {
  deciding_soma->NumInhibitorySynapses = 2;
  deciding_soma->Build();
 }

 // �������������� ������������ ���������
 ExcitatoryGen = AddMissingComponent<NPulseGeneratorTransit>(std::string("ExcitatoryGen"), "NPulseGeneratorTransit");
 ExcitatoryGen->SetCoord(MVector<double,3>(23, 4.83 + 2.33 * (NumObj - 1) / 2, 0));
 ExcitatoryGen->DisconnectAll("Output");

 // �������������� ������ �
 ANDNeuron = AddMissingComponent<NPulseNeuron>(std::string("ANDNeuron"), NeuronClassName);
 ANDNeuron->SetCoord(MVector<double,3>(29.33, 4.83 + 2.33 * (NumObj - 1) / 2, 0));
 ANDNeuron->DisconnectAll("Output");
 ANDNeuron->NumSomaMembraneParts = 2;
 ANDNeuron->Build();


 // ������ ����� ����� ������������
 // ������� ������������ ���������� ORNeuron
 if(!or_soma)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because ORNeuron->Soma1 isn't exists: "));
  return true;
 }

 // ������ ����� ����� ClsSpikeFr � ��������� ������� ���
 for (int i = 0; i < NumObj; i++)
 {
  NPulseSynapseCommon *or_excsynapse = or_soma->GetExcitatorySynapses(i);
  if(!or_excsynapse)
  {
   LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because ORNeuron->Soma1->ExcSynapse" + sntoa(i+1) + " isn't exists: "));
   return true;
  }

  // ������ �����
  if (!CheckLink(ClsSpikeFr[i]->GetLongName(GetThisAsSharedContainer()), "Output", or_excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
   res &= CreateLink(ClsSpikeFr[i]->GetLongName(GetThisAsSharedContainer()), "Output", or_excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
 }


  // ������� ������������ ���������� DecidingNeuron
 if(!deciding_soma)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because DecidingNeuron->Soma1 isn't exists: "));
  return true;
 }
 NPulseSynapseCommon *deciding_excsynapse1 = decidning_dend->GetExcitatorySynapses(0);
 NPulseSynapseCommon *deciding_inhsynapse1 = deciding_soma->GetInhibitorySynapses(0);
 NPulseSynapseCommon *deciding_inhsynapse2 = deciding_soma->GetInhibitorySynapses(1);
 if(!deciding_excsynapse1 || !deciding_inhsynapse1 || !deciding_inhsynapse2)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because synapses in DecidingNeuron->Soma1 isn't exists: "));
  return true;
 }

 // ������ ����� ����� ORNeuron � ������������ �������� �������� DecidingNeuron
 if (!CheckLink("ORNeuron", "Output", deciding_excsynapse1->GetLongName(GetThisAsSharedContainer()), "Input"))
  res &= CreateLink("ORNeuron", "Output", deciding_excsynapse1->GetLongName(GetThisAsSharedContainer()), "Input");

 // ������ ����� ����� SuppressUnit1 � ��������� �������� ���� DecidingNeuron
 if (!CheckLink("SuppressUnit1", "Output", deciding_inhsynapse1->GetLongName(GetThisAsSharedContainer()), "Input"))
  res &= CreateLink("SuppressUnit1", "Output", deciding_inhsynapse1->GetLongName(GetThisAsSharedContainer()), "Input");

 // ������ ����� ����� SuppressUnit2 � ��������� �������� ���� DecidingNeuron
 if (!CheckLink("SuppressUnit2", "Output", deciding_inhsynapse2->GetLongName(GetThisAsSharedContainer()), "Input"))
  res &= CreateLink("SuppressUnit2", "Output", deciding_inhsynapse2->GetLongName(GetThisAsSharedContainer()), "Input");


 // ������ ����� ����� DecidingNeuron � ExcitatoryGen
 if (!CheckLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input"))
  res &= CreateLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input");


 // ������� ������������ ���������� ANDNeuron
 std::shared_ptr<NPulseMembrane> and_soma1 = ANDNeuron->GetComponentL<NPulseMembrane>("Soma1", true);
 std::shared_ptr<NPulseMembrane> and_soma2 = ANDNeuron->GetComponentL<NPulseMembrane>("Soma2", true);
 if(!and_soma1 || !and_soma2)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because ANDNeuron->Soma1 or ANDNeuron->Soma2 isn't exists: "));
  return true;
 }
 NPulseSynapseCommon *and_excsynapse11 = and_soma1->GetExcitatorySynapses(0);
 NPulseSynapseCommon *and_excsynapse12 = and_soma2->GetExcitatorySynapses(0);
 if(!and_excsynapse11 || !and_excsynapse12)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because synapses in ANDNeuron isn't exists: "));
  return true;
 }

 // ������ ����� ����� ExcitatoryGen � ������������ �������� ANDNeuron->Soma1
 if (!CheckLink("ExcitatoryGen", "Output", and_excsynapse11->GetLongName(GetThisAsSharedContainer()), "Input"))
  res &= CreateLink("ExcitatoryGen", "Output", and_excsynapse11->GetLongName(GetThisAsSharedContainer()), "Input");

 // ������� �������� �������� � ������ t=T
 std::shared_ptr<NPulseGeneratorTransit> delay_T = SuppressUnit2->GetComponentL<NPulseGeneratorTransit>("Delay2", true);
 if(!delay_T)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because SuppressUnit2->Delay2 isn't exists: "));
  return true;
 }

 // ��������� ����� � SuppressUnit2->Delay2, ���� ��� ����
 if(delay_T && CheckLink(delay_T->GetLongName(GetThisAsSharedContainer()), "Output", "ExcitatoryGen", "Input"))
  res &= BreakLink(delay_T->GetLongName(GetThisAsSharedContainer()), "Output", "ExcitatoryGen", "Input");

 // ������ ����� ����� ���������� �������� � ������ t=T � ������������ �������� ANDNeuron->Soma2
 if (!CheckLink(delay_T->GetLongName(GetThisAsSharedContainer()), "Output", and_excsynapse12->GetLongName(GetThisAsSharedContainer()), "Input"))
  res &= CreateLink(delay_T->GetLongName(GetThisAsSharedContainer()), "Output", and_excsynapse12->GetLongName(GetThisAsSharedContainer()), "Input");

 /* ������ ������� ��� ����� �� ���� ����
 // ������ ����� ����� ���������� �������� � ������ t=T � ExcitatoryGen
 if (!CheckLink(delay_T->GetLongName(GetThisAsSharedContainer()), "Output", "ExcitatoryGen", "Input"))
  res &= CreateLink(delay_T->GetLongName(GetThisAsSharedContainer()), "Output", "ExcitatoryGen", "Input");
*/

 return res;
}


// Reset computation
bool NObjInArea::AReset(void)
{
 bool res = true;

 Output.ToZero();

 /*
 // ������ ����� ExcitatoryGen ������ � DecidingNeuron
 std::shared_ptr<NPulseGeneratorTransit> delayT = SuppressUnit2->GetComponentL<NPulseGeneratorTransit>("Delay2", true);
 if(delayT && CheckLink(delayT->GetLongName(GetThisAsSharedContainer()), "Output", "ExcitatoryGen", "Input"))
  res &= BreakLink(delayT->GetLongName(GetThisAsSharedContainer()), "Output", "ExcitatoryGen", "Input");
 if(!CheckLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input"))
  res &= CreateLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input");
*/
 if (Relinked)
 {
  Relinked = false;
  this->ABuild();
 }

 // ����������� ���������� �������
 for (size_t i = 0; i < ClsSpikeFr.size(); i++)
 {
  ClsSpikeFr[i]->Amplitude = Amplitude;
  ClsSpikeFr[i]->PulseLength = PulseLength;
  ClsSpikeFr[i]->Frequency = Frequency;
  ClsSpikeFr[i]->PulseCount = PulseCount;
 }

 // ����������� ����������� �����
 SuppressUnit1->OnlyInhibition = true;
 SuppressUnit1->SuppressionFreq = HighFreq;
 SuppressUnit1->Delay1 = Delay11;
 SuppressUnit1->Delay2 = Delay12;
 SuppressUnit2->OnlyInhibition = true;
 SuppressUnit2->SuppressionFreq = HighFreq;
 SuppressUnit2->Delay1 = Delay21;
 SuppressUnit2->Delay2 = Delay22;

 // ����������� ������������ ���������
 ExcitatoryGen->UsePatternOutput = true;
 ExcitatoryGen->PatternFrequency = HighFreq;

 // ����������� ����� ������������ ���� ������� �
 std::shared_ptr<NLTZone> ltzone = ANDNeuron->GetComponentL<NLTZone>("LTZone", true);
 ltzone->Threshold = 0.048;

 return res;
}

// Execute math. computations of current object on current step
bool NObjInArea::ACalculate(void)
{
 bool res = true;

 Output = ANDNeuron->Output;

 // ExcitatoryGen ������� ������� ������ � DecidingNeuron � ������� � ����� ��������� � ���������� ��������
 // ��������� ����� � DecidingNeuron, �� ���� ������ ����, � ������������ � SuppressUnit2->Delay2,
 // ����� ��������� �� ���� ������������ �������
 if (ExcitatoryGen->IsInPatternMode && !Relinked)
 {
  if(CheckLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input"))
   res &= BreakLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input");

  std::shared_ptr<NPulseGeneratorTransit> delay_T = SuppressUnit2->GetComponentL<NPulseGeneratorTransit>("Delay2", true);
  if(delay_T && !CheckLink(delay_T->GetLongName(GetThisAsSharedContainer()), "Output", "ExcitatoryGen", "Input"))
   res &= CreateLink(delay_T->GetLongName(GetThisAsSharedContainer()), "Output", "ExcitatoryGen", "Input");
  Relinked = true;
 }

 // ExcitatoryGen ������� ����������� ������� ������ � SuppressUnit2->Delay2 � ����������,
 // ������� ��������� ����� � SuppressUnit2->Delay2 � ������������ � DecidingNeuron ����� ��������� ���������
 if (!ExcitatoryGen->IsInPatternMode && Relinked)
 {
  std::shared_ptr<NPulseGeneratorTransit> delay_T = SuppressUnit2->GetComponentL<NPulseGeneratorTransit>("Delay2", true);
  if(delay_T && CheckLink(delay_T->GetLongName(GetThisAsSharedContainer()), "Output", "ExcitatoryGen", "Input"))
   res &= BreakLink(delay_T->GetLongName(GetThisAsSharedContainer()), "Output", "ExcitatoryGen", "Input");

  if(!CheckLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input"))
   res &= CreateLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input");
  Relinked = false;
 }

 return res;
}
// --------------------------
/* *************************************************************************** */

}
//---------------------------------------------------------------------------


