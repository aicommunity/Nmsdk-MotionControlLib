/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2011.
E-mail:            alexab@ailab.ru
url:            http://ailab.ru
Version:        1.0.0

This file - part of the project: NMSDK

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NNETWORK_LINKS_STATISTIC_CPP
#define NNETWORK_LINKS_STATISTIC_CPP

#include <iomanip>
#include "NNetworkLinksStatistic.h"
//#include "../../Nmsdk-PulseLib/Core/NPulseLTZone.h"
//#include "../../Nmsdk-PulseLib/Core/NPulseHebbSynapse.h"

namespace NMSDK {

// --------------------------
// ������������ � �����������
// --------------------------
NNetworkLinksStatistic::NNetworkLinksStatistic(void)
: StatsInterval("StatsInterval",this),
  Mode("Mode",this),
  Inputs("Inputs",this)
{
 StatsNumber=0;
 Mode=0;
 StatsStartTime=0;
 StatsInterval=0;
}

NNetworkLinksStatistic::~NNetworkLinksStatistic(void)
{
// StatsFile.SaveToFile(GetName()+string("_")+RDK::sntoa(StatsNumber++,5)+".ini");
}
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
NNetworkLinksStatistic* NNetworkLinksStatistic::New(void)
{
 return new NNetworkLinksStatistic;
}
// --------------------------


// --------------------------
// Computation methods
// --------------------------
// ��������� ����� ����, �������� ����������
bool NNetworkLinksStatistic::ReCreateFile(void)
{
 StatsFile.SaveToFile(GetName()+string("_")+RDK::sntoa(StatsNumber++,5)+".ini");
 StatsFile.Delete();

 return true;
}
// --------------------------

// --------------------------
// Computation methods
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool NNetworkLinksStatistic::ADefault(void)
{
 StatsInterval=0.1;
 Mode=0;

 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool NNetworkLinksStatistic::ABuild(void)
{
 return true;
}

// Reset computation
bool NNetworkLinksStatistic::AReset(void)
{
 ClearStats();
 StatsStartTime=Environment->GetTime().GetTime();
 return true;
}

// Execute math. computations of current object on current step
bool NNetworkLinksStatistic::ACalculate(void)
{
 // ���������, �������� ��� ��� ���� ��������� ����������
 if(Environment->GetTime().GetTime()-StatsStartTime>=StatsInterval)
 {
  UNet *network=0;
  map<UConnector*,vector<pair<UItem*,double> > > ResLinks;
  for(int i=0;i<int(Inputs->size());i++)
  {
   network=dynamic_cast<UNet*>(GetCItem(i).Item);
   if(!network)
	continue;

   RDK::ULinksList linkslist;
   network->GetLinks(linkslist,network);
/*
  LinksListCIterator K,I,J,endI;
  pair<LinksListCIterator,LinksListCIterator> b;

  NAItem *item,*itemowner;
  NAConnector *ownerconnector;
  NPulseSynapse *connector;


  I=linkslist.begin();
  endI=linkslist.end();
  while(I != endI)
  {
   b=linkslist.equal_range(I->first);
   item=dynamic_cast<NAItem*>(network->GetComponentL(I->first.Id));
   if(!item)
	{
	 I=b.second;
	 continue;
	}

   for(K=b.first; K != b.second; ++K)
	{
	 connector=dynamic_cast<NPulseSynapse*>(network->GetComponentL(K->second.Id));
	 if(!connector)
	  {
	   continue;
	  }

	 if(!dynamic_cast<NPulseLTZone*>(item))
	  continue;

//	 if(!dynamic_cast<NPulseSynapse*>(connector))
//	  continue;

	 itemowner=static_cast<NAItem*>(item->GetMainOwner());
	 ownerconnector=static_cast<NAItem*>(connector->GetMainOwner());
	 if(!itemowner || !ownerconnector)
	  continue;

	 string itemname=item->GetLongName(network);
	 string connname=connector->GetLongName(network);
	 pair<NAItem*,double> link;
	 link.first=itemowner;
	 link.second=connector->GetOutputData(1).Double[0];//connector->Resistance;
	 ResLinks[ownerconnector].push_back(link);
	}
   I=b.second;
  }

  // � ������ ���������
  map<NAConnector*,vector<pair<NAItem*,double> > >::iterator rI,rJ;
  rI=ResLinks.begin();
  rJ=ResLinks.end();
  vector<UId> ids;
  vector<NameT> names;
  vector<double> weights;
  for(;rI != rJ;++rI)
  {
   ids.resize(rI->second.size());
   names.resize(rI->second.size());
   weights.resize(rI->second.size());
   for(size_t i=0;i<ids.size();i++)
   {
	ids[i]=rI->second[i].first->GetId();
	names[i]=rI->second[i].first->GetName();
	weights[i]=rI->second[i].second;
   }
   StatsFile(rI->first->GetName(),"Id",RDK::sntoa(rI->first->Id));

   {
	stringstream strstr;
	RDK::operator <<(strstr,ids);
	StatsFile(rI->first->GetName(),"SourceId",string(strstr.str()));
   }

   {
	stringstream strstr;
	RDK::operator <<(strstr,names);
	StatsFile(rI->first->GetName(),"SourceName",string(strstr.str()));
   }

   {
	stringstream strstr;
	RDK::operator <<(strstr,weights);
	StatsFile(rI->first->GetName(),"Weight",string(strstr.str()));
   }

  }
*/
 }
   ClearStats();
   StatsStartTime=Environment->GetTime().GetTime();
   StatsFile.SaveToFile(GetName()+string("_")+RDK::sntoa(StatsNumber,5)+".ini");
  }



 return true;
}
// --------------------------


// --------------------------
// ��������������� ������ ����� ����������
// --------------------------
// ���������� ������� ����������
void NNetworkLinksStatistic::ClearStats(void)
{
 // ������ ������� ����������
/* StatsMin.clear();

 StatsMax.clear();

 StatsAvg.clear();

 StatsDelta.clear();*/
}

// ������ ������� �������� ������ ������� ����������
void NNetworkLinksStatistic::ResizeStats(void)
{
/* StatsMin.resize(NumInputs);
 StatsMax.resize(NumInputs);
 StatsAvg.resize(NumInputs);
 StatsDelta.resize(NumInputs);
 for(int i=0;i<NumInputs;i++)
 {
  StatsMin[i].resize(PInputDataSize[i]);
  StatsMax[i].resize(PInputDataSize[i]);
  StatsAvg[i].resize(PInputDataSize[i]);
  StatsDelta[i].resize(PInputDataSize[i]);
 }                                     */
}
// --------------------------

}

#endif
