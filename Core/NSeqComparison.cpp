/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   November, 2023
/// @brief  The component for sequences comparison
/// Изменила предлагаемую в диссертации схему для правильной работы нейрона:
/// оба сигнала замыкаются синапсы сомы (а не один на дендрит, другой на сому)

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
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NSeqComparison* NSeqComparison::New(void)
{
 return new NSeqComparison;
}
// --------------------------


// --------------------------
// Скрытые методы управления компонентами
// --------------------------

/// Выполняет завершающие пользовательские действия
/// при добавлении дочернего компонента в этот объект
// Метод будет вызван только если comp был успешно добавлен в список компонент
bool NSeqComparison::AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer)
{
 return true;
}

/// Выполняет предварительные пользовательские действия
/// при удалении дочернего компонента из этого объекта
// Метод будет вызван только если comp существует в списке компонент
bool NSeqComparison::ADelComponent(UEPtr<UContainer> comp)
{
 return true;
}

// --------------------------


// --------------------------
// Методы управления общедоступными свойствами
// --------------------------

/// Установка имени класса нейрона
bool NSeqComparison::SetNeuronClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка имени класса генератора последовательности импульсов
bool NSeqComparison::SetMultiGeneratorClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка длительности импульсов
bool NSeqComparison::SetPulseLength(const double &value)
{
 if(value <= 0)
  return false;

 int temp_size = min(KFClsSpikes.size(), FrClsSpikes.size());

 // Настраиваем генераторы образов
 for (int i = 0; i < temp_size; i++)
 {
  KFClsSpikes[i]->PulseLength = value;
  FrClsSpikes[i]->PulseLength = value;
 }

 return true;
}

/// Установка амплитуды импульсов
bool NSeqComparison::SetAmplitude(const double &value)
{
 if(value <= 0)
  return false;

 int temp_size = min(KFClsSpikes.size(), FrClsSpikes.size());

 // Настраиваем генераторы образов
 for (int i = 0; i < temp_size; i++)
 {
  KFClsSpikes[i]->Amplitude = value;
  FrClsSpikes[i]->Amplitude = value;
 }

 return true;
}

/// Установка частоты генерации (Гц) всех компонентов
bool NSeqComparison::SetFrequency(const double &value)
{
 if(value < 0)
  return false;

 int temp_size = min(KFClsSpikes.size(), FrClsSpikes.size());

 // Настраиваем генераторы образов
 for (int i = 0; i < temp_size; i++)
 {
  KFClsSpikes[i]->Frequency = value;
  FrClsSpikes[i]->Frequency = value;
 }

 return true;
}

/// Установка используемого порога низкопороговой зоны нейрона
bool NSeqComparison::SetLTZThreshold(const double &value)
{
 if(value <= 0)
  return false;

 if (!CompNeuron)
  return true;

 UEPtr<NLTZone> ltzone = CompNeuron->GetComponentL<NLTZone>("LTZone");  // GetLTZone();
 if(!ltzone)
  return true;

 ltzone->Threshold = value;

 return true;
}

