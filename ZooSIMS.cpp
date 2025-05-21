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

// Классы для представления объектов зоопарка

class Zoo;
class Animal;
class Enclosure;
class Worker;

// Перечисления для статусов здоровья животных, их типов, типов вольеров и работников

enum class AnimalHealth { HEALTHY, SICK, DEAD };
enum class AnimalType { LAND, AQUATIC, BIRD, REPTILE };

enum EnclosureType {
    ENCLOSURE_SMALL, ENCLOSURE_MEDIUM, ENCLOSURE_LARGE, ENCLOSURE_HUGE,
    ENCLOSURE_AQUARIUM, ENCLOSURE_BIRD_CAGE, ENCLOSURE_REPTILE_HOUSE, ENCLOSURE_PETTING_ZOO
};

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

// Сопоставление видов животных с подходящими климатами

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

// Сопоставление видов животных с их типами

const map<wstring, AnimalType> speciesTypes = {
    {L"Лев", AnimalType::LAND}, {L"Тигр", AnimalType::LAND},
    {L"Медведь", AnimalType::LAND}, {L"Волк", AnimalType::LAND},
    {L"Пингвин", AnimalType::BIRD}, {L"Крокодил", AnimalType::REPTILE},
    {L"Слон", AnimalType::LAND}, {L"Носорог", AnimalType::LAND},
    {L"Жираф", AnimalType::LAND}, {L"Зебра", AnimalType::LAND},
    {L"Дельфин", AnimalType::AQUATIC}, {L"Акула", AnimalType::AQUATIC},
    {L"Черепаха", AnimalType::REPTILE}, {L"Осьминог", AnimalType::AQUATIC}
};

// Названия и зарплаты работников

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

// Максимальный возраст для каждого вида животных

const map<wstring, int> speciesMaxAge = {
    {L"Лев", 15 + rand() % 10}, {L"Тигр", 14 + rand() % 8},
    {L"Медведь", 20 + rand() % 15}, {L"Волк", 8 + rand() % 5},
    {L"Пингвин", 10 + rand() % 7}, {L"Крокодил", 30 + rand() % 20},
    {L"Слон", 40 + rand() % 20}, {L"Носорог", 35 + rand() % 15},
    {L"Жираф", 25 + rand() % 10}, {L"Зебра", 20 + rand() % 10},
    {L"Дельфин", 30 + rand() % 15}, {L"Акула", 40 + rand() % 20},
    {L"Черепаха", 50 + rand() % 30}, {L"Осьминог", 5 + rand() % 3}
};

// Типы вольеров с их характеристиками

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

// Генерация имени животного в зависимости от пола

wstring generateAnimalName(const wstring& species, wchar_t gender) {
    if (gender == 'F') {
        if (species == L"Лев") return L"Львица";
        if (species == L"Тигр") return L"Тигрица";
        if (species == L"Медведь") return L"Медведица";
        if (species == L"Волк") return L"Волчица";
    }
    return species;
}

// Проверка, является ли строка числом

bool isNumber(const wstring& str) {
    return !str.empty() && all_of(str.begin(), str.end(), [](wchar_t ch) { return iswdigit(ch); });
}

// Безопасный ввод строки с проверкой на пустоту

wstring safeInputString(const wstring& prompt) {
    wstring input;
    while (true) {
        wcout << prompt;
        getline(wcin, input);
        if (!input.empty()) return input;
        wcout << L"Пожалуйста, введите непустую строку." << endl;
    }
}

// Безопасный ввод числа

int safeInputInt(const wstring& prompt) {
    wstring input;
    while (true) {
        wcout << prompt;
        getline(wcin, input);
        if (isNumber(input)) return stoi(input);
        wcout << L"Пожалуйста, введите число: ";
    }
}

// Класс Animal: представляет животное в зоопарке

class Animal {
public:
    // Основные атрибуты животного

    int id;
    wstring name;
    wstring species;
    int age;
    int weight;
    wstring climate;
    bool isPredator;
    int price;
    time_t lastUpdateTime;
    wchar_t gender;
    bool isAlive;
    int trueHappiness;
    int displayedHappiness;
    int maxAge;
    int parentId1;
    int parentId2;
    AnimalHealth health;
    AnimalType type;
    int daysSick;
    bool hasDisease;

// Конструктор животного

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

// Обновление отображаемого уровня счастья

    void updateDisplayedHappiness() { displayedHappiness = trueHappiness; }

// Получение цвета для отображения уровня счастья

