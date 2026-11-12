# NManipulatorAndGyro — манипулятор с гироскопом

## RU

**Класс**: `NManipulatorAndGyro` — компонент для моделирования манипулятора с гироскопом, вычисляющий результирующий момент с учетом внешнего момента, момента двигателя и гравитации.
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NManipulatorAndGyro", ...)`.
**Базовый класс**: `UNet` (из Rdk Framework).

NManipulatorAndGyro реализует модель манипулятора с учетом гравитации и гироскопических эффектов. Компонент вычисляет результирующий момент на основе внешнего момента, момента двигателя и гравитационного момента.

## UML-диаграмма классов

```mermaid
classDiagram
    UNet <|-- NManipulatorAndGyro
    class NManipulatorAndGyro {
        +Mass : double
        +Length : double
        +InputMomentumExt : MDMatrix~double~
        +InputMomentum : MDMatrix~double~
        +InputAngle : MDMatrix~double~
        +Output : MDMatrix~double~
        #gravity_constant : double
        +New() NManipulatorAndGyro*
        #ADefault() bool
        #ABuild() bool
        #AReset() bool
        #ACalculate() bool
    }
```

**Иерархия наследования:**
- `UNet` (Rdk Framework) — базовый класс для сетей компонентов
- `NManipulatorAndGyro` — манипулятор с гироскопом

## UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Manipulator as NManipulatorAndGyro
    participant Engine as Engine
    participant External as ExternalSource

    Storage->>Manipulator: new NManipulatorAndGyro()
    Storage->>Manipulator: Default()
    Manipulator->>Manipulator: ADefault()

    Storage->>Manipulator: Build()
    Manipulator->>Manipulator: ABuild()

    Storage->>Manipulator: Reset()
    Manipulator->>Manipulator: AReset()
    Note over Manipulator: gravity_constant = 9.8

    loop Каждый шаг вычислений
        Engine->>Manipulator: InputMomentum = engine_moment
        External->>Manipulator: InputMomentumExt = external_moment
        External->>Manipulator: InputAngle = angle
        Storage->>Manipulator: Calculate()
        Manipulator->>Manipulator: ACalculate()
        Note over Manipulator: Output = -external_moment + engine_moment - Mass*g*Length*sin(angle)
        Manipulator->>Engine: Output = resulting_moment
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
    Calculating --> ReadingInputs: Чтение входов
    ReadingInputs --> ComputingMoment: Вычисление момента
    ComputingMoment --> Ready: Завершение шага
    Ready --> Reset: AReset()
    Reset --> Ready: После сброса
    Ready --> [*]: Уничтожение
```

## UML-диаграмма активности

```mermaid
flowchart TD
    Start([Начало ACalculate]) --> ReadInputs["Чтение входов:<br/>external_moment, engine_moment, angle"]
    ReadInputs --> CalcGravity["Вычисление гравитационного момента:<br/>Mass * gravity_constant * Length * sin(angle)"]
    CalcGravity --> CalcOutput["Вычисление результирующего момента:<br/>Output = -external_moment + engine_moment - gravity_moment"]
    CalcOutput --> End([Конец])
```

## UML-диаграмма компонентов

```mermaid
graph TB
    Manipulator[[NManipulatorAndGyro]]
    BasicLib["Rdk-BasicLib<br/>Базовые компоненты"]

    Manipulator -->|использует| BasicLib

    InputMomentumExt["InputMomentumExt<br/>Внешний момент"]
    InputMomentum["InputMomentum<br/>Момент двигателя"]
    InputAngle["InputAngle<br/>Угол манипулятора"]
    Output["Output<br/>Результирующий момент"]

    Manipulator --> InputMomentumExt
    Manipulator --> InputMomentum
    Manipulator --> InputAngle
    Manipulator --> Output
