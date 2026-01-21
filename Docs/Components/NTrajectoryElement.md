## NTrajectoryElement — траектория

**Класс**: `NTrajectoryElement` — формирование/следование траектории.  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NTrajectoryElement", ...)`.

```mermaid
classDiagram
    UComponent <|-- NTrajectoryElement
    class NTrajectoryElement {
        +path : list
        +step()
    }
```

### Входы/выходы
- Вход: текущая позиция, целевой путь.
- Выход: следующая точка/команда движения.

```mermaid
flowchart LR
    path[Path] --> traj[NTrajectoryElement]
    feedback[Current pose] --> traj
    traj --> next[Next target]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant T as NTrajectoryElement
    participant Ctrl as Controller
    Ctrl-->>T: path + pose
    T-->>Ctrl: next waypoint
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## NTrajectoryElement — trajectory element

Generates next waypoints along a path using current pose feedback.
