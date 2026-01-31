# NObjInArea — объект в области

**Класс**: `NObjInArea` — компонент для обнаружения объектов в ограниченной области изображения с использованием нейросетевой структуры и блоков подавления.
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NObjInArea", ...)`.
**Базовый класс**: `UNet` (из Rdk Framework).

NObjInArea обнаруживает объекты в ограниченной области изображения, используя генераторы последовательностей импульсов, блоки подавления (NSuppressionUnit) и нейроны для принятия решений. Компонент создает сложную структуру для обработки визуальной информации.

## UML-диаграмма классов

```mermaid
classDiagram
    UNet <|-- NObjInArea
    NObjInArea *-- NPulseGeneratorMulti : ClsSpikeFr
    NObjInArea *-- NSuppressionUnit : SuppressUnit1
    NObjInArea *-- NSuppressionUnit : SuppressUnit2
    NObjInArea *-- NPulseNeuron : ORNeuron
    NObjInArea *-- NPulseNeuron : ANDNeuron
    NObjInArea *-- NPulseNeuron : DecidingNeuron
    NObjInArea *-- NPulseGeneratorTransit : ExcitatoryGen

    class NObjInArea {
        +NeuronClassName : string
        +MultiGeneratorClassName : string
        +PulseLength : double
        +Amplitude : double
        +Frequency : double
        +HighFreq : double
        +Delay11 : double
        +Delay12 : double
        +Delay21 : double
        +Delay22 : double
        +PulseCount : int
        +NumObj : int
        +DelaysClsSpikeFr : MDMatrix~double~
        +Output : MDMatrix~double~
        #ClsSpikeFr : vector~UEPtr~NPulseGeneratorMulti~~
        #OldNumObj : int
        #SuppressUnit1 : UEPtr~NSuppressionUnit~
        #SuppressUnit2 : UEPtr~NSuppressionUnit~
        #ORNeuron : UEPtr~NPulseNeuron~
        #ANDNeuron : UEPtr~NPulseNeuron~
        #DecidingNeuron : UEPtr~NPulseNeuron~
        #ExcitatoryGen : UEPtr~NPulseGeneratorTransit~
        #Relinked : bool
        +New() NObjInArea*
        #ADefault() bool
        #ABuild() bool
        #AReset() bool
        #ACalculate() bool
    }
```

**Иерархия наследования:**
- `UNet` (Rdk Framework) — базовый класс для сетей компонентов
- `NObjInArea` — обнаружение объектов в области

## UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Detector as NObjInArea
    participant ClsGen as ClsSpikeFr
    participant Suppress1 as SuppressUnit1
    participant Suppress2 as SuppressUnit2
    participant ORNeuron as ORNeuron
    participant ANDNeuron as ANDNeuron
    participant Deciding as DecidingNeuron

    Storage->>Detector: new NObjInArea()
    Storage->>Detector: Default()
    Detector->>Detector: ADefault()

    Storage->>Detector: Build()
    Detector->>Detector: ABuild()
    Detector->>ClsGen: Создание NumObj генераторов ClsSpikeFr
    Detector->>Suppress1: new NSuppressionUnit("SuppressUnit1")
    Detector->>Suppress2: new NSuppressionUnit("SuppressUnit2")
    Detector->>ORNeuron: new NPulseNeuron("ORNeuron")
    Detector->>ANDNeuron: new NPulseNeuron("ANDNeuron")
    Detector->>Deciding: new NPulseNeuron("DecidingNeuron")
    Detector->>Detector: Создание связей между компонентами

    loop Каждый шаг вычислений
        Storage->>Detector: Calculate()
        Detector->>Detector: ACalculate()
        Note over Detector: Обнаружение объектов в области
        Detector->>Deciding: Output = результат обнаружения
    end
```

## UML-диаграмма состояний

```mermaid
stateDiagram-v2
    [*] --> NotInitialized: Создание
    NotInitialized --> Initialized: ADefault()
    Initialized --> Building: ABuild()
    Building --> CreatingGenerators: Создание генераторов
    CreatingGenerators --> CreatingSuppress: Создание блоков подавления
    CreatingSuppress --> CreatingNeurons: Создание нейронов
    CreatingNeurons --> CreatingLinks: Создание связей
    CreatingLinks --> Ready: Готов к работе
    Ready --> Calculating: ACalculate()
    Calculating --> Detecting: Обнаружение объектов
    Detecting --> Ready: Завершение шага
    Ready --> Reset: AReset()
    Reset --> Ready: После сброса
    Ready --> [*]: Уничтожение
