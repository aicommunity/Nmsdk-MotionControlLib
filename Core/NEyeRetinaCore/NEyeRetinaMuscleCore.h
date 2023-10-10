#ifndef NEYERETINAMUSCLECORE_H
#define NEYERETINAMUSCLECORE_H

#include "NEyeRetinaBWCore.h"

typedef unsigned char* pbyte;

class NEyeRetinaMuscleCore
       {
       protected: // Данные
        pbyte *LeftInputs, *RightInputs, *TopInputs, *BottomInputs;

        // Результирующие выходные значения мышцы
        int LeftOut,RightOut,TopOut,BottomOut;

       protected: // Параметры
        // Размеры выходного уровня сетчатки
        size_t RetinaOutWidth, RetinaOutHeight;

        // Ширина рецептивного поля мыщцы
        size_t MRecSize;

        // Признак готовности модели к счету
        // Сбрасывается в false при необходимости обновить структуры данных
        // после смены параметров
        bool ReadyState;

       protected: // Временные переменные
        // Результирующие входные значения для мышцы
        int LeftInput,RightInput,TopInput,BottomInput;

        //


       public: // Методы
        // --------------------------
        // Конструкторы и деструкторы
        // --------------------------
        NEyeRetinaMuscleCore(void);
        virtual ~NEyeRetinaMuscleCore(void);
        // --------------------------

        // --------------------------
        // Методы доступа к данным
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
        // Методы доступа к параметрам
        // --------------------------
       public:
        inline size_t GetRetinaOutWidth(void) { return RetinaOutWidth; };
        inline size_t GetRetinaOutHeight(void) { return RetinaOutHeight; };
        inline size_t GetMRecSize(void) { return MRecSize; };
        // --------------------------

        // --------------------------
        // Методы управления параметрами
        // --------------------------
       public: 
        // Устанавливает разрешение сетчатки и ширину рецептивного поля мышцы 
        virtual void SetRetinaRecParams(size_t rwidth, size_t rheight, size_t mrecsize);
        // --------------------------

        // --------------------------
        // Методы счета
        // --------------------------
       protected:
        // Установка значений указателей рецептивных полей
        virtual void SetMRecData(unsigned char *retinaout);

        // Сброс модели (с сохранением настроек параметров)
        virtual void Reset(void);

        // Сброс модели с восстановлением настроек по умолчанию
        virtual void ResetDefaults(void);

        // Расчет
        virtual void Calculate(void);
        // --------------------------

        // --------------------------
        // Скрытые методы управления параметрами
        // --------------------------
       protected:
        // --------------------------

        // --------------------------
        // Скрытые методы счета
        // --------------------------
       protected:
        // Удаляет все структуры данных
        virtual void DestroyDataStructure(void);

        // Создает структуры данных
        virtual void CreateDataStructure(void);

        // Произодит расчет мышц
        virtual void CalcMuscles(void);
        // --------------------------
       };

#endif


 
