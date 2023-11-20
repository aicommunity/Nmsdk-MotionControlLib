/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2012.
E-mail:        alexab@ailab.ru
url:        http://ailab.ru

This file - part of the project: RDK

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */


#include "NSuppressionUnit.h"

//---------------------------------------------------------------------------
namespace NMSDK {

/* *************************************************************************** */
// --------------------------
// Constructors & destructors
// --------------------------
NSuppressionUnit::NSuppressionUnit(void)
 : PulseGeneratorClassName("PulseGeneratorClassName", this, &NSuppressionUnit::SetPulseGeneratorClassName),
   NeuronClassName("NeuronClassName", this, &NSuppressionUnit::SetNeuronClassName),
   SynapseClassName("SynapseClassName", this, &NSuppressionUnit::SetSynapseClassName),
   PulseLength("PulseLength", this, &NSuppressionUnit::SetPulseLength),
   Amplitude("Amplitude", this, &NSuppressionUnit::SetAmplitude),
   SuppressionFreq("SuppressionFreq", this, &NSuppressionUnit::SetSuppressionFreq),
   LTZThreshold("LTZThreshold", this, &NSuppressionUnit::SetLTZThreshold),
   Delay1("Delay1", this, &NSuppressionUnit::SetDelay1),
   Delay2("Delay2", this, &NSuppressionUnit::SetDelay2),
   SourceFreq("SourceFreq", this, &NSuppressionUnit::SetSourceFreq),
   SourceDelay("SourceDelay", this, &NSuppressionUnit::SetSourceDelay),
   TransitInput("TransitInput", this, &NSuppressionUnit::SetTransitInput),
   OnlyInhibition("OnlyInhibition", this, &NSuppressionUnit::SetOnlyInhibition),
   SingleUse("SingleUse", this, &NSuppressionUnit::SetSingleUse),
   Input("Input",this),
   Output("Output", this)
{
 SourceGenerator = NULL;
 DelayGenerators.clear();
 ControlledGenerator = NULL;
 ORNeuron = NULL;
 Neuron = NULL;
}

NSuppressionUnit::~NSuppressionUnit(void)
{
}
// --------------------------


// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NSuppressionUnit* NSuppressionUnit::New(void)
{
 return new NSuppressionUnit;
}
// --------------------------


// --------------------------
// Скрытые методы управления компонентами
// --------------------------

/// Выполняет завершающие пользовательские действия
/// при добавлении дочернего компонента в этот объект
// Метод будет вызван только если comp был успешно добавлен в список компонент
bool NSuppressionUnit::AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer)
{
 return true;
}

/// Выполняет предварительные пользовательские действия
/// при удалении дочернего компонента из этого объекта
// Метод будет вызван только если comp существует в списке компонент
bool NSuppressionUnit::ADelComponent(UEPtr<UContainer> comp)
{
 return true;
}

// --------------------------


// --------------------------
// Методы управления общедоступными свойствами
// --------------------------
/// Установка имени класса генераторов импульсов
bool NSuppressionUnit::SetPulseGeneratorClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка имени класса нейрона
bool NSuppressionUnit::SetNeuronClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка имени класса синапсов
bool NSuppressionUnit::SetSynapseClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка частоты импульсов подавляющего генератора
bool NSuppressionUnit::SetSuppressionFreq(const double &value)
{
 if(value < 0)
  return false;

 if (!ControlledGenerator)
  return true;
 ControlledGenerator->PatternFrequency = value;
 ControlledGenerator->Reset();

 return true;
}

/// Установка длительности импульсов
bool NSuppressionUnit::SetPulseLength(const double &value)
{
 if(value <= 0)
  return false;

 if (!SourceGenerator)
  return true;
 SourceGenerator->PulseLength = value;
 SourceGenerator->Reset();

 for(size_t i = 0; i < DelayGenerators.size(); i++)
 {
  DelayGenerators[i]->PulseLength = value;
  DelayGenerators[i]->Reset();
 }

 if (!ControlledGenerator)
  return true;
 ControlledGenerator->PulseLength = value;

 return true;
}

/// Установка амплитуды импульсов
bool NSuppressionUnit::SetAmplitude(const double &value)
{
 if(value <= 0)
  return false;

 if (!SourceGenerator)
  return true;
 SourceGenerator->Amplitude = value;
 SourceGenerator->Reset();

 for(size_t i = 0; i < DelayGenerators.size(); i++)
 {
  DelayGenerators[i]->Amplitude = value;
  DelayGenerators[i]->Reset();
 }

 if (!ControlledGenerator)
  return true;
 ControlledGenerator->Amplitude = value;

 return true;
}

/// Установка используемого порога низкопороговой зоны нейрона
bool NSuppressionUnit::SetLTZThreshold(const double &value)
{
 if(value <= 0)
  return false;

 if (!Neuron)
  return true;

 UEPtr<NLTZone> ltzone = Neuron->GetComponentL<NLTZone>("LTZone");  // GetLTZone();
 if(!ltzone)
  return true;

 ltzone->Threshold = value;

 return true;
}