    wstring getHappinessColor() const {
        if (!isAlive) return L"\033[90m";
        if (displayedHappiness >= 80) return L"\033[32m";
        if (displayedHappiness >= 60) return L"\033[33m";
        if (displayedHappiness >= 40) return L"\033[93m";
        if (displayedHappiness >= 20) return L"\033[31m";
        return L"\033[91m";
    }

// Обновление уровня счастья на основе условий содержания

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

// Попытка побега животного

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

// Проверка возраста животного

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

// Переименование животного

    void rename(const wstring& newName) {
        name = newName;
        wcout << L"Животное ID:" << id << L" теперь зовут " << name << endl;
    }

// Проверка возможности размножения

    bool canReproduce() const {
        return isAlive && age > 5 && age < maxAge - 2 && health == AnimalHealth::HEALTHY;
    }

// Оператор сравнения животных по ID

    bool operator==(const Animal& other) const { return id == other.id; }

// Оператор размножения животных

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

// Проверка болезни животного

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

// Класс Enclosure: представляет вольер в зоопарке

class Enclosure {
public:
    int id;
    int capacity;
    EnclosureType type;
    wstring climate;
    int maintenanceCost;
    bool isClean;
    vector<Animal*> containedAnimals;

// Конструктор вольера
    
    Enclosure(int _id, int c, EnclosureType t, wstring cl, int cost)
        : id(_id), capacity(c), type(t), climate(move(cl)), maintenanceCost(cost), isClean(true) {
    }

// Получение названия типа вольера

    wstring getTypeName() const {
        static const map<EnclosureType, wstring> typeNames = {
            {ENCLOSURE_SMALL, L"Малый"}, {ENCLOSURE_MEDIUM, L"Средний"},
            {ENCLOSURE_LARGE, L"Большой"}, {ENCLOSURE_HUGE, L"Огромный"},
            {ENCLOSURE_AQUARIUM, L"Аквариум"}, {ENCLOSURE_BIRD_CAGE, L"Птичья клетка"},
            {ENCLOSURE_REPTILE_HOUSE, L"Террариум"}, {ENCLOSURE_PETTING_ZOO, L"Контактный зоопарк"}
        };
        return typeNames.at(type);
    }

// Проверка подходит ли вольер для типа животного

    bool isSuitableForAnimalType(AnimalType animalType) const {
        switch (type) {
        case ENCLOSURE_AQUARIUM: return animalType == AnimalType::AQUATIC;
        case ENCLOSURE_BIRD_CAGE: return animalType == AnimalType::BIRD;
        case ENCLOSURE_REPTILE_HOUSE: return animalType == AnimalType::REPTILE;
        default: return animalType == AnimalType::LAND;
        }
    }

// Обновление состояния чистоты вольера

    void updateCleanliness() {
        if (!containedAnimals.empty() && rand() % 3 == 0) {
            isClean = false;
            wcout << L"Вольер ID:" << id << L" (" << getTypeName() << L") стал грязным." << endl;
        }
    }

// Добавление животного в вольер

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

// Удаление животного из вольера

    bool removeAnimal(int animalId) {
        for (auto it = containedAnimals.begin(); it != containedAnimals.end(); ++it) {
            if ((*it)->id == animalId) {
                containedAnimals.erase(it);
                return true;
            }
        }
        return false;
    }

// Очистка вольера

    void clean() {
        isClean = true;
        wcout << L"Вольер ID:" << id << L" (" << getTypeName() << L") был почищен." << endl;
    }

// Получение животных определенного пола

    vector<Animal*> getAnimalsByGender(wchar_t gender) {
        vector<Animal*> result;
        for (auto& animal : containedAnimals) {
            if (animal->getGender() == gender && animal->getIsAlive()) {
                result.push_back(animal);
            }
        }
        return result;
    }

// Распространение болезни среди животных в вольере

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

// Класс Worker: представляет работника зоопарка

class Worker {
public:
    int id;
    wstring name;
    WorkerType type;
    int salary;
    int capacity;

// Конструктор работника

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

// Получение названия типа работника

    wstring getTypeName() const { return workerTypeNames.at(type); }

// Выполнение обязанностей работника

    void performDuties(Zoo* zoo);

    // Очистка вольеров

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

// Лечение животных

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

// Класс Zoo: основной класс, управляющий зоопарком

class Zoo {
private:
    bool testMode;
    vector<unique_ptr<Animal>> marketAnimals;
    int marketRefreshCost;
    time_t lastMarketRefresh;
    int animalsBoughtToday;
    unordered_map<int, Animal*> animalsMap;
    unordered_map<int, Enclosure*> enclosuresMap;
    unordered_map<int, Worker*> workersMap;

// Обновление карт для быстрого поиска объектов

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
    wstring name;
    int money;
    int food;
    int popularity;
    vector<unique_ptr<Animal>> animals;
    vector<unique_ptr<Enclosure>> enclosures;
    vector<unique_ptr<Worker>> workers;
    int visitors;
    int days;
    int victoryDays;
    int nextAnimalId;
    int nextEnclosureId;
    int nextWorkerId;
    wstring directorName;

// Конструктор зоопарка

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

// Поиск животного по ID

