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

// ������
// --------------------------
// ������������ � �����������
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
// ������ ������� � ��������� ����������
// --------------------------
// --------------------------

// --------------------------
// ������ ��������� �����������
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
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<UContainer> neuron_trainer_weak = GetComponentL("NNeuronTrainer"+sntoa(j), true);
    std::shared_ptr<NNeuronTrainer> neuron_trainer;
    if(!neuron_trainer_weak.expired())
     neuron_trainer = std::dynamic_pointer_cast<NNeuronTrainer>(neuron_trainer_weak.lock());
    else
     neuron_trainer = nullptr;
     if (neuron_trainer)
     {
         neuron_trainer->NumInputDendrite = value; //������ ��������� � InputPattern
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
    for (int cnt = 0; cnt<int(NTrainers.size()); cnt++)
    {
        //std::shared_ptr<NNeuronTrainer> neuron_trainer = GetComponentL<NNeuronTrainer>("NeuronTrainer"+sntoa(cnt), true);
        std::shared_ptr<NNeuronTrainer> neuron_trainer = NTrainers[cnt];
        if (neuron_trainer)
        {
            neuron_trainer->NumInputDendrite = value.GetRows(); //������ ��������� � InputPattern
            neuron_trainer->Reset();
            neuron_trainer->InputPattern = value;
        }
    }

 return true;
}

// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
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
// ������� ������ ���������� ������������
// --------------------------
// ��������� ����������� ���������������� ��������
// ��� ���������� ��������� ���������� � ���� ������
// ����� ����� ������ ������ ���� comp ���
// ������� �������� � ������ ���������
bool NMazeMemory::AAddComponent(std::shared_ptr<UContainer> comp, std::shared_ptr<UIPointer> pointer)
{

 return true;
}

// ��������� ��������������� ���������������� ��������
// ��� �������� ��������� ���������� �� ����� �������
// ����� ����� ������ ������ ���� comp
// ���������� � ������ ���������
bool NMazeMemory::ADelComponent(std::shared_ptr<UContainer> comp)
{

 return true;
}
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------


// ����� �������� �����.
bool NMazeMemory::AReset(void)
{


 return true;
}

// �������������� �������� �� ��������� � ����� �������� �����
bool NMazeMemory::ADefault(void)
{
 Situation = false;
 InputActions.assign(3, 0); //(size,val)
 ActionNeuronsType = "NSPNeuronGen";
 FeaturesNum = 4; //x,y,alpha + ������������� �������� 0.2
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
 return true;
}


// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NMazeMemory::ABuild(void)
{
   if (TrajectoryElements.size()<1)
   {
      int x_base = 5;
      int x_shift = 7;

      //��������� ������� ��������
      ActionNeurons.resize(InputActions.size());
      for (int n=0; n<ActionNeurons.size(); n++)
      {
        ActionNeurons[n] = AddMissingComponent<NPulseNeuron>(std::string("ActionNeuron")+sntoa(n+1), ActionNeuronsType);
        if(!ActionNeurons[n])
         return true;
        ActionNeurons[n]->SetCoord(MVector<double,3>(x_base+n*x_shift, 2.0, 0));
      }

      //��������� �������� ������� ����������
      MVector<double,3> root_coords = {5.0, 5.0, 0};
      std::shared_ptr<NTrajectoryElement> root = CreatePoint(root_coords);
      BaseTE = root;
      //PassedTEs.push_back(root);
   }

 return true;
}