```

## UML-диаграмма активности

```mermaid
flowchart TD
    Start([Начало ABuild]) --> CheckObj{NumObj<br/>изменилось?}
    CheckObj -->|Да| DeleteOld[Удаление старых генераторов]
    CheckObj -->|Нет| CreateGenerators
    DeleteOld --> CreateGenerators[Создание NumObj генераторов ClsSpikeFr<br/>с задержками из DelaysClsSpikeFr]
    CreateGenerators --> CreateSuppress1[Создание SuppressUnit1<br/>с Delay1=Delay11, Delay2=Delay12]
    CreateSuppress1 --> CreateSuppress2[Создание SuppressUnit2<br/>с Delay1=Delay21, Delay2=Delay22]
    CreateSuppress2 --> CreateOR[Создание ORNeuron]
    CreateOR --> CreateAND[Создание ANDNeuron]
    CreateAND --> CreateDeciding[Создание DecidingNeuron]
    CreateDeciding --> CreateExcitatory[Создание ExcitatoryGen]
    CreateExcitatory --> LinkComponents[Создание связей между компонентами]
    LinkComponents --> End([Конец])
```

## UML-диаграмма компонентов

```mermaid
graph TB
    Detector[[NObjInArea]]
    PulseLib[Nmsdk-PulseLib<br/>NPulseGenerator, NPulseNeuron]
    MotionLib[Nmsdk-MotionControlLib<br/>NSuppressionUnit]
    BasicLib[Rdk-BasicLib<br/>Базовые компоненты]

    Detector -->|использует| PulseLib
    Detector -->|использует| MotionLib
    Detector -->|использует| BasicLib

    ClsGenerators[ClsSpikeFr<br/>Генераторы последовательностей]
    Suppress1[SuppressUnit1<br/>Блок подавления 1]
    Suppress2[SuppressUnit2<br/>Блок подавления 2]
    ORNeuron[ORNeuron<br/>Нейрон ИЛИ]
    ANDNeuron[ANDNeuron<br/>Нейрон И]
    DecidingNeuron[DecidingNeuron<br/>Нейрон принятия решения]

    Detector --> ClsGenerators
    Detector --> Suppress1
    Detector --> Suppress2
    Detector --> ORNeuron
    Detector --> ANDNeuron
    Detector --> DecidingNeuron