    Animal* findAnimal(int id) {
        auto it = animalsMap.find(id);
        return it != animalsMap.end() ? it->second : nullptr;
    }

// Поиск вольера по ID

    Enclosure* findEnclosure(int id) {
        auto it = enclosuresMap.find(id);
        return it != enclosuresMap.end() ? it->second : nullptr;
    }

// Поиск работника по ID

    Worker* findWorker(int id) {
        auto it = workersMap.find(id);
        return it != workersMap.end() ? it->second : nullptr;
    }

// Обновление рынка животных

    void refreshMarket() {
        marketAnimals.clear();
        int count = min(5 + rand() % 6, 10);

        for (int i = 0; i < count; i++) {
            wstring species = animalSpecies[rand() % animalSpecies.size()];
            auto climateIt = speciesClimates.find(species);
            if (climateIt == speciesClimates.end()) continue;

            const auto& possibleClimates = climateIt->second;
            wstring climate = possibleClimates[rand() % possibleClimates.size()];

            bool isPredator = (species == L"Лев" || species == L"Тигр" ||
                species == L"Медведь" || species == L"Волк" ||
                species == L"Крокодил" || species == L"Акула");

            wchar_t gender = (rand() % 2) ? 'M' : 'F';
            int age = rand() % 5;
            int weight = 1 + rand() % 200;
            int price = 5000 + rand() % 45000;

            marketAnimals.push_back(make_unique<Animal>(
                nextAnimalId++,
                generateAnimalName(species, gender),
                species,
                age,
                weight,
                climate,
                isPredator,
                price,
                gender
            ));
        }
        lastMarketRefresh = time(0);
        wcout << L"Рынок животных обновлен! Доступно " << marketAnimals.size() << L" животных." << endl;
    }

// Отображение главного меню

    void showMainMenu() {
        wcout << L"\n=== " << name << L" ===" << endl;
        wcout << L"Директор: " << directorName << endl;
        wcout << L"День: " << days << L" из " << victoryDays << endl;
        wcout << L"Баланс: " << money << L" руб." << endl;
        wcout << L"Еда: " << food << L" ед." << endl;
        wcout << L"Популярность: " << popularity << endl;
        wcout << L"Посетители сегодня: " << visitors << L" чел." << endl;
        wcout << L"Животные: " << animals.size() << L" шт." << endl;
        wcout << L"Вольеры: " << enclosures.size() << L" шт." << endl;
        wcout << L"Работники: " << workers.size() << L" чел." << endl;

        wcout << L"\nГлавное меню:" << endl;
        wcout << L"1. Животные" << endl;
        wcout << L"2. Вольеры" << endl;
        wcout << L"3. Работники" << endl;
        wcout << L"4. Купить еду" << endl;
        wcout << L"5. Заказать рекламу" << endl;
        wcout << L"6. Следующий день" << endl;
        wcout << L"7. Показать счастье животных" << endl;
        wcout << L"9. Выход" << endl;
    }

// Отображение меню животных

    void showAnimalsMenu() {
        wcout << L"\n=== Животные ===" << endl;
        wcout << L"1. Купить животное с рынка" << endl;
        wcout << L"2. Продать животное" << endl;
        wcout << L"3. Просмотреть животных" << endl;
        wcout << L"4. Обновить рынок животных (" << marketRefreshCost << L" руб.)" << endl;
        wcout << L"5. Переименовать животное" << endl;
        wcout << L"6. Попробовать размножить животных" << endl;
        wcout << L"0. Назад" << endl;
    }

// Отображение меню вольеров

    void showEnclosuresMenu() {
        wcout << L"\n=== Вольеры ===" << endl;
        wcout << L"1. Купить вольер" << endl;
        wcout << L"2. Просмотреть вольеры" << endl;
        wcout << L"0. Назад" << endl;
    }

// Отображение меню работников

    void showWorkersMenu() {
        wcout << L"\n=== Работники ===" << endl;
        wcout << L"1. Нанять работника" << endl;
        wcout << L"2. Просмотреть работников" << endl;
        wcout << L"0. Назад" << endl;
    }

// Отображение животных на рынке

