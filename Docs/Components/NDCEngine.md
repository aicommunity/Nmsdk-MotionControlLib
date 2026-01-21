## NDCEngine — DC-двигатель

**Класс**: `NDCEngine` — управление DC-мотором (скорость/направление).  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NDCEngine", ...)`.

```mermaid
classDiagram
    UComponent <|-- NDCEngine
    class NDCEngine {
        +speed : double
        +direction : int
    }
```

### Входы/выходы
- Вход: команда скорости/направления.
- Выход: сигналы на исполнительный блок (мотор/драйвер).

```mermaid
flowchart LR
    ctrl[Controller] --> eng[NDCEngine]
    eng --> motor[Motor output]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant Ctrl as Controller
    participant Eng as NDCEngine
    Ctrl-->>Eng: set speed/dir
    Eng-->>Ctrl: status (optional)
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## NDCEngine — DC motor controller

Controls motor speed/direction; input control commands, output drive signals.
