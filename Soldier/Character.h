/*****************************************************************************
 * File: Character.h
 * Author: Andrew A. Krupp
 * Class: COP 3003, Fall 2022
 * Desc: Declaration of a Character class object
 * Audit:
 *      1.1.0: 11/01/22: class creation and declaration
 *      1.1.1: 11/03/22: added maxHp field, added heal, getMaxHP,
 *                       and takeDamage Methods
 ****************************************************************************/

//HEADER GUARD
#ifndef SOLDIER_CHARACTER_H
#define SOLDIER_CHARACTER_H

class Character{
private:
    int lvl;
    int baseHP;
    int currentHP;
    int maxHP;

public:
    //Constructors
    //------------------------------------------------------------------------
    Character();
    Character(int lvl, int baseHP);

    //Accessors
    //------------------------------------------------------------------------
    void setLvl(int lvlToSet);
    void setBaseHP(int baseHPToSet);
    int getLvl();
    int getBaseHP();
    int getCurrentHP();
    int getMaxHP();

    //Methods
    //------------------------------------------------------------------------
    void setFullHP();
    void takeDamage(int damageToTake);
    void heal(int healingToTake);

};

#endif //SOLDIER_CHARACTER_H
