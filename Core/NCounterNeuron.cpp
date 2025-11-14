/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   October, 2023
/// @brief  Counter of impulses. Generate output impulse only after getting of MaxCount impulses


#include "NCounterNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {

/* *************************************************************************** */
// --------------------------
// Constructors & destructors
// --------------------------
NCounterNeuron::NCounterNeuron(void)
 : MembraneClassName("MembraneClassName", this, &NCounterNeuron::SetMembraneClassName),
   LTZoneClassName("LTZoneClassName", this, &NCounterNeuron::SetLTZoneClassName),
   ExcGeneratorClassName("ExcGeneratorClassName", this, &NCounterNeuron::SetExcGeneratorClassName),
   InhGeneratorClassName("InhGeneratorClassName", this, &NCounterNeuron::SetInhGeneratorClassName),
   MaxCount("MaxCount", this, &NCounterNeuron::SetMaxCount),
   CurCount("CurCount", this, &NCounterNeuron::SetCurCount),
   Input("Input", this)
{
 OldNumSoma = 0;
 Soma.clear();
}

NCounterNeuron::~NCounterNeuron(void)
{
}
// --------------------------


// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
NCounterNeuron* NCounterNeuron::New(void)
{
 return new NCounterNeuron;
}
// --------------------------


// --------------------------
// ������� ������ ���������� ������������
// --------------------------

/// ��������� ����������� ���������������� ��������
/// ��� ���������� ��������� ���������� � ���� ������
// ����� ����� ������ ������ ���� comp ��� ������� �������� � ������ ���������
bool NCounterNeuron::AAddComponent(std::shared_ptr<UContainer> comp, std::shared_ptr<UIPointer> pointer)
{
 return true;
}

/// ��������� ��������������� ���������������� ��������
/// ��� �������� ��������� ���������� �� ����� �������
// ����� ����� ������ ������ ���� comp ���������� � ������ ���������
bool NCounterNeuron::ADelComponent(std::shared_ptr<UContainer> comp)
{
 return true;
}

// --------------------------


// --------------------------
// ������ ���������� �������������� ����������
// --------------------------

/// ��������� ����� ������ ������� ��������
bool NCounterNeuron::SetMembraneClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// ��������� ����� ������ ������������ ����
bool NCounterNeuron::SetLTZoneClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// ��������� ����� ������ ���������, ��������� ������ ��� ������������� ������� ���������
bool NCounterNeuron::SetExcGeneratorClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// ��������� ����� ������ ���������, ��������� ������ ��� ���������� ������� ���������
bool NCounterNeuron::SetInhGeneratorClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// ��������� ����������� ��� ��������
bool NCounterNeuron::SetMaxCount(const int &value)
{
 if(value < 1)
  return false;

 if (value < CurCount)
 {
  SetCurCount(value);
  CurCount = value;
 }

 OldNumSoma = MaxCount;

 Ready = false;
 return true;
}

/// ��������� ������ ������� ��� ��������
bool NCounterNeuron::SetCurCount(const int &value)
{
 if(value < 0 || value > MaxCount)
  return false;

 Ready = false;
 return true;
}

// --------------------------


// --------------------------
// Computation methods
// --------------------------

/// �������� ����������� ������ � ������� ��� ���������� ����
bool NCounterNeuron::CreateSomaLinks(std::shared_ptr<NPulseMembrane> soma)
{
 bool res = true;
 std::shared_ptr<NPulseChannelCommon> channel1, channel2;

 // ������� ������ ��������
 channel1 = dynamic_pointer_cast<NPulseChannelCommon>(soma->GetComponent("ExcChannel",true));
 channel2 = dynamic_pointer_cast<NPulseChannelCommon>(soma->GetComponent("InhChannel",true));

 // GetThisAsSharedContainer() may throw bad_weak_ptr if object was created from raw pointer
 std::shared_ptr<UContainer> this_container;
 try {
  this_container = GetThisAsSharedContainer();
 } catch (const std::bad_weak_ptr&) {
  // Object not fully initialized - skip link creation
  LOG(WARNING) << "NCounterNeuron::CreateSomaLinks - bad_weak_ptr in GetThisAsSharedContainer(), skipping link creation";
  return true; // Return true to continue execution
 }
 if(!this_container) {
  LOG(WARNING) << "NCounterNeuron::CreateSomaLinks - GetThisAsSharedContainer() returned nullptr, skipping link creation";
  return true;
 }

 // ���������� ���� � �������������� ���� �������
 if(channel1)
  res&=CreateLink(channel1->GetLongName(this_container),"Output",LTZone->GetLongName(this_container),"Inputs");
 if(channel2)
  res&=CreateLink(channel2->GetLongName(this_container),"Output",LTZone->GetLongName(this_container),"Inputs");

 //   
 res&=CreateLink(LTZone->GetLongName(this_container), "Output", soma->GetLongName(this_container), "InputFeedbackSignal");

 //       
 if(channel1)
  res&=CreateLink("NegGenerator", "Output",channel1->GetLongName(this_container),"ChannelInputs");
 if(channel2)
  res&=CreateLink("PosGenerator","Output",channel2->GetLongName(this_container),"ChannelInputs");

 return res;
}

