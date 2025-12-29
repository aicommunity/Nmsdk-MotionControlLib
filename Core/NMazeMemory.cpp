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
  SituationCoords("SituationCoords",this, &NMazeMemory::SetSituationCoords),
  PassedTEsNames("PassedTEsNames",this)
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
 for (int j = 0; j < int(NTrainers.size()); j++)
 {
     UEPtr<NNeuronTrainer> neuron_trainer = NTrainers[j];
     if (neuron_trainer)
     {
         neuron_trainer->NumInputDendrite = value; //Заодно ресайзнет и InputPattern
         neuron_trainer->Reset();
     }

 }
 return true;
}

bool NMazeMemory::SetIsDone(const bool &value)
{
 return true;
}

bool NMazeMemory::SetSituationCoords(const MDMatrix<double> &value)
{
    for (int cnt = 0; cnt < int(NTrainers.size()); cnt++)
    {
        UEPtr<NNeuronTrainer> neuron_trainer = NTrainers[cnt];
        if (neuron_trainer)
        {
            neuron_trainer->NumInputDendrite = value.GetRows(); //Заодно ресайзнет и InputPattern
            neuron_trainer->Reset();
            neuron_trainer->InputPattern = value;
        }
    }

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
 yShift = 7.0;
 SideWeight = 0.75;

 IsNotFinished = false;
 WaitForSpike = 0;
 WaitForSpikePI = 0;
 IsWaitingForAnswer = false;
 WaitForAnswerCnt = 0;
 CheckFinish = false;

 //для отладки
 CheckBaseTE = "";

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
      BaseTE = root;
      CheckBaseTE = BaseTE->GetName();
      //PassedTEs.push_back(root);
   }

 return true;
}