// ��������� ������ ����� �������
bool NMazeMemory::ACalculate(void)
{
  if (IsWaitingForAnswer)
    {
        //����� ����� ������ ���������� �� PreControl �������
        if(WaitForAnswerCnt < 2000)
        {
            WaitForAnswerCnt++;
            return true;
        }

        //���������, ���� �� ����� �� "����"
        bool response = false;
        std::shared_ptr<NTrajectoryElement> responding_TE;

        int max_layer = 0;
        for (int j = 0; j< int(MultiPCs.size()); j++)
        {
            //���������, ���� �� ����� ����� ���� ��� ����
            if (TrajectoryElements[j]->Layer - BaseTE->Layer > 1)
            {
                // CRITICAL: GetComponentL now returns weak_ptr, need to lock
                std::weak_ptr<UContainer> neuron_weak = MultiPCs[j]->GetComponentL("PreControlNeuron1", true);
                std::shared_ptr<NPulseNeuron> neuron;
                if(!neuron_weak.expired())
                 neuron = std::dynamic_pointer_cast<NPulseNeuron>(neuron_weak.lock());
                else
                 neuron = nullptr;
                if(!neuron)
                 continue;
                std::weak_ptr<UContainer> ltzone_weak = neuron->GetComponentL("LTZone", true);
                std::shared_ptr<NPulseLTZoneCommon> ltzone;
                if(!ltzone_weak.expired())
                 ltzone = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzone_weak.lock());
                else
                 ltzone = nullptr;
                if(!ltzone)
                    return true;

                if (ltzone->OutputFrequency->As<double>(0)>0)
                {
                    response = true;
                    if (TrajectoryElements[j]->Layer > max_layer)
                    {
                        max_layer = TrajectoryElements[j]->Layer;
                        responding_TE = TrajectoryElements[j];
                        string check_responding_name = string(responding_TE->GetLongName(GetThisAsSharedContainer()));
                    }
                }
            }
        }

        if (response)//���� ���-�� ������� �� "����"
        {
            //��� ������� �� ����������� �������� - ����������� ����� �� ���������
            responding_TE->Paths[responding_TE->LastUsedPath]->Weight=0.6;
            responding_TE->LastUsedPath++;
            responding_TE->Paths[responding_TE->LastUsedPath]->Weight=1;
        }
        else //���� ��� ������
        {
            //��� ������� � ������ w  = 0.2
            //int check_path_n = BaseTE->LastUsedPath;
            string check_lu_paths = string(BaseTE->Paths[BaseTE->LastUsedPath]->GetLongName(GetThisAsSharedContainer()));

            BaseTE->Paths[BaseTE->LastUsedPath]->Weight=0.2;
            BaseTE->LastUsedPath++;
            //��� ��������� ����� w = 1,
            BaseTE->Paths[BaseTE->LastUsedPath]->Weight=1;
        }

        //������� ����������� ���� (TE+MultiPC)
        DelComponent(TEToDelete,true);
        DelComponent(MPCToDelete, true);


//        //��� ������� check
//        for (int cnt = 0; cnt< TrajectoryElements.size(); cnt++)
//        {
//            string te = string(TrajectoryElements[cnt]->GetName());
//        }
//        for (int cnt = 0; cnt< MultiPCs.size(); cnt++)
//        {
//            string mpc = string(MultiPCs[cnt]->GetName());
//        }


        //��������� �����, ������� ������� ��� "�����"
        NameT fin = BaseMPC->GetName()+".PreControlNeuron1.Soma1.ExcSynapse1";
        bool res(true);
        res&=BreakLink(BaseTE->GetLongName(GetThisAsSharedContainer()),"Output", fin,"Input");
        if(!res)
            return true;

        //���������� ����� � NeuronTrainer
        string name = string(BaseTE->GetName());
        int num = int(name[18]-'0');
        string check_name_nt = "NeuronTrainer"+sntoa(num);

        // CRITICAL: GetComponentL now returns weak_ptr, need to lock
        std::weak_ptr<UContainer> CurrentNT_weak = GetComponentL("NeuronTrainer"+sntoa(num), true);
        if(!CurrentNT_weak.expired())
         CurrentNT = std::dynamic_pointer_cast<NNeuronTrainer>(CurrentNT_weak.lock());
        else
         CurrentNT = nullptr;
        NameT post_input = BaseMPC->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
        res&=CreateLink(CurrentNT->GetLongName(GetThisAsSharedContainer()),"Output", post_input,"Input");
        if(!res)
            return true;

        //�������� �����
        //Situation = false;
        IsWaitingForAnswer = false;
        WaitForAnswerCnt = 0;
        return true;
    }

  if(IsNotFinished)
  {
    //����, ���� �������� NeuronTrainer + ��� �������, ����� ������� � ���������� TE
    if (CurrentNT->IsNeedToTrain==true)
       return true;
    else
    {
      WaitForSpike++;
      if(WaitForSpike < 2000)
       return true;
    }

    WaitForSpike = 0;

    //���������, ���� �� �������� ����������� (����� �������� ������)
    //bool check_activeForwards = CheckActiveForwards(BaseTE);
    if(!CheckActiveForwards(BaseTE)) //���� ��� �������� ����������� ������
    {
        //w �������� ����� �� ��(i-1) = 1
        int num = int(BaseTE->Paths.size())-1;
        string check_BaseTE_n = BaseTE->GetName();
        string check_Path_n = BaseTE->Paths[num]->GetLongName(GetThisAsSharedContainer());
        BaseTE->Paths[num]->Weight = 1; //����� ������ �� D1_5 - �.�. ������, ���������

        //w ���� ������ �� ���� ��(i) = 0.2
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock

        std::weak_ptr<UContainer> neuron1_weak = BaseTE->GetComponentL("Neuron1", true);

        std::shared_ptr<NPulseNeuron> neuron1;

        if(!neuron1_weak.expired())

         neuron1 = std::dynamic_pointer_cast<NPulseNeuron>(neuron1_weak.lock());

        else

         neuron1 = nullptr;
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock
        std::weak_ptr<UContainer> dend1_5_weak = neuron1->GetComponentL("Dendrite1_5",true);
        std::shared_ptr<NPulseMembrane> dend1_5;
        if(!dend1_5_weak.expired())
         dend1_5 = std::dynamic_pointer_cast<NPulseMembrane>(dend1_5_weak.lock());
        else
         dend1_5 = nullptr;
        int max = dend1_5->NumExcitatorySynapses;
        for (int i = 0; i<max; i++)
        {
            // CRITICAL: GetComponentL now returns weak_ptr, need to lock
            std::weak_ptr<UContainer> synapse_weak = dend1_5->GetComponentL("ExcSynapse"+sntoa(i+1),true);
            std::shared_ptr<NPulseSynapse> synapse;
            if(!synapse_weak.expired())
             synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak.lock());
            else
             synapse = nullptr;
            string check_syn_n = synapse->GetLongName(GetThisAsSharedContainer());
            synapse->Weight = 0.2;
        }

        //� ���� �� �� ����� MultiPC �������? - ������� ���������� ���������
        bool done = true;
        for(int i = 0; i<int(TrajectoryElements.size()); i++)
        {
            string name = string(TrajectoryElements[i]->GetName());
            int num = int(name[18]-'0');
            string check_name_nt = "NeuronTrainer"+sntoa(num);

            // CRITICAL: GetComponentL now returns weak_ptr, need to lock
            std::weak_ptr<UContainer> neuron_trainer_weak = GetComponentL("NeuronTrainer"+sntoa(num), true);
            std::shared_ptr<NNeuronTrainer> neuron_trainer;
            if(!neuron_trainer_weak.expired())
             neuron_trainer = std::dynamic_pointer_cast<NNeuronTrainer>(neuron_trainer_weak.lock());
            else
             neuron_trainer = nullptr;
            if(!((neuron_trainer)&&(neuron_trainer->IsNeedToTrain==false)))
            {
                done = false;
                break;
            }
        }
        IsDone = done;
    }


    //��������� � ���������� �������� ���������� (��������� CurrentTE)
    for(int i = 0; i<int(TrajectoryElements.size()); i++)
    {
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock

        std::weak_ptr<UContainer> neuron_weak = TrajectoryElements[i]->GetComponentL("Neuron1", true);

        std::shared_ptr<NPulseNeuron> neuron;

        if(!neuron_weak.expired())

         neuron = std::dynamic_pointer_cast<NPulseNeuron>(neuron_weak.lock());

        else

         neuron = nullptr;
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock

        std::weak_ptr<UContainer> ltzone_weak = neuron->GetComponentL("LTZone", true);

        std::shared_ptr<NPulseLTZoneCommon> ltzone;

        if(!ltzone_weak.expired())

         ltzone = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzone_weak.lock());

        else

         ltzone = nullptr;
        if(!ltzone)
            return true;

        if(ltzone->OutputFrequency->As<double>(0)>0)
        {
            CurrentTE=i;
            string currentf_name = string(TrajectoryElements[i]->GetName());// ��� ��

            //��������� LastUsedPath (CurrentForward)
            for(int j = 0; j<int(BaseTE->Paths.size()); j++)
            {
                string name = string(BaseTE->Paths[j]->GetLongName(GetThisAsSharedContainer()));// ��� �������
                //if (name==currentf_name)
                //int check_comparison = name.compare(0, currentf_name.length(), currentf_name);
                if (name.compare(0, currentf_name.length(), currentf_name)==0)
                {
                    BaseTE->LastUsedPath = j;
                    break;
                }
            }
        }
    }
    PrevTE = BaseTE;
    IsNotFinished = false;
  }

  if (Situation)//������������ ��������
  {    
    if (WaitForSpikePI==0)//��������� ����������,������ ���� ������ �� ����������� �����
    {
      //Situation = false;
      UpdateCurrentTE();
      BaseTE = TrajectoryElements[CurrentTE];
      BaseMPC = MultiPCs[CurrentTE];
      PassedTEs.push_back(BaseTE);


      if (int(PassedTEs.size()-2) >0) //-2 - �.� ���������� � 0 � �.�. ����� ���������� ����
      {
        PrevTE = PassedTEs[int(PassedTEs.size()-2)];
         string check_prevTE = string(PrevTE->GetName());
      }


      //���� ��� �������
      string check_baseTE = string(BaseTE->GetName());
      string check_baseMPC = string(BaseMPC->GetName());
      std::vector<string> check_passed;

      for (int j=0; j<int(PassedTEs.size()); j++)
      {
        check_passed.push_back(string(PassedTEs[j]->GetName()));
      }
    }

    //���������, ���� �� �������� PostInput ������� � ���� -
    //�.�.��������� �� ������� �������� � �����-���� �� ��� ���������
    if (WaitForSpikePI < 2000)
    {
        WaitForSpikePI++;
        return true;
    }
    WaitForSpikePI = 0;
    ActivePIs.clear();
    int active_index = CheckActivePIs(); //������� ����� �������� � ActivePIs
    if(ActivePIs.size()>0)//���� ���������� ������ �������� ��, ������ ��������,
    {
      MergingTEs(active_index); //������� �������� �� � ����
    }


   //���������, ���� �� ��� � ���� ����� (���� � �����, ���������=>���� ��������������� NeuronTrainer)
   string name = string(BaseTE->GetName());
   int num = int(name[18]-'0');
   string check_name_nt = "NeuronTrainer"+sntoa(num);
   // CRITICAL: GetComponentL now returns weak_ptr, need to lock
   std::weak_ptr<UContainer> CurrentNT_weak2 = GetComponentL("NeuronTrainer"+sntoa(num), true);
   if(!CurrentNT_weak2.expired())
    CurrentNT = std::dynamic_pointer_cast<NNeuronTrainer>(CurrentNT_weak2.lock());
   else
    CurrentNT = nullptr;

   if (!CurrentNT)//���� ��� �� ����
   {
       int OptionsNum = 0; //����� ��������� �������� � ������� �������� = ����� ��������� ��������� � ������� InputActions
       for (int j = 0; j < InputActions.size(); j++)//���������� ����� ��������� �������� �� �������� �������
       {
          if (InputActions[j]>0)
              OptionsNum++;
       }

       if (OptionsNum <1) // ���� ��� ��������� ����������� ��������
       {
         if (PassedTEs.size()>1)
         {
           //w �������� ����� �� ��(i-1) = 1,
           int num = int(BaseTE->Paths.size())-1;
           BaseTE->Paths[num]->Weight = 1;

           //w �������� ��(i) = 0,2
           num = PrevTE->LastUsedPath;
           PrevTE->Paths[num]->Weight = 0.2;

           //������ "���������" �� �� ����� ���������� �����
           PassedTEs.pop_back();
         }
         else
         {
            IsDone = true;
            return true;
         }

       }
       else // ���� ���� ��������� �����������
       {
         std::shared_ptr<NTrajectoryElement> traj_el;
         MVector<double,3> base_coords = BaseTE->GetCoord();
         CurrentLayer++;
         LinkPoint(traj_el, base_coords, OptionsNum);//������� TE+MultiPC � ����������� �����
       }

       //���������� ������� �������� (��������� �������� ���������)
       string name = string(BaseTE->GetName());
       int num = int(name[18]-'0');
       string check_name_nt = "NeuronTrainer"+sntoa(num);

       std::shared_ptr<NNeuronTrainer> neuron_trainer = AddMissingComponent<NNeuronTrainer>(std::string("NeuronTrainer"+sntoa(num)), "NNeuronTrainer");
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

       NameT start_name =  neuron_trainer->GetName();
       NameT finish_name = BaseMPC->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
       if(!CheckLink(start_name,finish_name))
           res1 &= CreateLink(start_name,"Output", finish_name, "Input");

       start_name =  BaseMPC->GetName()+".PreControlNeuron1.LTZone";
       finish_name = BaseMPC->GetName()+".ControlNeuron1-1.Soma1.ExcSynapse1";
       if(!CheckLink(start_name,finish_name))
           res2 &= CreateLink(start_name,"Output", finish_name, "Input");

       //������� ��� �������� ����� ��� "�����"
       string check_baseMPC = string(BaseMPC->GetName());
       // CRITICAL: GetComponentL now returns weak_ptr, need to lock

       std::weak_ptr<UContainer> precontrol_n_weak = BaseMPC->GetComponentL("PreControlNeuron1", true);

       std::shared_ptr<NPulseNeuron> precontrol_n;

       if(!precontrol_n_weak.expired())

        precontrol_n = std::dynamic_pointer_cast<NPulseNeuron>(precontrol_n_weak.lock());

       else

        precontrol_n = nullptr;
       // CRITICAL: GetComponentL now returns weak_ptr, need to lock

       std::weak_ptr<UContainer> precontrol_soma_weak = precontrol_n->GetComponentL("Soma1", true);

       std::shared_ptr<NPulseMembrane> precontrol_soma;

       if(!precontrol_soma_weak.expired())

        precontrol_soma = std::dynamic_pointer_cast<NPulseMembrane>(precontrol_soma_weak.lock());

       else

        precontrol_soma = nullptr;
       for(int i = 0; i < precontrol_soma->NumExcitatorySynapses(); i++)
       {
           // CRITICAL: GetComponentL now returns weak_ptr, need to lock
           std::weak_ptr<UContainer> pc_synapse_weak = precontrol_soma->GetComponentL("ExcSynapse"+sntoa(i+1),true);
           std::shared_ptr<NPulseSynapse> pc_synapse;
           if(!pc_synapse_weak.expired())
            pc_synapse = std::dynamic_pointer_cast<NPulseSynapse>(pc_synapse_weak.lock());
           else
            pc_synapse = nullptr;
           string check_pc_synapse = string(pc_synapse->GetLongName(GetThisAsSharedContainer()));
           if (pc_synapse)
               pc_synapse->DisconnectAllItems();
       }


     }
   else //���� ��� ���� � ���� �����
   {
     PassedTEs.clear();

     string check_base_name = string(BaseTE->GetName());
     //int check_base_layer = BaseTE->Layer;
     string check_prev_name = string(PrevTE->GetName());
     //int check_prev_layer = PrevTE->Layer;

     if (BaseTE->Layer <= PrevTE->Layer)//���� �� ���������� �� ������� ���� �� ��������� � ���������� �� (�.�. �� ������������ ������ � ������������ ���������)
     {
        //��� �����, �� ������� ������ � ��� �����, w = 0,2
        PrevTE->Paths[PrevTE->LastUsedPath]->Weight = 0.2;

        //int check_lastusedpath = PrevTE->LastUsedPath;
        string check_paths = string(PrevTE->Paths[PrevTE->LastUsedPath]->GetLongName(GetThisAsSharedContainer()));

        //������ "����"
        NameT fin = BaseMPC->GetName()+".PreControlNeuron1.Soma1.ExcSynapse1";
        string check_fin = string(fin);
        bool res(true);
        res&=CreateLink(BaseTE->GetLongName(GetThisAsSharedContainer()),"Output", fin,"Input");
        if(!res)
          return true;

        //��� �������
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock

        std::weak_ptr<UContainer> check_n1_weak = BaseTE->GetComponentL("Neuron1", true);

        std::shared_ptr<NPulseNeuron> check_n1;

        if(!check_n1_weak.expired())

         check_n1 = std::dynamic_pointer_cast<NPulseNeuron>(check_n1_weak.lock());

        else

         check_n1 = nullptr;
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock

        std::weak_ptr<UContainer> check_ltz1_weak = check_n1->GetComponentL("LTZone", true);

        std::shared_ptr<NPulseLTZoneCommon> check_ltz1;

        if(!check_ltz1_weak.expired())

         check_ltz1 = std::dynamic_pointer_cast<NPulseLTZoneCommon>(check_ltz1_weak.lock());

        else

         check_ltz1 = nullptr;
        double check_base_activity = check_ltz1->OutputFrequency->As<double>(0);
        if (check_base_activity>0)
        {}

        IsWaitingForAnswer = true;
        //return true;
      }
    } //����� "���� ��� ���� � ���� �����"

    Situation = false;

  }//����� if (Situation)

  return true;
}
// --------------------------



