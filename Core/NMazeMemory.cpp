// ===========================================================
// Version:        3.0.2
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2002.
E-mail:		alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NMAZEMEMORY_CPP
#define NMAZEMEMORY_CPP

#include "NMazeMemory.h"
#include <QString>

namespace NMSDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NMazeMemory::NMazeMemory(void)
: OptionsNum("OptionsNum",this, &NMazeMemory::SetOptionsNum),
  NewSituation("NewSituation",this, &NMazeMemory::SetNewSituation),
  RememberSituation("RememberSituation",this, &NMazeMemory::SetRememberSituation),
  FeaturesNum("FeaturesNum",this, &NMazeMemory::SetFeaturesNum),
  IsDeadlock("IsDeadlock",this, &NMazeMemory::SetIsDeadlock)

{

}

NMazeMemory::~NMazeMemory(void)
{
}
// --------------------------

// --------------------------
// Методы доступа к временным переменным
// --------------------------
// --------------------------

// --------------------------
// Методы упраления параметрами
// --------------------------
bool NMazeMemory::SetOptionsNum(const int &value)
{
 Ready=false;
 return true;
}

bool NMazeMemory::SetNewSituation(const bool &value)
{
 return true;
}

bool NMazeMemory::SetRememberSituation(const bool &value)
{
 Ready=false;
 return true;
}

bool NMazeMemory::SetFeaturesNum(const int &value)
{
 Ready=false;
 return true;
}

bool NMazeMemory::SetIsDeadlock(const bool &value)
{
 Ready=false;
 return true;
}


// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NMazeMemory* NMazeMemory::New(void)
{
 return new NMazeMemory;
}

UComponent* NMazeMemory::NewStatic(void)
{
 return new NMazeMemory;
}
// --------------------------

// --------------------------
// Скрытые методы управления компонентами
// --------------------------
// Выполняет завершающие пользовательские действия
// при добавлении дочернего компонента в этот объект
// Метод будет вызван только если comp был
// успешно добавлен в список компонент
bool NMazeMemory::AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer)
{

 return true;
}

