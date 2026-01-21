## NManipulatorSource — источник данных манипулятора

**Класс**: `NManipulatorSource` — предоставляет входные данные/состояния манипулятора (позиции, датчики, параметры).  
**Регистрация**: `UploadClass("NManipulatorSource", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault**: инициализация источника.
- **ABuild**: подключение к датчикам/эмуляторам.
- **AReset**: сброс состояния.
- **ACalculate**: выдача актуальных данных.

### I/O
- Выход: состояния/наблюдения для `NManipulator` и контроллеров.

```mermaid
classDiagram
    UComponent <|-- NManipulatorSource
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant Src as NManipulatorSource
    participant Man as NManipulator
    loop step
        Src->>Src: ACalculate()
        Src-->>Man: state/obs
    end
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    src[NManipulatorSource] --> obs[Manipulator state]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NManipulatorSource
Name = ManSrc1
```

---

## NManipulatorSource — manipulator state source (EN)

Provides state/observation data for manipulator controllers.

```mermaid
classDiagram
    UComponent <|-- NManipulatorSource
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Src as NManipulatorSource
    participant Man as NManipulator
    Src-->>Man: state/obs
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    src[NManipulatorSource] --> obs[Manipulator state]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
