#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#define DEFAULT_PORT 5050
#define BUFFER_SIZE 1024
#define MAX_PENDING_CONNECTIONS 5

// Variável global para o socket de escuta, para que o handler de sinal possa acessá-lo.
int listen_fd = -1;

// Função para encerrar o programa de forma limpa
void die(const char *message) {
    perror(message);
    if (listen_fd != -1) {
        close(listen_fd);
    }
    exit(EXIT_FAILURE);
}

// Handler para o sinal SIGINT (Ctrl+C)
void sigint_handler(int sig) {
    (void)sig; // Evita warning de variável não utilizada
    printf("\n[SERVER] Recebido sinal de interrupção. Encerrando...\n");
    if (listen_fd != -1) {
        close(listen_fd);
    }
    exit(EXIT_SUCCESS);
}

// Processa a requisição, calcula e preenche a resposta
void process_request(const char *request, char *response) {
    char command[10];
    double a, b;

    // Tenta parsear a entrada no formato "OP A B"
    int items_scanned = sscanf(request, "%s %lf %lf", command, &a, &b);

    if (items_scanned != 3) {
        sprintf(response, "ERR EINV comando_invalido_ou_formato_incorreto\n");
        return;
    }

    double result;
    if (strcmp(command, "ADD") == 0) {
        result = a + b;
    } else if (strcmp(command, "SUB") == 0) {
        result = a - b;
    } else if (strcmp(command, "MUL") == 0) {
        result = a * b;
    } else if (strcmp(command, "DIV") == 0) {
        if (b == 0) {
            sprintf(response, "ERR EZDV divisao_por_zero\n");
            return;
        }
        result = a / b;
    } else {
        sprintf(response, "ERR EINV operacao_desconhecida\n");
        return;
    }

    // Formata a resposta de sucesso com 6 casas decimais
    sprintf(response, "OK %.6f\n", result);
}

// Lida com a conexão de um cliente
void handle_connection(int client_fd, struct sockaddr_in *client_addr) {
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    char client_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &client_addr->sin_addr, client_ip, sizeof(client_ip));
    printf("[SERVER] Conexão aceita de %s:%d\n", client_ip, ntohs(client_addr->sin_port));

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_received <= 0) {
            if (bytes_received < 0) {
                perror("[SERVER] Erro no recv");
            }
            printf("[SERVER] Cliente %s:%d desconectado.\n", client_ip, ntohs(client_addr->sin_port));
            break;
        }

        // Remove o \n do final, se houver, para o strcmp
        buffer[strcspn(buffer, "\r\n")] = 0;
        printf("[SERVER] Recebido de %s: %s\n", client_ip, buffer);

        // Verifica se o cliente quer sair
        if (strcmp(buffer, "QUIT") == 0) {
            printf("[SERVER] Cliente %s solicitou desconexão.\n", client_ip);
            break;
        }

        process_request(buffer, response);

        // Envia a resposta de volta ao cliente
        send(client_fd, response, strlen(response), 0);
    }

    close(client_fd);
}

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;

    // Permite alterar a porta via linha de comando
    if (argc > 1) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Porta inválida: %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    }

    // Configura o handler de sinal para Ctrl+C
    signal(SIGINT, sigint_handler);

    // 1. Criar o socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        die("Falha ao criar socket");
    }

    // Permite reusar o endereço imediatamente após o servidor ser fechado
    int opt = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        die("Falha em setsockopt");
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Escuta em todas as interfaces
    server_addr.sin_port = htons(port);

    // 2. Vincular o socket ao endereço e porta
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        die("Falha no bind");
    }

    // 3. Colocar o socket em modo de escuta
    if (listen(listen_fd, MAX_PENDING_CONNECTIONS) < 0) {
        die("Falha no listen");
    }

    printf("[SERVER] Aguardando conexões na porta %d...\n", port);

    // 4. Loop principal para aceitar conexões
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            // Se o erro for EINTR, foi causado pelo nosso handler de sinal, podemos ignorar.
            if (errno == EINTR) continue; 
            perror("Falha no accept");
            continue; // Tenta o próximo
        }

        // Lida com o cliente (servidor iterativo: um por vez)
        handle_connection(client_fd, &client_addr);
    }

    close(listen_fd);
    return EXIT_SUCCESS;
}
