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
public: //  DLL  
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

//    
struct TServoData {
 unsigned char N;
 double	Angle;
 double	Force;
 unsigned char   SetDir; //  
 double	Speed; // 
 double Pressure;  //
 unsigned char	CurrentDir; //  
};

typedef enum {ServoLeft = 1, ServoRight = 2, ServoNone = 255} TServoDirect;

public: //   
UProperty<MDMatrix<double>, NWPhysicalManipulator, ptInput | ptPubState> Input;

UProperty<MDMatrix<double>, NWPhysicalManipulator, ptOutput | ptPubState> Output1;
UProperty<MDMatrix<double>, NWPhysicalManipulator, ptOutput | ptPubState> Output2;
UProperty<MDMatrix<double>, NWPhysicalManipulator, ptOutput | ptPubState> Output3;

public: //    
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

public: // 
//    ()
double MinMoment,MaxMoment;

//   
double MinAngle, MaxAngle;

//   
double MinVoltage, MaxVoltage;

//    
double FoundMinForce, FoundMaxForce;

//    
double FoundMinSpeed, FoundMaxSpeed;

//    
double OutputMul;

//   
double TimeDuration;

//    
double MinControlVoltage;

//       1
int AccumulationStep;

//       1
int DissociationStep;

//      
bool EmulatorMode;

//  COM-  
int ComPort;

//   
int ServoNumber;

//   dll 
// 0 -  
// 1 -  
// 2 -  
// 3 - 
int DllManipulatorMode;

//      
///       
int MaxSendCounter;

//     
///       
int MaxReadCounter;

public: // 
//   
vector<int> PulseState;

//   
vector<double> PreviousAngles;

//       
vector<unsigned> AnglesTimes;

//     
size_t NumAnglesHistory;

//     
double AnglesHistoryTime;

//  
double AngleSpeed;

//   
double OutputVoltage;

//   
double InputVoltage;

HWND WindowHandle;

protected: //    
double SafeOutput[3];
double SafeInput;
protected: //  

//     
int SendCounter;

//    
int ReadCounter;

//     
long long LastSentTime;
long long LastReadTime;
long long CurrentTime;
long long LastDuration;

vector<pair<WPARAM,LPARAM> > Params;

TServoData* ServoData;

//  
HMODULE ManipulatorDLL;

protected: // 
boost::thread SendThread, ReadThread;

bool SendThreadTerminated,ReadThreadTerminated;

///     
boost::mutex SendMutex;
boost::mutex ReadMutex;

///       
boost::mutex CommandMutex;
boost::mutex DataMutex;


public: // 
// --------------------------
//   
// --------------------------
NWPhysicalManipulator(void);
//NWPhysicalManipulator(const string &name);
virtual ~NWPhysicalManipulator(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual NWPhysicalManipulator* New(void);
// --------------------------

public:
// --------------------------
//   
// --------------------------
//  DLL
bool LoadManipulatorDll(void);

//  DLL
bool UnLoadManipulatorDll(void);

//  
bool InitManipulator(void);

//  
bool UnInitManipulator(void);

//       1
const int& GetAccumulationStep(void) const;
bool SetAccumulationStep(const int &value);

//       1
const int& GetDissociationStep(void) const;
bool SetDissociationStep(const int &value);
// --------------------------

// --------------------------
// Computation methods
// --------------------------
//        
virtual bool ADefault(void);

//     
//   
//    Reset()   Ready  true
//    
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
//  
// --------------------------
protected:
//    
bool ReadManipulatorData(void);

//       
bool MoveServoByCommand(void);

//       
bool MoveServoByNeuro(void);
// --------------------------

};
/* *************************************************************************** */

}

#endif
