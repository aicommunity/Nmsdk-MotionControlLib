## NSignumSeparator — разделитель по знаку (Nmsdk-MotionControlLib)

**Класс**: `NSignumSeparator` — разделяет входной сигнал по знаку/порогам (например, на положительную/отрицательную части).  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NSignumSeparator", ...)`.

### Lifecycle
- **ADefault**: инициализация порога/режима разделения.
- **ABuild**: проверка подключения входа и выходов.
- **AReset**: сброс внутренних состояний.
- **ACalculate**: вычисление signum/масок и отправка в разные выходы.

### I/O
- Вход: скаляр/вектор.
- Выход: раздельные каналы (pos/neg/zero) или маски.

```mermaid
classDiagram
    UComponent <|-- NSignumSeparator
```

Диаграмма показывает компонент-разделитель, который в `ACalculate()` раскладывает сигнал по каналам.

```mermaid
sequenceDiagram
    participant Src as Source
    participant Sep as NSignumSeparator
    participant P as PosOut
    participant N as NegOut
    Src-->>Sep: value
    Sep->>Sep: ACalculate()
    Sep-->>P: positive part
    Sep-->>N: negative part
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    v[Value] --> s[NSignumSeparator]
    s --> p[Positive]
    s --> n[Negative]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NSignumSeparator
Name = SignSep1
```

---

## NSignumSeparator — sign-based splitter (EN)

Splits a scalar/vector signal into positive/negative channels.

```mermaid
classDiagram
    UComponent <|-- NSignumSeparator
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Src as Source
    participant Sep as NSignumSeparator
    Src-->>Sep: value
    Sep-->>Src: pos/neg outputs
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    v[Value] --> s[NSignumSeparator]
    s --> p[Positive]
    s --> n[Negative]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
