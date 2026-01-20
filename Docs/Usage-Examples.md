# Примеры использования Nmsdk-MotionControlLib

## RU

### Пример 1: Управление DC-двигателем

```cpp
// Создание компонента DC-двигателя
auto engine = storage->CreateComponent<NDCEngine>();
engine->MaxSpeed = 100.0;
engine->Build();

// Установка скорости
engine->TargetSpeed = 50.0;
engine->Calculate();
```

### Пример 2: Управление манипулятором

```cpp
// Создание манипулятора
auto manipulator = storage->CreateComponent<NManipulator>();
manipulator->NumJoints = 6;
manipulator->Build();

// Установка целевой позиции
manipulator->TargetPosition = targetPos;
manipulator->Calculate();
```

### Пример 3: Контроль позиции

```cpp
// Создание элемента контроля позиции
auto positionControl = storage->CreateComponent<NPositionControlElement>();
positionControl->TargetPosition = targetPos;
positionControl->CurrentPosition = currentPos;
positionControl->Build();

// Вычисление управления
positionControl->Calculate();
auto controlSignal = positionControl->ControlOutput;
```

---

## EN

### Example 1: DC Motor Control

```cpp
// Creating DC motor component
auto engine = storage->CreateComponent<NDCEngine>();
engine->MaxSpeed = 100.0;
engine->Build();

// Setting speed
engine->TargetSpeed = 50.0;
engine->Calculate();
```

### Example 2: Manipulator Control

```cpp
// Creating manipulator
auto manipulator = storage->CreateComponent<NManipulator>();
manipulator->NumJoints = 6;
manipulator->Build();

// Setting target position
manipulator->TargetPosition = targetPos;
manipulator->Calculate();
```

### Example 3: Position Control

```cpp
// Creating position control element
auto positionControl = storage->CreateComponent<NPositionControlElement>();
positionControl->TargetPosition = targetPos;
positionControl->CurrentPosition = currentPos;
positionControl->Build();

// Computing control
positionControl->Calculate();
auto controlSignal = positionControl->ControlOutput;
```
