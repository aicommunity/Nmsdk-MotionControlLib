## NMazeMemory / NMazeMemorySimplified — память/навигация в лабиринте

**Классы**: `NMazeMemory`, `NMazeMemorySimplified` — хранение/обновление карты/памяти для навигации.  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NMazeMemory", ...)`, `"NMazeMemorySimplified"`.

```mermaid
classDiagram
    UComponent <|-- NMazeMemory
    NMazeMemory <|-- NMazeMemorySimplified
```

### Входы/выходы
- Вход: сенсорные события/позиция, столкновения.
- Выход: обновлённая карта/решение маршрута.

```mermaid
flowchart LR
    sensors[Sensors/Collisions] --> maze[NMazeMemory]
    maze --> path[Route/Map]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant Nav as Navigator
    participant Maze as NMazeMemory
    Nav-->>Maze: observations
    Maze-->>Nav: updated map/path
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## NMazeMemory — maze memory

Maintains map for navigation; simplified version offers lightweight variant.