// Выполняет предварительные пользовательские действия
// при удалении дочернего компонента из этого объекта
// Метод будет вызван только если comp
// существует в списке компонент
bool NMazeMemory::ADelComponent(UEPtr<UContainer> comp)
{

 return true;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------


// Сброс процесса счета.
bool NMazeMemory::AReset(void)
{


 return true;
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool NMazeMemory::ADefault(void)
{
 NewSituation = false;
 RememberSituation = false;
 FeaturesNum = 1;

 NewSituation = false;
 OptionsNum = 2;
 IsDeadlock = false;

 CurrentTE = 0;
 CurrentLayer = 0;
 LayerShift = 7.0;
 yShift = 5.0;
 SideWeight = 0.75;

 return true;
}


// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NMazeMemory::ABuild(void)
{
   if (TrajectoryElements.size()<1)
   {
       MVector<double,3> root_coords = {5.0, 4.0, 0};
       UEPtr<NTrajectoryElement> root = CreatePoint(root_coords);
   }

 return true;
}

// Выполняет расчет этого объекта
bool NMazeMemory::ACalculate(void)
{
//    if (IsDeadlock)
//    {
//        w обратной связи на ЭТ(i-1) = 1,
//        w текущего ЭТ(i) = 0,2
//        +убрать "тупиковый" ЭТ из стека пройденных точек
//    }
//    else
//    {
//     //Проверяем, есть ли другие активные ЭТ в сети -
//     //т.е.совпадает ли текущая ситуация с какой-либо из уже изученных
//     for(int i = 0; i++; i<CurrentTE)
//     {
//      UEPtr<NPulseNeuron> neuron = TrajectoryElements[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
//      UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
//      if(!ltzone)
//          return true;

//      if(ltzone->OutputFrequency->As<double>(0) >0)
//      {
//       //string check_actInp = InputNeurons[i]->GetName();
//       ActiveTE.push_back(TrajectoryElements[i]);
//      }

//      if(ActiveTE.size()>0)
//      {
//          //сливаем активные ЭТ в один
//      }

//     }
//    }

    if (!NewSituation)
        return true;

    //Обрабатываем новую ситуацию
    NewSituation = false;
    UEPtr<NTrajectoryElement> base_TE = TrajectoryElements[CurrentTE];
    //string check_basename = string(base_TE->GetName());
    PassedTEs.push_back(base_TE);
    UEPtr<NMultiPositionControl> base_MPC = MultiPCs[CurrentTE];
    //string check_baseMPCname = string(base_MPC->GetName());

    if (OptionsNum <1) // если нет доступных направлений движения
    {
      //w обратной связи на ЭТ(i-1) = 1,
      //w текущего ЭТ(i) = 0,2
    }
    else // если есть доступные направления
    {
      UEPtr<NTrajectoryElement> traj_el;
      //UEPtr<NTrajectoryElement> base_TE = TrajectoryElements[CurrentTE];
      MVector<double,3> base_coords = base_TE->GetCoord();

      CurrentLayer++;

      UEPtr<NPulseNeuron> base_neuron = TrajectoryElements[CurrentTE]->GetComponentL<NPulseNeuron>("Neuron1", true);
      //string check_baseneuron = string(base_neuron->GetName());
      UEPtr<NLTZone> ltzone_te = base_neuron->GetComponentL<NLTZone>("LTZone", true);
      if(!ltzone_te)
       return true;

      //добавляем синапсы на текущий ЭТ для обратных связей на сому
      UEPtr<NPulseMembrane> base_soma = base_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
      if (CurrentTE==0)
          base_soma->NumExcitatorySynapses = OptionsNum+1; //дополнительный синапс на первый ЭТ - для запускающего сигнала с высшего уровня СУ
      else
          base_soma->NumExcitatorySynapses = OptionsNum;
      base_soma->Reset();
      //на N1_D1_2
      UEPtr<NPulseMembrane> base_dend = base_neuron->GetComponentL<NPulseMembrane>("Dendrite1_2", true);
      base_dend->NumExcitatorySynapses = OptionsNum;
      base_dend->Reset();


      for(int k = 0; k<OptionsNum; k++)
      {
       //Рассчитываем координаты и добавляем блоки
       double x = (double)CurrentLayer*LayerShift + base_coords[0];
       double y = (double)k*yShift + base_coords[1];
       MVector<double,3> coords = {x, y, 0.0};
       traj_el = CreatePoint(coords);//ДОБАВЛЕНИЕ БЛОКОВ
       traj_el->Reset();
       base_TE->Forwards.push_back(traj_el);
       traj_el->Layer = CurrentLayer;

       //Строим связи от текущего элемента траектории на только что созданные
       bool res(true);
       //возбуждающее воздействие на N1_D5_syn1
       UEPtr<NPulseNeuron> input_neuron = traj_el->GetComponentL<NPulseNeuron>("Neuron1", true);
       UEPtr<NPulseSynapse> synapse = input_neuron->GetComponentL<NPulseSynapse>("Dendrite1_5.ExcSynapse1",true);
       if (k>0)
       {
         synapse->Weight = SideWeight;
       }
       else
       {
         traj_el->CurrentForward = k;
       }
       res&=CreateLink(base_TE->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
       if(!res)
        return true;
       //тормозное воздействие на N1_S1_syn1
       synapse = input_neuron->GetComponentL<NPulseSynapse>("Soma1.InhSynapse1",true);
       res&=CreateLink(base_TE->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
       if(!res)
        return true;
       //тормозное воздействие на N2_S1_syn1
       input_neuron = traj_el->GetComponentL<NPulseNeuron>("Neuron2", true);
       synapse = input_neuron->GetComponentL<NPulseSynapse>("Soma1.InhSynapse1",true);
       res&=CreateLink(base_TE->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
       if(!res)
        return true;

       //обратные связи
       traj_el->Backwards.push_back(base_TE);
       traj_el->CurrentBackward = 0;
       //на сому
       int syn_num;
       if (CurrentTE==0)
           syn_num = k+2;
       else
           syn_num = k+1;
       synapse = base_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(syn_num),true);
       synapse->Weight = 0.2;
       res&=CreateLink(traj_el->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
       if(!res)
        return true;
       //на N1_D1_2
       synapse = base_neuron->GetComponentL<NPulseSynapse>("Dendrite1_2.ExcSynapse"+sntoa(k+1),true);
       synapse->Weight = 0.2;
       res&=CreateLink(traj_el->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
       if(!res)
        return true;

       //связи между PreControl нейронами для "клича"
       int passed_max = PassedTEs.size();
       for (int j=0; j<passed_max; j++)
       {
        UEPtr<NMultiPositionControl> output_mpc = MultiPCs[j];
        UEPtr<NPulseNeuron> output_postinputn = output_mpc->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
        UEPtr<NLTZone> output_ltzone = output_postinputn->GetComponentL<NLTZone>("LTZone", true);

        UEPtr<NMultiPositionControl> input_mpc = MultiPCs[CurrentTE+k+1];
        UEPtr<NPulseNeuron> input_precontroln = input_mpc->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
        UEPtr<NPulseMembrane> input_soma = input_precontroln->GetComponentL<NPulseMembrane>("Soma1", true);

        int syn_num = input_soma->NumExcitatorySynapses;
        for (int i = 1; i<=syn_num; i++)
        {
         UEPtr<NPulseSynapse> syn = input_precontroln->GetComponentL<NPulseSynapse>("Soma1.ExcSynapse"+sntoa(i),true);
         if(!syn)
           return true;

         if (syn->Input.IsConnected())
         {
          if(syn_num==i)
          {
           input_soma->NumExcitatorySynapses++;
           input_soma->Reset();
           syn_num = input_soma->NumExcitatorySynapses;
          }
           continue; //перейти к следующему синапсу
         }
         else
         {
          res&=CreateLink(output_ltzone->GetLongName(this),"Output",syn->GetLongName(this),"Input");
          if(!res)
           return true;
          break;
         }
        }
       }

      }
    }

//    //Убираем входящие связи на PostInput, чтобы освободить их для запоминания ситуации
//    UEPtr<NPulseNeuron> base_postinput = base_MPC->GetComponentL<NPulseNeuron>("PostInputNeuron1", true);
//    UEPtr<NPulseMembrane> base_soma = base_postinput->GetComponentL<NPulseMembrane>("Soma1", true);
//    base_soma->NumExcitatorySynapses=0;
//    base_soma->Reset();
//    base_soma->NumExcitatorySynapses=1;
//    base_soma->Reset();

    //Запоминаем текущую ситуацию
    NameT start_name =  base_MPC->GetName()+".InputNeuron1-1.LTZone";
    NameT finish_name = base_MPC->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
    if(!CheckLink(start_name,finish_name))
        CreateLink(start_name,"Output", finish_name, "Input");
    start_name =  base_MPC->GetName()+".PreControlNeuron1.LTZone";
    finish_name = base_MPC->GetName()+".ControlNeuron1-1.Soma1.ExcSynapse1";
    if(!CheckLink(start_name,finish_name))
        CreateLink(start_name,"Output", finish_name, "Input");

//    base_MPC->RememberState = true;



//    //Переходим к следующему элементу траектории;
//    for(int i = 0; i<int(TrajectoryElements.size()); i++)
//    {
//     UEPtr<NPulseNeuron> neuron = TrajectoryElements[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
//     UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
//     if(!ltzone)
//         return true;
//     int layer = TrajectoryElements[i]->Layer;

//     double check_frequency = ltzone->OutputFrequency->As<double>(0);

//     if((layer==CurrentLayer)&&(ltzone->OutputFrequency->As<double>(0) >0))
//     {
//       CurrentTE=i;
//       string check_TE = TrajectoryElements[CurrentTE]->GetName();
//     }
//    }


	return true;
}
// --------------------------


UEPtr<NTrajectoryElement> NMazeMemory::CreatePoint(MVector<double,3> coords)
{
 UEPtr<UContainer> cont;
 UEPtr<UStorage> storage = GetStorage();
 UEPtr<NTrajectoryElement> traj_el;

 int te_num = int(TrajectoryElements.size());
 int mpc_num = int(MultiPCs.size());

 //Добавляем элемент траектории
 traj_el = AddMissingComponent<NTrajectoryElement>(std::string("NTrajectoryElement"+sntoa(te_num)), "NTrajectoryElement");
 traj_el->SetCoord(coords);
 TrajectoryElements.push_back(traj_el);

 //Добавляем блок MultiPC
 string MultiPCName = "NMultiPositionControl"+sntoa(mpc_num);
 cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NMultiPositionControl"));
 if(!cont)
   return 0;
 cont->SetName(MultiPCName);
 cont->SetCoord(MVector<double,3>(coords[0], coords[1]+2.0, 0.0));
 AddComponent(cont);

 //Настройка блока MultiPC
 UEPtr<NMultiPositionControl> multi_pc = dynamic_pointer_cast<NMultiPositionControl>(cont);
 MultiPCs.push_back(multi_pc);
 multi_pc->BuildSolo = true;
 multi_pc->ExternalControl = false;
 multi_pc->IsNeedToRebuild = true;
 multi_pc->InputsNum = FeaturesNum;
 multi_pc->PrebuildStructure = true;
 multi_pc->Reset();

 //Построение связей от PostInputNeuron к TrajectoryElement
 bool res(true);

 UEPtr<NPulseNeuron> postinput = multi_pc->GetComponentL<NPulseNeuron>("PostInputNeuron1", true);
 UEPtr<NPulseLTZoneCommon> ltzone = postinput->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
//       if(!ltzone)
//           return true;

 UEPtr<NPulseNeuron> neuron1 = traj_el->GetComponentL<NPulseNeuron>("Neuron1", true);
 UEPtr<NPulseSynapse> synapse = neuron1->GetComponentL<NPulseSynapse>("Dendrite1_1.InhSynapse1",true);
 res&=CreateLink(ltzone->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
 //      if(!res)
 //       return true;

 synapse = neuron1->GetComponentL<NPulseSynapse>("Dendrite1_3.InhSynapse1",true);
 res&=CreateLink(ltzone->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
 //      if(!res)
 //       return true;

 UEPtr<NPulseNeuron> neuron2 = traj_el->GetComponentL<NPulseNeuron>("Neuron2", true);
 synapse = neuron2->GetComponentL<NPulseSynapse>("Dendrite1_1.InhSynapse1",true);
 res&=CreateLink(ltzone->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
 //      if(!res)
 //       return true;

 synapse = neuron2->GetComponentL<NPulseSynapse>("Dendrite1_3.InhSynapse1",true);
 res&=CreateLink(ltzone->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
 //      if(!res)
 //       return true;

 return traj_el;
}

}
#endif
