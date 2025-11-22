/* ***********************************************************

E-mail:        
Url:           

This file is part of the project: 

File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NPositionControlElement_H
#define NPositionControlElement_H

#include "NEngineMotionControl.h"
#include "NMotionElement.h"

namespace NMSDK {

//class NEngineMotionControl;
class RDK_LIB_TYPE NPositionControlElement: public UNet
{
public: // 
RDK::UProperty<MDMatrix<double>, NPositionControlElement, ptPubState> CurrentPosition;
RDK::UProperty<MDMatrix<double>, NPositionControlElement, ptPubParameter> TargetPosition;
RDK::UProperty<string, NPositionControlElement, ptPubParameter> InputNeuronType;
RDK::UProperty<string, NPositionControlElement, ptPubParameter> ControlNeuronType;
RDK::UProperty<bool, NPositionControlElement, ptPubParameter> ExternalControl;
RDK::UProperty<bool, NPositionControlElement, ptPubState> RememberState;
RDK::UProperty<MDMatrix<double>, NPositionControlElement, ptPubState> Delta;

public: //  
vector<NNet*> InputNeurons;
vector<NNet*> ControlNeurons;
vector<NNet*> PreControlNeurons;

vector<NNet*> PostInputNeurons;

vector<UNet*> Generators;

public: // 
// --------------------------
//   
// --------------------------
NPositionControlElement(void);
virtual ~NPositionControlElement(void);
// --------------------------

// ---------------------
//   
// ---------------------
bool SetInputNeuronType(const string &value);
bool SetControlNeuronType(const string &value);
bool SetExternalControl(const bool &value);
// ---------------------

// ---------------------
//    
// ---------------------
// ---------------------

// --------------------------
//    
// --------------------------
//         
virtual NPositionControlElement* New(void);
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
template <typename T>
void SetPositions(vector<T*>& values, int x_base, int x_shift, int y_base, int y_shift);
public:
virtual bool CreateNeurons(void);
virtual bool CreateExternalControlElements(void);
virtual bool LinkNeurons(vector <NNet*> start, vector <NNet*> finish);
virtual bool UnlinkNeurons(vector <NNet*> start, vector <NNet*> finish);
virtual bool LinkGenerators(const bool &value);
virtual bool LinkGenerators(vector <UNet*> generators, vector <NNet*> neurons, bool link, bool is_sim);
virtual bool LinkNegative(vector <NNet*> start, vector <NNet*> finish);
virtual bool PositionNeurons();
vector<NNet*> GetInputNeurons(void);
vector<NNet*> GetControlNeurons(void);
};

}

#endif



