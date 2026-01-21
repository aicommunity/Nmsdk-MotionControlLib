# Обзор API Nmsdk-MotionControlLib

## RU

### Основные классы

#### MotionControlLibrary

Главный класс библиотеки, наследник `ULibrary`.

#### NDCEngine

DC-двигатель.

**Основные свойства:**
- `MaxSpeed` - максимальная скорость
- `TargetSpeed` - целевая скорость
- `CurrentSpeed` - текущая скорость
- `Position` - текущая позиция

#### NManipulator

Роботизированный манипулятор.

**Основные свойства:**
- `NumJoints` - количество суставов
- `TargetPosition` - целевая позиция
- `CurrentPosition` - текущая позиция
- `JointAngles` - углы суставов

#### NPositionControlElement

Элемент контроля позиции.

**Основные свойства:**
- `TargetPosition` - целевая позиция
- `CurrentPosition` - текущая позиция
- `ControlOutput` - выходной сигнал управления

#### NEyeRetina

Ретина глаза.

**Основные свойства:**
- `InputImage` - входное изображение
- `RetinaOutput` - выход ретины

### См. также

- Исходный код: `Libraries/Nmsdk-MotionControlLib/Core/`

---

## EN

### Main Classes

#### MotionControlLibrary

Main library class, inherits from `ULibrary`.

#### NDCEngine

DC motor.

**Main Properties:**
- `MaxSpeed` - maximum speed
- `TargetSpeed` - target speed
- `CurrentSpeed` - current speed
- `Position` - current position

#### NManipulator

Robotic manipulator.

**Main Properties:**
- `NumJoints` - number of joints
- `TargetPosition` - target position
- `CurrentPosition` - current position
- `JointAngles` - joint angles

#### NPositionControlElement

Position control element.

**Main Properties:**
- `TargetPosition` - target position
- `CurrentPosition` - current position
- `ControlOutput` - control output signal

#### NEyeRetina

Eye retina.

**Main Properties:**
- `InputImage` - input image
- `RetinaOutput` - retina output

### See Also

- Source code: `Libraries/Nmsdk-MotionControlLib/Core/`
