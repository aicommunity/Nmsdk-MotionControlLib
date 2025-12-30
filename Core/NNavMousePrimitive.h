// ===========================================================
// Version:        1.0.0
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2009.
E-mail:			alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NNavMousePrimitiveH
#define NNavMousePrimitiveH
#include "../../../Rdk/Deploy/Include/rdk.h"

//---------------------------------------------------------------------------
namespace NMSDK {

using namespace RDK;

/* *************************************************************************** */
class RDK_LIB_TYPE NNavMousePrimitive: public UNet
{
public: //  
///    "",   (0)   (PoleSize)
///    ,     
UProperty<double, NNavMousePrimitive, ptPubParameter> PoleSize;

///  "" (""  )
UProperty<double, NNavMousePrimitive, ptPubParameter> MouseSize;

///   "",    ""   
///      
UProperty<double, NNavMousePrimitive, ptPubParameter> VibrissSize;

///    1 ""  (..  Freqiency)
UProperty<double, NNavMousePrimitive, ptPubParameter> Velocity;

/// ,       
UProperty<double, NNavMousePrimitive, ptPubParameter> Frequency;

///  ,     
/// (0.1        )
UProperty<double, NNavMousePrimitive, ptPubParameter> Delay;

/// ,     
/// (0.1 -     0.2  )
UProperty<double, NNavMousePrimitive, ptPubParameter> PainDelay;

///     -  
///   - 0.001
UProperty<double, NNavMousePrimitive, ptPubParameter> PulseLength;

///      (  - )
UProperty<bool, NNavMousePrimitive, ptPubParameter> UseExternalInput;

/// 
UProperty<double, NNavMousePrimitive, ptPubParameter> Amplitude;

///   -    :
/// 0 -   
/// 1 -     Velocity
/// -1 -     Velocity
///   -  
UProperty<int, NNavMousePrimitive, ptPubParameter> MotionControlSimple;

public: //   
///  
UProperty<MDMatrix<double>, NNavMousePrimitive, ptInput | ptPubState> Input;

///  ,       
///    , 
UProperty<MDMatrix<double>, NNavMousePrimitive, ptInput | ptPubState> MotionControlSpikeForward;
UProperty<MDMatrix<double>, NNavMousePrimitive, ptInput | ptPubState> MotionControlSpikeBackward;
UProperty<MDMatrix<double>, NNavMousePrimitive, ptInput | ptPubState> MotionControlSpikeStop;

///     
///  0 -   
///  1 -     Velocity
/// -1 -     Velocity
UProperty<int, NNavMousePrimitive, ptPubState> MotionControlState;

///  -         
UProperty<int, NNavMousePrimitive, ptPubState> PainState;

///  ""  ""
UProperty<double, NNavMousePrimitive, ptPubState> MousePosition;

///  
UProperty<MDMatrix<double>, NNavMousePrimitive, ptOutput | ptPubState> Output;

///   
///      Frequency,    Delay
///        ,    
UProperty<MDMatrix<double>, NNavMousePrimitive, ptOutput | ptPubState> VibrissOutput;

///   
UProperty<MDMatrix<double>, NNavMousePrimitive, ptOutput | ptPubState> PainOutput;

protected: //  

//  ,     
std::vector<double> VibrDelays;

//       ,  
double ForwardSpikeTime;
double BackwardSpikeTime;
double StopSpikeTime;

// ,        
double start_iter_time;

double OldFrequency;
double ResetTime;

//   
std::vector<int> vibriss_counters;
int pain_counter;

//  
// TODO: (-     )
UGraphics Graphics;

public: // 
// --------------------------
//   
// --------------------------
NNavMousePrimitive(void);
virtual ~NNavMousePrimitive(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NNavMousePrimitive* New(void);
// --------------------------

protected:
// --------------------------
//    
// --------------------------

bool SetPoleSize(const double &value);
bool SetVibrissSize(const double &value);
bool SetMouseSize(const double &value);
bool SetVelocity(const double &value);

// --------------------------
//  
// --------------------------
//        
virtual bool ADefault(void);

//     
//   
//    Reset()   Ready  true
//    
virtual bool ABuild(void);

// Reset computation
virtual bool AReset(void);

//    .  :
// 1)      
// 2)  ,   
// 3) ,        
virtual bool UpdateState(void);

// Execute math. computations of current object on current step
virtual bool ACalculate(void);
// --------------------------
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
