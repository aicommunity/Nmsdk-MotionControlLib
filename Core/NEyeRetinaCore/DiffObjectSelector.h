#ifndef DIFFOBJECTSELECTOR_H
#define DIFFOBJECTSELECTOR_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>




/*
  class MovingSegment
  ��������� ��������� ��������� - ���������� ������� ������.
*/
struct MovingSegment
	   {
		// �������� ������ �������
		int X;

		// ������ �������
		int Width;
	   };

// --------------------------
// ������ ����������
// --------------------------
// ��������� ������� ������ 'currsegm' �� �������� ������� 'segm'+�������
void MSIntegrate(MovingSegment &currsegm, MovingSegment &segm);
// --------------------------


/*
  class MovingRect
  ��������� ��������� �������� ��������� - ���������� ����.
*/
struct MovingRect
	   {
		// ������� ����� ���� �������
		int X1,Y1;

		// ������ ������ ���� �������
		int X2,Y2;
	   };

// --------------------------
// ������ ����������
// --------------------------
// ��������� ������� ������ 'currrect' �� �������� ������� 'rect'+�������
void MRIntegrate(MovingRect &currrect, MovingRect &rect);
// --------------------------

/*
  class MovingSegmentContainer
  ��������� ��������� ������ �����������.
*/
struct MovingSegmentContainer
       {
		// ������ ����������
		// [�����_�����][�����_��������_�_������]
		MovingSegment** MovingSegmentList;

		// ����� ����� �����������
		int NumLines;

		// ����� ��������� � ������ ������
		int NumSegments;

        // ������� ����� ��������� � ������ ������
        int *CNumSegments;

//		vector<vector<MovingSegment> > MovingSegmentList;
	   };

// --------------------------
// ������ ������������� � ��������������
// --------------------------
void MSCInit(MovingSegmentContainer &obj, int numlines, int numsegments);
void MSCUninit(MovingSegmentContainer &obj);
// --------------------------

// --------------------------
// ������ ���������� �����������
// --------------------------
// ����� �����
bool MSCSetDimensions(MovingSegmentContainer &obj, int numlines, int numsegments);

// ����� ��������� ���������� ���������
//bool MSCSetNumSegments(MovingSegmentContainer &obj, int num);
// --------------------------

// --------------------------
// ������������� ���������
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
  ������������ ��������� ���������� ��������
*/
struct MovingSegmentSelector
	   {
		// ��������� �� ���������� �����������-��������
		char* Image;

		// ���������� �����������
		int XRes, YRes;

        // ��������� �������� - ���������
        // � ������� ����������� ��������
        MovingSegmentContainer Segments;

        // ��������� ���������� �������� ��������
		MovingRect *MoveObjects;

        // ����� ���������� �������� ��������
        int NumMoveObjects;

        // ������� ����� ���������� �������� ��������
        int CNumMoveObjects;

	   // ���������
		// ����� ��������� ������� ����������
		int NumFrames;

		// ������� ������� ����������
		int* FrameSize;

	   // ����� ���������
		// ���� ������� ��������������� ������ ���������� ���������
        bool IsSegmentedFlag;

        // ���� ��������� ������
        bool IsCalculatedFlag;

	   // ������ ����������������� ������� (Space Neuron)
		// ���� �������
		int** SNWeights;

		// ������ �������
        int* SNOutput;

        // ����� �������
        int* SNThresold;

        // ������������ ������������ �������
		int** SNTrFuncCoeff;

	   // ������ ���������� ������� (Counter Neuron)
		// ����� �������
		int CNOutput;
	   };

// --------------------------
// ������������� � ���������������
// --------------------------
void MSSInit(MovingSegmentSelector &obj);
void MSSUninit(MovingSegmentSelector &obj);
// --------------------------

// --------------------------
// ������ ���������� �����������
// --------------------------
// ����� ��������� ������� ����������
bool MSSSetNumFrames(MovingSegmentSelector &obj, int num);

// ������� ������� ����������
bool MSSSetFrameSize(MovingSegmentSelector &obj, int frame, int size);

bool MSSSetNumMoveObjects(MovingSegmentSelector &obj, int num);
// --------------------------

// --------------------------
// ������ ���������� ���������������� ��������
// --------------------------
// ���� �������
bool MSSSetSNWeight(MovingSegmentSelector &obj, int frame, int num, int value);

// ����� �������
bool MSSSetSNThresold(MovingSegmentSelector &obj, int frame, int value);

// ����������� ������������ �������
bool MSSSetSNTrFuncCoeff(MovingSegmentSelector &obj, int frame, int num, int value);
// --------------------------

// --------------------------
// ������ �����
// --------------------------
// ������������� ���� ��� �����
bool MSSSetImage(MovingSegmentSelector &obj, char *addr, int xres, int yres);

// ������������� ����� ���������� ���������
bool MSSSetSegments(MovingSegmentSelector &obj, int numlines, int *numsegments,
						 int *x_coords, int *widths);

// ��������� ���������� ����������� ���������� ���������
void MSSSimulate(MovingSegmentSelector &obj);

// ���������� ������ ��� �������� �� ��������� ���������
void MSSCalculate(MovingSegmentSelector &obj);

// ���������� ������ � �������� ���������
void MSSReset(MovingSegmentSelector &obj);

// ��������������� ��������� �� ���������
void MSSDefaults(MovingSegmentSelector &obj);
// --------------------------

// --------------------------
// ������� ������ �����
// --------------------------
// ������� ��������� ����������������� �������
int MSSSNActivationFn(MovingSegmentSelector &obj, int frame, int input);

// ������� ��������� ���������� �������
int MSSCNActivationFn(MovingSegmentSelector &obj);
// --------------------------
/*
		// --------------------------
		// ������������� ���������
		// --------------------------
	   public:
		inline MovingRect& operator () (int i)
		 { return MoveObjects[i]; };

		inline vector<MovingRect>& operator () (void)
		 { return MoveObjects; };
		// --------------------------
			 */
#endif

