## NPCN — элемент/узел PCN (Nmsdk-MotionControlLib)

**Класс**: `NPCN` — компонент, связанный с PCN-элементами управления/сети (по регистрации библиотеки).  
**Регистрация**: `UploadClass("NPCN", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault**: параметры элемента.
- **ABuild**: подключение входов/выходов.
- **AReset**: сброс состояния.
- **ACalculate**: вычисление шага/выходов.

### I/O
- Вход: сигналы/состояния (вектор).
- Выход: вычисленные значения (вектор/скаляр).

```mermaid
classDiagram
    UComponent <|-- NPCN
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant In as Inputs
    participant P as NPCN
    In-->>P: signals
    P->>P: ACalculate()
    P-->>In: outputs
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    in[Inputs] --> p[NPCN]
    p --> out[Outputs]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NPCN
Name = PCN1
```

---

## NPCN — PCN node/element (EN)

PCN-related component producing outputs from input signals.

```mermaid
classDiagram
    UComponent <|-- NPCN
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant In as Inputs
    participant P as NPCN
    In-->>P: signals
    P-->>In: outputs
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    in[Inputs] --> p[NPCN]
    p --> out[Outputs]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
