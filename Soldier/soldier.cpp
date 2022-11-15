/*****************************************************************************
 * File: soldier.cpp
 * Author: Andrew A. Krupp
 * Class: COP 3003, Fall 2022
 * Desc: creation of a finite state machine of a soldier in
 *      a RPG
 * Audit:
 *      1.0.0: 10/18/22: Lab 1 in class, project creation
 *      1.0.1: 10/20/22: Added loops and logic (switch and state changes)
 *      1.0.2: 10/27/22: made project modular (added functions), added logic
 *                       to checkPatrolling function, and added documentation
 *                       to functions
 *      1.0.3: 11/01/22: Added Character class, removed associated API
 *                       elements
 *      1.0.4: 11/03/22: Finished combat, damage, and hit functions.
 *                       Combat now occurs
 *      1.0.5: 11/08/22: Added pointers to functions and enemy (mostly for
 *                       demonstration purposes)
 ****************************************************************************/
#include <iostream>
#include <cstdlib> // random numbers
#include <ctime>   // random seed
#include "Character.h"

// Constants & Types
// ------------------------------------------------------------

const int START_LVL_DIE = 6;
const int HP_DIE = 28;
const int D_20 = 20;
const int D_8 = 8;
const int ARMOR_CLASS_BASE = 14;
const int ARMOR_CLASS_ATTACKING = 12;
const int ARMOR_CLASS_DEFENDING = 14;
const int ARMOR_CLASS_RETREATING = 16;

enum SoldierState {
    ENEMY = -1,
    HOMING,
    PATROLLING,
    ATTACKING,
    DEFENDING,
    RETREATING
}; // soldier state


// Function Prototypes (Declarations)
// ------------------------------------------------------------
SoldierState checkHoming(Character &soldierToCheck);
int checkForEnemy(Character *& enemyToCheck);
SoldierState checkPatrolling(Character * soldierToCompare, Character *& enemyToCompare);
SoldierState checkAttacking(Character soldierToCompare, Character * enemyToCompare);
SoldierState checkDefending(Character soldierToCompare, Character * enemyToCompare);
SoldierState checkRetreating(Character soldierToCompare, Character * enemyToCompare);
bool doesHit(SoldierState state);
void damage(Character &characterToDamage, Character characterAttacking);
void combat(Character &soldier, Character * enemy, SoldierState state);

// Function Definitions
// ------------------------------------------------------------

/**
 * Creates a soldier object with random lvl and base health. Then goes through the soldier
 * state diagram as displayed. Calls functions depending on which state soldier is in and if
 * combat needs to occur
 * @return 1 for program completion
 */
int main() {

    // init random numbers
    std::srand(std::time(NULL));


    Character soldier;
    SoldierState soldierState = HOMING;

    soldier.setLvl(std::rand() % START_LVL_DIE + 1);
    soldier.setBaseHP(std::rand() % HP_DIE + 1);
    soldier.setFullHP();
    std::cout << "Soldier is level " << soldier.getLvl()
              << ", with " << soldier.getCurrentHP()
              << " hit points!" << std::endl;
    Character * enemy = nullptr;

    // main game loop until soldier dies
    do {
        switch (soldierState) {

            case (HOMING):
                soldierState = checkHoming(soldier);
                break;
                // end Homing
            case (PATROLLING):
                soldierState = checkPatrolling(&soldier, enemy);
                break;
                //  end Patrolling
            case (ATTACKING):
                soldierState= checkAttacking(soldier, enemy);
                combat(soldier, enemy, soldierState);
                break;
            case (DEFENDING):
                soldierState = checkDefending(soldier, enemy);
                combat(soldier, enemy, soldierState);
                break;
            case (RETREATING):
                soldierState = checkRetreating(soldier, enemy);
                combat(soldier, enemy, soldierState);
        } // end Switch
    } while (soldier.getCurrentHP() > 0);
    std::cout << "Soldier Died" << std::endl;
    delete enemy;
    return 0;
} // end main

