# NManipulatorInput — вход манипулятора

## RU

**Класс**: `NManipulatorInput` — источник входных данных для манипулятора, преобразующий входной сигнал в напряжение.
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NManipulatorInput", ...)`.
**Базовый класс**: `NSource` (из Nmsdk-PulseLib).

NManipulatorInput является источником данных для манипулятора. Компонент получает входной сигнал и преобразует его в напряжение, которое передается на выход для управления манипулятором.

## UML-диаграмма классов

```mermaid
classDiagram
    NSource <|-- NManipulatorInput
    class NManipulatorInput {
        +Voltage : double
        +Input : MDMatrix~double~
        +Output : MDMatrix~double~
        +UpdateOutputFlag : bool
        +SetVoltage(value) bool
        +New() NManipulatorInput*
        #ADefault() bool
        #AReset() bool
        #ACalculate() bool
    }
```

**Иерархия наследования:**
- `NSource` (Nmsdk-PulseLib) — базовый класс для источников сигналов
- `NManipulatorInput` — вход манипулятора

## UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Input as NManipulatorInput
    participant Controller as Controller
    participant Manipulator as NManipulator

    Storage->>Input: new NManipulatorInput()
    Storage->>Input: Default()
    Input->>Input: ADefault()

    Storage->>Input: Build()
    Input->>Input: ABuild()

    Storage->>Input: Reset()
    Input->>Input: AReset()

    loop Каждый шаг вычислений
        Controller->>Input: Input = control_signal
        Storage->>Input: Calculate()
        Input->>Input: ACalculate()
        Note over Input: Voltage = Input, Output = Voltage
        Input->>Manipulator: Output = voltage
    end
```

## UML-диаграмма состояний

```mermaid
stateDiagram-v2
    [*] --> NotInitialized: Создание
    NotInitialized --> Initialized: ADefault()
    Initialized --> Built: ABuild()
    Built --> Ready: Готов к работе
    Ready --> Calculating: ACalculate()
    Calculating --> ReadingInput: Чтение Input
    ReadingInput --> SettingVoltage: Voltage = Input
    SettingVoltage --> UpdatingOutput: Output = Voltage
    UpdatingOutput --> Ready: Завершение шага
    Ready --> Reset: AReset()
    Reset --> Ready: После сброса
    Ready --> [*]: Уничтожение
```

## UML-диаграмма активности

```mermaid
flowchart TD
    Start([Начало ACalculate]) --> ReadInput["Чтение входного сигнала:<br/>Voltage = Input(0,0)"]
    ReadInput --> UpdateOutput["Обновление выхода:<br/>Output(0,0) = Voltage"]
    UpdateOutput --> End([Конец])
```

## UML-диаграмма компонентов

```mermaid
graph TB
    Input[[NManipulatorInput]]
    PulseLib["Nmsdk-PulseLib<br/>NSource"]
    BasicLib["Rdk-BasicLib<br/>Базовые компоненты"]

    Input -->|наследуется от| PulseLib
    Input -->|использует| BasicLib

    InputSignal["Input<br/>Входной сигнал управления"]
    Output["Output<br/>Выходное напряжение"]

    Input --> InputSignal
    Input --> Output
```

## Свойства

### Параметры

| Свойство | Тип | Флаги | Описание | Значение по умолчанию |
|----------|-----|-------|----------|----------------------|
| `Voltage` | `double` | `ptPubParameter` | Напряжение управления (В) | `0.0` |

### Входы

| Свойство | Тип | Флаги | Описание | Источник данных |
|----------|-----|-------|----------|-----------------|
| `Input` | `MDMatrix<double>` | `ptInput \| ptPubState` | Входной сигнал управления | Контроллер или другой источник управления |

### Выходы

| Свойство | Тип | Флаги | Описание | Назначение |
|----------|-----|-------|----------|------------|
| `Output` | `MDMatrix<double>` | `ptOutput` (из NSource) | Выходное напряжение | Передача манипулятору или другим компонентам |

### Внутренние флаги

| Свойство | Тип | Описание |
|----------|-----|----------|
| `UpdateOutputFlag` | `bool` | Флаг необходимости обновления выхода |

## Методы

### Конструкторы и деструкторы

#### `NManipulatorInput(void)`
**Назначение:** Конструктор компонента
**Параметры:** Нет
**Возвращаемое значение:** Нет
**Описание:** Инициализирует UpdateOutputFlag=false

#### `virtual ~NManipulatorInput(void)`
**Назначение:** Деструктор компонента
**Параметры:** Нет
**Возвращаемое значение:** Нет

### Методы жизненного цикла

#### `virtual bool ADefault(void)`
**Назначение:** Инициализация значений по умолчанию
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Устанавливает Voltage=0, инициализирует Input и Output нулями, вызывает ADefault() базового класса

#### `virtual bool AReset(void)`
**Назначение:** Сброс состояния компонента
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Устанавливает UpdateOutputFlag=true, вызывает AReset() базового класса

#### `virtual bool ACalculate(void)`
**Назначение:** Выполнение вычислений на текущем шаге
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Читает входной сигнал: `Voltage = Input(0,0)`, обновляет выход: `Output(0,0) = Voltage`

### Сеттеры свойств

#### `bool SetVoltage(const double &value)`
**Назначение:** Установка напряжения
**Параметры:**
- `value` - значение напряжения
**Возвращаемое значение:** `true` при успехе

### Публичные методы

#### `virtual NManipulatorInput* New(void)`
**Назначение:** Создание нового экземпляра компонента
**Параметры:** Нет
**Возвращаемое значение:** Указатель на новый экземпляр

## Примеры использования

### C++ код

```cpp
#include "NManipulatorInput.h"

