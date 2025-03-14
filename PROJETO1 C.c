// Guilherme Santos Da Silveira - 124 212 387.
// Ingrid Honorio da Silva - 118 210 380.
// https://youtu.be/_AjntSY2rRo


#include <stdio.h>    
#include <stdlib.h>     
#include <pthread.h>    // Biblioteca para manipulação de threads
#include <semaphore.h>  // Biblioteca para manipulação de semáforos
#include <unistd.h>     // Biblioteca para funções como sleep()

#define NUM_FLOORS 5  // Número de andares do prédio

int chamada = 0;  // Variável para indicar se o elevador está aguardando chamadas

// Estrutura que representa uma chamada do elevador
typedef struct {
    int start_floor;  // Andar de origem
    int end_floor;    // Andar de destino
} Call;

// Estrutura de um nó para a fila encadeada (fila de chamadas do elevador)
typedef struct Node {
    Call call;         // Chamada armazenada no nó
    struct Node* next; // Ponteiro para o próximo nó
} Node;

// Ponteiros para o início e o fim da fila de chamadas
Node* head = NULL;
Node* tail = NULL;

// Semáforo para controlar a fila de chamadas
sem_t queue_sem;

// Mutex para garantir acesso seguro à fila de chamadas
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;

// Função para adicionar uma chamada à fila
void enqueue(Call call) {
    Node* new_node = (Node*)malloc(sizeof(Node)); // Aloca memória para um novo nó
    new_node->call = call;   // Armazena a chamada
    new_node->next = NULL;   // O próximo nó é NULL pois será o último da fila

    pthread_mutex_lock(&queue_mutex); // Bloqueia a fila para acesso seguro
    if (tail == NULL) {  // Se a fila estiver vazia, o novo nó será o primeiro
        head = tail = new_node;
    } else {  // Caso contrário, adiciona o novo nó ao final da fila
        tail->next = new_node;
        tail = new_node;
    }
    pthread_mutex_unlock(&queue_mutex); // Libera a fila após a inserção
    sem_post(&queue_sem); // Incrementa o semáforo para indicar uma nova chamada
}

// Função para remover uma chamada da fila
Call dequeue() {
    pthread_mutex_lock(&queue_mutex); // Bloqueia a fila para acesso seguro
    Node* temp = head;  // Armazena o primeiro nó da fila
    Call call = temp->call; // Obtém a chamada do primeiro nó
    head = head->next; // Move o ponteiro da cabeça para o próximo nó
    if (head == NULL) { // Se a fila estiver vazia após a remoção, ajusta o tail
        tail = NULL;
    }
    pthread_mutex_unlock(&queue_mutex); // Libera a fila após a remoção
    free(temp); // Libera a memória do nó removido
    return call; // Retorna a chamada removida
}

// Função executada por cada usuário chamando o elevador
void* user_thread(void* arg) {
    int user_id = *((int*)arg); // Obtém o ID do usuário
    int start_floor = rand() % NUM_FLOORS; // Define um andar de origem aleatório
    int end_floor = rand() % NUM_FLOORS;   // Define um andar de destino aleatório
    while (end_floor == start_floor) {  // Garante que os andares não sejam iguais
        end_floor = rand() % NUM_FLOORS;
    }
    printf("[Usuário %d] Chamando o elevador do andar %d para o andar %d.\n", user_id, start_floor, end_floor);
    Call call = {start_floor, end_floor}; // Cria a chamada
    enqueue(call); // Adiciona a chamada à fila
    return NULL;
}

// Função executada pela thread do elevador
void* elevator_thread(void* arg) {
    int current_floor = 0; // O elevador começa no andar 0
    printf("[Elevador] Inicializado no andar %d. Aguardando chamadas...\n", current_floor);
    
    while (1) {  // Loop infinito para processar chamadas
        sem_wait(&queue_sem); // Aguarda até que haja uma chamada na fila
        Call call = dequeue(); // Remove a próxima chamada da fila

        // Se a fila estiver vazia após a remoção e não houver chamada pendente, aguarda
        if (head == NULL && chamada == 0) {
            printf("[Elevador] Aguardando nova chamada.\n");
            chamada = 1;
        } else {
            chamada = 0;
            printf("[Elevador] Nova chamada recebida: Andar %d -> %d.\n", call.start_floor, call.end_floor);
            
            // Movendo-se para o andar de origem
            if (current_floor != call.start_floor) {
                printf("[Elevador] Movendo-se para o andar %d...\n", call.start_floor);
                current_floor = call.start_floor;
            }
            printf("[Elevador] Chegou ao andar %d.\n", current_floor);
            printf("[Elevador] Aguardando o usuário embarcar.\n");
            sleep(2); // Simula o tempo de entrada do usuário

            // Movendo-se para o andar de destino
            printf("[Elevador] Movendo-se para o andar %d...\n", call.end_floor);
            current_floor = call.end_floor;
            printf("[Elevador] Chegou ao andar %d.\n", current_floor);
            printf("[Elevador] Aguardando o usuário desembarcar.\n");
            sleep(2); // Simula o tempo de saída do usuário
        }
    }
    return NULL;
}


int main() {
    srand(time(NULL)); // Inicializa a semente para gerar números aleatórios
    sem_init(&queue_sem, 0, 0); // Inicializa o semáforo da fila com valor 0

    pthread_t elevator; // Declara a thread do elevador
    pthread_create(&elevator, NULL, elevator_thread, NULL); // Cria a thread do elevador

    int user_id = 0; // Contador de usuários

    // Loop para criar 20 chamadas de usuários
    while (user_id < 20) {
        pthread_t user, user1; // Declara duas threads de usuários

        pthread_create(&user, NULL, user_thread, &user_id); // Cria a primeira thread de usuário
        user_id++; // Incrementa o contador

        pthread_create(&user1, NULL, user_thread, &user_id); // Cria a segunda thread de usuário
        user_id++; // Incrementa o contador

        pthread_detach(user);  // Permite que as threads sejam desalocadas automaticamente após terminarem
        pthread_detach(user1);

        sleep(1); // Pequeno atraso para simular chamadas acontecendo ao longo do tempo
    }

    pthread_join(elevator, NULL); // Aguarda a finalização da thread do elevador

    // Libera os recursos do semáforo e do mutex antes de encerrar
    sem_destroy(&queue_sem);
    pthread_mutex_destroy(&queue_mutex);

    return 0; 
}