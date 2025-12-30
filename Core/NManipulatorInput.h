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

#ifndef NMANIPULATOR_INPUT_H
#define NMANIPULATOR_INPUT_H

#include "../../Nmsdk-PulseLib/Core/NSource.h"


namespace NMSDK {

class RDK_LIB_TYPE NManipulatorInput: public NSource
{
public: //  
//  
UProperty<double,NManipulatorInput, ptPubParameter> Voltage;

public: //   
///   
UProperty<MDMatrix<double>, NManipulatorInput, ptInput | ptPubState> Input;

public: // 
bool UpdateOutputFlag;
// --------------------------
//   
// --------------------------
NManipulatorInput(void);
virtual ~NManipulatorInput(void);
// --------------------------

protected:
// --------------------------
//    
// --------------------------
//   
bool SetVoltage(const double &value);
// --------------------------

public:
// --------------------------
//    
// --------------------------
//         
virtual NManipulatorInput* New(void);
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

