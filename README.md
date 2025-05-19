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
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <locale.h>
#include <limits>
#include <cctype>
#include <map>
#include <algorithm>
#include <iomanip>
#include <memory>
#include <unordered_map>
#include <set>

using namespace std;

// Предварительные объявления классов
class Zoo;
class Animal;
class Enclosure;
class Worker;

/**
 * @brief Перечисление состояний здоровья животного
 */
enum class AnimalHealth { HEALTHY, SICK, DEAD };

/**
 * @brief Перечисление типов животных
 */
enum class AnimalType { LAND, AQUATIC, BIRD, REPTILE };

/**
 * @brief Перечисление типов вольеров
 */
enum EnclosureType {
    ENCLOSURE_SMALL, ENCLOSURE_MEDIUM, ENCLOSURE_LARGE, ENCLOSURE_HUGE,
    ENCLOSURE_AQUARIUM, ENCLOSURE_BIRD_CAGE, ENCLOSURE_REPTILE_HOUSE, ENCLOSURE_PETTING_ZOO
};

/**
 * @brief Перечисление типов работников зоопарка
 */
enum WorkerType {
    WORKER_VET, WORKER_CLEANER, WORKER_FEEDER, WORKER_TRAINER, WORKER_GUIDE
};

// Константные данные для игры
const vector<wstring> healthStatus = { L"Здоров", L"Болен", L"Мертв" };
const vector<wstring> animalTypes = { L"Наземное", L"Водное", L"Птица", L"Рептилия" };
const vector<wstring> climates = { L"Тропический", L"Умеренный", L"Полярный", L"Пустынный", L"Водный" };
const vector<wstring> animalSpecies = {
    L"Лев", L"Тигр", L"Медведь", L"Волк", L"Пингвин",
    L"Крокодил", L"Слон", L"Носорог", L"Жираф", L"Зебра",
    L"Дельфин", L"Акула", L"Черепаха", L"Осьминог"
};

// Карты соответствий видов животных и их характеристик
const map<wstring, vector<wstring>> speciesClimates = {
    {L"Лев", {L"Тропический", L"Пустынный"}},
    {L"Тигр", {L"Тропический", L"Умеренный"}},
    {L"Медведь", {L"Умеренный", L"Полярный"}},
    {L"Волк", {L"Умеренный", L"Полярный"}},
    {L"Пингвин", {L"Полярный"}},
    {L"Крокодил", {L"Тропический"}},
    {L"Слон", {L"Тропический", L"Пустынный"}},
    {L"Носорог", {L"Тропический", L"Пустынный"}},
    {L"Жираф", {L"Тропический", L"Пустынный"}},
    {L"Зебра", {L"Тропический", L"Пустынный"}},
    {L"Дельфин", {L"Водный"}},
    {L"Акула", {L"Водный"}},
    {L"Черепаха", {L"Водный", L"Тропический"}},
    {L"Осьминог", {L"Водный"}}
};

const map<wstring, AnimalType> speciesTypes = {
    {L"Лев", AnimalType::LAND}, {L"Тигр", AnimalType::LAND},
    {L"Медведь", AnimalType::LAND}, {L"Волк", AnimalType::LAND},
    {L"Пингвин", AnimalType::BIRD}, {L"Крокодил", AnimalType::REPTILE},
    {L"Слон", AnimalType::LAND}, {L"Носорог", AnimalType::LAND},
    {L"Жираф", AnimalType::LAND}, {L"Зебра", AnimalType::LAND},
    {L"Дельфин", AnimalType::AQUATIC}, {L"Акула", AnimalType::AQUATIC},
    {L"Черепаха", AnimalType::REPTILE}, {L"Осьминог", AnimalType::AQUATIC}
};

const map<WorkerType, wstring> workerTypeNames = {
    {WORKER_VET, L"Ветеринар"}, {WORKER_CLEANER, L"Уборщик"},
    {WORKER_FEEDER, L"Кормильщик"}, {WORKER_TRAINER, L"Дрессировщик"},
    {WORKER_GUIDE, L"Экскурсовод"}
};

