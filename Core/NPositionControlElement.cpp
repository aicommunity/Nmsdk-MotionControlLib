/* ***********************************************************

E-mail:        
Url:           

This file is part of the project: 

File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NPositionControlElement_CPP
#define NPositionControlElement_CPP

#include "NPositionControlElement.h"

namespace NMSDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NPositionControlElement::NPositionControlElement(void)
:
	CurrentPosition("CurrentPosition",this),
	TargetPosition("TargetPosition",this),
	InputNeuronType("InputNeuronType",this, &NPositionControlElement::SetInputNeuronType),
	ControlNeuronType("ControlNeuronType",this, &NPositionControlElement::SetControlNeuronType),
	ExternalControl("ExternalControl", this, &NPositionControlElement::SetExternalControl),
	RememberState("RememberState", this),
	Delta("Delta",this)
{
}

NPositionControlElement::~NPositionControlElement(void)
{
}
// --------------------------


// ---------------------
// Методы управления параметрами
// ---------------------
bool NPositionControlElement::SetInputNeuronType(const string &value)
{
 Ready=false;
 return true;
}
bool NPositionControlElement::SetControlNeuronType(const string &value)
{
 Ready=false;
 return true;
}
bool NPositionControlElement::SetExternalControl(const bool &value)
{
 LinkGenerators(value);
 Ready=false;
 return true;
}
// ---------------------

// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NPositionControlElement* NPositionControlElement::New(void)
{
 return new NPositionControlElement;
}
// --------------------------


// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NPositionControlElement::ADefault(void)
{
 InputNeuronType = "NNewSPNeuron";
 ControlNeuronType = "NNewSPNeuron";
 ExternalControl = false;
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NPositionControlElement::ABuild(void)
{
 CurrentPosition->Assign(2,1,0.0);
 TargetPosition->Assign(2,1,0.0);
 InputNeurons.clear();
 ControlNeurons.clear();
 Generators.clear();
 Delta->Assign(2,1,0.0);

 return true;
}

// Сброс процесса счета без потери настроек
bool NPositionControlElement::AReset(void)
{
 RememberState = false;
 return true;
}

// Выполняет расчет этого объекта
bool NPositionControlElement::ACalculate(void)
{
 return true;
}
template <typename T>
void NPositionControlElement::SetPositions(vector<T*>& values, int x_base, int x_shift, int y_base, int y_shift)
{
 for(int i=0;i<values.size();++i)
 {
  values[i]->SetCoord(MVector<double,3>(x_base + i*x_shift,y_base + i*y_shift,0));
 }
}
// --------------------------
bool NPositionControlElement::CreateNeurons()
{
   return true;
}

bool NPositionControlElement::CreateExternalControlElements(void)
{
 return true;
}

bool NPositionControlElement::LinkNeurons(vector <NNet*> start, vector <NNet*> finish)
{
	//vector <NNet*>::iterator I,J;
	for(size_t j=0;j<finish.size();j++)
	{
	  NPulseNeuron* neuron=dynamic_cast<NPulseNeuron*>(finish[j]);
	  UEPtr<NPulseMembraneCommon> branch;
	  bool hasEmptyMembrane=false;
	  for(size_t k=0;k<neuron->GetNumMembranes();k++)
	  {
	   NPulseMembrane* membr = neuron->GetMembrane(k);
	   if((neuron->GetNumOfConnectedSynToPosCh(membr)==0)&&(membr->GetName()!="LTMembrane"))
	   {
		 branch=neuron->GetMembrane(k);
		 hasEmptyMembrane=true;
		 break;
	   }
	  }
	  if(!hasEmptyMembrane)
	   branch=neuron->BranchDendrite("Soma1",false);
	  NameT finishName = finish[j]->GetName()+"."+branch->GetName()+".ExcChannel";
	  for(size_t i=0;i<start.size();i++)
	  {
	   NameT startName = start[i]->GetName()+".LTZone";
	   if(!CheckLink(startName,finishName))
		CreateLink(startName, 0, finishName);
//	   ExternalControl=false;
	  }
	}

	return true;
}

bool NPositionControlElement::UnlinkNeurons(vector <NNet*> start, vector <NNet*> finish)
{
 for(size_t i=0;i<start.size();i++)
 {
  NameT startName = start[i]->GetName()+".LTZone";
  NPulseNeuron* neuron=dynamic_cast<NPulseNeuron*>(finish[i]);
  for(size_t j=0;j<finish.size();j++)
  {
   for(size_t k=0;k<neuron->Membranes.size();k++)
   {
	NameT finishName = finish[j]->GetName()+"."+neuron->Membranes[k]->GetName()+".ExcChannel";
	if(CheckLink(startName,finishName))
	   BreakLink(startName,finishName);
   }
  }
 }
 return true;
}
bool NPositionControlElement::LinkGenerators(const bool &value)
{
 return true;
}
bool NPositionControlElement::LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link, bool is_sim)
{
 return true;
}
bool NPositionControlElement::LinkNegative(vector <NNet*> start, vector <NNet*> finish)
{
 return true;
}

bool NPositionControlElement::PositionNeurons()
{
   int x_base=5;
   int x_shift=0;
   int y_base=2;
   int y_shift = 2;
   SetPositions<NNet>(InputNeurons, x_base, x_shift, y_base, y_shift);
   x_base+=10;
   SetPositions<NNet>(PostInputNeurons, x_base, x_shift, y_base, y_shift);
   x_base+=10;
   SetPositions<NNet>(PreControlNeurons, x_base, x_shift, y_base, y_shift);
   x_base+=10;
   SetPositions<NNet>(ControlNeurons, x_base, x_shift, y_base, y_shift);
   x_base+=10;
   SetPositions<UNet>(Generators, x_base, x_shift, y_base, y_shift);
 return true;
}

vector<NNet*> NPositionControlElement::GetInputNeurons(void)
{
 return InputNeurons;
}

vector<NNet*> NPositionControlElement::GetControlNeurons(void)
{
 return ControlNeurons;
}

}
#endif



