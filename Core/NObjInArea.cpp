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
   PulseLength("PulseLength", this, &NObjInArea::SetPulseLength),
   Amplitude("Amplitude", this, &NObjInArea::SetAmplitude),
   Frequency("Frequency", this, &NObjInArea::SetFrequency),
   HighFreq("HighFreq", this, &NObjInArea::SetHighFreq),
   Delay1_1("Delay1_1", this, &NObjInArea::SetDelay1_1),
   Delay1_2("Delay1_2", this, &NObjInArea::SetDelay1_2),
   Delay2_1("Delay2_1", this, &NObjInArea::SetDelay2_1),
   Delay2_2("Delay2_2", this, &NObjInArea::SetDelay2_2),
   PulseCount("PulseCount", this, &NObjInArea::SetPulseCount),
   NumObj("NumObj", this, &NObjInArea::SetNumObj),
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
bool NObjInArea::AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer)
{
 return true;
}

/// ��������� ��������������� ���������������� ��������
/// ��� �������� ��������� ���������� �� ����� �������
// ����� ����� ������ ������ ���� comp ���������� � ������ ���������
bool NObjInArea::ADelComponent(UEPtr<UContainer> comp)
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

/// ��������� ������������ ���������
bool NObjInArea::SetPulseLength(const double &value)
{
 if(value <= 0)
  return false;

 // ����������� ���������� �������
 for (int i = 0; i < ClsSpikeFr.size(); i++)
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

/// ��������� ������� ��������� ����������� �����������
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

/// ��������� ������� ������� t=0, � �������� ���������� ���������� ������� ������ 1
bool NObjInArea::SetDelay1_1(const double &value)
{
 if(value < 0)
  return false;

 if (!SuppressUnit1)
  return true;

 SuppressUnit1->Delay1 = value;
 SuppressUnit1->Reset();

 return true;
}

/// ��������� ������� ������� t=t1, �� �������� ���������� ���������� ������� ������ 1
bool NObjInArea::SetDelay1_2(const double &value)
{
 if(value < 0)
  return false;

 if (!SuppressUnit1)
  return true;

 SuppressUnit1->Delay2 = value;
 SuppressUnit1->Reset();

 return true;
}

/// ��������� ������� ������� t=t2, � �������� ���������� ���������� ������� ������ N
bool NObjInArea::SetDelay2_1(const double &value)
{
 if(value < 0)
  return false;

 if (!SuppressUnit2)
  return true;

 SuppressUnit2->Delay1 = value;
 SuppressUnit2->Reset();

 return true;
}

/// ��������� ������� ������� t=T, �� �������� ���������� ���������� ������� ������ N
bool NObjInArea::SetDelay2_2(const double &value)
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

// --------------------------


// --------------------------
// Computation methods
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool NObjInArea::ADefault(void)
{
 NeuronClassName = "NSPNeuronGen";

 PulseLength = 0.001;
 Amplitude = 1.0;
 Frequency = 0.0;
 HighFreq = 500.0;

 Delay1_1 = 0;
 Delay1_2 = 0;
 Delay2_1 = 0;
 Delay2_2 = 0;

 PulseCount = 1;
 NumObj = 1;

 Output.Assign(1,1,0.0);

 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NObjInArea::ABuild(void)
{
 bool res = true;

 // ������� ������ ���������� ��������� ������� �������
 for(int i = NumObj; i < OldNumObj; i++)
  DelComponent(std::string("Cls")+sntoa(i+1)+std::string("SpikeFr"));

 // �������������� ���������� ��������� ������� �������
 ClsSpikeFr.resize(NumObj);
 for (int i = 0; i < NumObj; i++)
 {
  ClsSpikeFr[i] = AddMissingComponent<NPulseGeneratorMulti>(std::string("Cls")+sntoa(i+1)+std::string("SpikeFr"), "NPulseGeneratorMulti");
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
 UEPtr<NPulseMembrane> orsoma = ORNeuron->GetComponentL<NPulseMembrane>("Soma1", true);
 if (orsoma)
 {
  orsoma->NumExcitatorySynapses = NumObj;
  orsoma->Build();
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

 UEPtr<NPulseMembrane> decdend = DecidingNeuron->GetComponentL<NPulseMembrane>("Dendrite1_1", true);
 UEPtr<NPulseMembrane> decsoma = DecidingNeuron->GetComponentL<NPulseMembrane>("Soma1", true);
 if (decsoma)
 {
  decsoma->NumInhibitorySynapses = 2;
  decsoma->Build();
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
 if(!orsoma)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because ORNeuron->Soma1 isn't exists: "));
  return true;
 }

 // ������ ����� ����� ClsSpikeFr � ��������� ������� ���
 for (int i = 0; i < NumObj; i++)
 {
  NPulseSynapseCommon *orexcsynapse = orsoma->GetExcitatorySynapses(i);
  if(!orexcsynapse)
  {
   LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because ORNeuron->Soma1->ExcSynapse" + sntoa(i+1) + " isn't exists: "));
   return true;
  }

  // ������ �����
  if (!CheckLink(ClsSpikeFr[i]->GetLongName(this), "Output", orexcsynapse->GetLongName(this), "Input"))
   res &= CreateLink(ClsSpikeFr[i]->GetLongName(this), "Output", orexcsynapse->GetLongName(this), "Input");
 }


  // ������� ������������ ���������� DecidingNeuron
 if(!decsoma)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because DecidingNeuron->Soma1 isn't exists: "));
  return true;
 }
 NPulseSynapseCommon *decexcsynapse1 = decdend->GetExcitatorySynapses(0);
 NPulseSynapseCommon *decinhsynapse1 = decsoma->GetInhibitorySynapses(0);
 NPulseSynapseCommon *decinhsynapse2 = decsoma->GetInhibitorySynapses(1);
 if(!decexcsynapse1 || !decinhsynapse1 || !decinhsynapse2)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because synapses in DecidingNeuron->Soma1 isn't exists: "));
  return true;
 }

 // ������ ����� ����� ORNeuron � ������������ �������� �������� DecidingNeuron
 if (!CheckLink("ORNeuron", "Output", decexcsynapse1->GetLongName(this), "Input"))
  res &= CreateLink("ORNeuron", "Output", decexcsynapse1->GetLongName(this), "Input");

 // ������ ����� ����� SuppressUnit1 � ��������� �������� ���� DecidingNeuron
 if (!CheckLink("SuppressUnit1", "Output", decinhsynapse1->GetLongName(this), "Input"))
  res &= CreateLink("SuppressUnit1", "Output", decinhsynapse1->GetLongName(this), "Input");

 // ������ ����� ����� SuppressUnit2 � ��������� �������� ���� DecidingNeuron
 if (!CheckLink("SuppressUnit2", "Output", decinhsynapse2->GetLongName(this), "Input"))
  res &= CreateLink("SuppressUnit2", "Output", decinhsynapse2->GetLongName(this), "Input");


 // ������ ����� ����� DecidingNeuron � ExcitatoryGen
 if (!CheckLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input"))
  res &= CreateLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input");


 // ������� ������������ ���������� ANDNeuron
 UEPtr<NPulseMembrane> andsoma1 = ANDNeuron->GetComponentL<NPulseMembrane>("Soma1", true);
 UEPtr<NPulseMembrane> andsoma2 = ANDNeuron->GetComponentL<NPulseMembrane>("Soma2", true);
 if(!andsoma1 || !andsoma2)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because ANDNeuron->Soma1 or ANDNeuron->Soma2 isn't exists: "));
  return true;
 }
 NPulseSynapseCommon *andexcsynapse11 = andsoma1->GetExcitatorySynapses(0);
 NPulseSynapseCommon *andexcsynapse12 = andsoma2->GetExcitatorySynapses(0);
 if(!andexcsynapse11 || !andexcsynapse12)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because synapses in ANDNeuron isn't exists: "));
  return true;
 }

 // ������ ����� ����� ExcitatoryGen � ������������ �������� ANDNeuron->Soma1
 if (!CheckLink("ExcitatoryGen", "Output", andexcsynapse11->GetLongName(this), "Input"))
  res &= CreateLink("ExcitatoryGen", "Output", andexcsynapse11->GetLongName(this), "Input");

 // ������� �������� �������� � ������ t=T
 UEPtr<NPulseGeneratorTransit> delayT = SuppressUnit2->GetComponentL<NPulseGeneratorTransit>("Delay2", true);
 if(!delayT)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because SuppressUnit2->Delay2 isn't exists: "));
  return true;
 }

 // ��������� ����� � SuppressUnit2->Delay2, ���� ��� ����
 if(delayT && CheckLink(delayT->GetLongName(this), "Output", "ExcitatoryGen", "Input"))
  res &= BreakLink(delayT->GetLongName(this), "Output", "ExcitatoryGen", "Input");

 // ������ ����� ����� ���������� �������� � ������ t=T � ������������ �������� ANDNeuron->Soma2
 if (!CheckLink(delayT->GetLongName(this), "Output", andexcsynapse12->GetLongName(this), "Input"))
  res &= CreateLink(delayT->GetLongName(this), "Output", andexcsynapse12->GetLongName(this), "Input");

 /* ������ ������� ��� ����� �� ���� ����
 // ������ ����� ����� ���������� �������� � ������ t=T � ExcitatoryGen
 if (!CheckLink(delayT->GetLongName(this), "Output", "ExcitatoryGen", "Input"))
  res &= CreateLink(delayT->GetLongName(this), "Output", "ExcitatoryGen", "Input");
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
 UEPtr<NPulseGeneratorTransit> delayT = SuppressUnit2->GetComponentL<NPulseGeneratorTransit>("Delay2", true);
 if(delayT && CheckLink(delayT->GetLongName(this), "Output", "ExcitatoryGen", "Input"))
  res &= BreakLink(delayT->GetLongName(this), "Output", "ExcitatoryGen", "Input");
 if(!CheckLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input"))
  res &= CreateLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input");
*/
 if (Relinked)
 {
  Relinked = false;
  this->ABuild();
 }

 // ����������� ���������� �������
 for (int i = 0; i < ClsSpikeFr.size(); i++)
 {
  ClsSpikeFr[i]->Amplitude = Amplitude;
  ClsSpikeFr[i]->PulseLength = PulseLength;
  ClsSpikeFr[i]->Frequency = Frequency;
  ClsSpikeFr[i]->PulseCount = PulseCount;
 }

 // ����������� ����������� �����
 SuppressUnit1->OnlyInhibition = true;
 SuppressUnit1->SuppressionFreq = HighFreq;
 SuppressUnit1->Delay1 = Delay1_1;
 SuppressUnit1->Delay2 = Delay1_2;
 SuppressUnit2->OnlyInhibition = true;
 SuppressUnit2->SuppressionFreq = HighFreq;
 SuppressUnit2->Delay1 = Delay2_1;
 SuppressUnit2->Delay2 = Delay2_2;

 // ����������� ������������ ���������
 ExcitatoryGen->UsePatternOutput = true;
 ExcitatoryGen->PatternFrequency = HighFreq;

 // ����������� ����� ������������ ���� ������� �
 UEPtr<NLTZone> ltzone = ANDNeuron->GetComponentL<NLTZone>("LTZone", true);
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

  UEPtr<NPulseGeneratorTransit> delayT = SuppressUnit2->GetComponentL<NPulseGeneratorTransit>("Delay2", true);
  if(delayT && !CheckLink(delayT->GetLongName(this), "Output", "ExcitatoryGen", "Input"))
   res &= CreateLink(delayT->GetLongName(this), "Output", "ExcitatoryGen", "Input");
  Relinked = true;
 }

 // ExcitatoryGen ������� ����������� ������� ������ � SuppressUnit2->Delay2 � ����������,
 // ������� ��������� ����� � SuppressUnit2->Delay2 � ������������ � DecidingNeuron ����� ��������� ���������
 if (!ExcitatoryGen->IsInPatternMode && Relinked)
 {
  UEPtr<NPulseGeneratorTransit> delayT = SuppressUnit2->GetComponentL<NPulseGeneratorTransit>("Delay2", true);
  if(delayT && CheckLink(delayT->GetLongName(this), "Output", "ExcitatoryGen", "Input"))
   res &= BreakLink(delayT->GetLongName(this), "Output", "ExcitatoryGen", "Input");

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


