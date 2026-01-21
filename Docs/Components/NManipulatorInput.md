## NManipulatorInput — вход команд манипулятора

**Класс**: `NManipulatorInput` — принимает команды управления манипулятором (углы/скорости/режимы).  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NManipulatorInput", ...)`.  
**Storage-инстансы**: `ClassName = "NManipulatorInput"`.

### Lifecycle
- **ADefault**: значения по умолчанию и режим ввода.
- **ABuild**: подключение источников команд.
- **AReset**: сброс команд.
- **ACalculate**: обновление команд на шаге управления.

### I/O
- Вход: команды (скаляры/векторы).
- Выход: нормализованные команды для `NManipulator`.

```mermaid
classDiagram
    UComponent <|-- NManipulatorInput
```

Диаграмма показывает компонент ввода команд.

```mermaid
sequenceDiagram
    participant Src as CommandSource
    participant Inp as NManipulatorInput
    participant Man as NManipulator
    Src-->>Inp: commands
    Inp->>Inp: ACalculate()
    Inp-->>Man: command vector
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    cmd[Commands] --> inp[NManipulatorInput]
    inp --> man[NManipulator]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NManipulatorInput
Name = ManInput1
```

---

## NManipulatorInput — manipulator command input (EN)

Consumes control commands and outputs command vectors for manipulator controllers.

```mermaid
classDiagram
    UComponent <|-- NManipulatorInput
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Src as CommandSource
    participant Inp as NManipulatorInput
    Src-->>Inp: commands
    Inp-->>Src: outputs
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    cmd[Commands] --> inp[NManipulatorInput]
    inp --> man[NManipulator]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
