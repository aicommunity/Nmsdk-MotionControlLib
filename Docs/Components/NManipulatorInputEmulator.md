# NManipulatorInputEmulator — эмулятор ввода манипулятора

**Класс**: `NManipulatorInputEmulator` — эмулятор входных данных для манипулятора, упрощённая версия `NManipulatorInput` для тестирования и отладки.  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NManipulatorInputEmulator", ...)`.  
**Базовый класс**: `NManipulatorInput` (из Nmsdk-MotionControlLib).

NManipulatorInputEmulator является упрощённой версией `NManipulatorInput`, предназначенной для эмуляции входных команд манипулятора. Компонент наследует все свойства базового класса и просто передаёт входной сигнал на выход через напряжение. Используется для тестирования систем управления манипулятором без реального физического устройства.

## UML-диаграмма классов

```mermaid
classDiagram
    NSource <|-- NManipulatorInput
    NManipulatorInput <|-- NManipulatorInputEmulator
    class NManipulatorInputEmulator {
        +Voltage : double
        +Input : MDMatrix~double~
        +Output : MDMatrix~double~
        +UpdateOutputFlag : bool
        +New() NManipulatorInputEmulator*
        #ADefault() bool
        #AReset() bool
        #ACalculate() bool
    }
```

**Иерархия наследования:**
- `NSource` (Nmsdk-PulseLib) — базовый класс для источников сигналов
- `NManipulatorInput` — базовый класс входа манипулятора
- `NManipulatorInputEmulator` — эмулятор входа манипулятора

## UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Emulator as NManipulatorInputEmulator
    participant Source as SignalSource
    participant Manipulator as NManipulator
    
    Storage->>Emulator: new NManipulatorInputEmulator()
    Storage->>Emulator: Default()
    Emulator->>Emulator: ADefault()
    Note over Emulator: Voltage = 0
    
    Storage->>Emulator: Build()
    Emulator->>Emulator: ABuild()
    
    Storage->>Emulator: Reset()
    Emulator->>Emulator: AReset()
    Note over Emulator: UpdateOutputFlag = true
    
    loop Каждый шаг вычислений
        Source->>Emulator: Input = control_signal
        Storage->>Emulator: Calculate()
        Emulator->>Emulator: ACalculate()
        Note over Emulator: Voltage = Input(0,0)<br/>Output(0,0) = Voltage
        Emulator->>Manipulator: Output = voltage
    end
```

**Описание жизненного цикла:**
1. **Создание** - компонент создаётся через конструктор
2. **Инициализация (ADefault)** - установка `Voltage=0` по умолчанию
3. **Построение (ABuild)** - подготовка к работе (наследуется от базового класса)
4. **Сброс (AReset)** - установка `UpdateOutputFlag=true` и сброс состояния
5. **Вычисление (ACalculate)** - передача входного сигнала на выход: `Voltage = Input(0,0)`, `Output(0,0) = Voltage`

## UML-диаграмма состояний

```mermaid
stateDiagram-v2
    [*] --> NotInitialized: Создание
    NotInitialized --> Initialized: ADefault()
    Note right of Initialized: Voltage = 0
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

**Состояния компонента:**
- **NotInitialized** - компонент создан, но не инициализирован
- **Initialized** - компонент инициализирован с `Voltage=0`
- **Built** - готов к работе
- **Ready** - готов к вычислениям
- **Calculating** - выполняется вычисление
- **ReadingInput** - чтение входного сигнала
- **SettingVoltage** - установка напряжения из входного сигнала
- **UpdatingOutput** - обновление выходного сигнала
- **Reset** - состояние после сброса

## UML-диаграмма активности

```mermaid
flowchart TD
    Start([Начало ACalculate]) --> ReadInput[Чтение входного сигнала:<br/>Voltage = Input(0,0)]
    ReadInput --> UpdateOutput[Обновление выхода:<br/>Output(0,0) = Voltage]
    UpdateOutput --> End([Конец])
```

**Алгоритм работы ACalculate:**
1. Чтение входного сигнала: `Voltage = Input(0,0)`
2. Обновление выходного сигнала: `Output(0,0) = Voltage`

