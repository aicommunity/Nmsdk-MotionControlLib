# NEngineControlRangeAfferent — афферентный контроллер (range)

**Класс**: `NEngineControlRangeAfferent` — вариант контроллера движения на базе диапазонных (range) афферентов.  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NEngineControlRangeAfferent", ...)`.  
**Базовый класс**: `NEngineMotionControl` (из Nmsdk-MotionControlLib).

NEngineControlRangeAfferent является специализированной версией NEngineMotionControl, настроенной для работы с range-афферентами (разделение сигналов по диапазонам значений). Компонент использует NIntervalSeparator для обработки афферентных сигналов.

## UML-диаграмма классов

```mermaid
classDiagram
    UNet <|-- NEngineMotionControl
    NEngineMotionControl <|-- NEngineControlRangeAfferent
    NEngineControlRangeAfferent *-- NIntervalSeparator : IntervalSeparators
    
    class NEngineControlRangeAfferent {
        +CreationMode : int
        +NumControlLoops : int
        +NumMotionElements : int
        +AfferentMin : vector~double~
        +AfferentMax : vector~double~
        +AfferentRangeMode : int
        +New() NEngineControlRangeAfferent*
        #ADefault() bool
        #ABuild() bool
        #AReset() bool
        #ACalculate() bool
    }
```

**Иерархия наследования:**
- `UNet` (Rdk Framework) — базовый класс для сетей компонентов
- `NEngineMotionControl` — базовый движок управления движением
- `NEngineControlRangeAfferent` — афферентный контроллер (range)

## UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Controller as NEngineControlRangeAfferent
    participant IntervalSep as NIntervalSeparator
    participant MotionElem as NMotionElement
    
    Storage->>Controller: new NEngineControlRangeAfferent()
    Storage->>Controller: Default()
    Controller->>Controller: ADefault()
    Note over Controller: CreationMode = 1 (Range)
    
    Storage->>Controller: Build()
    Controller->>Controller: ABuild()
    Controller->>IntervalSep: Создание NIntervalSeparator для афферентов
    Controller->>MotionElem: Создание элементов движения
    
    loop Каждый шаг вычислений
        Storage->>Controller: Calculate()
        Controller->>Controller: ACalculate()
        Controller->>IntervalSep: Обработка афферентных сигналов по диапазонам
        Controller->>MotionElem: Управление элементами движения
    end
```

## UML-диаграмма состояний

```mermaid
stateDiagram-v2
    [*] --> NotInitialized: Создание
    NotInitialized --> Initialized: ADefault()
    Initialized --> Building: ABuild()
    Building --> CreatingInterval: Создание IntervalSeparators
    CreatingInterval --> CreatingMotions: Создание элементов движения
    CreatingMotions --> Ready: Готов к работе
    Ready --> Calculating: ACalculate()
    Calculating --> ProcessingAfferents: Обработка range-афферентов
    ProcessingAfferents --> Ready: Завершение шага
    Ready --> Reset: AReset()
    Reset --> Ready: После сброса
    Ready --> [*]: Уничтожение
```

## UML-диаграмма активности

```mermaid
flowchart TD
    Start([Начало ABuild]) --> SetMode[Установка CreationMode = 1<br/>Range режим]
    SetMode --> CalcRanges[Вычисление диапазонов афферентов<br/>на основе AfferentMin, AfferentMax]
    CalcRanges --> CreateIntervalSep[Создание NIntervalSeparator<br/>для каждого диапазона]
    CreateIntervalSep --> CreateMotions[Создание элементов движения]
    CreateMotions --> LinkInterval[Связывание IntervalSeparator<br/>с афферентными нейронами]
    LinkInterval --> End([Конец])
```

## UML-диаграмма компонентов

```mermaid
graph TB
    Controller[[NEngineControlRangeAfferent]]
    MotionLib[Nmsdk-MotionControlLib<br/>NIntervalSeparator, NMotionElement]
    PulseLib[Nmsdk-PulseLib<br/>NAfferentNeuron, NPulseNeuron]
    
    Controller -->|использует| MotionLib
    Controller -->|использует| PulseLib
    
    IntervalSeparators[IntervalSeparators<br/>Разделители по диапазонам]
    MotionElements[MotionElements<br/>Элементы движения]
    
    Controller --> IntervalSeparators
    Controller --> MotionElements
```

## Свойства

### Параметры

[Аналогично NEngineMotionControl, но с CreationMode=1 для Range режима]

### Параметры диапазонов

| Свойство | Тип | Флаги | Описание | Значение по умолчанию |
|----------|-----|-------|----------|----------------------|
| `AfferentMin` | `std::vector<double>` | `ptPubParameter` | Минимальные значения афферентов по контурам | `[-π/2]` |
| `AfferentMax` | `std::vector<double>` | `ptPubParameter` | Максимальные значения афферентов по контурам | `[π/2]` |
| `AfferentRangeMode` | `int` | `ptPubParameter` | Режим расчета диапазонов (0-3) | `2` |

### Входы

[Аналогично NEngineMotionControl]

### Выходы

[Аналогично NEngineMotionControl]

## Методы

[Аналогично NEngineMotionControl]

## Примеры использования

### C++ код

```cpp
// NEngineControlRangeAfferent создается через NEngineMotionControl
UEPtr<NEngineMotionControl> controller = new NEngineMotionControl;
controller->CreationMode = 1;  // Range режим
controller->NumMotionElements = 2;
controller->NumControlLoops = 3;
controller->AfferentMin = {-M_PI/2};
controller->AfferentMax = {M_PI/2};
controller->AfferentRangeMode = 2;
controller->Build();
```

### XML конфигурация

```xml
<Object Name="EngineControlRangeAfferent" ClassName="NEngineControlRangeAfferent">
    <!-- Использует параметры NEngineMotionControl с CreationMode=1 -->
    <Property Name="AfferentMin" Value="-1.57" />
    <Property Name="AfferentMax" Value="1.57" />
    <Property Name="AfferentRangeMode" Value="2" />
</Object>
```

### Использование в конфигурациях

Компонент `NEngineControlRangeAfferent` используется как специализированная версия NEngineMotionControl для работы с range-афферентами.

**Типичные сценарии использования:**
1. **Управление с range-афферентами** - управление движением с использованием разделения сигналов по диапазонам
2. **Гранулярная обработка** - более детальная обработка афферентных сигналов по диапазонам

**Типичные комбинации:**
- `NEngineControlRangeAfferent` + `NIntervalSeparator` - обработка афферентных сигналов по диапазонам

---

# NEngineControlRangeAfferent — afferent range controller

**Class**: `NEngineControlRangeAfferent` — motion controller variant based on range afferents.  
**Registration**: `NMotionControlLibrary.cpp` → `UploadClass("NEngineControlRangeAfferent", ...)`.  
**Base class**: `NEngineMotionControl` (from Nmsdk-MotionControlLib).

NEngineControlRangeAfferent is a specialized version of NEngineMotionControl configured for working with range afferents (signal separation by value ranges). The component uses NIntervalSeparator for processing afferent signals.

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

## Usage Examples

[Same as RU section, with English comments]