// Выполняет расчет этого объекта
bool NMazeMemory::ACalculate(void)
{
  UpdateNames();

  if(CheckFinish)
  {
      CheckIfFinished();
      CheckFinish = false;
      return true;
  }

  if (IsWaitingForAnswer)
  {
      //Пауза после подачи активности на PreControl нейроны
      if(WaitForAnswerCnt < 1000) //5000
      {
          WaitForAnswerCnt++;
          return true;
      }

      //Проверяем, есть ли ответ на "клич"
      bool response = false;
      UEPtr<NTrajectoryElement> responding_TE;
      UEPtr<NTrajectoryElement> responding_PrevTE;

      int max_layer = 0;
      for (int j = 0; j< int(MultiPCs.size()); j++)
      {
          //Проверяем, если ли ответ через слой или ниже
          if (TrajectoryElements[j]->Layer - BaseTE->Layer > 1)
          {
              UEPtr<NPulseNeuron> neuron = MultiPCs[j]->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
              UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
              if(!ltzone)
                  return true;

              if (ltzone->OutputFrequency->As<double>(0)>0)
              {
                  response = true;
                  if (TrajectoryElements[j]->Layer > max_layer)
                  {
                      max_layer = TrajectoryElements[j]->Layer;
                      responding_TE = TrajectoryElements[j];
                      responding_PrevTE = TrajectoryElements[j]->Backwards.back();//берем последний элемент вектора
                      string check_responding_TE= string(responding_TE->GetLongName(this));
                      string check_responding_PrevTE = responding_PrevTE->GetName();
                  }
              }
          }
      }


      if (response)//если кто-то ответил на "клич"
      {
        //Для отладки
        string check_responding_PrevTE = responding_PrevTE->GetName();
        int check_LastUsedForward = responding_PrevTE->LastUsedForward;
        int check_LastUsedBackward = responding_PrevTE->LastUsedBackward;

        vector<string> check_ForwardSyns;
        for (int i=0; i < responding_PrevTE->ForwardSyns.size(); i++)
        {
            check_ForwardSyns.push_back(responding_PrevTE->ForwardSyns[i]->GetLongName(this));
        }
        vector<string> check_BackwardSyns;
        for (int i=0; i < responding_PrevTE->BackwardSyns.size(); i++)
        {
            check_BackwardSyns.push_back(responding_PrevTE->BackwardSyns[i]->GetLongName(this));
        }



        //для нижнего из сработавших нейронов - переключить связь на следующую
        SwitchToNextLink(responding_PrevTE);

//        //для нижнего из сработавших нейронов - переключить связь на следующую
//        if(responding_PrevTE->LastUsedForward < int(responding_PrevTE->ForwardSyns.size()))//если не вышли за пределы ForwardSyns
//        {
//          responding_PrevTE->ForwardSyns[responding_PrevTE->LastUsedForward]->Weight = 0; //при 0.2 - пробивает активность на следующем TE(
//          responding_PrevTE->LastUsedForward++;
//          if(responding_PrevTE->LastUsedForward < int(responding_PrevTE->ForwardSyns.size()))//если не вышли за пределы ForwardSyns
//            responding_PrevTE->ForwardSyns[responding_PrevTE->LastUsedForward]->Weight = 1;
//          else//если вышли за пределы ForwardSyns, переходим к BackwardSyns
//          {
//            responding_PrevTE->LastUsedBackward = 0;
//            responding_PrevTE->BackwardSyns[responding_PrevTE->LastUsedBackward]->Weight = 1; //Обновляем веса у двух связей подряд -
//            responding_PrevTE->BackwardSyns[responding_PrevTE->LastUsedBackward+1]->Weight = 1; //на N1_S1 и N1_D1_2
//          }
//        }
//        else
//        {
//          if(responding_PrevTE->LastUsedBackward == -1)
//          {
//            responding_PrevTE->LastUsedBackward++;
//          }
//          responding_PrevTE->BackwardSyns[responding_PrevTE->LastUsedBackward]->Weight = 0; //Обновляем веса у двух связей подряд -
//          responding_PrevTE->BackwardSyns[responding_PrevTE->LastUsedBackward+1]->Weight = 0; //на N1_S1 и N1_D1_2
//          responding_PrevTE->LastUsedBackward = responding_PrevTE->LastUsedBackward+2;
//          if(responding_PrevTE->LastUsedBackward < int(responding_PrevTE->BackwardSyns.size())-2)
//          {
//            responding_PrevTE->BackwardSyns[responding_PrevTE->LastUsedBackward]->Weight = 1;
//            responding_PrevTE->BackwardSyns[responding_PrevTE->LastUsedBackward+1]->Weight = 1;
//          }
//        }


        //Для отладки
        check_LastUsedForward = responding_PrevTE->LastUsedForward;
        check_LastUsedBackward = responding_PrevTE->LastUsedBackward;
        check_ForwardSyns.clear();
        for (int i=0; i < responding_PrevTE->ForwardSyns.size(); i++)
        {
          check_ForwardSyns.push_back(responding_PrevTE->ForwardSyns[i]->GetLongName(this));
        }
        check_BackwardSyns.clear();
        for (int i=0; i < responding_PrevTE->BackwardSyns.size(); i++)
        {
          check_BackwardSyns.push_back(responding_PrevTE->BackwardSyns[i]->GetLongName(this));
        }
      }
      else //если нет ответа
      {
          //для текущей в списке w  = 0 (//0.2), для следующей связи w = 1
          SwitchToNextLink(BaseTE);

      }

      //Удаляем дублирующий узел (TE+MultiPC)
      DelComponent(TEToDelete,true);
      DelComponent(MPCToDelete, true);

      //Выключаем обратно веса, которые использовали для передачи активности
      for (int i=0; i < int(SynsToChngWeights.size()); i++)
      {
        string check_name = SynsToChngWeights[i]->GetLongName(this);
        SynsToChngWeights[i]->Weight = 0;//0.2
      }

      //Возвращаем связь с NeuronTrainer  //NT!
      string check_name_nt = CurrentNT->GetLongName(this);
      bool res(true);
      NameT post_input = BaseMPC->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
      res&=CreateLink(CurrentNT->GetLongName(this),"Output", post_input,"Input");
      if(!res)
          return true;

      //Разрываем связь, которую строили для "клича"
      NameT fin = BaseMPC->GetName()+".PreControlNeuron1.Soma1.ExcSynapse1";
      res&=BreakLink(BaseTE->GetLongName(this),"Output", fin,"Input");
      if(!res)
          return true;

      //Обнуляем флаги
      //Situation = false;
      IsWaitingForAnswer = false;
      WaitForAnswerCnt = 0;

      CheckFinish = true;

      return true;
  }


  if(IsNotFinished)
  {
    //Ждем, пока обучится NeuronTrainer + еще немного, чтобы перейти к следующему TE
    if (CurrentNT->IsNeedToTrain==true)
       return true;
    else
    {
      WaitForSpike++;
      if(WaitForSpike < 2000)
       return true;
    }

    WaitForSpike = 0;

    //CheckIfFinished!
    //Проверяем, есть ли активные направления (кроме обратных связей)
    bool check_activeForwards = CheckActiveForwards(BaseTE);
    if(!CheckActiveForwards(BaseTE)) //если нет активных направлений вперед
    {               
        //w обратной связи на ЭТ(i-1) = 1
        string prev_name = string(PrevTE->GetName());
        for (int i = 0; i < int(BaseTE->BackwardSyns.size()); i = i+2)
        {
          string name = string(BaseTE->BackwardSyns[i]->GetLongName(this)).substr(0,18);
          if (name == prev_name)
          {
            BaseTE->BackwardSyns[i]->Weight = 1;
            BaseTE->BackwardSyns[i+1]->Weight = 1;
            string check_backw1 = BaseTE->BackwardSyns[i]->GetLongName(this);
            string check_backw2 = BaseTE->BackwardSyns[i+1]->GetLongName(this);
            break;
          }
        }


        //w ВСЕХ связей на вход ЭТ(i) = 0.2
        UEPtr<NPulseNeuron> neuron1 = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);
        UEPtr<NPulseMembrane> dend1_5 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_5",true);
        int max = dend1_5->NumExcitatorySynapses;
        for (int i = 0; i<max; i++)
        {
            UEPtr<NPulseSynapse> synapse = dend1_5->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i+1),true);
            string check_syn_n = synapse->GetLongName(this);
            synapse->Weight = 0;//0.2
        }


        //У всех ли ЭТ блоки MultiPC обучены? - условие завершения алгоритма
        bool done = true;
        for(int i = 0; i<int(TrajectoryElements.size()); i++)
        {
            string name = string(TrajectoryElements[i]->GetName());
            int num = int(name[18]-'0');
            string check_name_nt = "NeuronTrainer"+sntoa(num);

            UEPtr<NNeuronTrainer> neuron_trainer = GetComponentL<NNeuronTrainer>("NeuronTrainer"+sntoa(num), true);
            if(!((neuron_trainer)&&(neuron_trainer->IsNeedToTrain==false)))
            {
                done = false;
                break;
            }
        }
        IsDone = done;
    }


    //Переходим к следующему элементу траектории (обновляем CurrentTE)
    for(int i = 0; i<int(TrajectoryElements.size()); i++)
    {
        UEPtr<NPulseNeuron> neuron = TrajectoryElements[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
        UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
        if(!ltzone)
            return true;

        if(ltzone->OutputFrequency->As<double>(0)>0)
        {
            CurrentTE=i;
            string currentf_name = string(TrajectoryElements[i]->GetName());// имя ЭТ

//            //обновляем LastUsedPath (CurrentForward)
//            for(int j = 0; j<int(BaseTE->Paths.size()); j++)
//            {
//                string name = string(BaseTE->Paths[j]->GetLongName(this));// имя синапса
//                //if (name==currentf_name)
//                int check_comparison = name.compare(0, currentf_name.length(), currentf_name);
//                if (name.compare(0, currentf_name.length(), currentf_name)==0)
//                {
//                    BaseTE->LastUsedPath = j;
//                    break;
//                }
//            }
        }
    }
    PrevTE = BaseTE;
    IsNotFinished = false;
  }


  if (Situation)//Обрабатываем ситуацию
  {    
    if (WaitForSpikePI==0)//Обновляем переменные,только если сейчас не выдерживаем паузу
    {
      UpdateCurrentTE();
      BaseTE = TrajectoryElements[CurrentTE];
      BaseMPC = MultiPCs[CurrentTE];
      PassedTEs.push_back(BaseTE);
      if (int(PassedTEs.size()-2) >= 0) //-2 - т.к индексация с 0 и т.к. берем предыдущий блок
      {
        PrevTE = PassedTEs[int(PassedTEs.size()-2)];

        //Обновляем LastUsedForward (LastUsedBackward)
        PrevTE->LastUsedForward++;
        if (PrevTE->LastUsedForward > int(PrevTE->ForwardSyns.size()))
        {
            PrevTE->LastUsedBackward++;
            if (PrevTE->LastUsedBackward >= int(PrevTE->BackwardSyns.size()))
                PrevTE->LastUsedBackward--;
        }
      }
      else
        PrevTE = nullptr;

      //Блок для отладки
      CheckBaseTE = BaseTE->GetName();
      string check_baseMPC = string(BaseMPC->GetName());
      std::vector<string> check_passed;
      for (int j=0; j<int(PassedTEs.size()); j++)
      {
        check_passed.push_back(string(PassedTEs[j]->GetName()));
      }
      string check_prevTE;
      if (PrevTE == nullptr)
          check_prevTE = "nullptr";
      else
          check_prevTE = PrevTE->GetName();
    }// для отладки - проверить PrevTE и LastUsedForward

    //Проверяем, есть ли активные PostInput нейроны в сети -
    //т.е.совпадает ли текущая ситуация с какой-либо из уже изученных
    if (WaitForSpikePI < 1000)
    {
        WaitForSpikePI++;
        return true;
    }
    WaitForSpikePI = 0;
    ActivePIs.clear(); //NT!
    int active_index = CheckActivePIs(); //функция также изменяет и ActivePIs
    if(ActivePIs.size()>0)//если обнаружили другой активный ЭТ, помимо текущего,
    {
      MergingTEs(active_index); //сливаем активные ЭТ в один
    }

    //Проверяем, были ли уже в этой точке (если были в точке, обучились => есть соответствующий NeuronTrainer)
    string name = BaseTE->GetName();
    int num = int(name[18]-'0');
    string check_name_nt = "NeuronTrainer"+sntoa(num);
    CurrentNT = GetComponentL<NNeuronTrainer>("NeuronTrainer"+sntoa(num), true);

    if (!CurrentNT)//если еще не были
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
//           //w обратной связи на ЭТ(i-1) = 1,
//           int num = int(BaseTE->Paths.size())-1;
//           BaseTE->Paths[num]->Weight = 1;

           //w обратной связи на ЭТ(i-1) = 1
           string prev_name = string(PrevTE->GetName());
           for (int i = 0; i < int(BaseTE->BackwardSyns.size()); i = i+2)
           {
             string name = string(BaseTE->BackwardSyns[i]->GetLongName(this)).substr(0,19);
             if (name == prev_name)
             {
               BaseTE->BackwardSyns[i]->Weight = 1;
               BaseTE->BackwardSyns[i+1]->Weight = 1;
               string check_backw1 = BaseTE->BackwardSyns[i]->GetLongName(this);
               string check_backw2 = BaseTE->BackwardSyns[i+1]->GetLongName(this);
               break;
             }
           }

           //w текущего ЭТ(i) = 0,2
           //num = PrevTE->LastUsedPath;
           //PrevTE->Paths[num]->Weight = 0;  //0.2

           //w ВСЕХ связей на вход ЭТ(i) = 0.2
           UEPtr<NPulseNeuron> neuron1 = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);
           UEPtr<NPulseMembrane> dend1_5 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_5",true);
           int check_max = dend1_5->NumExcitatorySynapses;
           for (int i = 0; i < int(dend1_5->NumExcitatorySynapses); i++)
           {
               UEPtr<NPulseSynapse> synapse = dend1_5->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i+1),true);
               string check_syn_n = synapse->GetLongName(this);
               synapse->Weight = 0;//0.2
           }


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
         LinkPoint(traj_el, base_coords, OptionsNum);//Создаем TE+MultiPC и необходимые связи
       }

       //Запоминаем текущую ситуацию (Обучились текущему положению)
       string name = string(BaseTE->GetName());
       int num = int(name[18]-'0');
       string check_name_nt = "NeuronTrainer"+sntoa(num);

       UEPtr<NNeuronTrainer> neuron_trainer = AddMissingComponent<NNeuronTrainer>(std::string("NeuronTrainer"+sntoa(num)), "NNeuronTrainer");
       MVector<double,3> base_coords = BaseTE->GetCoord();
       neuron_trainer->SetCoord(MVector<double,3>(base_coords[0], base_coords[1]+4.0, 0));
       neuron_trainer->NumInputDendrite = FeaturesNum;
       neuron_trainer->Reset();
       neuron_trainer->InputPattern = SituationCoords;
       //neuron_trainer->LTZThreshold = 0.155;
       NTrainers.push_back(neuron_trainer);
       CurrentNT = neuron_trainer;
       IsNotFinished = true;

       bool res1(true);
       bool res2(true);

       NameT start_name =  neuron_trainer->GetName(); //NT!
       NameT finish_name = BaseMPC->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
       if(!CheckLink(start_name,finish_name))
         res1 &= CreateLink(start_name,"Output", finish_name, "Input");

       start_name =  BaseMPC->GetName()+".PreControlNeuron1.LTZone";
       finish_name = BaseMPC->GetName()+".ControlNeuron1-1.Soma1.ExcSynapse1";
       if(!CheckLink(start_name,finish_name))
         res2 &= CreateLink(start_name,"Output", finish_name, "Input");

       //Удаляем все входящие связи для "клича"
       string check_baseMPC = string(BaseMPC->GetName());
       UEPtr<NPulseNeuron> precontrol_n = BaseMPC->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
       UEPtr<NPulseMembrane> precontrol_soma = precontrol_n->GetComponentL<NPulseMembrane>("Soma1", true);
       for(int i = 0; i < precontrol_soma->NumExcitatorySynapses(); i++)
       {
         UEPtr<NPulseSynapse> pc_synapse = precontrol_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i+1),true);
         string check_pc_synapse = string(pc_synapse->GetLongName(this));
         if (pc_synapse)
           pc_synapse->DisconnectAllItems();
       }
    }
    else //если уже были в этой точке
    {
      PassedTEs.clear();
      PassedTEs.push_back(BaseTE);

      string check_base_name = string(BaseTE->GetName());
      int check_base_layer = BaseTE->Layer;
      string check_prev_name = string(PrevTE->GetName());
      int check_prev_layer = PrevTE->Layer;

      if (BaseTE->Layer <= PrevTE->Layer)//Если не спустились на уровень ниже по сравнению с предыдущим ЭТ (т.е. не продвинулись дальше в исследовании лабиринта)
      {
        //Вес связи, по которой попали в эту точку, w = 0,2
        //PrevTE->Paths[PrevTE->LastUsedPath]->Weight = 0;//0.2
        //int check_lastusedpath = PrevTE->LastUsedPath;
        //string check_paths = string(PrevTE->Paths[PrevTE->LastUsedPath]->GetLongName(this));


        //Вес связи, по которой попали в эту точку, w = 0,2
        string check_link = "";
        string check_PrevTE;
        int prev_LUF;
        int prev_LUB;
        vector<NameT> ForwNames;
        vector<NameT> BackwNames;

        if (PrevTE)
        {
            check_PrevTE = PrevTE->GetName();
            prev_LUF = PrevTE->LastUsedForward;
            prev_LUB = PrevTE->LastUsedBackward;

            ForwNames.clear();
            for(int cnt = 0; cnt < PrevTE->ForwardSyns.size(); cnt++)
            {
                ForwNames.push_back(PrevTE->ForwardSyns[cnt]->GetLongName(this));
            }

            BackwNames.clear();
            for(int cnt = 0; cnt < PrevTE->BackwardSyns.size(); cnt++)
            {
                BackwNames.push_back(PrevTE->BackwardSyns[cnt]->GetLongName(this));
            }
        }

        if((PrevTE->LastUsedForward > -1)&&(PrevTE->LastUsedForward < int(PrevTE->ForwardSyns.size()))) // если еще не все дочерние связи проверили
        {
          check_link = PrevTE->ForwardSyns[PrevTE->LastUsedForward]->GetLongName(this);
          PrevTE->ForwardSyns[PrevTE->LastUsedForward]->Weight = 0; //0,2
        }
        else if((PrevTE->LastUsedBackward > -1)&&(PrevTE->LastUsedBackward < int(PrevTE->BackwardSyns.size()))) // если все дочерние проверили, переходим к обратным связям
        {
          check_link = PrevTE->BackwardSyns[PrevTE->LastUsedBackward]->GetLongName(this);
          PrevTE->BackwardSyns[PrevTE->LastUsedBackward]->Weight = 0; //0,2
        }


        //Кидаем "клич"
        NameT fin = BaseMPC->GetName()+".PreControlNeuron1.Soma1.ExcSynapse1";
        string check_fin = string(fin);
        bool res(true);
        res&=CreateLink(BaseTE->GetLongName(this),"Output", fin,"Input");
        if(!res)
          return true;

        IsWaitingForAnswer = true;
       }

     } //конец "если уже были в этой точке"

    Situation = false;

  }//конец if(Situation)
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

   if (TrajectoryElements.size()>0)
   {
       string check1 = TrajectoryElements[int(TrajectoryElements.size())-1]->GetName();
       char check_te_num = string(TrajectoryElements[int(TrajectoryElements.size())-1]->GetName())[18];

       string name = string(TrajectoryElements[int(TrajectoryElements.size())-1]->GetName());
       te_num = int(name[18]-'0')+1;
   }
   if (MultiPCs.size()>0)
   {
       string check2 = MultiPCs[int(MultiPCs.size())-1]->GetName();
       char check_mpc_num = string(MultiPCs[int(MultiPCs.size())-1]->GetName())[21];

       string name = string(MultiPCs[int(MultiPCs.size())-1]->GetName());
       mpc_num = int(name[21]-'0')+1;
   }

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
   string check_multipc = multi_pc->GetLongName(this);

   //Построение связей от PostInputNeuron к TrajectoryElement
   bool res(true);

   UEPtr<NPulseNeuron> postinput = multi_pc->GetComponentL<NPulseNeuron>("PostInputNeuron1", true);
   string check_postinput = postinput->GetLongName(this);
   UEPtr<NPulseLTZoneCommon> ltzone = postinput->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
   string check_ltzone = ltzone->GetLongName(this);

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


 bool NMazeMemory:: LinkPoint(UEPtr<NTrajectoryElement> traj_el,  MVector<double,3> base_coords, int options_num)
 {
     UEPtr<NPulseNeuron> base_neuron = TrajectoryElements[CurrentTE]->GetComponentL<NPulseNeuron>("Neuron1", true);
     UEPtr<NLTZone> ltzone_te = base_neuron->GetComponentL<NLTZone>("LTZone", true);
     if(!ltzone_te)
         return true;

     //добавляем синапсы на текущий ЭТ для обратных связей на сому
     UEPtr<NPulseMembrane> base_soma = base_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
     if (CurrentTE==0)
         base_soma->NumExcitatorySynapses = options_num+1; //дополнительный синапс на первый ЭТ - для запускающего сигнала с высшего уровня СУ
     else
         base_soma->NumExcitatorySynapses = options_num;
     base_soma->Reset();

     //на N1_D1_2
     UEPtr<NPulseMembrane> base_dend = base_neuron->GetComponentL<NPulseMembrane>("Dendrite1_2", true);
     base_dend->NumExcitatorySynapses = options_num;
     base_dend->Reset();

     int possible_action_num = 0; // номер возможного действия = 0..OptionsNum
     for(int input_action = 0; input_action<int(InputActions.size()); input_action++)//номер элемента во входном векторе действий = 0...InputActions.size()
     {
         if (InputActions[input_action]==0)
             continue;

         possible_action_num++;

     double x = (double)CurrentLayer*LayerShift + base_coords[0];
     double y = (double)(possible_action_num-1)*yShift + base_coords[1];
     MVector<double,3> coords = {x, y, 0.0};
     traj_el = CreatePoint(coords);
     traj_el->Reset();
     int check_Forwards = traj_el->Forwards.size();
     NameT check_traj_el = traj_el->GetName();
     NameT check_BaseTE = BaseTE->GetName();
     BaseTE->Forwards.push_back(traj_el);
     traj_el->Layer = CurrentLayer;

     //СТРОИМ СВЯЗИ ОТ ТЕКУЩЕГО ЭЛЕМЕНТА ТРАЕКТОРИИ НА ТОЛЬКО ЧТО СОЗДАННЫЕ
     bool res(true);
     //возбуждающее воздействие на N1_D5_syn1
     UEPtr<NPulseNeuron> input_neuron = traj_el->GetComponentL<NPulseNeuron>("Neuron1", true);
     UEPtr<NPulseSynapse> synapse = input_neuron->GetComponentL<NPulseSynapse>("Dendrite1_5.ExcSynapse1",true);
     if (possible_action_num>1)
     {
         synapse->Weight = SideWeight;
         string check_sn = synapse->GetLongName(this);
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
     BaseTE->ForwardSyns.push_back(synapse);

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
     int check_Backwards = traj_el->Backwards.size();
     check_traj_el = traj_el->GetName();
     check_BaseTE = BaseTE->GetName();
     traj_el->Backwards.clear();
     traj_el->Backwards.push_back(BaseTE);
     //traj_el->CurrentBackward = 0;

     //на сому
     int syn_num;
     if (CurrentTE==0)
         syn_num = possible_action_num+1;
     else
         syn_num = possible_action_num;
     synapse = base_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(syn_num),true);
     string check_s1 = synapse->GetLongName(this);
     synapse->Weight = 0;//0.2
     traj_el->BackwardSyns.clear();
     traj_el->BackwardSyns.push_back(synapse);
     res&=CreateLink(traj_el->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
     if(!res)
         return true;

     //на N1_D1_2
     synapse = base_neuron->GetComponentL<NPulseSynapse>("Dendrite1_2.ExcSynapse"+sntoa(possible_action_num),true);
     string check_s2 = synapse->GetLongName(this);
     synapse->Weight = 0;//0.2
     res&=CreateLink(traj_el->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
     if(!res)
         return true;
     traj_el->Paths.push_back(synapse);
     traj_el->BackwardSyns.push_back(synapse);
     int check_Paths = traj_el->Paths.size();

     //связи между PreControl нейронами для "клича"
     int passed_max = PassedTEs.size();
     for (int j=0; j<passed_max; j++)
     {   
         UEPtr<NMultiPositionControl> output_mpc = MultiPCs[j];
         UEPtr<NPulseNeuron> output_precontroln = output_mpc->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
         UEPtr<NLTZone> output_ltzone = output_precontroln->GetComponentL<NLTZone>("LTZone", true);
         if (!output_ltzone)
             return false;

         UEPtr<NMultiPositionControl> input_mpc = MultiPCs[int(MultiPCs.size())-1];//ПРОВЕРИТЬ
         UEPtr<NPulseNeuron> input_precontroln = input_mpc->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
         UEPtr<NPulseMembrane> input_soma = input_precontroln->GetComponentL<NPulseMembrane>("Soma1", true);
         if (!input_soma)
             return false;


         //блок для отладки
         string check_outp_mpc = string(output_mpc->GetName());
         string check_output_postinputn = string(output_mpc->GetLongName(this));
         string check_output_ltzone = string(output_ltzone->GetLongName(this));

         string check_inp_mpc = string(input_mpc->GetName());
         string check_input_precontroln = string(input_precontroln->GetLongName(this));
         string check_input_soma = string(input_soma->GetLongName(this));



         int syn_num = input_soma->NumExcitatorySynapses;
         for (int i = 1; i<=syn_num; i++)
         {
             UEPtr<NPulseSynapse> syn = input_precontroln->GetComponentL<NPulseSynapse>("Soma1.ExcSynapse"+sntoa(i),true);
             if(!syn)
                 return true;

             string check_syn = string(syn->GetName());

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
             if((i+1)>syn_max_num)
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
     return true;
 }


bool NMazeMemory::CheckActiveForwards(UEPtr<NTrajectoryElement> t_element)
{
  int max = int(t_element->Forwards.size());
  for(int i = 0; i<max; i++)
  {
    string checknname = t_element->Forwards[i]->GetName();

    if(t_element->Forwards[i]->GetName() == t_element->GetName()) // проверяем, что найденный возможный путь вперед не является циклом и не ведет снова к текущему ЭТ
    {
        continue;
    }

    UEPtr<NPulseNeuron> neuron = t_element->Forwards[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
    if(!neuron)
      return false;
    UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
    if(!ltzone)
      return false;

    string check_ltz = ltzone->GetLongName(this);
    double check_frequency = ltzone->OutputFrequency->As<double>(0);
    if(ltzone->OutputFrequency->As<double>(0) >0)
    {
      //if(t_element->Forwards[i]->GetName() != t_element->GetName()) // проверяем, что найденный возможный путь вперед не является циклом и не ведет снова к текущему ЭТ
      //{
          return true;
      //}
    }
  }
  return false;
}


int NMazeMemory::CheckActivePIs() //NT!
{

    int active_index = -1;
    for(int i = 0; i<CurrentTE; i++)
    {
        UEPtr<NPulseNeuron> neuron = MultiPCs[i]->GetComponentL<NPulseNeuron>("PostInputNeuron1", true);
        UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
        if(!ltzone)
          return -2;

        if(ltzone->OutputFrequency->As<double>(0) >0)
        {
            string check_actInp = TrajectoryElements[i]->GetName();
            ActivePIs.push_back(TrajectoryElements[i]);
            active_index = i;
        }
    }
    return active_index; //индекс активного элемента в массиве TEs
}


bool NMazeMemory::SwitchToNextLink(UEPtr<NTrajectoryElement> t_element)
{
    //Для отладки
    string check_TE = t_element->GetName();
    int check_LastUsedForward = t_element->LastUsedForward;
    int check_LastUsedBackward = t_element->LastUsedBackward;

    vector<string> check_ForwardSyns;
    for (int i=0; i < t_element->ForwardSyns.size(); i++)
    {
        check_ForwardSyns.push_back(t_element->ForwardSyns[i]->GetLongName(this));
    }
    vector<string> check_BackwardSyns;
    for (int i=0; i < t_element->BackwardSyns.size(); i++)
    {
        check_BackwardSyns.push_back(t_element->BackwardSyns[i]->GetLongName(this));
    }


    //если не вышли за пределы ForwardSyns
    if(t_element->LastUsedForward < int(t_element->ForwardSyns.size()))
    {
      if (t_element->LastUsedForward > -1)
      {
        t_element->ForwardSyns[t_element->LastUsedForward]->Weight = 0; //при 0.2 - пробивает активность на следующем TE(
      }
      t_element->LastUsedForward++;
      if(t_element->LastUsedForward < int(t_element->ForwardSyns.size()))//если не вышли за пределы ForwardSyns
      {
        t_element->ForwardSyns[t_element->LastUsedForward]->Weight = 1;
      }
      else//если вышли за пределы ForwardSyns, переходим к BackwardSyns
      {
        t_element->LastUsedBackward = 0;
        t_element->BackwardSyns[t_element->LastUsedBackward]->Weight = 1; //Обновляем веса у двух связей подряд -
        t_element->BackwardSyns[t_element->LastUsedBackward+1]->Weight = 1; //на N1_S1 и N1_D1_2
      }
    }
    else //если вышли за пределы ForwardSyns, переходим к обратным связям
    {
      if(t_element->LastUsedBackward > -1)
      {
          t_element->BackwardSyns[t_element->LastUsedBackward]->Weight = 0; //Обновляем веса у двух связей подряд -
          t_element->BackwardSyns[t_element->LastUsedBackward+1]->Weight = 0; //на N1_S1 и N1_D1_2
          t_element->LastUsedBackward = t_element->LastUsedBackward+2;
      }
      else
      {
          t_element->LastUsedBackward = 0;
      }
      if(t_element->LastUsedBackward < int(t_element->BackwardSyns.size())-2)
      {
        t_element->BackwardSyns[t_element->LastUsedBackward]->Weight = 1;
        t_element->BackwardSyns[t_element->LastUsedBackward+1]->Weight = 1;
      }
    }





    //Для отладки
    check_LastUsedForward = t_element->LastUsedForward;
    check_LastUsedBackward = t_element->LastUsedBackward;
    check_ForwardSyns.clear();
    for (int i=0; i < t_element->ForwardSyns.size(); i++)
    {
      check_ForwardSyns.push_back(t_element->ForwardSyns[i]->GetLongName(this));
    }
    check_BackwardSyns.clear();
    for (int i=0; i < t_element->BackwardSyns.size(); i++)
    {
      check_BackwardSyns.push_back(t_element->BackwardSyns[i]->GetLongName(this));
    }



    return true;
}


bool NMazeMemory::MergingTEs(int active_index)
{
  int k = PassedTEs.size()-2;
  if (k<0)
      return false;

  PrevTE = PassedTEs[k];
  if (!PrevTE)
      return false;
  NameT prevTE_name = PrevTE->GetName();

  for (int j = 0; j < int(ActivePIs.size()); j++)
  {
    NameT active_name = ActivePIs[j]->GetName();

    //ЕСЛИ СОВПАЛИ С СИТУАЦИЕЙ, В КОТОРОЙ БЫЛИ НА ПРОШЛОМ ШАГЕ (т.е. сливаем узел с предыдущим)
    if (prevTE_name == active_name)
    {
      //Увеличиваем вес обратных связей на прошлый TE
      //на соме
      NameT start_name =  BaseTE->GetName();
      UEPtr<NPulseNeuron> prev_neuron = PrevTE->GetComponentL<NPulseNeuron>("Neuron1", true);
      UEPtr<NPulseMembrane> prev_dend = prev_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
      SynsToChngWeights.clear();
      if(!prev_dend)
        return true;
      for (int i = 1; i<= prev_dend->NumExcitatorySynapses; i++)
      {
        UEPtr<NPulseSynapse> syn = prev_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
        if(!syn)
          return true;
        NameT finish_name = syn->GetLongName(this);
        if(CheckLink(start_name,finish_name))
        {
          syn->Weight = 1;
          SynsToChngWeights.push_back(syn);
        }

      }
      //на D1_2
      prev_dend = prev_neuron->GetComponentL<NPulseMembrane>("Dendrite1_2", true);
      if(!prev_dend)
        return true;
      for (int i = 1; i<= prev_dend->NumExcitatorySynapses; i++)
      {
        UEPtr<NPulseSynapse> syn = prev_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
        if(!syn)
            return true;
        NameT finish_name = syn->GetLongName(this);
        if(CheckLink(start_name,finish_name))
        {
          syn->Weight = 1;
          SynsToChngWeights.push_back(syn);
        }
      }

      //Временно отключаем воздействие NeuronTrainer, чтобы он не заглушал активность TE  //NT!
      bool res(true);
      NameT active_PI = MultiPCs[active_index]->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
      string check_actNT = string(active_PI);
      string check_NT = string(CurrentNT->GetLongName(this));
      res&=BreakLink(CurrentNT->GetLongName(this),"Output", active_PI,"Input");
      if(!res)
          return true;

      //Обновляем Forwards того TE, с которым сливаемся
      //(заменяем удаляемый узел на тот, с которым сливаем)
      //Можно просто удалить, без замены?         
      replace(PrevTE->Forwards.begin(), PrevTE->Forwards.end(), BaseTE, PrevTE);//(begin, end, el_to_be_replaced, el_to_replace)

      //Обновляем ForwardSyns - убираем синапс, на который заведена связь на удаляемый (текущий) элемент
      for (int i = 0; i < int(PrevTE->ForwardSyns.size()); i++)
      {
        string prev_syn_name = (PrevTE->ForwardSyns[i]->GetLongName(this)).substr(0,19);
        string base_syn_name = BaseTE->GetLongName(this);
        if (prev_syn_name == base_syn_name)
        {
          PrevTE->ForwardSyns.erase(PrevTE->ForwardSyns.begin()+i);
          if(i<=PrevTE->LastUsedForward)
              PrevTE->LastUsedForward--;
        }
      }

      //Для отладки
      vector<string> CheckForwardsName;
      int forwards_max = PrevTE->Forwards.size();
      for (int i = 0; i<forwards_max; i++)
      {
        CheckForwardsName.push_back(PrevTE->Forwards[i]->GetName());
      }

      //Обновляем, чтобы передать активность
      for(int clc_cnt = 0; clc_cnt <1000; clc_cnt++)
      {
        ActivePIs[j]->Calculate();
      }
    }
    else //ЕСЛИ СОВПАЛИ С СИТУАЦИЕЙ, В КОТОРОЙ БЫЛИ РАНЬШЕ, ЧЕМ НА ПРОШЛОМ ШАГЕ
    {
      //Обновляем значение CurrentNT
      string name = string(MultiPCs[active_index]->GetName());
      int num = int(name[21]-'0');
      string check_name_nt = "NeuronTrainer"+sntoa(num);
      CurrentNT = GetComponentL<NNeuronTrainer>("NeuronTrainer"+sntoa(num), true);
      if(!CurrentNT)
        return true;

      //Временно отключаем воздействие активного NeuronTrainer, чтобы он не заглушал активность TE  //NT!
      bool res(true);
      NameT active_PI = MultiPCs[active_index]->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
      res&=BreakLink(CurrentNT->GetLongName(this),"Output", active_PI,"Input");
      //if(!res)
      //    return true;

      //Передаем активность с текущего TE на тот, у которого активен NT (т.е. с TE ситуации, с которой совпали)
      NameT start = BaseTE->GetLongName(this);
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
  //     if(!res)
  //        return true;
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


      //Подключаем активный NT к текущему TE, чтобы переключить активность  //NT!
      NameT current_PI = BaseMPC->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
      res&=CreateLink(CurrentNT->GetLongName(this),"Output",current_PI,"Input");
      if(!res)
          return true;

      //Обновляем TE, на который перенесли активность
      for(int clc_cnt = 0; clc_cnt <1000; clc_cnt++)
      {
        ActivePIs[j]->Calculate();
        BaseTE->Calculate();
      }


      //переносим ВХОДНЫЕ СВЯЗИ с текущего ЭТ на активный
      start = PrevTE->GetLongName(this);
      //bool res;

      //возуждающее воздействие на N1_D1_5
      fin_neuron = ActivePIs[j]->GetComponentL<NPulseNeuron>("Neuron1", true);
      fin_dend = fin_neuron->GetComponentL<NPulseMembrane>("Dendrite1_5", true);
      dend_syns = fin_dend->NumExcitatorySynapses;
      UEPtr<NPulseSynapse> active_syn;// пригодится ниже :)
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
          active_syn = syn;
          syn->Weight = 0;
          finish = syn->GetLongName(this);
          res&=CreateLink(start,"Output",finish,"Input");
  //        if(!res)
  //         return true;
          break;
        }
      }

      //тормозное воздействие на N1_S1
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
          syn->Weight = 0;
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
          syn->Weight = 0;
          finish = syn->GetLongName(this);
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
            syn->Weight = 0;
            finish = syn->GetLongName(this);
            ActivePIs[j]->BackwardSyns.push_back(syn);
            res&=CreateLink(start,"Output",finish,"Input");
            //if(!res)
            //  return true;
            break;
          }
        }

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
            syn->Weight = 0;
            finish = syn->GetLongName(this);
            res&=CreateLink(start,"Output",finish,"Input");
      //        if(!res)
      //         return true;
              ActivePIs[j]->Paths.push_back(syn);
              ActivePIs[j]->BackwardSyns.push_back(syn);

              break;
           }
        }
      }

      //Обновляем Backwards того TE, с которым сливаемся
      string check_ActivePIs_j = ActivePIs[j]->GetName();
      string check_prevTE = PrevTE->GetName();
      ActivePIs[j]->Backwards.push_back(PrevTE);
      //ActivePIs[j]->Paths.push_back(PrevTE);

      //Обновляем Forwards того TE, с которым сливаемся
      //(заменяем удаляемый узел на тот, с которым сливаем)
      //Можно просто удалить, без замены?
      string check_base_name = BaseTE->GetName();
      string check_active_name = ActivePIs[j]->GetName();
      replace(PrevTE->Forwards.begin(), PrevTE->Forwards.end(), BaseTE, ActivePIs[j]); //(begin, end, el_to_be_replaced, el_to_replace)

      //Обновляем ForwardSyns - заменяем синапс, на который заведена связь от PrevTE к BaseTE, на синапс активного TE
      UEPtr<NPulseSynapse> base_syn = GetForwardSyn(PrevTE, BaseTE); //синапс, на который заведена связь от PrevTE к BaseT
      //replace(PrevTE->ForwardSyns.begin(), PrevTE->ForwardSyns.end(), base_syn, active_syn); //(begin, end, el_to_be_replaced, el_to_replace)

      string base_syn_name = base_syn->GetLongName(this);
      for (int i = 0; i < int(PrevTE->ForwardSyns.size()); i++)
      {
        string prev_syn_name = PrevTE->ForwardSyns[i]->GetLongName(this);
        if (prev_syn_name == base_syn_name)
        {
          PrevTE->ForwardSyns[i] = active_syn;
          if(i <= PrevTE->LastUsedForward)
              PrevTE->LastUsedForward--;
        }
      }


      //Для отладки
      vector<string> BackwardsName;
      int backwards_max = ActivePIs[j]->Backwards.size();
      for (int i = 0; i<backwards_max; i++)
      {
        BackwardsName.push_back(ActivePIs[j]->Backwards[i]->GetName());
      }

      vector<string>ForwardsName;
      int forwards_max = PrevTE->Forwards.size();
      for (int i = 0; i<forwards_max; i++)
      {
        ForwardsName.push_back(PrevTE->Forwards[i]->GetName());
      }
    }
  }

  //Запоминаем дублирующий TE (чтобы потом удалить), обновляем индексы
  TEToDelete = BaseTE;
  MPCToDelete = BaseMPC;
  BaseTE = ActivePIs[0];
  BaseMPC = MultiPCs[active_index];
  TrajectoryElements.erase(TrajectoryElements.begin()+CurrentTE);
  MultiPCs.erase(MultiPCs.begin()+CurrentTE);
  CurrentTE = active_index;
  CheckBaseTE = BaseTE->GetName();
  return true;
}

