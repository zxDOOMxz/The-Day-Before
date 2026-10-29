# AI Context Document — The Day Before

> Этот документ для нейросети. Содержит текущий статус проекта, что сделано, что нужно тестировать, и что делать дальше.

**Последнее обновление:** 29 октября 2026  
**Статус:** Phase 0, Week 3 завершена (Инвентарь готов)  
**Репозиторий:** https://github.com/zxDOOMxz/The-Day-Before

---

## 📊 Текущий статус проекта

### ✅ Что реализовано (C++ код)

#### 1. Базовая структура UE 5.8 проекта
- `.uproject` файл с конфигурацией UE 5.8
- Модуль `TheDayBefore` с `Build.cs`
- Директории: `Source/`, `Content/`, `Config/`, `Docs/`

#### 2. FPS система персонажа
**Файлы:**
- `Source/TheDayBefore/Characters/TDBCharacter.h/.cpp`
  - Базовый класс персонажа от первого лица
  - Камера, руки, тактические компоненты

- `Source/TheDayBefore/Camera/TDBFirstPersonCameraComponent.h/.cpp`
  - ADS (прицеливание) с интерполяцией FOV
  - Head bob (покачивание камеры при ходьбе)
  - Настройка DefaultFOV=90, ADSFOV=45

- `Source/TheDayBefore/Tactical/TDBLeanComponent.h/.cpp`
  - Наклон Q/E из-за укрытий
  - MaxLeanAngle=15°, MaxLeanOffset=30

- `Source/TheDayBefore/Tactical/TDBBreathComponent.h/.cpp`
  - Задержка дыхания Shift при ADS
  - MaxBreathStamina=100, BreathDrainRate=10

#### 3. Game Framework
**Файлы:**
- `Source/TheDayBefore/GameFramework/TDBGameMode.h/.cpp`
  - Базовый GameMode
  - Настройки спавна, правила игры

- `Source/TheDayBefore/GameFramework/TDBPlayerController.h/.cpp`
  - Enhanced Input система
  - Input Actions: Move, Look, Jump, Crouch, Sprint, Aim, Lean, Breath, Fire, Reload, Interact
  - Маппинги на WASD, мышь, Shift, Q/E, Space, C, E (подбор)
  - **HandleInteract()** — подбор ближайшего предмета в радиусе 2м

- `Source/TheDayBefore/GameFramework/TDBGameInstance.h/.cpp`
  - Управление глобальными системами
  - Инициализация DamageSystem

#### 4. Система оружия
**Файлы:**
- `Source/TheDayBefore/Weapons/TDBWeaponBase.h/.cpp`
  - Базовый класс оружия
  - Параметры: BaseDamage, FireRate, ReloadTime, MagazineSize, Accuracy, Recoil
  - Логика стрельбы (hitscan), перезарядки, отдачи
  - Сетевая репликация (bIsFiring, bIsReloading, CurrentAmmo)

- `Source/TheDayBefore/Weapons/TDBWeaponComponent.h/.cpp`
  - Компонент для персонажа
  - Управление оружием (Equip, Unequip, Fire, Reload)
  - Привязка оружия к сокету `weapon_r`

- `Source/TheDayBefore/Weapons/TDBPistol.h/.cpp`
  - Пример пистолета
  - Stats: 35 урон, 400 RPM, 15 патронов, Accuracy 0.85, Recoil 1.5/0.3

#### 5. Боевая система
**Файлы:**
- `Source/TheDayBefore/Combat/TDBDamageSystem.h/.cpp`
  - Расчёт урона с учётом брони
  - Множители по частям тела:
    - Head: ×4.0
    - Thorax (Spine): ×2.0
    - Arms: ×0.8-1.0
    - Legs: ×0.7-1.0
  - Система пробития брони (Penetration vs ArmorValue)

#### 6. Система инвентаря (Week 3)
**Файлы:**
- `Source/TheDayBefore/Inventory/TDBItemTypes.h`
  - `FItemDefinition` — статические данные предмета (DataTable)
    - Свойства: Id, Name, Description, Icon, Mesh, Weight, Size, Category, Rarity, StackSize, Value
    - Категории: Weapon, Armor, Ammo, Medical, Food, Resource, Key, Misc
    - Редкость: Common, Uncommon, Rare, Epic, Legendary
  - `FItemInstance` — экземпляр предмета в инвентаре
    - Свойства: ItemId, Quantity
    - Поддержка стеков

