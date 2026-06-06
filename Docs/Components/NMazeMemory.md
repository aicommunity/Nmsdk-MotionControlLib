# NMazeMemory — память лабиринта

## RU

**Класс**: `NMazeMemory` — компонент для создания и управления памятью лабиринта с использованием элементов траектории (NTrajectoryElement), блоков множественного контроля позиции (NMultiPositionControl) и нейронных тренеров (NNeuronTrainer).
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NMazeMemory", ...)`.
**Базовый класс**: `UNet` (из Rdk Framework).

NMazeMemory реализует систему памяти лабиринта, которая запоминает пройденные пути, создает новые элементы траектории при обнаружении новых ситуаций, и использует нейронные тренеры для запоминания признаков ситуаций. Компонент создает сложную динамическую структуру для навигации в лабиринте.

## UML-диаграмма классов

```mermaid
classDiagram
    UNet <|-- NMazeMemory
    NMazeMemory *-- NTrajectoryElement : TrajectoryElements
    NMazeMemory *-- NMultiPositionControl : MultiPCs
    NMazeMemory *-- NNeuronTrainer : NTrainers
    NMazeMemory *-- NPulseNeuron : ActionNeurons

    class NMazeMemory {
        +Situation : bool
        +InputActions : vector~int~
        +ActionNeuronsType : string
        +FeaturesNum : int
        +IsDone : bool
        +SituationCoords : MDMatrix~double~
        +PassedTEsNames : vector~string~
        #ActionNeurons : vector~UEPtr~NPulseNeuron~~
        #TrajectoryElements : vector~UEPtr~NTrajectoryElement~~
        #ActivePIs : vector~UEPtr~NTrajectoryElement~~
        #MultiPCs : vector~UEPtr~NMultiPositionControl~~
        #NTrainers : vector~UEPtr~NNeuronTrainer~~
        #CurrentTE : int
        #CurrentNT : UEPtr~NNeuronTrainer~
        #CurrentLayer : int
        #BaseTE : UEPtr~NTrajectoryElement~
        #BaseMPC : UEPtr~NMultiPositionControl~
        #PrevTE : UEPtr~NTrajectoryElement~
        #PassedTEs : vector~UEPtr~NTrajectoryElement~~
        +SetSituation(value) bool
        +SetInputActions(value) bool
        +SetActionNeuronsType(value) bool
        +SetFeaturesNum(value) bool
        +New() NMazeMemory*
        #ADefault() bool
        #ABuild() bool
        #AReset() bool
        #ACalculate() bool
    }
```

**Иерархия наследования:**
- `UNet` (Rdk Framework) — базовый класс для сетей компонентов
- `NMazeMemory` — память лабиринта

## UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Memory as NMazeMemory
    participant TE as NTrajectoryElement
    participant MPC as NMultiPositionControl
    participant NT as NNeuronTrainer

    Storage->>Memory: new NMazeMemory()
    Storage->>Memory: Default()
    Memory->>Memory: ADefault()

    Storage->>Memory: Build()
    Memory->>Memory: ABuild()
    Memory->>TE: Создание начальных элементов траектории
    Memory->>MPC: Создание блоков MultiPC
    Memory->>NT: Создание нейронных тренеров

    loop Каждый шаг вычислений
        Storage->>Memory: Calculate()
        Memory->>Memory: ACalculate()
        alt Новая ситуация
            Memory->>NT: Запоминание признаков ситуации
            Memory->>TE: Создание нового элемента траектории
            Memory->>MPC: Создание нового блока MultiPC
        end
        Memory->>TE: Обновление текущего элемента траектории
    end
```

## UML-диаграмма состояний

```mermaid
stateDiagram-v2
    [*] --> NotInitialized: Создание
    NotInitialized --> Initialized: ADefault()
    Initialized --> Building: ABuild()
    Building --> CreatingStructure: Создание начальной структуры
    CreatingStructure --> Ready: Готов к работе
    Ready --> Calculating: ACalculate()
    Calculating --> CheckingSituation: Проверка ситуации
    CheckingSituation --> NewSituation: Новая ситуация?
    NewSituation -->|Да| Training: Обучение NeuronTrainer
    NewSituation -->|Нет| Updating: Обновление траектории
    Training --> CreatingTE: Создание нового TE
    CreatingTE --> CreatingMPC: Создание нового MPC
    CreatingMPC --> Updating: Обновление траектории
    Updating --> Ready: Завершение шага
    Ready --> Reset: AReset()
    Reset --> Ready: После сброса
    Ready --> [*]: Уничтожение