std::shared_ptr<NTrajectoryElement> NMazeMemory::CreatePoint(MVector<double,3> coords)
  {
   std::shared_ptr<UContainer> cont;
   std::shared_ptr<UStorage> storage = GetStorage();
   if (!storage) {
     return nullptr;
   }
   std::shared_ptr<NTrajectoryElement> traj_el;



   int te_num = int(TrajectoryElements.size());
   int mpc_num = int(MultiPCs.size());

   if (TrajectoryElements.size()>0)
   {
       string check1 = TrajectoryElements[int(TrajectoryElements.size())-1]->GetName();
       //char check_te_num = string(TrajectoryElements[int(TrajectoryElements.size())-1]->GetName())[18];

       string name = string(TrajectoryElements[int(TrajectoryElements.size())-1]->GetName());
       te_num = int(name[18]-'0')+1;
   }
   if (MultiPCs.size()>0)
   {
       string check2 = MultiPCs[int(MultiPCs.size())-1]->GetName();
       //char check_mpc_num = string(MultiPCs[int(MultiPCs.size())-1]->GetName())[21];

       string name = string(MultiPCs[int(MultiPCs.size())-1]->GetName());
       mpc_num = int(name[21]-'0')+1;
   }

   //��������� ������� ����������
   traj_el = AddMissingComponent<NTrajectoryElement>(std::string("NTrajectoryElement"+sntoa(te_num)), "NTrajectoryElement");
   traj_el->SetCoord(coords);
   TrajectoryElements.push_back(traj_el);

   //��������� ���� MultiPC
   string MultiPCName = "NMultiPositionControl"+sntoa(mpc_num);
   cont=dynamic_pointer_cast<UContainer>(storage->TakeObject("NMultiPositionControl"));
   if(!cont)
     return 0;
   cont->SetName(MultiPCName);
   cont->SetCoord(MVector<double,3>(coords[0], coords[1]+2.0, 0.0));
   AddComponent(cont);

   //��������� ����� MultiPC
   std::shared_ptr<NMultiPositionControl> multi_pc = dynamic_pointer_cast<NMultiPositionControl>(cont);
   MultiPCs.push_back(multi_pc);
   multi_pc->BuildSolo = true;
   multi_pc->ExternalControl = false;
   multi_pc->IsNeedToRebuild = true;
   multi_pc->InputsNum = 1;//����� InputNeurons, ���� InputNeuronsType = NSPNeuronGen (�.�. �������� ��������������� ������� �������� ���������)
   multi_pc->PrebuildStructure = true;
   multi_pc->Reset();
   string check_multipc = multi_pc->GetLongName(GetThisAsSharedContainer());

   //���������� ������ �� PostInputNeuron � TrajectoryElement
   bool res(true);

   // CRITICAL: GetComponentL now returns weak_ptr, need to lock


   std::weak_ptr<UContainer> postinput_weak = multi_pc->GetComponentL("PostInputNeuron1", true);


   std::shared_ptr<NPulseNeuron> postinput;


   if(!postinput_weak.expired())


    postinput = std::dynamic_pointer_cast<NPulseNeuron>(postinput_weak.lock());


   else


    postinput = nullptr;
   string check_postinput = postinput->GetLongName(GetThisAsSharedContainer());
   // CRITICAL: GetComponentL now returns weak_ptr, need to lock

   std::weak_ptr<UContainer> ltzone_weak = postinput->GetComponentL("LTZone", true);

   std::shared_ptr<NPulseLTZoneCommon> ltzone;

   if(!ltzone_weak.expired())

    ltzone = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzone_weak.lock());

   else

    ltzone = nullptr;
   string check_ltzone = ltzone->GetLongName(GetThisAsSharedContainer());

  //       if(!ltzone)
  //           return true;

   // CRITICAL: GetComponentL now returns weak_ptr, need to lock


   std::weak_ptr<UContainer> neuron1_weak = traj_el->GetComponentL("Neuron1", true);


   std::shared_ptr<NPulseNeuron> neuron1;


   if(!neuron1_weak.expired())


    neuron1 = std::dynamic_pointer_cast<NPulseNeuron>(neuron1_weak.lock());


   else


    neuron1 = nullptr;
   // CRITICAL: GetComponentL now returns weak_ptr, need to lock

   std::weak_ptr<UContainer> synapse_weak = neuron1->GetComponentL("Dendrite1_1.InhSynapse1",true);

   std::shared_ptr<NPulseSynapse> synapse;

   if(!synapse_weak.expired())

    synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak.lock());

   else

    synapse = nullptr;
   res&=CreateLink(ltzone->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
   //      if(!res)
   //       return true;

   // CRITICAL: GetComponentL now returns weak_ptr, need to lock


   std::weak_ptr<UContainer> synapse_weak_d13 = neuron1->GetComponentL("Dendrite1_3.InhSynapse1",true);


   if(!synapse_weak_d13.expired())


     synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak_d13.lock());


   else


    synapse = nullptr;
   res&=CreateLink(ltzone->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
   //      if(!res)
   //       return true;

   // CRITICAL: GetComponentL now returns weak_ptr, need to lock


   std::weak_ptr<UContainer> neuron2_weak = traj_el->GetComponentL("Neuron2", true);


   std::shared_ptr<NPulseNeuron> neuron2;


   if(!neuron2_weak.expired())


    neuron2 = std::dynamic_pointer_cast<NPulseNeuron>(neuron2_weak.lock());


   else


    neuron2 = nullptr;
   // CRITICAL: GetComponentL now returns weak_ptr, need to lock
   std::weak_ptr<UContainer> synapse_weak2 = neuron2->GetComponentL("Dendrite1_1.InhSynapse1",true);
   if(!synapse_weak2.expired())
    synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak2.lock());
   else
    synapse = nullptr;
   res&=CreateLink(ltzone->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
   //      if(!res)
   //       return true;

   // CRITICAL: GetComponentL now returns weak_ptr, need to lock
   std::weak_ptr<UContainer> synapse_weak3 = neuron2->GetComponentL("Dendrite1_3.InhSynapse1",true);
   if(!synapse_weak3.expired())
    synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak3.lock());
   else
    synapse = nullptr;
   res&=CreateLink(ltzone->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
   //      if(!res)
   //       return true;

   return traj_el;
  }


 bool NMazeMemory:: LinkPoint(std::shared_ptr<NTrajectoryElement> traj_el,  MVector<double,3> base_coords, int options_num)
 {
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock

     std::weak_ptr<UContainer> base_neuron_weak = TrajectoryElements[CurrentTE]->GetComponentL("Neuron1", true);

     std::shared_ptr<NPulseNeuron> base_neuron;

     if(!base_neuron_weak.expired())

      base_neuron = std::dynamic_pointer_cast<NPulseNeuron>(base_neuron_weak.lock());

     else

      base_neuron = nullptr;
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock

     std::weak_ptr<UContainer> ltzone_te_weak = base_neuron->GetComponentL("LTZone", true);

     std::shared_ptr<NLTZone> ltzone_te;

     if(!ltzone_te_weak.expired())

      ltzone_te = std::dynamic_pointer_cast<NLTZone>(ltzone_te_weak.lock());

     else

      ltzone_te = nullptr;
     if(!ltzone_te)
         return true;

     //��������� ������� �� ������� �� ��� �������� ������ �� ����
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock

     std::weak_ptr<UContainer> base_soma_weak = base_neuron->GetComponentL("Soma1", true);

     std::shared_ptr<NPulseMembrane> base_soma;

     if(!base_soma_weak.expired())

      base_soma = std::dynamic_pointer_cast<NPulseMembrane>(base_soma_weak.lock());

     else

      base_soma = nullptr;
     if (CurrentTE==0)
         base_soma->NumExcitatorySynapses = options_num+1; //�������������� ������ �� ������ �� - ��� ������������ ������� � ������� ������ ��
     else
         base_soma->NumExcitatorySynapses = options_num;
     base_soma->Reset();
     //�� N1_D1_2
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock

     std::weak_ptr<UContainer> base_dend_weak = base_neuron->GetComponentL("Dendrite1_2", true);

     std::shared_ptr<NPulseMembrane> base_dend;

     if(!base_dend_weak.expired())

      base_dend = std::dynamic_pointer_cast<NPulseMembrane>(base_dend_weak.lock());

     else

      base_dend = nullptr;
     base_dend->NumExcitatorySynapses = options_num;
     base_dend->Reset();

     int possible_action_num = 0; // ����� ���������� �������� = 0..OptionsNum
     for(int input_action = 0; input_action<int(InputActions.size()); input_action++)//����� �������� �� ������� ������� �������� = 0...InputActions.size()
     {
         if (InputActions[input_action]==0)
             continue;

         possible_action_num++;

     double x = (double)CurrentLayer*LayerShift + base_coords[0];
     double y = (double)(possible_action_num-1)*yShift + base_coords[1];
     MVector<double,3> coords = {x, y, 0.0};
     traj_el = CreatePoint(coords);
     traj_el->Reset();
     BaseTE->Forwards.push_back(traj_el);
     traj_el->Layer = CurrentLayer;

     //������ ����� �� �������� �������� ���������� �� ������ ��� ���������
     bool res(true);
     //������������ ����������� �� N1_D5_syn1
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock

     std::weak_ptr<UContainer> input_neuron_weak = traj_el->GetComponentL("Neuron1", true);

     std::shared_ptr<NPulseNeuron> input_neuron;

     if(!input_neuron_weak.expired())

      input_neuron = std::dynamic_pointer_cast<NPulseNeuron>(input_neuron_weak.lock());

     else

      input_neuron = nullptr;
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock

     std::weak_ptr<UContainer> synapse_weak = input_neuron->GetComponentL("Dendrite1_5.ExcSynapse1",true);

     std::shared_ptr<NPulseSynapse> synapse;

     if(!synapse_weak.expired())

      synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak.lock());

     else

      synapse = nullptr;
     if (possible_action_num>1)
     {
         synapse->Weight = SideWeight;
         string check_sn = synapse->GetLongName(GetThisAsSharedContainer());
     }
     else
     {
         //traj_el->CurrentForward = 0;
         traj_el->LastUsedPath = 0;

     }
     res&=CreateLink(BaseTE->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
     if(!res)
         return true;
     BaseTE->Paths.push_back(synapse);

     //��������� ����������� �� N1_S1_syn1
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock

     std::weak_ptr<UContainer> synapse_weak_n1_s1 = input_neuron->GetComponentL("Soma1.InhSynapse1",true);

     if(!synapse_weak_n1_s1.expired())

      synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak_n1_s1.lock());

     else

      synapse = nullptr;
     res&=CreateLink(BaseTE->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
     if(!res)
         return true;
     //��������� ����������� �� N2_S1_syn1
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock

     std::weak_ptr<UContainer> input_neuron_weak_n2 = traj_el->GetComponentL("Neuron2", true);

     if(!input_neuron_weak_n2.expired())

      input_neuron = std::dynamic_pointer_cast<NPulseNeuron>(input_neuron_weak_n2.lock());

     else

      input_neuron = nullptr;
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock

     std::weak_ptr<UContainer> synapse_weak_n2_s1 = input_neuron->GetComponentL("Soma1.InhSynapse1",true);

     if(!synapse_weak_n2_s1.expired())

      synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak_n2_s1.lock());

     else

      synapse = nullptr;
     res&=CreateLink(BaseTE->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
     if(!res)
         return true;

     //�������� �����
     //int check_Backwards = traj_el->Backwards.size();
     traj_el->Backwards.push_back(BaseTE);
     //traj_el->CurrentBackward = 0;

     //�� ����
     int syn_num;
     if (CurrentTE==0)
         syn_num = possible_action_num+1;
     else
         syn_num = possible_action_num;
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock
     std::weak_ptr<UContainer> synapse_weak_soma = base_soma->GetComponentL("ExcSynapse"+sntoa(syn_num),true);
     if(!synapse_weak_soma.expired())
      synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak_soma.lock());
     else
      synapse = nullptr;
     string check_s1 = synapse->GetLongName(GetThisAsSharedContainer());
     synapse->Weight = 0.2;
     res&=CreateLink(traj_el->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
     if(!res)
         return true;

     //�� N1_D1_2
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock
     std::weak_ptr<UContainer> synapse_weak_d12 = base_neuron->GetComponentL("Dendrite1_2.ExcSynapse"+sntoa(possible_action_num),true);
     if(!synapse_weak_d12.expired())
      synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak_d12.lock());
     else
      synapse = nullptr;
     string check_s2 = synapse->GetLongName(GetThisAsSharedContainer());
     synapse->Weight = 0.2;
     res&=CreateLink(traj_el->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
     if(!res)
         return true;
     traj_el->Paths.push_back(synapse);
     //int check_Paths = traj_el->Paths.size();

     //����� ����� PreControl ��������� ��� "�����"
     size_t passed_max = PassedTEs.size();
     for (size_t j=0; j<passed_max; j++)
     {   
         std::shared_ptr<NMultiPositionControl> output_mpc = MultiPCs[j];
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock

         std::weak_ptr<UContainer> output_postinputn_weak = output_mpc->GetComponentL("PreControlNeuron1", true);

         std::shared_ptr<NPulseNeuron> output_postinputn;

         if(!output_postinputn_weak.expired())

          output_postinputn = std::dynamic_pointer_cast<NPulseNeuron>(output_postinputn_weak.lock());

         else

          output_postinputn = nullptr;
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock

         std::weak_ptr<UContainer> output_ltzone_weak = output_postinputn->GetComponentL("LTZone", true);

         std::shared_ptr<NLTZone> output_ltzone;

         if(!output_ltzone_weak.expired())

          output_ltzone = std::dynamic_pointer_cast<NLTZone>(output_ltzone_weak.lock());

         else

          output_ltzone = nullptr;
         if (!output_ltzone)
             return false;

         std::shared_ptr<NMultiPositionControl> input_mpc = MultiPCs[int(MultiPCs.size())-1];//���������
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock

         std::weak_ptr<UContainer> input_precontroln_weak = input_mpc->GetComponentL("PreControlNeuron1", true);

         std::shared_ptr<NPulseNeuron> input_precontroln;

         if(!input_precontroln_weak.expired())

          input_precontroln = std::dynamic_pointer_cast<NPulseNeuron>(input_precontroln_weak.lock());

         else

          input_precontroln = nullptr;
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock

         std::weak_ptr<UContainer> input_soma_weak = input_precontroln->GetComponentL("Soma1", true);

         std::shared_ptr<NPulseMembrane> input_soma;

         if(!input_soma_weak.expired())

          input_soma = std::dynamic_pointer_cast<NPulseMembrane>(input_soma_weak.lock());

         else

          input_soma = nullptr;
         if (!input_soma)
             return false;


         //���� ��� �������
         string check_outp_mpc = string(output_mpc->GetName());
         string check_output_postinputn = string(output_mpc->GetLongName(GetThisAsSharedContainer()));
         string check_output_ltzone = string(output_ltzone->GetLongName(GetThisAsSharedContainer()));

         string check_inp_mpc = string(input_mpc->GetName());
         string check_input_precontroln = string(input_precontroln->GetLongName(GetThisAsSharedContainer()));
         string check_input_soma = string(input_soma->GetLongName(GetThisAsSharedContainer()));



         int syn_num = input_soma->NumExcitatorySynapses;
         for (int i = 1; i<=syn_num; i++)
         {
             // CRITICAL: GetComponentL now returns weak_ptr, need to lock
             std::weak_ptr<UContainer> syn_weak_precontrol = input_precontroln->GetComponentL("Soma1.ExcSynapse"+sntoa(i),true);
             std::shared_ptr<NPulseSynapse> syn;
             if(!syn_weak_precontrol.expired())
              syn = std::dynamic_pointer_cast<NPulseSynapse>(syn_weak_precontrol.lock());
             else
              syn = nullptr;
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
                 continue; //������� � ���������� �������
             }
             else
             {
                 res&=CreateLink(output_ltzone->GetLongName(GetThisAsSharedContainer()),"Output",syn->GetLongName(GetThisAsSharedContainer()),"Input");
                 if(!res)
                     return true;
                 break;
             }
         }
     }

     //����� �� ������� ��������
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock

     std::weak_ptr<UContainer> action_soma_weak = ActionNeurons[input_action]->GetComponentL("Soma1",true);

     std::shared_ptr<NPulseMembrane> action_soma;

     if(!action_soma_weak.expired())

      action_soma = std::dynamic_pointer_cast<NPulseMembrane>(action_soma_weak.lock());

     else

      action_soma = nullptr;
     int syn_max_num = action_soma->NumExcitatorySynapses;
     for (int i = 1; i<=syn_max_num; i++)
     {
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock
         std::weak_ptr<UContainer> action_syn_weak = action_soma->GetComponentL("ExcSynapse"+sntoa(i),true);
         std::shared_ptr<NPulseSynapse> action_syn;
         if(!action_syn_weak.expired())
          action_syn = std::dynamic_pointer_cast<NPulseSynapse>(action_syn_weak.lock());
         else
          action_syn = nullptr;
         string check_act_name = action_soma->GetLongName(GetThisAsSharedContainer());

         if (action_syn->Input.IsConnected())
         {
             if((i+1)>syn_max_num)
             {
                 action_soma->NumExcitatorySynapses++;
                 action_soma->Reset();
                 syn_max_num = action_soma->NumExcitatorySynapses;
             }
             continue; //������� � ���������� �������
         }
         else
         {
             res&=CreateLink(traj_el->GetLongName(GetThisAsSharedContainer()),"Output",action_syn->GetLongName(GetThisAsSharedContainer()),"Input");
             if(!res)
                 return true;
             break;
         }
     }
   }
     return true;
 }