const map<WorkerType, int> workerBaseSalaries = {
    {WORKER_VET, 40000}, {WORKER_CLEANER, 20000},
    {WORKER_FEEDER, 25000}, {WORKER_TRAINER, 35000},
    {WORKER_GUIDE, 30000}
};

const map<wstring, int> speciesMaxAge = {
    {L"Лев", 15 + rand() % 10}, {L"Тигр", 14 + rand() % 8},
    {L"Медведь", 20 + rand() % 15}, {L"Волк", 8 + rand() % 5},
    {L"Пингвин", 10 + rand() % 7}, {L"Крокодил", 30 + rand() % 20},
    {L"Слон", 40 + rand() % 20}, {L"Носорог", 35 + rand() % 15},
    {L"Жираф", 25 + rand() % 10}, {L"Зебра", 20 + rand() % 10},
    {L"Дельфин", 30 + rand() % 15}, {L"Акула", 40 + rand() % 20},
    {L"Черепаха", 50 + rand() % 30}, {L"Осьминог", 5 + rand() % 3}
};

const map<int, tuple<EnclosureType, int, int, wstring>> enclosureTypes = {
    {1, {ENCLOSURE_SMALL, 5, 5000, L"Малый"}},
    {2, {ENCLOSURE_MEDIUM, 10, 8000, L"Средний"}},
    {3, {ENCLOSURE_LARGE, 15, 12000, L"Большой"}},
    {4, {ENCLOSURE_HUGE, 20, 15000, L"Огромный"}},
    {5, {ENCLOSURE_AQUARIUM, 3, 10000, L"Аквариум"}},
    {6, {ENCLOSURE_BIRD_CAGE, 8, 7000, L"Птичья клетка"}},
    {7, {ENCLOSURE_REPTILE_HOUSE, 5, 9000, L"Террариум"}},
    {8, {ENCLOSURE_PETTING_ZOO, 10, 6000, L"Контактный зоопарк"}}
};

/**
 * @brief Генерирует имя животного с учетом пола
 * @param species Вид животного
 * @param gender Пол животного ('M' или 'F')
 * @return Имя животного с учетом пола
 */
wstring generateAnimalName(const wstring& species, wchar_t gender) {
    if (gender == 'F') {
        if (species == L"Лев") return L"Львица";
        if (species == L"Тигр") return L"Тигрица";
        if (species == L"Медведь") return L"Медведица";
        if (species == L"Волк") return L"Волчица";
    }
    return species;
}

/**
 * @brief Проверяет, является ли строка числом
 * @param str Входная строка
 * @return true, если строка состоит только из цифр, иначе false
 */
bool isNumber(const wstring& str) {
    return !str.empty() && all_of(str.begin(), str.end(), [](wchar_t ch) { return iswdigit(ch); });
}

/**
 * @brief Безопасный ввод строки с проверкой на пустоту
 * @param prompt Приглашение для ввода
 * @return Введенная пользователем строка
 */
wstring safeInputString(const wstring& prompt) {
    wstring input;
    while (true) {
        wcout << prompt;
        getline(wcin, input);
        if (!input.empty()) return input;
        wcout << L"Пожалуйста, введите непустую строку." << endl;
    }
}

/**
 * @brief Безопасный ввод целого числа
 * @param prompt Приглашение для ввода
 * @return Введенное пользователем число
 */
int safeInputInt(const wstring& prompt) {
    wstring input;
    while (true) {
        wcout << prompt;
        getline(wcin, input);
        if (isNumber(input)) return stoi(input);
        wcout << L"Пожалуйста, введите число: ";
    }
}

/**
 * @brief Класс, представляющий животное в зоопарке
 */
