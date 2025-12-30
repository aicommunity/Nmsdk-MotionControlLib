// ===========================================================
// Version:        3.0.2
// ===========================================================
/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2002.
E-mail:        alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#ifndef NMANIPULATOR_SOURCE_H
#define NMANIPULATOR_SOURCE_H

#include "../../Nmsdk-PulseLib/Core/NSource.h"


namespace NMSDK {

class RDK_LIB_TYPE NManipulatorSource: public NSource
{
public: //     
///     
UProperty<double,NManipulatorSource, ptPubParameter> Angle;

///    
UProperty<double,NManipulatorSource, ptPubParameter> Speed;

///    
UProperty<double,NManipulatorSource, ptPubParameter> Force;

///    
UProperty<double,NManipulatorSource, ptPubParameter> Movement;

public: //   
/// Input angle
UProperty<MDMatrix<double>,NManipulatorSource, ptPubInput> InputAngle;

/// Input speed
UProperty<MDMatrix<double>,NManipulatorSource, ptPubInput> InputSpeed;

/// Input force
UProperty<MDMatrix<double>,NManipulatorSource, ptPubInput> InputForce;

/// Input movement
UProperty<MDMatrix<double>,NManipulatorSource, ptPubInput> InputMovement;

/// :   
UProperty<MDMatrix<double>,NManipulatorSource, ptOutput | ptPubState> OutputAngle;

/// :  
UProperty<MDMatrix<double>,NManipulatorSource, ptOutput | ptPubState> OutputSpeed;

/// :  
UProperty<MDMatrix<double>,NManipulatorSource, ptOutput | ptPubState> OutputForce;

/// :  
UProperty<MDMatrix<double>,NManipulatorSource, ptOutput | ptPubState> OutputMovement;

public: // 
bool UpdateOutputFlag;
// --------------------------
//   
// --------------------------
NManipulatorSource(void);
virtual ~NManipulatorSource(void);
// --------------------------

protected:
// --------------------------
//    
// --------------------------
//  
bool SetAngle(const double &value);

//  
bool SetSpeed(const double &value);

//  
bool SetForce(const double &value);
// --------------------------

public:
// --------------------------
//    
// --------------------------
//         
virtual NManipulatorSource* New(void);
// --------------------------

// --------------------------
//    
// --------------------------
protected:
//        
virtual bool ADefault(void);

//   .
virtual bool AReset(void);

//    
virtual bool ACalculate(void);
// --------------------------
};

}
#endif

