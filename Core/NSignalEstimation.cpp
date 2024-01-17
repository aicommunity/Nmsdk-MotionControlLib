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
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NSignalEstimation* NSignalEstimation::New(void)
{
 return new NSignalEstimation;
}
// --------------------------


// --------------------------
// Скрытые методы управления компонентами
// --------------------------

/// Выполняет завершающие пользовательские действия
/// при добавлении дочернего компонента в этот объект
// Метод будет вызван только если comp был успешно добавлен в список компонент
bool NSignalEstimation::AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer)
{
 return true;
}

/// Выполняет предварительные пользовательские действия
/// при удалении дочернего компонента из этого объекта
// Метод будет вызван только если comp существует в списке компонент
bool NSignalEstimation::ADelComponent(UEPtr<UContainer> comp)
{
 return true;
}

// --------------------------


// --------------------------
// Методы управления общедоступными свойствами
// --------------------------

/// Установка имени класса нейронов
bool NSignalEstimation::SetNeuronClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка имени класса генераторов импульсов
bool NSignalEstimation::SetGeneratorClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка длительности импульсов
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

/// Установка амплитуды импульсов
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

/// Установка частоты генерации (Гц) всех компонентов
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

/// Установка используемого порога низкопороговой зоны нейронов
bool NSignalEstimation::SetLTZThreshold(const double &value)
{
 if(value <= 0)
  return false;

 for (int i = 0; i < ZoneNeurons.size(); i++)
 {
  if (!ZoneNeurons[i])
   continue;

  UEPtr<NLTZone> ltzone = ZoneNeurons[i]->GetComponentL<NLTZone>("LTZone");  // GetLTZone();
  if(!ltzone)
   continue;

  ltzone->Threshold = value;
 }

 return true;
}

/// Установка задержки сигнала, который будет зонироваться
bool NSignalEstimation::SetDelaySignal(const double &value)
{
 if(value < 0.0)
  return false;

 if (SignalGen)
  SignalGen->Delay = value;

 return true;
}

/// Установка задержки синхронизирующего импульса
bool NSignalEstimation::SetDelaySinchro(const double &value)
{
 if(value < 0.0)
  return false;

 if (SinchroGen)
  SinchroGen->Delay = value;

 return true;
}

/// Установка количества областей, на которые будет зонироваться сигнал
bool NSignalEstimation::SetNumZones(const int &value)
{
 if(value < 2)
  return false;

 OldNumZones = NumZones;

 UpperLimitsOfZones.Resize(1, value, 0.0);

 Ready = false;
 return true;
}

/// Установка верхних ограничений для каждой из областей зонирования
bool NSignalEstimation::SetUpperLimitsOfZones(const MDMatrix<double> &value)
{
 bool res = true;

 Ready = false;
 return res;
}

/// Установка необходимости транзита зонируемого сигнала от внешнего источника
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

