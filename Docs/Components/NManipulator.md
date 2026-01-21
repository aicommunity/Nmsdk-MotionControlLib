## NManipulator — манипулятор

**Класс**: `NManipulator` (и `NManipulatorAndGyro`) — управление звеньями/суставами манипулятора.  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NManipulator", ...)`.

```mermaid
classDiagram
    UComponent <|-- NManipulator
    NManipulator <|-- NManipulatorAndGyro
    class NManipulator {
        +joints : array
        +angles : array
    }
```

### Входы/выходы
- Вход: целевые положения/углы, опционально данные гироскопа.
- Выход: команды приводу/позиции.

```mermaid
flowchart LR
    target[Target pose] --> man[NManipulator]
    man --> actuate[Joint commands]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant Cfg as Config
    participant Man as NManipulator
    participant Act as Actuators
    Cfg->>Man: init joints
    loop control
        Cfg-->>Man: target angles
        Man-->>Act: drive signals
    end
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## NManipulator — manipulator controller

Drives joints to target poses; may fuse gyro feedback (NManipulatorAndGyro).
