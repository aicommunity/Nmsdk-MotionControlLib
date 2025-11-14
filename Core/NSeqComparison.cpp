/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   November, 2023
/// @brief  The component for sequences comparison
/// �������� ������������ � ����������� ����� ��� ���������� ������ �������:
/// ��� ������� ���������� ������� ���� (� �� ���� �� �������, ������ �� ����)

#include "NSeqComparison.h"

//---------------------------------------------------------------------------
namespace NMSDK {

/* *************************************************************************** */
// --------------------------
// Constructors & destructors
// --------------------------
NSeqComparison::NSeqComparison(void)
 : NeuronClassName("NeuronClassName", this, &NSeqComparison::SetNeuronClassName),
   MultiGeneratorClassName("MultiGeneratorClassName", this, &NSeqComparison::SetMultiGeneratorClassName),
   PulseLength("PulseLength", this, &NSeqComparison::SetPulseLength),
   Amplitude("Amplitude", this, &NSeqComparison::SetAmplitude),
   Frequency("Frequency", this, &NSeqComparison::SetFrequency),
   LTZThreshold("LTZThreshold", this, &NSeqComparison::SetLTZThreshold),
   PulseCount("PulseCount", this, &NSeqComparison::SetPulseCount),
   NumClasses("NumClasses", this, &NSeqComparison::SetNumClasses),
   DelaysKFCls("DelaysKFCls", this, &NSeqComparison::SetDelaysKFCls),
   DelaysFrCls("DelaysFrCls", this, &NSeqComparison::SetDelaysFrCls),
   Output("Output", this)
{
 OldNumClasses = 0;
 KFClsSpikes.clear();
 FrClsSpikes.clear();
 CompNeuron = NULL;
}

NSeqComparison::~NSeqComparison(void) {}
// --------------------------


// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
NSeqComparison* NSeqComparison::New(void)
{
 return new NSeqComparison;
}
// --------------------------


// --------------------------
// ������� ������ ���������� ������������
// --------------------------

/// ��������� ����������� ���������������� ��������
/// ��� ���������� ��������� ���������� � ���� ������
// ����� ����� ������ ������ ���� comp ��� ������� �������� � ������ ���������
bool NSeqComparison::AAddComponent(std::shared_ptr<UContainer> comp, std::shared_ptr<UIPointer> pointer)
{
 return true;
}

/// ��������� ��������������� ���������������� ��������
/// ��� �������� ��������� ���������� �� ����� �������
// ����� ����� ������ ������ ���� comp ���������� � ������ ���������
bool NSeqComparison::ADelComponent(std::shared_ptr<UContainer> comp)
{
 return true;
}

// --------------------------


// --------------------------
// ������ ���������� �������������� ����������
// --------------------------

/// ��������� ����� ������ �������
bool NSeqComparison::SetNeuronClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// ��������� ����� ������ ���������� ������������������ ���������
bool NSeqComparison::SetMultiGeneratorClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// ��������� ������������ ���������
bool NSeqComparison::SetPulseLength(const double &value)
{
 if(value <= 0)
  return false;

 size_t temp_size = min(KFClsSpikes.size(), FrClsSpikes.size());

 // ����������� ���������� �������
 for (size_t i = 0; i < temp_size; i++)
 {
  KFClsSpikes[i]->PulseLength = value;
  FrClsSpikes[i]->PulseLength = value;
 }

 return true;
}

/// ��������� ��������� ���������
bool NSeqComparison::SetAmplitude(const double &value)
{
 if(value <= 0)
  return false;

 size_t temp_size = min(KFClsSpikes.size(), FrClsSpikes.size());

 // ����������� ���������� �������
 for (size_t i = 0; i < temp_size; i++)
 {
  KFClsSpikes[i]->Amplitude = value;
  FrClsSpikes[i]->Amplitude = value;
 }

 return true;
}

/// ��������� ������� ��������� (��) ���� �����������
bool NSeqComparison::SetFrequency(const double &value)
{
 if(value < 0)
  return false;

 size_t temp_size = min(KFClsSpikes.size(), FrClsSpikes.size());

 // ����������� ���������� �������
 for (size_t i = 0; i < temp_size; i++)
 {
  KFClsSpikes[i]->Frequency = value;
  FrClsSpikes[i]->Frequency = value;
 }

 return true;
}

/// ��������� ������������� ������ �������������� ���� �������
bool NSeqComparison::SetLTZThreshold(const double &value)
{
 if(value <= 0)
  return false;

 if (!CompNeuron)
  return true;

 std::shared_ptr<NLTZone> ltzone = CompNeuron->GetComponentL<NLTZone>("LTZone");  // GetLTZone();
 if(!ltzone)
  return true;

 ltzone->Threshold = value;

 return true;
}

/// ��������� ����������� ��������� �������
bool NSeqComparison::SetPulseCount(const int &value)
{
 if(value < 1)
  return false;

 size_t temp_size = min(KFClsSpikes.size(), FrClsSpikes.size());

 for (size_t i = 0; i < temp_size; i++)
 {
  KFClsSpikes[i]->PulseCount = value;
  KFClsSpikes[i]->Reset();
  FrClsSpikes[i]->PulseCount = value;
  FrClsSpikes[i]->Reset();
 }

 DelaysKFCls.Resize(NumClasses, value, 0.0);
 DelaysFrCls.Resize(NumClasses, value, 0.0);

 return true;
}

/// ��������� ���������� �������� ��� ���������
bool NSeqComparison::SetNumClasses(const int &value)
{
 if(value < 1)
  return false;

 OldNumClasses = NumClasses;

 Ready = false;
 return true;
}

/// ��������� ��������, �������� ����������� ��������� ������ �������
bool NSeqComparison::SetDelaysKFCls(const MDMatrix<double> &value)
{
 bool res = true;

 MDMatrix<double> delays;
 delays.Assign(1, PulseCount, 0.0);
 for(int i = 0; i < int(KFClsSpikes.size()); i++)
 {
  for (int j = 0; j < PulseCount; j++)
   delays[j] = value(i, j);

  KFClsSpikes[i]->Delays = delays;
  KFClsSpikes[i]->Reset();
 }

 return res;
}

/// ��������� ��������, �������� �������� ��������� ������ �������
bool NSeqComparison::SetDelaysFrCls(const MDMatrix<double> &value)
{
 bool res = true;

 MDMatrix<double> delays;
 delays.Assign(1, PulseCount, 0.0);
 for(int i = 0; i < int(FrClsSpikes.size()); i++)
 {
  for (int j = 0; j < PulseCount; j++)
   delays[j] = value(i, j);

  FrClsSpikes[i]->Delays = delays;
  FrClsSpikes[i]->Reset();
 }

 return res;
}
// --------------------------


// --------------------------
// Computation methods
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool NSeqComparison::ADefault(void)
{
 NeuronClassName = "NSPNeuronGen";
 MultiGeneratorClassName = "NPulseGeneratorMulti";

 PulseLength = 0.001;
 Amplitude = 1.0;
 Frequency = 0.0;
 LTZThreshold = 0.0115;

 PulseCount = 1;
 NumClasses = 1;

 DelaysKFCls.Assign(NumClasses, PulseCount, 0.0);
 DelaysFrCls.Assign(NumClasses, PulseCount, 0.0);
 Output.Assign(1,1,0.0);

 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NSeqComparison::ABuild(void)
{
 bool res = true;

 DelaysKFCls.Resize(NumClasses, PulseCount, 0.0);
 DelaysFrCls.Resize(NumClasses, PulseCount, 0.0);

 // ������� ������ ���������� ��������� ������� �������
 for(int i = NumClasses; i < OldNumClasses; i++)
 {
  DelComponent(std::string("KF_Cls")+sntoa(i+1)+std::string("_Spikes"));
  DelComponent(std::string("Fr_Cls")+sntoa(i+1)+std::string("_Spikes"));
 }

 // �������������� ���������� ��������� ������� �������
 KFClsSpikes.resize(NumClasses);
 FrClsSpikes.resize(NumClasses);
 for (int i = 0; i < NumClasses; i++)
 {
  KFClsSpikes[i] = AddMissingComponent<NPulseGeneratorMulti>(std::string("KF_Cls")+sntoa(i+1)+std::string("_Spikes"), MultiGeneratorClassName);
  if(!KFClsSpikes[i])
  {
   // Component not found - this is OK during CreateClassSamples
   return true;
  }
  KFClsSpikes[i]->SetCoord(MVector<double,3>(4, 2 + 5.33 * i, 0));
  KFClsSpikes[i]->DisconnectAll("Output");
  FrClsSpikes[i] = AddMissingComponent<NPulseGeneratorMulti>(std::string("Fr_Cls")+sntoa(i+1)+std::string("_Spikes"), MultiGeneratorClassName);
  if(!FrClsSpikes[i])
  {
   // Component not found - this is OK during CreateClassSamples
   return true;
  }
  FrClsSpikes[i]->SetCoord(MVector<double,3>(4, 4.33 + 5.33 * i, 0));
  FrClsSpikes[i]->DisconnectAll("Output");
 }


 // �������������� ������ ���
 CompNeuron = AddMissingComponent<NPulseNeuron>(std::string("CompNeuron"), NeuronClassName);
 if(!CompNeuron)
 {
  // Component not found - this is OK during CreateClassSamples
  return true;
 }
 CompNeuron->SetCoord(MVector<double,3>(11, 3.17 + 2.67 * (NumClasses - 1), 0));
 CompNeuron->DisconnectAll("Output");
 CompNeuron->NumSomaMembraneParts = NumClasses;

 /* ������� � ���������� ����������� ������������������� �� ��������
 // ������� ������ �������� ��������� � ������� ���
 for(int i = NumClasses; i < OldNumClasses; i++)
 {
  std::shared_ptr<NPulseMembrane> dendrite = CompNeuron->GetComponentL<NPulseMembrane>("Dendrite" + sntoa(i+1) + "_1", true);
  DelComponent(dendrite->GetLongName(GetThisAsSharedContainer()));
 }

 if(CompNeuron->StructureBuildMode != 2)
  CompNeuron->NumDendriteMembraneParts = NumClasses;
 else
 {
  std::vector<int> dend;
  dend.assign(NumClasses, 1);
  CompNeuron->NumDendriteMembranePartsVec = dend;
 }
 CompNeuron->Build();
 */
 CompNeuron->Reset();


 // ������ ����� ����� ������������ � ��������� �������
 for (int i = 0; i < NumClasses; i++)
 {
  std::shared_ptr<NPulseMembrane> dendrite = CompNeuron->GetComponentL<NPulseMembrane>("Dendrite" + sntoa(i+1) + "_1", true);
  std::shared_ptr<NPulseMembrane> soma = CompNeuron->GetComponentL<NPulseMembrane>("Soma" + sntoa(i+1), true);
  // �������� excsynapse, ���� ����� ������� ����� �� �������
//  NPulseSynapseCommon *excsynapse = dendrite->GetExcitatorySynapses(0);
  NPulseSynapseCommon *excsynapse = soma->GetExcitatorySynapses(0);
  NPulseSynapseCommon *inhsynapse = soma->GetInhibitorySynapses(0);

  if(!excsynapse || !inhsynapse)
  {
   LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because Synapse in CompNeuron isn't exists: "));
   return true;
  }

  if (!CheckLink(KFClsSpikes[i]->GetLongName(GetThisAsSharedContainer()), "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
   res &= CreateLink(KFClsSpikes[i]->GetLongName(GetThisAsSharedContainer()), "Output", excsynapse->GetLongName(GetThisAsSharedContainer()), "Input");

  if (!CheckLink(FrClsSpikes[i]->GetLongName(GetThisAsSharedContainer()), "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input"))
   res &= CreateLink(FrClsSpikes[i]->GetLongName(GetThisAsSharedContainer()), "Output", inhsynapse->GetLongName(GetThisAsSharedContainer()), "Input");
 }

 return res;
}


// Reset computation
bool NSeqComparison::AReset(void)
{
 bool res = true;

 Output.ToZero();

 size_t temp_size = min(KFClsSpikes.size(), FrClsSpikes.size());

 // ����������� ���������� �������
 for (size_t i = 0; i < temp_size; i++)
 {
  KFClsSpikes[i]->Amplitude = Amplitude;
  KFClsSpikes[i]->PulseLength = PulseLength;
  KFClsSpikes[i]->Frequency = Frequency;
  KFClsSpikes[i]->PulseCount = PulseCount;
  KFClsSpikes[i]->Reset();

  FrClsSpikes[i]->Amplitude = Amplitude;
  FrClsSpikes[i]->PulseLength = PulseLength;
  FrClsSpikes[i]->Frequency = Frequency;
  FrClsSpikes[i]->PulseCount = PulseCount;
  FrClsSpikes[i]->Reset();
 }

 // ����������� ����� ������������ ���� ������� �
 std::shared_ptr<NLTZone> ltzone = CompNeuron->GetComponentL<NLTZone>("LTZone", true);
 ltzone->Threshold = LTZThreshold;
 CompNeuron->UseAverageLTZonePotential = false;
 CompNeuron->Reset();

 return res;
}

// Execute math. computations of current object on current step
bool NSeqComparison::ACalculate(void)
{
 bool res = true;

 Output = CompNeuron->Output;

 return res;
}
// --------------------------
/* *************************************************************************** */

}
//---------------------------------------------------------------------------


