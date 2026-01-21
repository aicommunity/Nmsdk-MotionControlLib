## NManipulatorInputEmulator — эмулятор ввода манипулятора

**Класс**: `NManipulatorInputEmulator` — генерирует тестовые команды для манипулятора (демо/отладка).  
**Регистрация**: `UploadClass("NManipulatorInputEmulator", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault**: сценарий эмуляции по умолчанию.
- **ABuild**: подготовка генераторов/паттернов.
- **AReset**: сброс времени/состояния.
- **ACalculate**: выдача очередной команды.

### I/O
- Выход: команды для `NManipulatorInput`/`NManipulator`.

```mermaid
classDiagram
    UComponent <|-- NManipulatorInputEmulator
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant Emu as NManipulatorInputEmulator
    participant Inp as NManipulatorInput
    loop step
        Emu->>Emu: ACalculate()
        Emu-->>Inp: commands
    end
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    emu[NManipulatorInputEmulator] --> cmd[Commands]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NManipulatorInputEmulator
Name = ManInpEmu1
```

---

## NManipulatorInputEmulator — manipulator input emulator (EN)

Generates synthetic manipulator commands for testing/debugging.

```mermaid
classDiagram
    UComponent <|-- NManipulatorInputEmulator
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Emu as NManipulatorInputEmulator
    participant Inp as NManipulatorInput
    Emu-->>Inp: commands
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    emu[NManipulatorInputEmulator] --> cmd[Commands]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
