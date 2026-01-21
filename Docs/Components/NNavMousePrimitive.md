## NNavMousePrimitive — примитив навигации «мышь»

**Класс**: `NNavMousePrimitive` — простая навигационная логика в стиле движения «мыши».  
**Регистрация**: `UploadClass("NNavMousePrimitive", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault/ABuild/AReset**: инициализация параметров навигации.
- **ACalculate**: шаг навигации (выбор действия/направления).

### I/O
- Вход: наблюдения/состояние среды.
- Выход: команда движения.

```mermaid
classDiagram
    UComponent <|-- NNavMousePrimitive
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant Env as Environment
    participant Nav as NNavMousePrimitive
    Env-->>Nav: observation
    Nav->>Nav: ACalculate()
    Nav-->>Env: move command
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    obs[Observation] --> nav[NNavMousePrimitive]
    nav --> cmd[Move command]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NNavMousePrimitive
Name = Nav1
```

---

## NNavMousePrimitive — mouse-like navigation primitive (EN)

Simple navigation logic producing movement commands from observations.

```mermaid
classDiagram
    UComponent <|-- NNavMousePrimitive
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Env as Environment
    participant Nav as NNavMousePrimitive
    Env-->>Nav: observation
    Nav-->>Env: move command
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    obs[Observation] --> nav[NNavMousePrimitive]
    nav --> cmd[Move command]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
