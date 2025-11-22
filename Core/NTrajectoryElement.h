/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2010.
E-mail:			alexab@ailab.ru
url:			http://ailab.ru
Version:		1.0.0

This file - part of the project: NMSDK

File License:		BSD License
Project License:	GNU GPLv3 License
See file license.txt for more information
*********************************************************** */
#ifndef NTrajectoryElementH
#define NTrajectoryElementH

#include "../../Nmsdk-PulseLib/Core/NAfferentNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {

class RDK_LIB_TYPE NTrajectoryElement: public UNet
{
public: // 
///   
UProperty<std::string, NTrajectoryElement, ptPubParameter> NeuronClassName;

///      
UProperty<MDMatrix<double>, NTrajectoryElement, ptOutput | ptPubState> Output;

/// ,     
/// (   MazeMemory,   = 0)
UProperty<int, NTrajectoryElement, ptPubParameter> Layer;

//Input_u_top, Input_u_tcn  Input_y_pcn2 -          ( ),
//     NTrajectoryElement   .
// , ..      Input   Input .
//  ,     -        
//Input_u_top, Input_u_tcn  Input_y_pcn2   

///       (
//UProperty<MDMatrix<double>,NTrajectoryElement, ptInput | ptPubState> Input_u_top;

///   c   
//UProperty<MDMatrix<double>,NTrajectoryElement, ptInput | ptPubState> Input_u_tcn;

///        - PCN2 ( NMultiPositionControl),
///      
//UProperty<MDMatrix<double>,NTrajectoryElement, ptInput | ptPubState> Input_y_pcn2;

///      
/// (    )
///      MazeMemory
std::vector<UEPtr<NTrajectoryElement>> Forwards;

///   
/// ( )
///     MazeMemory
//int CurrentForward;

///    
///     MazeMemory
std::vector<UEPtr<NTrajectoryElement>> Backwards;

///   
///     MazeMemory
//int CurrentBackward;

///    
///     MazeMemory
std::vector<UEPtr<NPulseSynapse>> Paths;

///   (  )
///     MazeMemory
int LastUsedPath;

///,      TE  
///     MazeMemory
//int UsedPath;




protected:
///   
std::vector<UEPtr<NPulseNeuron>> Neurons;

///   1  2
int SomaSize;

///   1
std::vector<int> DendSizes1;

///   2
std::vector<int> DendSizes2;





public: // 
// --------------------------
//   
// --------------------------
NTrajectoryElement(void);
virtual ~NTrajectoryElement(void);
// --------------------------

// --------------------------
//   
// --------------------------
///  ,  
bool SetNeuronClassName(const std::string &value);

/// ,     
/// (   MazeMemory,   = 0)
bool SetLayer(const int &value);


// --------------------------

// --------------------------
//    

// --------------------------
public:
//         
virtual NTrajectoryElement* New(void);
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

};

}
//---------------------------------------------------------------------------
#endif