UEPtr<NPulseSynapse> NMazeMemory::GetForwardSyn(UEPtr<NTrajectoryElement> start_te, UEPtr<NTrajectoryElement> finish_te)
{
    NameT start_name = start_te->GetName();

    UEPtr<NPulseNeuron> neuron1 = finish_te->GetComponentL<NPulseNeuron>("Neuron1", true);
    UEPtr<NPulseMembrane> dend1_5 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_5", true);
    int syn_num = dend1_5->NumExcitatorySynapses;
    for (int i = 1; i <= syn_num; i++)
    {
      UEPtr<NPulseSynapse> syn = dend1_5->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
      if(!syn)
        return nullptr;
      NameT finish_name = syn->GetLongName(this);
      if(CheckLink(start_name,finish_name))
      {
          return (syn);
      }
    }
    return nullptr;
}


bool NMazeMemory::UpdateCurrentTE()
{
  //Переходим к следующему элементу траектории (обновляем CurrentTE)
  for(int i = 0; i<int(TrajectoryElements.size()); i++)
  {
    UEPtr<NPulseNeuron> neuron = TrajectoryElements[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
    UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
    if(!ltzone)
        return true;

    if(ltzone->OutputFrequency->As<double>(0)>0)
    {
      CurrentTE=i;
      string currentf_name = string(TrajectoryElements[i]->GetName());// имя ЭТ

      //обновляем LastUsedPath (CurrentForward)
      for(int j = 0; j<int(BaseTE->Paths.size()); j++)
      {
        string name = string(BaseTE->Paths[j]->GetLongName(this));// имя синапса
        //if (name==currentf_name)
        int check_comparison = name.compare(0, currentf_name.length(), currentf_name);
        if (name.compare(0, currentf_name.length(), currentf_name)==0)
        {
          BaseTE->LastUsedPath = j;
          break;
        }
      }
    }
  }
  return true;
}


bool NMazeMemory::UpdateNames()
{
  for(int i=0; i < int(TrajectoryElements.size()); i++)
  {
    string check_Te = TrajectoryElements[i]->GetName();
    //Обновляем Forwards
    TrajectoryElements[i]->ForwardsNames.resize(TrajectoryElements[i]->Forwards.size());
    int check_max_f = int(TrajectoryElements[i]->Forwards.size());
    for(int j=0; j < int(TrajectoryElements[i]->Forwards.size()); j++)
    {
      TrajectoryElements[i]->ForwardsNames[j] = TrajectoryElements[i]->Forwards[j]->GetName();
    }

    //Обновляем Backwards
    int check_max_b = int(TrajectoryElements[i]->Backwards.size());
    TrajectoryElements[i]->BackwardsNames.resize(TrajectoryElements[i]->Backwards.size());
    for(int j=0; j < int(TrajectoryElements[i]->Backwards.size()); j++)
    {
      TrajectoryElements[i]->BackwardsNames[j] = TrajectoryElements[i]->Backwards[j]->GetName();
    }

    //Обновляем Paths
    int check_max_p = int(TrajectoryElements[i]->Paths.size());
    TrajectoryElements[i]->PathsNames.resize(TrajectoryElements[i]->Paths.size());
    for(int j=0; j < int(TrajectoryElements[i]->Paths.size()); j++)
    {
      TrajectoryElements[i]->PathsNames[j] = TrajectoryElements[i]->Paths[j]->GetLongName(this);
    }

    //Обновляем ForwardSyns
    int check_max_fs = int(TrajectoryElements[i]->ForwardSyns.size());
    TrajectoryElements[i]->ForwardSynsNames.resize(TrajectoryElements[i]->ForwardSyns.size());
    for(int j=0; j < int(TrajectoryElements[i]->ForwardSyns.size()); j++)
    {
      TrajectoryElements[i]->ForwardSynsNames[j] = TrajectoryElements[i]->ForwardSyns[j]->GetLongName(this);
    }

    //Обновляем BackwardSyns
    int check_max_bs = int(TrajectoryElements[i]->BackwardSyns.size());
    TrajectoryElements[i]->BackwardSynsNames.resize(TrajectoryElements[i]->BackwardSyns.size());
    for(int j=0; j < int(TrajectoryElements[i]->BackwardSyns.size()); j++)
    {
      TrajectoryElements[i]->BackwardSynsNames[j] = TrajectoryElements[i]->BackwardSyns[j]->GetLongName(this);
    }

  }

  //Обновляем PassedTEs
  PassedTEsNames.resize(PassedTEs.size());
  for(int j=0; j < int(PassedTEs.size()); j++)
  {
    PassedTEsNames[j] = PassedTEs[j]->GetLongName(this);
  }

  return true;
}


bool NMazeMemory::CheckIfFinished()
{
    //Проверяем, есть ли активные направления (кроме обратных связей)
    bool check_activeForwards = CheckActiveForwards(BaseTE);
    if(!CheckActiveForwards(BaseTE)) //если нет активных направлений вперед
    {
        //w обратной связи на ЭТ(i-1) = 1
        if(PrevTE)
        {
          string prev_name = string(PrevTE->GetName());
          for (int i = 0; i < int(BaseTE->BackwardSyns.size()); i = i+2)
          {
            string name = string(BaseTE->BackwardSyns[i]->GetLongName(this)).substr(0,18);
            if (name == prev_name)
            {
              BaseTE->BackwardSyns[i]->Weight = 1;
              BaseTE->BackwardSyns[i+1]->Weight = 1;
              string check_backw1 = BaseTE->BackwardSyns[i]->GetLongName(this);
              string check_backw2 = BaseTE->BackwardSyns[i+1]->GetLongName(this);
              break;
            }
          }
        }



        //w ВСЕХ связей на вход ЭТ(i) = 0.2
        UEPtr<NPulseNeuron> neuron1 = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);
        UEPtr<NPulseMembrane> dend1_5 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_5",true);
        int max = dend1_5->NumExcitatorySynapses;
        for (int i = 0; i<max; i++)
        {
            UEPtr<NPulseSynapse> synapse = dend1_5->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i+1),true);
            string check_syn_n = synapse->GetLongName(this);
            synapse->Weight = 0;//0.2
        }


        //У всех ли ЭТ блоки MultiPC обучены? - условие завершения алгоритма
        bool done = true;
        for(int i = 0; i<int(TrajectoryElements.size()); i++)
        {
            string name = string(TrajectoryElements[i]->GetName());
            int num = int(name[18]-'0');
            string check_name_nt = "NeuronTrainer"+sntoa(num);

            UEPtr<NNeuronTrainer> neuron_trainer = GetComponentL<NNeuronTrainer>("NeuronTrainer"+sntoa(num), true);
            if(!((neuron_trainer)&&(neuron_trainer->IsNeedToTrain==false)))
            {
                done = false;
                break;
            }
        }
        IsDone = done;
    }


    //Переходим к следующему элементу траектории (обновляем CurrentTE)
    for(int i = 0; i<int(TrajectoryElements.size()); i++)
    {
        UEPtr<NPulseNeuron> neuron = TrajectoryElements[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
        UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
        if(!ltzone)
            return true;

        if(ltzone->OutputFrequency->As<double>(0)>0)
        {
            CurrentTE=i;
            string currentf_name = string(TrajectoryElements[i]->GetName());// имя ЭТ

//            //обновляем LastUsedPath (CurrentForward)
//            for(int j = 0; j<int(BaseTE->Paths.size()); j++)
//            {
//                string name = string(BaseTE->Paths[j]->GetLongName(this));// имя синапса
//                //if (name==currentf_name)
//                int check_comparison = name.compare(0, currentf_name.length(), currentf_name);
//                if (name.compare(0, currentf_name.length(), currentf_name)==0)
//                {
//                    BaseTE->LastUsedPath = j;
//                    break;
//                }
//            }
        }
    }
    PrevTE = BaseTE;

    return true;
}



