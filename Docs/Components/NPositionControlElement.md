## NPositionControlElement — контроль позиции

**Класс**: `NPositionControlElement` (и `NNewPositionControlElement`, `NMultiPositionControl`) — элементы позиционного управления.  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NPositionControlElement", ...)` и др.

```mermaid
classDiagram
    UComponent <|-- NPositionControlElement
    NPositionControlElement <|-- NNewPositionControlElement
    NPositionControlElement <|-- NMultiPositionControl
```

### Входы/выходы
- Вход: текущая позиция/ошибка.
- Выход: управляющий сигнал для привода/траектории.

```mermaid
flowchart LR
    err[Position error] --> ctrl[NPositionControlElement]
    ctrl --> command[Actuation command]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant C as Controller
    participant P as NPositionControlElement
    C-->>P: position feedback
    P-->>C: control output
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## NPositionControlElement — position control

Computes control output from position error; variants support new or multi-DOF control.
