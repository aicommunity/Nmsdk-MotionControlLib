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
: Situation("Situation",this, &NMazeMemory::SetSituation),
  InputActions("InputActions",this, &NMazeMemory::SetInputActions),
  ActionNeuronsType("ActionNeuronsType",this, &NMazeMemory::SetActionNeuronsType),
  FeaturesNum("FeaturesNum",this, &NMazeMemory::SetFeaturesNum),
  IsDone("IsDone",this, &NMazeMemory::SetIsDone),
  SituationCoords("SituationCoords",this, &NMazeMemory::SetSituationCoords)
  //IsDeadlock("IsDeadlock",this, &NMazeMemory::SetIsDeadlock)

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
bool NMazeMemory::SetSituation(const bool &value)
{
 return true;
}

bool NMazeMemory::SetInputActions(const std::vector<int> &value)
{
 return true;
}

bool NMazeMemory::SetActionNeuronsType(const string &value)
{
 return true;
}

bool NMazeMemory::SetFeaturesNum(const int &value)
{
 SituationCoords.Resize(value,1);
 for (int j = 0; j<int(MultiPCs.size()); j++)
 {
     UEPtr<NNeuronTrainer> neuron_trainer = MultiPCs[j]->GetComponentL<NNeuronTrainer>("InputNeuron1-1", true);
     neuron_trainer->NumInputDendrite = value; //Заодно ресайзнет и InputPattern
     neuron_trainer->Reset();
     neuron_trainer->IsNeedToTrain = false;
 }
 return true;
}

bool NMazeMemory::SetIsDone(const bool &value)
{
 return true;
}

bool NMazeMemory::SetSituationCoords(const MDMatrix<double> &value)
{
    for (int j = 0; j<int(MultiPCs.size()); j++)
    {
        UEPtr<NNeuronTrainer> neuron_trainer = MultiPCs[j]->GetComponentL<NNeuronTrainer>("InputNeuron1-1", true);
        neuron_trainer->NumInputDendrite = value.GetRows(); //Заодно ресайзнет и InputPattern
        neuron_trainer->Reset();
        neuron_trainer->InputPattern = value;
        neuron_trainer->IsNeedToTrain = false;
    }

    // for (int j = 0; j<int(MultiPCs.size()); j++)
    // {
    //   UEPtr<NNeuronTrainer> neuron_trainer = MultiPCs[j]->GetComponentL<NNeuronTrainer>("InputNeuron1-1", true);
    //   int cnt = 0;
    //   int cnt_max = value.GetRows();
    //   MDMatrix<double> pattern;
    //   pattern.Resize(cnt_max+1,1);
    //   neuron_trainer->InputPattern.Resize(cnt_max+1,1);
    //   for (cnt = 0; cnt< cnt_max; cnt++)
    //   {
    //      pattern[cnt] = value[cnt];
    //   }
    //   pattern[cnt] = 0.2;
    //   neuron_trainer->InputPattern = pattern;
    // }

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
 Situation = false;
 InputActions.assign(3, 0); //(size,val)
 ActionNeuronsType = "NSPNeuronGen";
 FeaturesNum = 4; //x,y,alpha + калибровочное значение 0.2
 IsDone = false;

 const int temp = FeaturesNum;
 SituationCoords.Resize(temp,1);; //(rows,cols,val)

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
      int x_base = 5;
      int x_shift = 7;

      //Добавляем нейроны действия
      ActionNeurons.resize(InputActions.size());
      for (int n=0; n<ActionNeurons.size(); n++)
      {
        ActionNeurons[n] = AddMissingComponent<NPulseNeuron>(std::string("ActionNeuron")+sntoa(n+1), ActionNeuronsType);
        if(!ActionNeurons[n])
         return true;
        ActionNeurons[n]->SetCoord(MVector<double,3>(x_base+n*x_shift, 2.0, 0));
      }

      //Добавляем корневой элемент траектории
      MVector<double,3> root_coords = {5.0, 5.0, 0};
      UEPtr<NTrajectoryElement> root = CreatePoint(root_coords);
   }

 return true;
}

