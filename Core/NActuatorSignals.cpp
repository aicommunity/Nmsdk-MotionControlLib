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
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NActuatorSignals* NActuatorSignals::New(void)
{
 return new NActuatorSignals;
}
// --------------------------


// --------------------------
// Скрытые методы управления компонентами
// --------------------------

/// Выполняет завершающие пользовательские действия
/// при добавлении дочернего компонента в этот объект
// Метод будет вызван только если comp был успешно добавлен в список компонент
bool NActuatorSignals::AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer)
{
 return true;
}

/// Выполняет предварительные пользовательские действия
/// при удалении дочернего компонента из этого объекта
// Метод будет вызван только если comp существует в списке компонент
bool NActuatorSignals::ADelComponent(UEPtr<UContainer> comp)
{
 return true;
}

// --------------------------


// --------------------------
// Методы управления общедоступными свойствами
// --------------------------

/// Установка имени класса нейрона
bool NActuatorSignals::SetNeuronClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка имени класса генераторов импульсов
bool NActuatorSignals::SetGeneratorClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка имени класса блока задержки сигнала
bool NActuatorSignals::SetDelayClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка длительности импульсов
bool NActuatorSignals::SetPulseLength(const double &value)
{
 if(value <= 0)
  return false;

 // Настраиваем генераторы
 UEPtr <NPulseGeneratorTransit> generators[5]{LeftEngine, RightEngine, Sinchro1, Sinchro2, NOTGenerator};

 for (int i = 0; i < 5; i++)
 {
  if(generators[i])
   generators[i]->PulseLength = value;
 }

 return true;
}

/// Установка амплитуды импульсов
bool NActuatorSignals::SetAmplitude(const double &value)
{
 if(value <= 0)
  return false;

 // Настраиваем генераторы
 UEPtr <NPulseGeneratorTransit> generators[5]{LeftEngine, RightEngine, Sinchro1, Sinchro2, NOTGenerator};

 for (int i = 0; i < 5; i++)
 {
  if(generators[i])
   generators[i]->Amplitude = value;
 }

 return true;
}

/// Установка частоты генерации (Гц) всех компонентов
bool NActuatorSignals::SetFrequency(const double &value)
{
 if(value < 0)
  return false;

 // Настраиваем генераторы
 UEPtr <NPulseGeneratorTransit> generators[5]{LeftEngine, RightEngine, Sinchro1, Sinchro2, NOTGenerator};

 for (int i = 0; i < 5; i++)
 {
  if(generators[i] && !generators[i]->UseTransitSignal)
   generators[i]->Frequency = value;
 }

 return true;
}

/// Установка задержки для левого двигателя
bool NActuatorSignals::SetLeftDelay(const double &value)
{
 if(value < 0)
  return false;

 if (LeftEngine)
  LeftEngine->Delay = value;

 return true;
}

/// Установка задержки для правого двигателя
bool NActuatorSignals::SetRightDelay(const double &value)
{
 if(value < 0)
  return false;

 if (RightEngine)
  RightEngine->Delay = value;

 return true;
}

/// Установка задержки синхронизирующего импульса, должен быть аналогом вращения назад
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

/// Установка задержки синхронизирующего импульса, должен быть аналогом вращения вперед
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


/// Установка необходимости транзита сигнала на двигатели от внешнего источника
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