```

## UML-диаграмма активности

```mermaid
flowchart TD
    Start([Начало ACalculate]) --> CheckSituation{Situation?}
    CheckSituation -->|Да| CheckActions["InputActions<br/>доступны?"]
    CheckSituation -->|Нет| UpdateCurrent[Обновление текущего TE]
    CheckActions -->|Да| CheckTraining["NeuronTrainer<br/>обучен?"]
    CheckActions -->|Нет| UpdateCurrent
    CheckTraining -->|Нет| TrainNT["Обучение NeuronTrainer<br/>на SituationCoords"]
    CheckTraining -->|Да| CreateTE[Создание нового NTrajectoryElement]
    TrainNT --> WaitTraining[Ожидание завершения обучения]
    WaitTraining --> CreateTE
    CreateTE --> CreateMPC[Создание нового NMultiPositionControl]
    CreateMPC --> LinkComponents[Связывание компонентов]
    LinkComponents --> UpdateCurrent
    UpdateCurrent --> CheckFinish{IsDone?}
    CheckFinish -->|Да| End([Конец])
    CheckFinish -->|Нет| End
```

## UML-диаграмма компонентов

```mermaid
graph TB
    Memory[[NMazeMemory]]
    MotionLib["Nmsdk-MotionControlLib<br/>NTrajectoryElement, NMultiPositionControl"]
    PulseLib["Nmsdk-PulseLib<br/>NNeuronTrainer, NPulseNeuron"]
    BasicLib["Rdk-BasicLib<br/>Базовые компоненты"]

    Memory -->|использует| MotionLib
    Memory -->|использует| PulseLib
    Memory -->|использует| BasicLib

    TrajectoryElements["TrajectoryElements<br/>Элементы траектории"]
    MultiPCs["MultiPCs<br/>Блоки множественного контроля"]
    NTrainers["NTrainers<br/>Нейронные тренеры"]
    ActionNeurons["ActionNeurons<br/>Нейроны действий"]

    Memory --> TrajectoryElements
    Memory --> MultiPCs
    Memory --> NTrainers
    Memory --> ActionNeurons
