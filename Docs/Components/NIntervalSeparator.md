## NIntervalSeparator — разделитель по интервалам (Nmsdk-MotionControlLib)

**Класс**: `NIntervalSeparator` — классифицирует/разделяет входной сигнал по диапазонам (интервалам).  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NIntervalSeparator", ...)`.

### Регистрация в UStorage
- `ClassName = "NIntervalSeparator"` в `Bin/ClDesc` / `Bin/Configs`.

### Lifecycle
- **ADefault**: установка границ интервалов и режима.
- **ABuild**: подготовка выходных каналов.
- **AReset**: очистка статистики (если ведётся).
- **ACalculate**: вычисление принадлежности интервалу и выдача на соответствующий выход.

### I/O
- **Вход**: скаляр/вектор.
- **Выход**: индекс интервала/маска/сигнал на выбранном выходе.

### classDiagram

```mermaid
classDiagram
    UComponent <|-- NIntervalSeparator
```

Диаграмма показывает `NIntervalSeparator` как компонент-раскладчик входа на интервалы.

### sequenceDiagram

```mermaid
sequenceDiagram
    participant Src as Source
    participant Sep as NIntervalSeparator
    Src-->>Sep: value
    Sep->>Sep: ACalculate()
    Sep-->>Src: interval idx/output
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

### flowchart

```mermaid
flowchart LR
    v[Value] --> sep[NIntervalSeparator]
    sep --> out[Interval output]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NIntervalSeparator
Name = IntervalSep1
```

---

## NIntervalSeparator — interval separator (EN)

Splits/classifies input signal into configured intervals and produces interval output.

```mermaid
classDiagram
    UComponent <|-- NIntervalSeparator
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Src as Source
    participant Sep as NIntervalSeparator
    Src-->>Sep: value
    Sep-->>Src: interval output
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    v[Value] --> sep[NIntervalSeparator]
    sep --> out[Interval output]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