```

## Свойства

### Параметры модели

| Свойство | Тип | Флаги | Описание | Значение по умолчанию |
|----------|-----|-------|----------|----------------------|
| `Mass` | `double` | `ptPubParameter` | Масса манипулятора (кг) | `1.0` |
| `Length` | `double` | `ptPubParameter` | Длина манипулятора (м) | `1.0` |

### Входы

| Свойство | Тип | Флаги | Описание | Источник данных |
|----------|-----|-------|----------|-----------------|
| `InputMomentumExt` | `MDMatrix<double>` | `ptInput \| ptPubState` | Внешний момент (Н·м) | Внешние источники |
| `InputMomentum` | `MDMatrix<double>` | `ptInput \| ptPubState` | Момент двигателя (Н·м) | Двигатель или привод |
| `InputAngle` | `MDMatrix<double>` | `ptInput \| ptPubState` | Угол манипулятора (рад) | Датчик угла или модель |

### Выходы

| Свойство | Тип | Флаги | Описание | Назначение |
|----------|-----|-------|----------|------------|
| `Output` | `MDMatrix<double>` | `ptOutput \| ptPubState` | Результирующий момент (Н·м) | Передача системам управления |

### Защищенные переменные

| Свойство | Тип | Описание |
|----------|-----|----------|
| `gravity_constant` | `double` | Константа гравитации (9.8 м/с²) |

## Методы

### Конструкторы и деструкторы

#### `NManipulatorAndGyro(void)`
**Назначение:** Конструктор компонента
**Параметры:** Нет
**Возвращаемое значение:** Нет
**Описание:** Инициализирует gravity_constant=0

#### `virtual ~NManipulatorAndGyro(void)`
**Назначение:** Деструктор компонента
**Параметры:** Нет
**Возвращаемое значение:** Нет

### Методы жизненного цикла

#### `virtual bool ADefault(void)`
**Назначение:** Инициализация значений по умолчанию
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Устанавливает Mass=1, Length=1, инициализирует входные и выходные матрицы нулями

#### `virtual bool ABuild(void)`
**Назначение:** Построение внутренней структуры компонента
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе

#### `virtual bool AReset(void)`
**Назначение:** Сброс состояния компонента
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Устанавливает gravity_constant=9.8

#### `virtual bool ACalculate(void)`
**Назначение:** Выполнение вычислений на текущем шаге
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Вычисляет результирующий момент:
- `Output = -InputMomentumExt + InputMomentum - Mass * gravity_constant * Length * sin(InputAngle)`

### Публичные методы

#### `virtual NManipulatorAndGyro* New(void)`
**Назначение:** Создание нового экземпляра компонента
**Параметры:** Нет
**Возвращаемое значение:** Указатель на новый экземпляр

## Примеры использования

### C++ код

```cpp
#include "NManipulatorAndGyro.h"

UEPtr<NManipulatorAndGyro> manipulator = new NManipulatorAndGyro;
manipulator->Default();
manipulator->Mass = 2.0;  // Масса 2 кг
manipulator->Length = 0.5;  // Длина 0.5 м
manipulator->Build();
manipulator->Reset();
```

### XML конфигурация

```xml
<Object Name="ManipulatorAndGyro" ClassName="NManipulatorAndGyro">
    <Property Name="Mass" Value="2.0" />
    <Property Name="Length" Value="0.5" />
    <Property Name="InputMomentum" Connect="Engine.OutputMoment" />
    <Property Name="InputAngle" Connect="AngleSensor.Output" />
