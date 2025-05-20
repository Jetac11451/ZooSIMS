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
// Обновляет отображаемый уровень счастья
    void updateDisplayedHappiness() { displayedHappiness = trueHappiness; }

    /*
     * Возвращает цвет для отображения уровня счастья
     * Разные цвета для разных диапазонов счастья
     */
    wstring getHappinessColor() const {
        if (!isAlive) return L"\033[90m"; // Серый для мертвых
        if (displayedHappiness >= 80) return L"\033[32m"; // Зеленый
        if (displayedHappiness >= 60) return L"\033[33m"; // Желтый
        if (displayedHappiness >= 40) return L"\033[93m"; // Ярко-желтый
        if (displayedHappiness >= 20) return L"\033[31m"; // Красный
        return L"\033[91m"; // Ярко-красный
    }

    /*
     * Обновляет уровень счастья на основе условий содержания
     * Учитывает кормление, чистоту вольера, одиночество и болезнь
     */
    void updateHappiness(bool isFed, bool isAlone, bool isClean) {
        if (!isAlive) return;
        int oldHappiness = trueHappiness;
        int change = 0;
        if (!isFed) change -= 25;
        if (!isClean) change -= 20;
        if (isAlone) change -= (isPredator ? 10 : 20);
        if (hasDisease) change -= 15;
        trueHappiness = max(0, min(100, trueHappiness + change));
        displayedHappiness = trueHappiness;
        if (oldHappiness != trueHappiness) {
            wstring message = name + L" (ID:" + to_wstring(id) + L") ";
            if (!isFed) message += L"голодает! ";
            if (!isClean) message += L"в грязном вольере! ";
            if (isAlone) message += L"одинок! ";
            if (hasDisease) message += L"болен! ";
            message += L"Счастье: " + to_wstring(oldHappiness) + L"% -> " + to_wstring(trueHappiness) + L"%";
            wcout << message << endl;
        }
    }

    /*
     * Проверяет, может ли животное сбежать из-за низкого счастья
     * Вероятность побега увеличивается при снижении счастья
     */
    bool tryEscape() {
        if (!isAlive) return false;
        if (trueHappiness <= 15) {
            int escapeChance = 25 - trueHappiness;
            if (rand() % 100 < escapeChance) {
                wcout << L"!!! " << name << L" (ID:" << id << L") сбежал из-за низкого уровня счастья (" << trueHappiness << L"%)!" << endl;
                isAlive = false;
                return true;
            }
        }
        return false;
    }

    /*
     * Проверяет, может ли животное умереть от старости
     * Вероятность смерти увеличивается с возрастом
     */
    bool checkAge() {
        if (age > maxAge) {
            int deathChance = min(100, age - maxAge);
            if (rand() % 100 < deathChance) {
                wcout << name << L" (ID:" << id << L") умер от старости в возрасте " << age << L" дней." << endl;
                isAlive = false;
                return true;
            }
        }
        return false;
    }

     * Проверяет, может ли животное размножаться
     * Животное должно быть живым, не слишком молодым/старым и здоровым
     */
    bool canReproduce() const {
        return isAlive && age > 5 && age < maxAge - 2 && health == AnimalHealth::HEALTHY;
    }

    // Оператор сравнения животных по ID
    bool operator==(const Animal& other) const { return id == other.id; }

    /*
     * Оператор размножения животных
     * Возвращает вектор потомков, если размножение возможно
     */
    vector<unique_ptr<Animal>> operator+(Animal& other) {
        vector<unique_ptr<Animal>> offspring;

        // Проверка возможности размножения
        if (!this->canReproduce() || !other.canReproduce()) {
            wcout << L"Одно из животных не может размножаться (слишком молодое/старое, мертвое или больное)!" << endl;
            return offspring;
        }

        // Проверка разнополости
        if (this->gender == other.gender) {
            wcout << L"Однополые животные не могут размножаться!" << endl;
            return offspring;
        }

        // Определение отца и матери
        Animal* father = (this->gender == 'M') ? this : &other;
        Animal* mother = (this->gender == 'F') ? this : &other;

        // Определение количества потомков в зависимости от вида
        int offspringCount = 1;
        int chance = rand() % 100;

        if (species == L"Носорог" || species == L"Слон" || species == L"Жираф") {
            offspringCount = 1;
        }
        else if (species == L"Лев" || species == L"Тигр") {
            if (chance < 40) offspringCount = 1;
            else if (chance < 80) offspringCount = 2;
            else offspringCount = 3;
        }
        else {
            if (chance < 60) offspringCount = 1;
            else if (chance < 90) offspringCount = 2;
            else offspringCount = 3;
        }

        // Создание потомков
        for (int i = 0; i < offspringCount; i++) {
            wstring babyName = L"Детеныш " + species;

            // Особые имена для детенышей некоторых видов
            if (species == L"Носорог") babyName = L"Носорожок";
            else if (species == L"Слон") babyName = L"Слонёнок";
            else if (species == L"Жираф") babyName = L"Жирафёнок";
            else if (species == L"Лев") babyName = L"Львёнок";
            else if (species == L"Тигр") babyName = L"Тигрёнок";
            else if (species == L"Дельфин") babyName = L"Дельфинёнок";
            else if (species == L"Акула") babyName = L"Акулёнок";

            // Расчет параметров потомка
            int babyWeight = (father->weight + mother->weight) / (10 + rand() % 5);
            int babyPrice = (father->price + mother->price) / (4 + rand() % 3);
            wchar_t babyGender = (rand() % 2) ? 'M' : 'F';

            // Создание нового животного-потомка
            offspring.push_back(make_unique<Animal>(
                0, babyName, species, 0, babyWeight, mother->climate,
                mother->isPredator, babyPrice, babyGender, -1, father->id, mother->id
            ));
        }

        wcout << L"Родилось " << offspringCount << L" "
            << (offspringCount == 1 ? L"детёныш" : L"детёнышей") << L" " << species << L"!" << endl;
        return offspring;
    }

    /*
     * Проверяет здоровье животного
     * Может вызвать болезнь, выздоровление или смерть
     */
    void checkDisease() {
        if (!isAlive || health == AnimalHealth::DEAD) return;

        if (health == AnimalHealth::HEALTHY) {
            // 5% шанс заболеть
            if (rand() % 100 < 5) {
                health = AnimalHealth::SICK;
                hasDisease = true;
                daysSick = 1;
                wcout << name << L" (ID:" << id << L") заболел!" << endl;
            }
        }
        else if (health == AnimalHealth::SICK) {
            daysSick++;
            // После 5 дней болезни - 30% шанс умереть
            if (daysSick > 5 && rand() % 100 < 30) {
                health = AnimalHealth::DEAD;
                isAlive = false;
                wcout << name << L" (ID:" << id << L") умер от болезни!" << endl;
            }
            // После 3 дней болезни - 20% шанс выздороветь
            else if (daysSick > 3 && rand() % 100 < 20) {
                health = AnimalHealth::HEALTHY;
                hasDisease = false;
                wcout << name << L" (ID:" << id << L") выздоровел!" << endl;
            }
        }
    }
