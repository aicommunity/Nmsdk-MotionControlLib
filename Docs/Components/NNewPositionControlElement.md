## NNewPositionControlElement — новый элемент контроля позиции

**Класс**: `NNewPositionControlElement` — обновлённая версия позиционного элемента управления.  
**Регистрация**: `UploadClass("NNewPositionControlElement", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault/ABuild/AReset**: настройка регулятора.
- **ACalculate**: вычисление команды по ошибке позиции.

### I/O
- Вход: позиция/цель/ошибка.
- Выход: управляющий сигнал.

```mermaid
classDiagram
    NPositionControlElement <|-- NNewPositionControlElement
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant FB as Feedback
    participant C as NNewPositionControlElement
    FB-->>C: position error
    C->>C: ACalculate()
    C-->>FB: control output
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    err[Error] --> c[NNewPositionControlElement]
    c --> u[Control output]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NNewPositionControlElement
Name = NewPosCtrl1
```

---

## NNewPositionControlElement — new position control element (EN)

Updated position control element producing actuation commands from position error.

```mermaid
classDiagram
    NPositionControlElement <|-- NNewPositionControlElement
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant FB as Feedback
    participant C as NNewPositionControlElement
    FB-->>C: position error
    C-->>FB: control output
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    err[Error] --> c[NNewPositionControlElement]
    c --> u[Control output]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