    void showMarketAnimals() {
        wcout << L"\n=== Животные на рынке ===" << endl;
        if (marketAnimals.empty()) {
            wcout << L"На рынке нет животных!" << endl;
            return;
        }
        for (size_t i = 0; i < marketAnimals.size(); i++) {
            const auto& animal = marketAnimals[i];
            wcout << (i + 1) << L". " << animal->name << L" (" << (animal->gender == 'M' ? L"Самец" : L"Самка") << L")"
                << L" | Вид: " << animal->species << L" | Возраст: " << animal->age << L" дней"
                << L" | Вес: " << animal->weight << L" кг" << L" | Климат: " << animal->climate
                << L" | Тип: " << animalTypes[static_cast<int>(animal->getType())]
                << L" | " << (animal->isPredator ? L"Хищник" : L"Травоядное")
                << L" | Цена: " << animal->price << L" руб." << endl;
        }
    }

// Покупка животного с рынка

    void buyAnimal() {
        if (days > 10 && animalsBoughtToday >= 1) {
            wcout << L"После 10 дня можно покупать только 1 животное в день!" << endl;
            return;
        }
        showMarketAnimals();
        if (marketAnimals.empty()) return;

        int choice = safeInputInt(L"Выберите животное для покупки (1-" + to_wstring(marketAnimals.size()) + L" или 0 для отмены): ");
        if (choice < 1 || choice > static_cast<int>(marketAnimals.size())) return;

        auto& animalToBuy = marketAnimals[choice - 1];
        if (money < animalToBuy->price) {
            wcout << L"Недостаточно денег для покупки!" << endl;
            return;
        }

        bool foundEnclosure = false;
        for (auto& enclosure : enclosures) {
            if (enclosure->containedAnimals.size() >= static_cast<size_t>(enclosure->capacity)) {
                continue;
            }
            if (enclosure->climate != animalToBuy->climate) {
                continue;
            }
            if (!enclosure->isSuitableForAnimalType(animalToBuy->getType())) {
                continue;
            }
            auto newAnimal = make_unique<Animal>(*animalToBuy);
            newAnimal->id = nextAnimalId++;
            animals.push_back(move(newAnimal));
            enclosure->addAnimal(animals.back().get());
            foundEnclosure = true;
            animalsBoughtToday++;
            money -= animalToBuy->price;
            wcout << L"Вы купили " << animalToBuy->name << L" за " << animalToBuy->price << L" руб." << endl;
            break;
        }

        if (!foundEnclosure) {
            bool hasAquarium = false;
            bool hasMatchingClimate = false;
            bool hasCapacity = false;

            for (auto& enclosure : enclosures) {
                if (enclosure->type == ENCLOSURE_AQUARIUM) {
                    hasAquarium = true;
                    if (enclosure->climate == animalToBuy->climate) {
                        hasMatchingClimate = true;
                    }
                    if (enclosure->containedAnimals.size() < static_cast<size_t>(enclosure->capacity)) {
                        hasCapacity = true;
                    }
                }
            }

            if (!hasAquarium && animalToBuy->getType() == AnimalType::AQUATIC) {
                wcout << L"Нет вольера типа 'Аквариум' для водного животного!" << endl;
            }
            else if (!hasMatchingClimate) {
                wcout << L"Нет вольера с подходящим климатом (" << animalToBuy->climate << L")!" << endl;
            }
            else if (!hasCapacity) {
                wcout << L"Все подходящие вольеры переполнены!" << endl;
            }
            else {
                wcout << L"Нет подходящего вольера для этого животного!" << endl;
            }
            return;
        }

        updateMaps();
    }

    void sellAnimal() {
        if (animals.empty()) {
            wcout << L"Нет животных для продажи!" << endl;
            return;
        }
        wcout << L"\n=== Продажа животного ===" << endl;
        wcout << L"Список животных:" << endl;
        for (const auto& animal : animals) {
            if (animal->getIsAlive()) {
                wcout << L"ID:" << animal->id << L" | " << animal->name << L" | Цена: " << static_cast<int>(animal->price * 0.7) << L" руб." << endl;
            }
        }

        int id = safeInputInt(L"Введите ID животного для продажи (0 для отмены): ");
        if (id == 0) return;

        auto it = find_if(animals.begin(), animals.end(), [id](const unique_ptr<Animal>& a) { return a->id == id && a->getIsAlive(); });
        if (it == animals.end()) {
            wcout << L"Животное с таким ID не найдено или уже мертво!" << endl;
            return;
        }

        for (auto& enclosure : enclosures) {
            enclosure->removeAnimal(id);
        }

        int sellPrice = static_cast<int>((*it)->price * 0.7);
        money += sellPrice;
        animals.erase(it);
        wcout << L"Вы продали животное за " << sellPrice << L" руб." << endl;
        updateMaps();
    }

