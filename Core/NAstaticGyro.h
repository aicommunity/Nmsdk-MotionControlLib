/* ***********************************************************
File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NAstaticGyro_H
#define NAstaticGyro_H

#include "../../../Rdk/Deploy/Include/rdk.h"

namespace NMSDK {

using namespace RDK;

class RDK_LIB_TYPE NAstaticGyro: public UNet
{
public: //  

//    
RDK::UProperty<double, NAstaticGyro, ptPubParameter> natural_freq;
// . 
RDK::UProperty<double, NAstaticGyro, ptPubParameter> z_g;

RDK::UProperty<double, NAstaticGyro,ptPubState> alpha;
RDK::UProperty<double, NAstaticGyro,ptPubState> betta;
RDK::UProperty<double, NAstaticGyro,ptPubState> gamma;

public: //   
UProperty<MDMatrix<double>, NAstaticGyro, ptInput | ptPubState> Input1;
UProperty<MDMatrix<double>, NAstaticGyro, ptInput | ptPubState> Input2;
UProperty<MDMatrix<double>, NAstaticGyro, ptInput | ptPubState> Input3;

UProperty<MDMatrix<double>, NAstaticGyro, ptOutput | ptPubState> Output1;
UProperty<MDMatrix<double>, NAstaticGyro, ptOutput | ptPubState> Output2;
UProperty<MDMatrix<double>, NAstaticGyro, ptOutput | ptPubState> Output3;

//
protected: // 

double x11_prev;
double x12_prev;
double x21_prev;
double x22_prev;

double y11_prev;
double y12_prev;
double y21_prev;
double y22_prev;

double z11_prev;
double z12_prev;
double z21_prev;
double z22_prev;

public: // 
// --------------------------
//   
// --------------------------
NAstaticGyro(void);
virtual ~NAstaticGyro(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NAstaticGyro* New(void);
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
