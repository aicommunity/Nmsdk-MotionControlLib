/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   December, 2023
/// @brief  The component for determining the direction of movement of the control object

#ifndef NActuatorSignalsH
#define NActuatorSignalsH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../Nmsdk-PulseLib/Core/NPulseDelay.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NActuatorSignals: public UNet
{
public: //  

 ///   
 UProperty<std::string, NActuatorSignals, ptPubParameter> NeuronClassName;

 ///    
 UProperty<std::string, NActuatorSignals, ptPubParameter> GeneratorClassName;

 ///     
 UProperty<std::string, NActuatorSignals, ptPubParameter> DelayClassName;

 ///   ()
 UProperty<double, NActuatorSignals, ptPubParameter> PulseLength;

 ///  
 UProperty<double, NActuatorSignals, ptPubParameter> Amplitude;

 ///   ()  
 UProperty<double, NActuatorSignals, ptPubParameter> Frequency;

 ///    
 UProperty<double, NActuatorSignals, ptPubParameter> LeftDelay;

 ///    
 UProperty<double, NActuatorSignals, ptPubParameter> RightDelay;

 ///   ,     
 UProperty<double, NActuatorSignals, ptPubParameter> Sinchro1Delay;

 ///   ,     
 UProperty<double, NActuatorSignals, ptPubParameter> Sinchro2Delay;

 ///         
 UProperty<bool, NActuatorSignals, ptPubParameter> UseTransitEngineSignal;

 ///          
 UProperty<bool, NActuatorSignals, ptPubParameter> UseTransitSinchroSignal;


public: //   

///  ,   
UProperty<MDMatrix<double>, NActuatorSignals, ptOutput | ptPubState> Stay;

///  ,    
UProperty<MDMatrix<double>, NActuatorSignals, ptOutput | ptPubState> Forward;

///  ,    
UProperty<MDMatrix<double>, NActuatorSignals, ptOutput | ptPubState> Back;

///  ,    
UProperty<MDMatrix<double>, NActuatorSignals, ptOutput | ptPubState> Left;

///  ,    
UProperty<MDMatrix<double>, NActuatorSignals, ptOutput | ptPubState> Right;


protected: // 

 ///    
 UEPtr <NPulseGeneratorTransit> LeftEngine;

 ///    
 UEPtr <NPulseGeneratorTransit> RightEngine;

 ///    1 -   
 UEPtr <NPulseGeneratorTransit> Sinchro1;

 ///    2 -   
 UEPtr <NPulseGeneratorTransit> Sinchro2;

 ///    NOTNeuron
 UEPtr <NPulseGeneratorTransit> NOTGenerator;


 ///  
 UEPtr <NPulseDelay> Delay1To2;

 ///  
 UEPtr <NPulseNeuron> ORNeuron;

 ///  
 UEPtr <NPulseNeuron> NOTNeuron;


 /// ,   
 UEPtr <NPulseNeuron> IsForwardNeuron;

 /// ,   
 UEPtr <NPulseNeuron> IsLeftNeuron;

 /// ,   
 UEPtr <NPulseNeuron> IsRightNeuron;

 /// ,   
 UEPtr <NPulseNeuron> IsBackNeuron;

 ///  ,   
 UEPtr <NPulseNeuron> StayNeuron;

 ///  ,    
 UEPtr <NPulseNeuron> ForwardNeuron;

 ///  ,    
 UEPtr <NPulseNeuron> LeftNeuron;

 ///  ,    
 UEPtr <NPulseNeuron> RightNeuron;

 ///  ,    
 UEPtr <NPulseNeuron> BackNeuron;


protected: //  


public: // 
// --------------------------
//   
// --------------------------
NActuatorSignals(void);
virtual ~NActuatorSignals(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NActuatorSignals* New(void);
// --------------------------

protected:
 // --------------------------
 //    
 // --------------------------

 ///    
 ///       
 //      comp      
 virtual bool AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer = 0);

 ///    
 ///       
 //      comp    
 virtual bool ADelComponent(UEPtr<UContainer> comp);

 // --------------------------

protected:
// --------------------------
//    
// --------------------------

///    
bool SetNeuronClassName(const std::string &value);

///     
bool SetGeneratorClassName(const std::string &value);

///      
bool SetDelayClassName(const std::string &value);

///   
bool SetPulseLength(const double &value);

///   
bool SetAmplitude(const double &value);

///    ()  
bool SetFrequency(const double &value);

///     
bool SetLeftDelay(const double &value);

///     
bool SetRightDelay(const double &value);

///    ,     
bool SetSinchro1Delay(const double &value);

///    ,     
bool SetSinchro2Delay(const double &value);

///         
bool SetUseTransitEngineSignal(const bool &value);

///          
bool SetUseTransitSinchroSignal(const bool &value);

// --------------------------

// --------------------------
// Computation methods
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

// Execute math. computations of current object on current step
virtual bool ACalculate(void);
// --------------------------
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
