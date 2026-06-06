# Инвентаризация компонентов Nmsdk-MotionControlLib

## RU

## Общая информация

**Дата создания:** 2025-01-27  
**Всего компонентов:** 40  
**Источник регистрации:** `Core/NMotionControlLibrary.cpp` и `Core/WinAPI/NWinAPIActLibrary.cpp`

## Базовые классы

### Иерархия наследования

```
ULibrary (Rdk)
  ├── NMotionControlLibrary
  └── NWinAPIActLibrary

UNet (Rdk)
  ├── [Большинство компонентов]
  └── [Специализированные компоненты]

NSource (Nmsdk-PulseLib)
  ├── NManipulatorInput
  ├── NManipulatorInputEmulator
  ├── NManipulatorSource
  ├── NManipulatorSourceEmulator
  └── NControlObjectSource

NReceiver (Nmsdk-PulseLib)
  ├── NFrequencyReceiver
  └── NPulseReceiver

NPositionControlElement (MotionControlLib)
  ├── NPCNElement
  ├── NNewPositionControlElement
  └── NMultiPositionControl

NPulseNeuronCommon (Nmsdk-PulseLib)
  └── NCounterNeuron
```

## Полный список компонентов по категориям

### 1. Engines / Actuators (Двигатели/Актуаторы)

| Компонент | Базовый класс | Файл | Описание |
|-----------|--------------|------|----------|
| **NDCEngine** | `UNet` | `Core/NDCEngine.h` | DC-двигатель, управление скоростью и направлением |
| **NActuatorSignals** | `UNet` | `Core/NActuatorSignals.h` | Сигналы актуаторов, управление сигналами для приводов |

### 2. Manipulators (Манипуляторы)

| Компонент | Базовый класс | Файл | Описание |
|-----------|--------------|------|----------|
| **NManipulator** | `UNet` | `Core/NManipulator.h` | Роботизированный манипулятор |
| **NManipulatorAndGyro** | `UNet` | `Core/NManipulatorAndGyro.h` | Манипулятор с гироскопом |
| **NManipulatorInput** | `NSource` | `Core/NManipulatorInput.h` | Входные данные манипулятора |
| **NManipulatorInputEmulator** | `NManipulatorInput` | `Core/NManipulatorInputEmulator.h` | Эмулятор входных данных манипулятора |
| **NManipulatorSource** | `NSource` | `Core/NManipulatorSource.h` | Источник данных манипулятора |
| **NManipulatorSourceEmulator** | `NManipulatorSource` | `Core/NManipulatorSourceEmulator.h` | Эмулятор источника данных манипулятора |
| **NWPhysicalManipulator** | `UNet` | `Core/WinAPI/NWPhysicalManipulator.h` | Физический манипулятор через WinAPI |

### 3. Sensors / Perception (Датчики/Восприятие)

| Компонент | Базовый класс | Файл | Описание |
|-----------|--------------|------|----------|
| **NEyeRetina** | `UNet` | `Core/NEyeRetina.h` | Ретина глаза, обработка изображений |
| **NAstaticGyro** | `UNet` | `Core/NAstaticGyro.h` | Астатический гироскоп |
| **NFrequencyReceiver** | `NReceiver` | `Core/NFrequencyReceiver.h` | Приемник частотных сигналов |
| **NPulseReceiver** | `NReceiver` | `Core/NPulseReceiver.h` | Приемник импульсных сигналов |
| **NSimpleStatistic** | `UNet` | `Core/NSimpleStatistic.h` | Простая статистика |
| **NNetworkLinksStatistic** | `UNet` | `Core/NNetworkLinksStatistic.h` | Статистика связей сети (закомментирован) |

### 4. Control / Logic (Управление/Логика)