bool NMazeMemory::CheckActiveForwards(std::shared_ptr<NTrajectoryElement> t_element)
{
  int max = int(t_element->Forwards.size());
  for(int i = 0; i<max; i++)
  {
    string checknname = t_element->Forwards[i]->GetName();
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<UContainer> neuron_weak_forwards = t_element->Forwards[i]->GetComponentL("Neuron1", true);
    std::shared_ptr<NPulseNeuron> neuron;
    if(!neuron_weak_forwards.expired())
     neuron = std::dynamic_pointer_cast<NPulseNeuron>(neuron_weak_forwards.lock());
    else
     neuron = nullptr;
    if(!neuron)
        return false;
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock

    std::weak_ptr<UContainer> ltzone_weak = neuron->GetComponentL("LTZone", true);

    std::shared_ptr<NPulseLTZoneCommon> ltzone;

    if(!ltzone_weak.expired())

     ltzone = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzone_weak.lock());

    else

     ltzone = nullptr;
    if(!ltzone)
      return false;

    string check_ltz = ltzone->GetLongName(GetThisAsSharedContainer());
    //double check_frequency = ltzone->OutputFrequency->As<double>(0);
    if(ltzone->OutputFrequency->As<double>(0) >0)
    {
      return true;
    }
  }
  return false;
}