    void showAnimals() {
        wcout << L"\n=== Список животных ===" << endl;
        if (animals.empty()) {
            wcout << L"Животных нет" << endl;
            return;
        }
        for (const auto& animal : animals) {
            wstring parentsInfo;
            if (animal->getParentId1() != -1 && animal->getParentId2() != -1) {
                parentsInfo = L" | Родители: " + to_wstring(animal->getParentId1()) + L" и " + to_wstring(animal->getParentId2());
            }
            wcout << L"ID:" << animal->id << L" | " << animal->name
                << L" (" << (animal->gender == 'M' ? L"Самец" : L"Самка") << L")"
                << L" | Вид: " << animal->species << L" | Возраст: " << animal->age << L" дней"
                << L" | Вес: " << animal->weight << L" кг" << L" | Климат: " << animal->climate
                << L" | Тип: " << animalTypes[static_cast<int>(animal->getType())]
                << L" | " << (animal->isPredator ? L"Хищник" : L"Травоядное")
                << L" | Здоровье: " << healthStatus[static_cast<int>(animal->getHealth())]
                << L" | Цена: " << animal->price << L" руб." << parentsInfo << endl;
        }
    }

    void renameAnimal() {
        if (animals.empty()) {
            wcout << L"Нет животных для переименования!" << endl;
            return;
        }
        showAnimals();
        int id = safeInputInt(L"Введите ID животного для переименования (0 для отмены): ");
        if (id == 0) return;

        auto animal = findAnimal(id);
        if (!animal) {
            wcout << L"Животное с таким ID не найдено!" << endl;
            return;
        }

        wcout << L"Введите новое имя для животного: ";
        wstring newName;
        getline(wcin, newName);
        animal->rename(newName);
    }

    void tryBreedAnimals() {
        if (animals.size() < 2) {
            wcout << L"Нужно как минимум 2 животных для размножения!" << endl;
            return;
        }
        showAnimals();
        int id1 = safeInputInt(L"Введите ID первого животного: ");
        int id2 = safeInputInt(L"Введите ID второго животного: ");

        auto animal1 = findAnimal(id1);
        auto animal2 = findAnimal(id2);
        if (!animal1 || !animal2 || !animal1->getIsAlive() || !animal2->getIsAlive()) {
            wcout << L"Одно или оба животных не найдены или мертвы!" << endl;
            return;
        }

        Enclosure* parentsEnclosure = nullptr;
        for (auto& enclosure : enclosures) {
            bool found1 = false, found2 = false;
            for (auto animal : enclosure->containedAnimals) {
                if (animal->id == id1) found1 = true;
                if (animal->id == id2) found2 = true;
            }
            if (found1 && found2) {
                parentsEnclosure = enclosure.get();
                break;
            }
        }

        if (!parentsEnclosure) {
            wcout << L"Животные должны быть в одном вольере для размножения!" << endl;
            return;
        }

        if (parentsEnclosure->containedAnimals.size() >= static_cast<size_t>(parentsEnclosure->capacity)) {
            wcout << L"В вольере нет места для потомства!" << endl;
            return;
        }

        auto offspring = *animal1 + *animal2;
        if (offspring.empty()) {
            wcout << L"Размножение не удалось!" << endl;
            return;
        }

        for (auto& baby : offspring) {
            baby->id = nextAnimalId++;
            animals.push_back(move(baby));
            if (!parentsEnclosure->addAnimal(animals.back().get())) {
                wcout << L"Не удалось добавить потомка в вольер!" << endl;
            }
        }
        updateMaps();
    }