//int NMazeMemory::CheckActiveNTs() //NT!
//{

//    int active_index = -1;
//    NameT activeNT_name = "";
//    for(int i = 0; i < (NTrainers.size()); i++)
//    {
//        UEPtr<NPulseNeuron> neuron = NTrainers[i]->GetComponentL<NPulseNeuron>("Neuron", true);
//        UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
//        if(!ltzone)
//          return -2;

//        if(ltzone->OutputFrequency->As<double>(0) >0)
//        {
//            activeNT_name = NTrainers[i]->GetName();
//            int num = int(activeNT_name[13]-'0');
//            NameT TE_name = "TrajectoryElement"+sntoa(num);

//            for(int j=0; j < int(TrajectoryElements.size()); j++)
//            {
//                if(TrajectoryElements[j]->GetName() == TE_name)
//                {
//                    active_index = j;
//                    ActivePIs.push_back(TrajectoryElements[i]);
//                    break;
//                }
//            }
//        }
//    }
//    return active_index; //индекс активного элемента в массиве TEs
//}





//vector<UEPtr<NPulseSynapse>> NMazeMemory::GetAllForwardSyns(UEPtr<NTrajectoryElement> traj_el)
//{

//  bool res(true);
//  vector<UEPtr<NPulseSynapse>> forward_syns;