- `Source/TheDayBefore/Inventory/TDBInventoryComponent.h/.cpp`
  - Grid-based инвентарь (10x5 слотов)
  - Система веса (макс 100кг)
  - Функции: AddItem(), RemoveItem(), HasItem(), GetItemCount()
  - События: OnInventoryChanged, OnItemAdded, OnItemRemoved
  - Сетевая репликация (Items array)

- `Source/TheDayBefore/Inventory/TDBItemPickup.h/.cpp`
  - Подбор предметов из мира
  - Collision sphere (радиус 2м)
  - Mesh для визуализации
  - Функции: CanBePickedUp(), PickUp()
  - Overlap события для UI подсказок

- `Content/DataTables/DT_Items.csv`
  - Пример таблицы предметов
  - 10 предметов: Pistol_Ammo, Rifle_Ammo, Bandage, Medkit, Water_Bottle, Canned_Food, Scrap_Metal, Electronics, Key_Card, Dogtag
  - Используется как DataTable в UE

### ⚙️ Конфигурация

**Config/DefaultEngine.ini:**
```ini
- Lumen + Nanite включены
- DX12 по умолчанию
- GameInstance = TDBGameInstance
- Camera settings: FOV 90/45, HeadBob, CameraLag
- Lean settings: MaxAngle 15, MaxOffset 30
- Breath settings: MaxStamina 100, DrainRate 10
```

**TheDayBefore.uproject:**
```json
- Engine: 5.8
- Plugins: Niagara, ModelingTools, GeometryScripting
- Target: Win64
```

**Source/TheDayBefore/TheDayBefore.Build.cs:**
```csharp
// Зависимости:
- Core, CoreUObject, Engine, InputCore
- EnhancedInput
- NavigationSystem, AIModule, GameplayTasks
- Niagara (для VFX)
- UMG, Slate (для UI)
- GameplayAbilities, GameplayTags
- NetCore (для сети)
```

---

## 🎯 Что нужно тестировать в UE Editor

### Шаг 1: Открыть проект
```bash
# Открой TheDayBefore.uproject в Unreal Engine 5.8
# UE автоматически скомпилирует C++ код
```

### Шаг 2: Создать Blueprint для оружия

**Путь:** `Content/Weapons/`

1. Create Blueprint → `BP_Pistol`
   - Parent Class: `TDBPistol`
   - Assign skeletal mesh (любой пистолет из Marketplace или Starter Content)
   - Attach mesh to `WeaponMesh` component

2. В `BP_Pistol` → Component Details:
   - `MuzzleFlash` → Assign Niagara System ( muzzle flash effect)
   - `WeaponAudio` → Assign Sound Cue (fire sound)

### Шаг 3: Создать Input Actions

**Путь:** `Content/Input/`

Создай следующие Input Actions:
- `IA_Move` → Value: Axis2D (Vector2D)
- `IA_Look` → Value: Axis2D (Vector2D)
- `IA_Jump` → Value: Digital (bool)
- `IA_Crouch` → Value: Digital (bool)
- `IA_Sprint` → Value: Digital (bool)
- `IA_Aim` → Value: Digital (bool)
- `IA_LeanLeft` → Value: Digital (bool)
- `IA_LeanRight` → Value: Digital (bool)
- `IA_HoldBreath` → Value: Digital (bool)
- `IA_Fire` → Value: Digital (bool)
- `IA_Reload` → Value: Digital (bool)
- `IA_Interact` → Value: Digital (bool) **(NEW for inventory)**

### Шаг 4: Создать Input Mapping Context

**Путь:** `Content/Input/`

Создай `IMC_Default` с маппингами:
```
W            → IA_Move (Y axis)
A            → IA_Move (X axis negative)
S            → IA_Move (Y axis negative)
D            → IA_Move (X axis)
Mouse 2D     → IA_Look
Space        → IA_Jump
C            → IA_Crouch
Left Shift   → IA_Sprint
Right Mouse  → IA_Aim
Q            → IA_LeanLeft
E            → IA_LeanRight
Left Shift   → IA_HoldBreath (while aiming)
Left Mouse   → IA_Fire
R            → IA_Reload
E            → IA_Interact **(NEW for inventory pickup)**
```

### Шаг 5: Настроить Character Blueprint

**Путь:** `Content/Characters/`

