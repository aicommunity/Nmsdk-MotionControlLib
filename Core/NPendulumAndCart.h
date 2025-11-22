/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2012.
E-mail:            alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NPendulumAndCartH
#define NPendulumAndCartH

#include "../../../Rdk/Deploy/Include/rdk.h"

//---------------------------------------------------------------------------
namespace NMSDK {

using namespace RDK;

/* *************************************************************************** */
class RDK_LIB_TYPE NPendulumAndCart: public UNet
{
public: //  
///  
UProperty<double,NPendulumAndCart, ptPubParameter> CartMass;

///  
UProperty<double,NPendulumAndCart, ptPubParameter> RodMass;

///  
UProperty<double,NPendulumAndCart, ptPubParameter> RodLength;

///   X
UProperty<double,NPendulumAndCart, ptPubParameter> OutXMovement;

///  
/// 0 -   
/// 1 -   PID
UProperty<double,NPendulumAndCart, ptPubParameter> Mode;

/// Ki
UProperty<double,NPendulumAndCart, ptPubParameter> Ki;

/// Kd
UProperty<double,NPendulumAndCart, ptPubParameter> Kd;

/// Kp
UProperty<double,NPendulumAndCart, ptPubParameter> Kp;

/// Ki
UProperty<double,NPendulumAndCart, ptPubParameter> MovementKi;

/// Kd
UProperty<double,NPendulumAndCart, ptPubParameter> MovementKd;

/// Kp
UProperty<double,NPendulumAndCart, ptPubParameter> MovementKp;

///    
UProperty<double,NPendulumAndCart, ptPubParameter> AngleWeight;

///    
UProperty<double,NPendulumAndCart, ptPubParameter> MovementWeight;

public: // 

///  
UProperty<double, NPendulumAndCart, ptPubState> ExtrenalMoment;

public: //   
UProperty<MDMatrix<double>, NPendulumAndCart, ptInput | ptPubState> Input1;

UProperty<MDMatrix<double>, NPendulumAndCart, ptInput | ptPubState> Input2;

///   
UProperty<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> Acceleration;

///   
UProperty<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> Angle;

///   
UProperty<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> Speed;

///  
UProperty<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> Movement;

///  
UProperty<MDMatrix<double>, NPendulumAndCart, ptOutput | ptPubState> MovementSpeed;

protected: // 
double theta0;
double x0;
double y,x,movement;
double y0;

protected: //  

public: // 
// --------------------------
//   
// --------------------------
NPendulumAndCart(void);
//NPendulumAndCart(const string &name);
virtual ~NPendulumAndCart(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NPendulumAndCart* New(void);
// --------------------------

protected:
// --------------------------
//    
// --------------------------
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
