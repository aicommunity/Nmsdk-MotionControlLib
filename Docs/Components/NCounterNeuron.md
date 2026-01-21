## NCounterNeuron — счётчик/интегратор сигналов

**Класс**: `NCounterNeuron` — компонент-счётчик (интегрирует события/сигналы, ведёт счёт).  
**Регистрация**: `UploadClass("NCounterNeuron", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault**: обнуление счётчика.
- **ABuild**: подключение входа событий.
- **AReset**: сброс счётчика.
- **ACalculate**: обновление счётчика по входным событиям.

### I/O
- Вход: импульс/событие или условие.
- Выход: значение счётчика (скаляр).

```mermaid
classDiagram
    UComponent <|-- NCounterNeuron
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant Src as Source
    participant Cnt as NCounterNeuron
    Src-->>Cnt: event
    Cnt->>Cnt: increment
    Cnt-->>Src: count
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    e[Event] --> c[NCounterNeuron]
    c --> n[Count]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NCounterNeuron
Name = Counter1
```

---

## NCounterNeuron — counter/integrator (EN)

Counts events and outputs running count value.

```mermaid
classDiagram
    UComponent <|-- NCounterNeuron
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Src as Source
    participant Cnt as NCounterNeuron
    Src-->>Cnt: event
    Cnt-->>Src: count
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    e[Event] --> c[NCounterNeuron]
    c --> n[Count]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
