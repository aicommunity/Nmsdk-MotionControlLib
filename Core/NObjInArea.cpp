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
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NObjInArea* NObjInArea::New(void)
{
 return new NObjInArea;
}
// --------------------------


// --------------------------
// Скрытые методы управления компонентами
// --------------------------

/// Выполняет завершающие пользовательские действия
/// при добавлении дочернего компонента в этот объект
// Метод будет вызван только если comp был успешно добавлен в список компонент
bool NObjInArea::AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer)
{
 return true;
}

/// Выполняет предварительные пользовательские действия
/// при удалении дочернего компонента из этого объекта
// Метод будет вызван только если comp существует в списке компонент
bool NObjInArea::ADelComponent(UEPtr<UContainer> comp)
{
 return true;
}

// --------------------------


// --------------------------
// Методы управления общедоступными свойствами
// --------------------------

/// Установка имени класса нейрона
bool NObjInArea::SetNeuronClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка имени класса генератора последовательности импульсов
bool NObjInArea::SetMultiGeneratorClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка длительности импульсов
bool NObjInArea::SetPulseLength(const double &value)
{
 if(value <= 0)
  return false;

 // Настраиваем генераторы образов
 for (int i = 0; i < ClsSpikeFr.size(); i++)
 {
  if (!ClsSpikeFr[i])
   continue;
  ClsSpikeFr[i]->PulseLength = value;
 }


 // Настраиваем блоки подавления
 if (!SuppressUnit1 || !SuppressUnit2)
  return true;
 SuppressUnit1->PulseLength = value;
 SuppressUnit1->Reset();
 SuppressUnit2->PulseLength = value;
 SuppressUnit2->Reset();

 // Настраиваем возбуждающий генератор
 if (!ExcitatoryGen)
  return true;
 ExcitatoryGen->PulseLength = value;

 return true;
}

/// Установка амплитуды импульсов
bool NObjInArea::SetAmplitude(const double &value)
{
 if(value <= 0)
  return false;

 // Настраиваем генераторы образов
 for (int i = 0; i < ClsSpikeFr.size(); i++)
 {
  if (!ClsSpikeFr[i])
   continue;
  ClsSpikeFr[i]->Amplitude = value;
 }


 // Настраиваем блоки подавления
 if (!SuppressUnit1 || !SuppressUnit2)
  return true;
 SuppressUnit1->Amplitude = value;
 SuppressUnit1->Reset();
 SuppressUnit2->Amplitude = value;
 SuppressUnit2->Reset();

 // Настраиваем возбуждающий генератор
 if (!ExcitatoryGen)
  return true;
 ExcitatoryGen->Amplitude = value;

 return true;
}

/// Установка частоты генерации (Гц) всех компонентов
bool NObjInArea::SetFrequency(const double &value)
{
 if(value < 0)
  return false;

 // Настраиваем генераторы образов
 for (int i = 0; i < ClsSpikeFr.size(); i++)
 {
  if (!ClsSpikeFr[i])
   continue;
  ClsSpikeFr[i]->Frequency = value;
 }


 // Настраиваем блоки подавления
 if (!SuppressUnit1 || !SuppressUnit2)
  return true;
 SuppressUnit1->SourceFreq = value;
 SuppressUnit2->SourceFreq = value;
 SuppressUnit1->Reset();
 SuppressUnit2->Reset();

 // Настраиваем возбуждающий генератор
 if (!ExcitatoryGen)
  return true;
 ExcitatoryGen->Frequency = value;

 return true;
}

/// Установка частоты импульсов подавляющих и возбуждающего генераторов
bool NObjInArea::SetHighFreq(const double &value)
{
 if(value < 0)
  return false;

  // Настраиваем блоки подавления
 if (!SuppressUnit1 || !SuppressUnit2 || !ExcitatoryGen)
  return true;

 SuppressUnit1->SuppressionFreq = value;
 SuppressUnit1->Reset();
 SuppressUnit2->SuppressionFreq = value;
 SuppressUnit2->Reset();
 ExcitatoryGen->PatternFrequency = value;

 return true;
}

