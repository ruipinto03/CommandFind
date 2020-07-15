#include <semaphore.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>


#define PRODUCERS 1
#define CONSUMERS 1
#define SHELF 10

sem_t semaphore_producer;
sem_t semaphore_consumer;

pthread_mutex_t mutex_producer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_consumer = PTHREAD_MUTEX_INITIALIZER;

/*Variaveis globais;*/

double GB = 1073741824;
double MB = 1048576;
double KB = 1024;
int MIN = 60;
char **shelf;
char bomb[] = "Puff ... ";

int pointer_consumer_position = 0;
int pointer_producer_position = 0;

/*STRUCT*/

typedef struct path {
    char *path;
} PATH;

typedef struct match {
    pthread_t tid; // identificador do thread
    char **match; // matriz para guardar os caminhos encontrados por um thread
    int number_matches;
    struct match *next;
} MATCH;

typedef struct list_match {
    int number_threads; // numero de threads lançados
    int number_match; // numero de match encontrados
    MATCH *first;
} LMATCH;

typedef int (*PARAM)(struct dirent *entry, char *value, char *path);

typedef struct arg {
    PARAM opt;
    char *value;
} ARG;

ARG args[5];

int n_args;

LMATCH match_list = {.first=NULL, .number_match=0, .number_threads=0};

pthread_mutex_t mutex_insertion = PTHREAD_MUTEX_INITIALIZER;


char *upper_string(char *string) {
    char *aux = (char *) malloc(sizeof(char) * strlen(string));
    for (int i = 0; string[i] != '\0'; i++) {
        aux[i] = toupper(string[i]);
    }
    return aux;
}


int name(struct dirent *entry, char *value, char *path) {

    char *value_last_occurrence = NULL;

    char *entry_last_occurrence = NULL;

    // printf("%s \n", value);

    //printf("Find by name: %s\n", value);

    if (value[0] == '*') {

        char character = value[1];

        if ((strrchr(value, character)) != NULL && strrchr(entry->d_name, character) != NULL) {

            value_last_occurrence = strrchr(value, character);

            entry_last_occurrence = strrchr(entry->d_name, character);

            if ((strcmp(value_last_occurrence, entry_last_occurrence) == 0)) {

                return 1;

            } else {

                return 0;

            }

        }

    }

    if ((strstr(entry->d_name, value)) != NULL) {

        return 1;

    }

    return 0;
}

int type(struct dirent *entry, char *value, char *path) {

    if (strcmp("d", value) == 0) {

        if (entry->d_type == DT_DIR) {

            return 1;

        } else {
            return 0;
        }

    } else if (strcmp("f", value) == 0) {

        if (entry->d_type == DT_REG) {

            return 1;

        } else {
            return 0;
        }

    }

    return 0;
}

int iname(struct dirent *entry, char *value, char *path) {

    char *value_last_occurrence = NULL;

    char *entry_last_occurrence = NULL;

    char *upper_value = upper_string(value);

    char *upper_entry = upper_string(entry->d_name);

    // printf("%s \n", value);

    //printf("Find by name: %s\n", value);

    if (value[0] == '*') {

        char character = value[1];

        if ((strrchr(upper_value, character)) != NULL && strrchr(upper_entry, character) != NULL) {

            value_last_occurrence = strrchr(upper_value, character);

            entry_last_occurrence = strrchr(upper_entry, character);
            if ((strcmp(value_last_occurrence, entry_last_occurrence) == 0)) {

                return 1;

            } else {

                return 0;

            }
        }


    }


    if ((strcmp(upper_entry, upper_value)) == 0) {

        return 1;

    }

    return 0;

}

