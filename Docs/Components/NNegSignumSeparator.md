## NNegSignumSeparator — отрицательная часть сигнала

**Класс**: `NNegSignumSeparator` — выделяет отрицательную составляющую (или канал) сигнала.  
**Регистрация**: `UploadClass("NNegSignumSeparator", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault/ABuild/AReset**: настройка и подготовка.
- **ACalculate**: вычисление отрицательной части (обычно с сохранением знака или модуля — по конфигу).

### I/O
- Вход: скаляр/вектор.
- Выход: отрицательная часть/маска.

```mermaid
classDiagram
    UComponent <|-- NNegSignumSeparator
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant Src as Source
    participant Sep as NNegSignumSeparator
    Src-->>Sep: value
    Sep->>Sep: ACalculate()
    Sep-->>Src: negative part
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    v[Value] --> n[NNegSignumSeparator]
    n --> out[Negative]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NNegSignumSeparator
Name = NegSep1
```

---

## NNegSignumSeparator — negative channel extractor (EN)

Extracts negative part of a signal.

```mermaid
classDiagram
    UComponent <|-- NNegSignumSeparator
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Src as Source
    participant Sep as NNegSignumSeparator
    Src-->>Sep: value
    Sep-->>Src: negative part
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    v[Value] --> n[NNegSignumSeparator]
    n --> out[Negative]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
