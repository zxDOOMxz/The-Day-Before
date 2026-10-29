# Техническое задание (Technical Specification) — The Day Before: Aftermath

**Версия:** 1.0  
**Дата:** 29 октября 2026  
**Статус:** Approved for Development  
**Движок:** Unreal Engine 5.8  
**Платформа:** PC (Windows 10/11, 64-bit)

---

## 1. Архитектура проекта

### 1.1 Модульная структура C++

Проект разделён на **7 независимых модулей**, каждый со своей зоной ответственности:

```
TheDayBefore/
├── Source/
│   ├── TheDayBeforeCore/          # Модуль 1: Ядро
│   │   ├── Framework/             # GameMode, GameState, PlayerState
│   │   ├── Characters/            # Character base classes
│   │   ├── Components/            # Базовые компоненты
│   │   └── Utilities/             # Утилиты, хелперы
│   │
│   ├── TheDayBeforeInventory/     # Модуль 2: Инвентарь
│   │   ├── InventorySystem/       # Система инвентаря
│   │   ├── ItemSystem/            # Предметы, стеки, категории
│   │   ├── Containers/            # Контейнеры (рюкзак, карманы)
│   │   └── LootSystem/            # Спавн лута, таблицы
│   │
│   ├── TheDayBeforeSurvival/      # Модуль 3: Выживание
│   │   ├── StatusSystem/          # Голод, жажда, здоровье
│   │   ├── DiseaseSystem/         # Болезни, лечение
│   │   ├── EnvironmentSystem/     # Температура, радиация
│   │   └── StressSystem/          # Стресс, психологические эффекты
│   │
│   ├── TheDayBeforeCombat/        # Модуль 4: Боевая система
│   │   ├── WeaponSystem/          # Оружие, баллистика
│   │   ├── DamageSystem/          # Урон, броня, пробитие
│   │   ├── AttachmentSystem/      # Модификации оружия
│   │   └── CombatAI/              # AI в бою
│   │
│   ├── TheDayBeforeEconomy/       # Модуль 5: Экономика
│   │   ├── TradingSystem/         # Торговля (NPC, P2P)
│   │   ├── CurrencySystem/        # Валюта, обмен
│   │   ├── CraftingSystem/        # Крафт, верстаки
│   │   └── ClanSystem/            # Кланы, территории
│   │
│   ├── TheDayBeforeAI/            # Модуль 6: Искусственный интеллект
│   │   ├── InfectedAI/            # Заражённые (Behavior Trees)
│   │   ├── HumanAI/               # AI-люди (мародёры, военные)
│   │   ├── PerceptionSystem/      # Восприятие (звук, зрение)
│   │   └── DirectorAI/            # Управление напряжением
│   │
│   └── TheDayBeforeNetwork/       # Модуль 7: Сеть
│       ├── Replication/           # Репликация данных
│       ├── InterestManagement/    # Interest-based репликация
│       ├── AntiCheat/             # Серверная валидация
│       └── Sync/                  # Синхронизация состояния
```

### 1.2 Зависимости модулей

```
Core ← Все модули зависят от Core

Inventory ← Combat (оружие в инвентаре)
Inventory ← Economy (торговля предметами)

Survival ← Core (статусы персонажа)
Survival ← Inventory (еда, лекарства)

Combat ← Inventory (оружие, патроны)
Combat ← Survival (урон, лечение)

Economy ← Inventory (предметы для торговли)
Economy ← Crafting (рецепты)

AI ← Combat (боевое поведение)
AI ← Survival (AI тоже выживает)

Network ← Все модули (репликация)
```

### 1.3 Blueprint-системы

Blueprint используются для:

| Слой | Примеры | Обоснование |
|------|---------|-------------|
| **UI/HUD** | Виджеты инвентаря, меню, диалоги | Быстрая итерация, дизайн |
| **Контент** | DataTables, DataAssets (лут-таблицы, рецепты) | Non-programmer access |
| **AI поведение** | Behavior Trees, Blackboards | Визуальное редактирование |
| **Прототипы** | Новые механики до стабилизации | Быстрая проверка идеи |
| **Квесты** | Квестовая логика, диалоги | Дизайнерский контроль |