int size(struct dirent *entry, char *value, char *path) {

    struct stat status;

    if (stat(path, &status) != 0) {

        perror("Status:");

        exit(EXIT_FAILURE);

    }

    double result = 0;

    char signal = value[0];

    char type_size[2];

    int aux = 0;

    for (int i = 1; value[i] != '\0'; i++) {

        if (value[i] >= '0' && value[i] <= '9') {

            result = (result * 10) + (value[i] - '0');

        } else {

            type_size[aux] = toupper(value[i]);

            aux++;
        }

    }

    if (strcmp(type_size, "KB") == 0) {

        if (signal == '+' && (status.st_size / KB) > result) {

            return 1;

        } else if (signal == '-' && (status.st_size / KB) < result) {

            return 1;

        }

    } else if (strcmp(type_size, "MB") == 0) {

        if (signal == '+' && (status.st_size / MB) > result) {

            return 1;

        } else if (signal == '-' && (status.st_size / MB) < result) {

            return 1;

        }

    } else if (strcmp(type_size, "B") == 0) {

        if (status.st_size > result) {

            return 1;

        } else if (status.st_size < result) {

            return 1;

        }

    }

    return 0;

}

int empty(struct dirent *entry, char *value, char *path) {

    struct stat status;

    struct dirent *new_entry;

    char *newPath = (char *) malloc(sizeof(char) * strlen(path));

    int i = 0;

    DIR *dir;

    // printf("%s \n",path);


    if (stat(path, &status) != 0) {
        perror("Status:");
        exit(EXIT_FAILURE);
    }

    if (S_ISDIR(status.st_mode)) {
        // printf("%s \n",newPath);
        sprintf(newPath, "%s/", path);
        if ((dir = opendir(newPath)) == NULL) {

            perror("DIR:");

            exit(EXIT_FAILURE);

        }

        while ((new_entry = readdir(dir)) != NULL) {
            i++;
        }
        // printf("%d\n", i);
        if (i > 2) {

            return 0;

        } else {
            return 1;
        }
    }


    if (status.st_size == 0) {

        return 1;

    } else {

        return 0;

    }

}

int executable(struct dirent *entry, char *value, char *path) {
    //printf("Find by iname: %s\n", value);
    char *newPath = (char *) malloc(sizeof(char) * strlen(path));
    sprintf(newPath, "%s/", path);
    struct stat status;
    if (stat(path, &status) != 0) {
        perror("Status:");
        exit(EXIT_FAILURE);
    }
    int result;
    if (access(newPath, F_OK)) {
        printf("%d\n", result);
        return 1;
    }

    if ((strcmp((status.st_mode & S_IXUSR) ? "x" : "-", "x") == 0 ||
         strcmp((status.st_mode & S_IXGRP) ? "x" : "-", "x") == 0 ||
         strcmp((status.st_mode & S_IXOTH) ? "x" : "-", "x") == 0) && !S_ISDIR(status.st_mode)) {
        return 1;
    }
    return 0;

    return 0;
}

int mmin(struct dirent *entry, char *value, char *path) {
    //printf("Find by iname: %s\n", value);
    int min = atoi(value);

    struct stat status;

    if (stat(path, &status) != 0) {

        perror("Status:");

        exit(EXIT_FAILURE);

    }


    int time_aux = (status.st_mtim.tv_sec / 60);
    time_t seconds = (time(NULL) / 60);
    int result = seconds - time_aux;
    //  printf("%s ->%d\n",path ,result);
    if (min >= result) {

        return 1;

    }

    return 0; // returntimespec 1 if match found
}

const static struct {
    const char *name;
    PARAM opt;
} function_map[] = {
        {"-name",       (PARAM) name},
        {"-iname",      (PARAM) iname},
        {"-type",       (PARAM) type},
        {"-size",       (PARAM) size},
        {"-mmin",       (PARAM) mmin},
        {"-executable", (PARAM) executable},
        {"-empty",      (PARAM) empty},
};

int find_func_id(char *arg) {
    for (int i = 0; i < (sizeof(function_map) / sizeof(function_map[0])); i++) { //  i = 0 to 6 in this case

        if (!strcmp(function_map[i].name, arg) && function_map[i].opt) { // this compare arg with name of the function

            return i; // returns the index of function

        }

    }

    return -1;

}


