#ifndef NMSDK_NEyeRetina_H
#define NMSDK_NEyeRetina_H

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "NEyeRetinaCore/NEyeRetinaBWCore.h"
#include "NEyeRetinaCore/NEyeRetinaMuscleCore.h"
#include "NEyeRetinaCore/DiffObjectSelector.h"

namespace NMSDK {

class NEyeRetina: public RDK::UNet
{
protected: // 
	int EyeX,EyeY,EyeWidth,EyeHeight;
	vector<pair<int,int> > EyeTraectory;

    int CaptureReadyCount;

	MovingSegmentSelector MovingObjects;
	// 
	NEyeRetinaBWCore EyeRetina;
	// 
	NEyeRetinaBWCore EyeRetinaRCone;
	NEyeRetinaBWCore EyeRetinaGCone;
	NEyeRetinaBWCore EyeRetinaBCone;

protected: //  
UProperty<UBitmap,NEyeRetina, ptPubParameter> CaptureImage;
UProperty<UBitmap,NEyeRetina, ptPubParameter> InputImage;
//UProperty<UBitmap,NEyeRetina, ptPubParameter> BipolarOuts;
UProperty<UBitmap,NEyeRetina, ptPubParameter> GanglionicOuts;
UProperty<UBitmap,NEyeRetina, ptPubParameter> RConeGanglionicOuts;
UProperty<UBitmap,NEyeRetina, ptPubParameter> GConeGanglionicOuts;
UProperty<UBitmap,NEyeRetina, ptPubParameter> BConeGanglionicOuts;
UProperty<UBitmap,NEyeRetina, ptPubParameter> RodGanglionicOuts;
UProperty<UBitmap,NEyeRetina, ptPubParameter> OnGanglionicOuts;
UProperty<UBitmap,NEyeRetina, ptPubParameter> OffGanglionicOuts;
//   
//      ,   
//    
UProperty<MDMatrix<double>,NEyeRetina, ptOutput | ptPubState> LeftGanglionicOut;
UProperty<MDMatrix<double>,NEyeRetina, ptOutput | ptPubState> RightGanglionicOut;
UProperty<MDMatrix<double>,NEyeRetina, ptOutput | ptPubState> TopGanglionicOut;
UProperty<MDMatrix<double>,NEyeRetina, ptOutput | ptPubState> BottomGanglionicOut;
//  
//    4-  
UProperty<UBitmap,NEyeRetina, ptPubParameter> LeftMuscle;
UProperty<UBitmap,NEyeRetina, ptPubParameter> RightMuscle;
UProperty<UBitmap,NEyeRetina, ptPubParameter> TopMuscle;
UProperty<UBitmap,NEyeRetina, ptPubParameter> BottomMuscle;

public: // 
// --------------------------
//   
// --------------------------
NEyeRetina(void);
virtual ~NEyeRetina(void);
// --------------------------

// ---------------------
//   
// ---------------------
// ---------------------

// ---------------------
//    
// ---------------------
// ---------------------

// --------------------------
//    
// --------------------------
//         
virtual NEyeRetina* New(void);
// --------------------------        

// --------------------------
//     
// --------------------------
protected:
void UpdateImages(void);

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



