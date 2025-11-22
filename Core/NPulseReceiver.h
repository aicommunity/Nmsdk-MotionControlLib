/* *********************************************************** 
@Copyright Alexander V. Bakhshiev, 2012.
E-mail:        alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: RDK

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NPulseReceiverH
#define NPulseReceiverH

#include "../../Nmsdk-PulseLib/Core/NReceiver.h"

//---------------------------------------------------------------------------
namespace NMSDK {

/* *************************************************************************** */
class RDK_LIB_TYPE NPulseReceiver: public NReceiver
{
public: //  
//   
UProperty<double, NPulseReceiver, ptPubParameter> MaxAccumulationRange;

protected: // 
UProperty<std::vector<MDMatrix<double>>, NPulseReceiver, ptInput | ptPubState> Inputs;

UProperty<MDMatrix<double>, NPulseReceiver, ptOutput | ptPubState> Output;

protected: //  
std::vector<std::list<double> > Data;
std::vector<std::list<double> > MatrixData;

std::vector<bool> PulseFlag;

public: // 
// --------------------------
//   
// --------------------------
NPulseReceiver(void);
virtual ~NPulseReceiver(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NPulseReceiver* New(void);
// --------------------------

protected:
// --------------------------
//    
// --------------------------
//   
bool SetMaxAccumulationRange(const double &value);
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
