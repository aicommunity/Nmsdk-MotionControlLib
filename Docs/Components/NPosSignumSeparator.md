## NPosSignumSeparator — положительная часть сигнала

**Класс**: `NPosSignumSeparator` — выделяет положительную составляющую (или канал) сигнала.  
**Регистрация**: `UploadClass("NPosSignumSeparator", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault/ABuild/AReset**: настройка и подготовка.
- **ACalculate**: вычисление положительной части.

### I/O
- Вход: скаляр/вектор.
- Выход: положительная часть/маска.

```mermaid
classDiagram
    UComponent <|-- NPosSignumSeparator
```

Диаграмма показывает специализированный разделитель знака.

```mermaid
sequenceDiagram
    participant Src as Source
    participant Sep as NPosSignumSeparator
    Src-->>Sep: value
    Sep->>Sep: ACalculate()
    Sep-->>Src: positive part
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    v[Value] --> p[NPosSignumSeparator]
    p --> out[Positive]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NPosSignumSeparator
Name = PosSep1
```

---

## NPosSignumSeparator — positive channel extractor (EN)

Extracts positive part of a signal.

```mermaid
classDiagram
    UComponent <|-- NPosSignumSeparator
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Src as Source
    participant Sep as NPosSignumSeparator
    Src-->>Sep: value
    Sep-->>Src: positive part
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    v[Value] --> p[NPosSignumSeparator]
    p --> out[Positive]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
