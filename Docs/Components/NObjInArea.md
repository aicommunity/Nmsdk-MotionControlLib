## NObjInArea — объект в области

**Класс**: `NObjInArea` — проверка/детекция объекта в заданной области (сигнал события).  
**Регистрация**: `UploadClass("NObjInArea", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault**: параметры области.
- **ABuild**: подключение источников объектов/координат.
- **AReset**: сброс события.
- **ACalculate**: проверка условия «в области» и выдача результата.

### I/O
- Вход: координаты/объекты.
- Выход: флаг/событие.

```mermaid
classDiagram
    UComponent <|-- NObjInArea
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant Src as Source
    participant D as NObjInArea
    Src-->>D: object coords
    D->>D: ACalculate()
    D-->>Src: inArea flag
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    obj[Object coords] --> d[NObjInArea]
    d --> flag[InArea flag]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NObjInArea
Name = ObjArea1
```

---

## NObjInArea — object-in-area check (EN)

Checks whether an object lies within a configured area; outputs boolean/event.

```mermaid
classDiagram
    UComponent <|-- NObjInArea
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Src as Source
    participant D as NObjInArea
    Src-->>D: object coords
    D-->>Src: inArea flag
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    obj[Object coords] --> d[NObjInArea]
    d --> flag[InArea flag]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
