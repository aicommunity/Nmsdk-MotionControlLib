## NEngineMotionControl — движок управления движением

**Класс**: `NEngineMotionControl` — объединяет сенсоры, контроллеры и актуаторы для движения.  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NEngineMotionControl", ...)`.

```mermaid
classDiagram
    UComponent <|-- NEngineMotionControl
    class NEngineMotionControl {
        +configure()
        +step()
    }
```

### Входы/выходы
- Вход: сенсоры (ретина, гироскоп, приемники), цели/команды.
- Выход: управляющие сигналы к двигателям/манипуляторам.

```mermaid
flowchart LR
    sensors[Sensors] --> eng[NEngineMotionControl]
    goals[Targets] --> eng
    eng --> actuators[Actuators/Engines]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant S as Sensors
    participant Eng as NEngineMotionControl
    participant A as Actuators
    S-->>Eng: observations
    Eng->>Eng: compute control
    Eng-->>A: control signals
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## NEngineMotionControl — motion control engine

Central engine combining perception and actuation to achieve motion targets.