```

## Свойства

### Параметры классов

| Свойство | Тип | Флаги | Описание | Значение по умолчанию |
|----------|-----|-------|----------|----------------------|
| `NeuronClassName` | `std::string` | `ptPubParameter` | Имя класса нейрона | Задается пользователем |
| `MultiGeneratorClassName` | `std::string` | `ptPubParameter` | Имя класса генератора последовательностей | Задается пользователем |

### Параметры импульсов

| Свойство | Тип | Флаги | Описание | Значение по умолчанию |
|----------|-----|-------|----------|----------------------|
| `PulseLength` | `double` | `ptPubParameter` | Длительность импульсов (с) | Задается пользователем |
| `Amplitude` | `double` | `ptPubParameter` | Амплитуда импульсов | Задается пользователем |
| `Frequency` | `double` | `ptPubParameter` | Частота генерации (Гц) | Задается пользователем |
| `HighFreq` | `double` | `ptPubParameter` | Высокая частота (Гц) | Задается пользователем |

### Параметры задержек

| Свойство | Тип | Флаги | Описание | Значение по умолчанию |
|----------|-----|-------|----------|----------------------|
| `Delay11` | `double` | `ptPubParameter` | Задержка 11 (начало подавления 1) | Задается пользователем |
| `Delay12` | `double` | `ptPubParameter` | Задержка 12 (конец подавления 1) | Задается пользователем |
| `Delay21` | `double` | `ptPubParameter` | Задержка 21 (начало подавления 2) | Задается пользователем |
| `Delay22` | `double` | `ptPubParameter` | Задержка 22 (конец подавления 2) | Задается пользователем |

### Параметры последовательностей

| Свойство | Тип | Флаги | Описание | Значение по умолчанию |
|----------|-----|-------|----------|----------------------|
| `PulseCount` | `int` | `ptPubParameter` | Количество импульсов в последовательности | Задается пользователем |
| `NumObj` | `int` | `ptPubParameter` | Количество объектов для обнаружения | Задается пользователем |
| `DelaysClsSpikeFr` | `MDMatrix<double>` | `ptPubParameter` | Задержки для генераторов последовательностей | Задается пользователем |

### Выходы

| Свойство | Тип | Флаги | Описание | Назначение |
|----------|-----|-------|----------|------------|
| `Output` | `MDMatrix<double>` | `ptOutput \| ptPubState` | Результат обнаружения объектов | Передача другим компонентам |

### Защищенные переменные

| Свойство | Тип | Описание |
|----------|-----|----------|
| `ClsSpikeFr` | `std::vector<UEPtr<NPulseGeneratorMulti>>` | Вектор генераторов последовательностей |
| `OldNumObj` | `int` | Предыдущее количество объектов |
| `SuppressUnit1` | `UEPtr<NSuppressionUnit>` | Блок подавления 1 |
| `SuppressUnit2` | `UEPtr<NSuppressionUnit>` | Блок подавления 2 |
| `ORNeuron` | `UEPtr<NPulseNeuron>` | Нейрон ИЛИ |
| `ANDNeuron` | `UEPtr<NPulseNeuron>` | Нейрон И |
| `DecidingNeuron` | `UEPtr<NPulseNeuron>` | Нейрон принятия решения |
| `ExcitatoryGen` | `UEPtr<NPulseGeneratorTransit>` | Возбуждающий генератор |
| `Relinked` | `bool` | Флаг пересвязывания |

## Методы

### Конструкторы и деструкторы

#### `NObjInArea(void)`
**Назначение:** Конструктор компонента
**Параметры:** Нет
**Возвращаемое значение:** Нет
**Описание:** Инициализирует OldNumObj=0, очищает ClsSpikeFr, устанавливает все указатели в NULL, Relinked=false

#### `virtual ~NObjInArea(void)`
**Назначение:** Деструктор компонента
**Параметры:** Нет
**Возвращаемое значение:** Нет

### Методы жизненного цикла

#### `virtual bool ADefault(void)`
**Назначение:** Инициализация значений по умолчанию
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Устанавливает значения по умолчанию для всех параметров

#### `virtual bool ABuild(void)`
**Назначение:** Построение внутренней структуры компонента
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Создает структуру:
1. Удаляет старые генераторы, если NumObj изменилось
2. Создает NumObj генераторов ClsSpikeFr с задержками из DelaysClsSpikeFr
3. Создает SuppressUnit1 и SuppressUnit2 с соответствующими задержками
4. Создает ORNeuron, ANDNeuron, DecidingNeuron
5. Создает ExcitatoryGen
6. Создает связи между компонентами

#### `virtual bool AReset(void)`
**Назначение:** Сброс состояния компонента
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе

#### `virtual bool ACalculate(void)`
**Назначение:** Выполнение вычислений на текущем шаге
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Вычисления выполняются нейронами внутри структуры автоматически

### Сеттеры свойств

Все сеттеры свойств обновляют параметры соответствующих генераторов, блоков подавления и нейронов при их существовании.

### Публичные методы

#### `virtual NObjInArea* New(void)`
**Назначение:** Создание нового экземпляра компонента
**Параметры:** Нет
**Возвращаемое значение:** Указатель на новый экземпляр

## Примеры использования

### C++ код

```cpp
#include "NObjInArea.h"

UEPtr<NObjInArea> detector = new NObjInArea;
detector->Default();
detector->NumObj = 3;
detector->PulseCount = 5;
detector->Delay11 = 0.0;
detector->Delay12 = 0.1;
detector->Delay21 = 0.2;
detector->Delay22 = 0.3;
// Настройка DelaysClsSpikeFr
detector->Build();
```

### XML конфигурация

```xml
<Object Name="ObjInArea" ClassName="NObjInArea">
    <Property Name="NumObj" Value="3" />
    <Property Name="PulseCount" Value="5" />
    <Property Name="Delay11" Value="0.0" />
    <Property Name="Delay12" Value="0.1" />
    <Property Name="Delay21" Value="0.2" />
    <Property Name="Delay22" Value="0.3" />
</Object>
```

### Использование в конфигурациях

Компонент `NObjInArea` используется для обнаружения объектов в ограниченной области изображения.

**Типичные сценарии использования:**
1. **Обнаружение объектов** - обнаружение объектов в заданной области изображения
2. **Обработка визуальной информации** - обработка визуальных данных для обнаружения объектов

**Типичные комбинации:**
- `NObjInArea` + `NEyeRetina` - обнаружение объектов на основе данных ретины
- `NObjInArea` + источники изображений - обработка визуальной информации

---

# NObjInArea — object in area

**Class**: `NObjInArea` — component for detecting objects in a restricted image area using a neural network structure and suppression units.
**Registration**: `NMotionControlLibrary.cpp` → `UploadClass("NObjInArea", ...)`.
**Base class**: `UNet` (from Rdk Framework).

NObjInArea detects objects in a restricted image area using pulse sequence generators, suppression units (NSuppressionUnit), and decision neurons. The component creates a complex structure for processing visual information.

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

- [Literature-References.md](../Literature-References.md): [A], 13, 19 — обнаружение объектов, восприятие и управление.

## Usage Examples

[Same as RU section, with English comments]
