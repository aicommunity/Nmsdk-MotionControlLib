## NSuppressionUnit — подавляющий блок

**Класс**: `NSuppressionUnit` — подавляет/ограничивает сигнал (например, при конфликтующих командах).  
**Регистрация**: `UploadClass("NSuppressionUnit", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault**: параметры подавления/порогов.
- **ABuild**: подключение входов.
- **AReset**: сброс.
- **ACalculate**: подавление/маскирование сигналов.

### I/O
- Вход: сигнал/команды.
- Выход: подавленный сигнал.

```mermaid
classDiagram
    UComponent <|-- NSuppressionUnit
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant Src as Source
    participant Sup as NSuppressionUnit
    Src-->>Sup: signal
    Sup->>Sup: ACalculate()
    Sup-->>Src: suppressed signal
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    s[Signal] --> sup[NSuppressionUnit]
    sup --> o[Suppressed]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NSuppressionUnit
Name = Supp1
```

---

## NSuppressionUnit — suppression unit (EN)

Applies suppression/masking to input signals.

```mermaid
classDiagram
    UComponent <|-- NSuppressionUnit
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Src as Source
    participant Sup as NSuppressionUnit
    Src-->>Sup: signal
    Sup-->>Src: suppressed signal
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    s[Signal] --> sup[NSuppressionUnit]
    sup --> o[Suppressed]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
