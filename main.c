#include <stdio.h>
#include "village.h"
#include "dungeon.h"
#include "npc.h"
#include "character.h"
#include "enemy.h"
#include "battleScene.h"
#include "attacker.h"

// 전역 캐릭터 배열
Character characters[4];

// Main game menu
void main_menu() {
    printf("\n--- Text-Based RPG Game ---\n");
    printf("1. Go to Village\n");
    printf("2. Enter Dungeon\n");
    printf("3. Exit Game\n");
    printf("Choice: ");
}

// Game loop function
void start_game() {
    Village village;
    Dungeon dungeon;

    initialize_characters(characters, 4);  // 캐릭터 초기화
    initialize_village(&village);
    initialize_dungeon(&dungeon);

    while (1) {
        main_menu();
        
        int choice;
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                village_menu(&village);
                break;
            case 2:
                printf("\n--- Entering the Dungeon ---\n");
                while (1) {
                    printf("Choose direction (N: North, S: South, E: East, W: West, 0: Exit Dungeon): ");
                    char direction;
                    scanf(" %c", &direction);
                    
                    if (direction == '0') {
                        printf("Exiting the dungeon.\n");
                        break;
                    }

                    move_party(&dungeon, direction, characters, 4);
                }
                break;
            case 3:
                printf("Exiting the game.\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}

int main() {
    start_game();
    return 0;
}
