/*****************************************************************************
 * File: Character.cpp
 * Author: Andrew A. Krupp
 * Class: COP 3003, Fall 2022
 * Desc: Definition of a Character class object
 * Audit:
 *      1.1.0: 11/01/22: class creation and definition
 *      1.1.1: 11/03/22: added maxHp field, added heal, getMaxHP,
 *                       and takeDamage Methods
 ****************************************************************************/

#include "Character.h"

//Constructors
//----------------------------------------------------------------------------
Character::Character(){
    lvl = 0;
    baseHP = 0;
    currentHP = 0;
    maxHP = baseHP * lvl;
}

Character::Character(int lvl, int baseHP){
    this->lvl = lvl;
    this->baseHP = baseHP;
    this->maxHP = lvl*baseHP;
    //this->currentHP = currentHP;
}

//Accessors
//----------------------------------------------------------------------------
void Character::setLvl(int lvlToSet){lvl = lvlToSet;}

void Character::setBaseHP(int baseHPToSet){baseHP = baseHPToSet;}

int Character::getLvl(){return lvl;}

int Character::getBaseHP(){return baseHP;}

int Character::getCurrentHP(){return currentHP;}

int Character::getMaxHP() {return maxHP;}

//Methods
//----------------------------------------------------------------------------
void Character::setFullHP() {
    currentHP = lvl * baseHP;
    maxHP = currentHP;
}

void Character::takeDamage(int damageToTake) {
    currentHP -= damageToTake;
}

void Character::heal(int healingToTake){
    currentHP += healingToTake;
    if(this ->currentHP > maxHP) {
        setFullHP();
    }
}