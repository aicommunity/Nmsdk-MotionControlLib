#ifndef NEYERETINAMUSCLECORE_H
#define NEYERETINAMUSCLECORE_H

#include "NEyeRetinaBWCore.h"

typedef unsigned char* pbyte;

class NEyeRetinaMuscleCore
       {
       protected: // ������
        pbyte *LeftInputs, *RightInputs, *TopInputs, *BottomInputs;

        // �������������� �������� �������� �����
        int LeftOut,RightOut,TopOut,BottomOut;

       protected: // ���������
        // ������� ��������� ������ ��������
        size_t RetinaOutWidth, RetinaOutHeight;

        // ������ ������������ ���� �����
        size_t MRecSize;

        // ������� ���������� ������ � �����
        // ������������ � false ��� ������������� �������� ��������� ������
        // ����� ����� ����������
        bool ReadyState;

       protected: // ��������� ����������
        // �������������� ������� �������� ��� �����
        int LeftInput,RightInput,TopInput,BottomInput;

        //


       public: // ������
        // --------------------------
        // ������������ � �����������
        // --------------------------
        NEyeRetinaMuscleCore(void);
        virtual ~NEyeRetinaMuscleCore(void);
        // --------------------------

        // --------------------------
        // ������ ������� � ������
        // --------------------------
       public:
        inline pbyte* GetLeftInputs(void) { return LeftInputs; };
        inline pbyte* GetRightInputs(void) { return RightInputs; };
        inline pbyte* GetTopInputs(void) { return TopInputs; };
        inline pbyte* GetBottomInputs(void) { return BottomInputs; };

        inline int GetLeftOut(void) { return LeftOut; };
        inline int GetRightOut(void) { return RightOut; };
        inline int GetTopOut(void) { return TopOut; };
        inline int GetBottomOut(void) { return BottomOut; };
        // --------------------------

        // --------------------------
        // ������ ������� � ����������
        // --------------------------
       public:
        inline size_t GetRetinaOutWidth(void) { return RetinaOutWidth; };
        inline size_t GetRetinaOutHeight(void) { return RetinaOutHeight; };
        inline size_t GetMRecSize(void) { return MRecSize; };
        // --------------------------

        // --------------------------
        // ������ ���������� �����������
        // --------------------------
       public: 
        // ������������� ���������� �������� � ������ ������������ ���� ����� 
        virtual void SetRetinaRecParams(size_t rwidth, size_t rheight, size_t mrecsize);
        // --------------------------

        // --------------------------
        // ������ �����
        // --------------------------
       protected:
        // ��������� �������� ���������� ����������� �����
        virtual void SetMRecData(unsigned char *retinaout);

        // ����� ������ (� ����������� �������� ����������)
        virtual void Reset(void);

        // ����� ������ � ��������������� �������� �� ���������
        virtual void ResetDefaults(void);

        // ������
        virtual void Calculate(void);
        // --------------------------

        // --------------------------
        // ������� ������ ���������� �����������
        // --------------------------
       protected:
        // --------------------------

        // --------------------------
        // ������� ������ �����
        // --------------------------
       protected:
        // ������� ��� ��������� ������
        virtual void DestroyDataStructure(void);

        // ������� ��������� ������
        virtual void CreateDataStructure(void);

        // ��������� ������ ����
        virtual void CalcMuscles(void);
        // --------------------------
       };

#endif


 
