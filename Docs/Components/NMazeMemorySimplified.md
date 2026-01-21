## NMazeMemorySimplified — упрощённая память лабиринта

**Класс**: `NMazeMemorySimplified` — облегчённая версия `NMazeMemory`.  
**Регистрация**: `UploadClass("NMazeMemorySimplified", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault/ABuild/AReset**: инициализация карты и сброс.
- **ACalculate**: обновление памяти/карты по наблюдениям.

### I/O
- Вход: события/позиция/столкновения.
- Выход: обновлённая карта/решение.

```mermaid
classDiagram
    NMazeMemory <|-- NMazeMemorySimplified
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant Nav as Navigator
    participant M as NMazeMemorySimplified
    Nav-->>M: observations
    M->>M: ACalculate()
    M-->>Nav: map/path
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    obs[Observations] --> mem[NMazeMemorySimplified]
    mem --> map[Map/Path]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NMazeMemorySimplified
Name = MazeMemS1
```

---

## NMazeMemorySimplified — simplified maze memory (EN)

Lightweight maze memory/map updater.

```mermaid
classDiagram
    NMazeMemory <|-- NMazeMemorySimplified
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Nav as Navigator
    participant M as NMazeMemorySimplified
    Nav-->>M: observations
    M-->>Nav: map/path
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    obs[Observations] --> mem[NMazeMemorySimplified]
    mem --> map[Map/Path]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
