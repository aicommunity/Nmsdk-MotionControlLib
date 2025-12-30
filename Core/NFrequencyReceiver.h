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

#ifndef NFrequencyReceiverH
#define NFrequencyReceiverH
#include "../../Nmsdk-PulseLib/Core/NReceiver.h"

//---------------------------------------------------------------------------
namespace NMSDK {

/* *************************************************************************** */
class RDK_LIB_TYPE NFrequencyReceiver: public NReceiver
{
public: //  
///   
UProperty<double, NFrequencyReceiver, ptPubParameter> LeftRange,RightRange;

///   
UProperty<double, NFrequencyReceiver, ptPubParameter> FrequencyStep;

protected: // 
UProperty<MDMatrix<double>, NFrequencyReceiver, ptPubOutput | ptState> OutputFreq;

///   
UProperty<size_t, NFrequencyReceiver, ptPubParameter> PulseCounter;

///   
///   -  
UProperty<MDMatrix<double>, NFrequencyReceiver, ptPubParameter> Input;

///   
///   -  
UProperty<MDMatrix<double>, NFrequencyReceiver, ptPubParameter> Results;

protected: //  
double PrevPulseStartTime;
double PrevPulseStopTime;

public: // 
// --------------------------
//   
// --------------------------
NFrequencyReceiver(void);
virtual ~NFrequencyReceiver(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NFrequencyReceiver* New(void);
// --------------------------

protected:
// --------------------------
//    
// --------------------------
///      
bool SetLeftRange(const double &range);

///      
bool SetRightRange(const double &range);

///    
bool SetFrequencyStep(const double &step);
// --------------------------

// --------------------------
// Watch results control methods
// --------------------------
public:
/// Return number of results points
size_t GetResultsSize(void) const;
// --------------------------

// --------------------------
// Computation methods
// --------------------------
///        
virtual bool ADefault(void);

///     
///   
///    Reset()   Ready  true
///    
virtual bool ABuild(void);

/// Reset computation
virtual bool AReset(void);

/// Execute math. computations of current object on current step
virtual bool ACalculate(void);
// --------------------------
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
