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
//      // CRITICAL: GetComponentL now returns weak_ptr, need to lock
//      std::weak_ptr<RDK::UContainer> neuron_trainer_weak = MultiPCs[j]->GetComponentL("InputNeuron1-1", true);
//      std::shared_ptr<NNeuronTrainer> neuron_trainer;
//      if(!neuron_trainer_weak.expired())
//       neuron_trainer = std::dynamic_pointer_cast<NNeuronTrainer>(neuron_trainer_weak.lock());
//      else
//       neuron_trainer = nullptr;
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
      if(!root)
      {
        // During CreateClassSamples, NTrajectoryElement may not be registered yet
        // Skip creating root point during initialization
        return true;
      }
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

         // CRITICAL: GetComponentL now returns weak_ptr, need to lock


         std::weak_ptr<RDK::UContainer> base_neuron_weak = TrajectoryElements[CurrentTE]->GetComponentL("Neuron1", true);


         std::shared_ptr<NPulseNeuron> base_neuron;


         if(!base_neuron_weak.expired())


          base_neuron = std::dynamic_pointer_cast<NPulseNeuron>(base_neuron_weak.lock());


         else


          base_neuron = nullptr;
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock

         std::weak_ptr<RDK::UContainer> ltzone_te_weak = base_neuron->GetComponentL("LTZone", true);

         std::shared_ptr<NLTZone> ltzone_te;

         if(!ltzone_te_weak.expired())

          ltzone_te = std::dynamic_pointer_cast<NLTZone>(ltzone_te_weak.lock());

         else

          ltzone_te = nullptr;
         if(!ltzone_te)
          return true;

         //��������� ������� �� ������� �� ��� �������� ������ �� ����
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock

         std::weak_ptr<RDK::UContainer> base_soma_weak = base_neuron->GetComponentL("Soma1", true);

         std::shared_ptr<NPulseMembrane> base_soma;

         if(!base_soma_weak.expired())

          base_soma = std::dynamic_pointer_cast<NPulseMembrane>(base_soma_weak.lock());

         else

          base_soma = nullptr;
         if (CurrentTE==0)
             base_soma->NumExcitatorySynapses = OptionsNum+1; //�������������� ������ �� ������ �� - ��� ������������ ������� � ������� ������ ��
         else
             base_soma->NumExcitatorySynapses = OptionsNum;
         base_soma->Reset();
         //�� N1_D1_2
         // CRITICAL: GetComponentL now returns weak_ptr, need to lock

         std::weak_ptr<RDK::UContainer> base_dend_weak = base_neuron->GetComponentL("Dendrite1_2", true);

         std::shared_ptr<NPulseMembrane> base_dend;

         if(!base_dend_weak.expired())

          base_dend = std::dynamic_pointer_cast<NPulseMembrane>(base_dend_weak.lock());

         else

          base_dend = nullptr;
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
          if(!traj_el)
          {
            // During CreateClassSamples, NTrajectoryElement may not be registered yet
            // Skip creating trajectory element during initialization
            continue;
          }
          traj_el->Reset();
          BaseTE->Forwards.push_back(traj_el);
          traj_el->Layer = CurrentLayer;

//          //������ ����� �� �������� �������� ���������� �� ������ ��� ���������
//          bool res(true);
//          //������������ ����������� �� N1_D5_syn1
//          // CRITICAL: GetComponentL now returns weak_ptr, need to lock
          std::weak_ptr<RDK::UContainer> input_neuron_weak = traj_el->GetComponentL("Neuron1", true);
          std::shared_ptr<NPulseNeuron> input_neuron;
          if(!input_neuron_weak.expired())
           input_neuron = std::dynamic_pointer_cast<NPulseNeuron>(input_neuron_weak.lock());
          else
           input_neuron = nullptr;
