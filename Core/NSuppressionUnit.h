/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   October, 2023
/// @brief  A component suppress input pulses in time from Delay1 to Delay2

#ifndef NSuppressionUnitH
#define NSuppressionUnitH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NSuppressionUnit: public UNet
{
public: //  

 ///    
 UProperty<std::string, NSuppressionUnit, ptPubParameter> PulseGeneratorClassName;

 ///   
 UProperty<std::string, NSuppressionUnit, ptPubParameter> NeuronClassName;

 ///   
 UProperty<std::string, NSuppressionUnit, ptPubParameter> SynapseClassName;


 ///   ()
 UProperty<double, NSuppressionUnit, ptPubParameter> PulseLength;

 ///  
 UProperty<double, NSuppressionUnit, ptPubParameter> Amplitude;

 ///   ()   
 UProperty<double, NSuppressionUnit, ptPubParameter> SuppressionFreq;

 ///     
 UProperty<double, NSuppressionUnit, ptPubParameter> LTZThreshold;


 ///   1,     
 UProperty<double, NSuppressionUnit, ptPubParameter> Delay1;

 ///   2,     
 UProperty<double, NSuppressionUnit, ptPubParameter> Delay2;


 ///   ()   
 UProperty<double, NSuppressionUnit, ptPubParameter> SourceFreq;

 ///    
 UProperty<double, NSuppressionUnit, ptPubParameter> SourceDelay;

 ///        ,
 ///          
 UProperty<bool, NSuppressionUnit, ptPubParameter> TransitInput;

 ///          
 ///       
 UProperty<bool, NSuppressionUnit, ptPubParameter> OnlyInhibition;

 ///          
 UProperty<bool, NSuppressionUnit, ptPubParameter> SingleUse;

public: //   
///    
UProperty<MDMatrix<double>, NSuppressionUnit, ptInput | ptPubState> Input;

///  
UProperty<MDMatrix<double>, NSuppressionUnit, ptOutput | ptPubState> Output;


protected: // 
 ///   
 UEPtr <NPulseGeneratorTransit> SourceGenerator;

 ///  
 std::vector<UEPtr<NPulseGeneratorTransit>> DelayGenerators;

 ///  
 UEPtr <NPulseNeuron> ORNeuron;

 ///   
 UEPtr <NPulseGeneratorTransit> ControlledGenerator;

 /// 
 UEPtr <NPulseNeuron> Neuron;

protected: //  


public: // 
// --------------------------
//   
// --------------------------
NSuppressionUnit(void);
virtual ~NSuppressionUnit(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NSuppressionUnit* New(void);
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
bool SetPulseGeneratorClassName(const std::string &value);

///    
bool SetNeuronClassName(const std::string &value);

///    
bool SetSynapseClassName(const std::string &value);

///     
bool SetSuppressionFreq(const double &value);

///   
bool SetPulseLength(const double &value);

///   
bool SetAmplitude(const double &value);

///      
bool SetLTZThreshold(const double &value);

///    1,     
bool SetDelay1(const double &value);

///    2,     
bool SetDelay2(const double &value);

///      (  )
bool SetSourceFreq(const double &value);

///     
bool SetSourceDelay(const double &value);

///       
bool SetTransitInput(const bool &value);

///       
bool SetOnlyInhibition(const bool &value);

///      
bool SetSingleUse(const bool &value);

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