void insert_match(char **matrix, pthread_t thread, int number_match) {
    MATCH *new = (MATCH *) malloc(sizeof(MATCH));
    new->tid = thread;
    new->match = matrix;
    new->number_matches += number_match;
    new->next = NULL;

    if (match_list.number_threads == 0) {
        match_list.first = new;
        match_list.number_threads += 1;
        match_list.number_match += number_match;
        return;
    }
    MATCH *temp = match_list.first;
    match_list.first = new;
    new->next = temp;
    match_list.number_threads += 1;
    match_list.number_match += number_match;
}

void parse(int argc, char **argv) {

    char *path; // array for save da path sended by arg

    int index = 0; // index of function

    for (int i = 2; i < argc; i++) { // change to i = 2

        index = find_func_id(argv[i]);

        if (index != -1) {

            args[n_args].opt = function_map[index].opt;

            if ((strcmp(argv[i], "-empty") == 0) || (strcmp(argv[i], "-executable") == 0)) {

                args[n_args].value = NULL;

                //  printf("%s\n",args[n_args].value);

                n_args++;
            } else if ((strcmp(argv[i], "-name") == 0) || (strcmp(argv[i], "-iname") == 0) ||

                       (strcmp(argv[i], "-type") == 0) || (strcmp(argv[i], "-mmin") == 0) ||

                       (strcmp(argv[i], "-size") == 0)) {

                args[n_args].value = argv[i + 1];

                //    printf("%s\n",args[n_args].value);

                n_args++;

            }

        }

    }

}

void parse1(int argc, char **argv) {

    char *path; // array for save da path sended by arg

    int index = 0; // index of function

    for (int i = 2; i < argc; i++) { // change to i = 2

        index = find_func_id(argv[i]);

        if (index != -1) {

            args[n_args].opt = function_map[index].opt;

            if ((strcmp(argv[i], "-empty") == 0) || (strcmp(argv[i], "-executable") == 0)) {

                args[n_args].value = NULL;

                //  printf("%s\n",args[n_args].value);

                n_args++;
            } else if ((strcmp(argv[i], "-name") == 0) || (strcmp(argv[i], "-iname") == 0) ||

                       (strcmp(argv[i], "-type") == 0) || (strcmp(argv[i], "-mmin") == 0) ||

                       (strcmp(argv[i], "-size") == 0)) {

                args[n_args].value = argv[i + 1];

                //    printf("%s\n",args[n_args].value);

                n_args++;

            }

        }

    }

}


void print_match_list() {
    if (match_list.number_match == 0) {
        printf("Were not found any match!\n");
        return;
    }
    printf("Were created %d \n ", match_list.number_threads);

    MATCH *temp = match_list.first;

    while (temp != NULL) { // cycle to scroll through the threads list.

        printf("\nThread id -> %lu , number of path found %d\n", temp->tid, temp->number_matches);

        for (int i = 0; i < temp->number_matches; i++) { // cycle to scroll through the path list

            printf("%d - PATH : %s\n", i, temp->match[i]);

        }

        temp = temp->next;

    }

}

void print_args_list() {

    for (int i = 0; i < n_args; i++) {

        if (args[i].value != NULL) {

            printf("arg: %d , %s\n", i, args[i].value);

        } else {

            printf("arg: %d , %d\n", i, i);

        }

    }

}

void print_matriz() {
    for (int i = 0; i < SHELF; i++) {
        printf("%d -> %s\n", i, shelf[i]);
    }
}

