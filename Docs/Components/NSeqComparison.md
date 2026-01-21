## NSeqComparison — сравнение последовательностей

**Класс**: `NSeqComparison` — сравнение последовательностей сигналов/состояний (например, для оценки совпадения траекторий).  
**Регистрация**: `UploadClass("NSeqComparison", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault**: настройка метрики сравнения.
- **ABuild**: подключение двух входных последовательностей.
- **AReset**: сброс накопителей.
- **ACalculate**: расчёт меры сходства/ошибки.

### I/O
- Вход: две последовательности (векторы/скаляры по шагам).
- Выход: мера сходства/ошибка.

```mermaid
classDiagram
    UComponent <|-- NSeqComparison
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant A as SeqA
    participant C as NSeqComparison
    participant B as SeqB
    A-->>C: values
    B-->>C: values
    C->>C: ACalculate()
    C-->>A: similarity/error
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    a[SeqA] --> c[NSeqComparison]
    b[SeqB] --> c
    c --> m[Metric]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NSeqComparison
Name = SeqCmp1
```

---

## NSeqComparison — sequence comparison (EN)

Compares two signal sequences and outputs similarity/error metric.

```mermaid
classDiagram
    UComponent <|-- NSeqComparison
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant A as SeqA
    participant C as NSeqComparison
    participant B as SeqB
    A-->>C: values
    B-->>C: values
    C-->>A: metric
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    a[SeqA] --> c[NSeqComparison]
    b[SeqB] --> c
    c --> m[Metric]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
