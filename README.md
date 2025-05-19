# ZooSIMS — Симулятор управления зоопарком

## Зачем?
Целью проекта было закрепить знания по ООП (объектно-ориентированному программированию), научиться проектировать сложные системы из объектов, использовать STL-контейнеры, указатели, работу с памятью и реализовать игровой цикл. Проект демонстрирует умение моделировать реальные процессы.

## Почему?
Потому что преподаватель так сказал сделать 

---

## 🛠 Технологии и инструменты

- **C++17** — основной язык программирования.
- **STL (Standard Template Library)** — использовались `vector`, `map`, `unordered_map`, `set`, `shared_ptr`, `unique_ptr`.
- **Windows API (частично)** — для поддержки юникода в консоли (`_setmode`).
- **Git + GitHub** — система контроля версий и хостинг проекта.

---

## 📁 Структура проекта
├── ZooSIMS.cpp # Основной файл программы

├── README.md # Этот файл

└── .gitignore # Файл исключений для Git

---

📚 Документация: 
```bash
 Enums
•  AnimalHealth: Состояние здоровья животного (HEALTHY, SICK, DEAD).
•  AnimalType: Тип животного (LAND, AQUATIC, BIRD, REPTILE).
•  EnclosureType: Типы вольеров (SMALL, MEDIUM, LARGE, HUGE, AQUARIUM, BIRD_CAGE, REPTILE_HOUSE, PETTING_ZOO).
•  WorkerType: Роли работников (VET, CLEANER, FEEDER, TRAINER, GUIDE).
 Константы
•  healthStatus: Массив строк с состояниями здоровья (Здоров, Болен, Мертв).
•  animalTypes: Массив строк с типами животных (Наземное, Водное, Птица, Рептилия).
•  climates: Доступные климаты (Тропический, Умеренный, Полярный, Пустынный, Водный).
•  animalSpecies: Список видов животных (например, Лев, Тигр, Медведь, Дельфин).
•  speciesClimates: Соответствие видов и подходящих климатов (например, Лев → Тропический, Пустынный).
•  speciesTypes: Соответствие видов и типов животных (например, Лев → LAND).
•  workerTypeNames: Названия ролей работников (например, VET → Ветеринар).
•  workerBaseSalaries: Базовые зарплаты работников (например, VET → 40,000 руб.).
•  speciesMaxAge: Максимальный возраст для каждого вида (случайный диапазон, например, Лев → 15-25 дней).
•  enclosureTypes: Характеристики вольеров (тип, вместимость, стоимость, название).
 Классы
 Animal
Свойства:
 •  id, name, species, age, weight, climate, isPredator, price, gender, isAlive, trueHappiness, displayedHappiness, maxAge, parentId1, parentId2, health, type, daysSick, hasDisease.
Методы:
 •  updateHappiness: Обновляет уровень счастья на основе еды, чистоты и одиночества.
 •  tryEscape: Проверяет шанс побега при низком счастье.
 •  checkAge: Проверяет смерть от старости.
 •  rename: Изменяет имя животного.
 •  canReproduce: Проверяет возможность размножения.
 •  operator+: Симулирует размножение с другим животным, возвращает потомство.
 •  checkDisease: Управляет болезнями и выздоровлением/смертью.
 Enclosure
Свойства:
 •  id, capacity, type, climate, maintenanceCost, isClean, containedAnimals.
Методы:
 •  isSuitableForAnimalType: Проверяет совместимость типа вольера с животным.
 •  addAnimal/removeAnimal: Управление животными в вольере.
 •  clean: Чистит вольер.
 •  spreadDisease: Распространение болезни среди животных.
 Worker
Свойства:
 •  id, name, type, salary, capacity.
Методы:
 •  performDuties: Выполняет задачи в зависимости от типа (чистка, лечение, кормление и т.д.).
 •  cleanEnclosures: Чистит вольеры.
 •  treatAnimals: Лечит больных животных.
 Zoo
Свойства:
 •  name, money, food, popularity, animals, enclosures, workers, visitors, days, victoryDays, nextAnimalId, nextEnclosureId, nextWorkerId, directorName, marketAnimals, marketRefreshCost, lastMarketRefresh, animalsBoughtToday.
Методы:
 •  refreshMarket: Обновляет рынок животных.
 •  buyAnimal/sellAnimal: Покупка и продажа животных.
 •  buyEnclosure: Создает новый вольер.
 •  hireWorker: Нанимает работника.
 •  nextDay: Симулирует новый день (обновление возраста, чистоты, счастья, дохода).
 •  payWorkers: Выплачивает зарплаты.

```

---

## Пример использования

Купить животное из магазина

Нанять ветеринара и уборщика

Покормить животных

Повысить популярность через рекламу

Разводить животных в одном вольере

Пройти 100 дней успешного управления зоопарком\

---

**Цель игры**

Успешно управлять зоопарком не менее 100 дней , сохраняя баланс денег, еды, здоровья животных и популярности.

---


## 🧪 Как запустить проект?

### Требования:
- Windows (или совместимая платформа с поддержкой `_setmode`)
- Компилятор C++17 (MSVC / g++ / clang++)
- Поддержка Unicode в консоли

### Сборка:
```bash
g++ -std=c++17 -o ZooSIMS main.cpp
