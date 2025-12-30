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
#ifndef NSimpleStatisticH
#define NSimpleStatisticH

#include "../../../Rdk/Deploy/Include/rdk.h"

namespace NMSDK {

using namespace RDK;

class RDK_LIB_TYPE NSimpleStatistic: public UNet
{
public: // 
//      
//     ,    
UProperty<UTime,NSimpleStatistic, ptPubParameter> StatsInterval;

//   
// 0 -  ,   
// < > Min(PInputData[0]->Double[0]) Max(PInputData[0]->Double[0]) Avg(PInputData[0]->Double[0]) ... Min(PInputData[1]->Double[0]) Max(PInputData[1]->Double[0]) Avg(PInputData[1]->Double[0])
// 1 -       ()
// < >
//   PInputData[0]...
//   PInputData[1]...
// ... ..
// 2 -       
// < > PInputData[0]->Double[0] PInputData[0]->Double[1] ... PInputData[1]->Double[0] PInputData[1]->Double[1] ...
UProperty<int,NSimpleStatistic, ptPubParameter> Mode;

//  /
UProperty<vector<string>,NSimpleStatistic, ptPubParameter> Headers;

public: //   
UProperty<std::vector<MDMatrix<double>>, NSimpleStatistic, ptInput | ptPubState> Inputs;

UProperty<MDMatrix<double>, NSimpleStatistic, ptOutput | ptPubState> Output;

protected: // 
//  
int StatsNumber;

protected: //  
//  
fstream* StatsFile;

//      
double StatsStartTime;

//   
vector<vector<double> > StatsMin;
vector<vector<double> > StatsMax;
vector<vector<double> > StatsAvg;
vector<vector<double> > StatsDelta;



public: // 
// --------------------------
//   
// --------------------------
NSimpleStatistic(void);
virtual ~NSimpleStatistic(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NSimpleStatistic* New(void);
// --------------------------

// --------------------------
// Computation methods
// --------------------------
//   ,  
bool ReCreateFile(void);
// --------------------------

// --------------------------
// Proctected computation methods
// --------------------------
protected:
//        
virtual bool ADefault(void);

//     
//   
//    Reset()   Ready  true
//    
virtual bool ABuild(void);

// Reset computation
virtual bool AReset(void);

// Execute math. computations of current object on current step
virtual bool ACalculate(void);
// --------------------------

// --------------------------
//    
// --------------------------
protected:
//   
void ClearStats(void);

//      
void ResizeStats(void);
// --------------------------

};

}

#endif
