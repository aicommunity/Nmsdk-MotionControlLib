/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2012.
E-mail:        alexab@ailab.ru
url:        http://ailab.ru

This file - part of the project: RDK

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */


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
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NCounterNeuron* NCounterNeuron::New(void)
{
 return new NCounterNeuron;
}
// --------------------------


// --------------------------
// Скрытые методы управления компонентами
// --------------------------

/// Выполняет завершающие пользовательские действия
/// при добавлении дочернего компонента в этот объект
// Метод будет вызван только если comp был успешно добавлен в список компонент
bool NCounterNeuron::AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer)
{
 return true;
}

/// Выполняет предварительные пользовательские действия
/// при удалении дочернего компонента из этого объекта
// Метод будет вызван только если comp существует в списке компонент
bool NCounterNeuron::ADelComponent(UEPtr<UContainer> comp)
{
 return true;
}

// --------------------------


// --------------------------
// Методы управления общедоступными свойствами
// --------------------------

/// Установка имени класса участка мембраны
bool NCounterNeuron::SetMembraneClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка имени класса генераторной зоны
bool NCounterNeuron::SetLTZoneClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка имени класса источника, задающего сигнал для возбуждающего ионного механизма
bool NCounterNeuron::SetExcGeneratorClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка имени класса источника, задающего сигнал для тормозного ионного механизма
bool NCounterNeuron::SetInhGeneratorClassName(const std::string &value)
{
 Ready = false;
 return true;
}

/// Установка ограничения для счётчика
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

/// Установка начала отсчёта для счётчика
bool NCounterNeuron::SetCurCount(const int &value)
{
 if(value < 1 || value > MaxCount)
  return false;

 Ready = false;
 return true;
}

// --------------------------


// --------------------------
// Computation methods
// --------------------------

/// Создание необходимых связей в нейроне для переданной сомы
bool NCounterNeuron::CreateSomaLinks(UEPtr<NPulseMembrane> soma)
{
 bool res = true;
 UEPtr<NPulseChannelCommon> channel1, channel2, ltchannel1,ltchannel2;

 // Находим каналы мембраны
 channel1 = dynamic_pointer_cast<NPulseChannelCommon>(soma->GetComponent("ExcChannel",true));
 channel2 = dynamic_pointer_cast<NPulseChannelCommon>(soma->GetComponent("InhChannel",true));

 // Подключаем сому к низкопороговой зоне нейрона
 if(channel1)
  res&=CreateLink(channel1->GetLongName(this),"Output",LTZone->GetLongName(this),"Inputs");
 if(channel2)
  res&=CreateLink(channel2->GetLongName(this),"Output",LTZone->GetLongName(this),"Inputs");

 // Устанавливаем обратную связь
 res&=CreateLink(LTZone->GetLongName(this), "Output", soma->GetLongName(this), "InputFeedbackSignal");

 // Связываем начальные значения мощностей ионных каналов и каналы
 if(channel1)
  res&=CreateLink("NegGenerator", "Output",channel1->GetLongName(this),"ChannelInputs");
 if(channel2)
  res&=CreateLink("PosGenerator","Output",channel2->GetLongName(this),"ChannelInputs");

 return res;
}

// Восстановление настроек по умолчанию и сброс процесса счета
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

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NCounterNeuron::ABuild(void)
{
 bool res = true;

 UEPtr<NPulseMembrane> membr;
 UEPtr<UNet> gen_pos,gen_neg;

 // Добавляем компонент генераторной зоны нейрона
 LTZone = AddMissingComponent<NLTZone>("LTZone", LTZoneClassName);
 LTZone->SetCoord(MVector<double,3>(25.3, 3.67 + MaxCount, 0));
 LTZone->Threshold = 0.0029;  // Подобранное значение порога
 LTZone->DisconnectAll("Output");


 // Добавляем источник, задающий сигнал для возбуждающего ионного механизма
 if(!ExcGeneratorClassName->empty())
 {
  gen_pos = AddMissingComponent<UNet>("PosGenerator", ExcGeneratorClassName);
  gen_pos->SetCoord(MVector<double,3>(4, 2, 0));
  gen_pos->DisconnectAll("Output");
 }
 else
  DelComponent("PosGenerator");

 // Добавляем источник, задающий сигнал для тормозного ионного механизма
 if(!InhGeneratorClassName->empty())
 {
  gen_neg=AddMissingComponent<UNet>("NegGenerator", InhGeneratorClassName);
  gen_neg->SetCoord(MVector<double,3>(4, 7.3 + (MaxCount - 1) * 2, 0));
  gen_neg->DisconnectAll("Output");
 }
 else
  DelComponent("NegGenerator");


 // Удаляем лишние участки мембраны
 for(int i = MaxCount; i < OldNumSoma; i++)
  DelComponent(std::string("Soma")+sntoa(i+1));


 // Добавляем участки мембраны
 Soma.resize(MaxCount);
 for(int i = 0; i < MaxCount; i++)
 {
  // Добавляем компонент учатска мембраны
  membr = AddMissingComponent<NPulseMembrane>(std::string("Soma")+sntoa(i+1), MembraneClassName);
  membr->SetCoord(MVector<double,3>(15.7, 4.67+ i * 2, 0));
  Soma[i] = membr;

 // Устанавливаем связи только для первых CurCount сом
  if(i < CurCount)
   res &= CreateSomaLinks(membr);
  // Удаляем лишние связи участков мембраны
  else
  {
   UEPtr<NPulseChannelCommon> channel1 = dynamic_pointer_cast<NPulseChannelCommon>(membr->GetComponent("ExcChannel",true));
   UEPtr<NPulseChannelCommon> channel2 = dynamic_pointer_cast<NPulseChannelCommon>(membr->GetComponent("InhChannel",true));
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
 // Передача информации со входа компонента на синапсы
 // !!! Нельзя подключаться к синапсам напрямую, тк возникает сбой в генераторе
 for(int i = 0; i < CurCount; i++)
 {
  NPulseSynapseCommon *synapse = Soma[i]->GetExcitatorySynapses(0);
  *synapse->Input = *Input;
 }

 // Фиксируем прохождение импульса
 if((Input()(0,0) >= 0.01) && (!TheSamePulse)) // Пришёл (первый) запускающий импульс
 {
  TheSamePulse = true;  // Ставим метку: импульс начался
 }
 else if ((Input()(0,0) <= 0.01) && (TheSamePulse))  // Сигнал на входе упал
 {
  TheSamePulse = false;  // Ставим метку: импульс закончился
  if (CurCount < MaxCount)
  {
   // Добавляем новую связь, увеличиваем счёт
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