void to_producer(char *path) {

    printf("TO PRODUCER PATH -> %s\n", path);

    struct stat status;

    struct dirent *entry;

    DIR *dir;

    if ((dir = opendir(path)) == NULL) {
        printf("Erro when try open Dir on function to_producer -> %s", path);
        exit(EXIT_FAILURE);
    }


    while ((entry = readdir(dir)) != NULL) {

        char *new_path = (char *) malloc(sizeof(char) * (strlen(entry->d_name) + strlen(path)));

        if ((strcmp(".", entry->d_name) != 0) && (strcmp("..", entry->d_name) != 0)) {

            sprintf(new_path, "%s%s", path, entry->d_name);

            if (stat(new_path, &status) == 0) {

                if ((S_ISDIR(status.st_mode))) {
                    printf("STRCAT BEFORE -> %s \n", new_path);
                    strcat(new_path, "/");
                    printf("STRCAT AFTER -> %s \n", new_path);


                    sem_wait(&semaphore_producer);

                    pthread_mutex_lock(&mutex_producer);

                    //        free(shelf[pointer_producer_position]);


                    shelf[pointer_producer_position] = malloc(sizeof(char) * strlen(new_path)); // erro aqui coloca um E

                   // memset(shelf[pointer_producer_position],strlen(new_path),sizeof(char));

                   // strcpy(shelf[pointer_producer_position], new_path);

                    sprintf(shelf[pointer_producer_position], "%s", new_path);

                    //        print_matriz();

                    pointer_producer_position = (pointer_producer_position + 1) % SHELF;

                    pthread_mutex_unlock(&mutex_producer);

                    sem_post(&semaphore_consumer);

                    print_matriz();

                    to_producer(new_path);

                }

            }
            strcpy(new_path, "");

        }

    }
    closedir(dir);

}

void *producer(void *param) {

    PATH *data_path = (PATH *) param;

    printf("\nDATA PATH -> %s\n\n", data_path->path);

    sem_wait(&semaphore_producer);

    pthread_mutex_lock(&mutex_producer);

    printf("PaTH PRODUCER -> %s\n\n", data_path->path);

    shelf[pointer_producer_position] = (char *) malloc(sizeof(char) * strlen(data_path->path));

    strcpy(shelf[pointer_producer_position], data_path->path);

    pointer_producer_position = (pointer_producer_position + 1) % SHELF;

    pthread_mutex_unlock(&mutex_producer);

    sem_post(&semaphore_consumer);

    print_matriz();

    to_producer(data_path->path);

    for (int i = 0; i < PRODUCERS; i++) {

        sem_wait(&semaphore_producer);

        pthread_mutex_lock(&mutex_producer);

        shelf[pointer_producer_position] = (char *) malloc(sizeof(char) * strlen(bomb));

        strcpy(shelf[pointer_producer_position], bomb);

        pointer_producer_position = (pointer_producer_position + 1) % SHELF;

        pthread_mutex_unlock(&mutex_producer);

        sem_post(&semaphore_consumer);

    }
    pthread_exit(NULL);
}

void to_consumer(char *path) {

    if (strcmp(path, bomb) == 0) {
        pthread_exit(NULL);
    }

    int x = 0; // variavel de ciclo

    int limit_matrix = 30; // limite para resize de matrix de match

    int position_matrix = 0;

    char **matrix_match = (char **) malloc(sizeof(char *) * limit_matrix);

    struct dirent *entry;

    struct stat status;

    DIR *dir;

    if ((dir = opendir(path)) == NULL) {

        printf("Erro when try open Dir on function to_consumer -> %s", path);

        exit(EXIT_FAILURE);

    }

    char *current_path;

    while ((entry = (readdir(dir))) != NULL) { // enquanto o dir nao estiver vazio

        if ((strcmp(".", entry->d_name) != 0) && (strcmp("..", entry->d_name) != 0)) { //verifica senao é as pastas que existem sempre

            current_path = (char *) malloc(sizeof(char) * strlen(entry->d_name) + strlen(path) + 1); // aloca memoria para o novo path

            sprintf(current_path, "%s%s", path, entry->d_name); // concatena o antigo path com o entry name

            if (stat(current_path, &status) == 0) { // passa os dados de current_path -> &status

                for (x = 0; x < n_args; x++) { // verificacao de argumentos passados ao programa

                    if (!args[x].opt(entry, args[x].value, current_path)) {

                        break;

                    }

                }

                if (x == n_args) { // caso o numero de args seja igual a x é porque foi encontrado match

                    if (limit_matrix / 2 == position_matrix) {

                        matrix_match = realloc(matrix_match, limit_matrix * 2);

                        limit_matrix *= 2;

                    }

                    matrix_match[position_matrix] = (char *) malloc(sizeof(char) * strlen(current_path));

                    strcpy(matrix_match[position_matrix], current_path);

                    position_matrix += 1;

                }

            }

        }

    }

    if (position_matrix != 0) { // Grava a matriz com os resultados

        pthread_mutex_lock(&mutex_insertion); // fecha o trinco para puder inserir devido a concorrencia a mesma estrutura
/*

        printf("%lu\n", pthread_self());

        for (int i = 0; i < position_matrix; i++) {

            printf("%s\n", matrix_match[i]);

        }
*/

        insert_match(matrix_match, pthread_self(), position_matrix);

        pthread_mutex_unlock(&mutex_insertion);
    }


}

