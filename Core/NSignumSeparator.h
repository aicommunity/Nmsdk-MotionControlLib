// ===========================================================
// Version:        1.0.1
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2009.
E-mail:		alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NSIGNUMSEPARATOR_H
#define NSIGNUMSEPARATOR_H

#include "../../../Rdk/Deploy/Include/rdk.h"


namespace NMSDK {

using namespace RDK;

class RDK_LIB_TYPE NSignumSeparator: public UNet
{
public: //  
///  
///    +-1
UProperty<vector<double>,NSignumSeparator, ptPubParameter> Sign;

/// Gain
UProperty<vector<double>,NSignumSeparator, ptPubParameter> Gain;

///  
UProperty<MDMatrix<double>,NSignumSeparator, ptInput | ptPubState> Input;

///  
UProperty<MDMatrix<double>,NSignumSeparator, ptOutput | ptPubState> Output;

public: // 
// --------------------------
//   
// --------------------------
NSignumSeparator(void);
virtual ~NSignumSeparator(void);
// --------------------------

protected:
// --------------------------
//    
// --------------------------
///  
bool SetSign(const double &value);

///   
bool SetGain(const double &value);
// --------------------------

public:
// --------------------------
//    
// --------------------------
///         
virtual NSignumSeparator* New(void);
// --------------------------

// --------------------------
//    
// --------------------------
protected:
///        
virtual bool ADefault(void);

///     
///   
///    Reset()   Ready  true
///    
virtual bool ABuild(void);

///   .
virtual bool AReset(void);

///    
virtual bool ACalculate(void);
// --------------------------
};

}
#endif