**Правило:** Если система стабильна и требует производительности — рефакторинг в C++.

### 1.4 Компонентная архитектура

Каждый актёр использует **компонентный подход**:

```cpp
// Пример: APlayerCharacter
UCLASS()
class APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Компоненты
    UPROPERTY()
    class UInventoryComponent* Inventory;
    
    UPROPERTY()
    class USurvivalComponent* Survival;
    
    UPROPERTY()
    class UCombatComponent* Combat;
    
    UPROPERTY()
    class UInteractionComponent* Interaction;
    
    UPROPERTY()
    class UNetworkComponent* NetSync;
};
```

**Преимущества:**
- Переиспользование кода
- Независимое тестирование компонентов
- Гибкая настройка через Blueprint

---

## 2. Структуры данных

### 2.1 Инвентарь (Inventory System)

#### 2.1.1 Основная структура

```cpp
USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    // Позиция в сетке (X, Y)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FIntPoint Position;

    // Размер предмета (Width, Height)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FIntPoint Size;

    // Предмет в слоте
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FItemInstance Item;

    // Заблокирован (для drag-and-drop)
    UPROPERTY()
    bool bIsLocked;
};

USTRUCT(BlueprintType)
struct FInventory
{
    GENERATED_BODY()

    // Уникальный ID инвентаря
    UPROPERTY()
    FGuid InventoryId;

    // Владелец (Player, NPC, Container)
    UPROPERTY()
    TWeakObjectPtr<UObject> Owner;

    // Размер сетки (например, 4x4 для стандартного рюкзака)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FIntPoint GridSize;

    // Слоты
    UPROPERTY()
    TArray<FInventorySlot> Slots;

    // Максимальный вес
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MaxWeight;

    // Текущий вес
    UPROPERTY(Replicated)
    float CurrentWeight;

    // Методы
    bool CanAddItem(const FItemDefinition& ItemDef, int32 Quantity) const;
    bool AddItem(const FItemInstance& Item);
    bool RemoveItem(const FGuid& ItemId, int32 Quantity);
    bool MoveItem(const FGuid& ItemId, const FIntPoint& NewPosition);
    FItemInstance* FindItem(const FGuid& ItemId);
};
```

#### 2.1.2 Предметы (Items)

```cpp
USTRUCT(BlueprintType)
struct FItemDefinition
{
    GENERATED_BODY()

    // Уникальный ID (из DataTable)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName ItemId;

    // Название
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayName;

    // Описание
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;

    // Иконка
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* Icon;

    // 3D модель (для мира)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* WorldMesh;

    // Размер в инвентаре (1x1, 2x2, 1x3...)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FIntPoint InventorySize;

    // Вес (кг)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Weight;

    // Категория
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EItemCategory Category;

    // Редкость
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EItemRarity Rarity;

    // Максимальный стек (0 = не стекается)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxStackSize;

    // Базовая цена
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 BaseValue;
};

USTRUCT(BlueprintType)
struct FItemInstance
{
    GENERATED_BODY()

    // Уникальный ID экземпляра
    UPROPERTY()
    FGuid InstanceId;

    // Ссылка на определение
    UPROPERTY()
    FName DefinitionId;

    // Количество (для стеков)
    UPROPERTY(Replicated)
    int32 Quantity;

    // Состояние (0-100%)
    UPROPERTY(Replicated)
    float Condition;

    // Модификации (для оружия)
    UPROPERTY(Replicated)
    TArray<FItemInstance> Attachments;

    // Кастомные данные (耐久, заряд, etc.)
    UPROPERTY()
    TMap<FName, float> CustomData;
};

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
    Weapon,
    Armor,
    Ammo,
    Medical,
    Food,
    Resource,
    Key,
    Quest,
    Misc
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    Common,
    Uncommon,
    Rare,
    Epic,
    Legendary
};
```