//  // N1_D1_5(Exc)
//  UEPtr<NPulseNeuron> neuron1 = traj_el->GetComponentL<NPulseNeuron>("Neuron1", true);
//  UEPtr<NPulseMembrane> dend1_5 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_5", true);
//  int syn_num = dend1_5->NumExcitatorySynapses;
//  for (int i = 1; i <= syn_num; i++)
//  {
//    UEPtr<NPulseSynapse> syn = dend1_5->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
//    if(!syn)
//      return forward_syns;

//    if (syn->Input.IsConnected())
//    {
//      if(syn_num==i)
//      {
//        dend1_5->NumExcitatorySynapses++;
//        dend1_5->Reset();
//        syn_num = dend1_5->NumExcitatorySynapses;
//      }
//      continue; //перейти к следующему синапсу
//    }
//    else
//    {
//      forward_syns.push_back(syn);
//      break;
//    }
//  }

//  // N1_S1(Inh)
//  UEPtr<NPulseMembrane> soma = neuron1->GetComponentL<NPulseMembrane>("Soma1", true);
//  syn_num = soma->NumInhibitorySynapses;
//  for (int i = 1; i <= syn_num; i++)
//  {
//    UEPtr<NPulseSynapse> syn = soma->GetComponentL<NPulseSynapse>("InhSynapse"+sntoa(i),true);
//    if(!syn)
//      return forward_syns;