/// Установка момента времени t=t0, с которого начинается подавление области кадра с левой стороны
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

/// Установка момента времени t=t1, до которого происходит подавление области кадра с левой стороны
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

/// Установка момента времени t=t2, с которого начинается подавление области кадра с правой стороны
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

/// Установка момента времени t=T, до которого происходит подавление области кадра с правой стороны
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

/// Установка размерности спайковых образов
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

/// Установка количества объектов для распознавания
bool NObjInArea::SetNumObj(const int &value)
{
 if(value < 1)
  return false;

 OldNumObj = NumObj;

 Ready = false;
 return true;
}

/// Установка задержек, задающих спайковые образы классов
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
// Восстановление настроек по умолчанию и сброс процесса счета
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

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NObjInArea::ABuild(void)
{
 bool res = true;

 DelaysClsSpikeFr.Resize(NumObj, PulseCount, 0.0);

 // Удаляем лишние генераторы спайковых образов классов
 for(int i = NumObj; i < OldNumObj; i++)
  DelComponent(std::string("Cls")+sntoa(i+1)+std::string("SpikeFr"));

 // Инициализируем генераторы спайковых образов классов
 ClsSpikeFr.resize(NumObj);
 for (int i = 0; i < NumObj; i++)
 {
  ClsSpikeFr[i] = AddMissingComponent<NPulseGeneratorMulti>(std::string("Cls")+sntoa(i+1)+std::string("SpikeFr"), MultiGeneratorClassName);
  ClsSpikeFr[i]->SetCoord(MVector<double,3>(4, 2 + 2.33 * i, 0));
  ClsSpikeFr[i]->DisconnectAll("Output");
 }


 // Инициализируем подавляющие блоки
 SuppressUnit1 = AddMissingComponent<NSuppressionUnit>(std::string("SuppressUnit1"), "NSuppressionUnit");
 SuppressUnit1->SetCoord(MVector<double,3>(4, 5.33 + 2.33 * (NumObj - 1), 0));
 SuppressUnit1->DisconnectAll("Output");

 SuppressUnit2 = AddMissingComponent<NSuppressionUnit>(std::string("SuppressUnit2"), "NSuppressionUnit");
 SuppressUnit2->SetCoord(MVector<double,3>(4, 5.33 + 2.33 * NumObj, 0));
 SuppressUnit2->DisconnectAll("Output");


 // Инициализируем нейрон ИЛИ
 ORNeuron = AddMissingComponent<NPulseNeuron>(std::string("ORNeuron"), NeuronClassName);
 ORNeuron->SetCoord(MVector<double,3>(10.33, 2 + 2.33 * (NumObj - 1) / 2, 0));
 ORNeuron->DisconnectAll("Output");
 UEPtr<NPulseMembrane> or_soma = ORNeuron->GetComponentL<NPulseMembrane>("Soma1", true);
 if (or_soma)
 {
  or_soma->NumExcitatorySynapses = NumObj;
  or_soma->Build();
 }

 // Инициализируем решающий нейрон
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

 UEPtr<NPulseMembrane> decidning_dend = DecidingNeuron->GetComponentL<NPulseMembrane>("Dendrite1_1", true);
 UEPtr<NPulseMembrane> deciding_soma = DecidingNeuron->GetComponentL<NPulseMembrane>("Soma1", true);
 if (deciding_soma)
 {
  deciding_soma->NumInhibitorySynapses = 2;
  deciding_soma->Build();
 }

 // Инициализируем возбуждающий генератор
 ExcitatoryGen = AddMissingComponent<NPulseGeneratorTransit>(std::string("ExcitatoryGen"), "NPulseGeneratorTransit");
 ExcitatoryGen->SetCoord(MVector<double,3>(23, 4.83 + 2.33 * (NumObj - 1) / 2, 0));
 ExcitatoryGen->DisconnectAll("Output");

 // Инициализируем нейрон И
 ANDNeuron = AddMissingComponent<NPulseNeuron>(std::string("ANDNeuron"), NeuronClassName);
 ANDNeuron->SetCoord(MVector<double,3>(29.33, 4.83 + 2.33 * (NumObj - 1) / 2, 0));
 ANDNeuron->DisconnectAll("Output");
 ANDNeuron->NumSomaMembraneParts = 2;
 ANDNeuron->Build();


 // Строим связи между компонентами
 // Находим составляющие компоненты ORNeuron
 if(!or_soma)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because ORNeuron->Soma1 isn't exists: "));
  return true;
 }

 // Создаём связи между ClsSpikeFr и синапсами нейрона ИЛИ
 for (int i = 0; i < NumObj; i++)
 {
  NPulseSynapseCommon *or_excsynapse = or_soma->GetExcitatorySynapses(i);
  if(!or_excsynapse)
  {
   LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because ORNeuron->Soma1->ExcSynapse" + sntoa(i+1) + " isn't exists: "));
   return true;
  }

  // Создаём связь
  if (!CheckLink(ClsSpikeFr[i]->GetLongName(this), "Output", or_excsynapse->GetLongName(this), "Input"))
   res &= CreateLink(ClsSpikeFr[i]->GetLongName(this), "Output", or_excsynapse->GetLongName(this), "Input");
 }


  // Находим составляющие компоненты DecidingNeuron
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

 // Создаём связь между ORNeuron и возбуждающим синапсом дендрита DecidingNeuron
 if (!CheckLink("ORNeuron", "Output", deciding_excsynapse1->GetLongName(this), "Input"))
  res &= CreateLink("ORNeuron", "Output", deciding_excsynapse1->GetLongName(this), "Input");

 // Создаём связь между SuppressUnit1 и тормозным синапсом сомы DecidingNeuron
 if (!CheckLink("SuppressUnit1", "Output", deciding_inhsynapse1->GetLongName(this), "Input"))
  res &= CreateLink("SuppressUnit1", "Output", deciding_inhsynapse1->GetLongName(this), "Input");

 // Создаём связь между SuppressUnit2 и тормозным синапсом сомы DecidingNeuron
 if (!CheckLink("SuppressUnit2", "Output", deciding_inhsynapse2->GetLongName(this), "Input"))
  res &= CreateLink("SuppressUnit2", "Output", deciding_inhsynapse2->GetLongName(this), "Input");


 // Создаём связь между DecidingNeuron и ExcitatoryGen
 if (!CheckLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input"))
  res &= CreateLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input");


 // Находим составляющие компоненты ANDNeuron
 UEPtr<NPulseMembrane> and_soma1 = ANDNeuron->GetComponentL<NPulseMembrane>("Soma1", true);
 UEPtr<NPulseMembrane> and_soma2 = ANDNeuron->GetComponentL<NPulseMembrane>("Soma2", true);
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

 // Создаём связь между ExcitatoryGen и возбуждающим синапсом ANDNeuron->Soma1
 if (!CheckLink("ExcitatoryGen", "Output", and_excsynapse11->GetLongName(this), "Input"))
  res &= CreateLink("ExcitatoryGen", "Output", and_excsynapse11->GetLongName(this), "Input");

 // Находим источник импульса в момент t=T
 UEPtr<NPulseGeneratorTransit> delay_T = SuppressUnit2->GetComponentL<NPulseGeneratorTransit>("Delay2", true);
 if(!delay_T)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because SuppressUnit2->Delay2 isn't exists: "));
  return true;
 }

 // Разрываем связь с SuppressUnit2->Delay2, если она была
 if(delay_T && CheckLink(delay_T->GetLongName(this), "Output", "ExcitatoryGen", "Input"))
  res &= BreakLink(delay_T->GetLongName(this), "Output", "ExcitatoryGen", "Input");

 // Создаём связь между источником импульса в момент t=T и возбуждающим синапсом ANDNeuron->Soma2
 if (!CheckLink(delay_T->GetLongName(this), "Output", and_excsynapse12->GetLongName(this), "Input"))
  res &= CreateLink(delay_T->GetLongName(this), "Output", and_excsynapse12->GetLongName(this), "Input");

 /* Нельзя сделать две связи на один вход
 // Создаём связь между источником импульса в момент t=T и ExcitatoryGen
 if (!CheckLink(delay_T->GetLongName(this), "Output", "ExcitatoryGen", "Input"))
  res &= CreateLink(delay_T->GetLongName(this), "Output", "ExcitatoryGen", "Input");
*/

 return res;
}