/// Установка момента времени Т1, с которого начинается подавление сигнала
bool NSuppressionUnit::SetDelay1(const double &value)
{
 if (!DelayGenerators[0])
  return true;

 DelayGenerators[0]->Delay = value;

 return true;
}

/// Установка момента времени Т2, до которого происходит подавление сигнала
bool NSuppressionUnit::SetDelay2(const double &value)
{
 if (!DelayGenerators[1])
  return true;

 if (value < DelayGenerators[0]->Delay)  // 2-я задержка не может быть меньше 1-й
  DelayGenerators[1]->Delay = DelayGenerators[0]->Delay;
 else
  DelayGenerators[1]->Delay = value;

 return true;
}

/// Установка частоты источника подавляемого сигнала (и генераторов задержек)
bool NSuppressionUnit::SetSourceFreq(const double &value)
{
 if(value < 0)
  return false;

 if (!TransitInput && SourceGenerator)
  SourceGenerator->Frequency = value;

 for (int i = 0; i < 2; i++)
 {
  if (DelayGenerators[i])
   DelayGenerators[i]->Frequency = value;
 }

 return true;
}

/// Установка задержки источника подавляемого сигнала
bool NSuppressionUnit::SetSourceDelay(const double &value)
{
 if(value < 0)
  return false;

 if (!TransitInput && SourceGenerator)
  SourceGenerator->Delay = value;

 return true;
}

/// Установка значения флага транзита сигнала внешнего источника
bool NSuppressionUnit::SetTransitInput(const bool &value)
{
 if (!SourceGenerator)
  return true;

 SourceGenerator->UseTransitSignal = value;

 // Если используется источник сигнала внутри компонента, то
 // применяем к нему пользовательнские настройки
 if (!value)
 {
  SourceGenerator->Frequency = SourceFreq;
  SourceGenerator->Delay = SourceDelay;
 }

 return true;
}

/// Установка значения флага использования только подавляющего сигнала
bool NSuppressionUnit::SetOnlyInhibition(const bool &value)
{
 SourceGenerator->SetActivity(!value);
 Neuron->SetActivity(!value);
 return true;
}

/// Установка значения флага единоразового подавления сигнала
bool NSuppressionUnit::SetSingleUse(const bool &value)
{
 for (int i = 0; i < 2; i++)
 {
  if (DelayGenerators[i])
   DelayGenerators[i]->Frequency = SourceFreq;
 }

 return true;
}

// --------------------------


