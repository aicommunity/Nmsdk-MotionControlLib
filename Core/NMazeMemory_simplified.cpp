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

#ifndef NMAZEMEMORY_SIMPLIFIED_CPP
#define NMAZEMEMORY_SIMPLIFIED_CPP

#include "NMazeMemory_simplified.h"
#include <QString>

namespace NMSDK {

// ������
// --------------------------
// ������������ � �����������
// --------------------------
NMazeMemory_simplified::NMazeMemory_simplified(void)
: Situation("Situation",this, &NMazeMemory_simplified::SetSituation),
  InputActions("InputActions",this, &NMazeMemory_simplified::SetInputActions),
  ActionNeuronsType("ActionNeuronsType",this, &NMazeMemory_simplified::SetActionNeuronsType),
  FeaturesNum("FeaturesNum",this, &NMazeMemory_simplified::SetFeaturesNum),
  IsDone("IsDone",this, &NMazeMemory_simplified::SetIsDone),
  SituationCoords("SituationCoords",this, &NMazeMemory_simplified::SetSituationCoords)
  //IsDeadlock("IsDeadlock",this, &NMazeMemory::SetIsDeadlock)

{

}

NMazeMemory_simplified::~NMazeMemory_simplified(void)
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
bool NMazeMemory_simplified::SetSituation(const bool &value)
{
 return true;
}

bool NMazeMemory_simplified::SetInputActions(const std::vector<int> &value)
{
 return true;
}

bool NMazeMemory_simplified::SetActionNeuronsType(const string &value)
{
 return true;
}

bool NMazeMemory_simplified::SetFeaturesNum(const int &value)
{
 return true;
}

bool NMazeMemory_simplified::SetIsDone(const bool &value)
{
 return true;
}

bool NMazeMemory_simplified::SetSituationCoords(const MDMatrix<double> &value)
{
//    for (int j = 0; j<int(MultiPCs.size()); j++)
//    {
//      std::shared_ptr<NNeuronTrainer> neuron_trainer = MultiPCs[j]->GetComponentL<NNeuronTrainer>("InputNeuron1-1", true);
//      int cnt = 0;
//      int cnt_max = value.GetRows();
//      MDMatrix<double> pattern;
//      pattern.Resize(cnt_max+1,1);
//      neuron_trainer->InputPattern.Resize(cnt_max+1,1);
//      for (cnt = 0; cnt< cnt_max; cnt++)
//      {
//         pattern[cnt] = value[cnt];
//      }
//      pattern[cnt] = 0.2;
//      neuron_trainer->InputPattern = pattern;
//    }

 return true;
}

// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
NMazeMemory_simplified* NMazeMemory_simplified::New(void)
{
 return new NMazeMemory_simplified;
}

UComponent* NMazeMemory_simplified::NewStatic(void)
{
 return new NMazeMemory_simplified;
}
// --------------------------

// --------------------------
// ������� ������ ���������� ������������
// --------------------------
// ��������� ����������� ���������������� ��������
// ��� ���������� ��������� ���������� � ���� ������
// ����� ����� ������ ������ ���� comp ���
// ������� �������� � ������ ���������
bool NMazeMemory_simplified::AAddComponent(std::shared_ptr<UContainer> comp, std::shared_ptr<UIPointer> pointer)
{

 return true;
}

// ��������� ��������������� ���������������� ��������
// ��� �������� ��������� ���������� �� ����� �������
// ����� ����� ������ ������ ���� comp
// ���������� � ������ ���������
bool NMazeMemory_simplified::ADelComponent(std::shared_ptr<UContainer> comp)
{

 return true;
}
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------


// ����� �������� �����.
bool NMazeMemory_simplified::AReset(void)
{


 return true;
}

// �������������� �������� �� ��������� � ����� �������� �����
bool NMazeMemory_simplified::ADefault(void)
{
 Situation = false;
 InputActions.assign(3, 0); //(size,val)
 ActionNeuronsType = "NSPNeuronGen";
 FeaturesNum = 3;
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


// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NMazeMemory_simplified::ABuild(void)
{
   if (TrajectoryElements.size()<1)
   {
//      int x_base = 5;
//      int x_shift = 7;

//      //��������� ������� ��������
//      ActionNeurons.resize(InputActions.size());
//      for (int n=0; n<ActionNeurons.size(); n++)
//      {
//        ActionNeurons[n] = AddMissingComponent<NPulseNeuron>(std::string("ActionNeuron")+sntoa(n+1), ActionNeuronsType);
//        if(!ActionNeurons[n])
//         return true;
//        ActionNeurons[n]->SetCoord(MVector<double,3>(x_base+n*x_shift, 2.0, 0));
//      }

      //��������� �������� ������� ����������
      MVector<double,3> root_coords = {5.0, 5.0, 0};
      std::shared_ptr<NTrajectoryElement> root = CreatePoint(root_coords);
   }

 return true;
}

// ��������� ������ ����� �������
bool NMazeMemory_simplified::ACalculate(void)
{

  if (Situation)//������������ ��������
  {
    Situation = false;
    BaseTE = TrajectoryElements[CurrentTE];
    BaseMPC = MultiPCs[CurrentTE];
    PassedTEs.push_back(BaseTE);



       int OptionsNum = 0; //����� ��������� �������� � ������� �������� = ����� ��������� ��������� � ������� InputActions
       for (int j = 0; j < InputActions.size(); j++)//���������� ����� ��������� �������� �� �������� �������
       {
          if (InputActions[j]>0)
              OptionsNum++;
       }

       if (OptionsNum <1) // ���� ��� ��������� ����������� ��������
       {

       }
       else // ���� ���� ��������� �����������
       {
         std::shared_ptr<NTrajectoryElement> traj_el;
         MVector<double,3> base_coords = BaseTE->GetCoord();

         CurrentLayer++;

         std::shared_ptr<NPulseNeuron> base_neuron = TrajectoryElements[CurrentTE]->GetComponentL<NPulseNeuron>("Neuron1", true);
         std::shared_ptr<NLTZone> ltzone_te = base_neuron->GetComponentL<NLTZone>("LTZone", true);
         if(!ltzone_te)
          return true;

         //��������� ������� �� ������� �� ��� �������� ������ �� ����
         std::shared_ptr<NPulseMembrane> base_soma = base_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
         if (CurrentTE==0)
             base_soma->NumExcitatorySynapses = OptionsNum+1; //�������������� ������ �� ������ �� - ��� ������������ ������� � ������� ������ ��
         else
             base_soma->NumExcitatorySynapses = OptionsNum;
         base_soma->Reset();
         //�� N1_D1_2
         std::shared_ptr<NPulseMembrane> base_dend = base_neuron->GetComponentL<NPulseMembrane>("Dendrite1_2", true);
         base_dend->NumExcitatorySynapses = OptionsNum;
         base_dend->Reset();

         int option_num = 0; // ����� ���������� �������� = 0..OptionsNum
         for(int input_action = 0; input_action<int(InputActions.size()); input_action++)//����� �������� �� ������� ������� �������� = 0...InputActions.size()
         {
           if (InputActions[input_action]==0)
               continue;

           option_num++;

          //������������ ���������� � ��������� �����
          double x = (double)CurrentLayer*LayerShift + base_coords[0];
          double y = (double)(option_num-1)*yShift + base_coords[1];
          MVector<double,3> coords = {x, y, 0.0};
          traj_el = CreatePoint(coords);//���������� ������
          traj_el->Reset();
          BaseTE->Forwards.push_back(traj_el);
          traj_el->Layer = CurrentLayer;

//          //������ ����� �� �������� �������� ���������� �� ������ ��� ���������
//          bool res(true);
//          //������������ ����������� �� N1_D5_syn1
//          std::shared_ptr<NPulseNeuron> input_neuron = traj_el->GetComponentL<NPulseNeuron>("Neuron1", true);
//          std::shared_ptr<NPulseSynapse> synapse = input_neuron->GetComponentL<NPulseSynapse>("Dendrite1_5.ExcSynapse1",true);
//          if (option_num>1)
//          {
//            synapse->Weight = SideWeight;
//            //string check_sn = synapse->GetLongName(GetThisAsSharedContainer());
//          }
//          else
//          {
//            //traj_el->CurrentForward = 0;
//            traj_el->LastUsedPath = 0;

//          }
//          res&=CreateLink(BaseTE->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
//          if(!res)
//           return true;
//          BaseTE->Paths.push_back(synapse);

//          //��������� ����������� �� N1_S1_syn1
//          synapse = input_neuron->GetComponentL<NPulseSynapse>("Soma1.InhSynapse1",true);
//          res&=CreateLink(BaseTE->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
//          if(!res)
//           return true;
//          //��������� ����������� �� N2_S1_syn1
//          input_neuron = traj_el->GetComponentL<NPulseNeuron>("Neuron2", true);
//          synapse = input_neuron->GetComponentL<NPulseSynapse>("Soma1.InhSynapse1",true);
//          res&=CreateLink(BaseTE->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
//          if(!res)
//           return true;

//          //�������� �����
//          traj_el->Backwards.push_back(BaseTE);
//          //traj_el->CurrentBackward = 0;

//          //�� ����
//          int syn_num;
//          if (CurrentTE==0)
//              syn_num = option_num+1;
//          else
//              syn_num = option_num;
//          synapse = base_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(syn_num),true);
//          string check_s1 = synapse->GetLongName(GetThisAsSharedContainer());
//          synapse->Weight = 0.2;
//          res&=CreateLink(traj_el->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
//          if(!res)
//           return true;

//          //�� N1_D1_2
//          synapse = base_neuron->GetComponentL<NPulseSynapse>("Dendrite1_2.ExcSynapse"+sntoa(option_num),true);
//          string check_s2 = synapse->GetLongName(GetThisAsSharedContainer());
//          synapse->Weight = 0.2;
//          res&=CreateLink(traj_el->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
//          if(!res)
//           return true;
//          traj_el->Paths.push_back(synapse);

//          //����� ����� PreControl ��������� ��� "�����"
//          int passed_max = PassedTEs.size();
//          for (int j=0; j<passed_max; j++)
//          {
//           std::shared_ptr<NMultiPositionControl> output_mpc = MultiPCs[j];
//           std::shared_ptr<NPulseNeuron> output_postinputn = output_mpc->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
//           std::shared_ptr<NLTZone> output_ltzone = output_postinputn->GetComponentL<NLTZone>("LTZone", true);

//           std::shared_ptr<NMultiPositionControl> input_mpc = MultiPCs[CurrentTE+option_num];//���������
//           std::shared_ptr<NPulseNeuron> input_precontroln = input_mpc->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
//           std::shared_ptr<NPulseMembrane> input_soma = input_precontroln->GetComponentL<NPulseMembrane>("Soma1", true);

//           int syn_num = input_soma->NumExcitatorySynapses;
//           for (int i = 1; i<=syn_num; i++)
//           {
//            std::shared_ptr<NPulseSynapse> syn = input_precontroln->GetComponentL<NPulseSynapse>("Soma1.ExcSynapse"+sntoa(i),true);
//            if(!syn)
//              return true;

//            if (syn->Input.IsConnected())
//            {
//              if(syn_num==i)
//              {
//               input_soma->NumExcitatorySynapses++;
//               input_soma->Reset();
//               syn_num = input_soma->NumExcitatorySynapses;
//              }
//              continue; //������� � ���������� �������
//            }
//            else
//            {
//              res&=CreateLink(output_ltzone->GetLongName(GetThisAsSharedContainer()),"Output",syn->GetLongName(GetThisAsSharedContainer()),"Input");
//              if(!res)
//               return true;
//              break;
//            }
//           }
//          }

//          //����� �� ������� ��������
//          std::shared_ptr<NPulseMembrane> action_soma = ActionNeurons[input_action]->GetComponentL<NPulseMembrane>("Soma1",true);
//          int syn_max_num = action_soma->NumExcitatorySynapses;
//          for (int i = 1; i<=syn_max_num; i++)
//          {
//           std::shared_ptr<NPulseSynapse> action_syn = action_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
//           string check_act_name = action_soma->GetLongName(GetThisAsSharedContainer());

//           if (action_syn->Input.IsConnected())
//           {
//             if(syn_num==i)
//             {
//              action_soma->NumExcitatorySynapses++;
//              action_soma->Reset();
//              syn_max_num = action_soma->NumExcitatorySynapses;
//             }
//             continue; //������� � ���������� �������
//           }
//           else
//           {
//             res&=CreateLink(traj_el->GetLongName(GetThisAsSharedContainer()),"Output",action_syn->GetLongName(GetThisAsSharedContainer()),"Input");
//             if(!res)
//              return true;
//             break;
//           }
//          }

         }
       }

   }//����� if (Situation)


return true;
 }
// --------------------------


std::shared_ptr<NTrajectoryElement> NMazeMemory_simplified::CreatePoint(MVector<double,3> coords)
{
 std::shared_ptr<UContainer> cont;
 // IMPORTANT: Don't create shared_ptr from raw pointer via .get()
 // Use GetStorage() directly
 std::shared_ptr<UStorage> storage = GetStorage();
 if (!storage) {
   return nullptr;
 }
 std::shared_ptr<NTrajectoryElement> traj_el;

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
 std::shared_ptr<NMultiPositionControl> multi_pc = dynamic_pointer_cast<NMultiPositionControl>(cont);
 MultiPCs.push_back(multi_pc);
 multi_pc->BuildSolo = true;
 multi_pc->ExternalControl = false;
 multi_pc->IsNeedToRebuild = true;
 multi_pc->InputsNum = 1;//����� InputNeurons, ���� InputNeuronsType = NSPNeuronGen (�.�. �������� ��������������� ������� �������� ���������)
 multi_pc->PrebuildStructure = true;
 multi_pc->Reset();
 if ("NNeuronTrainer"==string(multi_pc->InputNeuronType))
 {
     std::shared_ptr<NNeuronTrainer> neuron_tr = multi_pc->GetComponentL<NNeuronTrainer>("InputNeuron1-1", true);
     if(neuron_tr)
     {
         neuron_tr->NumInputDendrite = FeaturesNum+1;//����� ��������� �� InputNeuron, ���� InputNeuronsType = NNeuronTrainer (�.�. �������� ��������������� ������� ��������� ���������)
         neuron_tr->Reset();
         neuron_tr->SpikesFrequency = 30;//����� ��������� ����������� �� �� ���� ���������� ������� ��� ������������ �� ��������� ��
         neuron_tr->IsNeedToTrain = false;
     }
 }


 //���������� ������ �� PostInputNeuron � TrajectoryElement
 bool res(true);

 std::shared_ptr<NPulseNeuron> postinput = multi_pc->GetComponentL<NPulseNeuron>("PostInputNeuron1", true);
 std::shared_ptr<NPulseLTZoneCommon> ltzone = postinput->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
//       if(!ltzone)
//           return true;

 std::shared_ptr<NPulseNeuron> neuron1 = traj_el->GetComponentL<NPulseNeuron>("Neuron1", true);
 std::shared_ptr<NPulseSynapse> synapse = neuron1->GetComponentL<NPulseSynapse>("Dendrite1_1.InhSynapse1",true);
 res&=CreateLink(ltzone->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
 //      if(!res)
 //       return true;

 synapse = neuron1->GetComponentL<NPulseSynapse>("Dendrite1_3.InhSynapse1",true);
 res&=CreateLink(ltzone->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
 //      if(!res)
 //       return true;

 std::shared_ptr<NPulseNeuron> neuron2 = traj_el->GetComponentL<NPulseNeuron>("Neuron2", true);
 synapse = neuron2->GetComponentL<NPulseSynapse>("Dendrite1_1.InhSynapse1",true);
 res&=CreateLink(ltzone->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
 //      if(!res)
 //       return true;

 synapse = neuron2->GetComponentL<NPulseSynapse>("Dendrite1_3.InhSynapse1",true);
 res&=CreateLink(ltzone->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
 //      if(!res)
 //       return true;

 return traj_el;
}

bool NMazeMemory_simplified::CheckActiveForwards(std::shared_ptr<NTrajectoryElement> t_element)
{
  int max = int(t_element->Forwards.size());
  for(int i = 0; i<max; i++)
  {
    std::shared_ptr<NPulseNeuron> neuron = t_element->Forwards[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
    std::shared_ptr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
//    if(!ltzone)
//      return false;

    if(ltzone->OutputFrequency->As<double>(0) >0)
    {
      return true;
    }
  }
  return false;
}


std::vector<std::shared_ptr<NTrajectoryElement>> NMazeMemory_simplified::CheckActivePIs()
{
  std::vector<std::shared_ptr<NTrajectoryElement>> active_PIs;
  for(int i = 0; i<CurrentTE; i++)
  {
    std::shared_ptr<NPulseNeuron> neuron = MultiPCs[i]->GetComponentL<NPulseNeuron>("PostInputNeuron1", true);
    std::shared_ptr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
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



bool NMazeMemory_simplified::MergingTEs(int active_num)
{
    int k = int(PassedTEs.size())-2;
    if (k<0)
        return true;

    PrevTE = PassedTEs[k];
    string check_prevTE = string(PrevTE->GetName());

    for (int j=0; j<ActivePIs.size(); j++)
    {
      //��������� ������� ����� � �������� �� �� ��������
      NameT start = PrevTE->GetLongName(GetThisAsSharedContainer());
      bool res;
      NameT finish;

      //����������� ����������� �� N1_D1_5
      std::shared_ptr<NPulseNeuron> fin_neuron = ActivePIs[j]->GetComponentL<NPulseNeuron>("Neuron1", true);
      std::shared_ptr<NPulseMembrane> fin_dend = fin_neuron->GetComponentL<NPulseMembrane>("Dendrite1_5", true);
      int dend_syns = fin_dend->NumExcitatorySynapses;
      for (int n = 1; n<=dend_syns; n++)
      {
        std::shared_ptr<NPulseSynapse> syn = fin_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(n),true);
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
      std::shared_ptr<NPulseMembrane> fin_soma = fin_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
      int soma_syn = fin_soma->NumInhibitorySynapses;
      for (int n = 1; n<=soma_syn; n++)
      {
       std::shared_ptr<NPulseSynapse> syn = fin_soma->GetComponentL<NPulseSynapse>("InhSynapse"+sntoa(n),true);
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
      fin_neuron = ActivePIs[j]->GetComponentL<NPulseNeuron>("Neuron2", true);
      fin_soma = fin_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
      soma_syn = fin_soma->NumInhibitorySynapses;
      for (int n = 1; n<=soma_syn; n++)
      {
        std::shared_ptr<NPulseSynapse> syn = fin_soma->GetComponentL<NPulseSynapse>("InhSynapse"+sntoa(n),true);
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
        std::shared_ptr<NPulseNeuron> fin_neuron = BaseTE->Backwards[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
        std::shared_ptr<NPulseMembrane> fin_dend = fin_neuron->GetComponentL<NPulseMembrane>("Dendrite1_2", true);
        int dend_syns = fin_dend->NumExcitatorySynapses;
        for (int n = 1; n<=dend_syns; n++)
        {
          std::shared_ptr<NPulseSynapse> syn = fin_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(n),true);
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
        fin_neuron = BaseTE->Backwards[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
        fin_soma = fin_neuron->GetComponentL<NPulseMembrane>("Soma1", true);

        soma_syn = fin_soma->NumExcitatorySynapses;
        for (int n = 1; n<=soma_syn; n++)
        {
          std::shared_ptr<NPulseSynapse> syn = fin_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(n),true);
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

    DelComponent(BaseTE,true);
    BaseTE = ActivePIs[0];
    NameT check_base = BaseTE->GetName();

    DelComponent(BaseMPC, true);
    BaseMPC = MultiPCs[active_num];


    return true;
}


bool NMazeMemory_simplified::ProcessOptions()
{

  return true;
}


bool NMazeMemory_simplified::LastUsedLink()
{
    //��� �����, �� ������� ������ � ��� �����, w = 0,2
    //���� �������� ������ - �.�. �����, �� ������� ������ � ���� TE
    bool found = false;
    std::shared_ptr<NPulseNeuron> neuron1 = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);

    //��������� �������� ����� �� N1_D1_5
    std::shared_ptr<NPulseMembrane> dend1_5 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_5",true);
    for (int i = 1; i<=(dend1_5->NumExcitatorySynapses); i++)
    {
      std::shared_ptr<NPulseSynapse> synapse = dend1_5->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
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
      std::shared_ptr<NPulseMembrane> dend1_2 = neuron1->GetComponentL<NPulseMembrane>("Dendrite1_2",true);
      for (int i = 1; i<=(dend1_2->NumExcitatorySynapses); i++)
      {
        std::shared_ptr<NPulseSynapse> synapse = dend1_2->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
        if (synapse->Output().As<double>(0)>0)
        {
            synapse->Weight=0.2; //���������!
            found = true;
            break;
        }
      }

      //��������� �������� ����� (�� N1_S1)
      std::shared_ptr<NPulseMembrane> soma1 = neuron1->GetComponentL<NPulseMembrane>("Soma1",true);
      for (int i = 1; i<=(soma1->NumExcitatorySynapses); i++)
      {
        std::shared_ptr<NPulseSynapse> synapse = soma1->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i),true);
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



bool NMazeMemory_simplified::DeadlockProcessing()//������� �������
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

//    std::shared_ptr<NPulseNeuron> fin_neuron = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);
//    std::shared_ptr<NPulseMembrane> fin_dend = fin_neuron->GetComponentL<NPulseMembrane>("Dendrite1_2", true);
//    std::shared_ptr<NPulseMembrane> fin_soma = fin_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
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
//    std::shared_ptr<NPulseNeuron> base_neuron = BaseTE->GetComponentL<NPulseNeuron>("Neuron1", true);
//    std::shared_ptr<NPulseMembrane> base_dend = base_neuron->GetComponentL<NPulseMembrane>("Dendrite1_5", true);
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