    void buyEnclosure() {
        wcout << L"\n=== Покупка вольера ===" << endl;
        wcout << L"Типы вольеров:" << endl;
        for (const auto& entry : enclosureTypes) {
            int id = entry.first;
            const auto& data = entry.second;
            wcout << id << L". " << get<3>(data) << L" (" << get<1>(data) << L" животных, " << get<2>(data) << L" руб.)" << endl;
        }

        int typeChoice = safeInputInt(L"Выберите тип вольера (1-8 или 0 для отмены): ");
        if (typeChoice < 1 || typeChoice > 8) return;

        set<wstring> neededClimates;
        for (const auto& animal : animals) {
            neededClimates.insert(animal->climate);
        }

        if (neededClimates.empty()) {
            for (size_t i = 0; i < climates.size(); i++) {
                wcout << (i + 1) << L". " << climates[i] << endl;
            }
        }
        else {
            int index = 1;
            for (const auto& climate : neededClimates) {
                wcout << index++ << L". " << climate << endl;
            }
        }
        int climateChoice = safeInputInt(L"Выберите климат (1-" + to_wstring(neededClimates.empty() ? climates.size() : neededClimates.size()) + L"): ");
        if (climateChoice < 1 || climateChoice > static_cast<int>(neededClimates.empty() ? climates.size() : neededClimates.size())) return;

        wstring selectedClimate = neededClimates.empty() ? climates[climateChoice - 1] : *next(neededClimates.begin(), climateChoice - 1);

        const auto& data = enclosureTypes.at(typeChoice);
        EnclosureType type = get<0>(data);
        int capacity = get<1>(data);
        int price = get<2>(data);

        if (money < price) {
            wcout << L"Недостаточно денег для покупки!" << endl;
            return;
        }

        int newId = nextEnclosureId++;
        enclosures.push_back(make_unique<Enclosure>(newId, capacity, type, selectedClimate, price / 10));
        money -= price;
        wcout << L"Вы купили новый вольер (ID:" << newId << L") за " << price << L" руб." << endl;
        updateMaps();
    }

    void showEnclosures() {
        wcout << L"\n=== Список вольеров ===" << endl;
        if (enclosures.empty()) {
            wcout << L"Вольеров нет" << endl;
            return;
        }
        for (const auto& enclosure : enclosures) {
            wcout << L"ID:" << enclosure->id << L" | " << enclosure->getTypeName()
                << L" | Климат: " << enclosure->climate
                << L" | Животных: " << enclosure->containedAnimals.size() << L"/" << enclosure->capacity
                << L" | " << (enclosure->isClean ? L"Чистый" : L"Грязный") << endl;
        }
    }

    void hireWorker() {
        wcout << L"\n=== Наем работника ===" << endl;
        wcout << L"Типы работников:" << endl;
        for (const auto& entry : workerTypeNames) {
            WorkerType type = entry.first;
            wcout << static_cast<int>(type) + 1 << L". " << entry.second << L" (" << workerBaseSalaries.at(type) << L" руб./день)" << endl;
        }

        int typeChoice = safeInputInt(L"Выберите тип работника (1-5 или 0 для отмены): ");
        if (typeChoice < 1 || typeChoice > 5) return;

        WorkerType type = static_cast<WorkerType>(typeChoice - 1);
        if (money < workerBaseSalaries.at(type)) {
            wcout << L"Недостаточно денег для найма!" << endl;
            return;
        }

        wstring workerName = safeInputString(L"Введите имя работника: ");
        workers.push_back(make_unique<Worker>(nextWorkerId++, workerName, type));
        wcout << L"Вы наняли нового работника: " << workerName << L" (" << workerTypeNames.at(type) << L")" << endl;
        updateMaps();
    }

    void showWorkers() {
        wcout << L"\n=== Список работников ===" << endl;
        if (workers.empty()) {
            wcout << L"Работников нет" << endl;
            return;
        }
        for (const auto& worker : workers) {
            wcout << L"ID:" << worker->id << L" | " << worker->name << L" | "
                << worker->getTypeName() << L" | Зарплата: " << worker->salary << L" руб./день" << endl;
        }
    }

    void buyFood() {
        wcout << L"\n=== Покупка корма ===" << endl;
        wcout << L"1. Маленькая упаковка (50 ед., 5,000 руб.)" << endl;
        wcout << L"2. Средняя упаковка (100 ед., 9,000 руб.)" << endl;
        wcout << L"3. Большая упаковка (200 ед., 16,000 руб.)" << endl;

        int choice = safeInputInt(L"Выберите вариант (1-3 или 0 для отмены): ");
        if (choice < 1 || choice > 3) return;

        int amount = 0;
        int cost = 0;
        switch (choice) {
        case 1: amount = 50; cost = 5000; break;
        case 2: amount = 100; cost = 9000; break;
        case 3: amount = 200; cost = 16000; break;
        }

        if (money < cost) {
            wcout << L"Недостаточно денег для покупки!" << endl;
            return;
        }

        food += amount;
        money -= cost;
        wcout << L"Вы купили " << amount << L" единиц корма за " << cost << L" руб." << endl;
    }

