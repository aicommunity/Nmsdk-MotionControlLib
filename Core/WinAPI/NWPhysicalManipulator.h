/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2011.
E-mail:            alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */
#ifndef NWPhysicalManipulatorH
#define NWPhysicalManipulatorH

#include <windows.h>
#define BOOST_THREAD_USE_LIB
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>

#include "../../../Rdk/Deploy/Include/rdk.h"

namespace NMSDK {

using namespace RDK;

/* *************************************************************************** */
class RDK_LIB_TYPE NWPhysicalManipulator: public UNet
{
public: // Функции DLL Управления манипулятором
typedef bool (__stdcall *PInit)(HANDLE FormHandle,bool isemulator);
typedef bool (__stdcall *PDeInit)(void);
typedef bool (__stdcall *PComOpen)(unsigned short Port);
typedef bool (__stdcall *PComClose)(void);
typedef bool (__stdcall *PComStatus)(void);
typedef bool (__stdcall *PStart)(void);
typedef bool (__stdcall *PStop)(void);
typedef bool (__stdcall *PReset)(void);
typedef bool (__stdcall *PMoveServo)(unsigned char Servo,
										 unsigned char Dir,
										 unsigned char PWM,
										 unsigned char Time);
typedef bool (__stdcall *PGetServoData)(unsigned char servo, unsigned char *data);
typedef bool (__stdcall *PSetExternalData)(double moment, double currenttime);
typedef double (__stdcall *PGetModelTime)(void);
typedef bool (__stdcall *PGetState)(void);
typedef bool (__stdcall *PNeiroPulse)(unsigned char Servo, unsigned char NumItem, unsigned char TypeItem);
typedef bool (__stdcall *PNeiroPulseGroup)(unsigned char Servo, unsigned char MaskGroup);
typedef bool (__stdcall *PGetNeiroSumm)(unsigned char Servo);
typedef bool (__stdcall *PSetNeiroParam)(unsigned char NeiroPulseValue,unsigned char NeiroAutoDecValue);
typedef bool (__stdcall *PSetWorkMode)(unsigned char WorkMode);
typedef bool (__stdcall *PAngleServo)(unsigned char Servo,
										 unsigned short Angle,
										 int Mode);
typedef bool (__stdcall *PDiffServo)(unsigned char Servo,
										 unsigned char Dir,
										 unsigned short Angle);
typedef bool (__stdcall *PHoldServo)(unsigned char Servo);
typedef bool (__stdcall *PFreeServo)(unsigned char Servo);
typedef bool (__stdcall *PSetMinPWM)(unsigned char Servo, unsigned char MinPWM);

// Структура данных звена манипулятора
struct TServoData {
 unsigned char N;
 double	Angle;
 double	Force;
 unsigned char   SetDir; //заданное направление вращения
 double	Speed; //угловая скорость
 double Pressure;  //напряжение
 unsigned char	CurrentDir; //текущее направление вращения
};

typedef enum {ServoLeft = 1, ServoRight = 2, ServoNone = 255} TServoDirect;

public: // Входы и выходы
UPropertyInputData<MDMatrix<double>, NWPhysicalManipulator, ptInput | ptPubState> Input;

UPropertyOutputData<MDMatrix<double>, NWPhysicalManipulator, ptOutput | ptPubState> Output1;
UPropertyOutputData<MDMatrix<double>, NWPhysicalManipulator, ptOutput | ptPubState> Output2;
UPropertyOutputData<MDMatrix<double>, NWPhysicalManipulator, ptOutput | ptPubState> Output3;

public: // Функции прямого управления манипулятором
PInit DMInit;
PDeInit DMDeInit;
PComOpen DMComOpen;
PComClose DMComClose;
PComStatus DMComStatus;
PStart DMStart;
PStop DMStop;
PReset DMReset;
PMoveServo DMMoveServo;
PGetServoData DMGetServoData;
PSetExternalData DMSetExternalData;
PGetModelTime DMGetModelTime;

PGetState DMGetState;
PNeiroPulse DMNeiroPulse;
PNeiroPulseGroup DMNeiroPulseGroup;
PGetNeiroSumm DMGetNeiroSumm;
PSetNeiroParam DMSetNeiroParam;
PSetWorkMode DMSetWorkMode;

PAngleServo DMAngleServo;
PDiffServo DMDiffServo;
PHoldServo DMHoldServo;
PFreeServo DMFreeServo;
PSetMinPWM DMSetMinPWM;

public: // Параметры
// Диапазоны по току (моменту)
double MinMoment,MaxMoment;

// Диапазоны по углу
double MinAngle, MaxAngle;

//Дипазоны по входному напряжению
double MinVoltage, MaxVoltage;

// Найденные диапазоны по току
double FoundMinForce, FoundMaxForce;

// Найденные диапазоны по скорости
double FoundMinSpeed, FoundMaxSpeed;

// Множитель выходного упрвляющего напряжения
double OutputMul;

// Длительность управляющего воздействия
double TimeDuration;

// Минимальная величина управляющего воздействия
double MinControlVoltage;

// Шаг по накоплению в контроллере за 1мс
int AccumulationStep;

// Шаг по разряду в контроллере за 1мс
int DissociationStep;

// Режим эмулятора в библиотеке управления двигателем
bool EmulatorMode;

// Номер COM-порта упрвления двигателем
int ComPort;

// Номер управляемого сервопривода
int ServoNumber;

// Режим работы dll манипулятора
// 0 - Не задан
// 1 - Тестовый режим
// 2 - Командный режим
// 3 - Нейроуправление
int DllManipulatorMode;

// Максимум счетчика пропусков задания управляющего воздействия
/// Теперь это время между оптправками в мс
int MaxSendCounter;

// Максимум счетчика пропусков считывания данных
/// Теперь это время между чтениями в мс
int MaxReadCounter;

public: // Данные
// Состояния выходных нейронов
vector<int> PulseState;

// Предыдущие значения углов
vector<double> PreviousAngles;

// Моменты времени в которые запомнены значения углов
vector<unsigned> AnglesTimes;

// Количество значений в истории угла
size_t NumAnglesHistory;

// Время за которое накоплена история
double AnglesHistoryTime;

// Угловая скорость
double AngleSpeed;

// Выходное управляющее напряжение
double OutputVoltage;

// Входное управляющее напряжение
double InputVoltage;

HWND WindowHandle;

protected: // Данные для безопасного обмена
double SafeOutput[3];
double SafeInput;
protected: // Временные переменные

// Счетчик пропусков задания управляющего воздействия
int SendCounter;

// Счетчик пропусков считывания данных
int ReadCounter;

// Последнее время отправки управляющего пакета
long long LastSentTime;
long long LastReadTime;
long long CurrentTime;
long long LastDuration;

vector<pair<WPARAM,LPARAM> > Params;

TServoData* ServoData;

// Идентификатор библиотеки
HMODULE ManipulatorDLL;

protected: // Потоки
boost::thread SendThread, ReadThread;

bool SendThreadTerminated,ReadThreadTerminated;

/// Мьютекс дня блокировки данных класса
boost::mutex SendMutex;
boost::mutex ReadMutex;

/// Мьютекс дня блокировки доступа к очереди команд
boost::mutex CommandMutex;
boost::mutex DataMutex;


public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
NWPhysicalManipulator(void);
//NWPhysicalManipulator(const string &name);
virtual ~NWPhysicalManipulator(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual NWPhysicalManipulator* New(void);
// --------------------------

public:
// --------------------------
// Методы управления железом
// --------------------------
// Загрузка DLL
bool LoadManipulatorDll(void);

// Выгрузка DLL
bool UnLoadManipulatorDll(void);

// Инициализация манипулятора
bool InitManipulator(void);

// Деинициализация манипулятора
bool UnInitManipulator(void);

// Шаг по накоплению в контроллере за 1мс
const int& GetAccumulationStep(void) const;
bool SetAccumulationStep(const int &value);

// Шаг по разряду в контроллере за 1мс
const int& GetDissociationStep(void) const;
bool SetDissociationStep(const int &value);
// --------------------------

// --------------------------
// Computation methods
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ADefault(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool ABuild(void);

virtual void AInit(void);
virtual void AUnInit(void);

// Reset computation
virtual bool AReset(void);

// Execute math. computations of current object on current step
virtual bool ACalculate(void);

protected:
virtual void SendCommand(void);
virtual void ABasicSendCommand(void);
virtual void ReadData(void);
// --------------------------

// --------------------------
// Вспомогательные методы
// --------------------------
protected:
// Производит считывание входных данных
bool ReadManipulatorData(void);

// Формирование управляющего сигнала манипулятором в командном режиме
bool MoveServoByCommand(void);

// Формирование управляющего сигнала манипулятором в нейро режиме
bool MoveServoByNeuro(void);
// --------------------------

};
/* *************************************************************************** */

}

#endif
