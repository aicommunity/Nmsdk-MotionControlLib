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
	// Палочки
	NEyeRetinaBWCore EyeRetina;
	// Колбочки
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
// Выходы для мотонейронов
// Активность выходов с групп ганглиозных клеток, отвечающих за рефлекторные
// движения четырёх глазных мышц
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
// --------------------------
// Конструкторы и деструкторы
// --------------------------
// --------------------------
NEyeRetina(void);
virtual ~NEyeRetina(void);
// --------------------------

// ---------------------
// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
// ---------------------
// ---------------------

// ---------------------
// ---------------------
// ---------------------

// --------------------------
// --------------------------
virtual NEyeRetina* New(void);
// --------------------------        

// --------------------------
// --------------------------
// Скрытые методы управления счетом 
// --------------------------
// --------------------------
protected:
void UpdateImages(void);

// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ADefault(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
//    Reset()   Ready  true
virtual bool ABuild(void);

// Сброс процесса счета без потери настроек
virtual bool AReset(void);

// Выполняет расчет этого объекта
virtual bool ACalculate(void);
// --------------------------
};


}

#endif



