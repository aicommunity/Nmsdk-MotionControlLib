/* ***********************************************************
File License:       New BSD License
Project License:    New BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NAstaticGyro_H
#define NAstaticGyro_H

#include "../../../Rdk/Deploy/Include/rdk.h"

namespace NMSDK {

using namespace RDK;

class RDK_LIB_TYPE NAstaticGyro: public UNet
{
public: //  

// Собственная частота вращения гироскопа
RDK::UProperty<double, NAstaticGyro, ptPubParameter> natural_freq;
// Коэф. демпфирования
RDK::UProperty<double, NAstaticGyro, ptPubParameter> z_g;

RDK::UProperty<double, NAstaticGyro,ptPubState> alpha;
RDK::UProperty<double, NAstaticGyro,ptPubState> betta;
RDK::UProperty<double, NAstaticGyro,ptPubState> gamma;

public: //   
UProperty<MDMatrix<double>, NAstaticGyro, ptInput | ptPubState> Input1;
UProperty<MDMatrix<double>, NAstaticGyro, ptInput | ptPubState> Input2;
UProperty<MDMatrix<double>, NAstaticGyro, ptInput | ptPubState> Input3;

UProperty<MDMatrix<double>, NAstaticGyro, ptOutput | ptPubState> Output1;
UProperty<MDMatrix<double>, NAstaticGyro, ptOutput | ptPubState> Output2;
UProperty<MDMatrix<double>, NAstaticGyro, ptOutput | ptPubState> Output3;

//
protected: // 

double x11_prev;
double x12_prev;
double x21_prev;
double x22_prev;

double y11_prev;
double y12_prev;
double y21_prev;
double y22_prev;

double z11_prev;
double z12_prev;
double z21_prev;
double z22_prev;

public: // 
// --------------------------
// --------------------------
// Конструкторы и деструкторы
// --------------------------
// --------------------------
NAstaticGyro(void);
virtual ~NAstaticGyro(void);
// --------------------------

// --------------------------
// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
// --------------------------
virtual NAstaticGyro* New(void);
// --------------------------

// --------------------------
// --------------------------
// Скрытые методы управления счетом 
// --------------------------
// --------------------------
protected:
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