/// Установка необходимости транзита сигнала на синхронизирующие генераторы от внешнего источника
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
// Восстановление настроек по умолчанию и сброс процесса счета
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

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NActuatorSignals::ABuild(void)
{
 bool res = true;

 UEPtr<NPulseMembrane> soma, dendrite;
 NPulseSynapseCommon *excsynapse, *inhsynapse;

 // Инициализируем компоненты
 {
 // Инициализируем источник сигнала левого двигателя
 LeftEngine = AddMissingComponent<NPulseGeneratorTransit>(std::string("LeftEngine"), GeneratorClassName);
 LeftEngine->SetCoord(MVector<double,3>(4, 2.33, 0));
 LeftEngine->DisconnectAll("Output");

 // Инициализируем источник сигнала правого двигателя
 RightEngine = AddMissingComponent<NPulseGeneratorTransit>(std::string("RightEngine"), GeneratorClassName);
 RightEngine->SetCoord(MVector<double,3>(4, 5, 0));
 RightEngine->DisconnectAll("Output");

 // Инициализируем источник синхронизирующего сигнала 1 - аналога вращения назад
 Sinchro1 = AddMissingComponent<NPulseGeneratorTransit>(std::string("Sinchro1"), GeneratorClassName);
 Sinchro1->SetCoord(MVector<double,3>(4, 10.33, 0));
 Sinchro1->DisconnectAll("Output");

 // Инициализируем источник синхронизирующего сигнала 2 - аналога вращения вперед
 Sinchro2 = AddMissingComponent<NPulseGeneratorTransit>(std::string("Sinchro2"), GeneratorClassName);
 Sinchro2->SetCoord(MVector<double,3>(4, 13, 0));
 Sinchro2->DisconnectAll("Output");

 // Инициализируем генератор для нейрона NOTNeuron
 NOTGenerator = AddMissingComponent<NPulseGeneratorTransit>(std::string("NOTGenerator"), GeneratorClassName);
 NOTGenerator->SetCoord(MVector<double,3>(21.33, 2.33, 0));
 NOTGenerator->DisconnectAll("Output");


 // Инициализируем блок задержки
 Delay1To2 = AddMissingComponent<NPulseDelay>(std::string("Delay1To2"), DelayClassName);
 Delay1To2->SetCoord(MVector<double,3>(21.33, 11.66, 0));
 Delay1To2->DisconnectAll("Output");


 // Инициализируем нейрон ИЛИ
 ORNeuron = AddMissingComponent<NPulseNeuron>(std::string("ORNeuron"), NeuronClassName);
 ORNeuron->SetCoord(MVector<double,3>(21.33, 7.66, 0));
 ORNeuron->DisconnectAll("Output");
 soma = ORNeuron->GetComponentL<NPulseMembrane>("Soma1", true);
 if (soma)
 {
  soma->NumExcitatorySynapses = 4;
  soma->Build();
 }

 // Инициализируем нейрон НЕ
 NOTNeuron = AddMissingComponent<NPulseNeuron>(std::string("NOTNeuron"), NeuronClassName);
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


 // Инициализируем нейрон, определяющий движение вперёд
 IsForwardNeuron = AddMissingComponent<NPulseNeuron>(std::string("IsForward"), NeuronClassName);
 IsForwardNeuron->SetCoord(MVector<double,3>(12.66, 3.66, 0));
 IsForwardNeuron->DisconnectAll("Output");
 IsForwardNeuron->NumSomaMembraneParts = 2;
 IsForwardNeuron->Build();

 // Инициализируем нейрон, определяющий движение назад
 IsBackNeuron = AddMissingComponent<NPulseNeuron>(std::string("IsBack"), NeuronClassName);
 IsBackNeuron->SetCoord(MVector<double,3>(12.66, 11.66, 0));
 IsBackNeuron->DisconnectAll("Output");
 IsBackNeuron->NumSomaMembraneParts = 2;
 IsBackNeuron->Build();

 // Инициализируем нейрон, определяющий поворот налево
 IsLeftNeuron = AddMissingComponent<NPulseNeuron>(std::string("IsLeft"), NeuronClassName);
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

 // Инициализируем нейрон, определяющий поворот направо
 IsRightNeuron = AddMissingComponent<NPulseNeuron>(std::string("IsRight"), NeuronClassName);
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


 // Инициализируем выходной нейрон, сигнализирующий об остановке
 StayNeuron = AddMissingComponent<NPulseNeuron>(std::string("Stay"), NeuronClassName);
 StayNeuron->SetCoord(MVector<double,3>(30, 2.33, 0));
 StayNeuron->DisconnectAll("Output");

 // Инициализируем выходной нейрон, сигнализирующий о движении вперёд
 ForwardNeuron = AddMissingComponent<NPulseNeuron>(std::string("Forward"), NeuronClassName);
 ForwardNeuron->SetCoord(MVector<double,3>(30, 5, 0));
 ForwardNeuron->DisconnectAll("Output");

 // Инициализируем выходной нейрон, сигнализирующий о повороте налево
 LeftNeuron = AddMissingComponent<NPulseNeuron>(std::string("Left"), NeuronClassName);
 LeftNeuron->SetCoord(MVector<double,3>(30, 7.66, 0));
 LeftNeuron->DisconnectAll("Output");

 // Инициализируем выходной нейрон, сигнализирующий о повороте направо
 RightNeuron = AddMissingComponent<NPulseNeuron>(std::string("Right"), NeuronClassName);
 RightNeuron->SetCoord(MVector<double,3>(30, 10.33, 0));
 RightNeuron->DisconnectAll("Output");

 // Инициализируем выходной нейрон, сигнализирующий о движении назад
 BackNeuron = AddMissingComponent<NPulseNeuron>(std::string("Back"), NeuronClassName);
 BackNeuron->SetCoord(MVector<double,3>(30, 13, 0));
 BackNeuron->DisconnectAll("Output");
}


 // Строим связи между компонентами
 {
 // Связи на IsForward
 {
  soma = IsForwardNeuron->GetComponent<NPulseMembrane>("Soma1", true);
  excsynapse = soma->GetExcitatorySynapses(0);
  inhsynapse = soma->GetInhibitorySynapses(0);

  // Создаём связь c LeftEngine
  if (!CheckLink("LeftEngine", "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink("LeftEngine", "Output", excsynapse->GetLongName(this), "Input");

  // Создаём связь c Sinchro1
  if (!CheckLink("Sinchro1", "Output", inhsynapse->GetLongName(this), "Input"))
   res &= CreateLink("Sinchro1", "Output", inhsynapse->GetLongName(this), "Input");


  soma = IsForwardNeuron->GetComponent<NPulseMembrane>("Soma2", true);
  excsynapse = soma->GetExcitatorySynapses(0);
  inhsynapse = soma->GetInhibitorySynapses(0);

  // Создаём связь c RightEngine
  if (!CheckLink("RightEngine", "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink("RightEngine", "Output", excsynapse->GetLongName(this), "Input");

  // Создаём связь c Sinchro1
  if (!CheckLink("Sinchro1", "Output", inhsynapse->GetLongName(this), "Input"))
   res &= CreateLink("Sinchro1", "Output", inhsynapse->GetLongName(this), "Input");
 }

 // Связи на IsLeft
 {
  dendrite = IsLeftNeuron->GetComponent<NPulseMembrane>("Dendrite1_10", true);
  excsynapse = dendrite->GetExcitatorySynapses(0);
  inhsynapse = dendrite ->GetInhibitorySynapses(0);

  // Создаём связь c LeftEngine
  if (!CheckLink("LeftEngine", "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink("LeftEngine", "Output", excsynapse->GetLongName(this), "Input");

  // Создаём связь c Sinchro2
  if (!CheckLink("Sinchro2", "Output", inhsynapse->GetLongName(this), "Input"))
   res &= CreateLink("Sinchro2", "Output", inhsynapse->GetLongName(this), "Input");


  soma = IsLeftNeuron->GetComponent<NPulseMembrane>("Soma2", true);
  excsynapse = soma->GetExcitatorySynapses(0);
  inhsynapse = soma->GetInhibitorySynapses(0);

  // Создаём связь c RightEngine
  if (!CheckLink("RightEngine", "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink("RightEngine", "Output", excsynapse->GetLongName(this), "Input");

  // Создаём связь c Sinchro1
  if (!CheckLink("Sinchro1", "Output", inhsynapse->GetLongName(this), "Input"))
   res &= CreateLink("Sinchro1", "Output", inhsynapse->GetLongName(this), "Input");
 }

 // Связи на IsRight
 {
  dendrite = IsRightNeuron->GetComponent<NPulseMembrane>("Dendrite2_10", true);
  excsynapse = dendrite->GetExcitatorySynapses(0);
  inhsynapse = dendrite ->GetInhibitorySynapses(0);

  // Создаём связь c RightEngine
  if (!CheckLink("RightEngine", "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink("RightEngine", "Output", excsynapse->GetLongName(this), "Input");

  // Создаём связь c Sinchro2
  if (!CheckLink("Sinchro2", "Output", inhsynapse->GetLongName(this), "Input"))
   res &= CreateLink("Sinchro2", "Output", inhsynapse->GetLongName(this), "Input");


  soma = IsRightNeuron->GetComponent<NPulseMembrane>("Soma1", true);
  excsynapse = soma->GetExcitatorySynapses(0);
  inhsynapse = soma->GetInhibitorySynapses(0);

  // Создаём связь c LeftEngine
  if (!CheckLink("LeftEngine", "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink("LeftEngine", "Output", excsynapse->GetLongName(this), "Input");

  // Создаём связь c Sinchro1
  if (!CheckLink("Sinchro1", "Output", inhsynapse->GetLongName(this), "Input"))
   res &= CreateLink("Sinchro1", "Output", inhsynapse->GetLongName(this), "Input");
 }

 // Связи на IsBack
 {
  soma = IsBackNeuron->GetComponent<NPulseMembrane>("Soma1", true);
  excsynapse = soma->GetExcitatorySynapses(0);
  inhsynapse = soma->GetInhibitorySynapses(0);

  // Создаём связь c LeftEngine
  if (!CheckLink("LeftEngine", "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink("LeftEngine", "Output", excsynapse->GetLongName(this), "Input");

  // Создаём связь c Sinchro2
  if (!CheckLink("Sinchro2", "Output", inhsynapse->GetLongName(this), "Input"))
   res &= CreateLink("Sinchro2", "Output", inhsynapse->GetLongName(this), "Input");


  soma = IsBackNeuron->GetComponent<NPulseMembrane>("Soma2", true);
  excsynapse = soma->GetExcitatorySynapses(0);
  inhsynapse = soma->GetInhibitorySynapses(0);

  // Создаём связь c RightEngine
  if (!CheckLink("RightEngine", "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink("RightEngine", "Output", excsynapse->GetLongName(this), "Input");

  // Создаём связь c Sinchro2
  if (!CheckLink("Sinchro2", "Output", inhsynapse->GetLongName(this), "Input"))
   res &= CreateLink("Sinchro2", "Output", inhsynapse->GetLongName(this), "Input");
 }

 // Связи на ORNeuron
 {
  soma = ORNeuron->GetComponent<NPulseMembrane>("Soma1", true);

  // Создаём связь c IsForward
  excsynapse = soma->GetExcitatorySynapses(0);
  if (!CheckLink("IsForward", "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink("IsForward", "Output", excsynapse->GetLongName(this), "Input");

  // Создаём связь c IsLeft
  excsynapse = soma->GetExcitatorySynapses(1);
  if (!CheckLink("IsLeft", "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink("IsLeft", "Output", excsynapse->GetLongName(this), "Input");

  // Создаём связь c IsRight
  excsynapse = soma->GetExcitatorySynapses(2);
  if (!CheckLink("IsRight", "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink("IsRight", "Output", excsynapse->GetLongName(this), "Input");

  // Создаём связь c Delay1To2
  excsynapse = soma->GetExcitatorySynapses(3);
  if (!CheckLink("Delay1To2", "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink("Delay1To2", "Output", excsynapse->GetLongName(this), "Input");
 }

 // Связи на NOTNeuron
 {
  // Создаём связь c ORNeuron
  soma = NOTNeuron->GetComponent<NPulseMembrane>("Soma1", true);
  inhsynapse = soma->GetInhibitorySynapses(0);
  if (!CheckLink("ORNeuron", "Output", inhsynapse->GetLongName(this), "Input"))
   res &= CreateLink("ORNeuron", "Output", inhsynapse->GetLongName(this), "Input");

  // Создаём связь c NOTGenerator
  dendrite = NOTNeuron->GetComponent<NPulseMembrane>("Dendrite1_2", true);
  excsynapse = dendrite->GetExcitatorySynapses(0);
  if (!CheckLink("NOTGenerator", "Output", excsynapse->GetLongName(this), "Input"))
   res &= CreateLink("NOTGenerator", "Output", excsynapse->GetLongName(this), "Input");
 }

 // Связь c IsBack на Delay1To2
 if (!CheckLink("IsBack", "Output", "Delay1To2", "Input"))
  res &= CreateLink("IsBack", "Output", "Delay1To2", "Input");

 // Связь с NOTNeuron на Stay
 soma = StayNeuron->GetComponent<NPulseMembrane>("Soma1", true);
 excsynapse = soma->GetExcitatorySynapses(0);
 if (!CheckLink("NOTNeuron", "Output", excsynapse->GetLongName(this), "Input"))
  res &= CreateLink("NOTNeuron", "Output", excsynapse->GetLongName(this), "Input");

 // Связь с IsForward на Forward
 soma = ForwardNeuron->GetComponent<NPulseMembrane>("Soma1", true);
 excsynapse = soma->GetExcitatorySynapses(0);
 if (!CheckLink("IsForward", "Output", excsynapse->GetLongName(this), "Input"))
  res &= CreateLink("IsForward", "Output", excsynapse->GetLongName(this), "Input");

 // Связь с IsLeft на Left
 soma = LeftNeuron->GetComponent<NPulseMembrane>("Soma1", true);
 excsynapse = soma->GetExcitatorySynapses(0);
 if (!CheckLink("IsLeft", "Output", excsynapse->GetLongName(this), "Input"))
  res &= CreateLink("IsLeft", "Output", excsynapse->GetLongName(this), "Input");

 // Связь с IsRight на Right
 soma = RightNeuron->GetComponent<NPulseMembrane>("Soma1", true);
 excsynapse = soma->GetExcitatorySynapses(0);
 if (!CheckLink("IsRight", "Output", excsynapse->GetLongName(this), "Input"))
  res &= CreateLink("IsRight", "Output", excsynapse->GetLongName(this), "Input");

 // Связь с Delay1To2 на Back
 soma = BackNeuron->GetComponent<NPulseMembrane>("Soma1", true);
 excsynapse = soma->GetExcitatorySynapses(0);
 if (!CheckLink("Delay1To2", "Output", excsynapse->GetLongName(this), "Input"))
  res &= CreateLink("Delay1To2", "Output", excsynapse->GetLongName(this), "Input");
}

 return res;
}


// Reset computation
bool NActuatorSignals::AReset(void)
{
 bool res = true;

 // Сбрасываем выходные сигналы
 Stay.ToZero();
 Forward.ToZero();
 Back.ToZero();
 Left.ToZero();
 Right.ToZero();

 // Настраиваем генераторы
 UEPtr <NPulseGeneratorTransit> generators[5]{LeftEngine, RightEngine, Sinchro1, Sinchro2, NOTGenerator};
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

 // Настраиваем блок задержки
 Delay1To2->DelayTime = Sinchro2Delay - Sinchro1Delay;

 // Настраиваем пороги нейронов
 UEPtr<NLTZone> ltzone = NOTNeuron->GetComponentL<NLTZone>("LTZone", true);
 ltzone->Threshold = 0.01;

 UEPtr <NPulseNeuron> is_neurons[4]{IsForwardNeuron, IsLeftNeuron, IsRightNeuron, IsBackNeuron};
 for (int i = 0; i < 4; i++)
 {
  ltzone = is_neurons[i]->GetComponentL<NLTZone>("LTZone", true);
  ltzone->Threshold = 0.013;
 }

 return res;
}

// Execute math. computations of current object on current step
bool NActuatorSignals::ACalculate(void)
{
 bool res = true;

 // Передаём информацию на выходы
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