| Компонент | Базовый класс | Файл | Описание |
|-----------|--------------|------|----------|
| **NEngineMotionControl** | `UNet` | `Core/NEngineMotionControl.h` | Центральный движок управления движением |
| **NEngineControlSignumAfferent** | `NEngineMotionControl` | (вариант конфигурации) | Движок управления с сигнум-афферентами |
| **NEngineControlRangeAfferent** | `NEngineMotionControl` | (вариант конфигурации) | Движок управления с диапазонными афферентами |
| **N2AsfNewSimplestAfferentBranchedEngineControl** | `NEngineMotionControl` | (вариант конфигурации) | Упрощенная ветвящаяся система управления |
| **NSignumSeparator** | `UNet` | `Core/NSignumSeparator.h` | Разделитель сигнум-сигналов |
| **NPosSignumSeparator** | `NSignumSeparator` | (вариант конфигурации) | Разделитель положительных сигнум-сигналов |
| **NNegSignumSeparator** | `NSignumSeparator` | (вариант конфигурации) | Разделитель отрицательных сигнум-сигналов |
| **NIntervalSeparator** | `UNet` | `Core/NIntervalSeparator.h` | Разделитель интервалов |
| **NSuppressionUnit** | `UNet` | `Core/NSuppressionUnit.h` | Блок подавления сигналов |
| **NCounterNeuron** | `NPulseNeuronCommon` | `Core/NCounterNeuron.h` | Счетчик нейронов |
| **NSeqComparison** | `UNet` | `Core/NSeqComparison.h` | Сравнение последовательностей |
| **NSignalEstimation** | `UNet` | `Core/NSignalEstimation.h` | Оценка сигналов |
| **NObjInArea** | `UNet` | `Core/NObjInArea.h` | Обнаружение объектов в области |

### 5. Navigation / Trajectories / Memory (Навигация/Траектории/Память)

| Компонент | Базовый класс | Файл | Описание |
|-----------|--------------|------|----------|
| **NNavMousePrimitive** | `UNet` | `Core/NNavMousePrimitive.h` | Примитив навигации мыши |
| **NPositionControlElement** | `UNet` | `Core/NPositionControlElement.h` | Элемент контроля позиции |
| **NNewPositionControlElement** | `NPositionControlElement` | `Core/NNewPositionControlElement.h` | Новый элемент контроля позиции |
| **NMultiPositionControl** | `NPositionControlElement` | `Core/NMultiPositionControl.h` | Множественный контроль позиции |
| **NTrajectoryElement** | `UNet` | `Core/NTrajectoryElement.h` | Элемент траектории |
| **NMazeMemory** | `UNet` | `Core/NMazeMemory.h` | Память лабиринта |
| **NMazeMemorySimplified** | `UNet` | `Core/NMazeMemorySimplified.h` | Упрощенная память лабиринта |

### 6. Motion Elements (Элементы движения)

| Компонент | Базовый класс | Файл | Описание |
|-----------|--------------|------|----------|
| **NNewMotionElement** | `NMotionElement` | `Core/NMotionElement.h` | Элемент движения (регистрируется как NNewMotionElement) |
| **NPCN** | `NPCNElement` | `Core/NPCNElement.h` | PCN элемент (Position Control Network) |
| **NPendulumAndCart** | `UNet` | `Core/NPendulumAndCart.h` | Маятник и тележка |
| **NControlObjectSource** | `NSource` | `Core/NControlObjectSource.h` | Источник объекта управления |

## Зависимости между компонентами

### Иерархия наследования внутри библиотеки

```
NPositionControlElement
  ├── NPCNElement (регистрируется как NPCN)
  ├── NNewPositionControlElement
  └── NMultiPositionControl

NSignumSeparator
  ├── NPosSignumSeparator (вариант конфигурации)
  └── NNegSignumSeparator (вариант конфигурации)

NManipulatorInput
  └── NManipulatorInputEmulator

NManipulatorSource
  └── NManipulatorSourceEmulator

NEngineMotionControl
  ├── NEngineControlSignumAfferent (вариант конфигурации)
  ├── NEngineControlRangeAfferent (вариант конфигурации)
  └── N2AsfNewSimplestAfferentBranchedEngineControl (вариант конфигурации)
```

### Зависимости от внешних библиотек

#### Nmsdk-PulseLib
- `NSource` - базовый класс для источников
- `NReceiver` - базовый класс для приемников
- `NPulseNeuronCommon` - базовый класс для импульсных нейронов
- `NReceptor` - используется в NEngineMotionControl
- `NNet` - используется в NEngineMotionControl
- `NPulseGenerator` - используется в NEngineMotionControl