Компонент просто передаёт входной сигнал на выход без дополнительной обработки, что делает его удобным для эмуляции команд управления манипулятором.

## UML-диаграмма компонентов

```mermaid
graph TB
    subgraph "Nmsdk-MotionControlLib"
        Emulator[NManipulatorInputEmulator]
        BaseInput[NManipulatorInput]
    end
    
    subgraph "Nmsdk-PulseLib"
        NSource[NSource]
    end
    
    subgraph "Rdk-BasicLib"
        UNet[UNet]
    end
    
    Emulator -->|наследуется от| BaseInput
    BaseInput -->|наследуется от| NSource
    NSource -->|наследуется от| UNet
    
    Input[Input<br/>Входной сигнал управления]
    Output[Output<br/>Выходное напряжение]
    
    Emulator --> Input
    Emulator --> Output
```

**Зависимости:**
- **Nmsdk-MotionControlLib**: базовый класс `NManipulatorInput`
- **Nmsdk-PulseLib**: базовый класс `NSource`
- **Rdk-BasicLib**: базовый фреймворк (`UNet`)

## Свойства компонента

### Параметры (ptPubParameter)

| Свойство | Тип | Значение по умолчанию | Описание |
|----------|-----|----------------------|----------|
| `Voltage` | `double` | `0` | Напряжение управления (устанавливается из Input) |

### Входы/Выходы

| Свойство | Тип | Флаги | Описание |
|----------|-----|-------|----------|
| `Input` | `MDMatrix<double>` | `ptInput \| ptPubState` | Входной сигнал управления |
| `Output` | `MDMatrix<double>` | `ptOutput \| ptPubState` | Выходное напряжение (наследуется от NSource) |

### Внутренние свойства

| Свойство | Тип | Описание |
|----------|-----|----------|
| `UpdateOutputFlag` | `bool` | Флаг обновления выхода (устанавливается в `true` при сбросе) |

## Методы компонента

### Жизненный цикл

#### `ADefault() -> bool`
**Назначение**: Инициализация параметров по умолчанию.

**Описание**: Устанавливает `Voltage=0` и вызывает `NManipulatorInput::ADefault()`.

#### `AReset() -> bool`
**Назначение**: Сброс состояния компонента.

**Описание**: Устанавливает `UpdateOutputFlag=true` и вызывает `NManipulatorInput::AReset()`.

#### `ACalculate() -> bool`
**Назначение**: Передача входного сигнала на выход.

**Алгоритм:**
1. Чтение входного сигнала: `Voltage = Input(0,0)`
2. Обновление выхода: `Output(0,0) = Voltage`

**Возвращаемое значение**: `true` при успешном выполнении

## Примеры использования

### C++ код

```cpp
#include "NManipulatorInputEmulator.h"

// Создание компонента
UEPtr<NManipulatorInputEmulator> emulator = storage->CreateComponent<NManipulatorInputEmulator>("ManInpEmu1");

// Инициализация
emulator->Default();
emulator->Build();
emulator->Reset();

// В цикле вычислений
while (simulation_running) {
    // Установка входного сигнала (например, от контроллера)
    emulator->Input(0, 0) = control_signal;
    
    emulator->Calculate();
    
    // Получение выходного напряжения
    double voltage = emulator->Output(0, 0);
    // или
    double voltage = emulator->Voltage;
}
```

### XML конфигурация

```xml
<Component>
    <ClassName>NManipulatorInputEmulator</ClassName>
    <Name>ManInpEmu1</Name>
    <Properties>
        <Voltage>0</Voltage>
    </Properties>
</Component>
```

## Связи с конфигурационными проектами

Компонент `NManipulatorInputEmulator` используется в проектах, требующих:
- **Эмуляции входных команд манипулятора** - для тестирования систем управления без физического устройства
- **Отладки систем управления** - для проверки работы контроллеров и алгоритмов управления
- **Демонстрации функциональности** - для показа работы системы без реального манипулятора

