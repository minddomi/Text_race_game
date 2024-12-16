#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <windows.h>
#include <string.h>

#define MAX_HORSES 10
#define RACE_DISTANCE 80

typedef struct
{
    char player_name[10];
    char horse_name[10];
    int position;
    int frame;
    char color;
    int ranking;
    int last_rank;
} Horse;

char* legs[] = {
    " //\\\\",
    " //ㅣ\\",
    " ///ㅣ",
    " /ㅣ/ㅣ",
    " ㅣㅣ//",
    " \\\\//",
    " \\\\ㅣㅣ",
    " \\ㅣ\\ㅣ",
    " ㅣ/\\\\" };

char* colors[] = {
    "\033[31m", // 빨강
    "\033[32m", // 초록
    "\033[33m", // 노랑
    "\033[34m", // 파랑
    "\033[35m", // 자홍
    "\033[36m", // 청록
    "\033[37m", // 흰색
    "\033[90m", // 회색
    "\033[91m", // 주황
    "\033[92m"  // 연두
};

// 콘솔 화면 지우기
void clear_screen()
{
    system("cls");
}

void set_console_font_size(int font_size)
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = font_size;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

// 실시간 순위 업데이트 함수
void real_time_ranking(Horse horses[], int num_horses)
{
    for (int i = 0; i < num_horses; i++)
    {
        horses[i].ranking = 1;
    }

    for (int i = 0; i < num_horses; i++)
    {
        for (int j = 0; j < num_horses; j++)
        {
            if (horses[i].position < horses[j].position)
            {
                horses[i].ranking++;
            }
        }
    }
}

// 최종 순위 업데이트
void last_ranking(Horse horses[], int num_horses, int* current_rank)
{
    for (int i = 0; i < num_horses; i++)
    {
        if (horses[i].position == RACE_DISTANCE && horses[i].last_rank == 0)
        {
            horses[i].last_rank = *current_rank;
            int count_same_rank = 1;

            for (int j = i + 1; j < num_horses; j++)
            {
                if (horses[j].position == RACE_DISTANCE && horses[j].last_rank == 0 && horses[j].position == horses[i].position)
                {
                    horses[j].last_rank = *current_rank;
                    count_same_rank++;
                }
            }

            (*current_rank)+= count_same_rank;
        }
    }
}

// 경주 상황 출력
void display_race(Horse horses[], int num_horses, int frame)
{
    clear_screen();
    set_console_font_size(24);
    printf("<start>       1        10        20       30        40       50        60        70       80(end)\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < num_horses; i++)
    {   
        printf("%s", horses[i].horse_name);
        for (int j = 0; j < horses[i].position; j++)
        {
            printf(" ");
        }
        printf("%s~%s/>\033[0m", colors[horses[i].color], horses[i].player_name);

        if (horses[i].last_rank > 0)
        {
            printf(" \033[37m[%d위 입니다!]\033[0m", horses[i].last_rank);
        }
        else
        {
            if (horses[i].position > 55)
            {
                printf("[누가 승리하게 될까요!]");
            }
            else
            {
                printf("[현재 %d위]", horses[i].ranking);
            }
        }
        printf("\n");

        printf("\033[30m%s\033[0m", horses[i].horse_name);

        for (int j = 0; j < horses[i].position; j++)
        {
            printf(" ");
        }
        printf("%s%s\033[0m\n\n", colors[horses[i].color], legs[frame % 9]);
    }
}

// 경주마 이동
void move_horses(Horse horses[], int num_horses)
{
    for (int i = 0; i < num_horses; i++)
    {
        if (horses[i].position < RACE_DISTANCE)
        {
            horses[i].position += rand() % 3;
            if (horses[i].position > RACE_DISTANCE)
            {
                horses[i].position = RACE_DISTANCE;
            }
        }
    }
}

// 경주 종료 확인
int is_race_over(Horse horses[], int num_horses)
{
    for (int i = 0; i < num_horses; i++)
    {
        if (horses[i].position < RACE_DISTANCE)
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    srand(time(0));

    int num_horses;
    char input[50];

    // 경주마 수 입력
    while (1)
    {
        printf("달리는 말의 수를 입력하세요 (1~%d): ", MAX_HORSES);
        fgets(input, sizeof(input), stdin);
        num_horses = atoi(input);

        if (num_horses >= 1 && num_horses <= MAX_HORSES)
        {
            break;
        }
        printf("유효하지 않은 입력입니다. 1에서 %d 사이의 숫자를 입력하세요.\n", MAX_HORSES);
    }

    Horse horses[MAX_HORSES];

    // 각 경주마 이름 입력
    for (int i = 0; i < num_horses; i++)
    {
        while (1)
        {
            printf("경주마 %d에 탈 플레이어 이름을 쓰세요(두 글자): ", i + 1);
            fgets(input, sizeof(input), stdin);

            input[strcspn(input, "\n")] = '\0';

            int len = strlen(input);
            int is_hangul = 1;

            if (len == 4)
            {
                for (int j = 0; j < len; j += 2)
                {
                    if ((unsigned char)input[j] < 0xB0 || (unsigned char)input[j] > 0xC8)
                    {
                        is_hangul = 0;
                        break;
                    }
                }
            }

            else
            {
                is_hangul = 0;
            }

            if (is_hangul)
            {
                strcpy(horses[i].player_name, input);
                break;
            }
            printf("유효하지 않은 입력입니다. 한글 두 글자를 입력하세요.\n");
        }

        sprintf(horses[i].horse_name, "Horse%d", i + 1);
        horses[i].position = 0;
        horses[i].ranking = 0;
        horses[i].last_rank = 0;
        horses[i].color = rand() % 10;
    }

    int current_rank = 1;
    int frame = 0;

    while (1)
    {
        real_time_ranking(horses, num_horses);
        last_ranking(horses, num_horses, &current_rank);
        display_race(horses, num_horses, frame);
        move_horses(horses, num_horses);
        Sleep(200);
        frame++;

        if (is_race_over(horses, num_horses))
        {
            break;
        }
    }
    real_time_ranking(horses, num_horses);
    last_ranking(horses, num_horses, &current_rank);
    display_race(horses, num_horses, frame);
    move_horses(horses, num_horses);
    Sleep(200);
    frame++;

    printf("\n<최종 순위>\n");
    for (int i = 1; i <= num_horses; i++)
    {
        for (int j = 0; j < num_horses; j++)
        {
            if (horses[j].last_rank == i)
            {
                printf("%d위: %s (플레이어: %s)\n", i, horses[j].horse_name, horses[j].player_name);
            }
        }
    }

    printf("\n경주가 종료되었습니다!\n");

    return 0;
}
