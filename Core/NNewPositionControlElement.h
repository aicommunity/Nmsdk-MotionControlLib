/* ***********************************************************

E-mail:        
Url:           

This file is part of the project: 

File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NNewPositionControlElement_H
#define NNewPositionControlElement_H

#include "NEngineMotionControl.h"
#include "NMotionElement.h"
#include "NPositionControlElement.h"

namespace NMSDK {

class NEngineMotionControl;
class RDK_LIB_TYPE NNewPositionControlElement: public NPositionControlElement
{
public: // 
RDK::UProperty<MDMatrix<double>,NNewPositionControlElement, ptPubParameter> MotionControl;
RDK::UProperty<bool, NNewPositionControlElement, ptPubParameter> SimControl;
//RDK::UProperty<bool, NNewPositionControlElement, ptPubState> RememberState;
//RDK::UProperty<MDMatrix<double>, NNewPositionControlElement, ptPubState> CurrentPosition;
//RDK::UProperty<MDMatrix<double>, NNewPositionControlElement> TargetPosition;
//RDK::UProperty<string, NNewPositionControlElement, ptPubParameter> InputNeuronType;
//RDK::UProperty<string, NNewPositionControlElement, ptPubParameter> ControlNeuronType;
//RDK::UProperty<bool, NNewPositionControlElement, ptPubParameter> ExternalControl;
//RDK::UProperty<MDMatrix<double>, NNewPositionControlElement, ptPubState> Delta;

public: //  
//        
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > LeftInputNeurons;

//        
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > RightInputNeurons;

//        
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > LeftControlNeurons;

//        
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > RightControlNeurons;

//        
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > LeftPostInputNeurons;

//        
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > RightPostInputNeurons;

//        
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > LeftPreControlNeurons;

//        
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > RightPreControlNeurons;


//vector<UNet*> Generators;

//     
// G(0,0) G(0,1), ..., G(0,M) -    0 
// G(1,0) G(1,1), ..., G(1,M) -    1 
vector<vector<UNet*> > LeftGenerators;

//     
// G(0,0) G(0,1), ..., G(0,M) -    0 
// G(1,0) G(1,1), ..., G(1,M) -    1 
vector<vector<UNet*> > RightGenerators;

//
//    MultiPositionControl
UProperty<MDMatrix<double>, NNewPositionControlElement, ptPubOutput> Output;

UEPtr<NEngineMotionControl> MotionControlElement;

public: // 
// --------------------------
//   
// --------------------------
NNewPositionControlElement(void);
virtual ~NNewPositionControlElement(void);
// --------------------------

// ---------------------
//   
// ---------------------
bool SetInputNeuronType(const string &value);
bool SetControlNeuronType(const string &value);
// ---------------------

// ---------------------
//    
// ---------------------
// ---------------------

// --------------------------
//    
// --------------------------
//         
virtual NNewPositionControlElement* New(void);
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
bool CreateNeurons(void);
bool CreateExternalControlElements(void);
bool LinkNeurons(vector <NNet*> start, vector <NNet*> finish);
bool LinkNeuronsNeg(vector <NNet*> start, vector <NNet*> finish);
bool UnlinkNeurons(vector <NNet*> start, vector <NNet*> finish);
virtual bool LinkGenerators(const bool &value);
bool LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link, bool is_sim);
bool LinkNegative(vector <NNet*> start, vector <NNet*> finish);
public:
vector<NNet*> GetInputNeurons(void);
vector<NNet*> GetControlNeurons(void);
};

}

#endif