class Animal {
public:
    int id;                     ///< Уникальный идентификатор животного
    wstring name;               ///< Имя животного
    wstring species;            ///< Вид животного
    int age;                    ///< Возраст в днях
    int weight;                 ///< Вес в кг
    wstring climate;            ///< Предпочитаемый климат
    bool isPredator;            ///< Является ли хищником
    int price;                  ///< Цена животного
    time_t lastUpdateTime;      ///< Время последнего обновления состояния
    wchar_t gender;             ///< Пол ('M' или 'F')
    bool isAlive;               ///< Живое ли животное
    int trueHappiness;          ///< Реальный уровень счастья (0-100)
    int displayedHappiness;     ///< Отображаемый уровень счастья
    int maxAge;                 ///< Максимальный возраст для этого вида
    int parentId1;              ///< ID первого родителя (-1 если нет)
    int parentId2;              ///< ID второго родителя (-1 если нет)
    AnimalHealth health;        ///< Состояние здоровья
    AnimalType type;            ///< Тип животного
    int daysSick;               ///< Количество дней болезни
    bool hasDisease;            ///< Есть ли заболевание

    /**
     * @brief Конструктор животного
     * @param _id Уникальный идентификатор
     * @param n Имя животного
     * @param s Вид животного
     * @param a Возраст в днях
     * @param w Вес в кг
     * @param c Предпочитаемый климат
     * @param pred Является ли хищником
     * @param p Цена животного
     * @param g Пол ('M' или 'F')
     * @param maxA Максимальный возраст (-1 для автоматического расчета)
     * @param p1 ID первого родителя
     * @param p2 ID второго родителя
     */
    Animal(int _id, wstring n, wstring s, int a, int w, wstring c, bool pred, int p, wchar_t g, int maxA = -1, int p1 = -1, int p2 = -1)
        : id(_id), name(move(n)), species(move(s)), age(a), weight(w), climate(move(c)), isPredator(pred), price(p),
        trueHappiness(70 + rand() % 31), displayedHappiness(trueHappiness), lastUpdateTime(time(0)),
        gender(g), isAlive(true), maxAge(maxA), parentId1(p1), parentId2(p2), health(AnimalHealth::HEALTHY),
        type(speciesTypes.at(species)), daysSick(0), hasDisease(false) {
        name = generateAnimalName(species, gender);
        if (maxAge == -1) {
            auto it = speciesMaxAge.find(species);
            maxAge = (it != speciesMaxAge.end()) ? it->second : (10 + rand() % 10);
        }
        if (age < 3) {
            trueHappiness = min(100, trueHappiness + 15);
            displayedHappiness = trueHappiness;
        }
    }

    /**
     * @brief Обновляет отображаемый уровень счастья
     */
    void updateDisplayedHappiness() { displayedHappiness = trueHappiness; }

    /**
     * @brief Возвращает цвет для отображения уровня счастья
     * @return ANSI-код цвета
     */
    wstring getHappinessColor() const {
        if (!isAlive) return L"\033[90m";
        if (displayedHappiness >= 80) return L"\033[32m";
        if (displayedHappiness >= 60) return L"\033[33m";
        if (displayedHappiness >= 40) return L"\033[93m";
        if (displayedHappiness >= 20) return L"\033[31m";
        return L"\033[91m";
    }

    /**
     * @brief Обновляет уровень счастья животного
     * @param isFed Было ли животное накормлено
     * @param isAlone Находится ли животное одно в вольере
     * @param isClean Чистый ли вольер
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

    /**
     * @brief Попытка побега животного из-за низкого уровня счастья
     * @return true, если животное сбежало, иначе false
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

    /**
     * @brief Проверяет, не умерло ли животное от старости
     * @return true, если животное умерло, иначе false
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

    /**
     * @brief Переименовывает животное
     * @param newName Новое имя животного
     */
    void rename(const wstring& newName) {
        name = newName;
        wcout << L"Животное ID:" << id << L" теперь зовут " << name << endl;
    }

    /**
     * @brief Проверяет, может ли животное размножаться
     * @return true, если животное может размножаться, иначе false
     */
    bool canReproduce() const {
        return isAlive && age > 5 && age < maxAge - 2 && health == AnimalHealth::HEALTHY;
    }

    /**
     * @brief Оператор сравнения животных по ID
     */
    bool operator==(const Animal& other) const { return id == other.id; }

