# Обзор API Nmsdk-MotionControlLib

## RU

### Основные классы

#### MotionControlLibrary

Главный класс библиотеки, наследник `ULibrary`.

#### NDCEngine

DC-двигатель. Модель с электромеханическими параметрами.

**Параметры (ptPubParameter):**
- `EMFactor` — коэффициент ЭДС
- `Inductance` — индуктивность
- `Resistance` — сопротивление
- `Tm` — постоянная времени
- `ReductionRate` — передаточное отношение
- `OutMoment` — выходной момент

**Входы:** `InputVoltage`, `InputMomentum` (напряжение и момент нагрузки).

**Выходы:** `OutputMomentum`, `OutputAngle`, `OutputAngleSpeed` (момент, угол, угловая скорость).

#### NManipulator

Роботизированный манипулятор (упрощённая электрическая модель).

**Параметры:** `EMFactor`, `Inductance`, `Resistance`.

**Входы:** `Input` (управляющий сигнал).

**Выходы:** `Output` (ток/сигнал для приводов).

#### NPositionControlElement

Элемент контроля позиции. Базовый класс для нейросетевого контроля позиции.

**Параметры и состояние:** `TargetPosition`, `CurrentPosition`, `InputNeuronType`, `ControlNeuronType`, `ExternalControl`, `RememberState`, `Delta`.

**Внутренние структуры:** `InputNeurons`, `ControlNeurons`, `Generators`.

#### NEyeRetina

Ретина глаза. Обработка визуальной информации.

**Входы/параметры:** `CaptureImage`, `InputImage`.

**Выходы (изображения):** `GanglionicOuts`, `RConeGanglionicOuts`, `GConeGanglionicOuts`, `BConeGanglionicOuts`, `RodGanglionicOuts`, `OnGanglionicOuts`, `OffGanglionicOuts`.

**Выходы (сигналы управления):** `LeftGanglionicOut`, `RightGanglionicOut`, `TopGanglionicOut`, `BottomGanglionicOut`.

**Параметры мышц:** `LeftMuscle`, `RightMuscle`, `TopMuscle`, `BottomMuscle`.

### См. также

- Исходный код: `Libraries/Nmsdk-MotionControlLib/Core/`
- Каталог компонентов: [Component-Catalog.md](Component-Catalog.md)
- Детальная документация по классам: [Components/](Components/)

---

## EN

### Main Classes

#### MotionControlLibrary

Main library class, inherits from `ULibrary`.

#### NDCEngine

DC motor. Model with electromechanical parameters.

**Parameters (ptPubParameter):** `EMFactor`, `Inductance`, `Resistance`, `Tm`, `ReductionRate`, `OutMoment`.

**Inputs:** `InputVoltage`, `InputMomentum` (voltage and load torque).

**Outputs:** `OutputMomentum`, `OutputAngle`, `OutputAngleSpeed` (torque, angle, angular speed).

#### NManipulator

Robotic manipulator (simplified electrical model).

**Parameters:** `EMFactor`, `Inductance`, `Resistance`.

**Inputs:** `Input` (control signal).

**Outputs:** `Output` (current/signal for actuators).

#### NPositionControlElement

Position control element. Base class for neural position control.

**Parameters and state:** `TargetPosition`, `CurrentPosition`, `InputNeuronType`, `ControlNeuronType`, `ExternalControl`, `RememberState`, `Delta`.

**Internal structures:** `InputNeurons`, `ControlNeurons`, `Generators`.

#### NEyeRetina

Eye retina. Visual information processing.

**Inputs/parameters:** `CaptureImage`, `InputImage`.

**Outputs (images):** `GanglionicOuts`, `RConeGanglionicOuts`, `GConeGanglionicOuts`, `BConeGanglionicOuts`, `RodGanglionicOuts`, `OnGanglionicOuts`, `OffGanglionicOuts`.

**Outputs (control signals):** `LeftGanglionicOut`, `RightGanglionicOut`, `TopGanglionicOut`, `BottomGanglionicOut`.

**Muscle parameters:** `LeftMuscle`, `RightMuscle`, `TopMuscle`, `BottomMuscle`.

### See Also

- Source code: `Libraries/Nmsdk-MotionControlLib/Core/`
- Component catalog: [Component-Catalog.md](Component-Catalog.md)
- Per-class documentation: [Components/](Components/)