void *consumer() {

    char *path_consumer;

    while (1) {

        sem_wait(&semaphore_consumer); // decrementa o semaforo do consumidor

        pthread_mutex_lock(&mutex_consumer); // fecha o trinco do consumidor

        path_consumer = (char *) malloc(sizeof(char) *
                                        strlen(shelf[pointer_consumer_position])); // aloca memoria para o caminho que ele tem que varrer a verificar os ficheiros

        strcpy(path_consumer, shelf[pointer_consumer_position]); // copia o caminho para a variavel path_consumer
        strcpy(shelf[pointer_consumer_position], ""); // copia o caminho para a variavel path_consumer

        shelf[pointer_consumer_position] = NULL; // coloca o valor a null que estava a ser ocupado pelo o path

        pointer_consumer_position = (pointer_consumer_position + 1) % SHELF; // incrementa para a proxima posicao a ser consumida

        pthread_mutex_unlock(&mutex_consumer); // abre o trinco do consumidor

        sem_post(&semaphore_producer); // incrementa o semaforo do produtor

        to_consumer(path_consumer); // envia o path para ser consumido

    }

}


int main(int argc, char **argv) {

    shelf = (char **) malloc(sizeof(char *) * SHELF);

    pthread_t thread_producer; // declaracao de um thread para o

    pthread_t thread_consumers[CONSUMERS];

    parse(argc, argv);

    print_args_list();

    PATH *new = (PATH *) malloc(sizeof(PATH *));

    char work_directory[1024];
/*       printf("\n%s\n\n", argv[1]);
    for (int i = 0; i < argc; i++)
        printf("argv %s\n", argv[i]);*/

    if (strcmp(argv[1], ".") == 0) {

        getcwd(work_directory, sizeof(work_directory));

        new->path = (char *) malloc(sizeof(char) * strlen(work_directory)); // change 2 to 1

        strcpy(new->path, work_directory);

        strcat(new->path, "/");

        //       printf("\n%s\n\n", new->path);

    } else {

        new->path = (char *) malloc(sizeof(char) + strlen(argv[1]));

        strcpy(new->path, argv[1]);

        // strcat(new->path,"/");

    }


    printf("PATH INICIAL -> %s\n", new->path);

    sem_init(&semaphore_producer, 0, SHELF);

    sem_init(&semaphore_consumer, 0, 0);

    pthread_create(&thread_producer, NULL, &producer, new);

    for (int i = 0; i < CONSUMERS; i++) {
        pthread_create(&thread_consumers[i], NULL, &consumer, NULL);
    }

    for (int i = 0; i < CONSUMERS; i++) {
        //      pthread_join(thread_consumers[i],NULL);
    }

    pthread_join(thread_producer, NULL);

    print_match_list();
    exit(0);

}