// Выполняет расчет этого объекта
bool NMazeMemory::ACalculate(void)
{
//  //Проставляем координаты текущей ситуации во все MultiPC схемы
//  for (int j = 0; j<int(MultiPCs.size()); j++)
//  {
//    UEPtr<NNeuronTrainer> neuron_trainer = MultiPCs[j]->GetComponentL<NNeuronTrainer>("InputNeuron1-1", true);
//    int cnt = 0;
//    int cnt_max = SituationCoords.GetRows();
//    //neuron_trainer->InputPattern.Resize(cnt_max+1,1);
//    MDMatrix<double> pattern;
//    pattern.Resize(cnt_max+1,1);
//    for (cnt = 0; cnt< SituationCoords.GetRows(); cnt++)
//    {
//       pattern[cnt] = SituationCoords[cnt];
//    }
//    pattern[cnt] = 0.2;
//    neuron_trainer->InputPattern = pattern;
//  }

  if (Situation)//Обрабатываем ситуацию
  {
    Situation = false;
    BaseTE = TrajectoryElements[CurrentTE];
    BaseMPC = MultiPCs[CurrentTE];
    PassedTEs.push_back(BaseTE);

    //int k = PassedTEs.size()-2;
    if (int(PassedTEs.size()-2) >0) //-2 - т.к индексация с 0 и т.к. берем предыдущий блок
    {
      PrevTE = PassedTEs[int(PassedTEs.size()-2)];
      string check_prevTE = string(PrevTE->GetName());
    }


    //Блок для отладки
    string check_baseTE = string(BaseTE->GetName());
    string check_baseMPC = string(BaseMPC->GetName());
//    std::vector<string> check_passed;
//    for (int j=0; j<int(PassedTEs.size()); j++)
//    {
//        check_passed[j] = string(PassedTEs[j]->GetName());
//    }


    //Проверяем, есть ли активные PostInput нейроны в сети -
    //т.е.совпадает ли текущая ситуация с какой-либо из уже изученных
    ActivePIs.clear();
    int active_number = -1;
    for(int i = 0; i<CurrentTE; i++)
    {
      UEPtr<NPulseNeuron> neuron = MultiPCs[i]->GetComponentL<NPulseNeuron>("PostInputNeuron1", true);
      UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
      //if(!ltzone)
        //return active_PIs;

      if(ltzone->OutputFrequency->As<double>(0) >0)
      {
        string check_actInp = TrajectoryElements[i]->GetName();
        ActivePIs.push_back(TrajectoryElements[i]);
        active_number = i;
      }
    }

   if(ActivePIs.size()>0)//если обнаружили другой активный ЭТ, помимо текущего,
   {
     MergingTEs(active_number); //сливаем активные ЭТ в один
   }


   //Проверяем, были ли уже в этой точке (были в точке = обучились=>есть связь между Input и PostInput)
   NameT inp_n =  BaseMPC->GetName()+".InputNeuron1-1";
   NameT pi_n = BaseMPC->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";

   if(!CheckLink(inp_n,pi_n)) //Если еще не были в этой точке
   {
       int OptionsNum = 0; //число возможных действий в текущей ситуации = числу ненулевых элементов в векторе InputActions
       for (int j = 0; j < InputActions.size(); j++)//Определяем число возможных действий по входному вектору
       {
          if (InputActions[j]>0)
              OptionsNum++;
       }

       if (OptionsNum <1) // если нет доступных направлений движения
       {
         if (PassedTEs.size()>1)
         {
           //w обратной связи на ЭТ(i-1) = 1,
           int num = int(BaseTE->Paths.size())-1;
           BaseTE->Paths[num]->Weight = 1;

           //w текущего ЭТ(i) = 0,2
           num = PrevTE->LastUsedPath;
           PrevTE->Paths[num]->Weight = 0.2;

           //убрать "тупиковый" ЭТ из стека пройденных точек
           PassedTEs.pop_back();
         }
         else
         {
            IsDone = true;
            return true;
         }

       }
       else // если есть доступные направления
       {
         UEPtr<NTrajectoryElement> traj_el;
         MVector<double,3> base_coords = BaseTE->GetCoord();

         CurrentLayer++;

         UEPtr<NPulseNeuron> base_neuron = TrajectoryElements[CurrentTE]->GetComponentL<NPulseNeuron>("Neuron1", true);
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

         int option_num = 0; // номер возможного действия = 0..OptionsNum
         for(int input_action = 0; input_action<int(InputActions.size()); input_action++)//номер элемента во входном векторе действий = 0...InputActions.size()
         {
           if (InputActions[input_action]==0)
               continue;

           option_num++;

          //Рассчитываем координаты и добавляем блоки
          double x = (double)CurrentLayer*LayerShift + base_coords[0];
          double y = (double)(option_num-1)*yShift + base_coords[1];
          MVector<double,3> coords = {x, y, 0.0};
          traj_el = CreatePoint(coords);//ДОБАВЛЕНИЕ БЛОКОВ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          traj_el->Reset();
          BaseTE->Forwards.push_back(traj_el);
          traj_el->Layer = CurrentLayer;

          //СТРОИМ СВЯЗИ ОТ ТЕКУЩЕГО ЭЛЕМЕНТА ТРАЕКТОРИИ НА ТОЛЬКО ЧТО СОЗДАННЫЕ
          bool res(true);
          //возбуждающее воздействие на N1_D5_syn1
          UEPtr<NPulseNeuron> input_neuron = traj_el->GetComponentL<NPulseNeuron>("Neuron1", true);
          UEPtr<NPulseSynapse> synapse = input_neuron->GetComponentL<NPulseSynapse>("Dendrite1_5.ExcSynapse1",true);
          if (option_num>1)
          {
            synapse->Weight = SideWeight;
            //string check_sn = synapse->GetLongName(this);
          }
          else
          {
            //traj_el->CurrentForward = 0;
            traj_el->LastUsedPath = 0;

          }
          res&=CreateLink(BaseTE->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
          if(!res)
           return true;
          BaseTE->Paths.push_back(synapse);

          //тормозное воздействие на N1_S1_syn1
          synapse = input_neuron->GetComponentL<NPulseSynapse>("Soma1.InhSynapse1",true);
          res&=CreateLink(BaseTE->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
          if(!res)
           return true;
          //тормозное воздействие на N2_S1_syn1
          input_neuron = traj_el->GetComponentL<NPulseNeuron>("Neuron2", true);
          synapse = input_neuron->GetComponentL<NPulseSynapse>("Soma1.InhSynapse1",true);
          res&=CreateLink(BaseTE->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
          if(!res)
           return true;

          //ОБРАТНЫЕ СВЯЗИ
          traj_el->Backwards.push_back(BaseTE);
          //traj_el->CurrentBackward = 0;

          //на сому
          int syn_num;
          if (CurrentTE==0)
              syn_num = option_num+1;
          else
              syn_num = option_num;
          synapse = base_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(syn_num),true);
          string check_s1 = synapse->GetLongName(this);
          synapse->Weight = 0.2;
          res&=CreateLink(traj_el->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
          if(!res)
           return true;

          //на N1_D1_2
          synapse = base_neuron->GetComponentL<NPulseSynapse>("Dendrite1_2.ExcSynapse"+sntoa(option_num),true);
          string check_s2 = synapse->GetLongName(this);
          synapse->Weight = 0.2;
          res&=CreateLink(traj_el->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
          if(!res)
           return true;
          traj_el->Paths.push_back(synapse);

          //связи между PreControl нейронами для "клича"
          int passed_max = PassedTEs.size();
          for (int j=0; j<passed_max; j++)
          {
           UEPtr<NMultiPositionControl> output_mpc = MultiPCs[j];
           UEPtr<NPulseNeuron> output_postinputn = output_mpc->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
           UEPtr<NLTZone> output_ltzone = output_postinputn->GetComponentL<NLTZone>("LTZone", true);

           UEPtr<NMultiPositionControl> input_mpc = MultiPCs[CurrentTE+option_num];//ПРОВЕРИТЬ
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

          //связи на нейроны действий
          UEPtr<NPulseMembrane> action_soma = ActionNeurons[input_action]->GetComponentL<NPulseMembrane>("Soma1",true);
          int syn_max_num = action_soma->NumExcitatorySynapses;
          for (int i = 1; i<=syn_max_num; i++)
          {
           UEPtr<NPulseSynapse> action_syn = action_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
           string check_act_name = action_soma->GetLongName(this);

           if (action_syn->Input.IsConnected())
           {
             if(syn_num==i)
             {
              action_soma->NumExcitatorySynapses++;
              action_soma->Reset();
              syn_max_num = action_soma->NumExcitatorySynapses;
             }
             continue; //перейти к следующему синапсу
           }
           else
           {
             res&=CreateLink(traj_el->GetLongName(this),"Output",action_syn->GetLongName(this),"Input");
             if(!res)
              return true;
             break;
           }
          }

         }
       }

       //Запоминаем текущую ситуацию (Обучились текущему положению)
       UEPtr<NNeuronTrainer> neuron_trainer = BaseMPC->GetComponentL<NNeuronTrainer>("InputNeuron1-1", true);
//       MDMatrix<double> pattern;
//       int cnt = 0;
//       int cnt_max = SituationCoords.GetRows();
//       pattern.Resize(cnt_max+1,1);
//       for (cnt = 0; cnt< SituationCoords.GetRows(); cnt++)
//       {
//          pattern[cnt] = SituationCoords[cnt];
//       }
//       pattern[cnt] = 0.2;
//       neuron_trainer->InputPattern = pattern;

       string check_name = neuron_trainer->GetLongName(this);
       neuron_trainer->IsNeedToTrain = true;
       bool check_isntt1 = neuron_trainer->IsNeedToTrain;
       //neuron_trainer->Reset();

       bool res1(true);
       bool res2(true);

       NameT start_name =  BaseMPC->GetName()+".InputNeuron1-1";
       NameT finish_name = BaseMPC->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
       if(!CheckLink(start_name,finish_name))
           res1 &= CreateLink(start_name,"Output", finish_name, "Input");

       start_name =  BaseMPC->GetName()+".PreControlNeuron1.LTZone";
       finish_name = BaseMPC->GetName()+".ControlNeuron1-1.Soma1.ExcSynapse1";
       if(!CheckLink(start_name,finish_name))
           res2 &= CreateLink(start_name,"Output", finish_name, "Input");

     }
     else //если уже были в этой точке
     {
       //UEPtr<NTrajectoryElement> PrevTE;
       //int k = PassedTEs.size()-2; //-1 - из-за индексации с 0 и -1 - т.к. берем предыдущий блок
       //if (k>0)
         //PrevTE = PassedTEs[k];
       //string check_prevTE = string(PrevTE->GetName());

       PassedTEs.clear();

       if (BaseTE->Layer < PrevTE->Layer)//Если поднялись на уровень выше по сравнению с предыдущим ЭТ
       {
         //Вес связи, по которой попали в эту точку, w = 0,2
         PrevTE->Paths[PrevTE->LastUsedPath]->Weight = 0.2;


         //Кидаем "клич"
         NameT fin = BaseMPC->GetName()+".PreControlNeuron1.Soma1.ExcSynapse1";
         bool res(true);
         res&=CreateLink(BaseTE->GetLongName(this),"Output", fin,"Input");
         //if(!res)
           //return true;

         bool response = false;
         UEPtr<NTrajectoryElement> responding_TE;

         int max_layer = 0;
         for (int j = 0; j< int(MultiPCs.size()); j++)
         {
           if (BaseTE->Layer - TrajectoryElements[j]->Layer > 1)
           {
             UEPtr<NPulseNeuron> neuron = MultiPCs[j]->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
             UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
             if(!ltzone)
                 return true;

             if (ltzone->OutputFrequency->As<double>(0)>0) //Проверяем, ответил ли кто-то через слой или ниже
             {
               response = true;
               if (TrajectoryElements[j]->Layer > max_layer)
                   max_layer = TrajectoryElements[j]->Layer;
             }
           }
         }

         if (response)//если кто-то ответил на "клич"
         {
           //для нижнего из сработавших нейронов - переключить связь на следующую
           responding_TE->Paths[responding_TE->LastUsedPath]->Weight=0.6;
           responding_TE->LastUsedPath++;
           responding_TE->Paths[responding_TE->LastUsedPath]->Weight=1;
         }
         else //если нет ответа
         {
           //для текущей в списке w  = 0.2
           //для следующей связи w = 1,
           BaseTE->Paths[responding_TE->LastUsedPath]->Weight=0.2;
           BaseTE->LastUsedPath++;
           BaseTE->Paths[responding_TE->LastUsedPath]->Weight=1;
         }

         res&=BreakLink(BaseTE->GetLongName(this),"Output", fin,"Input");
       }
     } //конец "если уже были в этой точке"

   }//конец if (Situation)

//     //Проверяем, есть ли активные направления (кроме обратных связей)
//     bool check_activeForwards = CheckActiveForwards(BaseTE);
//     if(!CheckActiveForwards(BaseTE)) //если нет активных направлений вперед
//     {
//       //w обратной связи на ЭТ(i-1) = 1
//       int num = int(BaseTE->Paths.size())-1;
//       string check_BaseTE_n = BaseTE->GetName();
//       string check_Path_n = BaseTE->Paths[num]->GetLongName(this);
//       BaseTE->Paths[num]->Weight = 1;

//       //w ВСЕХ связей на вход ЭТ(i) = 0.2
//       UEPtr<NPulseNeuron> neuron1 = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);
//       UEPtr<NPulseMembrane> dend1_5 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_5",true);
//       int max = dend1_5->NumExcitatorySynapses;
//       for (int i = 0; i<max; i++)
//       {
//           UEPtr<NPulseSynapse> synapse = dend1_5->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i+1),true);
//           string check_syn_n = synapse->GetLongName(this);
//           synapse->Weight = 0.2;
//       }

//       //У всех ли ЭТ блоки MultiPC обучены? - условие завершения алгоритма
//       bool done = true;
//       for(int i = 0; i<int(MultiPCs.size()); i++)
//       {
//         UEPtr<NNeuronTrainer> neuron_trainer = MultiPCs[i]->GetComponentL<NNeuronTrainer>("InputNeuron1-1", true);
//         UEPtr<NPulseNeuron> neuron = neuron_trainer->GetComponentL<NPulseNeuron>("Neuron",true);
//         UEPtr<NLTZone> ltzoneTr = neuron->GetComponentL<NLTZone>("LTZone", true);
//         NameT start_name =  ltzoneTr->GetLongName(this);
//         NameT finish_name = MultiPCs[i]->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
//         if(!CheckLink(start_name,finish_name))
//         {
//           done = false;
//           break;
//         }
//       }
//       IsDone = done;
//     }


//     //Переходим к следующему элементу траектории (обновляем CurrentTE)
//     for(int i = 0; i<int(TrajectoryElements.size()); i++)
//     {
//       UEPtr<NPulseNeuron> neuron = TrajectoryElements[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
//       UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
//       if(!ltzone)
//           return true;

//       if(ltzone->OutputFrequency->As<double>(0)>0)
//       {
//         CurrentTE=i;
//         string currentf_name = string(TrajectoryElements[i]->GetName());// имя ЭТ

//         //обновляем LastUsedPath (CurrentForward)
//         for(int j = 0; j<int(BaseTE->Paths.size()); j++)
//         {
//           string name = string(BaseTE->Paths[j]->GetLongName(this));// имя синапса
//           //if (name==currentf_name)
//           int check_comparison = name.compare(0, currentf_name.length(), currentf_name);
//           if (name.compare(0, currentf_name.length(), currentf_name)==0)
//           {
//             BaseTE->LastUsedPath = j;
//             break;
//           }
//         }
//       }
//     }

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
        multi_pc->InputsNum = 1;//Число InputNeurons, если InputNeuronsType = NSPNeuronGen (т.е. ситуация характеризуется набором бинарных признаков)
        multi_pc->PrebuildStructure = true;
        multi_pc->Reset();

        if ("NNeuronTrainerMemory"==string(multi_pc->InputNeuronType))
        {
            UEPtr<NNeuronTrainer> neuron_tr = multi_pc->GetComponentL<NNeuronTrainer>("InputNeuron1-1", true);
            if(neuron_tr)
            {
                neuron_tr->NumInputDendrite = FeaturesNum;//Число дендритов на InputNeuron, если InputNeuronsType = NNeuronTrainer (т.е. ситуация характеризуется набором численных признаков)
                neuron_tr->Reset();
                neuron_tr->SpikesFrequency = 30;//Чтобы тормозное воздействие на ЭТ было достаточно сильным для переключения на следующий ЭТ
                neuron_tr->IsUsedInMemory = false;
            }
        }


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

       bool NMazeMemory::CheckActiveForwards(UEPtr<NTrajectoryElement> t_element)
       {
         int max = int(t_element->Forwards.size());
         for(int i = 0; i<max; i++)
         {
           UEPtr<NPulseNeuron> neuron = t_element->Forwards[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
           UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
       //    if(!ltzone)
       //      return false;

           if(ltzone->OutputFrequency->As<double>(0) >0)
           {
             return true;
           }
         }
         return false;
       }


       std::vector<UEPtr<NTrajectoryElement>> NMazeMemory::CheckActivePIs()
       {
         std::vector<UEPtr<NTrajectoryElement>> active_PIs;
         for(int i = 0; i<CurrentTE; i++)
         {
           UEPtr<NPulseNeuron> neuron = MultiPCs[i]->GetComponentL<NPulseNeuron>("PostInputNeuron1", true);
           UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
       //    if(!ltzone)
       //      return active_PIs;

           if(ltzone->OutputFrequency->As<double>(0) >0)
           {
             string check_actInp = TrajectoryElements[i]->GetName();
             active_PIs.push_back(TrajectoryElements[i]);
           }
         }
         return active_PIs;
       }



       bool NMazeMemory::MergingTEs(int active_num)
       {
           int k = PassedTEs.size()-2;
           if (k<0)
               return true;

           PrevTE = PassedTEs[k];
           string check_prevTE = string(PrevTE->GetName());

           for (int j=0; j<ActivePIs.size(); j++)
           {
             //переносим ВХОДНЫЕ СВЯЗИ с текущего ЭТ на активный
             NameT start = PrevTE->GetLongName(this);
             bool res;
             NameT finish;

             //возуждающее воздействие на N1_D1_5
             UEPtr<NPulseNeuron> fin_neuron = ActivePIs[j]->GetComponentL<NPulseNeuron>("Neuron1", true);
             UEPtr<NPulseMembrane> fin_dend = fin_neuron->GetComponentL<NPulseMembrane>("Dendrite1_5", true);
             int dend_syns = fin_dend->NumExcitatorySynapses;
             for (int n = 1; n<=dend_syns; n++)
             {
               UEPtr<NPulseSynapse> syn = fin_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(n),true);
               if(!syn)
                 return true;

               if (syn->Input.IsConnected())
               {
                if(n==dend_syns)
                {
                 fin_dend->NumExcitatorySynapses++;
                 fin_dend->Reset();
                 dend_syns = fin_dend->NumExcitatorySynapses;
                }
                continue; //перейти к следующему синапсу
               }
               else
               {
                //finish_name = ActivePIs[j]->GetName()+".Neuron1.Dendrite1_5.ExcSynapse"+sntoa(n);
                finish = syn->GetLongName(this);
                res&=CreateLink(start,"Output",finish,"Input");
       //         if(!res)
       //          return true;
                break;
               }
             }

             //тормозное воздействие на N1_S1
             UEPtr<NPulseMembrane> fin_soma = fin_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
             int soma_syn = fin_soma->NumInhibitorySynapses;
             for (int n = 1; n<=soma_syn; n++)
             {
              UEPtr<NPulseSynapse> syn = fin_soma->GetComponentL<NPulseSynapse>("InhSynapse"+sntoa(n),true);
              if(!syn)
                return true;

              if (syn->Input.IsConnected())
              {
                if(n==soma_syn)
                {
                  fin_soma->NumInhibitorySynapses++;
                  fin_soma->Reset();
                  soma_syn = fin_soma->NumInhibitorySynapses;
                }
                continue; //перейти к следующему синапсу
              }
              else
              {
               //finish = ActivePIs[j]->GetName()+".Neuron1.Soma1.InhSynapse"+sntoa(n);
               finish = syn->GetLongName(this);
               res&=CreateLink(start,"Output",finish,"Input");
               //if(!res)
               // return true;
               break;
              }
             }

             //тормозное воздействие на N2_S1
             fin_neuron = ActivePIs[j]->GetComponentL<NPulseNeuron>("Neuron2", true);
             fin_soma = fin_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
             soma_syn = fin_soma->NumInhibitorySynapses;
             for (int n = 1; n<=soma_syn; n++)
             {
               UEPtr<NPulseSynapse> syn = fin_soma->GetComponentL<NPulseSynapse>("InhSynapse"+sntoa(n),true);
               if(!syn)
                 return true;

               if (syn->Input.IsConnected())
               {
                 if(n==soma_syn)
                 {
                   fin_soma->NumInhibitorySynapses++;
                   fin_soma->Reset();
                   soma_syn = fin_soma->NumInhibitorySynapses;
                 }
                 continue; //перейти к следующему синапсу
               }
               else
               {
                 finish = syn->GetLongName(this);
                 //finish = ActivePIs[j]->GetName()+".Neuron1.Soma1.InhSynapse"+sntoa(n);
                 res&=CreateLink(start,"Output",finish,"Input");
                 //if(!res)
                 //  return true;
                 break;
               }
             }

             //переносим ОБРАТНЫЕ СВЯЗИ с текущего ЭТ на активный
             start = ActivePIs[j]->GetLongName(this);

             int back_max = BaseTE->Backwards.size();
             for (int i = 0; i<back_max; i++)
             {
               //на N1_D1_2
               UEPtr<NPulseNeuron> fin_neuron = BaseTE->Backwards[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
               UEPtr<NPulseMembrane> fin_dend = fin_neuron->GetComponentL<NPulseMembrane>("Dendrite1_2", true);
               int dend_syns = fin_dend->NumExcitatorySynapses;
               for (int n = 1; n<=dend_syns; n++)
               {
                 UEPtr<NPulseSynapse> syn = fin_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(n),true);
                 if(!syn)
                   return true;

                 if (syn->Input.IsConnected())
                 {
                  if(n==dend_syns)
                  {
                   fin_dend->NumExcitatorySynapses++;
                   fin_dend->Reset();
                   dend_syns = fin_dend->NumExcitatorySynapses;
                  }
                   continue; //перейти к следующему синапсу
                 }
                 else
                 {
                  //finish_name = ActivePIs[j]->GetName()+".Neuron1.Dendrite1_5.ExcSynapse"+sntoa(n);
                  finish = syn->GetLongName(this);
                  res&=CreateLink(start,"Output",finish,"Input");
           //       if(!res)
           //        return true;
                  ActivePIs[j]->Paths.push_back(syn);
                  break;
                 }
               }


               //на N1_S1
               fin_neuron = BaseTE->Backwards[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
               fin_soma = fin_neuron->GetComponentL<NPulseMembrane>("Soma1", true);

               soma_syn = fin_soma->NumExcitatorySynapses;
               for (int n = 1; n<=soma_syn; n++)
               {
                 UEPtr<NPulseSynapse> syn = fin_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(n),true);
                 if(!syn)
                   return true;

                 if (syn->Input.IsConnected())
                 {
                   if(n==soma_syn)
                   {
                     fin_soma->NumExcitatorySynapses++;
                     fin_soma->Reset();
                     soma_syn = fin_soma->NumExcitatorySynapses;
                   }
                   continue; //перейти к следующему синапсу
                 }
                 else
                 {
                   finish = syn->GetLongName(this);
                   //finish = ActivePIs[j]->GetName()+".Neuron1.Soma1.ExcSynapse"+sntoa(n);
                   res&=CreateLink(start,"Output",finish,"Input");
                   //if(!res)
                   //  return true;
                   break;
                 }
               }

             }
             ActivePIs[j]->Backwards.push_back(PrevTE);
             //ActivePIs[j]->Paths.push_back(PrevTE);
           }

           DelComponent(BaseTE,true);
           BaseTE = ActivePIs[0];
           NameT check_base = BaseTE->GetName();

           DelComponent(BaseMPC, true);
           BaseMPC = MultiPCs[active_num];


           return true;
       }


       bool NMazeMemory::ProcessOptions()
       {

         return true;
       }


       bool NMazeMemory::LastUsedLink()
       {
           //Вес связи, по которой попали в эту точку, w = 0,2
           //Ищем активный синапс - т.е. связь, по которой пришли в этот TE
           bool found = false;
           UEPtr<NPulseNeuron> neuron1 = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);

           //Проверяем входящие связи на N1_D1_5
           UEPtr<NPulseMembrane> dend1_5 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_5",true);
           for (int i = 1; i<=(dend1_5->NumExcitatorySynapses); i++)
           {
             UEPtr<NPulseSynapse> synapse = dend1_5->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
             if (synapse->Output().As<double>(0)>0)
             {
                 synapse->Weight=0.2; //ПРОВЕРИТЬ!
                 found = true;
                 break;
             }
           }

           if (!found)
           {
             //Проверяем обратные связи (на N1_D1_2)
             UEPtr<NPulseMembrane> dend1_2 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_2",true);
             for (int i = 1; i<=(dend1_2->NumExcitatorySynapses); i++)
             {
               UEPtr<NPulseSynapse> synapse = dend1_2->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
               if (synapse->Output().As<double>(0)>0)
               {
                   synapse->Weight=0.2; //ПРОВЕРИТЬ!
                   found = true;
                   break;
               }
             }

             //Проверяем обратные связи (на N1_S1)
             UEPtr<NPulseMembrane> soma1 = neuron1->GetComponentL<NPulseMembrane>("Soma1",true);
             for (int i = 1; i<=(soma1->NumExcitatorySynapses); i++)
             {
               UEPtr<NPulseSynapse> synapse = soma1->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
               if (synapse->Output().As<double>(0)>0)
               {
                   synapse->Weight=0.2; //ПРОВЕРИТЬ!
                   found = true;
                   break;
               }
             }
           }

           return true;
       }



       bool NMazeMemory::DeadlockProcessing()//ТРЕБУЕТ ОТЛАДКИ
       {
       //    //w обратной связи на ЭТ(i-1) = 1
       //    UEPtr<NTrajectoryElement> PrevTE;
       //    int k = PassedTEs.size()-2;
       //    if (k>=0)
       //    {
       //      PrevTE = PassedTEs[k];
       //      string check_prevTE = string(PrevTE->GetName());
       //    }
       //    NameT start_name = PrevTE->GetName()+".Output";

       //    UEPtr<NPulseNeuron> fin_neuron = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);
       //    UEPtr<NPulseMembrane> fin_dend = fin_neuron->GetComponentL<NPulseMembrane>("Dendrite1_2", true);
       //    UEPtr<NPulseMembrane> fin_soma = fin_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
       //    int check_dmax = fin_dend->NumExcitatorySynapses();

       //    for(int i=0; i< fin_dend->NumExcitatorySynapses(); i++)
       //    {
       //      NameT finish_name = BaseTE->GetName()+".Neuron1.Dendrite1_2.ExcSynapse"+sntoa(i+1);
       //      if(CheckLink(start_name,finish_name))
       //      {
       //          UEPtr<NPulseSynapse> fin_synapse = fin_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i), true);
       //          fin_synapse->Weight=1;

       //              //fin_synapse = fin_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i), true);
       //              //fin_synapse->Weight=1;
       //      }
       //    }

       //    //w текущего ЭТ(i) = 0,2
       //    UEPtr<NPulseNeuron> base_neuron = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);
       //    UEPtr<NPulseMembrane> base_dend = base_neuron->GetComponentL<NPulseMembrane>("Dendrite1_5", true);
       //    for (int j=0; j<base_dend->NumExcitatorySynapses; j++)
       //    {
       //      UEPtr<NPulseSynapse> base_synapse = base_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(j), true);
       //      base_synapse->Weight=0.2;
       //    }

       //    //+убрать "тупиковый" ЭТ из стека пройденных точек
       //    PassedTEs.pop_back();

           return true;
       }

       }
       #endif