    /**
     * @brief Оператор размножения животных
     * @param other Другое животное для размножения
     * @return Вектор потомков (может быть пустым)
     */
    vector<unique_ptr<Animal>> operator+(Animal& other) {
        vector<unique_ptr<Animal>> offspring;

        if (!this->canReproduce() || !other.canReproduce()) {
            wcout << L"Одно из животных не может размножаться (слишком молодое/старое, мертвое или больное)!" << endl;
            return offspring;
        }

        if (this->gender == other.gender) {
            wcout << L"Однополые животные не могут размножаться!" << endl;
            return offspring;
        }

        Animal* father = (this->gender == 'M') ? this : &other;
        Animal* mother = (this->gender == 'F') ? this : &other;

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

        for (int i = 0; i < offspringCount; i++) {
            wstring babyName = L"Детеныш " + species;

            if (species == L"Носорог") babyName = L"Носорожок";
            else if (species == L"Слон") babyName = L"Слонёнок";
            else if (species == L"Жираф") babyName = L"Жирафёнок";
            else if (species == L"Лев") babyName = L"Львёнок";
            else if (species == L"Тигр") babyName = L"Тигрёнок";
            else if (species == L"Дельфин") babyName = L"Дельфинёнок";
            else if (species == L"Акула") babyName = L"Акулёнок";

            int babyWeight = (father->weight + mother->weight) / (10 + rand() % 5);
            int babyPrice = (father->price + mother->price) / (4 + rand() % 3);
            wchar_t babyGender = (rand() % 2) ? 'M' : 'F';

            offspring.push_back(make_unique<Animal>(
                0, babyName, species, 0, babyWeight, mother->climate,
                mother->isPredator, babyPrice, babyGender, -1, father->id, mother->id
            ));
        }

        wcout << L"Родилось " << offspringCount << L" "
            << (offspringCount == 1 ? L"детёныш" : L"детёнышей") << L" " << species << L"!" << endl;
        return offspring;
    }

    /**
     * @brief Проверяет и обновляет состояние здоровья животного
     */
    void checkDisease() {
        if (!isAlive || health == AnimalHealth::DEAD) return;

        if (health == AnimalHealth::HEALTHY) {
            if (rand() % 100 < 5) {
                health = AnimalHealth::SICK;
                hasDisease = true;
                daysSick = 1;
                wcout << name << L" (ID:" << id << L") заболел!" << endl;
            }
        }
        else if (health == AnimalHealth::SICK) {
            daysSick++;
            if (daysSick > 5 && rand() % 100 < 30) {
                health = AnimalHealth::DEAD;
                isAlive = false;
                wcout << name << L" (ID:" << id << L") умер от болезни!" << endl;
            }
            else if (daysSick > 3 && rand() % 100 < 20) {
                health = AnimalHealth::HEALTHY;
                hasDisease = false;
                wcout << name << L" (ID:" << id << L") выздоровел!" << endl;
            }
        }
    }

    // Геттеры
    int getDisplayedHappiness() const { return displayedHappiness; }
    int getTrueHappiness() const { return trueHappiness; }
    wchar_t getGender() const { return gender; }
    int getParentId1() const { return parentId1; }
    int getParentId2() const { return parentId2; }
    bool getIsAlive() const { return isAlive; }
    wstring getSpecies() const { return species; }
    AnimalHealth getHealth() const { return health; }
    AnimalType getType() const { return type; }
};

/**
 * @brief Класс, представляющий вольер в зоопарке
 */
class Enclosure {
public:
    int id;                     ///< Уникальный идентификатор вольера
    int capacity;               ///< Вместимость вольера
    EnclosureType type;         ///< Тип вольера
    wstring climate;            ///< Климат вольера
    int maintenanceCost;        ///< Стоимость обслуживания
    bool isClean;               ///< Чистый ли вольер
    vector<Animal*> containedAnimals; ///< Животные в вольере

    /**
     * @brief Конструктор вольера
     * @param _id Уникальный идентификатор
     * @param c Вместимость
     * @param t Тип вольера
     * @param cl Климат
     * @param cost Стоимость обслуживания
     */
    Enclosure(int _id, int c, EnclosureType t, wstring cl, int cost)
        : id(_id), capacity(c), type(t), climate(move(cl)), maintenanceCost(cost), isClean(true) {
    }

