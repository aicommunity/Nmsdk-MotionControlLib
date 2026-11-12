/// @author Barymova Elizaveta (elizaveta.barymova@gmail.com)
/// @date   October, 2023
/// @brief  Counter of impulses. Generate output impulse only after getting of MaxCount impulses

#ifndef NCounterNeuronH
#define NCounterNeuronH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../Nmsdk-PulseLib/Core/NPulseMembrane.h"
#include "../../Nmsdk-PulseLib/Core/NPulseNeuronCommon.h"

//---------------------------------------------------------------------------
namespace NMSDK {
using namespace RDK;
/* *************************************************************************** */
class RDK_LIB_TYPE NCounterNeuron: public NPulseNeuronCommon
{
public: //  

 ///    
 UProperty<std::string, NCounterNeuron, ptPubParameter> MembraneClassName;

 ///    
 UProperty<std::string, NCounterNeuron, ptPubParameter> LTZoneClassName;

 ///   ,      
 UProperty<std::string, NCounterNeuron, ptPubParameter> ExcGeneratorClassName;

 ///   ,      
 UProperty<std::string, NCounterNeuron, ptPubParameter> InhGeneratorClassName;


 ///     
 ///        ,     
 UProperty<int, NCounterNeuron, ptPubParameter> MaxCount;

 ///     
 ///            
 UProperty<int, NCounterNeuron, ptPubParameter> CurCount;


public: //   
 ///    
 UProperty<MDMatrix<double>, NCounterNeuron, ptInput | ptPubState> Input;

protected: // 

///   
vector<UEPtr<NPulseMembrane> > Soma;

///     ,    
int OldNumSoma;

protected: //  
/// ,       ,     
///      ,     > 0
///         ,  TheSamePulse == true
bool TheSamePulse = false;

public: // 
// --------------------------
//   
// --------------------------
NCounterNeuron(void);
virtual ~NCounterNeuron(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NCounterNeuron* New(void);
// --------------------------

protected:
 // --------------------------
 //    
 // --------------------------

 ///    
 ///       
 //      comp      
 virtual bool AAddComponent(UEPtr<UContainer> comp, UEPtr<UIPointer> pointer = 0);

 ///    
 ///       
 //      comp    
 virtual bool ADelComponent(UEPtr<UContainer> comp);

 // --------------------------

protected:
// --------------------------
//    
// --------------------------

///     
bool SetMembraneClassName(const std::string &value);

///     
bool SetLTZoneClassName(const std::string &value);

///    ,      
bool SetExcGeneratorClassName(const std::string &value);

///    ,      
bool SetInhGeneratorClassName(const std::string &value);

///    
bool SetMaxCount(const int &value);

///     
bool SetCurCount(const int &value);

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

///        
bool CreateSomaLinks(UEPtr<NPulseMembrane> soma);

// --------------------------
};
/* *************************************************************************** */


}
//---------------------------------------------------------------------------
#endif