//    if (syn->Input.IsConnected())
//    {
//      if(syn_num==i)
//      {
//        soma->NumInhibitorySynapses++;
//        soma->Reset();
//        syn_num = soma->NumInhibitorySynapses;
//      }
//      continue; //перейти к следующему синапсу
//    }
//    else
//    {
//      forward_syns.push_back(syn);
//      break;
//    }
//  }

//  // N2_S1(Inh)
//  UEPtr<NPulseNeuron> neuron2 = traj_el->GetComponentL<NPulseNeuron>("Neuron2", true);
//  soma = neuron2->GetComponentL<NPulseMembrane>("Soma1", true);
//  syn_num = soma->NumInhibitorySynapses;
//  for (int i = 1; i <= syn_num; i++)
//  {
//    UEPtr<NPulseSynapse> syn = soma->GetComponentL<NPulseSynapse>("InhSynapse"+sntoa(i),true);
//    if(!syn)
//      return forward_syns;

//    if (syn->Input.IsConnected())
//    {
//      if(syn_num==i)
//      {
//        soma->NumInhibitorySynapses++;
//        soma->Reset();
//        syn_num = soma->NumInhibitorySynapses;
//      }
//      continue; //перейти к следующему синапсу
//    }
//    else
//    {
//      forward_syns.push_back(syn);
//      break;
//    }
//  }

