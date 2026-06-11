#include "RPG.h"

// =====================
//   WEAPON
// =====================
Weapon::Weapon() {
    name   = "Bare Hands";
    damage = 2;
    type   = "None";
}

Weapon::Weapon(string n, int dmg, string t) {
    name   = n;
    damage = dmg;
    type   = t;
}

void Weapon::display() const {
    setGreen();
    cout << name << " [" << type << "] - Damage: " << damage << endl;
    setReset();
}

// =====================
//   INVENTORY
// =====================
Inventory::Inventory() {
    potions = 2;
    gold    = 0;
}

void Inventory::addWeapon(Weapon w) {
    weapons.push_back(w);
    setGreen();
    cout << ">> " << w.getName() << " added to inventory!" << endl;
    setReset();
}

void Inventory::addPotion(int qty) {
    potions += qty;
}

void Inventory::addGold(int amount) {
    gold += amount;
}

bool Inventory::usePotion() {
    if (potions > 0) {
        potions--;
        return true;
    }
    return false;
}

Weapon Inventory::getBestWeapon() const {
    if (weapons.empty()) return Weapon();
    Weapon best = weapons[0];
    for (int i = 0; i < (int)weapons.size(); i++) {
        if (weapons[i].getDamage() > best.getDamage())
            best = weapons[i];
    }
    return best;
}

void Inventory::display() const {
    setYellow();
    cout << "\n--- Inventory ---"    << endl;
    cout << "Gold    : " << gold    << endl;
    cout << "Potions : " << potions << endl;
    cout << "Weapons : "            << endl;
    setReset();
    if (weapons.empty()) {
        setRed();
        cout << "  (No weapons)" << endl;
        setReset();
    }
    for (int i = 0; i < (int)weapons.size(); i++) {
        cout << "  - ";
        weapons[i].display();
    }
}

// =====================
//   CHARACTER
// =====================
Character::Character(string n, int hp, int atk, int def) {
    name        = n;
    health      = hp;
    maxHealth   = hp;
    attackPower = atk;
    defense     = def;
    level       = 1;
}

void Character::takeDamage(int dmg) {
    int actualDmg = dmg - defense;
    if (actualDmg < 1) actualDmg = 1;
    health -= actualDmg;
    if (health < 0) health = 0;
}

