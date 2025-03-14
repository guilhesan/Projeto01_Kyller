# Projeto01_Kyller

# Simulação de Elevador com Threads e Semáforos

Este projeto simula o funcionamento de um elevador utilizando **threads**, **semáforos** e **mutexes** para gerenciar chamadas concorrentes de usuários. O código é desenvolvido para rodar em sistemas **Linux**.

## Como Funciona?

O sistema cria **usuários que fazem chamadas para o elevador**, solicitando transporte de um andar de origem para um andar de destino. O **elevador** processa as chamadas em ordem FIFO (First In, First Out) utilizando uma fila encadeada protegida por mutexes e um semáforo para sincronização.

### **Fluxo do Sistema**

1. Usuários geram chamadas aleatórias de um andar para outro.
2. As chamadas são armazenadas em uma fila encadeada.
3. O elevador verifica a fila e atende as chamadas na ordem de chegada.
4. O elevador se move para o andar de origem, espera o embarque, depois se move para o andar de destino e espera o desembarque.
5. O processo se repete até todas as chamadas serem atendidas.

## **Compilando e Executando**

Este projeto utiliza a biblioteca **pthread** e **semaphore**. Para compilar e executar o programa, siga os passos abaixo:

### **1. Compilar o código**

Abra um terminal na pasta do projeto e execute:

```bash
gcc -o elevador elevador.c -pthread -lrt
```

### **2. Executar o programa**

```bash
./elevador
```

## **Estrutura do Código**

O código está organizado da seguinte maneira:

- **Fila de chamadas**: Implementada como uma fila encadeada.
- **Controle de concorrência**: Uso de `pthread_mutex_t` para proteger o acesso à fila e `sem_t` para sinalizar chamadas pendentes.
- **Threads**:
  - Uma thread para o **elevador** que processa chamadas.
  - Várias threads representando **usuários** chamando o elevador.

## **Principais Funções**

- `void* user_thread(void* arg)`: Simula um usuário que chama o elevador de um andar aleatório para outro.
- `void enqueue(Call call)`: Adiciona uma chamada à fila de espera.
- `Call dequeue()`: Remove e retorna a próxima chamada da fila.
- `void* elevator_thread(void* arg)`: Implementa o funcionamento do elevador, processando chamadas na ordem de chegada.

## **Dependências**

Certifique-se de ter instalado em seu ambiente Linux:

- **GCC**
- **pthread** (Threading para POSIX)
- **librt** (Realtime library para semáforos e temporizadores)

Se necessário, instale os pacotes com:

```sh
sudo apt update && sudo apt install build-essential
```

Projeto desenvolvido por **Guilherme  Santos da Silveira e Ingrid Honorio da Silva**.

### 📌 Observação

Este código foi desenvolvido para execução em ambientes **Linux**.

---

**Licença:** Este projeto está sob a licença MIT.