/**
 * checks soldiers hp to see if it is full, if it is full the soldier begins patrolling.
 * Otherwise, the soldier begins healing
 * @param soldierToCheck the current homing soldier
 * @return returns PATROLLING state once the soldier reaches full health
 */
SoldierState checkHoming(Character &soldierToCheck){
    SoldierState retMode = HOMING;
    if (soldierToCheck.getCurrentHP() == soldierToCheck.getMaxHP()) {
        std::cout << "soldier is at full health." << std::endl;
        retMode = PATROLLING;
    }  // end soldierToCheck full HP
    else {
        std::cout << "soldierToCheck needs to heal." << std::endl;
        soldierToCheck.heal(soldierToCheck.getMaxHP() * .2);
    } // end soldierToCheck low HP
    return retMode;
}

/**
 * Takes in the enemy, and rolls a dice to see if an enemy is encountered (roll of a 1), if an enemy is encountered,
 * the enemies lvl, health, and base hp is set
 * @param enemyToCheck the current enemy to check
 * @return returns either 1 or 0. 0 if the enemy was not encountered, 1 if an enemy is encountered
 */
int checkForEnemy(Character *& enemy){
    int encounteredEnemy = 0;
    int enemyCheck = std::rand() % D_20 + 1;
    if (enemyCheck == 1) {
        enemy = new Character();
        enemy->setLvl(std::rand() % START_LVL_DIE + 1);
        enemy->setBaseHP(std::rand() % HP_DIE + 1);
        enemy->setFullHP();
        std::cout << "Soldier has encountered a level " << enemy->getLvl()
                  << " enemy with " << enemy->getCurrentHP()
                  << " hit points!" << std::endl;
        encounteredEnemy = 1;
        return encounteredEnemy;
    }  // end enemyToCheck with HP
    else {
        std::cout << "All clear." << std::endl;
        return encounteredEnemy;
    } // end enemyToCheck no HP

}
/**
 * Calls the check for enemy, if an enemy is encountered it checks it's level, if the soldier is
 * greater than two levels above the soldier, the state is changed to ATTACKING, if the soldiers
 * level is within a range of 2 of the enemies level, the soldier defends, and if the soldiers level
 * is greater than two less than the enemies level the soldier retreats
 * @param soldierToCompare the values contained in the soldier object to be used to compare to the enemy
 * @param enemyToCompare the enemy, passed by reference so it can be updated/created
 * @return returns the state based on the comparison between the two characters
 */
SoldierState checkPatrolling(Character * soldierToCompare, Character *& enemyToCompare) {
    SoldierState retMode = PATROLLING;
    if (checkForEnemy(enemyToCompare)) { //checkForEnemy returns 1 if enemy is encountered
        std::cout << "Should the soldier fight or flee?" << std::endl;
        if (soldierToCompare->getLvl() > enemyToCompare->getLvl() + 2) {
            retMode = ATTACKING;
        } else if (soldierToCompare->getLvl() < enemyToCompare->getLvl() - 2) {
            retMode = RETREATING;
        } else {
            retMode = DEFENDING;
        }// end fight or flee
    }
    return retMode;
}

/**
 * Function checks if the soldier needs to remain attacking, or change to defending or patrolling
 * @param soldierToCompare copied values from the character object to compare to the other character
 *                         object
 * @param enemyToCompare copied values from the character object to compare to the other character
 *                         object
 * @return returns the state based on the soldiers or enemies hp
 */
SoldierState checkAttacking(Character soldierToCompare, Character * enemyToCompare){
    SoldierState retMode = ATTACKING;
    std::cout << "Soldier is attacking" << std::endl; //place holder, logic will be inserted later
    if(soldierToCompare.getCurrentHP() < (soldierToCompare.getMaxHP()*0.65)){
        retMode = DEFENDING;
    } else if (enemyToCompare->getCurrentHP() <= 0){
        std::cout << "Enemy is dead, returning to patrol" << std::endl;
        retMode = PATROLLING;
    }
    return retMode;
}

