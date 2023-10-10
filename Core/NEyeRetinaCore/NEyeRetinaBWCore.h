#ifndef NEYERETINABWCORE_H
#define NEYERETINABWCORE_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
//#include <mem>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include "rdk.h"

using namespace std;
using namespace RDK;

typedef unsigned char byte;

enum ReceptorType
{
	// �������
	Rod = 0,
	// �������� - R
	RCone = 1,
	// �������� - G
	GCone = 2,
	// �������� - B
	BCone = 3
};

class NEyeRetinaBWCore
       {
       protected: // ������
        // ������� ����������� � ������� 0BGR
		UColorT *InputImage;

        // ������ � ��������� (������� ������� ������� � ���������)
        int **GanglionicHistory;

        // ������ ���������� ��������
        int *ARetinaOut;

        // ������� �������� ������ ���������
        int **BipolarHistory;

        // ����� ������� ������� ������ ���������
        int *BipolarHistorySum;

        // ����� ������� ������� ������ ����������� ������
        int *GanglionicHistorySum;

        // ����������� � ������������ �������� ������� ������ ���������
        int BiMin, BiMax;

        // ����������� � ������������ �������� ������� ������ ����. ������
        int GangMin, GangMax;

        // ������� ��������� ������� �������� ��������
        int *BiMinHistory, *BiMaxHistory;
        int *GangMinHistory, *GangMaxHistory;

       protected: // ���������
        // ������� ���������� ����� ������ ����������� ������
        // �� ��������� (true - ���������)
        bool GangCalcEnabled;

        // ���������� ����������� ������ ���������� � ���������
        int BiWidth, BiHeight;

        // ���������� ����������� ������ ����������� ������
        int GangWidth, GangHeight;

        // ��� �������� �� ���� ��������� ��� ������ ����������� ������
        int GangStep;

        // ������ ������������ ���� ���������
        // (������ <= ������� ������������ ���� �������������� ������)
        int BipolarRecSize;

        // ������ ������������ ���� �������������� ������
        int HorizontRecSize;

        // ������ ������������ ���� ���������
        int GanglionicRecSize;

        // ������ ������������ ���� ����������� ������
        int AmocrineRecSize;

        // ������������ ������� ������ ���������
        int BipolarHistorySize;
        
        // ������������ ������� ������ ����������� ������
        int GanglionicHistorySize;

        // ������������ ������� ��������� ������� �������� ��������
        int MinMaxHistorySize;

		// ���������� ����������� ���������������� ��� ������� �� ��������� � ����������
		double RodReduceCoef;

		// ������ ����� ���������������� ��� ��������
		int ConeFreqThr;

        // ������������ ������� �� �������
        int* BiWeights;

        // ������������ ������� �� �������������� ������
        int* HorWeights;

        // ������������ ������� �� ����������� ������
        int* GangWeights;

        // ������������ ������� �� ����������� ������
        int* AmWeights;

        // ������� ���������� ������ � �����
        // ������������ � false ��� ������������� �������� ��������� ������
        // ����� ����� ����������
        bool ReadyState;
        
       protected: // ��������� ����������
        // ����� ������� �����������
        int InputImageLength;

        // ����� ������� ������ ����������� ������
        int GangLength;

        // ����� ��� �������� ���������� � ������� ���������� ������ ���������
		int ***BiNeibBuf;
        int BiNeibBufSize;

        // ������ �������� ��������� ��������� ������� ������ ���������
		int **BiNeibBufNumbers;

        // ����� ��� �������� ���������� � ������� ���������� ������ �����. ������
		int ***GangNeibBuf;
        int GangNeibBufSize;

        // ������ �������� ��������� ��������� ������� ������ ����������� ������
        int **GangNeibBufNumbers;

        // ������ �������� (��������) �������� � ������ ������� ������ ���������
        int CurrBipolarHistIndex;

        // ������ �������� (��������) �������� � ������ ������� ������ ���������
        int CurrGanglionicHistIndex;

        // ������ �������� (��������) �������� � ������ ������� ��������� �������
        int CurrMinMaxHistIndex;

	   public:
		// ���������� ����������� ������ �� ���������
		int LeftOnActivity,TopOnActivity, RightOnActivity, BottomOnActivity;
		int LeftOffActivity,TopOffActivity, RightOffActivity, BottomOffActivity;


       public: // ������
        // --------------------------
        // ������������ � �����������
        // --------------------------
        NEyeRetinaBWCore(void);
        virtual ~NEyeRetinaBWCore(void);
        // --------------------------

        // --------------------------
        // ������ ������� � ������ 
        // --------------------------
       public:
        inline UColorT* GetInputImage(void) { return InputImage; };
        inline int* GetBipolarOut(void)
         {
          return (GangCalcEnabled)?GanglionicHistory[CurrGanglionicHistIndex]:ARetinaOut;
         }
        inline int* GetARetinaOut(void) { return ARetinaOut; }

        inline int GetBipolarHistory(size_t i, size_t time)
         { return BipolarHistory[i][time]; }

        inline int GetBiMin(void) { return BiMin; }
        inline int GetBiMax(void) { return BiMax; }
        inline int GetGangMin(void)
         { return (GangCalcEnabled)?GangMin:BiMin; }
        inline int GetGangMax(void)
         { return (GangCalcEnabled)?GangMax:BiMax; }
		// --------------------------

        // --------------------------
        // ������ ������� � ���������� 
        // --------------------------
       public:
        inline bool GetGangCalcEnabled(void) { return GangCalcEnabled; }
        inline int GetBiWidth(void) { return BiWidth; }
        inline int GetBiHeight(void) { return BiHeight; }

        inline int GetGangWidth(void) { return GangWidth; }
        inline int GetGangHeight(void) { return GangHeight; }

        inline int GetGangStep(void) { return GangStep; }

        inline int GetBipolarRecSize(void) { return BipolarRecSize-1; }
        inline int GetHorizontRecSize(void) { return HorizontRecSize-1; }
        inline int GetGanglionicRecSize(void) { return GanglionicRecSize-1; }
        inline int GetAmocrineRecSize(void) { return AmocrineRecSize-1; }

        inline int GetBipolarHistorySize(void) { return BipolarHistorySize-1; }
        inline int GetGanglionicHistorySize(void) { return GanglionicHistorySize-1; }
        inline int GetMinMaxHistorySize(void) { return MinMaxHistorySize; }

        inline int GetBiWeight(int lev) { return BiWeights[lev]; }
        inline int GetHorWeight(int lev) { return HorWeights[lev]; }
        inline int GetGangWeight(int lev) { return GangWeights[lev]; }
        inline int GetAmWeight(int lev) { return AmWeights[lev]; }

        inline double GetRodReduceCoef(void) { return RodReduceCoef; }
        inline int GetConeFreqThr(void) { return ConeFreqThr; }

        inline bool GetReadyState(void) { return ReadyState; }
        // --------------------------

        // --------------------------
        // ������ ���������� ����������� 
        // --------------------------
       public:
        virtual bool SetGangCalcEnabled(bool enabled);

        virtual bool SetRetinaRes(int width, int height, int gangstep);

        virtual void SetBipolarRecSize(int size);
        virtual void SetHorizontRecSize(int size);
        virtual void SetGanglionicRecSize(int size);
        virtual void SetAmocrineRecSize(int size);

        virtual bool SetBipolarHistorySize(int size);
        virtual bool SetGanglionicHistorySize(int size);

        virtual bool SetMinMaxHistorySize(int size);

		bool SetRodReduceCoef(double coef);
		bool SetConeFreqThr(int thr);
		// --------------------------

        // --------------------------
        // ������ �����
        // --------------------------
       public:
		// ������������� ����� �����������
		// �� ����� ��������� ��� ���������. �� ��������� - �������
		virtual bool SetInputImage(UColorT *bmp, ReceptorType rec_type = Rod);

		// ����� ������ � �������� ��������� (� �����������
        // �������� ����������)
        virtual void Reset(void);

        // ����� ������ � �������� ��������� (� ���������������
        // �������� �� ���������)
        virtual void ResetDefaults(void);

        // ������
        virtual void Calculate(void);
        // --------------------------

        // --------------------------
        // ������� ������ ���������� ����������� 
        // --------------------------
       protected:
        // ������� ����� BiNeibBuf
        virtual void CreateBiNeibBuf(void);

        // ������� ����� GangNeibBuf 
        virtual void CreateGangNeibBuf(void);

        // ���������� ���� ������� �� ����������� ���� ������� 0-level 
        virtual void CreateWeights(int *weights, size_t level);
        // --------------------------

        // --------------------------
        // ������� ������ �����
        // --------------------------
       protected:
        // ������� ��� ��������� ������
        virtual void DestroyDataStructure(void);

        // �������� ������������ ���������� ��������� ������
        // � ����� ������ (� ����������� ��������)
        virtual bool UpdateDataStructure(void);

        // ������������� ������� �����������
		virtual void InputImageCalculate(ReceptorType rec_type);

        // ������ ������� ����� ���������
        virtual void BipolarCalculate(void);

        // ������ ������� ����� ���������
        virtual void GanglionicCalculate(void);

        // ������� ����� ������� ������ ���������
        virtual void ResetBipolarHistory(void);

        // ������� ����� ������� ������ ����������� ������
        virtual void ResetGanglionicHistory(void);

        // ������� ����� ������� ��������� ������� �������� ��������
        virtual void ResetMinMaxHistory(void);

        // ���������� ����� ������� �������� � ������� index, ������ level
        // � ���������� ��������� � ������� NeibBuf � NeibBufNumbers
        // �� ������� putindex
        virtual void SearchNeighbors(int index, int putindex, int level,
										int ***NeibBuf,
                                        int **NeibBufNumbers);
        // --------------------------
       };

#endif