int NMazeMemory::CheckActivePIs()
{

    int active_index = -1;
    for(int i = 0; i<CurrentTE; i++)
    {
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock

        std::weak_ptr<UContainer> neuron_weak = MultiPCs[i]->GetComponentL("PostInputNeuron1", true);

        std::shared_ptr<NPulseNeuron> neuron;

        if(!neuron_weak.expired())

         neuron = std::dynamic_pointer_cast<NPulseNeuron>(neuron_weak.lock());

        else

         neuron = nullptr;
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock

        std::weak_ptr<UContainer> ltzone_weak = neuron->GetComponentL("LTZone", true);

        std::shared_ptr<NPulseLTZoneCommon> ltzone;

        if(!ltzone_weak.expired())

         ltzone = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzone_weak.lock());

        else

         ltzone = nullptr;
        //if(!ltzone)
        //return active_PIs;

        if(ltzone->OutputFrequency->As<double>(0) >0)
        {
            string check_actInp = TrajectoryElements[i]->GetName();
            ActivePIs.push_back(TrajectoryElements[i]);
            active_index = i;
        }
    }
    return active_index; //������ ��������� �������� � ������� TEs
}



bool NMazeMemory::MergingTEs(int active_index)
{
    int k = int(PassedTEs.size())-2;
    if (k<0)
        return true;

    PrevTE = PassedTEs[k];
    string prevTE_name = string(PrevTE->GetName());

    for (int j=0; j<ActivePIs.size(); j++)
    {
      string active_name = string(ActivePIs[j]->GetName());

      //���� ������� � ���������, � ������� ���� �� ������� ����
      if (prevTE_name == active_name)
      {
          //����������� ��� �������� ������ �� ������� TE
          //�� ����
          NameT start_name =  BaseTE->GetName();
          // CRITICAL: GetComponentL now returns weak_ptr, need to lock

          std::weak_ptr<UContainer> prev_neuron_weak = PrevTE->GetComponentL("Neuron1", true);

          std::shared_ptr<NPulseNeuron> prev_neuron;

          if(!prev_neuron_weak.expired())

           prev_neuron = std::dynamic_pointer_cast<NPulseNeuron>(prev_neuron_weak.lock());

          else

           prev_neuron = nullptr;
          // CRITICAL: GetComponentL now returns weak_ptr, need to lock

          std::weak_ptr<UContainer> prev_dend_weak = prev_neuron->GetComponentL("Soma1", true);

          std::shared_ptr<NPulseMembrane> prev_dend;

          if(!prev_dend_weak.expired())

           prev_dend = std::dynamic_pointer_cast<NPulseMembrane>(prev_dend_weak.lock());

          else

           prev_dend = nullptr;
          if(!prev_dend)
              return true;
          for (int i = 1; i<= prev_dend->NumExcitatorySynapses; i++)
          {
              // CRITICAL: GetComponentL now returns weak_ptr, need to lock
              std::weak_ptr<UContainer> syn_weak_prev_dend = prev_dend->GetComponentL("ExcSynapse"+sntoa(i),true);
              std::shared_ptr<NPulseSynapse> syn;
              if(!syn_weak_prev_dend.expired())
               syn = std::dynamic_pointer_cast<NPulseSynapse>(syn_weak_prev_dend.lock());
              else
               syn = nullptr;
              if(!syn)
                  return true;
              NameT finish_name = syn->GetLongName(GetThisAsSharedContainer());
              if(CheckLink(start_name,finish_name))
                  syn->Weight = 1;
          }
          //�� D1_2
          // CRITICAL: GetComponentL now returns weak_ptr, need to lock

          std::weak_ptr<UContainer> prev_dend_weak_d12 = prev_neuron->GetComponentL("Dendrite1_2", true);

          if(!prev_dend_weak_d12.expired())

           prev_dend = std::dynamic_pointer_cast<NPulseMembrane>(prev_dend_weak_d12.lock());

          else

           prev_dend = nullptr;
          if(!prev_dend)
              return true;
          for (int i = 1; i<= prev_dend->NumExcitatorySynapses; i++)
          {
              // CRITICAL: GetComponentL now returns weak_ptr, need to lock
              std::weak_ptr<UContainer> syn_weak_prev_dend = prev_dend->GetComponentL("ExcSynapse"+sntoa(i),true);
              std::shared_ptr<NPulseSynapse> syn;
              if(!syn_weak_prev_dend.expired())
               syn = std::dynamic_pointer_cast<NPulseSynapse>(syn_weak_prev_dend.lock());
              else
               syn = nullptr;
              if(!syn)
                  return true;
              NameT finish_name = syn->GetLongName(GetThisAsSharedContainer());
              if(CheckLink(start_name,finish_name))
                  syn->Weight = 1;
          }

          //�������� ��������� ����������� NeuronTrainer, ����� �� �� �������� ���������� TE
          bool res(true);
          NameT active_PI = MultiPCs[active_index]->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
          string check_actNT = string(active_PI);
          string check_NT = string(CurrentNT->GetLongName(GetThisAsSharedContainer()));
          res&=BreakLink(CurrentNT->GetLongName(GetThisAsSharedContainer()),"Output", active_PI,"Input");
          if(!res)
              return true;


          for(int clc_cnt = 0; clc_cnt <2000; clc_cnt++)
          {
              ActivePIs[j]->Calculate();
          }
          continue;
      }


      //���� ������� � ���������, � ������� ���� ������, ��� �� ������� ����
      //��������� ������� ����� � �������� �� �� ��������
      NameT start = PrevTE->GetLongName(GetThisAsSharedContainer());
      bool res;
      NameT finish;

      //����������� ����������� �� N1_D1_5
      // CRITICAL: GetComponentL now returns weak_ptr, need to lock

      std::weak_ptr<UContainer> fin_neuron_weak = ActivePIs[j]->GetComponentL("Neuron1", true);

      std::shared_ptr<NPulseNeuron> fin_neuron;

      if(!fin_neuron_weak.expired())

       fin_neuron = std::dynamic_pointer_cast<NPulseNeuron>(fin_neuron_weak.lock());

      else

       fin_neuron = nullptr;
      // CRITICAL: GetComponentL now returns weak_ptr, need to lock

      std::weak_ptr<UContainer> fin_dend_weak = fin_neuron->GetComponentL("Dendrite1_5", true);

      std::shared_ptr<NPulseMembrane> fin_dend;

      if(!fin_dend_weak.expired())

       fin_dend = std::dynamic_pointer_cast<NPulseMembrane>(fin_dend_weak.lock());

      else

       fin_dend = nullptr;
      int dend_syns = fin_dend->NumExcitatorySynapses;
      for (int n = 1; n<=dend_syns; n++)
      {
          // CRITICAL: GetComponentL now returns weak_ptr, need to lock
          std::weak_ptr<UContainer> syn_weak3 = fin_dend->GetComponentL("ExcSynapse"+sntoa(n),true);
          std::shared_ptr<NPulseSynapse> syn;
          if(!syn_weak3.expired())
           syn = std::dynamic_pointer_cast<NPulseSynapse>(syn_weak3.lock());
          else
           syn = nullptr;
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
         continue; //������� � ���������� �������
        }
        else
        {
         //finish_name = ActivePIs[j]->GetName()+".Neuron1.Dendrite1_5.ExcSynapse"+sntoa(n);
            finish = syn->GetLongName(GetThisAsSharedContainer());
         res&=CreateLink(start,"Output",finish,"Input");
//         if(!res)
//          return true;
         break;
        }
      }

      //��������� ����������� �� N1_S1
      // CRITICAL: GetComponentL now returns weak_ptr, need to lock

      std::weak_ptr<UContainer> fin_soma_weak = fin_neuron->GetComponentL("Soma1", true);

      std::shared_ptr<NPulseMembrane> fin_soma;

      if(!fin_soma_weak.expired())

       fin_soma = std::dynamic_pointer_cast<NPulseMembrane>(fin_soma_weak.lock());

      else

       fin_soma = nullptr;
      int soma_syn = fin_soma->NumInhibitorySynapses;
      for (int n = 1; n<=soma_syn; n++)
      {
       // CRITICAL: GetComponentL now returns weak_ptr, need to lock
       std::weak_ptr<UContainer> syn_weak_fin_soma = fin_soma->GetComponentL("InhSynapse"+sntoa(n),true);
       std::shared_ptr<NPulseSynapse> syn;
       if(!syn_weak_fin_soma.expired())
        syn = std::dynamic_pointer_cast<NPulseSynapse>(syn_weak_fin_soma.lock());
       else
        syn = nullptr;
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
         continue; //������� � ���������� �������
       }
       else
       {
        //finish = ActivePIs[j]->GetName()+".Neuron1.Soma1.InhSynapse"+sntoa(n);
            finish = syn->GetLongName(GetThisAsSharedContainer());
        res&=CreateLink(start,"Output",finish,"Input");
        //if(!res)
        // return true;
        break;
       }
      }

      //��������� ����������� �� N2_S1
      // CRITICAL: GetComponentL now returns weak_ptr, need to lock
      std::weak_ptr<UContainer> fin_neuron_weak2 = ActivePIs[j]->GetComponentL("Neuron2", true);
      if(!fin_neuron_weak2.expired())
       fin_neuron = std::dynamic_pointer_cast<NPulseNeuron>(fin_neuron_weak2.lock());
      else
       fin_neuron = nullptr;
      if(!fin_neuron)
       continue;
      std::weak_ptr<UContainer> fin_soma_weak2 = fin_neuron->GetComponentL("Soma1", true);
      if(!fin_soma_weak2.expired())
       fin_soma = std::dynamic_pointer_cast<NPulseMembrane>(fin_soma_weak2.lock());
      else
       fin_soma = nullptr;
      soma_syn = fin_soma->NumInhibitorySynapses;
      for (int n = 1; n<=soma_syn; n++)
      {
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock
        std::weak_ptr<UContainer> syn_weak = fin_soma->GetComponentL("InhSynapse"+sntoa(n),true);
        std::shared_ptr<NPulseSynapse> syn;
        if(!syn_weak.expired())
         syn = std::dynamic_pointer_cast<NPulseSynapse>(syn_weak.lock());
        else
         syn = nullptr;
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
          continue; //������� � ���������� �������
        }
        else
        {
            finish = syn->GetLongName(GetThisAsSharedContainer());
          //finish = ActivePIs[j]->GetName()+".Neuron1.Soma1.InhSynapse"+sntoa(n);
          res&=CreateLink(start,"Output",finish,"Input");
          //if(!res)
          //  return true;
          break;
        }
      }

      //��������� �������� ����� � �������� �� �� ��������
      start = ActivePIs[j]->GetLongName(GetThisAsSharedContainer());

      size_t back_max = BaseTE->Backwards.size();
      for (size_t i = 0; i<back_max; i++)
      {
        //�� N1_D1_2
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock
        std::weak_ptr<UContainer> fin_neuron_weak = BaseTE->Backwards[i]->GetComponentL("Neuron1", true);
        std::shared_ptr<NPulseNeuron> fin_neuron;
        if(!fin_neuron_weak.expired())
         fin_neuron = std::dynamic_pointer_cast<NPulseNeuron>(fin_neuron_weak.lock());
        else
         fin_neuron = nullptr;
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock

        std::weak_ptr<UContainer> fin_dend_weak = fin_neuron->GetComponentL("Dendrite1_2", true);

        std::shared_ptr<NPulseMembrane> fin_dend;

        if(!fin_dend_weak.expired())

         fin_dend = std::dynamic_pointer_cast<NPulseMembrane>(fin_dend_weak.lock());

        else

         fin_dend = nullptr;
        int dend_syns = fin_dend->NumExcitatorySynapses;
        for (int n = 1; n<=dend_syns; n++)
        {
          // CRITICAL: GetComponentL now returns weak_ptr, need to lock
          std::weak_ptr<UContainer> syn_weak2 = fin_dend->GetComponentL("ExcSynapse"+sntoa(n),true);
          std::shared_ptr<NPulseSynapse> syn;
          if(!syn_weak2.expired())
           syn = std::dynamic_pointer_cast<NPulseSynapse>(syn_weak2.lock());
          else
           syn = nullptr;
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
            continue; //������� � ���������� �������
          }
          else
          {
           //finish_name = ActivePIs[j]->GetName()+".Neuron1.Dendrite1_5.ExcSynapse"+sntoa(n);
            finish = syn->GetLongName(GetThisAsSharedContainer());
           res&=CreateLink(start,"Output",finish,"Input");
    //       if(!res)
    //        return true;
           ActivePIs[j]->Paths.push_back(syn);
           break;
          }
        }


        //�� N1_S1
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock
        std::weak_ptr<UContainer> fin_neuron_weak3 = BaseTE->Backwards[i]->GetComponentL("Neuron1", true);
        if(!fin_neuron_weak3.expired())
         fin_neuron = std::dynamic_pointer_cast<NPulseNeuron>(fin_neuron_weak3.lock());
        else
         fin_neuron = nullptr;
        if(!fin_neuron)
         continue;
        std::weak_ptr<UContainer> fin_soma_weak3 = fin_neuron->GetComponentL("Soma1", true);
        if(!fin_soma_weak3.expired())
         fin_soma = std::dynamic_pointer_cast<NPulseMembrane>(fin_soma_weak3.lock());
        else
         fin_soma = nullptr;

        soma_syn = fin_soma->NumExcitatorySynapses;
        for (int n = 1; n<=soma_syn; n++)
        {
          // CRITICAL: GetComponentL now returns weak_ptr, need to lock
          std::weak_ptr<UContainer> syn_weak4 = fin_soma->GetComponentL("ExcSynapse"+sntoa(n),true);
          std::shared_ptr<NPulseSynapse> syn;
          if(!syn_weak4.expired())
           syn = std::dynamic_pointer_cast<NPulseSynapse>(syn_weak4.lock());
          else
           syn = nullptr;
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
            continue; //������� � ���������� �������
          }
          else
          {
            finish = syn->GetLongName(GetThisAsSharedContainer());
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

    //������� ����������� TE, ��������� �������
    //DelComponent(BaseTE,true);
    TEToDelete = BaseTE;
    BaseTE = ActivePIs[0];
    NameT check_base = BaseTE->GetName();
    TrajectoryElements.erase(TrajectoryElements.begin()+CurrentTE);

    //DelComponent(BaseMPC, true);
    MPCToDelete = BaseMPC;
    BaseMPC = MultiPCs[active_index];
    MultiPCs.erase(MultiPCs.begin()+CurrentTE);
    CurrentTE = active_index;

    return true;
}