</Object>
```

### Использование в конфигурациях

Компонент `NManipulatorAndGyro` используется для моделирования манипулятора с учетом гравитации и гироскопических эффектов.

**Типичные сценарии использования:**
1. **Моделирование манипулятора** - симуляция динамики манипулятора с гравитацией
2. **Интеграция с гироскопом** - использование данных гироскопа для управления

**Типичные комбинации:**
- `NManipulatorAndGyro` + `NAstaticGyro` - манипулятор с гироскопом
- `NManipulatorAndGyro` + двигатели - управление манипулятором

---

## EN

NManipulatorAndGyro — manipulator with gyroscope

**Class**: `NManipulatorAndGyro` — component for modeling a manipulator with a gyroscope, calculating resulting moment accounting for external moment, engine moment, and gravity.
**Registration**: `NMotionControlLibrary.cpp` → `UploadClass("NManipulatorAndGyro", ...)`.
**Base class**: `UNet` (from Rdk Framework).

NManipulatorAndGyro implements a manipulator model accounting for gravity and gyroscopic effects. The component calculates resulting moment based on external moment, engine moment, and gravitational moment.

## Class Diagram

```mermaid
classDiagram
    UNet <|-- NManipulatorAndGyro
    class NManipulatorAndGyro {
        +Mass : double
        +Length : double
        +InputMomentumExt : MDMatrix~double~
        +InputMomentum : MDMatrix~double~
        +InputAngle : MDMatrix~double~
        +Output : MDMatrix~double~
        #gravity_constant : double
        +New() NManipulatorAndGyro*
        #ADefault() bool
        #ABuild() bool
        #AReset() bool
        #ACalculate() bool
    }
```

## Sequence Diagram

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Manipulator as NManipulatorAndGyro
    participant Engine as Engine
    participant External as ExternalSource

    Storage->>Manipulator: new NManipulatorAndGyro()
    Storage->>Manipulator: Default()
    Manipulator->>Manipulator: ADefault()

    Storage->>Manipulator: Build()
    Manipulator->>Manipulator: ABuild()

    Storage->>Manipulator: Reset()
    Manipulator->>Manipulator: AReset()
    Note over Manipulator: gravity_constant = 9.8

    loop Each calculation step
        Engine->>Manipulator: InputMomentum = engine_moment
        External->>Manipulator: InputMomentumExt = external_moment
        External->>Manipulator: InputAngle = angle
        Storage->>Manipulator: Calculate()
        Manipulator->>Manipulator: ACalculate()
        Note over Manipulator: Output = -external_moment + engine_moment - Mass*g*Length*sin(angle)
        Manipulator->>Engine: Output = resulting_moment
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
    Calculating --> ReadingInputs: Reading inputs
    ReadingInputs --> ComputingMoment: Computing moment
    ComputingMoment --> Ready: Step complete
    Ready --> Reset: AReset()
    Reset --> Ready: After reset
    Ready --> [*]: Destroy
```

## Activity Diagram

```mermaid
flowchart TD
    Start([Start ACalculate]) --> ReadInputs["Reading inputs:<br/>external_moment, engine_moment, angle"]
    ReadInputs --> CalcGravity["Computing gravitational moment:<br/>Mass * gravity_constant * Length * sin(angle)"]
    CalcGravity --> CalcOutput["Computing resultant moment:<br/>Output = -external_moment + engine_moment - gravity_moment"]
    CalcOutput --> End([End])
```

## Component Diagram

```mermaid
graph TB
    Manipulator[[NManipulatorAndGyro]]
    BasicLib["Rdk-BasicLib<br/>Basic components"]

    Manipulator -->|uses| BasicLib

    InputMomentumExt["InputMomentumExt<br/>External moment"]
    InputMomentum["InputMomentum<br/>Engine moment"]
    InputAngle["InputAngle<br/>Manipulator angle"]
    Output["Output<br/>Resultant moment"]

    Manipulator --> InputMomentumExt
    Manipulator --> InputMomentum
    Manipulator --> InputAngle
    Manipulator --> Output
```

## Properties

[Same structure as RU section, translated to English]

## Methods

[Same structure as RU section, translated to English]

## References

- [Literature-References.md](../Literature-References.md): [A], 19, 21 — manipulator and gyroscope in the control hierarchy.

## Usage Examples

[Same as RU section, with English comments]
