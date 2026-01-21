## NMultiPositionControl — многоканальный контроль позиции

**Класс**: `NMultiPositionControl` — позиционный контроллер для нескольких степеней свободы.  
**Регистрация**: `UploadClass("NMultiPositionControl", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault**: параметры регуляторов по каналам.
- **ABuild**: подключение обратной связи/целей.
- **AReset**: сброс интеграторов.
- **ACalculate**: расчёт команд управления по ошибкам.

### I/O
- Вход: векторы ошибок/позиций/целей.
- Выход: вектор команд актуаторов.

```mermaid
classDiagram
    NPositionControlElement <|-- NMultiPositionControl
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant FB as Feedback
    participant C as NMultiPositionControl
    participant Act as Actuators
    FB-->>C: error vector
    C->>C: ACalculate()
    C-->>Act: command vector
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    err[Error vector] --> c[NMultiPositionControl]
    c --> cmd[Commands]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NMultiPositionControl
Name = MPC1
```

---

## NMultiPositionControl — multi-DOF position control (EN)

Computes actuator commands from multi-dimensional position errors.

```mermaid
classDiagram
    NPositionControlElement <|-- NMultiPositionControl
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant FB as Feedback
    participant C as NMultiPositionControl
    participant Act as Actuators
    FB-->>C: error vector
    C-->>Act: command vector
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    err[Error vector] --> c[NMultiPositionControl]
    c --> cmd[Commands]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