#### 2.1.3 Хранение данных

**DataTables** (для определений предметов):
- `DT_ItemDefinitions` — все предметы в игре
- `DT_LootTables` — таблицы спавна лута по зонам
- `DT_CraftingRecipes` — рецепты крафта

**Сохранение** (для экземпляров):
- Локально: `USaveGame` (для одиночной игры)
- Сервер: База данных (SQL/NoSQL) для мультиплеера

### 2.2 Прогресс игрока (Player Progression)

```cpp
USTRUCT(BlueprintType)
struct FPlayerProgression
{
    GENERATED_BODY()

    // Уровень (1-80)
    UPROPERTY(Replicated)
    int32 Level;

    // Опыт
    UPROPERTY(Replicated)
    int64 Experience;

    // Навыки (прокачка использованием)
    UPROPERTY(Replicated)
    TMap<ESkillType, FSkillData> Skills;

    // Репутация с фракциями
    UPROPERTY(Replicated)
    TMap<FName, int32> FactionReputation;

    // Разблокированные рецепты
    UPROPERTY(Replicated)
    TArray<FName> UnlockedRecipes;

    // Достижения
    UPROPERTY()
    TArray<FName> Achievements;

    // Статистика
    UPROPERTY()
    FPlayerStatistics Statistics;
};

USTRUCT(BlueprintType)
struct FSkillData
{
    GENERATED_BODY()

    // Уровень навыка (1-100)
    UPROPERTY(Replicated)
    int32 Level;

    // Прогресс до следующего уровня (0-100%)
    UPROPERTY(Replicated)
    float Progress;

    // Общий опыт в этом навыке
    UPROPERTY()
    int64 TotalExperience;
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Endurance,      // Выносливость
    Marksmanship,   // Стрельба
    Medicine,       // Медицина
    Crafting,       // Крафт
    Stealth,        // Скрытность
    Trading,        // Торговля
    Strength,       // Сила
    Agility         // Ловкость
};
```

### 2.3 Сохранение состояния

**Локальное сохранение (Stash, настройки):**
```cpp
UCLASS()
class UPlayerSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    FPlayerProgression Progression;

    UPROPERTY()
    FInventory StashInventory;

    UPROPERTY()
    FVector LastLocation;

    UPROPERTY()
    FRotator LastRotation;

    UPROPERTY()
    TArray<FQuestState> ActiveQuests;

    UPROPERTY()
    TMap<FName, float> PlayerSettings;
};
```

**Серверное сохранение (мультиплеер):**
- Используется **PostgreSQL** для структурных данных
- **Redis** для кэша и сессий
- Сериализация через **JSON** или **Protobuf**

---

## 3. Сетевая модель

### 3.1 Архитектура

**Dedicated Server** (авторитетный сервер):
- Сервер владеет всей игровой логикой
- Клиенты отправляют только **input** (нажатия клавиш, движения мыши)
- Сервер валидирует всё и рассылает обновления

```
[Client 1] ──Input──> [Server] ──State──> [Client 1]
[Client 2] ──Input──> [Server] ──State──> [Client 2]
[Client 3] ──Input──> [Server] ──State──> [Client 3]
```

### 3.2 Репликация

#### 3.2.1 Что реплицируется

| Данные | Частота | Метод | Приоритет |
|--------|---------|-------|-----------|
| **Позиция игрока** | 30 Hz | RepNotify | Высокий |
| **Анимации** | 30 Hz | RepNotify | Высокий |
| **Здоровье** | On change | ReplicatedUsing | Высокий |
| **Инвентарь** | On change | ReplicatedUsing | Средний |
| **Позиция лута** | 10 Hz | RepNotify | Средний |
| **Заражённые AI** | 20 Hz | RepNotify | Средний |
| **Погода** | 0.1 Hz | RepNotify | Низкий |
| **Время суток** | 0.1 Hz | RepNotify | Низкий |

