#include <stdio.h>
#include <stdlib.h>
#include "../include/dungeon.h"
#include "../include/battleScene.h"
#include "../include/character.h"
#include "../include/enemy.h"
#include "../include/attacker.h"

#define N 4
#define M 4

typedef struct {
    int x;
    int y;
} Position;

Position playerPos = {0, 0};
Position exitPos = {3, 3};

void initialize_dungeon(Dungeon *dungeon) {
    dungeon->x = 0;
    dungeon->y = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            dungeon->map[i][j] = EVENT_NOWAY;
        }
    }
}

void initialize_dungeon_from_file(Dungeon *dungeon, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open dungeon file.\n");
        return;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int event;
            fscanf(file, "[%d]", &event);
            dungeon->map[i][j] = event;
        }
    }

    fclose(file);
}

void move_party(Dungeon *dungeon, char direction, Character characters[], int char_count) {
    int dx = 0, dy = 0;
    switch (direction) {
        case 'N': case 'n': dy = -1; break;
        case 'S': case 's': dy = 1; break;
        case 'E': case 'e': dx = 1; break;
        case 'W': case 'w': dx = -1; break;
        default: 
            printf("Invalid direction.\n");
            return;
    }
    movePlayer(dungeon, dx, dy, characters, char_count);
    printf("Current position: (%d, %d)\n", dungeon->x, dungeon->y);
    printMap(dungeon);
}

void printMap(Dungeon *dungeon) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (i == dungeon->y && j == dungeon->x)
                printf("[P] ");
            else if (i == exitPos.y && j == exitPos.x)
                printf("[E] ");
            else if (dungeon->map[i][j] == EVENT_ENEMY)
                printf("[EN]");
            else if (dungeon->map[i][j] == EVENT_CHEST)
                printf("[CH]");
            else if (dungeon->map[i][j] == EVENT_NOWAY)
                printf("[-] ");
            else if (dungeon->map[i][j] == EVENT_PATH)
                printf("[ ] ");
        }
        printf("\n");
    }
}

void movePlayer(Dungeon *dungeon, int dx, int dy, Character characters[], int char_count) {
    int newX = dungeon->x + dx;
    int newY = dungeon->y + dy;

    if (newX >= 0 && newX < M && newY >= 0 && newY < N && dungeon->map[newY][newX] != EVENT_NOWAY) {
        dungeon->x = newX;
        dungeon->y = newY;
        printf("Moved to (%d, %d)\n", newX, newY);

        if (newX == exitPos.x && newY == exitPos.y) {
            printf("Exit found! Dungeon cleared!\n");
        } else if (!handle_event(dungeon, characters, char_count)) {
            printf("There is nothing here.\n");
        }
    } else {
        printf("Cannot move to (%d, %d)\n", newX, newY);
    }
}

int handle_event(Dungeon *dungeon, Character characters[], int char_count) {
    int event = dungeon->map[dungeon->y][dungeon->x];
    
    switch (event) {
        case EVENT_NONE:
            return 0;
        case EVENT_ENEMY: {
            printf("You encountered enemies! Starting battle with all enemies at this location.\n");
            break;
        }
        case EVENT_CHEST:
            printf("You found a treasure chest!\n");
            break;
    }

    if (event != EVENT_NONE) {
        dungeon->map[dungeon->y][dungeon->x] = EVENT_PATH;
    }
    return event != EVENT_NONE;
}

int main() {
    Dungeon dungeon;
    Character characters[4];
    int char_count = 4;

    initialize_dungeon(&dungeon);
    initialize_dungeon_from_file(&dungeon, "dungeon_map.txt");

    printMap(&dungeon);

    char direction;
    while (1) {
        printf("Enter direction (N/S/E/W): ");
        scanf(" %c", &direction);
        move_party(&dungeon, direction, characters, char_count);
    }

    return 0;
}