void Character::heal(int amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

void Character::displayStats() const {
    setYellow();
    cout << "\n===== " << name << " =====" << endl;
    setReset();
    cout << "Type    : " << getType()    << endl;
    cout << "Level   : " << level        << endl;
    setGreen();
    cout << "HP      : " << health << "/" << maxHealth << endl;
    setReset();
    cout << "Attack  : " << attackPower  << endl;
    cout << "Defense : " << defense      << endl;
}

// =====================
//   PLAYER
// =====================
Player::Player(string n) : Character(n, 100, 15, 5) {
    experience     = 0;
    expToNextLevel = 100;
    inventory.addWeapon(Weapon("Rusty Dagger", 10, "Dagger"));
}

void Player::attack(Character& target) {
    Weapon best  = inventory.getBestWeapon();
    int totalDmg = attackPower + best.getDamage();
    setGreen();
    cout << name << " attacked with " << best.getName()
         << "! (" << totalDmg << " damage)" << endl;
    setReset();
    target.takeDamage(totalDmg);
}

void Player::gainExp(int exp) {
    experience += exp;
    setGreen();
    cout << ">> +" << exp << " Experience gained!" << endl;
    setReset();
    if (experience >= expToNextLevel) levelUp();
}

void Player::levelUp() {
    level++;
    experience     = 0;
    expToNextLevel = level * 120;
    maxHealth     += 20;
    health         = maxHealth;
    attackPower   += 5;
    defense       += 2;
    setGreen();
    cout << "\n*** LEVEL UP! You are now Level " << level << " ***" << endl;
    cout << "HP, Attack and Defense increased!" << endl;
    setReset();
}

bool Player::usePotion() {
    if (inventory.usePotion()) {
        heal(30);
        setGreen();
        cout << ">> Potion used! +30 HP restored." << endl;
        setReset();
        return true;
    }
    setRed();
    cout << ">> No potions left!" << endl;
    setReset();
    return false;
}

void Player::saveToFile() const {
    // SaveData folder auto create
    BOOL created = CreateDirectoryA("SaveData", NULL);
    if (created || GetLastError() == ERROR_ALREADY_EXISTS) {
        ofstream file("SaveData\\save_game.txt");
        if (file.is_open()) {
            file << name        << "\n"
                 << health      << "\n"
                 << maxHealth   << "\n"
                 << attackPower << "\n"
                 << defense     << "\n"
                 << level       << "\n"
                 << experience  << "\n"
                 << inventory.getGold()    << "\n"
                 << inventory.getPotions() << "\n";
            file.close();
            setGreen();
            cout << ">> Game saved successfully!" << endl;
            cout << ">> Location: SaveData\\save_game.txt" << endl;
            setReset();
        } else {
            setRed();
            cout << ">> ERROR: File could not be created!" << endl;
            setReset();
        }
    } else {
        setRed();
        cout << ">> ERROR: Could not create SaveData folder!" << endl;
        setReset();
    }
}

void Player::loadFromFile() {
    ifstream file("SaveData\\save_game.txt");
    if (file.is_open()) {
        int gold, potions;
        file >> name >> health >> maxHealth
             >> attackPower >> defense >> level
             >> experience >> gold >> potions;
        file.close();
        for (int i = 2; i < potions; i++)
            inventory.addPotion(1);
        inventory.addGold(gold);
        setGreen();
        cout << ">> Game loaded successfully!" << endl;
        setReset();
    } else {
        setRed();
        cout << ">> No save file found. Please save first!" << endl;
        setReset();
    }
}

// =====================
//   ENEMY
// =====================
Enemy::Enemy(string type) : Character(type, 50, 10, 2) {
    enemyType   = type;
    goldDrop    = 0;
    expDrop     = 0;

    if (type == "Sand Wraith") {
        health = maxHealth = 45;
        attackPower = 10;
        defense     = 2;
        goldDrop    = 12;
        expDrop     = 35;
    } else if (type == "Jungle Demon") {
        health = maxHealth = 70;
        attackPower = 18;
        defense     = 4;
        goldDrop    = 22;
        expDrop     = 55;
    } else if (type == "Mountain Troll") {
        health = maxHealth = 90;
        attackPower = 22;
        defense     = 8;
        goldDrop    = 30;
        expDrop     = 70;
    } else if (type == "Ruins Phantom") {
        health = maxHealth = 60;
        attackPower = 15;
        defense     = 3;
        goldDrop    = 18;
        expDrop     = 50;
    } else if (type == "Death Dragon") {
        health = maxHealth = 220;
        attackPower = 40;
        defense     = 18;
        goldDrop    = 150;
        expDrop     = 250;
    }
}

void Enemy::attack(Character& target) {
    setRed();
    cout << name << " attacked you! ("
         << attackPower << " damage)" << endl;
    setReset();
    target.takeDamage(attackPower);
}

// =====================
//   GAMEMAP
// =====================
GameMap::GameMap() {
    currentRoom = 0;

    Room r1;
    r1.name        = "Cursed Desert";
    r1.description = "Scorching sands hide ancient evil. A Sand Wraith emerges!";
    r1.enemyType   = "Sand Wraith";
    r1.hasChest    = true;
    r1.visited     = false;
    rooms.push_back(r1);

    Room r2;
    r2.name        = "Dark Jungle";
    r2.description = "The trees bleed. Something watches from the shadows...";
    r2.enemyType   = "Jungle Demon";
    r2.hasChest    = false;
    r2.visited     = false;
    rooms.push_back(r2);

    Room r3;
    r3.name        = "Haunted Mountains";
    r3.description = "Frozen screams echo through the peaks. A Troll blocks the path!";
    r3.enemyType   = "Mountain Troll";
    r3.hasChest    = true;
    r3.visited     = false;
    rooms.push_back(r3);

    Room r4;
    r4.name        = "Forgotten Ruins";
    r4.description = "The dead walk here. A Phantom rises from cursed ground!";
    r4.enemyType   = "Ruins Phantom";
    r4.hasChest    = false;
    r4.visited     = false;
    rooms.push_back(r4);

    Room r5;
    r5.name        = "Dragon Den";
    r5.description = "Bones of fallen heroes litter the floor. Death Dragon awaits!";
    r5.enemyType   = "Death Dragon";
    r5.hasChest    = false;
    r5.visited     = false;
    rooms.push_back(r5);
}

void GameMap::displayMap() const {
    setYellow();
    cout << "\n=======================================" << endl;
    cout << "             WORLD MAP                 " << endl;
    cout << "=======================================" << endl;
    setReset();

    for (int i = 0; i < (int)rooms.size(); i++) {
        if (i == currentRoom) setGreen();
        else setWhite();

        cout << i + 1 << ". " << rooms[i].name;
        if (i == currentRoom) cout << "  << (You are here)";
        if (rooms[i].visited) cout << "  [Visited]";

        if (!rooms[i].enemyType.empty()) {
            setRed();
            cout << "  [!! Enemy]";
            if (i == currentRoom) setGreen(); else setWhite();
        }
        if (rooms[i].hasChest) {
            setYellow();
            cout << "  [$ Chest]";
            if (i == currentRoom) setGreen(); else setWhite();
        }
        cout << endl;
        setReset();
        cout << "   -> " << rooms[i].description << endl;
    }
    setYellow();
    cout << "=======================================" << endl;
    setReset();
}

Room& GameMap::getCurrentRoom() {
    return rooms[currentRoom];
}

bool GameMap::moveToRoom(int index) {
    if (index >= 0 && index < (int)rooms.size()) {
        currentRoom = index;
        rooms[currentRoom].visited = true;
        setGreen();
        cout << "\n>> Moving to: " << rooms[currentRoom].name << endl;
        setReset();
        return true;
    }
    setRed();
    cout << ">> Invalid location!" << endl;
    setReset();
    return false;
}

// =====================
//   BATTLE
// =====================
Battle::Battle(Player& p, Enemy& e) : player(p), enemy(e) {}

void Battle::showBattleStatus() {
    setYellow();
    cout << "\n=======================================" << endl;
    cout << "           BATTLE STATUS               " << endl;
    cout << "=======================================" << endl;
    setGreen();
    cout << player.getName() << " HP: "
         << player.getHealth() << "/" << player.getMaxHealth() << endl;
    setRed();
    cout << enemy.getName() << " HP: "
         << enemy.getHealth() << "/" << enemy.getMaxHealth() << endl;
    setYellow();
    cout << "=======================================" << endl;
    setReset();
}

void Battle::playerTurn() {
    setWhite();
    cout << "\n--- Your Turn! What will you do? ---" << endl;
    setGreen();
    cout << "1. Attack" << endl;
    cout << "2. Use Potion (" << player.getInventory().getPotions() << " remaining)" << endl;
    cout << "3. View Stats" << endl;
    setReset();
    cout << "Choice: ";

    int choice;
    cin >> choice;

    if (choice == 1) {
        player.attack(enemy);
    } else if (choice == 2) {
        player.usePotion();
    } else if (choice == 3) {
        player.displayStats();
        player.getInventory().display();
    } else {
        setRed();
        cout << "Invalid choice! Auto-attacking..." << endl;
        setReset();
        player.attack(enemy);
    }
}

void Battle::enemyTurn() {
    if (enemy.isAlive()) {
        setRed();
        cout << "\n--- Enemy's Turn! ---" << endl;
        setReset();
        enemy.attack(player);
    }
}

bool Battle::start() {
    setRed();
    cout << "\n=======================================" << endl;
    cout << "           BATTLE STARTED!             " << endl;
    cout << "=======================================" << endl;
    cout << "Enemy Encountered: " << enemy.getName() << endl;
    cout << "Prepare yourself, warrior!"              << endl;
    setReset();
    enemy.displayStats();

    while (player.isAlive() && enemy.isAlive()) {
        showBattleStatus();
        playerTurn();

        if (!enemy.isAlive()) {
            setGreen();
            cout << "\n=======================================" << endl;
            cout << "      VICTORY! Enemy Defeated!         " << endl;
            cout << "=======================================" << endl;
            cout << ">> +" << enemy.getGoldDrop() << " Gold earned!"       << endl;
            cout << ">> +" << enemy.getExpDrop()  << " Experience gained!" << endl;
            setReset();
            player.getInventory().addGold(enemy.getGoldDrop());
            player.gainExp(enemy.getExpDrop());
            return true;
        }

        enemyTurn();

        if (!player.isAlive()) {
            setRed();
            cout << "\n=======================================" << endl;
            cout << "       DEFEAT! You have fallen!        " << endl;
            cout << "=======================================" << endl;
            cout << "The darkness claims your soul..."       << endl;
            cout << "=======================================" << endl;
            setReset();
            return false;
        }
    }
    return false;
}

// =====================
//   GAME
// =====================
Game::Game() {
    player  = NULL;
    gameMap = NULL;
}

Game::~Game() {
    delete player;
    delete gameMap;
}

void Game::showMainMenu() {
    setRed();
    cout << "\n=======================================" << endl;
    cout << "       SHADOW QUEST - HORROR RPG       " << endl;
    cout << "   Darkness awaits... dare you enter?  " << endl;
    cout << "=======================================" << endl;
    setWhite();
    cout << "  1. Start New Game                    " << endl;
    cout << "  2. Load Saved Game                   " << endl;
    setRed();
    cout << "  3. Exit                              " << endl;
    cout << "=======================================" << endl;
    setReset();
    cout << "Enter Choice: ";
}

void Game::startNewGame() {
    string name;
    setRed();
    cout << "\n=======================================" << endl;
    cout << "          NEW GAME STARTED             " << endl;
    cout << "=======================================" << endl;
    setReset();
    cout << "Enter your hero's name: ";
    cin >> name;

    player  = new Player(name);
    gameMap = new GameMap();

    setRed();
    cout << "\n---------------------------------------" << endl;
    cout << "Welcome, " << name << "..."               << endl;
    cout << "You wake up in a cursed world."           << endl;
    cout << "Monsters lurk in every shadow."           << endl;
    cout << "Survive the horror... if you can!"        << endl;
    cout << "---------------------------------------"   << endl;
    setReset();
}

void Game::loadGame() {
    setYellow();
    cout << "\n=======================================" << endl;
    cout << "           LOADING GAME...             " << endl;
    cout << "=======================================" << endl;
    setReset();
    player  = new Player("temp");
    gameMap = new GameMap();
    player->loadFromFile();
}

void Game::handleRoom() {
    Room& room = gameMap->getCurrentRoom();

    setRed();
    cout << "\n=======================================" << endl;
    cout << "Location : " << room.name               << endl;
    setReset();
    cout << room.description                          << endl;
    setRed();
    cout << "=======================================" << endl;
    setReset();

    if (!room.enemyType.empty()) {
        setRed();
        cout << "\n!! DANGER! " << room.enemyType
             << " appears from the darkness!" << endl;
        setReset();

        Enemy  enemy(room.enemyType);
        Battle battle(*player, enemy);
        bool   won = battle.start();

        if (!won) {
            setRed();
            cout << "\n=======================================" << endl;
            cout << "              GAME OVER                " << endl;
            cout << "=======================================" << endl;
            cout << "Your soul is lost forever..."          << endl;
            cout << "=======================================" << endl;
            setReset();
            return;
        }
        room.enemyType = "";
        setGreen();
        cout << "\n>> The creature is slain! Path is clear." << endl;
        setReset();
    }

    if (room.hasChest) {
        setYellow();
        cout << "\n>> A cursed chest lies in the corner!"   << endl;
        cout << ">> Bone Blade added to inventory!"         << endl;
        cout << ">> 1 Blood Potion added to inventory!"     << endl;
        setReset();
        player->getInventory().addWeapon(Weapon("Bone Blade", 22, "Sword"));
        player->getInventory().addPotion(1);
        room.hasChest = false;
    }

    if (room.enemyType.empty() && !room.hasChest) {
        setGreen();
        cout << ">> The area is silent... for now."         << endl;
        setReset();
    }
}

void Game::exploreMap() {
    setWhite();
    cout << "\n>> The cursed world awaits. Choose wisely..." << endl;
    setReset();

    while (player->isAlive()) {
        gameMap->displayMap();

        setYellow();
        cout << "\n=======================================" << endl;
        cout << "               OPTIONS                 " << endl;
        cout << "=======================================" << endl;
        setWhite();
        cout << "  1. Travel to Cursed Desert           " << endl;
        cout << "  2. Travel to Dark Jungle             " << endl;
        cout << "  3. Travel to Haunted Mountains       " << endl;
        cout << "  4. Travel to Forgotten Ruins         " << endl;
        setRed();
        cout << "  5. Travel to Dragon Den [FINAL BOSS] " << endl;
        setWhite();
        cout << "  6. View Inventory                    " << endl;
        cout << "  7. View Stats                        " << endl;
        setGreen();
        cout << "  8. Save Game                         " << endl;
        setWhite();
        cout << "  9. Return to Main Menu               " << endl;
        setYellow();
        cout << "=======================================" << endl;
        setReset();
        cout << "Enter Choice: ";

        int choice;
        cin >> choice;

        if (choice >= 1 && choice <= 5) {
            gameMap->moveToRoom(choice - 1);
            handleRoom();

            Room& r = gameMap->getCurrentRoom();
            if (r.name == "Dragon Den" && r.enemyType.empty()) {
                setGreen();
                cout << "\n=======================================" << endl;
                cout << "          CONGRATULATIONS!             " << endl;
                cout << "=======================================" << endl;
                cout << "  You slayed the Death Dragon!         " << endl;
                cout << "  The curse is broken!                 " << endl;
                cout << "  You are the ultimate horror survivor!" << endl;
                cout << "=======================================" << endl;
                cout << "           GAME COMPLETE!              " << endl;
                cout << "=======================================" << endl;
                setReset();
                break;
            }

        } else if (choice == 6) {
            player->getInventory().display();
        } else if (choice == 7) {
            player->displayStats();
        } else if (choice == 8) {
            player->saveToFile();
        } else if (choice == 9) {
            setWhite();
            cout << "\n>> Returning to Main Menu..." << endl;
            setReset();
            break;
        } else {
            setRed();
            cout << "\n>> Invalid choice! Please enter 1-9." << endl;
            setReset();
        }
    }
}

void Game::run() {
    int choice;
    do {
        showMainMenu();
        cin >> choice;

        if (choice == 1) {
            startNewGame();
            exploreMap();
        } else if (choice == 2) {
            loadGame();
            exploreMap();
        } else if (choice == 3) {
            setRed();
            cout << "\n=======================================" << endl;
            cout << "   The darkness swallows you whole...  " << endl;
            cout << "           Goodbye, Hero!              " << endl;
            cout << "=======================================" << endl;
            setReset();
        } else {
            setRed();
            cout << "\n>> Invalid choice! Enter 1, 2, or 3." << endl;
            setReset();
        }

    } while (choice != 3);
}