    /**
     * @brief Возвращает название типа вольера
     * @return Название типа
     */
    wstring getTypeName() const {
        static const map<EnclosureType, wstring> typeNames = {
            {ENCLOSURE_SMALL, L"Малый"}, {ENCLOSURE_MEDIUM, L"Средний"},
            {ENCLOSURE_LARGE, L"Большой"}, {ENCLOSURE_HUGE, L"Огромный"},
            {ENCLOSURE_AQUARIUM, L"Аквариум"}, {ENCLOSURE_BIRD_CAGE, L"Птичья клетка"},
            {ENCLOSURE_REPTILE_HOUSE, L"Террариум"}, {ENCLOSURE_PETTING_ZOO, L"Контактный зоопарк"}
        };
        return typeNames.at(type);
    }

    /**
     * @brief Проверяет, подходит ли тип вольера для данного типа животного
     * @param animalType Тип животного
     * @return true, если подходит, иначе false
     */
    bool isSuitableForAnimalType(AnimalType animalType) const {
        switch (type) {
        case ENCLOSURE_AQUARIUM: return animalType == AnimalType::AQUATIC;
        case ENCLOSURE_BIRD_CAGE: return animalType == AnimalType::BIRD;
        case ENCLOSURE_REPTILE_HOUSE: return animalType == AnimalType::REPTILE;
        default: return animalType == AnimalType::LAND;
        }
    }

    /**
     * @brief Обновляет состояние чистоты вольера
     */
    void updateCleanliness() {
        if (!containedAnimals.empty() && rand() % 3 == 0) {
            isClean = false;
            wcout << L"Вольер ID:" << id << L" (" << getTypeName() << L") стал грязным." << endl;
        }
    }

    /**
     * @brief Добавляет животное в вольер
     * @param animal Указатель на животное
     * @return true, если животное успешно добавлено, иначе false
     */
    bool addAnimal(Animal* animal) {
        if (containedAnimals.size() >= static_cast<size_t>(capacity)) {
            wcout << L"Вольер переполнен!" << endl;
            return false;
        }
        if (animal->climate != climate) {
            wcout << L"Климат вольера не подходит для этого животного!" << endl;
            return false;
        }
        if (!isSuitableForAnimalType(animal->getType())) {
            wcout << L"Тип вольера не подходит для этого животного!" << endl;
            return false;
        }
        containedAnimals.push_back(animal);
        wcout << animal->name << L" (ID:" << animal->id << L") помещен в вольер ID:" << id << endl;
        return true;
    }

