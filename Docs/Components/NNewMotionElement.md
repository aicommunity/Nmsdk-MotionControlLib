## NNewMotionElement — новый элемент движения

**Класс**: `NNewMotionElement` — элемент движения (новая реализация/вариант).  
**Регистрация**: `UploadClass("NNewMotionElement", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault**: параметры элемента.
- **ABuild**: подключение к двигателям/состоянию.
- **AReset**: сброс состояния.
- **ACalculate**: расчёт шага движения.

### I/O
- Вход: цели/ошибки/сенсоры.
- Выход: команды движения.

```mermaid
classDiagram
    UComponent <|-- NNewMotionElement
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant C as Controller
    participant M as NNewMotionElement
    C-->>M: targets
    M->>M: ACalculate()
    M-->>C: motion command
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    t[Targets] --> m[NNewMotionElement]
    m --> cmd[Motion command]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NNewMotionElement
Name = MotionEl1
```

---

## NNewMotionElement — new motion element (EN)

Computes a motion command from targets/sensor inputs.

```mermaid
classDiagram
    UComponent <|-- NNewMotionElement
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant C as Controller
    participant M as NNewMotionElement
    C-->>M: targets
    M-->>C: motion command
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    t[Targets] --> m[NNewMotionElement]
    m --> cmd[Motion command]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
