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

#ifndef NMAZEMEMORY_H
#define NMAZEMEMORY_H


#include "NTrajectoryElement.h"
#include "NMultiPositionControl.h"
#include "../../Nmsdk-PulseLib/Core/NNeuronTrainer.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"
#include "../../../Rdk/Deploy/Include/rdk.h"
#include <iostream>
#include <fstream>
#include <cstring>

namespace NMSDK {

//using namespace RDK;

// ,       
class RDK_LIB_TYPE NMazeMemory: public UNet
{
public: // 
///      (, )
UProperty<bool, NMazeMemory, ptPubParameter> Situation;

///       ( )
/// -  = -  ,
///   :
/// 1 -     , 0 -  
/// [0] -  , [1] -  , [2] -  , [3] - 
UProperty<std::vector<int>,NMazeMemory, ptPubParameter> InputActions;

///  
RDK::UProperty<string, NMazeMemory, ptPubParameter> ActionNeuronsType;

///  ,   
UProperty<int, NMazeMemory, ptPubParameter> FeaturesNum;

///    (   )
UProperty<bool, NMazeMemory, ptPubParameter> IsDone;

///   ( = {x,y,alpha},  alpha -  
UProperty<MDMatrix<double>, NMazeMemory, ptPubParameter> SituationCoords;

protected:
/// 
std::vector<UEPtr<NPulseNeuron>> ActionNeurons;

///    
std::vector<UEPtr<NTrajectoryElement>> TrajectoryElements;

///  PostInput   
std::vector<UEPtr<NTrajectoryElement>> ActivePIs;

///  MultiPC
std::vector<UEPtr<NMultiPositionControl>> MultiPCs;

///  NeuronTrainer (   )
std::vector<UEPtr<NNeuronTrainer>> NTrainers;

///     
int CurrentTE;

///   NeuronTrainer  
UEPtr<NNeuronTrainer> CurrentNT;

///   
int CurrentLayer;

///       X (   )
double LayerShift;

///       Y (   )
double yShift;

///     (   )
double SideWeight;

///  
std::vector<UEPtr<NTrajectoryElement>> PassedTEs;

///  
UEPtr<NTrajectoryElement> BaseTE;

///MultiPC,    
UEPtr<NMultiPositionControl> BaseMPC;

///  
UEPtr<NTrajectoryElement> PrevTE;

///    NeuronTrainer
///(,   NT,    )
bool IsNotFinished;

///-,
///     NeuronTrainer,
///      TE
int WaitForSpike;


///-,
///     TE,
///     PostInputNeuron
int WaitForSpikePI;


bool IsWaitingForAnswer;

///-,
///    
///  PreControl 
int WaitForAnswerCnt;


//,    
UEPtr<NTrajectoryElement> TEToDelete;
UEPtr<NMultiPositionControl> MPCToDelete;


public: // 
// --------------------------
//   
// --------------------------
NMazeMemory(void);
virtual ~NMazeMemory(void);
// --------------------------

// --------------------------
//   
// --------------------------
///      (, )
bool SetSituation(const bool &value);

///       ( )
/// -  = -  ,
///   :
/// 1 -     , 0 -  
/// [0] -  , [1] -  , [2] -  , [3] - 
bool SetInputActions(const std::vector<int> &value);

///  
bool SetActionNeuronsType(const string &value);

///  ,   
bool SetFeaturesNum(const int &value);

///    (   )
bool SetIsDone(const bool &value);

///   ( = {x,y,alpha},  alpha -  
bool SetSituationCoords(const MDMatrix<double> &value);

// --------------------------

// --------------------------
//     
// --------------------------

// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NMazeMemory* New(void);
static UComponent* NewStatic(void);
// --------------------------

// --------------------------
//    
// --------------------------
protected:
//    
//       
//      comp 
//     
virtual bool AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer=0);

//    
//       
//      comp
//    
virtual bool ADelComponent(UEPtr<UContainer> comp);
// --------------------------


// --------------------------
//    
// --------------------------
protected:

//        
virtual bool ADefault(void);

//     
//   
//    Reset()   Ready  true
//    
virtual bool ABuild(void);

//   .
virtual bool AReset(void);

//    
virtual bool ACalculate(void);

//    ( ), ..
//  TrajectoryElement+MultiPC
UEPtr<NTrajectoryElement> CreatePoint(MVector<double,3> coords);

//      ( )
//    
bool LinkPoint(UEPtr<NTrajectoryElement> traj_el,  MVector<double,3> base_coords, int option_num);

//   
//        TrajectoryElements,
//   PostInput 
//..  TE,     
bool MergingTEs(int active_num);

//     TE   
bool UpdateCurrentTE();

//,    PostInput   
int CheckActivePIs();

//,         
bool CheckActiveForwards(UEPtr<NTrajectoryElement> t_element);

//     
//(   PreControl   ,
//       )
bool CallForResponse();

// ,      , w = 0,2
bool LastUsedLink();

//   ()
bool ProcessOptions();

// 
bool DeadlockProcessing();

// --------------------------
};

}
#endif
