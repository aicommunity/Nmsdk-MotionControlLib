#ifndef NMSDK_NEyeRetina_H
#define NMSDK_NEyeRetina_H

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "NEyeRetinaCore/NEyeRetinaBWCore.h"
#include "NEyeRetinaCore/NEyeRetinaMuscleCore.h"
#include "NEyeRetinaCore/DiffObjectSelector.h"

namespace NMSDK {

class NEyeRetina: public RDK::UNet
{
protected: // ��������
	int EyeX,EyeY,EyeWidth,EyeHeight;
	vector<pair<int,int> > EyeTraectory;

    int CaptureReadyCount;

	MovingSegmentSelector MovingObjects;
	// �������
	NEyeRetinaBWCore EyeRetina;
	// ��������
	NEyeRetinaBWCore EyeRetinaRCone;
	NEyeRetinaBWCore EyeRetinaGCone;
	NEyeRetinaBWCore EyeRetinaBCone;

protected: // ���������� ���������
UPropertyInputData<UBitmap,NEyeRetina> CaptureImage;
UPropertyOutputData<UBitmap,NEyeRetina> InputImage;
//UPropertyOutputData<UBitmap,NEyeRetina> BipolarOuts;
UPropertyOutputData<UBitmap,NEyeRetina> GanglionicOuts;
UPropertyOutputData<UBitmap,NEyeRetina> RConeGanglionicOuts;
UPropertyOutputData<UBitmap,NEyeRetina> GConeGanglionicOuts;
UPropertyOutputData<UBitmap,NEyeRetina> BConeGanglionicOuts;
UPropertyOutputData<UBitmap,NEyeRetina> RodGanglionicOuts;
UPropertyOutputData<UBitmap,NEyeRetina> OnGanglionicOuts;
UPropertyOutputData<UBitmap,NEyeRetina> OffGanglionicOuts;
// ������ ��� ������������
// ���������� ������� � ����� ����������� ������, ���������� �� ������������
// �������� ������ ������� ����
UPropertyOutputData<MDMatrix<double>,NEyeRetina, ptOutput | ptPubState> LeftGanglionicOut;
UPropertyOutputData<MDMatrix<double>,NEyeRetina, ptOutput | ptPubState> RightGanglionicOut;
UPropertyOutputData<MDMatrix<double>,NEyeRetina, ptOutput | ptPubState> TopGanglionicOut;
UPropertyOutputData<MDMatrix<double>,NEyeRetina, ptOutput | ptPubState> BottomGanglionicOut;
// ��� �������
// ���������� ������ ��� 4-� ������� ����
UPropertyOutputData<UBitmap,NEyeRetina> LeftMuscle;
UPropertyOutputData<UBitmap,NEyeRetina> RightMuscle;
UPropertyOutputData<UBitmap,NEyeRetina> TopMuscle;
UPropertyOutputData<UBitmap,NEyeRetina> BottomMuscle;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NEyeRetina(void);
virtual ~NEyeRetina(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------

// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual NEyeRetina* New(void);
// --------------------------        

// --------------------------
// ������� ������ ���������� ������ 
// --------------------------
protected:
void UpdateImages(void);

// �������������� �������� �� ��������� � ����� �������� �����
virtual bool ADefault(void);

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
virtual bool ABuild(void);

// ����� �������� ����� ��� ������ ��������
virtual bool AReset(void);

// ��������� ������ ����� �������
virtual bool ACalculate(void);
// --------------------------
};


}

#endif



