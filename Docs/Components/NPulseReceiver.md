## NPulseReceiver — приёмник импульсных сигналов (Nmsdk-MotionControlLib)

**Класс**: `NPulseReceiver` — принимает спайковые/импульсные сигналы (обычно из SNN-блоков) и преобразует в формат для управления/аналитики.  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NPulseReceiver", ...)`.  
**Storage-инстансы**: `ClassName = "NPulseReceiver"`.

### Lifecycle
- **ADefault**: параметры окна/интеграции импульсов.
- **ABuild**: подключение к источнику спайков/импульсов.
- **AReset**: очистка накопленной активности.
- **ACalculate**: агрегация импульсов и выдача выходных признаков.

### I/O
- **Входы**: импульсные события/векторы активности.
- **Выходы**: скаляр/вектор активности (например, частота/интенсивность).

```mermaid
classDiagram
    UComponent <|-- NPulseReceiver
```

Диаграмма фиксирует `NPulseReceiver` как компонент-приёмник.

```mermaid
sequenceDiagram
    participant Net as Network
    participant Rcv as NPulseReceiver
    loop timestep
        Net-->>Rcv: spikes
        Rcv->>Rcv: ACalculate()
        Rcv-->>Net: activity features
    end
```

Диаграмма показывает интеграцию спайков по шагам и выдачу признаков активности.

```mermaid
flowchart LR
    spikes[Spike events] --> r[NPulseReceiver]
    r --> act[Activity features]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

### Config snippet

```ini
[Component]
ClassName = NPulseReceiver
Name = PulseRcv1
```

---

## NPulseReceiver — spike/pulse receiver (EN)

Aggregates spikes/pulses into activity features usable by motion-control modules.

```mermaid
classDiagram
    UComponent <|-- NPulseReceiver
```

Description: this class diagram shows the component position in the type hierarchy and key relations.

```mermaid
sequenceDiagram
    participant Net as Network
    participant Rcv as NPulseReceiver
    Net-->>Rcv: spikes
    Rcv-->>Net: activity features
```

Description: this sequence diagram shows a typical runtime interaction and call order.

```mermaid
flowchart LR
    spikes[Spike events] --> r[NPulseReceiver]
    r --> act[Activity features]
```

Description: this flowchart shows the data/signal flow (inputs → component → outputs).