```

## Свойства

### Параметры ситуации

| Свойство | Тип | Флаги | Описание | Значение по умолчанию |
|----------|-----|-------|----------|----------------------|
| `Situation` | `bool` | `ptPubParameter` | Флаг ситуации с множественным выбором (перекресток, развилка) | `false` |
| `InputActions` | `std::vector<int>` | `ptPubParameter` | Вектор возможных действий: [0]=направо, [1]=налево, [2]=прямо, [3]=стоп | `[0,0,0]` |
| `SituationCoords` | `MDMatrix<double>` | `ptPubParameter` | Координаты ситуации (x, y, alpha, ...) | Задается пользователем |
| `FeaturesNum` | `int` | `ptPubParameter` | Количество признаков ситуации | `4` |
| `ActionNeuronsType` | `string` | `ptPubParameter` | Тип нейронов действий | `"NSPNeuronGen"` |
| `IsDone` | `bool` | `ptPubParameter` | Флаг завершения работы (все варианты исследованы) | `false` |
| `PassedTEsNames` | `std::vector<string>` | `ptPubParameter` | Имена пройденных элементов траектории | Задается пользователем |

### Защищенные переменные

| Свойство | Тип | Описание |
|----------|-----|----------|
| `ActionNeurons` | `std::vector<UEPtr<NPulseNeuron>>` | Вектор нейронов действий |
| `TrajectoryElements` | `std::vector<UEPtr<NTrajectoryElement>>` | Вектор всех элементов траектории |
| `ActivePIs` | `std::vector<UEPtr<NTrajectoryElement>>` | Активные PostInput нейроны |
| `MultiPCs` | `std::vector<UEPtr<NMultiPositionControl>>` | Блоки множественного контроля позиции |
| `NTrainers` | `std::vector<UEPtr<NNeuronTrainer>>` | Нейронные тренеры для запоминания признаков |
| `CurrentTE` | `int` | Индекс текущего элемента траектории |
| `CurrentNT` | `UEPtr<NNeuronTrainer>` | Текущий нейронный тренер |
| `CurrentLayer` | `int` | Номер текущего слоя |
| `BaseTE` | `UEPtr<NTrajectoryElement>` | Текущий элемент траектории |
| `BaseMPC` | `UEPtr<NMultiPositionControl>` | MultiPC текущего элемента |
| `PrevTE` | `UEPtr<NTrajectoryElement>` | Предыдущий элемент траектории |
| `PassedTEs` | `std::vector<UEPtr<NTrajectoryElement>>` | Пройденные элементы траектории |

## Методы

### Конструкторы и деструкторы

#### `NMazeMemory(void)`
**Назначение:** Конструктор компонента
**Параметры:** Нет
**Возвращаемое значение:** Нет

#### `virtual ~NMazeMemory(void)`
**Назначение:** Деструктор компонента
**Параметры:** Нет
**Возвращаемое значение:** Нет

### Методы жизненного цикла

#### `virtual bool ADefault(void)`
**Назначение:** Инициализация значений по умолчанию
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Устанавливает Situation=false, InputActions=[0,0,0], ActionNeuronsType="NSPNeuronGen", FeaturesNum=4, IsDone=false, инициализирует внутренние переменные

#### `virtual bool ABuild(void)`
**Назначение:** Построение внутренней структуры компонента
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Создает начальную структуру:
1. Создает начальные элементы траектории, если их нет
2. Создает нейроны действий
3. Создает блоки MultiPC и нейронные тренеры
4. Создает связи между компонентами

#### `virtual bool AReset(void)`
**Назначение:** Сброс состояния компонента
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе

#### `virtual bool ACalculate(void)`
**Назначение:** Выполнение вычислений на текущем шаге
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:**
1. Проверяет наличие ситуации (Situation)
2. Если новая ситуация, обучает NeuronTrainer на SituationCoords
3. Создает новые элементы траектории и блоки MultiPC при необходимости
4. Обновляет текущий элемент траектории
5. Управляет переходами между элементами траектории

### Сеттеры свойств

Все сеттеры свойств обновляют соответствующие параметры внутренних компонентов.

### Публичные методы

#### `virtual NMazeMemory* New(void)`
**Назначение:** Создание нового экземпляра компонента
**Параметры:** Нет
**Возвращаемое значение:** Указатель на новый экземпляр

## Примеры использования

### C++ код

```cpp
#include "NMazeMemory.h"

UEPtr<NMazeMemory> memory = new NMazeMemory;
memory->Default();
memory->FeaturesNum = 4;  // x, y, alpha, calibration
memory->ActionNeuronsType = "NSPNeuronGen";
memory->Build();
```

### XML конфигурация

```xml
<Object Name="MazeMemory" ClassName="NMazeMemory">
    <Property Name="FeaturesNum" Value="4" />
    <Property Name="ActionNeuronsType" Value="NSPNeuronGen" />
    <Property Name="Situation" Value="false" />
    <Property Name="InputActions" Value="0,0,0" />
</Object>
```

### Использование в конфигурациях

Компонент `NMazeMemory` используется для создания и управления памятью лабиринта в системах навигации.

**Типичные сценарии использования:**
1. **Навигация в лабиринте** - запоминание пройденных путей и создание карты лабиринта
2. **Обучение навигации** - обучение системы навигации на основе опыта

**Типичные комбинации:**
- `NMazeMemory` + `NNavMousePrimitive` - навигация мыши с памятью
- `NMazeMemory` + `NTrajectoryElement` - построение графа траекторий

---

## EN

NMazeMemory — maze memory

**Class**: `NMazeMemory` — component for creating and managing maze memory using trajectory elements (NTrajectoryElement), multi-position control blocks (NMultiPositionControl), and neural trainers (NNeuronTrainer).
**Registration**: `NMotionControlLibrary.cpp` → `UploadClass("NMazeMemory", ...)`.
**Base class**: `UNet` (from Rdk Framework).

NMazeMemory implements a maze memory system that remembers traversed paths, creates new trajectory elements upon discovering new situations, and uses neural trainers to remember situation features. The component creates a complex dynamic structure for maze navigation.

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

## Источники

- [Literature-References.md](../Literature-References.md): [A], 22, 24 — моторная память, запоминание пространственных конфигураций.
