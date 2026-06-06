# NEngineControlSignumAfferent — афферентный контроллер (signum)

**Класс**: `NEngineControlSignumAfferent` — вариант контроллера движения на базе signum-признаков (афферентные сигналы).
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NEngineControlSignumAfferent", ...)`.
**Базовый класс**: `NEngineMotionControl` (из Nmsdk-MotionControlLib).

NEngineControlSignumAfferent является специализированной версией NEngineMotionControl, настроенной для работы с signum-афферентами (разделение сигналов по знаку). Компонент использует NSignumSeparator для обработки афферентных сигналов.

## UML-диаграмма классов

```mermaid
classDiagram
    UNet <|-- NEngineMotionControl
    NEngineMotionControl <|-- NEngineControlSignumAfferent
    NEngineControlSignumAfferent *-- NSignumSeparator : SignumSeparators

    class NEngineControlSignumAfferent {
        +CreationMode : int
        +NumControlLoops : int
        +NumMotionElements : int
        +AfferentMin : vector~double~
        +AfferentMax : vector~double~
        +New() NEngineControlSignumAfferent*
        #ADefault() bool
        #ABuild() bool
        #AReset() bool
        #ACalculate() bool
    }
```

**Иерархия наследования:**
- `UNet` (Rdk Framework) — базовый класс для сетей компонентов
- `NEngineMotionControl` — базовый движок управления движением
- `NEngineControlSignumAfferent` — афферентный контроллер (signum)

## UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Controller as NEngineControlSignumAfferent
    participant SignumSep as NSignumSeparator
    participant MotionElem as NMotionElement

    Storage->>Controller: new NEngineControlSignumAfferent()
    Storage->>Controller: Default()
    Controller->>Controller: ADefault()
    Note over Controller: CreationMode = 0 (Signum)

    Storage->>Controller: Build()
    Controller->Controller: ABuild()
    Controller->>SignumSep: Создание NSignumSeparator для афферентов
    Controller->>MotionElem: Создание элементов движения

    loop Каждый шаг вычислений
        Storage->>Controller: Calculate()
        Controller->>Controller: ACalculate()
        Controller->>SignumSep: Обработка афферентных сигналов
        Controller->>MotionElem: Управление элементами движения
    end
```

## UML-диаграмма состояний

```mermaid
stateDiagram-v2
    [*] --> NotInitialized: Создание
    NotInitialized --> Initialized: ADefault()
    Initialized --> Building: ABuild()
    Building --> CreatingSignum: Создание SignumSeparators
    CreatingSignum --> CreatingMotions: Создание элементов движения
    CreatingMotions --> Ready: Готов к работе
    Ready --> Calculating: ACalculate()
    Calculating --> ProcessingAfferents: Обработка signum-афферентов
    ProcessingAfferents --> Ready: Завершение шага
    Ready --> Reset: AReset()
    Reset --> Ready: После сброса
    Ready --> [*]: Уничтожение
```

## UML-диаграмма активности

```mermaid
flowchart TD
    Start([Начало ABuild]) --> SetMode["Установка CreationMode = 0<br/>Signum режим"]
    SetMode --> CreateSignumSep["Создание NSignumSeparator<br/>для обработки афферентов"]
    CreateSignumSep --> CreateMotions[Создание элементов движения]
    CreateMotions --> LinkSignum["Связывание SignumSeparator<br/>с афферентными нейронами"]
    LinkSignum --> End([Конец])
```

## UML-диаграмма компонентов

```mermaid
graph TB
    Controller[[NEngineControlSignumAfferent]]
    MotionLib["Nmsdk-MotionControlLib<br/>NSignumSeparator, NMotionElement"]
    PulseLib["Nmsdk-PulseLib<br/>NAfferentNeuron, NPulseNeuron"]

    Controller -->|использует| MotionLib
    Controller -->|использует| PulseLib

    SignumSeparators["SignumSeparators<br/>Разделители по знаку"]
    MotionElements["MotionElements<br/>Элементы движения"]

    Controller --> SignumSeparators
    Controller --> MotionElements
```

## Свойства

### Параметры

[Аналогично NEngineMotionControl, но с CreationMode=0 для Signum режима]

### Входы

[Аналогично NEngineMotionControl]

### Выходы

[Аналогично NEngineMotionControl]

## Методы

[Аналогично NEngineMotionControl]

## Примеры использования

### C++ код

```cpp
// NEngineControlSignumAfferent создается через NEngineMotionControl
UEPtr<NEngineMotionControl> controller = new NEngineMotionControl;
controller->CreationMode = 0;  // Signum режим
controller->NumMotionElements = 2;
controller->NumControlLoops = 3;
controller->Build();
```

### XML конфигурация

```xml
<Object Name="EngineControlSignumAfferent" ClassName="NEngineControlSignumAfferent">
    <!-- Использует параметры NEngineMotionControl с CreationMode=0 -->
</Object>
```

### Использование в конфигурациях

Компонент `NEngineControlSignumAfferent` используется как специализированная версия NEngineMotionControl для работы с signum-афферентами.

**Типичные сценарии использования:**
1. **Управление с signum-афферентами** - управление движением с использованием разделения сигналов по знаку
2. **Упрощенная обработка** - упрощенная обработка афферентных сигналов

**Типичные комбинации:**
- `NEngineControlSignumAfferent` + `NSignumSeparator` - обработка афферентных сигналов

---

# NEngineControlSignumAfferent — afferent controller (signum)

**Class**: `NEngineControlSignumAfferent` — motion controller variant based on signum features (afferent signals).
**Registration**: `NMotionControlLibrary.cpp` → `UploadClass("NEngineControlSignumAfferent", ...)`.
**Base class**: `NEngineMotionControl` (from Nmsdk-MotionControlLib).

NEngineControlSignumAfferent is a specialized version of NEngineMotionControl configured for working with signum afferents (signal separation by sign). The component uses NSignumSeparator for processing afferent signals.

## Class Diagram

[Same as RU section]

## Sequence Diagram

[Same as RU section]

## State Diagram

[Same as RU section]

## Activity Diagram

[Same as RU section]

## Component Diagram

[Same as RU section]

## Properties

[Same structure as RU section, translated to English]

## Methods

[Same structure as RU section, translated to English]

## Источники

- [Literature-References.md](../Literature-References.md): [A], 19, 20, 21, 22, 27 — движок управления с афферентами, иерархия и моторная память.

## Usage Examples

[Same as RU section, with English comments]