1. Create Blueprint → `BP_TDBCharacter`
   - Parent Class: `TDBCharacter`

2. В Components:
   - Add Skeletal Mesh → `ArmsMesh` (first person arms)
   - Assign mesh from Starter Content или Marketplace

3. В Character Mesh (или Arms Mesh):
   - Add socket → `weapon_r` (right hand bone)
   - Position: правая рука, где держит оружие

4. В `WeaponComponent`:
   - `WeaponSocket` = `weapon_r`
   - `AvailableWeapons` → Add `BP_Pistol`

5. В Player Controller:
   - Assign all Input Actions (`IA_Move`, `IA_Look`, etc.)
   - Assign `DefaultMappingContext` = `IMC_Default`

### Шаг 6: Импортировать DataTable предметов (NEW)

**Путь:** `Content/DataTables/`

1. Импортируй CSV файл:
   - File → Import to `/Game/DataTables/`
   - Выбери `DT_Items.csv` из `Content/DataTables/`
   - UE создаст DataTable с именем `DT_Items`

2. Проверь структуру:
   - Row Structure: `FItemDefinition`
   - Должно быть 10 строк (предметов)

### Шаг 7: Создать тестовый уровень

**Путь:** `Content/Maps/Dev/`

1. Create Level → `DevMap`
2. Add to level:
   - `BP_TDBCharacter` as Player Start
   - Floor (large plane)
   - Lighting (Sun + Sky Light)
   - Sky Sphere
   - Несколько объектов для укрытий (коробки, стены)
   - **TDBItemPickup** → размести 3-5 предметов (например, Medkit, Pistol_Ammo x2)

3. В World Settings:
   - GameMode Override → `TDBGameMode`
   - Default Pawn → `BP_TDBCharacter`

### Шаг 8: Тестирование

Запусти PIE (Play In Editor) и проверь:

**Движение:**
- ✅ WASD — движение
- ✅ Mouse — камера
- ✅ Space — прыжок
- ✅ C — присед
- ✅ Left Shift — спринт

**Тактика:**
- ✅ Right Mouse — ADS (прицеливание, FOV меняется)
- ✅ Q — наклон влево
- ✅ E — наклон вправо
- ✅ Left Shift (при ADS) — задержка дыхания (камера стабилизируется)

**Стрельба:**
- ✅ Left Mouse — стрельба (hitscan)
- ✅ R — перезарядка
- ✅ Отдача работает (камера дёргается вверх)
- ✅ Патроны уменьшаются
- ✅ Автоматическая перезарядка при пустом магазине

**Проверь в логах:**
```
LogTemp: TheDayBefore Module Started
LogTemp: Damage System Initialized
LogTemp: TheDayBefore GameInstance Initialized
LogTemp: Pistol initialized
LogTemp: Added item: [ItemId] x[Quantity] **(NEW)**
LogTemp: Picked up item: [ItemId] x[Quantity] **(NEW)**
```

**Подбор предметов (NEW):**
- ✅ Подойди к TDBItemPickup (в радиусе 2м)
- ✅ Нажми E — предмет добавляется в инвентарь
- ✅ Проверь в логах сообщение "Picked up item"
- ✅ Предмет исчезает из мира

---

## 🐛 Известные проблемы / TODO

### Проблемы, которые могут возникнуть:

1. **Нет mesh для оружия**
   - Решение: Используй любой mesh из Starter Content или Marketplace
   - Или создай простой box в Modeling Mode

2. **Нет звуков**
   - Решение: Добавь Sound Cue в `WeaponAudio` component
   - Или используй placeholder звуки из Starter Content

3. **Нет Niagara эффектов**
   - Решение: Создай простой muzzle flash эффект
   - Или отключи временно (закомментируй `MuzzleFlash->Activate()`)

4. **Socket `weapon_r` не найден**
   - Решение: Добавь socket вручную в skeletal mesh
   - Или измени `WeaponSocket` name в `WeaponComponent`

5. **Input Actions не назначены**
   - Решение: Проверь, что все `IA_*` созданы и назначены в `PlayerController`

### Что НЕ реализовано (следующие этапы):

#### Week 4: Блокаут карты
- ❌ Базовый ландшафт (64 км²)
- ❌ Биомы (город, лес, промзона)
- ❌ Здания (серые боксы)
- ❌ World Partition настройка

