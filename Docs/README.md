# Nmsdk-MotionControlLib - Документация

## RU

### Назначение

**Nmsdk-MotionControlLib** предоставляет компоненты для управления движением, робототехники и систем управления. Библиотека интегрирует импульсные нейронные сети, компьютерное зрение и аппаратное обеспечение.

### Краткий обзор

Библиотека включает компоненты для:
- Управления двигателями и приводами
- Управления манипуляторами
- Контроля позиции и траекторий
- Работы с ретиной глаза
- Интеграции с гироскопами

### Быстрый старт

#### Управление манипулятором

```cpp
// Создание компонента манипулятора
auto manipulator = storage->CreateComponent<NManipulator>("Manipulator");
manipulator->Default();
manipulator->Build();

// Управление позицией
manipulator->SetPosition(x, y, z);
manipulator->Calculate();
```

#### Работа с ретиной глаза

```cpp
// Создание компонента ретины
auto retina = storage->CreateComponent<NEyeRetina>("Retina");
retina->InputImage.AttachTo(&camera->OutputImage);
retina->Build();
retina->Calculate();
```

### Связь с корневой документацией

Для обзорной информации см. корневую документацию проекта:
- `Docs/Libraries/Nmsdk-MotionControlLib.md` - обзор библиотеки (в корневом репозитории)

### Детальная документация

- [Architecture.md](Architecture.md) - архитектура библиотеки
- [Usage-Examples.md](Usage-Examples.md) - примеры использования
- [API-Overview.md](API-Overview.md) - обзор API
- [Component-Catalog.md](Component-Catalog.md) - каталог компонентов

---

## EN

### Purpose

**Nmsdk-MotionControlLib** provides components for motion control, robotics, and control systems. The library integrates spiking neural networks, computer vision, and hardware.

### Brief Overview

The library includes components for:
- Motor and actuator control
- Manipulator control
- Position and trajectory control
- Eye retina operations
- Gyroscope integration

### Link to Root Documentation

For overview information see root project documentation:
- `Docs/Libraries/Nmsdk-MotionControlLib.md` - library overview (in root repository)

### Detailed Documentation

- [Architecture.md](Architecture.md) - library architecture
- [Usage-Examples.md](Usage-Examples.md) - usage examples
- [API-Overview.md](API-Overview.md) - API overview
- [Component-Catalog.md](Component-Catalog.md) - component catalog
