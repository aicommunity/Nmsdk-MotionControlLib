## NEngineControlSignumAfferent — афферентный контроллер (signum)

**Класс**: `NEngineControlSignumAfferent` — вариант контроллера движения на базе signum-признаков (афферентные сигналы).  
**Регистрация**: `UploadClass("NEngineControlSignumAfferent", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault**: настройки афферентного преобразования (пороги/маски).
- **ABuild**: связывание входов сенсоров и выходов управления.
- **AReset**: сброс внутренних состояний.
- **ACalculate**: вычисление управляющего сигнала на основе signum-афферентов.

### I/O
- Вход: сенсорные признаки/ошибки (скаляры/векторы).
- Выход: управляющие команды на двигатели/манипулятор.

```mermaid
classDiagram
    UComponent <|-- NEngineControlSignumAfferent
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant S as Sensors
    participant C as NEngineControlSignumAfferent
    participant A as Actuators
    S-->>C: afferent signals
    C->>C: ACalculate()
    C-->>A: control commands
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    s[Sensors] --> c[NEngineControlSignumAfferent]
    c --> a[Actuators]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NEngineControlSignumAfferent
Name = CtrlSignum1
```

---

## NEngineControlSignumAfferent — afferent controller (EN)

Computes control actions from signum-like afferent features.

```mermaid
classDiagram
    UComponent <|-- NEngineControlSignumAfferent
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant S as Sensors
    participant C as NEngineControlSignumAfferent
    participant A as Actuators
    S-->>C: features
    C-->>A: commands
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    s[Sensors] --> c[NEngineControlSignumAfferent]
    c --> a[Actuators]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