```

```bash
* Обновляет состояние чистоты вольера
     * С шансом 1/3 вольер может стать грязным
     */
    void updateCleanliness() {
        if (!containedAnimals.empty() && rand() % 3 == 0) {
            isClean = false;
            wcout << L"Вольер ID:" << id << L" (" << getTypeName() << L") стал грязным." << endl;
 * Возвращает животных определенного пола
     * Используется для размножения
     */
    vector<Animal*> getAnimalsByGender(wchar_t gender) {
        vector<Animal*> result;
        for (auto& animal : containedAnimals) {
            if (animal->getGender() == gender && animal->getIsAlive()) {
                result.push_back(animal);
            }
        }
        return result;
    }
* Распространяет болезнь среди животных в вольере
     * Если есть хотя бы одно больное животное, другие могут заболеть с шансом 20%
     */
    void spreadDisease() {
        if (containedAnimals.empty()) return;

        bool hasSickAnimal = false;
        for (auto animal : containedAnimals) {
            if (animal->getHealth() == AnimalHealth::SICK) {
                hasSickAnimal = true;
                break;
            }
        }

        if (hasSickAnimal) {
            for (auto animal : containedAnimals) {
                if (animal->getHealth() == AnimalHealth::HEALTHY && rand() % 100 < 20) {
                    animal->checkDisease();
                }
            }
        }
    }
```

```bash
  * Чистит вольеры
     * Может почистить до capacity вольеров за день
     */
    void cleanEnclosures(vector<unique_ptr<Enclosure>>& enclosures) {
        int cleaned = 0;
        for (auto& enclosure : enclosures) {
            if (cleaned >= capacity) break;
            if (!enclosure->isClean) {
                enclosure->clean();
                cleaned++;
            }
        }
        if (cleaned > 0) {
            wcout << name << L" почистил " << cleaned << L" вольеров." << endl;
        }
    }

    /*
     * Лечит животных
     * Может вылечить до capacity животных за день
     */
    void treatAnimals(vector<unique_ptr<Animal>>& animals) {
        int treated = 0;
        for (auto& animal : animals) {
            if (treated >= capacity) break;
            if (animal->getHealth() == AnimalHealth::SICK) {
                animal->checkDisease();
                treated++;
            }
        }
        if (treated > 0) {
            wcout << name << L" вылечил " << treated << L" животных." << endl;
        }
    }
};
```
``` bash
// Обновляет карты для быстрого поиска
    void updateMaps() {
        animalsMap.clear();
        for (auto& animal : animals) {
            animalsMap[animal->id] = animal.get();
        }
        enclosuresMap.clear();
        for (auto& enclosure : enclosures) {
            enclosuresMap[enclosure->id] = enclosure.get();
        }
        workersMap.clear();
        for (auto& worker : workers) {
            workersMap[worker->id] = worker.get();
        }
    }


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