#### 3.2.2 Пример репликации

```cpp
UCLASS()
class APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Позиция реплицируется
    UPROPERTY(ReplicatedUsing = OnRep_Position)
    FVector_NetQuantize10 ReplicatedPosition;

    UFUNCTION()
    void OnRep_Position();

    // Здоровье реплицируется при изменении
    UPROPERTY(ReplicatedUsing = OnRep_Health)
    float Health;

    UFUNCTION()
    void OnRep_Health();

    // Инвентарь реплицируется целиком при изменении
    UPROPERTY(ReplicatedUsing = OnRep_Inventory)
    FInventory ReplicatedInventory;

    UFUNCTION()
    void OnRep_Inventory();

    // Стандартная репликация
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
    {
        DOREPLIFETIME(APlayerCharacter, ReplicatedPosition);
        DOREPLIFETIME(APlayerCharacter, Health);
        DOREPLIFETIME(APlayerCharacter, ReplicatedInventory);
    }
};
```

### 3.3 Interest Management

**Проблема:** На карте 100 игроков, но каждый видит только 30-40 других.

**Решение:** Interest-based репликация — клиент получает данные только о близких объектах.

```cpp
UCLASS()
class UInterestManager : public UActorComponent
{
    GENERATED_BODY()

public:
    // Радиус интереса (метры)
    UPROPERTY(EditAnywhere)
    float InterestRadius = 15000.0f; // 15 км

    // Обновление interest каждые N секунд
    UPROPERTY(EditAnywhere)
    float UpdateInterval = 1.0f;

    // Актуальный список реплицируемых актёров
    UPROPERTY()
    TSet<AActor*> RelevantActors;

    // Проверка, должен ли актёр реплицироваться для клиента
    bool IsRelevantForClient(AActor* Actor, APlayerController* Client) const
    {
        float Distance = FVector::Dist(Actor->GetActorLocation(), Client->GetPawn()->GetActorLocation());
        return Distance <= InterestRadius;
    }

    // Обновление списка релевантных актёров
    void UpdateRelevancy()
    {
        for (APlayerController* PC : GetAllPlayers())
        {
            TArray<AActor*> NewRelevantActors;
            
            for (AActor* Actor : GetAllActors())
            {
                if (IsRelevantForClient(Actor, PC))
                {
                    NewRelevantActors.Add(Actor);
                }
            }
            
            // UE автоматически обновит репликацию
            PC->SetRelevantActors(NewRelevantActors);
        }
    }
};
```

### 3.4 RPC (Remote Procedure Calls)

**Server RPC** (клиент → сервер):
```cpp
UFUNCTION(Server, Reliable)
void Server_MoveTo(FVector Destination);

UFUNCTION(Server, Reliable)
void Server_PickupItem(FGuid ItemId);

UFUNCTION(Server, Unreliable)
void Server_FireWeapon(FVector Direction);
```

**Client RPC** (сервер → конкретный клиент):
```cpp
UFUNCTION(Client, Reliable)
void Client_ShowDamageNumber(float Damage, FVector HitLocation);

UFUNCTION(Client, Reliable)
void Client_PlayHitSound(ESoundType SoundType);
```

**Multicast RPC** (сервер → все клиенты):
```cpp
UFUNCTION(NetMulticast, Unreliable)
void Multicast_PlayExplosionEffect(FVector Location, float Radius);
```

### 3.5 Синхронизация времени

**Проблема:** Задержка (latency) между клиентом и сервером.

**Решение:**
- **Server-Authoritative:** Сервер решает всё, клиент только предсказывает
- **Client-Side Prediction:** Клиент предсказывает движение, сервер корректирует
- **Server Reconciliation:** Клиент откатывается к серверному состоянию и пересчитывает

