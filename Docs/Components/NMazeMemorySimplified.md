# NMazeMemorySimplified — упрощенная память лабиринта

## RU

**Класс**: `NMazeMemorySimplified` — упрощенная версия компонента памяти лабиринта с использованием элементов траектории (NTrajectoryElement), блоков множественного контроля позиции (NMultiPositionControl) и нейронных тренеров (NNeuronTrainer).
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NMazeMemorySimplified", ...)`.
**Базовый класс**: `UNet` (из Rdk Framework).

NMazeMemorySimplified является упрощенной версией NMazeMemory, реализующей систему памяти лабиринта с упрощенной логикой создания и управления элементами траектории.

## UML-диаграмма классов

```mermaid
classDiagram
    UNet <|-- NMazeMemorySimplified
    NMazeMemorySimplified *-- NTrajectoryElement : TrajectoryElements
    NMazeMemorySimplified *-- NMultiPositionControl : MultiPCs
    NMazeMemorySimplified *-- NNeuronTrainer : NTrainers
    NMazeMemorySimplified *-- NPulseNeuron : ActionNeurons

    class NMazeMemorySimplified {
        +Situation : bool
        +InputActions : vector~int~
        +ActionNeuronsType : string
        +FeaturesNum : int
        +IsDone : bool
        +SituationCoords : MDMatrix~double~
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
        +New() NMazeMemorySimplified*
        #ADefault() bool
        #ABuild() bool
        #AReset() bool
        #ACalculate() bool
    }
```

**Иерархия наследования:**
- `UNet` (Rdk Framework) — базовый класс для сетей компонентов
- `NMazeMemorySimplified` — упрощенная память лабиринта

## UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Memory as NMazeMemorySimplified
    participant TE as NTrajectoryElement
    participant MPC as NMultiPositionControl
    participant NT as NNeuronTrainer

    Storage->>Memory: new NMazeMemorySimplified()
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
    CheckTraining -->|Нет| TrainNT[Обучение NeuronTrainer]
    CheckTraining -->|Да| CreateTE[Создание нового TE]
    TrainNT --> CreateTE
    CreateTE --> CreateMPC[Создание нового MPC]
    CreateMPC --> UpdateCurrent
    UpdateCurrent --> End([Конец])
```

## UML-диаграмма компонентов

```mermaid
graph TB
    Memory[[NMazeMemorySimplified]]
    MotionLib["Nmsdk-MotionControlLib<br/>NTrajectoryElement, NMultiPositionControl"]
    PulseLib["Nmsdk-PulseLib<br/>NNeuronTrainer, NPulseNeuron"]
    BasicLib["Rdk-BasicLib<br/>Базовые компоненты"]

    Memory -->|использует| MotionLib
    Memory -->|использует| PulseLib
    Memory -->|использует| BasicLib

    TrajectoryElements["TrajectoryElements<br/>Элементы траектории"]
    MultiPCs["MultiPCs<br/>Блоки множественного контроля"]
    NTrainers["NTrainers<br/>Нейронные тренеры"]

    Memory --> TrajectoryElements
    Memory --> MultiPCs
    Memory --> NTrainers
```

## Свойства

### Параметры ситуации

| Свойство | Тип | Флаги | Описание | Значение по умолчанию |
|----------|-----|-------|----------|----------------------|
| `Situation` | `bool` | `ptPubParameter` | Флаг ситуации с множественным выбором | `false` |
| `InputActions` | `std::vector<int>` | `ptPubParameter` | Вектор возможных действий | `[0,0,0]` |
| `SituationCoords` | `MDMatrix<double>` | `ptPubParameter` | Координаты ситуации | Задается пользователем |
| `FeaturesNum` | `int` | `ptPubParameter` | Количество признаков ситуации | `4` |
| `ActionNeuronsType` | `string` | `ptPubParameter` | Тип нейронов действий | `"NSPNeuronGen"` |
| `IsDone` | `bool` | `ptPubParameter` | Флаг завершения работы | `false` |

### Защищенные переменные

[Аналогично NMazeMemory, но без некоторых сложных механизмов]

## Методы

### Конструкторы и деструкторы

#### `NMazeMemorySimplified(void)`
**Назначение:** Конструктор компонента
**Параметры:** Нет
**Возвращаемое значение:** Нет

#### `virtual ~NMazeMemorySimplified(void)`
**Назначение:** Деструктор компонента
**Параметры:** Нет
**Возвращаемое значение:** Нет

### Методы жизненного цикла

#### `virtual bool ADefault(void)`
**Назначение:** Инициализация значений по умолчанию
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Аналогично NMazeMemory, но без некоторых сложных механизмов

#### `virtual bool ABuild(void)`
**Назначение:** Построение внутренней структуры компонента
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Создает упрощенную структуру аналогично NMazeMemory

#### `virtual bool AReset(void)`
**Назначение:** Сброс состояния компонента
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе

#### `virtual bool ACalculate(void)`
**Назначение:** Выполнение вычислений на текущем шаге
**Параметры:** Нет
**Возвращаемое значение:** `true` при успехе
**Описание:** Упрощенная логика по сравнению с NMazeMemory

### Публичные методы

#### `virtual NMazeMemorySimplified* New(void)`
**Назначение:** Создание нового экземпляра компонента
**Параметры:** Нет
**Возвращаемое значение:** Указатель на новый экземпляр

## Примеры использования

### C++ код

```cpp
#include "NMazeMemorySimplified.h"

UEPtr<NMazeMemorySimplified> memory = new NMazeMemorySimplified;
memory->Default();
memory->FeaturesNum = 4;
memory->Build();
```

### XML конфигурация

```xml
<Object Name="MazeMemorySimplified" ClassName="NMazeMemorySimplified">
    <Property Name="FeaturesNum" Value="4" />
    <Property Name="ActionNeuronsType" Value="NSPNeuronGen" />
</Object>
```

### Использование в конфигурациях

Компонент `NMazeMemorySimplified` используется как упрощенная версия NMazeMemory для навигации в лабиринте.

**Типичные сценарии использования:**
1. **Упрощенная навигация** - навигация в лабиринте с упрощенной логикой
2. **Быстрое прототипирование** - быстрое создание систем навигации

**Типичные комбинации:**
- `NMazeMemorySimplified` + `NNavMousePrimitive` - упрощенная навигация мыши

---

## EN

NMazeMemorySimplified — simplified maze memory

**Class**: `NMazeMemorySimplified` — simplified version of maze memory component using trajectory elements (NTrajectoryElement), multi-position control blocks (NMultiPositionControl), and neural trainers (NNeuronTrainer).
**Registration**: `NMotionControlLibrary.cpp` → `UploadClass("NMazeMemorySimplified", ...)`.
**Base class**: `UNet` (from Rdk Framework).

NMazeMemorySimplified is a simplified version of NMazeMemory, implementing a maze memory system with simplified logic for creating and managing trajectory elements.

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

- [Literature-References.md](../Literature-References.md): [A], 22, 24 — память лабиринта, пространственные конфигурации.
