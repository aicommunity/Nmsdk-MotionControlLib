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
	// Палочка
	Rod = 0,
	// Колбочка - R
	RCone = 1,
	// Колбочка - G
	GCone = 2,
	// Колбочка - B
	BCone = 3
};

class NEyeRetinaBWCore
       {
       protected: // Данные
        // Входное изображение в формате 0BGR
		UColorT *InputImage;

        // Выходы с биполяров (включая историю сигнала с биполяров)
        int **GanglionicHistory;

        // Выходы аналоговые сетчатки
        int *ARetinaOut;

        // История сигналов уровня биполяров
        int **BipolarHistory;

        // Сумма текущей истории уровня биполяров
        int *BipolarHistorySum;

        // Сумма текущей истории уровня ганглиозных клеток
        int *GanglionicHistorySum;

        // Минимальное и максимальное значения выходов уровня биполяров
        int BiMin, BiMax;

        // Минимальное и максимальное значения выходов уровня ганг. клеток
        int GangMin, GangMax;

        // Истории граничных уровней выходных сигналов
        int *BiMinHistory, *BiMaxHistory;
        int *GangMinHistory, *GangMaxHistory;

       protected: // Параметры
        // Признак разрешения счета уровня ганглиозных клеток
        // По умолчанию (true - разрешено)
        bool GangCalcEnabled;

        // Разрешение изображения уровня рецепторов и биполяров
        int BiWidth, BiHeight;

        // Разрешение изображения уровня ганглиозных клеток
        int GangWidth, GangHeight;

        // Шаг смещения по полю биполяров для уровня ганглиозных клеток
        int GangStep;

        // Размер рецептивного поля биполяров
        // (всегда <= размера рецептивного поля горизонтальной клетки)
        int BipolarRecSize;

        // Размер рецептивного поля горизонтальной клетки
        int HorizontRecSize;

        // Размер рецептивного поля биполяров
        int GanglionicRecSize;

        // Размер рецептивного поля амокриновой клетки
        int AmocrineRecSize;

        // Длительность истории уровня биполяров
        int BipolarHistorySize;
        
        // Длительность истории уровня ганглиозных клеток
        int GanglionicHistorySize;

        // Длительность истории граничных уровней выходных сигналов
        int MinMaxHistorySize;

		// Понижающий коэффициент чувствительности для палочек по сравнению с колбочками
		double RodReduceCoef;

		// Нижний порог чувствительности для колбочек
		int ConeFreqThr;

        // Коэффициенты влияния на биполяр
        int* BiWeights;

        // Коэффициенты влияния на горизонтальную клетку
        int* HorWeights;

        // Коэффициенты влияния на ганглиозную клетку
        int* GangWeights;

        // Коэффициенты влияния на амокриновую клетку
        int* AmWeights;

        // Признак готовности модели к счету
        // Сбрасывается в false при необходимости обновить структуры данных
        // после смены параметров
        bool ReadyState;
        
       protected: // Временные переменные
        // Длина массива изображения
        int InputImageLength;

        // Длина массива уровня ганглиозных клеток
        int GangLength;

        // Буфер для хранения информации о соседях рецепторов уровня биполяров
		int ***BiNeibBuf;
        int BiNeibBufSize;

        // Массив значений количеств найденных соседей уровня биполяров
		int **BiNeibBufNumbers;

        // Буфер для хранения информации о соседях рецепторов уровня гангл. клеток
		int ***GangNeibBuf;
        int GangNeibBufSize;

        // Массив значений количеств найденных соседей уровня ганглиозных клеток
        int **GangNeibBufNumbers;

        // Индекс нулевого (текущего) значения в буфере истории уровня биполяров
        int CurrBipolarHistIndex;

        // Индекс нулевого (текущего) значения в буфере истории уровня биполяров
        int CurrGanglionicHistIndex;

        // Индекс нулевого (текущего) значения в буфере истории граничных уровней
        int CurrMinMaxHistIndex;

	   public:
		// Активность ганглиозных клеток на перефирии
		int LeftOnActivity,TopOnActivity, RightOnActivity, BottomOnActivity;
		int LeftOffActivity,TopOffActivity, RightOffActivity, BottomOffActivity;


       public: // Методы
        // --------------------------
        // Конструкторы и деструкторы
        // --------------------------
        NEyeRetinaBWCore(void);
        virtual ~NEyeRetinaBWCore(void);
        // --------------------------

        // --------------------------
        // Методы доступа к данным 
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
        // Методы доступа к параметрам 
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
        // Методы управления параметрами 
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
        // Методы счета
        // --------------------------
       public:
		// Устанавливает новое изображение
		// На входе указываем тип рецептора. По умолчанию - палочка
		virtual bool SetInputImage(UColorT *bmp, ReceptorType rec_type = Rod);

		// Сброс модели в исходное состояние (с сохранением
        // настроек параметров)
        virtual void Reset(void);

        // Сброс модели в исходное состояние (с восстановлением
        // настроек по умолчанию)
        virtual void ResetDefaults(void);

        // Расчет
        virtual void Calculate(void);
        // --------------------------

        // --------------------------
        // Скрытые методы управления параметрами 
        // --------------------------
       protected:
        // Создает буфер BiNeibBuf
        virtual void CreateBiNeibBuf(void);

        // Создает буфер GangNeibBuf 
        virtual void CreateGangNeibBuf(void);

        // Генерирует веса вкладов от рецептивной зоны размера 0-level 
        virtual void CreateWeights(int *weights, size_t level);
        // --------------------------

        // --------------------------
        // Скрытые методы счета
        // --------------------------
       protected:
        // Удаляет все структуры данных
        virtual void DestroyDataStructure(void);

        // Вызывает пересоздание внутренней структуры класса
        // и сброс модели (с сохранением настроек)
        virtual bool UpdateDataStructure(void);

        // Пересчитывает входное изображение
		virtual void InputImageCalculate(ReceptorType rec_type);

        // Расчет первого этапа обработки
        virtual void BipolarCalculate(void);

        // Расчет второго этапа обработки
        virtual void GanglionicCalculate(void);

        // Очищает буфер истории уровня биполяров
        virtual void ResetBipolarHistory(void);

        // Очищает буфер истории уровня ганглиозных клеток
        virtual void ResetGanglionicHistory(void);

        // Очищает буфер истории граничных уровней выходных сигналов
        virtual void ResetMinMaxHistory(void);

        // Производит поиск соседей элемента с номером index, уровня level
        // и записывает результат в массивы NeibBuf и NeibBufNumbers
        // по индексу putindex
        virtual void SearchNeighbors(int index, int putindex, int level,
										int ***NeibBuf,
                                        int **NeibBufNumbers);
        // --------------------------
       };

#endif
