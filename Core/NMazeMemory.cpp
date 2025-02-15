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
     //UEPtr<NNeuronTrainerMemory> neuron_trainer = MultiPCs[j]->GetComponentL<NNeuronTrainerMemory>("InputNeuron1-1", true);
     UEPtr<NNeuronTrainer> neuron_trainer = GetComponentL<NNeuronTrainer>("NNeuronTrainer"+sntoa(j), true);
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
        //UEPtr<NNeuronTrainerMemory> neuron_trainer = MultiPCs[j]->GetComponentL<NNeuronTrainerMemory>("InputNeuron1-1", true);
        UEPtr<NNeuronTrainer> neuron_trainer = GetComponentL<NNeuronTrainer>("NeuronTrainer"+sntoa(cnt), true);
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
bool NMazeMemory::AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer)
{

 return true;
}

// ��������� ��������������� ���������������� ��������
// ��� �������� ��������� ���������� �� ����� �������
// ����� ����� ������ ������ ���� comp
// ���������� � ������ ���������
bool NMazeMemory::ADelComponent(UEPtr<UContainer> comp)
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
      UEPtr<NTrajectoryElement> root = CreatePoint(root_coords);
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
        UEPtr<NTrajectoryElement> responding_TE;

        int max_layer = 0;
        for (int j = 0; j< int(MultiPCs.size()); j++)
        {
            //���������, ���� �� ����� ����� ���� ��� ����
            if (BaseTE->Layer - TrajectoryElements[j]->Layer > 1)
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
                        //string check_responding_name = string(responding_TE->GetLongName(this));
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
            BaseTE->Paths[BaseTE->LastUsedPath]->Weight=0.2;
            BaseTE->LastUsedPath++;
            //��� ��������� ����� w = 1,
            BaseTE->Paths[BaseTE->LastUsedPath]->Weight=1;
        }

        //������� ����������� ���� (TE+MultiPC)
        DelComponent(TEToDelete,true);
        DelComponent(MPCToDelete, true);

        //��������� �����, ������� ������� ��� "�����"
        NameT fin = BaseMPC->GetName()+".PreControlNeuron1.Soma1.ExcSynapse1";
        bool res(true);
        res&=BreakLink(BaseTE->GetLongName(this),"Output", fin,"Input");
        if(!res)
            return true;

        //���������� ����� � NeuronTrainer
        CurrentNT = GetComponentL<NNeuronTrainer>("NeuronTrainer"+sntoa(CurrentTE), true);
        NameT post_input = BaseMPC->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
        //string check_PI = string(post_input);
        //string check_name_nt = "NeuronTrainer"+sntoa(CurrentTE);
        res&=CreateLink(CurrentNT->GetLongName(this),"Output", post_input,"Input");
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
      if(WaitForSpike < 5000)
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
        string check_Path_n = BaseTE->Paths[num]->GetLongName(this);
        BaseTE->Paths[num]->Weight = 1; //����� ������ �� D1_5 - �.�. ������, ���������

        //w ���� ������ �� ���� ��(i) = 0.2
        UEPtr<NPulseNeuron> neuron1 = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);
        UEPtr<NPulseMembrane> dend1_5 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_5",true);
        int max = dend1_5->NumExcitatorySynapses;
        for (int i = 0; i<max; i++)
        {
            UEPtr<NPulseSynapse> synapse = dend1_5->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i+1),true);
            string check_syn_n = synapse->GetLongName(this);
            synapse->Weight = 0.2;
        }

        //� ���� �� �� ����� MultiPC �������? - ������� ���������� ���������
        bool done = true;
        for(int i = 0; i<int(MultiPCs.size()); i++)
        {
            UEPtr<NNeuronTrainer> neuron_trainer = GetComponentL<NNeuronTrainer>("NeuronTrainer"+sntoa(i), true);
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
            UEPtr<NPulseNeuron> neuron = TrajectoryElements[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
            UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
            if(!ltzone)
                return true;

            if(ltzone->OutputFrequency->As<double>(0)>0)
            {
                CurrentTE=i;
                string currentf_name = string(TrajectoryElements[i]->GetName());// ��� ��

                //��������� LastUsedPath (CurrentForward)
                for(int j = 0; j<int(BaseTE->Paths.size()); j++)
                {
                    string name = string(BaseTE->Paths[j]->GetLongName(this));// ��� �������
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
        PrevTE = BaseTE;
        IsNotFinished = false;
  }

  if (Situation)//������������ ��������
  {
    if (WaitForSpikePI==0)//��������� ����������,������ ���� ������ �� ����������� �����
    {
      //Situation = false;
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
   string check_name_nt = "NeuronTrainer"+sntoa(CurrentTE);
   CurrentNT = GetComponentL<NNeuronTrainer>("NeuronTrainer"+sntoa(CurrentTE), true);

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
         UEPtr<NTrajectoryElement> traj_el;
         MVector<double,3> base_coords = BaseTE->GetCoord();
         CurrentLayer++;
         LinkPoint(traj_el, base_coords, OptionsNum);//������� TE+MultiPC � ����������� �����
       }

       //���������� ������� �������� (��������� �������� ���������)
       UEPtr<NNeuronTrainer> neuron_trainer = AddMissingComponent<NNeuronTrainer>(std::string("NeuronTrainer"+sntoa(CurrentTE)), "NNeuronTrainer");
       MVector<double,3> base_coords = BaseTE->GetCoord();
       neuron_trainer->SetCoord(MVector<double,3>(base_coords[0], base_coords[1]+4.0, 0));
       neuron_trainer->NumInputDendrite = FeaturesNum;
       neuron_trainer->Reset();
       neuron_trainer->InputPattern = SituationCoords;
       //neuron_trainer->Reset(); //�������, ����� ������ ��������������� ����� ������� Situation
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
   else //���� ��� ���� � ���� �����
   {
     PassedTEs.clear();

     string check_base_name = string(BaseTE->GetName());
     int check_base_layer = BaseTE->Layer;
     string check_prev_name = string(PrevTE->GetName());
     int check_prev_layer = PrevTE->Layer;

     if (BaseTE->Layer <= PrevTE->Layer)//���� �� ���������� �� ������� ���� �� ��������� � ���������� �� (�.�. �� ������������ ������ � ������������ ���������)
     {
        //��� �����, �� ������� ������ � ��� �����, w = 0,2
        PrevTE->Paths[PrevTE->LastUsedPath]->Weight = 0.2;

        int check_lastusedpath = PrevTE->LastUsedPath;
        string check_paths = string(PrevTE->Paths[PrevTE->LastUsedPath]->GetLongName(this));

        //������ "����"
        NameT fin = BaseMPC->GetName()+".PreControlNeuron1.Soma1.ExcSynapse1";
        string check_fin = string(fin);
        bool res(true);
        res&=CreateLink(BaseTE->GetLongName(this),"Output", fin,"Input");
        if(!res)
          return true;

        //��� �������
        UEPtr<NPulseNeuron> check_n1 = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);
        UEPtr<NPulseLTZoneCommon> check_ltz1 = check_n1->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
        double check_base_activity = check_ltz1->OutputFrequency->As<double>(0);
        if (check_base_activity>0)
        {}

        IsWaitingForAnswer = true;
        return true;
      }
    } //����� "���� ��� ���� � ���� �����"

    Situation = false;

  }//����� if (Situation)

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
   UEPtr<NMultiPositionControl> multi_pc = dynamic_pointer_cast<NMultiPositionControl>(cont);
   MultiPCs.push_back(multi_pc);
   multi_pc->BuildSolo = true;
   multi_pc->ExternalControl = false;
   multi_pc->IsNeedToRebuild = true;
   multi_pc->InputsNum = 1;//����� InputNeurons, ���� InputNeuronsType = NSPNeuronGen (�.�. �������� ��������������� ������� �������� ���������)
   multi_pc->PrebuildStructure = true;
   multi_pc->Reset();

   //���������� ������ �� PostInputNeuron � TrajectoryElement
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


 bool NMazeMemory:: LinkPoint(UEPtr<NTrajectoryElement> traj_el,  MVector<double,3> base_coords, int options_num)
 {
     UEPtr<NPulseNeuron> base_neuron = TrajectoryElements[CurrentTE]->GetComponentL<NPulseNeuron>("Neuron1", true);
     UEPtr<NLTZone> ltzone_te = base_neuron->GetComponentL<NLTZone>("LTZone", true);
     if(!ltzone_te)
         return true;

     //��������� ������� �� ������� �� ��� �������� ������ �� ����
     UEPtr<NPulseMembrane> base_soma = base_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
     if (CurrentTE==0)
         base_soma->NumExcitatorySynapses = options_num+1; //�������������� ������ �� ������ �� - ��� ������������ ������� � ������� ������ ��
     else
         base_soma->NumExcitatorySynapses = options_num;
     base_soma->Reset();
     //�� N1_D1_2
     UEPtr<NPulseMembrane> base_dend = base_neuron->GetComponentL<NPulseMembrane>("Dendrite1_2", true);
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

     //��������� ����������� �� N1_S1_syn1
     synapse = input_neuron->GetComponentL<NPulseSynapse>("Soma1.InhSynapse1",true);
     res&=CreateLink(BaseTE->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
     if(!res)
         return true;
     //��������� ����������� �� N2_S1_syn1
     input_neuron = traj_el->GetComponentL<NPulseNeuron>("Neuron2", true);
     synapse = input_neuron->GetComponentL<NPulseSynapse>("Soma1.InhSynapse1",true);
     res&=CreateLink(BaseTE->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
     if(!res)
         return true;

     //�������� �����
     traj_el->Backwards.push_back(BaseTE);
     //traj_el->CurrentBackward = 0;

     //�� ����
     int syn_num;
     if (CurrentTE==0)
         syn_num = possible_action_num+1;
     else
         syn_num = possible_action_num;
     synapse = base_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(syn_num),true);
     string check_s1 = synapse->GetLongName(this);
     synapse->Weight = 0.2;
     res&=CreateLink(traj_el->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
     if(!res)
         return true;

     //�� N1_D1_2
     synapse = base_neuron->GetComponentL<NPulseSynapse>("Dendrite1_2.ExcSynapse"+sntoa(possible_action_num),true);
     string check_s2 = synapse->GetLongName(this);
     synapse->Weight = 0.2;
     res&=CreateLink(traj_el->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
     if(!res)
         return true;
     traj_el->Paths.push_back(synapse);

     //����� ����� PreControl ��������� ��� "�����"
     int passed_max = PassedTEs.size();
     for (int j=0; j<passed_max; j++)
     {   
         UEPtr<NMultiPositionControl> output_mpc = MultiPCs[j];
         UEPtr<NPulseNeuron> output_postinputn = output_mpc->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
         UEPtr<NLTZone> output_ltzone = output_postinputn->GetComponentL<NLTZone>("LTZone", true);
         if (!output_ltzone)
             return false;

         UEPtr<NMultiPositionControl> input_mpc = MultiPCs[int(MultiPCs.size())-1];//���������
         UEPtr<NPulseNeuron> input_precontroln = input_mpc->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
         UEPtr<NPulseMembrane> input_soma = input_precontroln->GetComponentL<NPulseMembrane>("Soma1", true);
         if (!input_soma)
             return false;


         //���� ��� �������
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
                 continue; //������� � ���������� �������
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

     //����� �� ������� ��������
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
             continue; //������� � ���������� �������
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
        UEPtr<NPulseNeuron> neuron = MultiPCs[i]->GetComponentL<NPulseNeuron>("PostInputNeuron1", true);
        UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
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
    int k = PassedTEs.size()-2;
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
            //������ ����� � �������� TE �� ��������, ����� �������� ����������
            NameT start = BaseTE->GetLongName(this);
            bool res(true);
            NameT finish;

            UEPtr<NPulseNeuron> fin_neuron = ActivePIs[j]->GetComponentL<NPulseNeuron>("Neuron1", true);
            UEPtr<NPulseMembrane> fin_dend = fin_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
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
                    continue; //������� � ���������� �������
                }
                else
                {
                    finish = syn->GetLongName(this);
                    res&=CreateLink(start,"Output",finish,"Input");
                    if(!res)
                       return true;
                    break;
                }
            }

            //�������� ��������� ����������� NeuronTrainer, ����� �� �� �������� ���������� TE
            NameT active_PI = MultiPCs[active_index]->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
            string check_actNT = string(active_PI);
            string check_NT = string(CurrentNT->GetLongName(this));
            res&=BreakLink(CurrentNT->GetLongName(this),"Output", active_PI,"Input");
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
      NameT start = PrevTE->GetLongName(this);
      bool res;
      NameT finish;

      //����������� ����������� �� N1_D1_5
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
         continue; //������� � ���������� �������
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

      //��������� ����������� �� N1_S1
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
         continue; //������� � ���������� �������
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

      //��������� ����������� �� N2_S1
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
          continue; //������� � ���������� �������
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

      //��������� �������� ����� � �������� �� �� ��������
      start = ActivePIs[j]->GetLongName(this);

      int back_max = BaseTE->Backwards.size();
      for (int i = 0; i<back_max; i++)
      {
        //�� N1_D1_2
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
            continue; //������� � ���������� �������
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


        //�� N1_S1
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
            continue; //������� � ���������� �������
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


bool NMazeMemory::ProcessOptions()
{

  return true;
}


bool NMazeMemory::LastUsedLink()
{
    //��� �����, �� ������� ������ � ��� �����, w = 0,2
    //���� �������� ������ - �.�. �����, �� ������� ������ � ���� TE
    bool found = false;
    UEPtr<NPulseNeuron> neuron1 = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);

    //��������� �������� ����� �� N1_D1_5
    UEPtr<NPulseMembrane> dend1_5 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_5",true);
    for (int i = 1; i<=(dend1_5->NumExcitatorySynapses); i++)
    {
      UEPtr<NPulseSynapse> synapse = dend1_5->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
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
      UEPtr<NPulseMembrane> dend1_2 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_2",true);
      for (int i = 1; i<=(dend1_2->NumExcitatorySynapses); i++)
      {
        UEPtr<NPulseSynapse> synapse = dend1_2->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
        if (synapse->Output().As<double>(0)>0)
        {
            synapse->Weight=0.2; //���������!
            found = true;
            break;
        }
      }

      //��������� �������� ����� (�� N1_S1)
      UEPtr<NPulseMembrane> soma1 = neuron1->GetComponentL<NPulseMembrane>("Soma1",true);
      for (int i = 1; i<=(soma1->NumExcitatorySynapses); i++)
      {
        UEPtr<NPulseSynapse> synapse = soma1->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
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

//    //w �������� ��(i) = 0,2
//    UEPtr<NPulseNeuron> base_neuron = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);
//    UEPtr<NPulseMembrane> base_dend = base_neuron->GetComponentL<NPulseMembrane>("Dendrite1_5", true);
//    for (int j=0; j<base_dend->NumExcitatorySynapses; j++)
//    {
//      UEPtr<NPulseSynapse> base_synapse = base_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(j), true);
//      base_synapse->Weight=0.2;
//    }

//    //+������ "���������" �� �� ����� ���������� �����
//    PassedTEs.pop_back();

    return true;
}

}

#endif
