#ifndef NMSDK_NEyeRetina_H
#define NMSDK_NEyeRetina_H

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "NEyeRetinaCore/NEyeRetinaBWCore.h"
#include "NEyeRetinaCore/NEyeRetinaMuscleCore.h"
#include "NEyeRetinaCore/DiffObjectSelector.h"

namespace NMSDK {

class NEyeRetina: public RDK::UNet
{
protected: // Свойства
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

protected: // Переменные состояния
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
// Выходы для мотонейронов
// Активность выходов с групп ганглиозных клеток, отвечающих за рефлекторные
// движения четырёх глазных мышц
UPropertyOutputData<MDMatrix<double>,NEyeRetina, ptOutput | ptPubState> LeftGanglionicOut;
UPropertyOutputData<MDMatrix<double>,NEyeRetina, ptOutput | ptPubState> RightGanglionicOut;
UPropertyOutputData<MDMatrix<double>,NEyeRetina, ptOutput | ptPubState> TopGanglionicOut;
UPropertyOutputData<MDMatrix<double>,NEyeRetina, ptOutput | ptPubState> BottomGanglionicOut;
// ДЛЯ ОТЛАДКИ
// АКТИВНОСТЬ ВХОДОВ ДЛЯ 4-ч ГЛАЗНЫХ МЫШЦ
UPropertyOutputData<UBitmap,NEyeRetina> LeftMuscle;
UPropertyOutputData<UBitmap,NEyeRetina> RightMuscle;
UPropertyOutputData<UBitmap,NEyeRetina> TopMuscle;
UPropertyOutputData<UBitmap,NEyeRetina> BottomMuscle;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NEyeRetina(void);
virtual ~NEyeRetina(void);
// --------------------------

// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------

// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NEyeRetina* New(void);
// --------------------------        

// --------------------------
// Скрытые методы управления счетом 
// --------------------------
protected:
void UpdateImages(void);

// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ADefault(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool ABuild(void);

// Сброс процесса счета без потери настроек
virtual bool AReset(void);

// Выполняет расчет этого объекта
virtual bool ACalculate(void);
// --------------------------
};


}

#endif