```cpp
UCLASS()
class APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Клиент предсказывает движение
    void MoveInput(FVector Direction)
    {
        if (IsLocallyControlled())
        {
            // Сохраняем input для późniejszej реконсиляции
            SavedMoves.Add(MoveId, Direction);
            
            // Применяем движение локально
            AddMovementInput(Direction);
            
            // Отправляем на сервер
            Server_Move(Direction, MoveId);
            MoveId++;
        }
    }

    UFUNCTION(Server, Reliable)
    void Server_Move(FVector Direction, int32 MoveId);

    // Сервер корректирует позицию клиента
    UFUNCTION(Client, Reliable)
    void Client_CorrectPosition(FVector ServerPosition, int32 LastProcessedMove)
    {
        // Откатываемся к серверной позиции
        SetActorLocation(ServerPosition);
        
        // Пересчитываем все неподтверждённые движения
        for (auto& Move : SavedMoves)
        {
            if (Move.Key > LastProcessedMove)
            {
                AddMovementInput(Move.Value);
            }
        }
    }
};
```

### 3.6 Anti-Cheat

**Серверная валидация:**

```cpp
bool UAntiCheatSystem::ValidateMovement(APlayerCharacter* Character, const FMoveData& MoveData)
{
    // Проверка скорости
    float MaxSpeed = Character->GetMaxSpeed();
    if (MoveData.Speed > MaxSpeed * 1.1f) // 10% tolerance
    {
        return false; // Чит!
    }

    // Проверка коллизий
    if (MoveData.bIsClipping)
    {
        return false;
    }

    // Проверка teleportation
    float Distance = FVector::Dist(MoveData.StartLocation, MoveData.EndLocation);
    if (Distance > MaxSpeed * DeltaTime * 2.0f)
    {
        return false;
    }

    return true;
}

bool UAntiCheatSystem::ValidateDamage(APlayerCharacter* Attacker, AActor* Target, float Damage)
{
    // Проверка дистанции
    float Distance = FVector::Dist(Attacker->GetActorLocation(), Target->GetActorLocation());
    float MaxRange = Attacker->GetWeaponRange();
    
    if (Distance > MaxRange * 1.1f)
    {
        return false; // Слишком далеко
    }

    // Проверка линии видимости
    if (!HasLineOfSight(Attacker, Target))
    {
        return false; // Стреляет сквозь стены
    }

    return true;
}
```

---

## 4. Пайплайны загрузки

### 4.1 World Partition

**UE 5.8 World Partition** — автоматическое разбиение мира на грид-ячейки.

```cpp
// Настройки World Partition (в редакторе)
UCLASS()
class UWorldPartitionSettings : public UObject
{
    GENERATED_BODY()

public:
    // Размер грид-ячейки (метры)
    UPROPERTY(EditAnywhere)
    int32 GridSize = 4096; // 4 км

    // Загружать ячейки в радиусе (в ячейках)
    UPROPERTY(EditAnywhere)
    int32 LoadingRadius = 2; // 2 ячейки вокруг игрока

    // Стримить ассеты
    UPROPERTY(EditAnywhere)
    bool bStreamAssets = true;
};
```

**Как работает:**
1. Мир разбит на 16x16 ячеек (64 км / 4 км = 16)
2. Игрок в центре, загружается 5x5 ячеек вокруг (20x20 км)
3. Далекие ячейки выгружаются из памяти

### 4.2 Data Layers

**Data Layers** — загрузка/выгрузка состояний мира.

```cpp
UCLASS()
class AWorldStateManager : public AActor
{
    GENERATED_BODY()

public:
    // Слои данных
    UPROPERTY()
    UDataLayerInstance* DayLayer;      // День (безопасный)
    
    UPROPERTY()
    UDataLayerInstance* NightLayer;    // Ночь (опасный)
    
    UPROPERTY()
    UDataLayerInstance* RaidLayer;     // Рейд (PvP активен)

    // Переключение слоя
    void SwitchToNightMode()
    {
        NightLayer->SetIsActivated(true);
        DayLayer->SetIsActivated(false);
        
        // Спавн ночных заражённых
        SpawnNightInfected();
        
        // Изменение AI поведения
        UpdateAIBehavior(ENightBehavior::Aggressive);
    }
};
```

