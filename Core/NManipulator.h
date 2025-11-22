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

#ifndef NManipulatorH
#define NManipulatorH
#include "../../../Rdk/Deploy/Include/rdk.h"

//---------------------------------------------------------------------------
namespace NMSDK {

using namespace RDK;

/* *************************************************************************** */
class RDK_LIB_TYPE NManipulator: public UNet
{
public: //  
///  
UProperty<double, NManipulator, ptPubParameter> EMFactor;

/// 
UProperty<double, NManipulator, ptPubParameter> Inductance;

/// 
UProperty<double, NManipulator, ptPubParameter> Resistance;

public: //   
///  
UProperty<MDMatrix<double>, NManipulator, ptInput | ptPubState> Input;

///  
UProperty<MDMatrix<double>, NManipulator, ptOutput | ptPubState> Output;

protected: //  
//  
double Current;

//  
UGraphics Graphics;

public: // 
// --------------------------
//   
// --------------------------
NManipulator(void);
virtual ~NManipulator(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NManipulator* New(void);
// --------------------------

protected:
// --------------------------
//    
// --------------------------
//  
bool SetEMFactor(const double &value);

// 
bool SetInductance(const double &value);

// 
bool SetResistance(const double &value);
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