//          // CRITICAL: GetComponentL now returns weak_ptr, need to lock
          std::weak_ptr<RDK::UContainer> synapse_weak = input_neuron->GetComponentL("Dendrite1_5.ExcSynapse1",true);
          std::shared_ptr<NPulseSynapse> synapse;
          if(!synapse_weak.expired())
           synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak.lock());
          else
           synapse = nullptr;
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
//          // CRITICAL: GetComponentL now returns weak_ptr, need to lock
//          std::weak_ptr<RDK::UContainer> synapse_weak_assign = input_neuron->GetComponentL("Soma1.InhSynapse1",true);
//          if(!synapse_weak_assign.expired())
//           synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak_assign.lock());
//          else
//           synapse = nullptr;
//          res&=CreateLink(BaseTE->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
//          if(!res)
//           return true;
//          //��������� ����������� �� N2_S1_syn1
//          // CRITICAL: GetComponentL now returns weak_ptr, need to lock
          std::weak_ptr<RDK::UContainer> input_neuron_weak_assign = traj_el->GetComponentL("Neuron2", true);
          if(!input_neuron_weak_assign.expired())
           input_neuron = std::dynamic_pointer_cast<NPulseNeuron>(input_neuron_weak_assign.lock());
          else
           input_neuron = nullptr;
//          // CRITICAL: GetComponentL now returns weak_ptr, need to lock
//          std::weak_ptr<RDK::UContainer> synapse_weak_assign = input_neuron->GetComponentL("Soma1.InhSynapse1",true);
//          if(!synapse_weak_assign.expired())
//           synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak_assign.lock());
//          else
//           synapse = nullptr;
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
//           // CRITICAL: GetComponentL now returns weak_ptr, need to lock
////           std::weak_ptr<RDK::UContainer> output_postinputn_weak = output_mpc->GetComponentL("PreControlNeuron1", true);
//           std::shared_ptr<NPulseNeuron> output_postinputn;
//           if(!output_postinputn_weak.expired())
//            output_postinputn = std::dynamic_pointer_cast<NPulseNeuron>(output_postinputn_weak.lock());
//           else
//            output_postinputn = nullptr;
//           // CRITICAL: GetComponentL now returns weak_ptr, need to lock
//           std::weak_ptr<RDK::UContainer> output_ltzone_weak = output_postinputn->GetComponentL("LTZone", true);
//           std::shared_ptr<NLTZone> output_ltzone;
//           if(!output_ltzone_weak.expired())
//            output_ltzone = std::dynamic_pointer_cast<NLTZone>(output_ltzone_weak.lock());
//           else
//            output_ltzone = nullptr;

//           std::shared_ptr<NMultiPositionControl> input_mpc = MultiPCs[CurrentTE+option_num];//���������
//           // CRITICAL: GetComponentL now returns weak_ptr, need to lock
//           std::weak_ptr<RDK::UContainer> input_precontroln_weak = input_mpc->GetComponentL("PreControlNeuron1", true);
//           std::shared_ptr<NPulseNeuron> input_precontroln;
//           if(!input_precontroln_weak.expired())
//            input_precontroln = std::dynamic_pointer_cast<NPulseNeuron>(input_precontroln_weak.lock());
//           else
//            input_precontroln = nullptr;
//           // CRITICAL: GetComponentL now returns weak_ptr, need to lock
//           std::weak_ptr<RDK::UContainer> input_soma_weak = input_precontroln->GetComponentL("Soma1", true);
//           std::shared_ptr<NPulseMembrane> input_soma;
//           if(!input_soma_weak.expired())
//            input_soma = std::dynamic_pointer_cast<NPulseMembrane>(input_soma_weak.lock());
//           else
//            input_soma = nullptr;

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
//          // CRITICAL: GetComponentL now returns weak_ptr, need to lock
          std::weak_ptr<RDK::UContainer> action_soma_weak = ActionNeurons[input_action]->GetComponentL("Soma1",true);
          std::shared_ptr<NPulseMembrane> action_soma;
          if(!action_soma_weak.expired())
           action_soma = std::dynamic_pointer_cast<NPulseMembrane>(action_soma_weak.lock());
          else
           action_soma = nullptr;
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
 if(!traj_el)
 {
   LOG(ERROR) << "NMazeMemory_simplified::CreatePoint - Failed to create NTrajectoryElement, class may not be registered yet";
   return nullptr;
 }
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
     // CRITICAL: GetComponentL now returns weak_ptr, need to lock

     std::weak_ptr<RDK::UContainer> neuron_tr_weak = multi_pc->GetComponentL("InputNeuron1-1", true);

     std::shared_ptr<NNeuronTrainer> neuron_tr;

     if(!neuron_tr_weak.expired())

      neuron_tr = std::dynamic_pointer_cast<NNeuronTrainer>(neuron_tr_weak.lock());

     else

      neuron_tr = nullptr;
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

 // CRITICAL: GetComponentL now returns weak_ptr, need to lock


 std::weak_ptr<RDK::UContainer> postinput_weak = multi_pc->GetComponentL("PostInputNeuron1", true);


 std::shared_ptr<NPulseNeuron> postinput;


 if(!postinput_weak.expired())


  postinput = std::dynamic_pointer_cast<NPulseNeuron>(postinput_weak.lock());


 else


  postinput = nullptr;
 // CRITICAL: GetComponentL now returns weak_ptr, need to lock

 std::weak_ptr<RDK::UContainer> ltzone_weak = postinput->GetComponentL("LTZone", true);

 std::shared_ptr<NPulseLTZoneCommon> ltzone;

 if(!ltzone_weak.expired())

  ltzone = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzone_weak.lock());

 else

  ltzone = nullptr;