// --------------------------
// Computation methods
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NSuppressionUnit::ADefault(void)
{
 PulseGeneratorClassName = "NPulseGeneratorTransit";
 NeuronClassName = "NSPNeuronGen";
 SynapseClassName = "NPSynapseBio";

 PulseLength = 0.001;
 Amplitude = 1.0;
 SuppressionFreq = 500.0;
 LTZThreshold = 0.0115;

 Delay1 = 0;
 Delay2 = 0;

 SourceFreq = 0.0;
 SourceDelay = 0.0;
 TransitInput = false;
 OnlyInhibition = false;
 SingleUse = false;

 Input->Assign(1,1,0.0);
 Output.Assign(1,1,0.0);

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NSuppressionUnit::ABuild(void)
{
 bool res = true;

 // Инициализируем генератор входных импульсов
 SourceGenerator = AddMissingComponent<NPulseGeneratorTransit>(std::string("Source"), PulseGeneratorClassName);
 SourceGenerator->SetCoord(MVector<double,3>(4.6, 3, 0));
 if (TransitInput)
  SourceGenerator->UseTransitSignal = true;  // Включаем режим транзита сигнала от внешнего источника
 SourceGenerator->DisconnectAll("Output");

 // Инициализируем генераторы управляющих импульсов
 DelayGenerators.resize(2);
 for(int i = 0; i < 2; i++)
 {
  DelayGenerators[i] = AddMissingComponent<NPulseGeneratorTransit>(std::string("Delay") + sntoa(i + 1), PulseGeneratorClassName);
  DelayGenerators[i]->SetCoord(MVector<double,3>(4.6, 6 + i * 2, 0));
  DelayGenerators[i]->DisconnectAll("Output");
 }

 // Инициализируем нейрон ИЛИ
 ORNeuron = AddMissingComponent<NPulseNeuron>(std::string("ORNeuron"), NeuronClassName);
 ORNeuron->SetCoord(MVector<double,3>(11.33, 7, 0));
 ORNeuron->DisconnectAll("Output");
 UEPtr<NPulseMembrane> orsoma = ORNeuron->GetComponentL<NPulseMembrane>("Soma1", true);
 if (orsoma)
 {
  orsoma->NumExcitatorySynapses = 2;
  orsoma->Build();
 }

 // Инициализируем подавляющий генератор
 ControlledGenerator = AddMissingComponent<NPulseGeneratorTransit>(std::string("ControlledGenerator"), PulseGeneratorClassName);
 ControlledGenerator->SetCoord(MVector<double,3>(18, 7, 0));
 ControlledGenerator->UsePatternOutput = true;  // Включаем режим генерации импульсов с повышенной частотой
 ControlledGenerator->DisconnectAll("Output");

 // Инициализируем нейрон
 Neuron = AddMissingComponent<NPulseNeuron>(std::string("Neuron"), NeuronClassName);
 Neuron->SetCoord(MVector<double,3>(18, 3, 0));

 UEPtr<NLTZone> ltzone = Neuron->GetComponentL<NLTZone>("LTZone");  // GetLTZone();
 if(!ltzone)
  return true;
 ltzone->Threshold = LTZThreshold;


 // Создаём связи между элементами

 // Создаём связь между ORNeuron и ControlledGenerator
 if (!CheckLink("ORNeuron", "Output", "ControlledGenerator", "Input"))
  res &= CreateLink("ORNeuron", "Output", "ControlledGenerator", "Input");


 // Находим нужные элементы в Neuron
 UEPtr<NPulseMembrane> soma = Neuron->GetComponentL<NPulseMembrane>("Soma1", true);
 if(!soma)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because Soma1 isn't exists: "));
  return true;
 }
 NPulseSynapseCommon *excsynapse = soma->GetExcitatorySynapses(0);
 NPulseSynapseCommon *inhsynapse = soma->GetInhibitorySynapses(0);
 if(!excsynapse || !inhsynapse)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because synapse isn't exists: "));
  return true;
 }

 // Создаём связь между ControlledGenerator и тормозным синапсом сомы Neuron
 if (!CheckLink("ControlledGenerator", "Output", inhsynapse->GetLongName(this), "Input"))
  res &= CreateLink("ControlledGenerator", "Output", inhsynapse->GetLongName(this), "Input");

 // Создаём связь между источником входного сигнала Source и синапсом сомы Neuron
 if (!CheckLink("Source", "Output", excsynapse->GetLongName(this), "Input"))
  res &= CreateLink("Source", "Output", excsynapse->GetLongName(this), "Input");


 // Находим составляющие компоненты ORNeuron
 if(!orsoma)
 {
  LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because ORNeuron->Soma1 isn't exists: "));
  return true;
 }

 // Создаём связи между Delay1, Delay2 и синапсами нейрона ORNeuron
 for (int i = 0; i < 2; i++)
 {
  NPulseSynapseCommon *orexcsynapse = orsoma->GetExcitatorySynapses(i);
  if(!orexcsynapse)
  {
   LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because ORNeuron->Soma1->ExcSynapse" + sntoa(i+1) + " isn't exists: "));
   return true;
  }

  // Создаём связь
  if (!CheckLink(DelayGenerators[i]->GetLongName(this), "Output", orexcsynapse->GetLongName(this), "Input"))
   res &= CreateLink(DelayGenerators[i]->GetLongName(this), "Output", orexcsynapse->GetLongName(this), "Input");
 }

 return res;
}


// Reset computation
bool NSuppressionUnit::AReset(void)
{
 Output.ToZero();

 DelayGenerators[0]->UseTransitSignal = false;
 for(int i = 0; i < 2; i++)
 {
  DelayGenerators[i]->PulseLength = PulseLength;
  DelayGenerators[i]->Amplitude = Amplitude;
  DelayGenerators[i]->Frequency = SourceFreq;
 }
 DelayGenerators[0]->Delay = Delay1;
 DelayGenerators[1]->Delay = Delay2;

 ControlledGenerator->PatternFrequency = SuppressionFreq;
 ControlledGenerator->PulseLength = PulseLength;
 ControlledGenerator->Amplitude = Amplitude;

 SourceGenerator->PulseLength = PulseLength;
 SourceGenerator->Amplitude = Amplitude;

 if (!TransitInput)
 {
  SourceGenerator->Frequency = SourceFreq;
  SourceGenerator->Delay = SourceDelay;
 }

 return true;
}


// Execute math. computations of current object on current step
bool NSuppressionUnit::ACalculate(void)
{
 // Передаём входную информацию со входа компонента на внутренний источник,
 // если поднят соответствующий флаг
 if (TransitInput)
  *SourceGenerator->Input = *Input;

 // Передаём выходную информацию
 if (OnlyInhibition)
  Output = ControlledGenerator->Output;
 else if(Neuron)
  Output = Neuron->Output;

 if (SingleUse)
 {
  // Первый генератор уже сработал - выключаем
  if (DelayGenerators[0]->Frequency != 0 && ControlledGenerator->IsInPatternMode)
   DelayGenerators[0]->Frequency = 0;
  // Второй также уже сработал - можем выключить
  if (DelayGenerators[1]->Frequency != 0 && !ControlledGenerator->IsInPatternMode && DelayGenerators[0]->Frequency == 0)
   DelayGenerators[1]->Frequency = 0;
 }

 return true;
}
// --------------------------
/* *************************************************************************** */

}
//---------------------------------------------------------------------------