UEPtr<NManipulatorInput> input = new NManipulatorInput;
input->Default();
input->Build();
input->Reset();

// Подключение контроллера
UEPtr<Controller> controller = new Controller;
controller->OutputSignal.Connect(input->Input);

// Подключение к манипулятору
UEPtr<NManipulator> manipulator = new NManipulator;
input->Output.Connect(manipulator->Input);

// Использование в цикле
for (int step = 0; step < numSteps; step++) {
    controller->Calculate();
    input->Calculate();
    manipulator->Calculate();
}
```

### XML конфигурация

```xml
<Object Name="ManipulatorInput" ClassName="NManipulatorInput">
    <Property Name="Input" Connect="Controller.OutputSignal" />
    <Property Name="Output" Connect="Manipulator.Input" />
</Object>
```

### Использование в конфигурациях

Компонент `NManipulatorInput` используется как источник входных данных для манипуляторов в системах управления движением.

**Типичные сценарии использования:**
1. **Управление манипулятором** - преобразование управляющих сигналов в напряжение для манипулятора
2. **Интеграция с контроллерами** - подключение между контроллерами и манипуляторами

**Типичные комбинации:**
- `NManipulatorInput` + `NManipulator` - управление манипулятором
- `NManipulatorInput` + контроллеры - различные стратегии управления

---

## EN

NManipulatorInput — manipulator input

**Class**: `NManipulatorInput` — source of input data for manipulator, converting input signal to voltage.
**Registration**: `NMotionControlLibrary.cpp` → `UploadClass("NManipulatorInput", ...)`.
**Base class**: `NSource` (from Nmsdk-PulseLib).

NManipulatorInput is a data source for manipulators. The component receives an input signal and converts it to voltage, which is output for manipulator control.

## Class Diagram

```mermaid
classDiagram
    NSource <|-- NManipulatorInput
    class NManipulatorInput {
        +Voltage : double
        +Input : MDMatrix~double~
        +Output : MDMatrix~double~
        +UpdateOutputFlag : bool
        +SetVoltage(value) bool
        +New() NManipulatorInput*
        #ADefault() bool
        #AReset() bool
        #ACalculate() bool
    }
```

## Sequence Diagram

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Input as NManipulatorInput
    participant Controller as Controller
    participant Manipulator as NManipulator

    Storage->>Input: new NManipulatorInput()
    Storage->>Input: Default()
    Input->>Input: ADefault()

    Storage->>Input: Build()
    Input->>Input: ABuild()

    Storage->>Input: Reset()
    Input->>Input: AReset()

    loop Each calculation step
        Controller->>Input: Input = control_signal
        Storage->>Input: Calculate()
        Input->>Input: ACalculate()
        Note over Input: Voltage = Input, Output = Voltage
        Input->>Manipulator: Output = voltage
    end
```

## State Diagram

```mermaid
stateDiagram-v2
    [*] --> NotInitialized: Creation
    NotInitialized --> Initialized: ADefault()
    Initialized --> Built: ABuild()
    Built --> Ready: Ready
    Ready --> Calculating: ACalculate()
    Calculating --> ReadingInput: Reading Input
    ReadingInput --> SettingVoltage: Voltage = Input
    SettingVoltage --> UpdatingOutput: Output = Voltage
    UpdatingOutput --> Ready: Step complete
    Ready --> Reset: AReset()
    Reset --> Ready: After reset
    Ready --> [*]: Destroy
```

## Activity Diagram

```mermaid
flowchart TD
    Start([Start ACalculate]) --> ReadInput["Reading input signal:<br/>Voltage = Input(0,0)"]
    ReadInput --> UpdateOutput["Updating output:<br/>Output(0,0) = Voltage"]
    UpdateOutput --> End([End])
```

## Component Diagram

```mermaid
graph TB
    Input[[NManipulatorInput]]
    PulseLib["Nmsdk-PulseLib<br/>NSource"]
    BasicLib["Rdk-BasicLib<br/>Basic components"]

    Input -->|inherits from| PulseLib
    Input -->|uses| BasicLib

    InputSignal["Input<br/>Control input signal"]
    Output["Output<br/>Output voltage"]

    Input --> InputSignal
    Input --> Output
```

## Properties

[Same structure as RU section, translated to English]

## Methods

[Same structure as RU section, translated to English]

## References

- [Literature-References.md](../Literature-References.md): [A], 19, 21 — manipulator input in the control hierarchy.

## Usage Examples

[Same as RU section, with English comments]
