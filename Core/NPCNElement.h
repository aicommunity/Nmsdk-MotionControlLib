//---------------------------------------------------------------------------

#ifndef NPCNElementH
#define NPCNElementH
//---------------------------------------------------------------------------
#include "NEngineMotionControl.h"
#include "NMotionElement.h"
#include "NPositionControlElement.h"

namespace NMSDK {

//class NEngineMotionControl;
class RDK_LIB_TYPE NPCNElement: public NPositionControlElement
{
public: // 
RDK::UProperty<MDMatrix<double>,NPCNElement, ptPubParameter> MotionControl;
RDK::UProperty<bool, NPCNElement, ptPubParameter> SimControl;

public: //  

//vector<UNet*> Generators;
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
vector<vector<NNet*> > LeftAfferentNeurons;

//        
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > RightAfferentNeurons;

//        
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > LeftPostAfferentNeurons;

//        
// PIN(0,0) PIN(0,1), ..., PIN(0,M) -    0 
// PIN(1,0) PIN(1,1), ..., PIN(1,M) -    1 
vector<vector<NNet*> > RightPostAfferentNeurons;

//     
// G(0,0) G(0,1), ..., G(0,M) -    0 
// G(1,0) G(1,1), ..., G(1,M) -    1 
vector<vector<UNet*> > LeftGenerators;

//     
// G(0,0) G(0,1), ..., G(0,M) -    0 
// G(1,0) G(1,1), ..., G(1,M) -    1 
vector<vector<UNet*> > RightGenerators;


UEPtr<NEngineMotionControl> MotionControlElement;

public: // 
// --------------------------
//   
// --------------------------
NPCNElement(void);
virtual ~NPCNElement(void);
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
virtual NPCNElement* New(void);
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