#### Rdk-BasicLib
- Базовые компоненты и утилиты

#### Rdk-CvBasicLib
- Компоненты компьютерного зрения (используются в NEyeRetina)

#### Rdk-HardwareLib
- Компоненты для работы с аппаратным обеспечением

## Статистика по базовым классам

| Базовый класс | Количество компонентов | Компоненты |
|---------------|------------------------|------------|
| `UNet` | 30 | Большинство компонентов |
| `NSource` | 5 | NManipulatorInput, NManipulatorInputEmulator, NManipulatorSource, NManipulatorSourceEmulator, NControlObjectSource |
| `NReceiver` | 2 | NFrequencyReceiver, NPulseReceiver |
| `NPositionControlElement` | 3 | NPCNElement, NNewPositionControlElement, NMultiPositionControl |
| `NPulseNeuronCommon` | 1 | NCounterNeuron |
| `ULibrary` | 2 | NMotionControlLibrary, NWinAPIActLibrary |

## Особые случаи

### Варианты конфигурации (не отдельные классы)

Следующие компоненты создаются как варианты конфигурации существующих классов:

1. **NPosSignumSeparator** - создается из NSignumSeparator с `Sign = [1.0]`
2. **NNegSignumSeparator** - создается из NSignumSeparator с `Sign = [-1.0]`
3. **NEngineControlSignumAfferent** - создается из NEngineMotionControl с `CreationMode=0`
4. **NEngineControlRangeAfferent** - создается из NEngineMotionControl с `CreationMode=0`
5. **N2AsfNewSimplestAfferentBranchedEngineControl** - создается из NEngineMotionControl с `CreationMode=14`

### Закомментированные компоненты

- **NNetworkLinksStatistic** - закомментирован в `NMotionControlLibrary.cpp` (строки 106-111)

## Приоритеты для документации

### Высокий приоритет
1. NEngineMotionControl - центральный компонент
2. NDCEngine - базовый двигатель
3. NManipulator - манипулятор
4. NEyeRetina - ретина глаза
5. NMotionElement (NNewMotionElement) - элемент движения

### Средний приоритет
- Компоненты управления позицией
- Датчики и источники
- Обработчики и сепараторы
- Навигация и память

### Низкий приоритет
- Специализированные компоненты
- Варианты конфигурации
- WinAPI компоненты

## Примечания

1. Все компоненты регистрируются через `UploadClass()` в методе `CreateClassSamples()`
2. Большинство компонентов наследуются от `UNet` (базовый класс Rdk Framework)
3. Некоторые компоненты используют базовые классы из `Nmsdk-PulseLib`
4. Варианты конфигурации не требуют отдельных файлов документации, но должны быть описаны в документации базового класса
5. Компоненты с наследованием внутри библиотеки должны иметь ссылки на базовые классы в документации

---

## EN

## General information

**Created:** 2025-01-27  
**Total components:** 40  
**Registration source:** `Core/NMotionControlLibrary.cpp` and `Core/WinAPI/NWinAPIActLibrary.cpp`

## Base classes

### Inheritance hierarchy

```
ULibrary (Rdk)
  ├── NMotionControlLibrary
  └── NWinAPIActLibrary

UNet (Rdk)
  ├── [Most components]
  └── [Specialized components]

NSource (Nmsdk-PulseLib)
  ├── NManipulatorInput
  ├── NManipulatorInputEmulator
  ├── NManipulatorSource
  ├── NManipulatorSourceEmulator
  └── NControlObjectSource

NReceiver (Nmsdk-PulseLib)
  ├── NFrequencyReceiver
  └── NPulseReceiver

NPositionControlElement (MotionControlLib)
  ├── NPCNElement
  ├── NNewPositionControlElement
  └── NMultiPositionControl

NPulseNeuronCommon (Nmsdk-PulseLib)
  └── NCounterNeuron
```

## Full component list by category

### 1. Engines / Actuators

| Component | Base class | File | Description |
|-----------|--------------|------|----------|
| **NDCEngine** | `UNet` | `Core/NDCEngine.h` | DC motor, speed and direction control |
| **NActuatorSignals** | `UNet` | `Core/NActuatorSignals.h` | Actuator signals, drive signal management |