//       if(!ltzone)
//           return true;

 // CRITICAL: GetComponentL now returns weak_ptr, need to lock


 std::weak_ptr<RDK::UContainer> neuron1_weak = traj_el->GetComponentL("Neuron1", true);


 std::shared_ptr<NPulseNeuron> neuron1;


 if(!neuron1_weak.expired())


  neuron1 = std::dynamic_pointer_cast<NPulseNeuron>(neuron1_weak.lock());


 else


  neuron1 = nullptr;
 // CRITICAL: GetComponentL now returns weak_ptr, need to lock

 std::weak_ptr<RDK::UContainer> synapse_weak = neuron1->GetComponentL("Dendrite1_1.InhSynapse1",true);

 std::shared_ptr<NPulseSynapse> synapse;

 if(!synapse_weak.expired())

  synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak.lock());

 else

  synapse = nullptr;
 res&=CreateLink(ltzone->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
 //      if(!res)
 //       return true;

 // CRITICAL: GetComponentL now returns weak_ptr, need to lock


 std::weak_ptr<RDK::UContainer> synapse_weak_assign_d13 = neuron1->GetComponentL("Dendrite1_3.InhSynapse1",true);


 if(!synapse_weak_assign_d13.expired())


  synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak_assign_d13.lock());


 else


  synapse = nullptr;
 res&=CreateLink(ltzone->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
 //      if(!res)
 //       return true;

 // CRITICAL: GetComponentL now returns weak_ptr, need to lock


 std::weak_ptr<RDK::UContainer> neuron2_weak = traj_el->GetComponentL("Neuron2", true);


 std::shared_ptr<NPulseNeuron> neuron2;
 std::weak_ptr<RDK::UContainer> synapse_weak_d11_neuron2;

 if(!neuron2_weak.expired())
 {
  neuron2 = std::dynamic_pointer_cast<NPulseNeuron>(neuron2_weak.lock());
  if(neuron2)
  {
   // CRITICAL: GetComponentL now returns weak_ptr, need to lock
   synapse_weak_d11_neuron2 = neuron2->GetComponentL("Dendrite1_1.InhSynapse1",true);
  }
 }
 else
 {
  neuron2 = nullptr;
 }

 if(!synapse_weak_d11_neuron2.expired())
  synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak_d11_neuron2.lock());
 else
  synapse = nullptr;

 if(synapse)
  res&=CreateLink(ltzone->GetLongName(GetThisAsSharedContainer()),"Output",synapse->GetLongName(GetThisAsSharedContainer()),"Input");
 //      if(!res)
 //       return true;

 // CRITICAL: GetComponentL now returns weak_ptr, need to lock
 std::weak_ptr<RDK::UContainer> synapse_weak_assign_d13_2;
 if(neuron2)
  synapse_weak_assign_d13_2 = neuron2->GetComponentL("Dendrite1_3.InhSynapse1",true);

 if(!synapse_weak_assign_d13_2.expired())
  synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak_assign_d13_2.lock());


 else


  synapse = nullptr;
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
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> neuron_weak = t_element->Forwards[i]->GetComponentL("Neuron1", true);
    std::shared_ptr<NPulseNeuron> neuron;
    if(!neuron_weak.expired())
     neuron = std::dynamic_pointer_cast<NPulseNeuron>(neuron_weak.lock());
    else
     neuron = nullptr;
    if(!neuron)
     continue;
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock

    std::weak_ptr<RDK::UContainer> ltzone_weak = neuron->GetComponentL("LTZone", true);

    std::shared_ptr<NPulseLTZoneCommon> ltzone;

    if(!ltzone_weak.expired())

     ltzone = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzone_weak.lock());

    else

     ltzone = nullptr;
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
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock

    std::weak_ptr<RDK::UContainer> neuron_weak = MultiPCs[i]->GetComponentL("PostInputNeuron1", true);

    std::shared_ptr<NPulseNeuron> neuron;

    if(!neuron_weak.expired())

     neuron = std::dynamic_pointer_cast<NPulseNeuron>(neuron_weak.lock());

    else

     neuron = nullptr;
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock

    std::weak_ptr<RDK::UContainer> ltzone_weak = neuron->GetComponentL("LTZone", true);

    std::shared_ptr<NPulseLTZoneCommon> ltzone;

    if(!ltzone_weak.expired())

     ltzone = std::dynamic_pointer_cast<NPulseLTZoneCommon>(ltzone_weak.lock());

    else

     ltzone = nullptr;
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
      // CRITICAL: GetComponentL now returns weak_ptr, need to lock

      std::weak_ptr<RDK::UContainer> fin_neuron_weak = ActivePIs[j]->GetComponentL("Neuron1", true);

      std::shared_ptr<NPulseNeuron> fin_neuron;

      if(!fin_neuron_weak.expired())

       fin_neuron = std::dynamic_pointer_cast<NPulseNeuron>(fin_neuron_weak.lock());

      else

       fin_neuron = nullptr;
      // CRITICAL: GetComponentL now returns weak_ptr, need to lock

      std::weak_ptr<RDK::UContainer> fin_dend_weak = fin_neuron->GetComponentL("Dendrite1_5", true);

      std::shared_ptr<NPulseMembrane> fin_dend;

      if(!fin_dend_weak.expired())

       fin_dend = std::dynamic_pointer_cast<NPulseMembrane>(fin_dend_weak.lock());

      else

       fin_dend = nullptr;
      int dend_syns = fin_dend->NumExcitatorySynapses;
      for (int n = 1; n<=dend_syns; n++)
      {
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock
        std::weak_ptr<RDK::UContainer> syn_weak = fin_dend->GetComponentL("ExcSynapse"+sntoa(n),true);
        std::shared_ptr<NPulseSynapse> syn;
        if(!syn_weak.expired())
         syn = std::dynamic_pointer_cast<NPulseSynapse>(syn_weak.lock());
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

      std::weak_ptr<RDK::UContainer> fin_soma_weak = fin_neuron->GetComponentL("Soma1", true);

      std::shared_ptr<NPulseMembrane> fin_soma;

      if(!fin_soma_weak.expired())

       fin_soma = std::dynamic_pointer_cast<NPulseMembrane>(fin_soma_weak.lock());

      else

       fin_soma = nullptr;
      int soma_syn = fin_soma->NumInhibitorySynapses;
      for (int n = 1; n<=soma_syn; n++)
      {
       // CRITICAL: GetComponentL now returns weak_ptr, need to lock
       std::weak_ptr<RDK::UContainer> syn_weak = fin_soma->GetComponentL("InhSynapse"+sntoa(n),true);
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

      std::weak_ptr<RDK::UContainer> fin_neuron_weak_assign = ActivePIs[j]->GetComponentL("Neuron2", true);

      if(!fin_neuron_weak_assign.expired())

       fin_neuron = std::dynamic_pointer_cast<NPulseNeuron>(fin_neuron_weak_assign.lock());

      else

       fin_neuron = nullptr;
      // CRITICAL: GetComponentL now returns weak_ptr, need to lock

      std::weak_ptr<RDK::UContainer> fin_soma_weak_assign = fin_neuron->GetComponentL("Soma1", true);

      if(!fin_soma_weak_assign.expired())

       fin_soma = std::dynamic_pointer_cast<NPulseMembrane>(fin_soma_weak_assign.lock());

      else

       fin_soma = nullptr;
      soma_syn = fin_soma->NumInhibitorySynapses;
      for (int n = 1; n<=soma_syn; n++)
      {
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock
        std::weak_ptr<RDK::UContainer> syn_weak = fin_soma->GetComponentL("InhSynapse"+sntoa(n),true);
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
        std::weak_ptr<RDK::UContainer> fin_neuron_weak = BaseTE->Backwards[i]->GetComponentL("Neuron1", true);
        std::shared_ptr<NPulseNeuron> fin_neuron;
        if(!fin_neuron_weak.expired())
         fin_neuron = std::dynamic_pointer_cast<NPulseNeuron>(fin_neuron_weak.lock());
        else
         fin_neuron = nullptr;
        if(!fin_neuron)
         continue;

        std::weak_ptr<RDK::UContainer> fin_dend_weak = fin_neuron->GetComponentL("Dendrite1_2", true);

        std::shared_ptr<NPulseMembrane> fin_dend;

        if(!fin_dend_weak.expired())

         fin_dend = std::dynamic_pointer_cast<NPulseMembrane>(fin_dend_weak.lock());

        else

         fin_dend = nullptr;
        int dend_syns = fin_dend->NumExcitatorySynapses;
        for (int n = 1; n<=dend_syns; n++)
        {
          // CRITICAL: GetComponentL now returns weak_ptr, need to lock
          std::weak_ptr<RDK::UContainer> syn_weak = fin_dend->GetComponentL("ExcSynapse"+sntoa(n),true);
          std::shared_ptr<NPulseSynapse> syn;
          if(!syn_weak.expired())
           syn = std::dynamic_pointer_cast<NPulseSynapse>(syn_weak.lock());
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
        std::weak_ptr<RDK::UContainer> fin_neuron_weak_2 = BaseTE->Backwards[i]->GetComponentL("Neuron1", true);
        if(!fin_neuron_weak_2.expired())
         fin_neuron = std::dynamic_pointer_cast<NPulseNeuron>(fin_neuron_weak_2.lock());
        else
         fin_neuron = nullptr;
        if(!fin_neuron)
         continue;

        std::weak_ptr<RDK::UContainer> fin_soma_weak_assign = fin_neuron->GetComponentL("Soma1", true);

        if(!fin_soma_weak_assign.expired())

         fin_soma = std::dynamic_pointer_cast<NPulseMembrane>(fin_soma_weak_assign.lock());

        else

         fin_soma = nullptr;

        soma_syn = fin_soma->NumExcitatorySynapses;
        for (int n = 1; n<=soma_syn; n++)
        {
          // CRITICAL: GetComponentL now returns weak_ptr, need to lock
          std::weak_ptr<RDK::UContainer> syn_weak = fin_soma->GetComponentL("ExcSynapse"+sntoa(n),true);
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
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock

    std::weak_ptr<RDK::UContainer> neuron1_weak = BaseTE->GetComponentL("Neuron1", true);

    std::shared_ptr<NPulseNeuron> neuron1;

    if(!neuron1_weak.expired())

     neuron1 = std::dynamic_pointer_cast<NPulseNeuron>(neuron1_weak.lock());

    else

     neuron1 = nullptr;

    //��������� �������� ����� �� N1_D1_5
    // CRITICAL: GetComponentL now returns weak_ptr, need to lock

    std::weak_ptr<RDK::UContainer> dend1_5_weak = neuron1->GetComponentL("Dendrite1_5",true);

    std::shared_ptr<NPulseMembrane> dend1_5;

    if(!dend1_5_weak.expired())

     dend1_5 = std::dynamic_pointer_cast<NPulseMembrane>(dend1_5_weak.lock());

    else

     dend1_5 = nullptr;
    for (int i = 1; i<=(dend1_5->NumExcitatorySynapses); i++)
    {
      // CRITICAL: GetComponentL now returns weak_ptr, need to lock
      std::weak_ptr<RDK::UContainer> synapse_weak = dend1_5->GetComponentL("ExcSynapse"+sntoa(i),true);
      std::shared_ptr<NPulseSynapse> synapse;
      if(!synapse_weak.expired())
       synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak.lock());
      else
       synapse = nullptr;
      if(!synapse)
       continue;
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

      std::weak_ptr<RDK::UContainer> dend1_2_weak = neuron1->GetComponentL("Dendrite1_2",true);

      std::shared_ptr<NPulseMembrane> dend1_2;

      if(!dend1_2_weak.expired())

       dend1_2 = std::dynamic_pointer_cast<NPulseMembrane>(dend1_2_weak.lock());

      else

       dend1_2 = nullptr;
      for (int i = 1; i<=(dend1_2->NumExcitatorySynapses); i++)
      {
        // CRITICAL: GetComponentL now returns weak_ptr, need to lock
        std::weak_ptr<RDK::UContainer> synapse_weak = dend1_2->GetComponentL("ExcSynapse"+sntoa(i),true);
        std::shared_ptr<NPulseSynapse> synapse;
        if(!synapse_weak.expired())
         synapse = std::dynamic_pointer_cast<NPulseSynapse>(synapse_weak.lock());
        else
         synapse = nullptr;
        if(!synapse)
         continue;
        if (synapse->Output().As<double>(0)>0)
        {
            synapse->Weight=0.2; //���������!
            found = true;
            break;
        }
      }

      //��������� �������� ����� (�� N1_S1)
      // CRITICAL: GetComponentL now returns weak_ptr, need to lock

      std::weak_ptr<RDK::UContainer> soma1_weak = neuron1->GetComponentL("Soma1",true);

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

//    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> fin_neuron_weak = BaseTE->GetComponentL("Neuron1", true);
    std::shared_ptr<NPulseNeuron> fin_neuron;
    if(!fin_neuron_weak.expired())
     fin_neuron = std::dynamic_pointer_cast<NPulseNeuron>(fin_neuron_weak.lock());
    else
     fin_neuron = nullptr;
//    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> fin_dend_weak = fin_neuron->GetComponentL("Dendrite1_2", true);
    std::shared_ptr<NPulseMembrane> fin_dend;
    if(!fin_dend_weak.expired())
     fin_dend = std::dynamic_pointer_cast<NPulseMembrane>(fin_dend_weak.lock());
    else
     fin_dend = nullptr;
//    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> fin_soma_weak = fin_neuron->GetComponentL("Soma1", true);
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
    std::weak_ptr<RDK::UContainer> base_neuron_weak = BaseTE->GetComponentL("Neuron1", true);
    std::shared_ptr<NPulseNeuron> base_neuron;
    if(!base_neuron_weak.expired())
     base_neuron = std::dynamic_pointer_cast<NPulseNeuron>(base_neuron_weak.lock());
    else
     base_neuron = nullptr;
//    // CRITICAL: GetComponentL now returns weak_ptr, need to lock
    std::weak_ptr<RDK::UContainer> base_dend_weak = base_neuron->GetComponentL("Dendrite1_5", true);
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