bool NMazeMemory::UpdateCurrentTE()
{
    //��������� � ���������� �������� ���������� (��������� CurrentTE)
    for(int i = 0; i<int(TrajectoryElements.size()); i++)
    {
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock

        std::weak_ptr<UContainer> neuron_weak = TrajectoryElements[i]->GetComponentL("Neuron1", true);

        std::shared_ptr<NPulseNeuron> neuron;

        if(!neuron_weak.expired())

         neuron = std::dynamic_pointer_cast<NPulseNeuron>(neuron_weak.lock());

        else

         neuron = nullptr;
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock

        std::weak_ptr<UContainer> ltzone_weak = neuron->GetComponentL("LTZone", true);

        std::shared_ptr<NPulseLTZoneCommon> ltzone;

        if(!ltzone_weak.expired())

         ltzone = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzone_weak.lock());

        else

         ltzone = nullptr;
        if(!ltzone)
            return true;

        if(ltzone->OutputFrequency->As<double>(0)>0)
        {
            CurrentTE=i;
            string currentf_name = string(TrajectoryElements[i]->GetName());// ��� ��

            //��������� LastUsedPath (CurrentForward)
            for(int j = 0; j<int(BaseTE->Paths.size()); j++)
            {
                string name = string(BaseTE->Paths[j]->GetLongName(GetThisAsSharedContainer()));// ��� �������
                //if (name==currentf_name)
                //int check_comparison = name.compare(0, currentf_name.length(), currentf_name);
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


bool NMazeMemory::ProcessOptions()
{

  return true;
}


bool NMazeMemory::LastUsedLink()
{
    //��� �����, �� ������� ������ � ��� �����, w = 0,2
    //���� �������� ������ - �.�. �����, �� ������� ������ � ���� TE
    bool found = false;
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock

    std::weak_ptr<UContainer> neuron1_weak = BaseTE->GetComponentL("Neuron1", true);

    std::shared_ptr<NPulseNeuron> neuron1;

    if(!neuron1_weak.expired())

     neuron1 = std::dynamic_pointer_cast<NPulseNeuron>(neuron1_weak.lock());

    else

     neuron1 = nullptr;

    //��������� �������� ����� �� N1_D1_5
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock

    std::weak_ptr<UContainer> dend1_5_weak = neuron1->GetComponentL("Dendrite1_5",true);

    std::shared_ptr<NPulseMembrane> dend1_5;

    if(!dend1_5_weak.expired())

     dend1_5 = std::dynamic_pointer_cast<NPulseMembrane>(dend1_5_weak.lock());

    else

     dend1_5 = nullptr;
    for (int i = 1; i<=(dend1_5->NumExcitatorySynapses); i++)
    {
      // CRITICAL: GetComponentL now returns weak_ptr, need to lock
      std::weak_ptr<UContainer> synapse_weak_d15 = dend1_5->GetComponentL("ExcSynapse"+sntoa(i),true);
      std::shared_ptr<NPulseSynapse> synapse;
      if(!synapse_weak_d15.expired())
       synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak_d15.lock());
      else
       synapse = nullptr;
      if (synapse->Output().As<double>(0)>0)
      {
          synapse->Weight=0.2; //���������!
          found = true;
          break;
      }
    }

    if (!found)
    {
      //��������� �������� ����� (�� N1_D1_2)
      // CRITICAL: GetComponentL now returns weak_ptr, need to lock

      std::weak_ptr<UContainer> dend1_2_weak = neuron1->GetComponentL("Dendrite1_2",true);

      std::shared_ptr<NPulseMembrane> dend1_2;

      if(!dend1_2_weak.expired())

       dend1_2 = std::dynamic_pointer_cast<NPulseMembrane>(dend1_2_weak.lock());

      else

       dend1_2 = nullptr;
      for (int i = 1; i<=(dend1_2->NumExcitatorySynapses); i++)
      {
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock
        std::weak_ptr<UContainer> synapse_weak_d12_loop = dend1_2->GetComponentL("ExcSynapse"+sntoa(i),true);
        std::shared_ptr<NPulseSynapse> synapse;
        if(!synapse_weak_d12_loop.expired())
         synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak_d12_loop.lock());
        else
         synapse = nullptr;
        if (synapse->Output().As<double>(0)>0)
        {
            synapse->Weight=0.2; //���������!
            found = true;
            break;
        }
      }

      //��������� �������� ����� (�� N1_S1)
      // CRITICAL: GetComponentL now returns weak_ptr, need to lock
      std::weak_ptr<UContainer> soma1_weak = neuron1->GetComponentL("Soma1",true);
      std::shared_ptr<NPulseMembrane> soma1;
      if(!soma1_weak.expired())
       soma1 = std::dynamic_pointer_cast<NPulseMembrane>(soma1_weak.lock());
      else
       soma1 = nullptr;
      for (int i = 1; i<=(soma1->NumExcitatorySynapses); i++)
      {
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock
        std::weak_ptr<UContainer> synapse_weak = soma1->GetComponentL("ExcSynapse"+sntoa(i),true);
        std::shared_ptr<NPulseSynapse> synapse;
        if(!synapse_weak.expired())
         synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak.lock());
        else
         synapse = nullptr;
        if (synapse->Output().As<double>(0)>0)
        {
            synapse->Weight=0.2; //���������!
            found = true;
            break;
        }
      }
    }

    return true;
}