### 2. Manipulators

| Component | Base class | File | Description |
|-----------|--------------|------|----------|
| **NManipulator** | `UNet` | `Core/NManipulator.h` | Robotic manipulator |
| **NManipulatorAndGyro** | `UNet` | `Core/NManipulatorAndGyro.h` | Manipulator with gyroscope |
| **NManipulatorInput** | `NSource` | `Core/NManipulatorInput.h` | Manipulator input data |
| **NManipulatorInputEmulator** | `NManipulatorInput` | `Core/NManipulatorInputEmulator.h` | Manipulator input data emulator |
| **NManipulatorSource** | `NSource` | `Core/NManipulatorSource.h` | Manipulator data source |
| **NManipulatorSourceEmulator** | `NManipulatorSource` | `Core/NManipulatorSourceEmulator.h` | Manipulator data source emulator |
| **NWPhysicalManipulator** | `UNet` | `Core/WinAPI/NWPhysicalManipulator.h` | Physical manipulator via WinAPI |

### 3. Sensors / Perception

| Component | Base class | File | Description |
|-----------|--------------|------|----------|
| **NEyeRetina** | `UNet` | `Core/NEyeRetina.h` | Eye retina, image processing |
| **NAstaticGyro** | `UNet` | `Core/NAstaticGyro.h` | Astatic gyroscope |
| **NFrequencyReceiver** | `NReceiver` | `Core/NFrequencyReceiver.h` | Frequency signal receiver |
| **NPulseReceiver** | `NReceiver` | `Core/NPulseReceiver.h` | Pulse signal receiver |
| **NSimpleStatistic** | `UNet` | `Core/NSimpleStatistic.h` | Simple statistics |
| **NNetworkLinksStatistic** | `UNet` | `Core/NNetworkLinksStatistic.h` | Network link statistics (commented out) |

### 4. Control / Logic

| Component | Base class | File | Description |
|-----------|--------------|------|----------|
| **NEngineMotionControl** | `UNet` | `Core/NEngineMotionControl.h` | Central motion control engine |
| **NEngineControlSignumAfferent** | `NEngineMotionControl` | (configuration variant) | Control engine with signum afferents |
| **NEngineControlRangeAfferent** | `NEngineMotionControl` | (configuration variant) | Control engine with range afferents |
| **N2AsfNewSimplestAfferentBranchedEngineControl** | `NEngineMotionControl` | (configuration variant) | Simplified branched control system |
| **NSignumSeparator** | `UNet` | `Core/NSignumSeparator.h` | Signum signal separator |
| **NPosSignumSeparator** | `NSignumSeparator` | (configuration variant) | Positive signum signal separator |
| **NNegSignumSeparator** | `NSignumSeparator` | (configuration variant) | Negative signum signal separator |
| **NIntervalSeparator** | `UNet` | `Core/NIntervalSeparator.h` | Interval separator |
| **NSuppressionUnit** | `UNet` | `Core/NSuppressionUnit.h` | Signal suppression unit |
| **NCounterNeuron** | `NPulseNeuronCommon` | `Core/NCounterNeuron.h` | Neuron counter |
| **NSeqComparison** | `UNet` | `Core/NSeqComparison.h` | Sequence comparison |
| **NSignalEstimation** | `UNet` | `Core/NSignalEstimation.h` | Signal estimation |
| **NObjInArea** | `UNet` | `Core/NObjInArea.h` | Object detection in area |

### 5. Navigation / Trajectories / Memory

| Component | Base class | File | Description |
|-----------|--------------|------|----------|
| **NNavMousePrimitive** | `UNet` | `Core/NNavMousePrimitive.h` | Mouse navigation primitive |
| **NPositionControlElement** | `UNet` | `Core/NPositionControlElement.h` | Position control element |
| **NNewPositionControlElement** | `NPositionControlElement` | `Core/NNewPositionControlElement.h` | New position control element |
| **NMultiPositionControl** | `NPositionControlElement` | `Core/NMultiPositionControl.h` | Multi-position control |
| **NTrajectoryElement** | `UNet` | `Core/NTrajectoryElement.h` | Trajectory element |
| **NMazeMemory** | `UNet` | `Core/NMazeMemory.h` | Maze memory |
| **NMazeMemorySimplified** | `UNet` | `Core/NMazeMemorySimplified.h` | Simplified maze memory |

