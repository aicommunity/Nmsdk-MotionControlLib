/* ***********************************************************
File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NManipulatorAndGyro_H
#define NManipulatorAndGyro_H

#include "../../../Rdk/Deploy/Include/rdk.h"

namespace NMSDK {

using namespace RDK;

class RDK_LIB_TYPE NManipulatorAndGyro: public UNet
{
public: // 
UProperty<double, NManipulatorAndGyro, ptPubParameter> Mass;

UProperty<double, NManipulatorAndGyro, ptPubParameter> Length;

public: //   
///  
UProperty<MDMatrix<double>, NManipulatorAndGyro, ptInput | ptPubState> InputMomentumExt;

///    
UProperty<MDMatrix<double>, NManipulatorAndGyro, ptInput | ptPubState> InputMomentum;

///    
UProperty<MDMatrix<double>, NManipulatorAndGyro, ptInput | ptPubState> InputAngle;

UProperty<MDMatrix<double>, NManipulatorAndGyro, ptOutput | ptPubState> Output;


protected: //  
 double gravity_constant;

 public: // 
// --------------------------
//   
// --------------------------
NManipulatorAndGyro(void);
virtual ~NManipulatorAndGyro(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NManipulatorAndGyro* New(void);
// --------------------------

// --------------------------
//     
// --------------------------
protected:
//        
virtual bool ADefault(void);

//     
//   
//    Reset()   Ready  true
//    
virtual bool ABuild(void);

//      
virtual bool AReset(void);

//    
virtual bool ACalculate(void);
// --------------------------
};
}
#endif