#### Week 5: UI и полировка
- ❌ UI инвентаря (UMG виджет с сеткой)
- ❌ Drag-and-drop предметов
- ❌ Визуализация веса
- ❌ Подсказки взаимодействия ("Press E to pickup")

---

## 🚀 Что делать после успешного тестирования

Если всё работает:
1. Закоммить изменения (если создавал новые файлы в Content/)
2. Пуш в GitHub
3. Напиши мне: "Всё работает, продолжаем"

Если что-то сломано:
1. Скопируй ошибки из логов UE
2. Опиши, что не работает
3. Напиши мне с деталями

### Следующие шаги:

**Вариант A:** Продолжить с Week 3 (Инвентарь)
- Реализовать систему инвентаря (C++)
- Создать UI для инвентаря (UMG)
- Добавить подбор предметов

**Вариант B:** Перейти к Week 4 (Блокаут карты)
- Создать базовый ландшафт
- Настроить World Partition
- Добавить серые здания

**Вариант C:** Улучшить текущее
- Добавить анимации оружия
- Улучшить звуки
- Добавить эффекты (кровь, попадания)

---

## 📝 Технические детали для нейросети

### Структура репозитория:
```
TheDayBefore/
├── Source/TheDayBefore/
│   ├── Characters/
│   ├── Camera/
│   ├── Tactical/
│   ├── Weapons/
│   ├── Combat/
│   ├── Inventory/      **(NEW)**
│   └── GameFramework/
├── Content/
│   ├── Characters/
│   ├── Weapons/
│   ├── Maps/
│   ├── Input/
│   ├── DataTables/     **(NEW)**
│   └── ...
├── Config/
│   ├── DefaultEngine.ini
│   ├── DefaultGame.ini
│   └── ...
├── Docs/
│   ├── GDD.md
│   ├── TechnicalSpec.md
│   └── AI_Context.md (этот файл)
└── TheDayBefore.uproject
```

### Ключевые классы:
- `ATDBCharacter` → базовый FPS персонаж
- `UTDBFirstPersonCameraComponent` → камера с ADS
- `UTDBLeanComponent` → наклон Q/E
- `UTDBBreathComponent` → задержка дыхания
- `ATDBWeaponBase` → базовое оружие
- `ATDBPistol` → пример пистолета
- `UTDBWeaponComponent` → управление оружием
- `UTDBDamageSystem` → система урона
- `UTDBInventoryComponent` → инвентарь **(NEW)**
- `ATDBItemPickup` → подбор предметов **(NEW)**
- `FItemDefinition` → структура предмета **(NEW)**
- `FItemInstance` → экземпляр предмета **(NEW)**
- `ATDBGameMode` → игровые правила
- `ATDBPlayerController` → ввод
- `UTDBGameInstance` → глобальные системы

### Сетевая репликация:
- `bIsFiring`, `bIsReloading`, `CurrentAmmo` → оружие
- `LeanState` → наклон
- `AimState` → прицеливание
- `BreathStamina` → дыхание
- `Items` array → инвентарь **(NEW)**

### Важные сокеты:
- `weapon_r` → правая рука для оружия

### Важные параметры:
- `DefaultFOV` = 90
- `ADSFOV` = 45
- `MaxLeanAngle` = 15
- `MaxBreathStamina` = 100

---

## 🎮 Управление (для тестирования)

**Движение:**
- W/S — вперёд/назад
- A/D — влево/вправо
- Space — прыжок
- C — присед
- Left Shift — спринт

**Камера:**
- Mouse X/Y — обзор

**Тактика:**
- Right Mouse — прицеливание (ADS)
- Q — наклон влево
- E — наклон вправо
- Left Shift (при ADS) — задержка дыхания

**Оружие:**
- Left Mouse — стрельба
- R — перезарядка

**Инвентарь:**
- E — подобрать предмет (в радиусе 2м)

**Инвентарь (NEW):**
- E — подобрать предмет (в радиусе 2м)

---

## 📚 Полезные ссылки

- **GitHub:** https://github.com/zxDOOMxz/The-Day-Before
- **GDD:** Docs/GDD.md
- **Technical Spec:** Docs/TechnicalSpec.md
- **UE 5.8 Docs:** https://docs.unrealengine.com/5.8/

---

**Конец документа**

*Этот документ обновляется после каждого значимого изменения. Нейросеть должна прочитать этот файл перед началом работы над проектом.*
