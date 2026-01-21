## N2AsfNewSimplestAfferentBranchedEngineControl — афферентный разветвлённый контроллер

**Класс**: `N2AsfNewSimplestAfferentBranchedEngineControl` — специализированный контроллер движения на основе разветвлённых афферентов (по регистрации библиотеки).  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("N2AsfNewSimplestAfferentBranchedEngineControl", ...)`.

### Lifecycle
- **ADefault**: параметры ветвления/правил.
- **ABuild**: подключение сенсоров и актуаторов.
- **AReset**: сброс состояния ветвей.
- **ACalculate**: вычисление команды на основе афферентных ветвей.

### I/O
- Вход: сенсорные признаки/ошибки (векторы).
- Выход: команды управления (векторы/скаляры).

```mermaid
classDiagram
    UComponent <|-- N2AsfNewSimplestAfferentBranchedEngineControl
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant S as Sensors
    participant C as N2AsfNewSimplestAfferentBranchedEngineControl
    participant A as Actuators
    S-->>C: features
    C->>C: branch + ACalculate()
    C-->>A: commands
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    s[Sensors] --> c[N2AsfNewSimplestAfferentBranchedEngineControl]
    c --> a[Actuators]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = N2AsfNewSimplestAfferentBranchedEngineControl
Name = BranchedCtrl1
```

---

## N2AsfNewSimplestAfferentBranchedEngineControl — branched afferent control (EN)

Specialised branched afferent controller producing actuator commands from sensor features.

```mermaid
classDiagram
    UComponent <|-- N2AsfNewSimplestAfferentBranchedEngineControl
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant S as Sensors
    participant C as N2AsfNewSimplestAfferentBranchedEngineControl
    participant A as Actuators
    S-->>C: features
    C-->>A: commands
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    s[Sensors] --> c[N2AsfNewSimplestAfferentBranchedEngineControl]
    c --> a[Actuators]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
