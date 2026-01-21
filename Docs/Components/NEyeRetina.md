## NEyeRetina — ретина

**Класс**: `NEyeRetina` — модель ретины, преобразующая входной сигнал в карту признаков.  
**Регистрация**: `NMotionControlLibrary.cpp` → `UploadClass("NEyeRetina", ...)`.

```mermaid
classDiagram
    UComponent <|-- NEyeRetina
    class NEyeRetina {
        +resolution : pair
        +channels : int
    }
```

### Входы/выходы
- Вход: визуальный поток/bitmap.
- Выход: признаковая карта для последующих блоков.

```mermaid
flowchart LR
    cam[Camera input] --> retina[NEyeRetina]
    retina --> feats[Feature map]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant Cam as Camera
    participant Ret as NEyeRetina
    participant Ctrl as Controller
    Cam-->>Ret: frame
    Ret-->>Ctrl: features
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## NEyeRetina — retina

Transforms visual input into feature map for downstream motion/control modules.
