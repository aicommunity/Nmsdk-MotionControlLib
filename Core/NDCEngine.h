// ===========================================================
// Version:        1.0.1
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

#ifndef NDCEngineH
#define NDCEngineH

#include "../../../Rdk/Deploy/Include/rdk.h"

//---------------------------------------------------------------------------
namespace NMSDK {

using namespace RDK;

/* *************************************************************************** */
class RDK_LIB_TYPE NDCEngine: public UNet
{
public: //  
///  
UProperty<double, NDCEngine, ptPubParameter> EMFactor;

/// 
UProperty<double, NDCEngine, ptPubParameter> Inductance;

/// 
UProperty<double, NDCEngine, ptPubParameter> Resistance;

///   
UProperty<double, NDCEngine, ptPubParameter> Tm;

///  
UProperty<double, NDCEngine, ptPubParameter> ReductionRate;

///  
UProperty<double, NDCEngine, ptPubParameter> OutMoment;

public: //   
///  
UProperty<MDMatrix<double>, NDCEngine, ptInput | ptPubState> InputVoltage;

///    
UProperty<MDMatrix<double>, NDCEngine, ptInput | ptPubState> InputMomentum;

///  
UProperty<MDMatrix<double>, NDCEngine, ptOutput | ptPubState> OutputMomentum;

///    
UProperty<MDMatrix<double>, NDCEngine, ptOutput | ptPubState> OutputAngle;

///    
UProperty<MDMatrix<double>, NDCEngine, ptOutput | ptPubState> OutputAngleSpeed;

protected: // 
//  
UProperty<double,NDCEngine, ptPubParameter> Angle;

//  
double Current;

//  
double EMF;

//   
double Moment;

//  
double AngleSpeed;

//  
double DiffMoment;

protected: //  

public: // 
// --------------------------
//   
// --------------------------
NDCEngine(void);
virtual ~NDCEngine(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NDCEngine* New(void);
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

//   
bool SetTm(const double &value);

//  
bool SetReductionRate(const double &value);
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