// Reset computation
bool NObjInArea::AReset(void)
{
 bool res = true;

 Output.ToZero();

 /*
 // Строим связь ExcitatoryGen только с DecidingNeuron
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

 // Настраиваем генераторы образов
 for (int i = 0; i < ClsSpikeFr.size(); i++)
 {
  ClsSpikeFr[i]->Amplitude = Amplitude;
  ClsSpikeFr[i]->PulseLength = PulseLength;
  ClsSpikeFr[i]->Frequency = Frequency;
  ClsSpikeFr[i]->PulseCount = PulseCount;
 }

 // Настраиваем подавляющие блоки
 SuppressUnit1->OnlyInhibition = true;
 SuppressUnit1->SuppressionFreq = HighFreq;
 SuppressUnit1->Delay1 = Delay11;
 SuppressUnit1->Delay2 = Delay12;
 SuppressUnit2->OnlyInhibition = true;
 SuppressUnit2->SuppressionFreq = HighFreq;
 SuppressUnit2->Delay1 = Delay21;
 SuppressUnit2->Delay2 = Delay22;

 // Настраиваем возбуждающий генератор
 ExcitatoryGen->UsePatternOutput = true;
 ExcitatoryGen->PatternFrequency = HighFreq;

 // Настраиваем порог генераторной зоны нейрона И
 UEPtr<NLTZone> ltzone = ANDNeuron->GetComponentL<NLTZone>("LTZone", true);
 ltzone->Threshold = 0.048;

 return res;
}

// Execute math. computations of current object on current step
bool NObjInArea::ACalculate(void)
{
 bool res = true;

 Output = ANDNeuron->Output;

 // ExcitatoryGen получил входной сигнал с DecidingNeuron и перешел с режим генерации с повышенной частотой
 // разрываем связь с DecidingNeuron, тк вход только один, и подключаемся к SuppressUnit2->Delay2,
 // чтобы дождаться от него выключающего сигнала
 if (ExcitatoryGen->IsInPatternMode && !Relinked)
 {
  if(CheckLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input"))
   res &= BreakLink("DecidingNeuron", "Output", "ExcitatoryGen", "Input");

  UEPtr<NPulseGeneratorTransit> delay_T = SuppressUnit2->GetComponentL<NPulseGeneratorTransit>("Delay2", true);
  if(delay_T && !CheckLink(delay_T->GetLongName(this), "Output", "ExcitatoryGen", "Input"))
   res &= CreateLink(delay_T->GetLongName(this), "Output", "ExcitatoryGen", "Input");
  Relinked = true;
 }

 // ExcitatoryGen получил выключающий входной сигнал с SuppressUnit2->Delay2 и выключился,
 // поэтому разрываем связь с SuppressUnit2->Delay2 и подключаемся к DecidingNeuron перед следующей итерацией
 if (!ExcitatoryGen->IsInPatternMode && Relinked)
 {
  UEPtr<NPulseGeneratorTransit> delay_T = SuppressUnit2->GetComponentL<NPulseGeneratorTransit>("Delay2", true);
  if(delay_T && CheckLink(delay_T->GetLongName(this), "Output", "ExcitatoryGen", "Input"))
   res &= BreakLink(delay_T->GetLongName(this), "Output", "ExcitatoryGen", "Input");

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