    /**
     * @brief Удаляет животное из вольера
     * @param animalId ID животного
     * @return true, если животное найдено и удалено, иначе false
     */
    bool removeAnimal(int animalId) {
        for (auto it = containedAnimals.begin(); it != containedAnimals.end(); ++it) {
            if ((*it)->id == animalId) {
                containedAnimals.erase(it);
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Очищает вольер
     */
    void clean() {
        isClean = true;
        wcout << L"Вольер ID:" << id << L" (" << getTypeName() << L") был почищен." << endl;
    }

    /**
     * @brief Возвращает животных определенного пола
     * @param gender Пол ('M' или 'F')
     * @return Вектор указателей на животных
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

    /**
     * @brief Распространяет болезнь среди животных в вольере
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
};

/**
 * @brief Класс, представляющий работника зоопарка
 */
class Worker {
public:
    int id;             ///< Уникальный идентификатор работника
    wstring name;       ///< Имя работника
    WorkerType type;    ///< Тип работника
    int salary;         ///< Зарплата
    int capacity;       ///< Емкость (сколько задач может выполнить за день)

    /**
     * @brief Конструктор работника
     * @param _id Уникальный идентификатор
     * @param n Имя работника
     * @param t Тип работника
     */
    Worker(int _id, wstring n, WorkerType t)
        : id(_id), name(move(n)), type(t), salary(workerBaseSalaries.at(t)), capacity(0) {
        static const map<WorkerType, int> workerCapacities = {
            {WORKER_VET, 20}, {WORKER_CLEANER, 3},
            {WORKER_FEEDER, 2}, {WORKER_TRAINER, 5},
            {WORKER_GUIDE, 0}
        };
        capacity = workerCapacities.at(type);
        if (type == WORKER_GUIDE) salary += 5000;
    }

    /**
     * @brief Возвращает название типа работника
     * @return Название типа
     */
    wstring getTypeName() const { return workerTypeNames.at(type); }

    /**
     * @brief Выполняет обязанности работника
     * @param zoo Указатель на зоопарк
     */
    void performDuties(Zoo* zoo);

    /**
     * @brief Очищает вольеры
     * @param enclosures Ссылка на вектор вольеров
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

    /**
     * @brief Лечит животных
     * @param animals Ссылка на вектор животных
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

/**
 * @brief Класс, представляющий зоопарк
 */
class Zoo {
private:
    bool testMode;                      ///< Режим тестирования
    vector<unique_ptr<Animal>> marketAnimals; ///< Животные на рынке
    int marketRefreshCost;              ///< Стоимость обновления рынка
    time_t lastMarketRefresh;           ///< Время последнего обновления рынка
    int animalsBoughtToday;             ///< Сколько животных куплено сегодня
    unordered_map<int, Animal*> animalsMap; ///< Карта животных для быстрого поиска
    unordered_map<int, Enclosure*> enclosuresMap; ///< Карта вольеров для быстрого поиска
    unordered_map<int, Worker*> workersMap; ///< Карта работников для быстрого поиска

    /**
     * @brief Обновляет карты для быстрого поиска
     */
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

public:
    wstring name;               ///< Название зоопарка
    int money;                  ///< Деньги зоопарка
    int food;                   ///< Количество еды
    int popularity;             ///< Популярность зоопарка
    vector<unique_ptr<Animal>> animals; ///< Животные в зоопарке
    vector<unique_ptr<Enclosure>> enclosures; ///< Вольеры в зоопарке
    vector<unique_ptr<Worker>> workers; ///< Работники зоопарка
    int visitors;               ///< Количество посетителей
    int days;                   ///< Количество прошедших дней
    int victoryDays;            ///< Количество дней для победы
    int nextAnimalId;           ///< Следующий ID для животного
    int nextEnclosureId;        ///< Следующий ID для вольера
    int nextWorkerId;           ///< Следующий ID для работника
    wstring directorName;       ///< Имя директора

    /**
     * @brief Конструктор зоопарка
     * @param zooName Название зоопарка
     * @param victoryDaysCount Количество дней для победы
     */
    Zoo(wstring zooName, int victoryDaysCount)
        : name(move(zooName)), money(500000), food(100), popularity(10), visitors(20),
        days(0), victoryDays(victoryDaysCount), nextAnimalId(1), nextEnclosureId(1),
        nextWorkerId(1), testMode(false), marketRefreshCost(5000), animalsBoughtToday(0) {
        (void)_setmode(_fileno(stdout), _O_U16TEXT);
        (void)_setmode(_fileno(stdin), _O_U16TEXT);
        setlocale(LC_ALL, "ru_RU.UTF-8");

        directorName = safeInputString(L"Введите имя директора зоопарка: ");
        refreshMarket();
    }

    /**
     * @brief Находит животное по ID
     * @param id ID животного
     * @return Указатель на животное или nullptr, если не найдено
     */
    Animal* findAnimal(int id) {
        auto it = animalsMap.find(id);
        return it != animalsMap.end() ? it->second : nullptr;
    }

    /**
     * @brief Находит вольер по ID
     * @param id ID вольера
     * @return Указатель на вольер или nullptr, если не найдено
     */
    Enclosure* findEnclosure(int id) {
        auto it = enclosuresMap.find(id);
        return it != enclosuresMap.end() ? it->second : nullptr;
    }

    /**
     * @brief Находит работника по ID
     * @param id ID работника
     * @return Указатель на работника или nullptr, если не найдено
     */
    Worker* findWorker(int id) {
        auto it = workersMap.find(id);
        return it != workersMap.end() ? it->second : nullptr;
   



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