/// Установка необходимости транзита сигнала на синхронизирующий генератор от внешнего источника
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
// Восстановление настроек по умолчанию и сброс процесса счета
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

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NSignalEstimation::ABuild(void)
{
 bool res = true;

 // Инициализируем генератор зонируемого сигнала
 SignalGen = AddMissingComponent<NPulseGeneratorTransit>(std::string("Signal"), GeneratorClassName);
 SignalGen->SetCoord(MVector<double,3>(4, 1.67, 0));
 SignalGen->DisconnectAll("Output");

 // Инициализируем генератор синхронизирующего сигнала
 SinchroGen = AddMissingComponent<NPulseGeneratorTransit>(std::string("Sinchro"), GeneratorClassName);
 SinchroGen->SetCoord(MVector<double,3>(4, 4, 0));
 SinchroGen->DisconnectAll("Output");


 ZoneNeurons.resize(NumZones);

 // Удаляем зонирующие нейроны
 for(int i = 0; i < OldNumZones; i++)
  DelComponent(std::string("Zone_")+sntoa(i+1));

 // Инициализируем зонирующие нейроны
 for (int i = 0; i < NumZones; i++)
 {
  ZoneNeurons[i] = AddMissingComponent<NPulseNeuron>(std::string("Zone_")+sntoa(i+1), NeuronClassName);
  ZoneNeurons[i]->SetCoord(MVector<double,3>(13, 1.67 + 2.33 * i, 0));
  ZoneNeurons[i]->DisconnectAll("Output");
  ZoneNeurons[i]->NumSomaMembraneParts = 2;
  ZoneNeurons[i]->Reset();

  // На каждой соме устанавливаем дополнительные торм. синапсы для обратных связей м/у нейронами
  for (int j = 1; j <= 2; j++)
  {
   UEPtr<NPulseMembrane> soma = ZoneNeurons[i]->GetComponentL<NPulseMembrane>("Soma" + sntoa(j), true);
   if (soma)
   {
    soma->NumInhibitorySynapses = NumZones - 1;
    soma->Build();
   }
  }

  // Устанавливаем число дендритов = задержки для каждой зоны
  if(ZoneNeurons[i]->StructureBuildMode != 2)
   ZoneNeurons[i]->NumDendriteMembraneParts = 2;
  else
  {
   std::vector<int> dend;
   dend.assign(2, 1);
   dend[0] = UpperLimitsOfZones[NumZones - i - 1] / 0.01 - 1;
   ZoneNeurons[i]->NumDendriteMembranePartsVec = dend;
  }

  ZoneNeurons[i]->Reset();
 }

 // Строим связи с генераторов на нейроны
 for (int i = 0; i < NumZones; i++)
 {
  int dendrite_num = UpperLimitsOfZones[NumZones - i - 1] / 0.01 - 1;
  UEPtr<NPulseMembrane> dendrite = ZoneNeurons[i]->GetComponentL<NPulseMembrane>("Dendrite1_"+sntoa(dendrite_num), true);
  if (!dendrite)
   continue;
  NPulseSynapseCommon *synapse = dendrite->GetExcitatorySynapses(0);
  if (!synapse)
   continue;

  // Создаём связь c генератора сигнала SignalGen на первый дендрит
  if (!CheckLink(SignalGen->GetLongName(this), "Output", synapse->GetLongName(this), "Input"))
   res &= CreateLink(SignalGen->GetLongName(this), "Output", synapse->GetLongName(this), "Input");


  dendrite = ZoneNeurons[i]->GetComponentL<NPulseMembrane>("Dendrite2_1", true);
  if (!dendrite)
   continue;
  synapse = dendrite->GetExcitatorySynapses(0);
  if (!synapse)
   continue;

  // Создаём связь c генератора синхронизирующего импульса SinchroGen на первый дендрит
  if (!CheckLink(SinchroGen->GetLongName(this), "Output", synapse->GetLongName(this), "Input"))
   res &= CreateLink(SinchroGen->GetLongName(this), "Output", synapse->GetLongName(this), "Input");
 }


 // Строим обратные тормозные связи между нейронами
 for (int i = 0; i < NumZones; i++)
 {
  UEPtr<NPulseMembrane> soma1 = ZoneNeurons[i]->GetComponentL<NPulseMembrane>(std::string("Soma1"), true);
  UEPtr<NPulseMembrane> soma2 = ZoneNeurons[i]->GetComponentL<NPulseMembrane>(std::string("Soma2"), true);
  if(!soma1 || !soma2)
   continue;

  int index_synapse = 1;

  for (int j = 0; j < NumZones; j++)
  {
   if (i == j)
    continue;

   UEPtr<NPulseSynapse> soma1_synapse = soma1->GetComponentL<NPulseSynapse>(std::string("InhSynapse" + sntoa(index_synapse)), true);
   UEPtr<NPulseSynapse> soma2_synapse = soma2->GetComponentL<NPulseSynapse>(std::string("InhSynapse" + sntoa(index_synapse)), true);
   if(!soma1_synapse || !soma2_synapse)
       return true;

   if (!CheckLink(ZoneNeurons[j]->GetLongName(this), "Output", soma1_synapse->GetLongName(this), "Input"))
    res &= CreateLink(ZoneNeurons[j]->GetLongName(this), "Output", soma1_synapse->GetLongName(this), "Input");

   if (!CheckLink(ZoneNeurons[j]->GetLongName(this), "Output", soma2_synapse->GetLongName(this), "Input"))
    res &= CreateLink(ZoneNeurons[j]->GetLongName(this), "Output", soma2_synapse->GetLongName(this), "Input");

   index_synapse++;
  }
 }

 return res;
}


// Reset computation
bool NSignalEstimation::AReset(void)
{
 bool res = true;

 // Настраиваем генераторы - общие настройки
 UEPtr <NPulseGeneratorTransit> generators[2]{SignalGen, SinchroGen};
 for (int i = 0; i < 2; i++)
 {
  if(generators[i])
  {
   generators[i]->Amplitude = Amplitude;
   generators[i]->PulseLength = PulseLength;
  }
 }
 // Отдельно генератор зорнируемого сигнала
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
 // Отдельно генератор синхронизирующий генератор
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


 // Настраиваем нейроны
 for (int i = 0; i < NumZones; i++)
 {
  if(!ZoneNeurons[i])
   continue;
  UEPtr<NLTZone> ltzone = ZoneNeurons[i]->GetComponentL<NLTZone>("LTZone");  // GetLTZone();
  if(!ltzone)
   continue;
  ltzone->Threshold = LTZThreshold;
 }


 // Необходимые сопротивления для синапсов на дендритах от 1 до 31
 // Для первого по умолчанию 86к, но здесь эмпирически выведено необх. знач. 50к
 vector<double> synapse_resistance = {50.0, 55.0, 43.0, 37.0, 32.5, 29.5, 27.0, 25.0,
                                      23.7, 22.5, 21.5, 20.5, 19.6, 18.9, 18.2, 17.6,
                                      17.0, 16.5, 16.1, 15.7, 15.3, 14.9, 14.6, 14.3,
                                      14.0, 13.7, 13.4, 13.2, 13.0, 12.8, 12.5};

 // Настраиваем синапсы на дендритах
 for (int i = 0; i < NumZones; i++)
 {
  // Первый дендрит
  int dendrite_num = UpperLimitsOfZones[NumZones - i - 1] / 0.01 - 1;
  UEPtr<NPulseMembrane> dendrite = ZoneNeurons[i]->GetComponentL<NPulseMembrane>("Dendrite1_"+sntoa(dendrite_num), true);
  if (!dendrite)
   continue;
  NPulseSynapseCommon *synapse = dendrite->GetExcitatorySynapses(0);
  if (!synapse)
   continue;
  synapse->Resistance = synapse_resistance[dendrite_num - 1] * 1000000;

  // Второй дендрит
  dendrite = ZoneNeurons[i]->GetComponentL<NPulseMembrane>("Dendrite2_1", true);
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

 // Передаём входную информацию со входов компонента на внутренние источники,
 // если подняты соответствующие флаги
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