### 4.3 Striming ассетов

**Soft References** — загрузка только когда нужно:

```cpp
UCLASS()
class AItemSpawner : public AActor
{
    GENERATED_BODY()

public:
    // Soft reference на меш предмета
    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<UStaticMesh> ItemMesh;

    // Загрузка при приближении игрока
    void OnPlayerApproached()
    {
        if (!ItemMesh.IsValid())
        {
            StreamableManager.RequestAsyncLoad(
                ItemMesh.ToSoftObjectPath(),
                FStreamableDelegate::CreateUObject(this, &AItemSpawner::OnMeshLoaded)
            );
        }
    }

    void OnMeshLoaded()
    {
        UStaticMesh* LoadedMesh = ItemMesh.Get();
        GetStaticMeshComponent()->SetStaticMesh(LoadedMesh);
    }
};
```

### 4.4 Оптимизация

**LOD (Level of Detail):**
```cpp
// Nanite автоматически управляет LOD
// Но для не-Nanite объектов:
UCLASS()
class ABuilding : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    TArray<UStaticMesh*> LODMeshes;

    void UpdateLOD(float DistanceToPlayer)
    {
        int32 LODIndex = 0;
        
        if (DistanceToPlayer > 10000.0f) LODIndex = 3;
        else if (DistanceToPlayer > 5000.0f) LODIndex = 2;
        else if (DistanceToPlayer > 1000.0f) LODIndex = 1;
        
        GetStaticMeshComponent()->SetStaticMesh(LODMeshes[LODIndex]);
    }
};
```

**Occlusion Culling:**
- UE автоматически скрывает объекты за стенами
- Настраивается через `Occlusion Query`

**HLOD (Hierarchical LOD):**
- Далекие объекты объединяются в один меш
- Настраивается через `Hierarchical LOD System`

---

## 5. API интерфейсы

### 5.1 Game Instance API

**Централизованный доступ к системам:**

```cpp
UCLASS()
class UTDGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // Системы (синглтоны)
    UPROPERTY()
    UInventorySystem* InventorySystem;
    
    UPROPERTY()
    USurvivalSystem* SurvivalSystem;
    
    UPROPERTY()
    UCombatSystem* CombatSystem;
    
    UPROPERTY()
    UEconomySystem* EconomySystem;
    
    UPROPERTY()
    UAISystem* AISystem;
    
    UPROPERTY()
    UNetworkSystem* NetworkSystem;

    // Инициализация при старте
    virtual void Init() override
    {
        Super::Init();
        
        InventorySystem = NewObject<UInventorySystem>(this);
        SurvivalSystem = NewObject<USurvivalSystem>(this);
        CombatSystem = NewObject<UCombatSystem>(this);
        EconomySystem = NewObject<UEconomySystem>(this);
        AISystem = NewObject<UAISystem>(this);
        NetworkSystem = NewObject<UNetworkSystem>(this);
        
        // Инициализация систем
        InventorySystem->Initialize();
        SurvivalSystem->Initialize();
        CombatSystem->Initialize();
        EconomySystem->Initialize();
        AISystem->Initialize();
        NetworkSystem->Initialize();
    }
};
```

### 5.2 Event System

**Делегаты для слабой связанности:**

```cpp
UCLASS()
class UEventSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // События
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDied, APlayerCharacter*, Player);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemPickedUp, APlayerCharacter*, Player, const FItemInstance&, Item);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerDamaged, APlayerCharacter*, Player, float, Damage, AActor*, Attacker);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExtractionSuccess, APlayerCharacter*, Player, const FVector&, Location);

    UPROPERTY()
    FOnPlayerDied OnPlayerDied;
    
    UPROPERTY()
    FOnItemPickedUp OnItemPickedUp;
    
    UPROPERTY()
    FOnPlayerDamaged OnPlayerDamaged;
    
    UPROPERTY()
    FOnExtractionSuccess OnExtractionSuccess;

    // Вызов событий
    void BroadcastPlayerDied(APlayerCharacter* Player)
    {
        OnPlayerDied.Broadcast(Player);
    }

    void BroadcastItemPickedUp(APlayerCharacter* Player, const FItemInstance& Item)
    {
        OnItemPickedUp.Broadcast(Player, Item);
    }
};
```

