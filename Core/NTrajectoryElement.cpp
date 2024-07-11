/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2010.
E-mail:			alexab@ailab.ru
url:			http://ailab.ru
Version:		1.0.0

This file - part of the project: NMSDK

File License:		BSD License
Project License:	GNU GPLv3 License
See file license.txt for more information
*********************************************************** */
#ifndef NMotionElementCPP
#define NMotionElementCPP

#include "NTrajectoryElement.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"
//---------------------------------------------------------------------------
namespace NMSDK {

// --------------------------
// Конструкторы и деструкторы
// --------------------------
NTrajectoryElement::NTrajectoryElement(void)
: NeuronClassName("NeuronClassName",this, &NTrajectoryElement::SetNeuronClassName),
  Output("Output",this)
  //Input_u_top("Input_u_top",this),
  //Input_u_tcn("Input_u_tcn",this),
  //Input_y_pcn2("Input_y_pcn2",this),
{
}

NTrajectoryElement::~NTrajectoryElement(void)
{

}
// --------------------------

// --------------------------
// Методы управления параметрами
// --------------------------
bool NTrajectoryElement::SetNeuronClassName(const std::string &value)
{
 Ready=false;
 return true;
}

// --------------------------



// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NTrajectoryElement* NTrajectoryElement::New(void)
{
 return new NTrajectoryElement;
}
// --------------------------

// --------------------------
// Proctected computation methods
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NTrajectoryElement::ADefault(void)
{
 NeuronClassName = "NSPNeuronGen"; //NNewSPNeuron
 SomaSize = 1;
 DendSizes1.resize(SomaSize);
 DendSizes1[0] = 5;
 DendSizes2.resize(SomaSize);
 DendSizes2[0] = 3;

 return true;
}


// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NTrajectoryElement::ABuild(void)
{
 //Координаты для размещения нейронов
 int y_base = 2;
 int y_shift = 2;

 //Создаем нейроны
 Neurons.resize(2);
 for (int i = 0; i<2; i++)
 {
  Neurons[i] = AddMissingComponent<NPulseNeuron>(std::string("Neuron")+sntoa(i+1), NeuronClassName);
  if(!Neurons[i])
   return true;
  Neurons[i]->SetCoord(MVector<double,3>(4.0, y_base+i*y_shift, 0));
  Neurons[i]->NumSomaMembraneParts = SomaSize;
  Neurons[i]->Reset();
 }

   Neurons[0]->NumDendriteMembranePartsVec = DendSizes1;
   Neurons[0]->Reset();
   Neurons[1]->NumDendriteMembranePartsVec = DendSizes2;
   Neurons[1]->Reset();


 //Строим связи
 bool res(true);

 //НОВАЯ КОНФИГУРАЦИЯ (подобрана для обновленной NNewSPNeuron)
 //От N1 к N2
 UEPtr<NLTZone> ltzone1 = Neurons[0]->GetComponentL<NLTZone>("LTZone", true);
 if(!ltzone1)
     return true;

 UEPtr<NPulseSynapse> n2_s1_d1 = Neurons[1]->GetComponentL<NPulseSynapse>("Dendrite1_1.ExcSynapse1",true);
 res&=CreateLink(ltzone1->GetLongName(this),"Output",n2_s1_d1->GetLongName(this),"Input");
 if(!res)
  return true;

 UEPtr<NPulseSynapse> n2_s1_d3 = Neurons[1]->GetComponentL<NPulseSynapse>("Dendrite1_3.ExcSynapse1",true);
 res&=CreateLink(ltzone1->GetLongName(this),"Output",n2_s1_d3->GetLongName(this),"Input");
 if(!res)
  return true;

 //От N2 к N1
 UEPtr<NLTZone> ltzone2 = Neurons[1]->GetComponentL<NLTZone>("LTZone", true);
 if(!ltzone2)
     return true;

 UEPtr<NPulseSynapse> n1_s1_d1 = Neurons[0]->GetComponentL<NPulseSynapse>("Dendrite1_1.ExcSynapse1",true);
 res&=CreateLink(ltzone2->GetLongName(this),"Output",n1_s1_d1->GetLongName(this),"Input");
 if(!res)
  return true;

 UEPtr<NPulseSynapse> n1_s1_d3 = Neurons[0]->GetComponentL<NPulseSynapse>("Dendrite1_3.ExcSynapse1",true);
 res&=CreateLink(ltzone2->GetLongName(this),"Output",n1_s1_d3->GetLongName(this),"Input");
 if(!res)
  return true;

 return true;
}


// Reset computation
bool NTrajectoryElement::AReset(void)
{
 return true;
}


// Execute math. computations of current object on current step
bool NTrajectoryElement::ACalculate(void)
{
 //Передача сигнала к выходу блока
  Output = Neurons[0]->Output;

 return true;
}




// --------------------------


}
//---------------------------------------------------------------------------
#endif
