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
    UEPtr<NTrajectoryElement> base_TE = TrajectoryElements[CurrentTE];
    UEPtr<NMultiPositionControl> base_MPC = MultiPCs[CurrentTE];

    if (NewSituation)
    {
      //������������ ����� ��������
      NewSituation = false;
      //base_TE = TrajectoryElements[CurrentTE];
      PassedTEs.push_back(base_TE);
      //base_MPC = MultiPCs[CurrentTE];


      if (IsDeadlock)//���� ������ � �����
      {
        //w �������� ����� �� ��(i-1) = 1
        UEPtr<NTrajectoryElement> prev_TE;
        int k = PassedTEs.size()-1;
        if (k>=0)
        {
          prev_TE = PassedTEs[k];
        }
        NameT start_name = prev_TE->GetName()+".Output";
        UEPtr<NPulseNeuron> fin_neuron = base_TE->GetComponentL<NPulseNeuron>("Neuron1", true);
        UEPtr<NPulseMembrane> fin_dend = fin_neuron->GetComponentL<NPulseMembrane>("Dendrite1_2", true);

        for(int i=0; i< fin_dend->NumExcitatorySynapses(); i++)
        {
          UEPtr<NPulseSynapse> fin_synapse = fin_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(i), true);
          NameT finish_name = fin_synapse->GetName();
          if(CheckLink(start_name,finish_name))
              fin_synapse->Weight=1;
        }

        //w �������� ��(i) = 0,2
        UEPtr<NPulseNeuron> base_neuron = base_TE->GetComponentL<NPulseNeuron>("Neuron1", true);
        UEPtr<NPulseMembrane> base_dend = base_neuron->GetComponentL<NPulseMembrane>("Dendrite1_5", true);
        for (int j=0; j<base_dend->NumExcitatorySynapses; j++)
        {
          UEPtr<NPulseSynapse> base_synapse = base_dend->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(j), true);
          base_synapse->Weight=0.2;
        }

        //+������ "���������" �� �� ����� ���������� �����
        PassedTEs.pop_back();
      }
//      else
//      {
//       //���������, ���� �� ������ �������� �� � ���� -
//       //�.�.��������� �� ������� �������� � �����-���� �� ��� ���������
//       for(int i = 0; i++; i<CurrentTE)
//       {
//        UEPtr<NPulseNeuron> neuron = TrajectoryElements[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
//        UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
//        if(!ltzone)
//            return true;

//        if(ltzone->OutputFrequency->As<double>(0) >0)
//        {
//         //string check_actInp = InputNeurons[i]->GetName();
//         ActiveTE.push_back(TrajectoryElements[i]);
//        }

//        if(ActiveTE.size()>0)
//        {
//            //������� �������� �� � ����
//        }

//       }
//      }






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

//      //��������� � ���������� �������� ���������� (��������� CurrentTE)
//      for(int i = 0; i<int(TrajectoryElements.size()); i++)
//      {
//       UEPtr<NPulseNeuron> neuron = TrajectoryElements[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
//       UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
//       if(!ltzone)
//           return true;
//       int layer = TrajectoryElements[i]->Layer;

//       if (layer<CurrentLayer)
//           continue;
//       else if (layer>CurrentLayer)
//           break;

//       string check_name = TrajectoryElements[i]->GetName();
//       double check_frequency = ltzone->OutputFrequency->As<double>(0);

//       //(layer==CurrentLayer)&&
//       if(ltzone->OutputFrequency->As<double>(0)>0)
//       {
//         CurrentTE=i;
//         string check_TE = TrajectoryElements[CurrentTE]->GetName();
//       }
//      }

      //��������� � ���������� �������� ���������� (��������� CurrentForward)
       for(int i = 0; i<int(base_TE->Forwards.size()); i++)
       {
        UEPtr<NPulseNeuron> neuron = base_TE->Forwards[i]->GetComponentL<NPulseNeuron>("Neuron1", true);
        UEPtr<NPulseLTZoneCommon> ltzone = neuron->GetComponentL<NPulseLTZoneCommon>("LTZone", true);
        if(!ltzone)
            return true;

        //string check_name = TrajectoryElements[i]->GetName();
        //double check_frequency = ltzone->OutputFrequency->As<double>(0);

        if(ltzone->OutputFrequency->As<double>(0)>0)
        {
         base_TE->CurrentForward = i;
         NameT currentf_name = base_TE->Forwards[i]->GetName();

         for(int j = 0; j<int(TrajectoryElements.size()); j++)//(��������� CurrentTE)
         {
           NameT name = TrajectoryElements[j]->GetName();
           if (name==currentf_name)
           {
               CurrentTE=j;
               break;
           }
         }
         break;
        }

      }


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

//bool NMazeMemory::ProcessOptions()
//{
//  // ���� ���� ��������� �����������
//  UEPtr<NTrajectoryElement> traj_el;
//  MVector<double,3> base_coords = base_TE->GetCoord();

//  CurrentLayer++;

//  UEPtr<NPulseNeuron> base_neuron = TrajectoryElements[CurrentTE]->GetComponentL<NPulseNeuron>("Neuron1", true);
//  UEPtr<NLTZone> ltzone_te = base_neuron->GetComponentL<NLTZone>("LTZone", true);
//  if(!ltzone_te)
//   return true;