### 6. Motion Elements

| Component | Base class | File | Description |
|-----------|--------------|------|----------|
| **NNewMotionElement** | `NMotionElement` | `Core/NMotionElement.h` | Motion element (registered as NNewMotionElement) |
| **NPCN** | `NPCNElement` | `Core/NPCNElement.h` | PCN element (Position Control Network) |
| **NPendulumAndCart** | `UNet` | `Core/NPendulumAndCart.h` | Pendulum and cart |
| **NControlObjectSource** | `NSource` | `Core/NControlObjectSource.h` | Control object source |

## Dependencies between components

### Inheritance hierarchy within the library

```
NPositionControlElement
  ├── NPCNElement (registered as NPCN)
  ├── NNewPositionControlElement
  └── NMultiPositionControl

NSignumSeparator
  ├── NPosSignumSeparator (configuration variant)
  └── NNegSignumSeparator (configuration variant)

NManipulatorInput
  └── NManipulatorInputEmulator

NManipulatorSource
  └── NManipulatorSourceEmulator

NEngineMotionControl
  ├── NEngineControlSignumAfferent (configuration variant)
  ├── NEngineControlRangeAfferent (configuration variant)
  └── N2AsfNewSimplestAfferentBranchedEngineControl (configuration variant)
```

### Dependencies on external libraries

#### Nmsdk-PulseLib
- `NSource` — base class for sources
- `NReceiver` — base class for receivers
- `NPulseNeuronCommon` — base class for pulse neurons
- `NReceptor` — used in NEngineMotionControl
- `NNet` — used in NEngineMotionControl
- `NPulseGenerator` — used in NEngineMotionControl

#### Rdk-BasicLib
- Base components and utilities

#### Rdk-CvBasicLib
- Computer vision components (used in NEyeRetina)

#### Rdk-HardwareLib
- Hardware interface components

## Statistics by base class

| Base class | Component count | Components |
|---------------|------------------------|------------|
| `UNet` | 30 | Most components |
| `NSource` | 5 | NManipulatorInput, NManipulatorInputEmulator, NManipulatorSource, NManipulatorSourceEmulator, NControlObjectSource |
| `NReceiver` | 2 | NFrequencyReceiver, NPulseReceiver |
| `NPositionControlElement` | 3 | NPCNElement, NNewPositionControlElement, NMultiPositionControl |
| `NPulseNeuronCommon` | 1 | NCounterNeuron |
| `ULibrary` | 2 | NMotionControlLibrary, NWinAPIActLibrary |

## Special cases

### Configuration variants (not separate classes)

The following components are created as configuration variants of existing classes:

1. **NPosSignumSeparator** — created from NSignumSeparator with `Sign = [1.0]`
2. **NNegSignumSeparator** — created from NSignumSeparator with `Sign = [-1.0]`
3. **NEngineControlSignumAfferent** — created from NEngineMotionControl with `CreationMode=0`
4. **NEngineControlRangeAfferent** — created from NEngineMotionControl with `CreationMode=0`
5. **N2AsfNewSimplestAfferentBranchedEngineControl** — created from NEngineMotionControl with `CreationMode=14`

### Commented-out components

- **NNetworkLinksStatistic** — commented out in `NMotionControlLibrary.cpp` (lines 106-111)

## Documentation priorities

### High priority
1. NEngineMotionControl — central component
2. NDCEngine — base motor
3. NManipulator — manipulator
4. NEyeRetina — eye retina
5. NMotionElement (NNewMotionElement) — motion element

### Medium priority
- Position control components
- Sensors and sources
- Processors and separators
- Navigation and memory

### Low priority
- Specialized components
- Configuration variants
- WinAPI components

## Notes

1. All components are registered via `UploadClass()` in the `CreateClassSamples()` method
2. Most components inherit from `UNet` (Rdk Framework base class)
3. Some components use base classes from `Nmsdk-PulseLib`
4. Configuration variants do not require separate documentation files, but should be described in the base class documentation
5. Components with inheritance within the library should have links to base classes in their documentation
