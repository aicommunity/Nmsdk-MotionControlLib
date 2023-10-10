#ifndef DIFFOBJECTSELECTOR_H
#define DIFFOBJECTSELECTOR_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>




/*
  class MovingSegment
  Описывает единичный результат - движущийся участок строки.
*/
struct MovingSegment
	   {
		// Смещение начала объекта
		int X;

		// Ширина объекта
		int Width;
	   };

// --------------------------
// Методы управления
// --------------------------
// Расширяет текущий объект 'currsegm' до размеров объекта 'segm'+текущий
void MSIntegrate(MovingSegment &currsegm, MovingSegment &segm);
// --------------------------


/*
  class MovingRect
  Описывает одиночный конечный результат - движущийся кадр.
*/
struct MovingRect
	   {
		// Верхний левый угол объекта
		int X1,Y1;

		// Правый нижний угол объекта
		int X2,Y2;
	   };

// --------------------------
// Методы управления
// --------------------------
// Расширяет текущий объект 'currrect' до размеров объекта 'rect'+текущий
void MRIntegrate(MovingRect &currrect, MovingRect &rect);
// --------------------------

/*
  class MovingSegmentContainer
  Описывает хранилище набора результатов.
*/
struct MovingSegmentContainer
       {
		// Данные контейнера
		// [число_строк][число_объектов_в_строке]
		MovingSegment** MovingSegmentList;

		// Число строк изображения
		int NumLines;

		// Число сегментов в каждой строке
		int NumSegments;

        // Текущее число сегментов в каждой строке
        int *CNumSegments;

//		vector<vector<MovingSegment> > MovingSegmentList;
	   };

// --------------------------
// Методы инициализации и деинциализации
// --------------------------
void MSCInit(MovingSegmentContainer &obj, int numlines, int numsegments);
void MSCUninit(MovingSegmentContainer &obj);
// --------------------------

// --------------------------
// Методы управления параметрами
// --------------------------
// Число строк
bool MSCSetDimensions(MovingSegmentContainer &obj, int numlines, int numsegments);

// Число найденных движущихся сегментов
//bool MSCSetNumSegments(MovingSegmentContainer &obj, int num);
// --------------------------

// --------------------------
// Перегруженные операторы
// --------------------------
 /*		inline MovingSegmentContainer& operator = (const MovingSegmentContainer &cont)
		 { MovingSegmentList=cont.MovingSegmentList; return *this; };

		inline MovingSegment& operator () (int line, int segment)
		 { return MovingSegmentList[line][segment]; };

		inline vector<MovingSegment>& operator () (int line)
		 { return MovingSegmentList[line]; };
*/		 
// --------------------------




/*
  class MovingSegmentSelector.
  Обеспечивает выделение движущихся объектов
*/
struct MovingSegmentSelector
	   {
		// Указатель на разностное изображение-источник
		char* Image;

		// Разрешение изображения
		int XRes, YRes;

        // множество структур - сегментов
        // в которых фиксировано движение
        MovingSegmentContainer Segments;

        // множество выделенных областей движения
		MovingRect *MoveObjects;

        // число выделенных областей движения
        int NumMoveObjects;

        // Текущее число выделенных областей движения
        int CNumMoveObjects;

	   // Параметры
		// Число различных фреймов наблюдения
		int NumFrames;

		// Размеры фреймов наблюдения
		int* FrameSize;

	   // Флаги состояний
		// Флаг наличия сформированного набора движущихся сегментов
        bool IsSegmentedFlag;

        // Флаг успешного рачета
        bool IsCalculatedFlag;

	   // Модель пространственного нейрона (Space Neuron)
		// Веса нейрона
		int** SNWeights;

		// Выходы нейрона
        int* SNOutput;

        // Порог нейрона
        int* SNThresold;

        // Коэффициенты передаточной функции
		int** SNTrFuncCoeff;

	   // Модель считающего нейрона (Counter Neuron)
		// Выход нейрона
		int CNOutput;
	   };

// --------------------------
// инициализация и деинициализация
// --------------------------
void MSSInit(MovingSegmentSelector &obj);
void MSSUninit(MovingSegmentSelector &obj);
// --------------------------

// --------------------------
// Методы управления параметрами
// --------------------------
// Число различных фреймов наблюдения
bool MSSSetNumFrames(MovingSegmentSelector &obj, int num);

// Размеры фреймов наблюдения
bool MSSSetFrameSize(MovingSegmentSelector &obj, int frame, int size);

bool MSSSetNumMoveObjects(MovingSegmentSelector &obj, int num);
// --------------------------

// --------------------------
// Методы управления пространственным нейроном
// --------------------------
// Веса нейрона
bool MSSSetSNWeight(MovingSegmentSelector &obj, int frame, int num, int value);

// Порог нейрона
bool MSSSetSNThresold(MovingSegmentSelector &obj, int frame, int value);

// Коэффициент передаточной функции
bool MSSSetSNTrFuncCoeff(MovingSegmentSelector &obj, int frame, int num, int value);
// --------------------------

// --------------------------
// Методы счета
// --------------------------
// Устанавливает кадр для счета
bool MSSSetImage(MovingSegmentSelector &obj, char *addr, int xres, int yres);

// Устанавливает набор движущихся сегментов
bool MSSSetSegments(MovingSegmentSelector &obj, int numlines, int *numsegments,
						 int *x_coords, int *widths);

// Эмулирует аппаратное определение движущихся сегментов
void MSSSimulate(MovingSegmentSelector &obj);

// Производит расчет зон движения по имеющимся сегментам
void MSSCalculate(MovingSegmentSelector &obj);

// Сбрасывает модель в исходное состояние
void MSSReset(MovingSegmentSelector &obj);

// Восстанавливает настройки по умолчанию
void MSSDefaults(MovingSegmentSelector &obj);
// --------------------------

// --------------------------
// Скрытые методы счета
// --------------------------
// Функция активации пространственного нейрона
int MSSSNActivationFn(MovingSegmentSelector &obj, int frame, int input);

// Функция активации считающего нейрона
int MSSCNActivationFn(MovingSegmentSelector &obj);
// --------------------------
/*
		// --------------------------
		// Перегруженные операторы
		// --------------------------
	   public:
		inline MovingRect& operator () (int i)
		 { return MoveObjects[i]; };

		inline vector<MovingRect>& operator () (void)
		 { return MoveObjects; };
		// --------------------------
			 */
#endif

