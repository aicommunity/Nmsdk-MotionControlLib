# Примеры использования Nmsdk-MotionControlLib

## RU

### Пример 1: Управление DC-двигателем

```cpp
#include "NDCEngine.h"

// Создание компонента DC-двигателя
UEPtr<NDCEngine> engine = storage->CreateComponent<NDCEngine>("Engine1");
engine->EMFactor = 1.0;
engine->Inductance = 0.01;
engine->Resistance = 1.0;
engine->Tm = 0.1;
engine->ReductionRate = 10.0;
engine->Default();
engine->Build();
engine->Reset();

// В цикле: подача напряжения и момента нагрузки, получение выхода
engine->InputVoltage(0, 0) = voltage;
engine->InputMomentum(0, 0) = load_moment;
engine->Calculate();
double moment = engine->OutputMomentum(0, 0);
double angle = engine->OutputAngle(0, 0);
double angle_speed = engine->OutputAngleSpeed(0, 0);
```

### Пример 2: Управление манипулятором

```cpp
#include "NManipulator.h"

// Создание манипулятора
UEPtr<NManipulator> manipulator = storage->CreateComponent<NManipulator>("Manip1");
manipulator->EMFactor = 1.0;
manipulator->Inductance = 0.01;
manipulator->Resistance = 1.0;
manipulator->Default();
manipulator->Build();
manipulator->Reset();

// Установка управляющего сигнала и получение выхода (ток)
manipulator->Input(0, 0) = control_signal;
manipulator->Calculate();
double output_current = manipulator->Output(0, 0);
```

### Пример 3: Контроль позиции

```cpp
#include "NPositionControlElement.h"

// Создание элемента контроля позиции
UEPtr<NPositionControlElement> positionControl = storage->CreateComponent<NPositionControlElement>("PosCtrl1");
positionControl->TargetPosition(0, 0) = target_pos;
positionControl->CurrentPosition(0, 0) = current_pos;
positionControl->ExternalControl = false;
positionControl->Default();
positionControl->Build();
positionControl->Reset();

// Вычисление управления (Delta и выходы нейросети обновляются внутри)
positionControl->Calculate();
double delta = positionControl->Delta(0, 0);
```

### Пример 4: Ретина глаза

```cpp
#include "NEyeRetina.h"

// Создание ретины
UEPtr<NEyeRetina> retina = storage->CreateComponent<NEyeRetina>("Retina1");
retina->Default();
retina->Build();
retina->Reset();

// Установка входного изображения (через InputImage или CaptureImage)
// retina->InputImage = ...;
retina->Calculate();
// Выходы: LeftGanglionicOut, RightGanglionicOut, TopGanglionicOut, BottomGanglionicOut
double left_out = retina->LeftGanglionicOut(0, 0);
double right_out = retina->RightGanglionicOut(0, 0);
```

---

## EN

### Example 1: DC Motor Control

```cpp
#include "NDCEngine.h"

UEPtr<NDCEngine> engine = storage->CreateComponent<NDCEngine>("Engine1");
engine->EMFactor = 1.0;
engine->Inductance = 0.01;
engine->Resistance = 1.0;
engine->Tm = 0.1;
engine->ReductionRate = 10.0;
engine->Default();
engine->Build();
engine->Reset();

engine->InputVoltage(0, 0) = voltage;
engine->InputMomentum(0, 0) = load_moment;
engine->Calculate();
double moment = engine->OutputMomentum(0, 0);
double angle = engine->OutputAngle(0, 0);
double angle_speed = engine->OutputAngleSpeed(0, 0);
```

### Example 2: Manipulator Control

```cpp
#include "NManipulator.h"

UEPtr<NManipulator> manipulator = storage->CreateComponent<NManipulator>("Manip1");
manipulator->EMFactor = 1.0;
manipulator->Inductance = 0.01;
manipulator->Resistance = 1.0;
manipulator->Default();
manipulator->Build();
manipulator->Reset();

manipulator->Input(0, 0) = control_signal;
manipulator->Calculate();
double output_current = manipulator->Output(0, 0);
```

### Example 3: Position Control

```cpp
#include "NPositionControlElement.h"

UEPtr<NPositionControlElement> positionControl = storage->CreateComponent<NPositionControlElement>("PosCtrl1");
positionControl->TargetPosition(0, 0) = target_pos;
positionControl->CurrentPosition(0, 0) = current_pos;
positionControl->ExternalControl = false;
positionControl->Default();
positionControl->Build();
positionControl->Reset();

positionControl->Calculate();
double delta = positionControl->Delta(0, 0);
```

### Example 4: Eye Retina

```cpp
#include "NEyeRetina.h"

UEPtr<NEyeRetina> retina = storage->CreateComponent<NEyeRetina>("Retina1");
retina->Default();
retina->Build();
retina->Reset();

retina->Calculate();
double left_out = retina->LeftGanglionicOut(0, 0);
double right_out = retina->RightGanglionicOut(0, 0);
```
