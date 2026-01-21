## NAstaticGyro — гироскоп

**Класс**: `NAstaticGyro` — измеряет угловую скорость/ориентацию.  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NAstaticGyro", ...)`.

```mermaid
classDiagram
    UComponent <|-- NAstaticGyro
    class NAstaticGyro {
        +omega : vector
        +angle : vector
    }
```

### Входы/выходы
- Вход: поток датчика/шум.
- Выход: ориентация/угловая скорость для контроллеров.

```mermaid
flowchart LR
    sensor[Gyro sensor] --> gyro[NAstaticGyro]
    gyro --> ctrl[Control modules]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant G as NAstaticGyro
    participant C as Controller
    loop sensing
        G-->>C: omega/angle
    end
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## NAstaticGyro — gyro

Provides angular velocity/orientation signals to control algorithms.