/// Установка размерности спайковых образов
bool NSeqComparison::SetPulseCount(const int &value)
{
 if(value < 1)
  return false;

 int temp_size = min(KFClsSpikes.size(), FrClsSpikes.size());

 for (int i = 0; i < temp_size; i++)
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

/// Установка количества объектов для сравнения
bool NSeqComparison::SetNumClasses(const int &value)
{
 if(value < 1)
  return false;

 OldNumClasses = NumClasses;

 Ready = false;
 return true;
}

/// Установка задержек, задающих проверенные спайковые образы классов
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

/// Установка задержек, задающих тестовые спайковые образы классов
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
// Восстановление настроек по умолчанию и сброс процесса счета
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

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NSeqComparison::ABuild(void)
{
 bool res = true;

 DelaysKFCls.Resize(NumClasses, PulseCount, 0.0);
 DelaysFrCls.Resize(NumClasses, PulseCount, 0.0);

 // Удаляем лишние генераторы спайковых образов классов
 for(int i = NumClasses; i < OldNumClasses; i++)
 {
  DelComponent(std::string("KF_Cls")+sntoa(i+1)+std::string("_Spikes"));
  DelComponent(std::string("Fr_Cls")+sntoa(i+1)+std::string("_Spikes"));
 }

 // Инициализируем генераторы спайковых образов классов
 KFClsSpikes.resize(NumClasses);
 FrClsSpikes.resize(NumClasses);
 for (int i = 0; i < NumClasses; i++)
 {
  KFClsSpikes[i] = AddMissingComponent<NPulseGeneratorMulti>(std::string("KF_Cls")+sntoa(i+1)+std::string("_Spikes"), MultiGeneratorClassName);
  KFClsSpikes[i]->SetCoord(MVector<double,3>(4, 2 + 5.33 * i, 0));
  KFClsSpikes[i]->DisconnectAll("Output");
  FrClsSpikes[i] = AddMissingComponent<NPulseGeneratorMulti>(std::string("Fr_Cls")+sntoa(i+1)+std::string("_Spikes"), MultiGeneratorClassName);
  FrClsSpikes[i]->SetCoord(MVector<double,3>(4, 4.33 + 5.33 * i, 0));
  FrClsSpikes[i]->DisconnectAll("Output");
 }


 // Инициализируем нейрон ИЛИ
 CompNeuron = AddMissingComponent<NPulseNeuron>(std::string("CompNeuron"), NeuronClassName);
 CompNeuron->SetCoord(MVector<double,3>(11, 3.17 + 2.67 * (NumClasses - 1), 0));
 CompNeuron->DisconnectAll("Output");
 CompNeuron->NumSomaMembraneParts = NumClasses;

 /* Вариант с замыканием проверенных последовательностей на дендриты
 // Удаляем лишние сегменты дендритов в нейроне ИЛИ
 for(int i = NumClasses; i < OldNumClasses; i++)
 {
  UEPtr<NPulseMembrane> dendrite = CompNeuron->GetComponentL<NPulseMembrane>("Dendrite" + sntoa(i+1) + "_1", true);
  DelComponent(dendrite->GetLongName(this));
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


 // Строим связи между генераторами и синапсами нейрона
 for (int i = 0; i < NumClasses; i++)
 {
  UEPtr<NPulseMembrane> dendrite = CompNeuron->GetComponentL<NPulseMembrane>("Dendrite" + sntoa(i+1) + "_1", true);
  UEPtr<NPulseMembrane> soma = CompNeuron->GetComponentL<NPulseMembrane>("Soma" + sntoa(i+1), true);
  // Заменить excsynapse, если нужно сделать связь на дендрит
//  NPulseSynapseCommon *excsynapse = dendrite->GetExcitatorySynapses(0);
  NPulseSynapseCommon *excsynapse = soma->GetExcitatorySynapses(0);
  NPulseSynapseCommon *inhsynapse = soma->GetInhibitorySynapses(0);

  if(!excsynapse || !inhsynapse)
  {
   LogMessageEx(RDK_EX_DEBUG, __FUNCTION__, std::string("Can't create link because Synapse in CompNeuron isn't exists: "));
   return true;
  }

  if (!CheckLink(KFClsSpikes[i]->GetLongName(this), "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink(KFClsSpikes[i]->GetLongName(this), "Output", excsynapse->GetLongName(this), "Input");

  if (!CheckLink(FrClsSpikes[i]->GetLongName(this), "Output", inhsynapse->GetLongName(this), "Input"))
   res &= CreateLink(FrClsSpikes[i]->GetLongName(this), "Output", inhsynapse->GetLongName(this), "Input");
 }

 return res;
}


// Reset computation
bool NSeqComparison::AReset(void)
{
 bool res = true;

 Output.ToZero();

 int temp_size = min(KFClsSpikes.size(), FrClsSpikes.size());

 // Настраиваем генераторы образов
 for (int i = 0; i < temp_size; i++)
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

 // Настраиваем порог генераторной зоны нейрона И
 UEPtr<NLTZone> ltzone = CompNeuron->GetComponentL<NLTZone>("LTZone", true);
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


