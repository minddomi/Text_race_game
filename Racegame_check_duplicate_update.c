#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define RACE_DISTANCE 40 // 트랙 길이

typedef struct {
    char name[10];
    int position;
    int ranking; // 순위 저장
} Horse;

// 콘솔 화면 지우기
void clear_screen() {
    system("cls");
}

// 선두 확인
int get_leader(Horse horses[], int num_horses) {
    int leader = 0;
    for (int i = 0; i < num_horses; i++) {
        if (horses[i].position > horses[leader].position) {
            leader = i;
        }
    }
    return leader;
}

// 경주 상황 출력
void display_race(Horse horses[], int num_horses, int player_horses[], int num_players) {
    clear_screen();
    printf("<start>      1       10       20       30       40(end)\n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < num_horses; i++) {
        printf("%s: ", horses[i].name);
        for (int j = 0; j < horses[i].position; j++) {
            printf(" ");
        }
        printf(">");
        for (int p = 0; p < num_players; p++) {
            if (player_horses[p] == i) {
                printf(" (P%d)", p + 1); //플레이어 선택 표시
            }
        }
        printf("\n");
    }

    //선두마 표시
    int leader = get_leader(horses, num_horses);
    printf("\n현재 선두: %s (Horse %d)\n", horses[leader].name, leader + 1);
}

// 경주마 이동
void move_horses(Horse horses[], int num_horses) {
    for (int i = 0; i < num_horses; i++) {
        if (horses[i].position < RACE_DISTANCE) {
            horses[i].position += rand() % 3; // 0~2칸 무작위 이동
            if (horses[i].position > RACE_DISTANCE) {
                horses[i].position = RACE_DISTANCE;
            }
        }
    }
}

// 경주 종료 확인
int is_race_over(Horse horses[], int num_horses) {
    for (int i = 0; i < num_horses; i++) {
        if (horses[i].position < RACE_DISTANCE) {
            return 0; // 아직 경주가 끝나지 않음
        }
    }
    return 1; // 경주 종료
}

// 순위 계산 함수
void update_rankings(Horse horses[], int num_horses, int* current_rank) {
    for (int i = 0; i < num_horses; i++) {
        if (horses[i].position == RACE_DISTANCE && horses[i].ranking == 0) {
            horses[i].ranking = (*current_rank)++;
        }
    }
}

// 순위 정렬 함수
void sort_horses_by_ranking(Horse horses[], int num_horses) {
    for (int i = 0; i < num_horses - 1; i++) {
        for (int j = i + 1; j < num_horses; j++) {
            if (horses[i].ranking > horses[j].ranking) {
                Horse temp = horses[i];
                horses[i] = horses[j];
                horses[j] = temp;
            }
        }
    }
}

// 중복 선택 확인 함수
int already_selected(int player_horses[], int current_player, int horse_number) {
    for (int i = 0; i < current_player; i++) {
        if (player_horses[i] == horse_number) {
            return 1; // 이미 선택된 말
        }
    }
    return 0; // 중복 아님
}

int main() {
    int num_players;
    printf("플레이어 수를 입력하세요 (1~10): ");
    scanf("%d", &num_players);
    if (num_players < 1 || num_players > 10) {
        printf("플레이어 수는 1에서 10 사이여야 합니다.\n");
        return 1;
    }

    int num_horses = num_players;
    Horse horses[100];
    int player_horses[100];

    for (int i = 0; i < num_horses; i++) {
        sprintf(horses[i].name, "Horse%d", i + 1);
        horses[i].position = 0;
        horses[i].ranking = 0;
    }

    for (int i = 0; i < num_players; i++) {
        while (1) {
            printf("플레이어 %d, 경주마를 선택하세요 (1~%d): ", i + 1, num_horses);
            scanf("%d", &player_horses[i]);
            if (player_horses[i] < 1 || player_horses[i] > num_horses) {
                printf("유효하지 않은 선택입니다. 다시 입력하세요.\n");
            }
            else if (already_selected(player_horses, i, player_horses[i] - 1)) {
                printf("이미 선택된 말입니다. 다른 말을 선택하세요.\n");
            }
            else {
                break; // 중복x
            }
        }
        player_horses[i]--;
    }
    
    srand(time(0));
    int current_rank = 1;

    while (1) {
        display_race(horses, num_horses, player_horses, num_players);
        move_horses(horses, num_horses);
        update_rankings(horses, num_horses, &current_rank);
        Sleep(200);
        if (is_race_over(horses, num_horses)) {
            printf("\n경주가 종료되었습니다!\n");
            break;
        }
    }

    sort_horses_by_ranking(horses, num_horses);
    printf("\n최종 순위:\n");
    for (int i = 0; i < num_horses; i++) {
        printf("%d위: %s\n", horses[i].ranking, horses[i].name);
    }

    return 0;
}