### 5.3 Component Interfaces

**Интерфейсы для взаимодействия:**

```cpp
// Интерфейс для предметов, которые можно подобрать
UINTERFACE(MinimalAPI, Blueprintable)
class UPickable : public UInterface
{
    GENERATED_BODY()
};

class IPickable
{
    GENERATED_BODY()

public:
    // Можно ли подобрать?
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    bool CanBePickedUp(APlayerCharacter* Picker) const;

    // Подобрать предмет
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    void Pickup(APlayerCharacter* Picker);
};

// Интерфейс для взаимодействуемых объектов
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};

class IInteractable
{
    GENERATED_BODY()

public:
    // Можно ли взаимодействовать?
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    bool CanInteract(APlayerCharacter* Player) const;

    // Взаимодействовать
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    void Interact(APlayerCharacter* Player);

    // Текст подсказки (например, "Нажмите E чтобы открыть")
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    FText GetInteractionPrompt() const;
};
```

### 5.4 Save/Load System

**Система сохранений:**

```cpp
UCLASS()
class USaveSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Сохранить игру
    UFUNCTION(BlueprintCallable)
    bool SaveGame(const FString& SaveSlot)
    {
        UPlayerSaveGame* SaveGameInstance = Cast<UPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass()));
        
        // Собираем данные
        SaveGameInstance->Progression = GetPlayerProgression();
        SaveGameInstance->StashInventory = GetStashInventory();
        SaveGameInstance->LastLocation = GetPlayerLocation();
        SaveGameInstance->ActiveQuests = GetActiveQuests();
        
        // Сохраняем
        return UGameplayStatics::SaveGameToDisk(SaveSlot, SaveGameInstance);
    }

    // Загрузить игру
    UFUNCTION(BlueprintCallable)
    bool LoadGame(const FString& SaveSlot)
    {
        USaveGame* LoadedGame = UGameplayStatics::LoadGameFromDisk(SaveSlot);
        UPlayerSaveGame* SaveGameInstance = Cast<UPlayerSaveGame>(LoadedGame);
        
        if (SaveGameInstance)
        {
            // Восстанавливаем данные
            SetPlayerProgression(SaveGameInstance->Progression);
            SetStashInventory(SaveGameInstance->StashInventory);
            SetPlayerLocation(SaveGameInstance->LastLocation);
            SetActiveQuests(SaveGameInstance->ActiveQuests);
            
            return true;
        }
        
        return false;
    }
};
```

---

## 6. Производительность и оптимизация

### 6.1 Целевые метрики

| Платформа | FPS | Разрешение | Настройки |
|-----------|-----|------------|-----------|
| **RTX 3070** | 60 FPS | 1440p | High/Ultra |
| **RTX 2060** | 30 FPS | 1080p | Medium |
| **GTX 1660** | 30 FPS | 1080p | Low |

### 6.2 Профилирование

**Инструменты:**
- **Unreal Insights** — основной инструмент профилирования
- **Stat Unit** — CPU, GPU, Draw Calls
- **GPU Visualizer** — детальный анализ GPU
- **Memory Profiler** — использование RAM/VRAM

**Бюджеты:**

| Метрика | Бюджет | Критический |
|---------|--------|-------------|
| **Frame Time** | 16.6 ms (60 FPS) | 33 ms (30 FPS) |
| **Draw Calls** | < 3000 | > 5000 |
| **Triangles** | < 10M | > 20M |
| **RAM** | < 12 GB | > 16 GB |
| **VRAM** | < 6 GB | > 8 GB |