    void advertise() {
        wcout << L"\n=== Реклама зоопарка ===" << endl;
        wcout << L"1. Бюджетная реклама (+1 популярность, 5,000 руб.)" << endl;
        wcout << L"2. Стандартная реклама (+3 популярность, 15,000 руб.)" << endl;
        wcout << L"3. Широкая рекламная кампания (+7 популярность, 30,000 руб.)" << endl;

        int choice = safeInputInt(L"Выберите вариант (1-3 или 0 для отмены): ");
        if (choice < 1 || choice > 3) return;

        int popIncrease = 0;
        int cost = 0;
        switch (choice) {
        case 1: popIncrease = 1; cost = 5000; break;
        case 2: popIncrease = 3; cost = 15000; break;
        case 3: popIncrease = 7; cost = 30000; break;
        }

        if (money < cost) {
            wcout << L"Недостаточно денег для рекламы!" << endl;
            return;
        }

        popularity += popIncrease;
        money -= cost;
        wcout << L"Рекламная кампания успешно проведена! Популярность увеличилась на " << popIncrease << L"." << endl;
    }

    void showAnimalHappiness() {
        wcout << L"\n=== Уровень счастья животных ===" << endl;
        if (animals.empty()) {
            wcout << L"Животных нет" << endl;
            return;
        }
        for (const auto& animal : animals) {
            wstring genderStr = (animal->gender == 'M') ? L"Самец" : L"Самка";
            wcout << animal->name << L" (" << genderStr << L", ID:" << animal->id << L") - "
                << animal->getHappinessColor() << animal->getDisplayedHappiness() << L"%\033[0m" << endl;
        }
    }

    void refreshMarketForMoney() {
        if (money < marketRefreshCost) {
            wcout << L"Недостаточно денег для обновления рынка!" << endl;
            return;
        }
        money -= marketRefreshCost;
        refreshMarket();
    }

    void nextDay() {
        try {
            days++;
            animalsBoughtToday = 0;
            wcout << L"\n=== День " << days << L" ===" << endl;

            // Обновление возраста и проверка здоровья животных
            for (auto it = animals.begin(); it != animals.end(); ) {
                (*it)->age++;
                (*it)->checkDisease();

                if ((*it)->checkAge()) {
                    for (auto& enclosure : enclosures) {
                        enclosure->removeAnimal((*it)->id);
                    }
                    it = animals.erase(it);
                }
                else {
                    ++it;
                }
            }

            // Обновление состояния вольеров
            for (auto& enclosure : enclosures) {
                enclosure->updateCleanliness();
                enclosure->spreadDisease();
            }

            // Работа работников
            for (auto& worker : workers) {
                worker->performDuties(this);
            }

            // Обновление счастья животных
            for (auto& enclosure : enclosures) {
                bool isFed = (food >= static_cast<int>(animals.size()));
                for (auto animal : enclosure->containedAnimals) {
                    bool isAlone = enclosure->containedAnimals.size() == 1;
                    animal->updateHappiness(isFed, isAlone, enclosure->isClean);
                }
            }

            // Проверка на побег животных
            for (auto& enclosure : enclosures) {
                for (auto it = enclosure->containedAnimals.begin(); it != enclosure->containedAnimals.end(); ) {
                    if ((*it)->tryEscape()) {
                        animals.erase(remove_if(animals.begin(), animals.end(),
                            [&](const unique_ptr<Animal>& a) { return a->id == (*it)->id; }), animals.end());
                        it = enclosure->containedAnimals.erase(it);
                    }
                    else {
                        ++it;
                    }
                }
            }

            // Кормление животных
            if (!animals.empty()) {
                int food_needed = static_cast<int>(animals.size());
                if (food >= food_needed) {
                    food -= food_needed;
                    wcout << L"Животные накормлены. Остаток еды: " << food << L" ед." << endl;
                }
                else {
                    food = 0;
                    wcout << L"Предупреждение: не хватает еды для всех животных!" << endl;
                }
            }

            // Расчет посетителей и дохода
            visitors = min(2 * popularity, 1000);
            int income = visitors * static_cast<int>(animals.size()) * 100;
            money += income;

            // Изменение популярности
            popularity = max(0, popularity + (rand() % 3 - 1));

            wcout << L"Популярность зоопарка: " << popularity << endl;
            wcout << L"Посетители сегодня: " << visitors << L" чел." << endl;
            wcout << L"Доход от посетителей: " << income << L" руб." << endl;

            // Выплата зарплат
            payWorkers();

            // Показать счастье животных
            showAnimalHappiness();

            // Проверка условий окончания игры
            if (food == 0 && !animals.empty()) {
                wcout << L"Все животные умерли от голода! Игра окончена." << endl;
                exit(0);
            }

            if (money < 0) {
                wcout << L"Вы банкрот! Игра окончена." << endl;
                exit(0);
            }

            if (days >= victoryDays) {
                wcout << L"Поздравляем! Вы успешно управляли зоопарком " << victoryDays << L" дней!" << endl;
                exit(0);
            }

            // Периодическое обновление рынка
            if (days % 3 == 0) {
                refreshMarket();
            }
        }
        catch (const std::bad_alloc& e) {
            wcout << L"Ошибка памяти! Слишком много объектов." << endl;
            wcout << L"Попробуйте продать некоторых животных или уволить работников." << endl;
        }
        catch (const std::exception& e) {
            wcout << L"Ошибка: " << e.what() << endl;
        }
    }