//  return forward_syns;
//}





//vector<UEPtr<NPulseSynapse>> NMazeMemory::GetAllBackwardSyns(UEPtr<NTrajectoryElement> traj_el)
//{
//    bool res(true);
//    vector<UEPtr<NPulseSynapse>> backward_syns;

//    // N1_D1_2(Exc)
//    UEPtr<NPulseNeuron> neuron1 = traj_el->GetComponentL<NPulseNeuron>("Neuron1", true);
//    UEPtr<NPulseMembrane> dend1_2 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_2", true);
//    int syn_num = dend1_2->NumExcitatorySynapses;
//    for (int i = 1; i <= syn_num; i++)
//    {
//        UEPtr<NPulseSynapse> syn = dend1_2->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
//        if(!syn)
//            return backward_syns;

//        if (syn->Input.IsConnected())
//        {
//            if(syn_num==i)
//            {
//                dend1_2->NumExcitatorySynapses++;
//                dend1_2->Reset();
//                syn_num = dend1_2->NumExcitatorySynapses;
//            }
//            continue; //перейти к следующему синапсу
//        }
//        else
//        {
//            syn->Weight = 0;//0.2
//            backward_syns.push_back(syn);
//            break;
//        }
//    }

//    // N1_S1(Exc)
//    UEPtr<NPulseMembrane> soma = neuron1->GetComponentL<NPulseMembrane>("Soma1", true);
//    syn_num = soma->NumExcitatorySynapses;
//    for (int i = 1; i <= syn_num; i++)
//    {
//        UEPtr<NPulseSynapse> syn = soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
//        if(!syn)
//            return backward_syns;