bool NMazeMemory::CallForResponse()
{


    return true;
}



bool NMazeMemory::DeadlockProcessing()//������� �������
{
//    //w �������� ����� �� ��(i-1) = 1
//    std::shared_ptr<NTrajectoryElement> PrevTE;
//    int k = PassedTEs.size()-2;
//    if (k>=0)
//    {
//      PrevTE = PassedTEs[k];
//      string check_prevTE = string(PrevTE->GetName());
//    }
//    NameT start_name = PrevTE->GetName()+".Output";

//    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<UContainer> fin_neuron_weak = BaseTE->GetComponentL("Neuron1", true);
    std::shared_ptr<NPulseNeuron> fin_neuron;
    if(!fin_neuron_weak.expired())
     fin_neuron = std::dynamic_pointer_cast<NPulseNeuron>(fin_neuron_weak.lock());
    else
     fin_neuron = nullptr;
//    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<UContainer> fin_dend_weak = fin_neuron->GetComponentL("Dendrite1_2", true);
    std::shared_ptr<NPulseMembrane> fin_dend;
    if(!fin_dend_weak.expired())
     fin_dend = std::dynamic_pointer_cast<NPulseMembrane>(fin_dend_weak.lock());
    else
     fin_dend = nullptr;
//    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<UContainer> fin_soma_weak = fin_neuron->GetComponentL("Soma1", true);
    std::shared_ptr<NPulseMembrane> fin_soma;
    if(!fin_soma_weak.expired())
     fin_soma = std::dynamic_pointer_cast<NPulseMembrane>(fin_soma_weak.lock());
    else
     fin_soma = nullptr;
//    int check_dmax = fin_dend->NumExcitatorySynapses();

//    for(int i=0; i< fin_dend->NumExcitatorySynapses(); i++)
//    {
//      NameT finish_name = BaseTE->GetName()+".Neuron1.Dendrite1_2.ExcSynapse"+sntoa(i+1);
//      if(CheckLink(start_name,finish_name))
//      {
//          std::shared_ptr<NPulseSynapse> fin_synapse = fin_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i), true);
//          fin_synapse->Weight=1;

//              //fin_synapse = fin_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i), true);
//              //fin_synapse->Weight=1;
//      }
//    }

//    //w �������� ��(i) = 0,2
//    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<UContainer> base_neuron_weak = BaseTE->GetComponentL("Neuron1", true);
    std::shared_ptr<NPulseNeuron> base_neuron;
    if(!base_neuron_weak.expired())
     base_neuron = std::dynamic_pointer_cast<NPulseNeuron>(base_neuron_weak.lock());
    else
     base_neuron = nullptr;
//    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<UContainer> base_dend_weak = base_neuron->GetComponentL("Dendrite1_5", true);
    std::shared_ptr<NPulseMembrane> base_dend;
    if(!base_dend_weak.expired())
     base_dend = std::dynamic_pointer_cast<NPulseMembrane>(base_dend_weak.lock());
    else
     base_dend = nullptr;
//    for (int j=0; j<base_dend->NumExcitatorySynapses; j++)
//    {
//      std::shared_ptr<NPulseSynapse> base_synapse = base_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(j), true);
//      base_synapse->Weight=0.2;
//    }

//    //+������ "���������" �� �� ����� ���������� �����
//    PassedTEs.pop_back();

    return true;
}

}

#endif