/**
 * Function checks if the soldier needs to remain defending, or change to retreating or patrolling
 * @param soldierToCompare copied values from the character object to compare to the other character
 *                         object
 * @param enemyToCompare copied values from the character object to compare to the other character
 *                         object
 * @return returns either DEFENDING, RETREATING, or PATROLLING depending on the health of the soldier and enemy
 */
SoldierState checkDefending(Character soldierToCompare, Character * enemyToCompare){
    SoldierState retMode = DEFENDING;
    std::cout << "Soldier is defending" << std::endl; //place holder, logic will be inserted later
    if(soldierToCompare.getCurrentHP() < (soldierToCompare.getMaxHP()*0.25)){
        retMode = RETREATING;
    } else if (enemyToCompare->getCurrentHP() <= 0){
        std::cout << "Enemy is dead, returning to patrol" << std::endl;
        retMode = PATROLLING;
    }
    return retMode;
}

/**
 * Function checks if the soldier needs to remain retreating, or change to homing
 * @param soldierToCompare copied values from the character object to compare to the other character
 *                         object
 * @param enemyToCompare copied values from the character object to compare to the other character
 *                         object
 * @return returns either RETREATING or HOMING depending on the health of the soldier and enemy
 */
SoldierState checkRetreating(Character soldierToCompare, Character * enemyToCompare){
    SoldierState retMode = RETREATING;
    std::cout << "Soldier is retreating" << std::endl; //place holder, logic will be inserted later
    if(soldierToCompare.getCurrentHP() < (soldierToCompare.getMaxHP()*0.01) || enemyToCompare->getCurrentHP() <= 0){
        std::cout << "soldier needs to heal, returning to base" << std::endl;
        retMode = HOMING;
    }
    return retMode;
}

/**
 * Takes in the state of the character rolling to hit, and determines how high their roll needs to be to hit
 * an attack
 * @param state the state of the character trying to hit
 * @return true or false depending on if the attack roll was greater than the AC
 */
bool doesHit(SoldierState state){
    bool hit = false;
    int AC = ARMOR_CLASS_BASE;
    if(state == ATTACKING){
        AC = ARMOR_CLASS_ATTACKING;
    } else if(state == DEFENDING){
        AC = ARMOR_CLASS_DEFENDING;
    } else if(state == RETREATING) {
        AC = ARMOR_CLASS_RETREATING;
    }
    int attackRoll = std::rand() % D_20 + 1;
    if(attackRoll >= AC){
        hit = true;
    }
    return hit;
}

/**
 * rolls the damage of a successful attack and subtracts that damage from that characters current hp
 * @param characterToDamage passed by reference, the character to subtract health from
 * @param characterAttacking the character to get the copied level to add to the damage from
 */
void damage(Character &characterToDamage, Character characterAttacking){
    int damageRoll = std::rand() % D_8 + 1 + characterAttacking.getLvl();
    characterToDamage.takeDamage(damageRoll);
    std::cout << "Dealt " << damageRoll << " damage!" << std::endl;
}

/**
 * this function first checks to see if the enemy is dead, if not then hits are rolled.
 * If a hit lands, damage is done, and is reported to the user
 * @param soldier passed by reference, character to be modified by the damage function
 * @param enemy passed by reference, character to be modified by the damage function
 * @param state passed by value, the state determines the AC of the enemies attack roll
 */
void combat(Character &soldier, Character * enemy, SoldierState state){
    //soldiers turn
    if(enemy->getCurrentHP() > 0) {
        if (doesHit(ENEMY)) {
            std::cout << "Soldier hit! ";
            damage(* enemy, soldier);
            std::cout << "Enemy health is now " << enemy->getCurrentHP() << std::endl;
        }
        //enemies turn
        if (doesHit(state)) {
            std::cout << "Enemy hit! ";
            damage(soldier, * enemy);
            std::cout << "Soldier health is now " << soldier.getCurrentHP() << std::endl;
        }
    }
}