    void payWorkers() {
        int totalSalary = 0;
        for (const auto& worker : workers) {
            totalSalary += worker->salary;
        }

        if (money < totalSalary) {
            wcout << L"Недостаточно денег для выплаты зарплат! Работники увольняются." << endl;
            workers.clear();
            workersMap.clear();
            return;
        }

        money -= totalSalary;
        wcout << L"Выплачено зарплат работникам: " << totalSalary << L" руб." << endl;
    }
};

void Worker::performDuties(Zoo* zoo) {
    switch (type) {
    case WORKER_CLEANER:
        cleanEnclosures(zoo->enclosures);
        break;
    case WORKER_VET:
        for (auto& animal : zoo->animals) {
            if (animal->getHealth() == AnimalHealth::SICK) {
                if (rand() % 100 < 70) {
                    animal->health = AnimalHealth::HEALTHY;
                    animal->hasDisease = false;
                    animal->daysSick = 0;
                }
            }
        }
        wcout << name << L" проверил здоровье всех животных." << endl;
        break;
    case WORKER_FEEDER:
        if (zoo->food >= (int)zoo->animals.size()) {
            zoo->food -= (int)zoo->animals.size();
            wcout << name << L" накормил всех животных." << endl;
        }
        else {
            wcout << name << L" не смог накормить животных - недостаточно еды!" << endl;
        }
        break;
    case WORKER_TRAINER:
        zoo->popularity += 2;
        wcout << name << L" провел дрессировку. Популярность +2." << endl;
        break;
    case WORKER_GUIDE:
        zoo->popularity += 3;
        zoo->money += zoo->visitors * 100;
        wcout << name << L" провел экскурсию. Популярность +3, доход " << zoo->visitors * 100 << L" руб." << endl;
        break;
    }
}

int wmain() {
    srand(static_cast<unsigned>(time(0)));
    setlocale(LC_ALL, "ru_RU.UTF-8");
    (void)_setmode(_fileno(stdout), _O_U16TEXT);
    (void)_setmode(_fileno(stdin), _O_U16TEXT);
    (void)_setmode(_fileno(stderr), _O_U16TEXT);

    wcout << L"=== Добро пожаловать в ZooSIMS ===" << endl;
    wcout << L"Введите название вашего зоопарка: ";
    wstring name;
    getline(wcin, name);

    Zoo zoo(name, 100);

    while (true) {
        zoo.showMainMenu();
        int choice = safeInputInt(L"Выберите действие: ");

        switch (choice) {
        case 1: {
            zoo.showAnimalsMenu();
            int subChoice = safeInputInt(L"Выберите действие: ");
            switch (subChoice) {
            case 1: zoo.buyAnimal(); break;
            case 2: zoo.sellAnimal(); break;
            case 3: zoo.showAnimals(); break;
            case 4: zoo.refreshMarketForMoney(); break;
            case 5: zoo.renameAnimal(); break;
            case 6: zoo.tryBreedAnimals(); break;
            case 0: break;
            default: wcout << L"Неверный выбор!" << endl;
            }
            break;
        }
        case 2: {
            zoo.showEnclosuresMenu();
            int subChoice = safeInputInt(L"Выберите действие: ");
            switch (subChoice) {
            case 1: zoo.buyEnclosure(); break;
            case 2: zoo.showEnclosures(); break;
            case 0: break;
            default: wcout << L"Неверный выбор!" << endl;
            }
            break;
        }
        case 3: {
            zoo.showWorkersMenu();
            int subChoice = safeInputInt(L"Выберите действие: ");
            switch (subChoice) {
            case 1: zoo.hireWorker(); break;
            case 2: zoo.showWorkers(); break;
            case 0: break;
            default: wcout << L"Неверный выбор!" << endl;
            }
            break;
        }
        case 4: zoo.buyFood(); break;
        case 5: zoo.advertise(); break;
        case 6: zoo.nextDay(); break;
        case 7: zoo.showAnimalHappiness(); break;
        case 9: return 0;
        default: wcout << L"Неверный выбор!" << endl;
        }
    }
}
