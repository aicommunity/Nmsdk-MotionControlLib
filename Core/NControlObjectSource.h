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

#ifndef NCONTROL_OBJECT_SOURCE_H
#define NCONTROL_OBJECT_SOURCE_H

#include "../../Nmsdk-PulseLib/Core/NSource.h"


namespace NMSDK {

class RDK_LIB_TYPE NControlObjectSource: public NSource
{
public: //  
///    
///   -   ,    -  
///  
UProperty<MDVector<int>,NControlObjectSource,ptPubParameter> DataIndexes;

///     
UProperty<MDVector<double>,NControlObjectSource,ptPubParameter> DataShift;

///   
UProperty<MDVector<double>,NControlObjectSource,ptPubParameter> DataMul;

public: //   
///  
UProperty<MDMatrix<double>,NControlObjectSource, ptPubInput> Input;

public: // 
bool UpdateOutputFlag;
// --------------------------
//   
// --------------------------
NControlObjectSource(void);
virtual ~NControlObjectSource(void);
// --------------------------

protected:
// --------------------------
//    
// --------------------------
//  
bool SetDataShift(const MDVector<double> &value);
// --------------------------

public:
// --------------------------
//    
// --------------------------
//         
virtual NControlObjectSource* New(void);
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

