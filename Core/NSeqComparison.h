/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   November, 2023
/// @brief  The component for sequences comparison

#ifndef NSeqComparisonH
#define NSeqComparisonH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGeneratorMulti.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuron.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NSeqComparison: public UNet
{
public: //  

 ///   
 UProperty<std::string, NSeqComparison, ptPubParameter> NeuronClassName;

 ///     
 UProperty<std::string, NSeqComparison, ptPubParameter> MultiGeneratorClassName;


 ///   ()
 UProperty<double, NSeqComparison, ptPubParameter> PulseLength;

 ///  
 UProperty<double, NSeqComparison, ptPubParameter> Amplitude;

 ///   ()  
 UProperty<double, NSeqComparison, ptPubParameter> Frequency;

 ///     
 UProperty<double, NSeqComparison, ptPubParameter> LTZThreshold;


 ///      
 UProperty<int, NSeqComparison, ptPubParameter> PulseCount;

 ///    
 UProperty<int, NSeqComparison, ptPubParameter> NumClasses;

 /// ,     
 UProperty<MDMatrix<double>, NSeqComparison, ptPubParameter> DelaysKFCls;

 /// ,     
 UProperty<MDMatrix<double>, NSeqComparison, ptPubParameter> DelaysFrCls;


public: //   

///  
UProperty<MDMatrix<double>, NSeqComparison, ptOutput | ptPubState> Output;


protected: // 
 ///     
 std::vector<UEPtr <NPulseGeneratorMulti>> KFClsSpikes;

 ///     
 std::vector<UEPtr <NPulseGeneratorMulti>> FrClsSpikes;

 ///  
 UEPtr <NPulseNeuron> CompNeuron;

 ///     ,    
 int OldNumClasses;

protected: //  


public: // 
// --------------------------
//   
// --------------------------
NSeqComparison(void);
virtual ~NSeqComparison(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NSeqComparison* New(void);
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
bool SetMultiGeneratorClassName(const std::string &value);

///   
bool SetPulseLength(const double &value);

///   
bool SetAmplitude(const double &value);

///    ()  
bool SetFrequency(const double &value);

///      
bool SetLTZThreshold(const double &value);

///    
bool SetPulseCount(const int &value);

///     
bool SetNumClasses(const int &value);

///  ,     
bool SetDelaysKFCls(const MDMatrix<double> &value);

///  ,     
bool SetDelaysFrCls(const MDMatrix<double> &value);

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
