#ifndef RPG_H
#define RPG_H

#include <string>
#include <vector>
#include <windows.h>
#include <fstream>
#include <iostream>
using namespace std;

// =====================
//   COLORS
// =====================
inline void setRed()    { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); }
inline void setGreen()  { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); }
inline void setYellow() { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); }
inline void setWhite()  { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); }
inline void setReset()  { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  }

// =====================
//   WEAPON CLASS
// =====================
class Weapon {
private:
    string name;
    int damage;
    string type;
public:
    Weapon();
    Weapon(string n, int dmg, string t);
    string getName() const { return name; }
    int getDamage() const  { return damage; }
    string getType() const { return type; }
    void display() const;
};

// =====================
//   INVENTORY CLASS
// =====================
class Inventory {
private:
    vector<Weapon> weapons;
    int potions;
    int gold;
public:
    Inventory();
    void addWeapon(Weapon w);
    void addPotion(int qty);
    void addGold(int amount);
    bool usePotion();
    Weapon getBestWeapon() const;
    int getGold() const    { return gold; }
    int getPotions() const { return potions; }
    void display() const;
};

// =====================
//  CHARACTER (Abstract)
// =====================
class Character {
protected:
    string name;
    int health;
    int maxHealth;
    int attackPower;
    int defense;
    int level;
public:
    Character(string n, int hp, int atk, int def);
    virtual ~Character() {}
    virtual void attack(Character& target) = 0;
    virtual string getType() const = 0;
    void takeDamage(int dmg);
    void heal(int amount);
    void displayStats() const;
    string getName() const     { return name; }
    int getHealth() const      { return health; }
    int getMaxHealth() const   { return maxHealth; }
    int getAttackPower() const { return attackPower; }
    int getDefense() const     { return defense; }
    int getLevel() const       { return level; }
    bool isAlive() const       { return health > 0; }
};

// =====================
//    PLAYER CLASS
// =====================
class Player : public Character {
private:
    int experience;
    int expToNextLevel;
    Inventory inventory;
public:
    Player(string name);
    void attack(Character& target);
    string getType() const { return "Hero"; }
    void gainExp(int exp);
    void levelUp();
    bool usePotion();
    Inventory& getInventory() { return inventory; }
    int getExperience() const { return experience; }
    void saveToFile() const;
    void loadFromFile();
};

// =====================
//    ENEMY CLASS
// =====================
class Enemy : public Character {
private:
    int goldDrop;
    int expDrop;
    string enemyType;
public:
    Enemy(string type);
    void attack(Character& target);
    string getType() const { return enemyType; }
    int getGoldDrop() const { return goldDrop; }
    int getExpDrop() const  { return expDrop; }
};

// =====================
//    ROOM STRUCT
// =====================
struct Room {
    string name;
    string description;
    string enemyType;
    bool hasChest;
    bool visited;
};

// =====================
//    GAMEMAP CLASS
// =====================
class GameMap {
private:
    vector<Room> rooms;
    int currentRoom;
public:
    GameMap();
    void displayMap() const;
    Room& getCurrentRoom();
    bool moveToRoom(int index);
    int getRoomCount() const { return (int)rooms.size(); }
};

// =====================
//    BATTLE CLASS
// =====================
class Battle {
private:
    Player& player;
    Enemy&  enemy;
    void playerTurn();
    void enemyTurn();
    void showBattleStatus();
public:
    Battle(Player& p, Enemy& e);
    bool start();
};

// =====================
//     GAME CLASS
// =====================
class Game {
private:
    Player*  player;
    GameMap* gameMap;
    void showMainMenu();
    void startNewGame();
    void loadGame();
    void exploreMap();
    void handleRoom();
public:
    Game();
    ~Game();
    void run();
};

#endif