//        if (syn->Input.IsConnected())
//        {
//            if(syn_num==i)
//            {
//                soma->NumExcitatorySynapses++;
//                soma->Reset();
//                syn_num = soma->NumExcitatorySynapses;
//            }
//            continue; //перейти к следующему синапсу
//        }
//        else
//        {
//            syn->Weight = 0;//0.2
//            backward_syns.push_back(syn);
//            break;
//        }
//    }

//    return backward_syns;
//}






//bool NMazeMemory::UpdateLastUsed(UEPtr<NTrajectoryElement> prev_te, UEPtr<NTrajectoryElement> base_te)
//{
//   string base_name = string(base_te->GetName());
//   //bool found_flag = false;

//   //Ищем в ForwardSyns
//   for (int i = 0; i < int(prev_te->ForwardSyns.size()); i++)
//   {
//     string name = string(prev_te->ForwardSyns[i]->GetLongName(this)).substr(0,19);
//     if (name == base_name)
//     {
//       prev_te->LastUsedForward = i;
//       //found_flag = true;
//       return true;
//     }
//   }

//   //Если не нашли, ищем в BackwardSyns
//   for (int i = 0; i < int(prev_te->BackwardSyns.size()); i = i+2)
//   {
//     string name = string(prev_te->BackwardSyns[i]->GetLongName(this)).substr(0,19);
//     if (name == base_name)
//     {
//       prev_te->LastUsedBackward = i;
//       return true;
//     }
//   }

//   return false;
//}





//bool NMazeMemory::DeadlockProcessing()//ТРЕБУЕТ ОТЛАДКИ
//{
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

//    return true;
//}

}

#endif