### 6.3 Оптимизация AI

**Проблема:** 500+ заражённых на карте.

**Решение:**
```cpp
UCLASS()
class AInfectedAI : public ACharacter
{
    GENERATED_BODY()

public:
    // Обновление AI только когда игрок близко
    UPROPERTY(EditAnywhere)
    float AIUpdateDistance = 5000.0f; // 5 км

    // Тик раз в N кадров для далеких AI
    UPROPERTY(EditAnywhere)
    int32 TickInterval = 10; // Каждый 10-й кадр

    virtual void Tick(float DeltaTime) override
    {
        Super::Tick(DeltaTime);

        // Проверка дистанции до ближайшего игрока
        float DistanceToPlayer = GetDistanceToNearestPlayer();
        
        if (DistanceToPlayer > AIUpdateDistance)
        {
            // Обновляем AI реже
            if (GetWorld()->GetFrameNumber() % TickInterval != 0)
            {
                return;
            }
        }

        // Обновление AI логики
        UpdateAIBehavior();
    }
};
```

### 6.4 Оптимизация сети

**Проблема:** Высокий трафик при 100 игроках.

**Решение:**
- **Bandwidth:** < 100 KB/s на игрока
- **Compression:** Сжатие данных (позиция → FVector_NetQuantize10)
- **Prioritization:** Важные данные (здоровье) реплицируются чаще
- **Deduplication:** Не отправлять одинаковые данные дважды

```cpp
// Сжатие позиции (10 бит на компонент = ~3 мм точность)
UPROPERTY(Replicated)
FVector_NetQuantize10 NetworkPosition;

// Сжатие вращения (16 бит на компонент)
UPROPERTY(Replicated)
FRotator_NetQuantize Normal;
```

---

## 7. Безопасность

### 7.1 Серверная валидация

**Никогда не доверяй клиенту!**

```cpp
// Плохо: Клиент говорит "я подобрал предмет"
UFUNCTION(Server, Reliable)
void Server_PickupItem(FGuid ItemId)
{
    // Без проверки!
    Inventory->AddItem(ItemId);
}

// Хорошо: Сервер проверяет
UFUNCTION(Server, Reliable)
void Server_PickupItem(FGuid ItemId)
{
    // Проверка дистанции
    AItem* Item = FindItemById(ItemId);
    if (!Item) return;
    
    float Distance = GetDistanceTo(Item);
    if (Distance > 200.0f) // Слишком далеко
    {
        return;
    }
    
    // Проверка линии видимости
    if (!HasLineOfSightTo(Item))
    {
        return;
    }
    
    // Проверка, что предмет ещё не подобран
    if (Item->bIsPickedUp)
    {
        return;
    }
    
    // Всё ок, подбираем
    Inventory->AddItem(ItemId);
    Item->bIsPickedUp = true;
}
```

### 7.2 Anti-Cheat интеграция

**Easy Anti-Cheat (EAC) / BattlEye:**
```cpp
// В TheDayBefore.Build.cs
PublicAdditionalLibraries.Add("EasyAntiCheat.lib");

// Инициализация при старте
void ATDGameMode::InitGame()
{
    Super::InitGame();
    
    // Запуск EAC
    EAC_Launch("TheDayBefore", "1.0.0");
}
```

---

## 8. Следующие шаги

### Phase 0: Прототипы (4 недели)

1. **Неделя 1:** Движение персонажа + камера
2. **Неделя 2:** Базовая стрельба + урон
3. **Неделя 3:** Инвентарь (прототип)
4. **Неделя 4:** Блокаут карты (серый бокс-лейвл)

### Phase 1: Core Systems (12 недель)

- Полная система инвентаря
- Система выживания
- Боевая система (оружие, урон, броня)
- AI заражённых (базовые типы)
- Сетевая инфраструктура

---

**Конец документа**

*Документ подготовлен для разработки "The Day Before: Aftermath" на UE 5.8*
