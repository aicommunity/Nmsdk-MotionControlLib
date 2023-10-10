/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2012.
E-mail:            alexab@ailab.ru
url:            http://ailab.ru

This file - part of the project: Neuro Modeler Software Developer Kit (NMSDK)

File License:        BSD License
Project License:     BSD License
See file license.txt for more information
*********************************************************** */

#include "NPendulumAndCart.h"

//---------------------------------------------------------------------------
namespace NMSDK{

/* *************************************************************************** */
// --------------------------
// Constructors & destructors
// --------------------------
NPendulumAndCart::NPendulumAndCart(void)
// : NADItem(name),
 : CartMass("CartMass",this),
   RodMass("RodMass",this),
   RodLength("RodLength",this),
   OutXMovement("OutXMovement",this),
   Mode("Mode",this),
   Ki("Ki",this),
   Kd("Kd",this),
   Kp("Kp",this),
   MovementKi("MovementKi",this),
   MovementKd("MovementKd",this),
   MovementKp("MovementKp",this),
   AngleWeight("AngleWeight",this),
   MovementWeight("MovementWeight",this),
   ExtrenalMoment("ExtrenalMoment",this),

   Input1("Input1",this),
   Input2("Input2",this),
   Acceleration("Acceleration",this),
   Angle("Angle",this),
   Speed("Speed",this),
   Movement("Movement",this),
   MovementSpeed("MovementSpeed",this)
{
 theta0=0;
 x0=0;
 y=0;
 x=0;
 movement=0;
 y0=0;
}

NPendulumAndCart::~NPendulumAndCart(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
NPendulumAndCart* NPendulumAndCart::New(void)
{
 return new NPendulumAndCart;
}
// --------------------------

// --------------------------
// Computation methods
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool NPendulumAndCart::ADefault(void)
{
 CartMass=1;
 RodMass=0.1;
 RodLength=1;

 ExtrenalMoment=0.0;
 Kp=0;
 Kd=0;
 Ki=0;
 MovementKp=0;
 MovementKd=0;
 MovementKi=0;
 AngleWeight=1;
 MovementWeight=1;

 Input1->Assign(1,1,0.0);
 Input2->Assign(1,1,0.0);

 Acceleration.Assign(1,1,0.0);
 Angle.Assign(1,1,0.0);
 Speed.Assign(1,1,0.0);
 Movement.Assign(1,1,0.0);
 MovementSpeed.Assign(1,1,0.0);

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool NPendulumAndCart::ABuild(void)
{

 return true;
}

// Reset computation
bool NPendulumAndCart::AReset(void)
{
 theta0 = 0;
 y = 0;
 x = 0;
 movement = 0;
 MovementSpeed.Assign(1,1,0.0);

 Speed.Assign(1,1,0.0);
 Acceleration.Assign(1,1,0.0);

 return true;
}

// Execute math. computations of current object on current step
bool NPendulumAndCart::ACalculate(void)
{
 vector<double> input;

 input.resize(2);

 input[0]=(*Input1)(0,0);
 input[1]=(*Input2)(0,0);

 if(Mode == 1)
 {
  input[0]=(theta0*Kp+y*Kd+Ki*theta0/TimeStep)*AngleWeight+(movement*MovementKp+x*MovementKd+MovementKi*movement/TimeStep)*MovementWeight;
 // input[0]=(theta0*Kp+y*Kd+Ki*theta0/TimeStep)*AngleWeight+(movement*MovementKp+x*MovementKd+MovementKi*movement/TimeStep)*MovementWeight; // input[0]=(theta0*Kp+y*Kd+Ki*theta0/TimeStep)*AngleWeight+(movement*Kp+x*Kd+Ki*movement/TimeStep)*MovementWeight;
 }

 input[0]+=ExtrenalMoment;
 double k1 = 3*9.8*(CartMass + RodMass)/((4*CartMass + RodMass)*RodLength);
 double k2 = 3/(4*CartMass + RodMass)/RodLength;
 double a=k1;
 double b=k2*input[0];

 y = y + (a*theta0 + b)/TimeStep; //-OutXMovement;
 Angle(0,0) = theta0 + (y-OutXMovement)/TimeStep;
// Angle = theta0 + (y)/TimeStep;

 double speed=(Angle(0,0)-theta0)*TimeStep;
 Acceleration(0,0)=(speed-Speed(0,0))*TimeStep;

 Speed(0,0)=speed;
 theta0 = Angle(0,0);

 double km1 = 3*9.8*RodMass*Angle(0,0)/(4*CartMass + RodMass);
 double km2 = 4*input[0]/(4*CartMass + RodMass);
 x = x+(km1+km2)/TimeStep;
 Movement(0,0) = movement + x/TimeStep;

 MovementSpeed(0,0)=x;
 movement = Movement(0,0);

 return true;
}
// --------------------------
/* *************************************************************************** */

}
//---------------------------------------------------------------------------