Типичные сценарии использования:
- Тестирование алгоритмов управления манипулятором
- Отладка систем с обратной связью
- Демонстрационные проекты
- Обучение работе с системами управления движением

**Связь с другими компонентами:**
- Может подключаться к выходам контроллеров (`NEngineMotionControl`, `NPositionControlElement`)
- Выходы подключаются к `NManipulator` или другим компонентам, требующим входного напряжения
- Часто используется вместе с `NManipulatorSourceEmulator` для полной эмуляции манипулятора

**Отличия от NManipulatorInput:**
- `NManipulatorInputEmulator` является упрощённой версией для эмуляции
- Основное отличие - упрощённая логика в `ACalculate` (простая передача сигнала)
- Предназначен для тестирования и отладки, а не для работы с реальным устройством

---

## NManipulatorInputEmulator — manipulator input emulator (EN)

**Class**: `NManipulatorInputEmulator` — emulator for manipulator input data, simplified version of `NManipulatorInput` for testing and debugging.  
**Registration**: `NMotionControlLibrary.cpp` → `UploadClass("NManipulatorInputEmulator", ...)`.  
**Base class**: `NManipulatorInput` (from Nmsdk-MotionControlLib).

NManipulatorInputEmulator is a simplified version of `NManipulatorInput` designed for emulating manipulator input commands. The component inherits all properties from the base class and simply passes the input signal to the output through voltage. Used for testing manipulator control systems without a real physical device.

## Class Diagram

```mermaid
classDiagram
    NSource <|-- NManipulatorInput
    NManipulatorInput <|-- NManipulatorInputEmulator
    class NManipulatorInputEmulator {
        +Voltage : double
        +Input : MDMatrix~double~
        +Output : MDMatrix~double~
        +New() NManipulatorInputEmulator*
        #ADefault() bool
        #AReset() bool
        #ACalculate() bool
    }
```

## Sequence Diagram

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Emulator as NManipulatorInputEmulator
    participant Source as SignalSource
    
    Storage->>Emulator: new NManipulatorInputEmulator()
    Storage->>Emulator: Default()
    Emulator->>Emulator: ADefault()
    loop Each calculation step
        Source->>Emulator: Input = control_signal
        Emulator->>Emulator: ACalculate()
        Note over Emulator: Voltage = Input(0,0)<br/>Output(0,0) = Voltage
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
    Calculating --> Ready: Step complete
```

## Activity Diagram

```mermaid
flowchart TD
    Start([Start ACalculate]) --> ReadInput[Read Input signal:<br/>Voltage = Input(0,0)]
    ReadInput --> UpdateOutput[Update Output:<br/>Output(0,0) = Voltage]
    UpdateOutput --> End([End])
```

## Properties

| Property | Type | Default | Description |
|----------|------|---------|-------------|
| `Voltage` | `double` | `0` | Control voltage (set from Input) |
| `Input` | `MDMatrix<double>` | - | Input control signal |
| `Output` | `MDMatrix<double>` | - | Output voltage |

## Methods

### Lifecycle Methods

#### `ADefault() -> bool`
**Purpose**: Initialize default parameters.

**Description**: Sets `Voltage=0` and calls `NManipulatorInput::ADefault()`.

#### `ACalculate() -> bool`
**Purpose**: Pass input signal to output.

**Description**: Reads input signal: `Voltage = Input(0,0)` and updates output: `Output(0,0) = Voltage`.

## Usage Examples

### C++ Code

```cpp
UEPtr<NManipulatorInputEmulator> emulator = storage->CreateComponent<NManipulatorInputEmulator>("ManInpEmu1");
emulator->Default();
emulator->Build();
emulator->Reset();

while (simulation_running) {
    emulator->Input(0, 0) = control_signal;
    emulator->Calculate();
    double voltage = emulator->Output(0, 0);
}
```

### XML Configuration

```xml
<Component>
    <ClassName>NManipulatorInputEmulator</ClassName>
    <Name>ManInpEmu1</Name>
    <Properties>
        <Voltage>0</Voltage>
    </Properties>
</Component>
```
