# Инвентаризация компонентов Nmsdk-MotionControlLib

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