//  //��������� ������� �� ������� �� ��� �������� ������ �� ����
//  UEPtr<NPulseMembrane> base_soma = base_neuron->GetComponentL<NPulseMembrane>("Soma1", true);
//  if (CurrentTE==0)
//      base_soma->NumExcitatorySynapses = OptionsNum+1; //�������������� ������ �� ������ �� - ��� ������������ ������� � ������� ������ ��
//  else
//      base_soma->NumExcitatorySynapses = OptionsNum;
//  base_soma->Reset();
//  //�� N1_D1_2
//  UEPtr<NPulseMembrane> base_dend = base_neuron->GetComponentL<NPulseMembrane>("Dendrite1_2", true);
//  base_dend->NumExcitatorySynapses = OptionsNum;
//  base_dend->Reset();


//  for(int k = 0; k<OptionsNum; k++)
//  {
//   //������������ ���������� � ��������� �����
//   double x = (double)CurrentLayer*LayerShift + base_coords[0];
//   double y = (double)k*yShift + base_coords[1];
//   MVector<double,3> coords = {x, y, 0.0};
//   traj_el = CreatePoint(coords);//���������� ������
//   traj_el->Reset();
//   base_TE->Forwards.push_back(traj_el);
//   traj_el->Layer = CurrentLayer;

//   //������ ����� �� �������� �������� ���������� �� ������ ��� ���������
//   bool res(true);
//   //������������ ����������� �� N1_D5_syn1
//   UEPtr<NPulseNeuron> input_neuron = traj_el->GetComponentL<NPulseNeuron>("Neuron1", true);
//   UEPtr<NPulseSynapse> synapse = input_neuron->GetComponentL<NPulseSynapse>("Dendrite1_5.ExcSynapse1",true);
//   if (k>0)
//   {
//     synapse->Weight = SideWeight;
//   }
//   else
//   {
//     traj_el->CurrentForward = k;
//   }
//   res&=CreateLink(base_TE->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
//   if(!res)
//    return true;
//   //��������� ����������� �� N1_S1_syn1
//   synapse = input_neuron->GetComponentL<NPulseSynapse>("Soma1.InhSynapse1",true);
//   res&=CreateLink(base_TE->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
//   if(!res)
//    return true;
//   //��������� ����������� �� N2_S1_syn1
//   input_neuron = traj_el->GetComponentL<NPulseNeuron>("Neuron2", true);
//   synapse = input_neuron->GetComponentL<NPulseSynapse>("Soma1.InhSynapse1",true);
//   res&=CreateLink(base_TE->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
//   if(!res)
//    return true;

//   //�������� �����
//   traj_el->Backwards.push_back(base_TE);
//   traj_el->CurrentBackward = 0;
//   //�� ����
//   int syn_num;
//   if (CurrentTE==0)
//       syn_num = k+2;
//   else
//       syn_num = k+1;
//   synapse = base_soma->GetComponentL<NPulseSynapse>("ExcSynapse"+sntoa(syn_num),true);
//   synapse->Weight = 0.2;
//   res&=CreateLink(traj_el->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
//   if(!res)
//    return true;
//   //�� N1_D1_2
//   synapse = base_neuron->GetComponentL<NPulseSynapse>("Dendrite1_2.ExcSynapse"+sntoa(k+1),true);
//   synapse->Weight = 0.2;
//   res&=CreateLink(traj_el->GetLongName(this),"Output",synapse->GetLongName(this),"Input");
//   if(!res)
//    return true;

//   //����� ����� PreControl ��������� ��� "�����"
//   int passed_max = PassedTEs.size();
//   for (int j=0; j<passed_max; j++)
//   {
//    UEPtr<NMultiPositionControl> output_mpc = MultiPCs[j];
//    UEPtr<NPulseNeuron> output_postinputn = output_mpc->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
//    UEPtr<NLTZone> output_ltzone = output_postinputn->GetComponentL<NLTZone>("LTZone", true);

//    UEPtr<NMultiPositionControl> input_mpc = MultiPCs[CurrentTE+k+1];
//    UEPtr<NPulseNeuron> input_precontroln = input_mpc->GetComponentL<NPulseNeuron>("PreControlNeuron1", true);
//    UEPtr<NPulseMembrane> input_soma = input_precontroln->GetComponentL<NPulseMembrane>("Soma1", true);

//    int syn_num = input_soma->NumExcitatorySynapses;
//    for (int i = 1; i<=syn_num; i++)
//    {
//     UEPtr<NPulseSynapse> syn = input_precontroln->GetComponentL<NPulseSynapse>("Soma1.ExcSynapse"+sntoa(i),true);
//     if(!syn)
//       return true;

//     if (syn->Input.IsConnected())
//     {
//      if(syn_num==i)
//      {
//       input_soma->NumExcitatorySynapses++;
//       input_soma->Reset();
//       syn_num = input_soma->NumExcitatorySynapses;
//      }
//       continue; //������� � ���������� �������
//     }
//     else
//     {
//      res&=CreateLink(output_ltzone->GetLongName(this),"Output",syn->GetLongName(this),"Input");
//      if(!res)
//       return true;
//      break;
//     }
//    }
//   }
//  }

//  return true;
//}

}
#endif
