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
: OptionsNum("OptionsNum",this, &NMazeMemory::SetOptionsNum),
  Situation("Situation",this, &NMazeMemory::SetNewSituation),
  //RememberSituation("RememberSituation",this, &NMazeMemory::SetRememberSituation),
  FeaturesNum("FeaturesNum",this, &NMazeMemory::SetFeaturesNum),
  IsDeadlock("IsDeadlock",this, &NMazeMemory::SetIsDeadlock)

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
bool NMazeMemory::SetOptionsNum(const int &value)
{
 Ready=false;
 return true;
}

bool NMazeMemory::SetNewSituation(const bool &value)
{
 return true;
}

//bool NMazeMemory::SetRememberSituation(const bool &value)
//{
// Ready=false;
// return true;
//}

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
 //RememberSituation = false;
 FeaturesNum = 1;
 OptionsNum = 2;
 IsDeadlock = false;

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
bool NMazeMemory::ABuild(void)
{
   if (TrajectoryElements.size()<1)
   {
       MVector<double,3> root_coords = {5.0, 4.0, 0};
       UEPtr<NTrajectoryElement> root = CreatePoint(root_coords);
   }

 return true;
}

// ��������� ������ ����� �������
bool NMazeMemory::ACalculate(void)
{
  base_TE = TrajectoryElements[CurrentTE];
  base_MPC = MultiPCs[CurrentTE];

  string check_baseTE = string(base_TE->GetName());
  string check_baseMPC = string(base_MPC->GetName());

  if (Situation)
  {
    //������������ ����� ��������
    Situation = false;
    PassedTEs.push_back(base_TE);

    //���������, ���� �� �������� PostInput ������� � ���� -
    //�.�.��������� �� ������� �������� � �����-���� �� ��� ���������
    ActivePIs.clear();
    //ActivePIs = CheckActivePIs();

    int active_number = -1;
    for(int i = 0; i<CurrentTE; i++)
    {
      UEPtr<NPulseNeuron> neuron = MultiPCs[i]->GetComponentL<NPulseNeuron>("PostInputNeuron1", true);
      UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
  //    if(!ltzone)
  //      return active_PIs;

      if(ltzone->OutputFrequency->As<double>(0) >0)
      {
        string check_actInp = TrajectoryElements[i]->GetName();
        ActivePIs.push_back(TrajectoryElements[i]);
        active_number = i;
      }
    }

   if(ActivePIs.size()>0)//���� ���������� ������ �������� ��, ������ ��������,
   {
     MergingTEs(active_number); //������� �������� �� � ����
   }


   //���������, ���� �� ��� � ���� �����
   NameT inp_n =  base_MPC->GetName()+".InputNeuron1-1.LTZone";
   NameT pi_n = base_MPC->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";

   if(!CheckLink(inp_n,pi_n)) //���� ��� �� ���� � ���� �����
   {
       if (OptionsNum <1) // ���� ��� ��������� ����������� ��������
       {
         //w �������� ����� �� ��(i-1) = 1,
         //w �������� ��(i) = 0,2
       }
       else // ���� ���� ��������� �����������
       {
         UEPtr<NTrajectoryElement> traj_el;
         MVector<double,3> base_coords = base_TE->GetCoord();

         CurrentLayer++;

         UEPtr<NPulseNeuron> base_neuron = TrajectoryElements[CurrentTE]->GetComponentL<NPulseNeuron>("Neuron1", true);
         UEPtr<NLTZone> ltzone_te = base_neuron->GetComponentL<NLTZone>("LTZone", true);
         if(!ltzone_te)
          return true;

         //��������� ������� �� ������� �� ��� �������� ������ �� ����
         UEPtr<NPulseMembrane> base_soma = base_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
         if (CurrentTE==0)
             base_soma->NumExcitatorySynapses = OptionsNum+1; //�������������� ������ �� ������ �� - ��� ������������ ������� � ������� ������ ��
         else
             base_soma->NumExcitatorySynapses = OptionsNum;
         base_soma->Reset();
         //�� N1_D1_2
         UEPtr<NPulseMembrane> base_dend = base_neuron->GetComponentL<NPulseMembrane>("Dendrite1_2", true);
         base_dend->NumExcitatorySynapses = OptionsNum;
         base_dend->Reset();


         for(int k = 0; k<OptionsNum; k++)
         {
          //������������ ���������� � ��������� �����
          double x = (double)CurrentLayer*LayerShift + base_coords[0];
          double y = (double)k*yShift + base_coords[1];
          MVector<double,3> coords = {x, y, 0.0};
          traj_el = CreatePoint(coords);//���������� ������
          traj_el->Reset();
          base_TE->Forwards.push_back(traj_el);
          traj_el->Layer = CurrentLayer;

          //������ ����� �� �������� �������� ���������� �� ������ ��� ���������
          bool res(true);
          //������������ ����������� �� N1_D5_syn1
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
          //��������� ����������� �� N1_S1_syn1
          synapse = input_neuron->GetComponentL<NPulseSynapse>("Soma1.InhSynapse1",true);
          res&=CreateLink(base_TE->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
          if(!res)
           return true;
          //��������� ����������� �� N2_S1_syn1
          input_neuron = traj_el->GetComponentL<NPulseNeuron>("Neuron2", true);
          synapse = input_neuron->GetComponentL<NPulseSynapse>("Soma1.InhSynapse1",true);
          res&=CreateLink(base_TE->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
          if(!res)
           return true;

          //�������� �����
          traj_el->Backwards.push_back(base_TE);
          traj_el->CurrentBackward = 0;
          //�� ����
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
          //�� N1_D1_2
          synapse = base_neuron->GetComponentL<NPulseSynapse>("Dendrite1_2.ExcSynapse"+sntoa(k+1),true);
          synapse->Weight = 0.2;
          res&=CreateLink(traj_el->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
          if(!res)
           return true;

          //����� ����� PreControl ��������� ��� "�����"
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
         }
       }

   }
   else //���� ��� ���� � ���� �����
   {
     UEPtr<NTrajectoryElement> prev_TE;
     int k = PassedTEs.size()-2; //-1 - ��-�� ���������� � 0 � -1 - �.�. ����� ���������� ����
     if (k<0)
         return true;
     prev_TE = PassedTEs[k];
     string check_prevTE = string(prev_TE->GetName());

     if (base_TE->Layer < prev_TE->Layer)//���� ��������� �� ������� ���� �� ��������� � ���������� ��
     {
       //��� �����, �� ������� ������ � ��� �����, w = 0,2
       //���� �������� ������ - �.�. �����, �� ������� ������ � ���� TE
       bool found = false;
       UEPtr<NPulseNeuron> neuron1 = base_TE->GetComponentL<NPulseNeuron>("Neuron1", true);

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

       //������ "����"
       NameT fin = base_MPC->GetName()+".PreControlNeuron1.Soma1.ExcSynapse1";
       bool res;
       res&=CreateLink(base_TE->GetLongName(this),"Output", fin,"Input");
//       if(!res)
//        return true;
       for (int j = 0; j< MultiPCs.size(); j++)
       {

       }



     }

   }





   //���������� ������� ��������
   NameT start_name =  base_MPC->GetName()+".InputNeuron1-1.LTZone";
   NameT finish_name = base_MPC->GetName()+".PostInputNeuron1.Soma1.ExcSynapse1";
   if(!CheckLink(start_name,finish_name))
       CreateLink(start_name,"Output", finish_name, "Input");
   start_name =  base_MPC->GetName()+".PreControlNeuron1.LTZone";
   finish_name = base_MPC->GetName()+".ControlNeuron1-1.Soma1.ExcSynapse1";
   if(!CheckLink(start_name,finish_name))
       CreateLink(start_name,"Output", finish_name, "Input");
   //base_MPC->RememberState = true;
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
      NameT currentf_name = TrajectoryElements[i]->GetName();

      bool found = false;

      //��������� CurrentForward
      for(int j = 0; j<int(base_TE->Forwards.size()); j++)
      {
        NameT name = base_TE->Forwards[j]->GetName();
        if (name==currentf_name)
        {
          base_TE->CurrentForward = j;
          found = true;
          break;
        }
      }
      if (found)
        break;

      //��������� CurrentBackward
      for(int j = 0; j<int(base_TE->Backwards.size()); j++)
      {
        NameT name = base_TE->Backwards[j]->GetName();
        if (name==currentf_name)
        {
          base_TE->CurrentBackward = j;
          found = true;
          break;
        }
      }
      break;
    }
  }




 //��������� � ���������� �������� ���������� (��������� CurrentForward) 
// for(int i = 0; i<int(base_TE->Forwards.size()); i++)
// {
//   UEPtr<NPulseNeuron> neuron = base_TE->Forwards[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
//   UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
//   if(!ltzone)
//       return true;

//   if(ltzone->OutputFrequency->As<double>(0)>0)
//   {
//    base_TE->CurrentForward = i;
//    NameT currentf_name = base_TE->Forwards[i]->GetName();

//    for(int j = 0; j<int(TrajectoryElements.size()); j++)//(��������� CurrentTE)
//    {
//      NameT name = TrajectoryElements[j]->GetName();
//      if (name==currentf_name)
//      {
//          CurrentTE=j;
//          break;
//      }
//    }
//    break;
//   }
// }

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
 multi_pc->InputsNum = FeaturesNum;
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
    UEPtr<NTrajectoryElement> prev_TE;
    int k = PassedTEs.size()-2;

    if (k<0)
        return true;

    prev_TE = PassedTEs[k];
    string check_prevTE = string(prev_TE->GetName());

    for (int j=0; j<ActivePIs.size(); j++)
    {
      //��������� ������� ����� � �������� �� �� ��������
      NameT start = prev_TE->GetLongName(this);
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

      int back_max = base_TE->Backwards.size();
      for (int i = 0; i<back_max; i++)
      {
        //�� N1_D1_2
        UEPtr<NPulseNeuron> fin_neuron = base_TE->Backwards[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
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
           break;
          }
        }


        //�� N1_S1
        fin_neuron = base_TE->Backwards[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
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


    }

    DelComponent(base_TE,true);
    base_TE = ActivePIs[0];
    NameT check_base = base_TE->GetName();

    DelComponent(base_MPC, true);
    base_MPC = MultiPCs[active_num];


    return true;
}


bool NMazeMemory::ProcessOptions()
{

  return true;
}


bool NMazeMemory::DeadlockProcessing()//������� �������
{
//    //w �������� ����� �� ��(i-1) = 1
//    UEPtr<NTrajectoryElement> prev_TE;
//    int k = PassedTEs.size()-2;
//    if (k>=0)
//    {
//      prev_TE = PassedTEs[k];
//      string check_prevTE = string(prev_TE->GetName());
//    }
//    NameT start_name = prev_TE->GetName()+".Output";

//    UEPtr<NPulseNeuron> fin_neuron = base_TE->GetComponentL<NPulseNeuron>("Neuron1", true);
//    UEPtr<NPulseMembrane> fin_dend = fin_neuron->GetComponentL<NPulseMembrane>("Dendrite1_2", true);
//    UEPtr<NPulseMembrane> fin_soma = fin_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
//    int check_dmax = fin_dend->NumExcitatorySynapses();

//    for(int i=0; i< fin_dend->NumExcitatorySynapses(); i++)
//    {
//      NameT finish_name = base_TE->GetName()+".Neuron1.Dendrite1_2.ExcSynapse"+sntoa(i+1);
//      if(CheckLink(start_name,finish_name))
//      {
//          UEPtr<NPulseSynapse> fin_synapse = fin_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i), true);
//          fin_synapse->Weight=1;

//              //fin_synapse = fin_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i), true);
//              //fin_synapse->Weight=1;
//      }
//    }

//    //w �������� ��(i) = 0,2
//    UEPtr<NPulseNeuron> base_neuron = base_TE->GetComponentL<NPulseNeuron>("Neuron1", true);
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