//       
bool NCounterNeuron::ADefault(void)
{
 if(!NPulseNeuronCommon::ADefault())
  return false;

 MembraneClassName = "NPMembrane";
 LTZoneClassName = "NPLTZone";
 ExcGeneratorClassName = "NPNeuronPosCGenerator";
 InhGeneratorClassName = "NPNeuronNegCGenerator";
 MaxCount = 1;
 CurCount = 1;
 TheSamePulse = false;

 Input->Assign(1,1,0.0);

 return true;
}

//     
//   
//    Reset()  Ready true
//   
bool NCounterNeuron::ABuild(void)
{
 bool res = true;

 std::shared_ptr<NPulseMembrane> membr;
 std::shared_ptr<UNet> gen_pos, gen_neg;

 //     
 LTZone = AddMissingComponent<NLTZone>("LTZone", LTZoneClassName);
 if(!LTZone)
 {
  // Component not found - this is OK during CreateClassSamples
  // Component will be created later when needed
  return true;
 }
 LTZone->SetCoord(MVector<double,3>(25.3, 3.67 + MaxCount, 0));
 LTZone->Threshold = 0.0029;  //   
 LTZone->DisconnectAll("Output");


 //  ,      
 if(!ExcGeneratorClassName->empty())
 {
  gen_pos = AddMissingComponent<UNet>("PosGenerator", ExcGeneratorClassName);
  if(!gen_pos)
  {
   // Component not found - this is OK during CreateClassSamples
   return true;
  }
  gen_pos->SetCoord(MVector<double,3>(4, 2, 0));
  gen_pos->DisconnectAll("Output");
 }
 else
  DelComponent("PosGenerator");

 //  ,      
 if(!InhGeneratorClassName->empty())
 {
  gen_neg=AddMissingComponent<UNet>("NegGenerator", InhGeneratorClassName);
  if(!gen_neg)
  {
   // Component not found - this is OK during CreateClassSamples
   return true;
  }
  gen_neg->SetCoord(MVector<double,3>(4, 7.3 + (MaxCount - 1) * 2, 0));
  gen_neg->DisconnectAll("Output");
 }
 else
  DelComponent("NegGenerator");


 //    
 for(int i = MaxCount; i < OldNumSoma; i++)
  DelComponent(std::string("Soma")+sntoa(i+1));


 //   
 Soma.resize(MaxCount);
 for(int i = 0; i < MaxCount; i++)
 {
  //    
  membr = AddMissingComponent<NPulseMembrane>(std::string("Soma")+sntoa(i+1), MembraneClassName);
  if(!membr)
  {
   // Component not found - this is OK during CreateClassSamples
   return true;
  }
  membr->SetCoord(MVector<double,3>(15.7, 4.67+ i * 2, 0));
  Soma[i] = membr;

 //      CurCount 
  if(i < CurCount)
   res &= CreateSomaLinks(membr);
  //     
  else
  {
   std::shared_ptr<NPulseChannelCommon> channel1 = dynamic_pointer_cast<NPulseChannelCommon>(membr->GetComponent("ExcChannel",true));
   std::shared_ptr<NPulseChannelCommon> channel2 = dynamic_pointer_cast<NPulseChannelCommon>(membr->GetComponent("InhChannel",true));
   channel1->DisconnectAll("Output");
   channel2->DisconnectAll("Output");
  }
 }

 if(!NPulseNeuronCommon::ABuild())
  res = false;

 return res;
}


// Reset computation
bool NCounterNeuron::AReset(void)
{
 TheSamePulse = false;

 return NPulseNeuronCommon::AReset();
}

// Execute math. computations of current object on current step
bool NCounterNeuron::ACalculate(void)
{
  bool res = true;
 // �������� ���������� �� ����� ���������� �� �������
 // !!! ������ ������������ � �������� ��������, �� ��������� ���� � ����������
 for(int i = 0; i < CurCount; i++)
 {
  NPulseSynapseCommon *synapse = Soma[i]->GetExcitatorySynapses(0);
  *synapse->Input = *Input;
 }

 // ��������� ����������� ��������
 if((Input()(0,0) >= 0.01) && (!TheSamePulse)) // ������ (������) ����������� �������
 {
  TheSamePulse = true;  // ������ �����: ������� �������
 }
 else if ((Input()(0,0) <= 0.01) && (TheSamePulse))  // ������ �� ����� ����
 {
  TheSamePulse = false;  // ������ �����: ������� ����������
  if (CurCount < MaxCount)
  {
   // ��������� ����� �����, ����������� ����
   CurCount++;
   res &= CreateSomaLinks(Soma[CurCount - 1]);
  }
 }

 return res & NPulseNeuronCommon::ACalculate();
}
// --------------------------
/* *************************************************************************** */

}
//---------------------------------------------------------------------------


