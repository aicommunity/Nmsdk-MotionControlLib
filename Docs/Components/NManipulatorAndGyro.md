## NManipulatorAndGyro — манипулятор с гироскопом

**Класс**: `NManipulatorAndGyro` — вариант управления манипулятором с учётом данных гироскопа.  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NManipulatorAndGyro", ...)`.

### Lifecycle
- **ADefault**: параметры фильтрации/слияния данных.
- **ABuild**: подключение `NManipulator` и `NAstaticGyro`.
- **AReset**: сброс фильтров.
- **ACalculate**: вычисление команд манипулятора с учётом ориентации.

### I/O
- Вход: целевые команды + данные гироскопа.
- Выход: команды на суставы.

```mermaid
classDiagram
    NManipulator <|-- NManipulatorAndGyro
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant Gy as NAstaticGyro
    participant Man as NManipulatorAndGyro
    Gy-->>Man: orientation
    Man-->>Gy: optional feedback
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    cmd[Target] --> man[NManipulatorAndGyro]
    gyro[Gyro] --> man
    man --> out[Joint commands]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NManipulatorAndGyro
Name = ManGyro1
```

---

## NManipulatorAndGyro — manipulator with gyro (EN)

Manipulator control with gyro feedback fusion.

```mermaid
classDiagram
    NManipulator <|-- NManipulatorAndGyro
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Gy as NAstaticGyro
    participant Man as NManipulatorAndGyro
    Gy-->>Man: orientation
    Man-->>Gy: outputs
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    cmd[Target] --> man[NManipulatorAndGyro]
    gyro[Gyro] --> man
    man --> out[Joint commands]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
