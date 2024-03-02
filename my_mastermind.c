#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int code_check(char* str, char c, int flag) {
    int i, j;
    if(flag == 0) {
        for(i = 0; str[i]; i++) {
            if(str[i] == c) {
                return 1;
            }
        }
    }
    if(flag == 1) {
        for(i = 0; str[i]; i++) {
            for(j = 0; str[j]; j++) {
                if(str[i] == str[j] && i != j) {
                    return 1;
                }
                if(str[j] < 48 ) {
                    return 1;
                }
                if(str[i] > 56) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

char* code_create() {
    char* code = (char*)malloc(sizeof(char)* 5);
    code[4] = '\0';
    int i = 0;
    int num;
    while(i < 4) {
        num = rand()%10;
        if(code_check(code, (num+48), 0) == 0) {
            code[i] = (char)(num+48);
            i++;
        }
    }
    return code;
}

int my_strlen(char* str) {
    int len = 0;
    while(str[len]) {
        len++;
    }
    return len;
}

int my_strcmp(char* str1, char* str2) {
    if(my_strlen(str1) != my_strlen(str2)) {
        return -1;
    }
    for(int i = 0; str1[i]; i++) {
        if(str1[i] != str2[i]) {
            return (str1[i] - str2[i]);
        }
    }
    return 0;
}

int miss_p(char* str1, char* str2) {
    int result = 0, i, j;
    for(i=0; str1[i]; i++) {
        for(j=0; str2[j]; j++) {
            if(str1[i] == str2[j] && i != j) {
                result++;
            }
        }   
    }
    return result;
}

int well_p(char* str1, char* str2) {
    int result = 0, i, j;
    for(i=0; str1[i]; i++) {
        for(j=0; str2[j]; j++) {
            if(str1[i] == str2[j] && i == j) {
                result++;
            }
        }
    }
    return result;
}

char* reading() {
    char* code = (char*)malloc(sizeof(char)*5);
    code[0] = '\0';
    code[1] = '\0';
    code[2] = '\0';
    code[3] = '\0';
    code[4] = '\0';
    int i = 0;
    char c;
    while(read(0, &c, 1)) {
        if(i < 4) {
            code[i] = c;
        }
        if(c == '\n') {
            if(i > 4) {
                return "error";
            }
            return code;
        }
        i++;
    }
    return "quit";
}

int start_game(char* s_code, int round) {
    char* code = s_code;
    int i, flag = 0;
    printf("Will you find the secret code?\nPlease enter a valid guess\n");
    for(i = 0; i<round; i++) {
        printf("---\nRound %d\n",i);
        write(1, ">", 1);
        code = reading();
        if(my_strcmp(code, "quit") == 0) {
            return 1;
        }
        if(code_check(code, '\0', 1) == 1 || my_strcmp(code, "error") == 0)  {
            printf("Wrong input!\n");
            write(1, ">", 1);
            while(1) {
                code = reading();
                if(my_strcmp(code, "quit") == 0) {
                    return 1;
                }
                if(code_check(code, '\0', 1) == 1 || my_strcmp(code, "error") == 0) {
                    printf("Wrong input!\n");
                    write(1, ">", 1);
                    continue;
                }
                if(code_check(code, '\0', 1) == 0) {
                    if(well_p(code, s_code) == 4) {
                        printf("Congratz! You did it!");
                        return 0;
                    }
                    flag = 1;
                    printf("Well placed pieces: %d\n", well_p(s_code, code));
                    printf("Misplaced pieces: %d\n", miss_p(s_code, code));
                    break;
                }
            }
        }
        if(code_check(code, '\0', 1) == 0 && flag == 0) {
            if(well_p(code, s_code) == 4) {
                printf("Congratz! You did it!");
                return 0;
            }
            printf("Well placed pieces: %d\n", well_p(s_code, code));
            printf("Misplaced pieces: %d\n", miss_p(s_code, code));
        }
        flag = 0;
    }
    printf("GAME OVER\n");
    return 1;
}

int main(int ac, char** av) {
    int round = 10;
    char* secret_code = NULL;
    int i;
    if(ac > 1) {
        for(i = 0; av[i]; i++) {
            if(my_strcmp("-c", av[i]) == 0) {
                if(code_check(av[i+1], '\0', 1) == 0) {
                    secret_code = av[i+1];
                    i+=2; 
                }
                else {
                    printf("wrong code input\n");
                    return 1;
                }
            }
            if(i >= ac) {
                break;
            }
            if(my_strcmp("-t", av[i]) == 0) {
                round = atoi(av[i+1]);
                i+=2;
            }
        }
    }
    if(secret_code == NULL) {
        secret_code = code_create();
    }
    if(start_game(secret_code, round) == 1) {
        return 1;
    }
    return 0;
}
