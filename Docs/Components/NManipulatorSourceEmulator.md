## NManipulatorSourceEmulator — эмулятор источника манипулятора

**Класс**: `NManipulatorSourceEmulator` — генерирует синтетические состояния/наблюдения манипулятора.  
**Регистрация**: `UploadClass("NManipulatorSourceEmulator", ...)` в `NMotionControlLibrary.cpp`.

### Lifecycle
- **ADefault**: стартовый сценарий.
- **ABuild**: подготовка генераторов/шумов.
- **AReset**: сброс сценария.
- **ACalculate**: выдача очередного состояния.

### I/O
- Выход: состояние/наблюдения для `NManipulatorSource`/`NManipulator`.

```mermaid
classDiagram
    UComponent <|-- NManipulatorSourceEmulator
```

Пояснение: диаграмма классов показывает место компонента в иерархии и ключевые связи.

```mermaid
sequenceDiagram
    participant Emu as NManipulatorSourceEmulator
    participant Src as NManipulatorSource
    loop step
        Emu->>Emu: ACalculate()
        Emu-->>Src: synthetic state
    end
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

```mermaid
flowchart LR
    emu[NManipulatorSourceEmulator] --> obs[Synthetic state]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NManipulatorSourceEmulator
Name = ManSrcEmu1
```

---

## NManipulatorSourceEmulator — manipulator source emulator (EN)

Produces synthetic manipulator states/observations for testing.

```mermaid
classDiagram
    UComponent <|-- NManipulatorSourceEmulator
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Emu as NManipulatorSourceEmulator
    participant Src as NManipulatorSource
    Emu-->>Src: synthetic state
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    emu[NManipulatorSourceEmulator] --> obs[Synthetic state]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
