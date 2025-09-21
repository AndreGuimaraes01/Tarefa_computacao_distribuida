#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void die(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <IP do Servidor> <Porta>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);

    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Porta inválida: %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // 1. Criar o socket do cliente
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        die("Falha ao criar socket");
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Converte o endereço IP de string para formato de rede
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Endereço IP inválido: %s\n", server_ip);
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 2. Conectar ao servidor
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        die("Falha ao conectar ao servidor");
    }

    printf("[INFO] Conectado a %s:%d. Digite 'QUIT' para sair.\n", server_ip, port);

    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];

    // Loop principal para ler entrada do usuário e comunicar com o servidor
    while (1) {
        printf("> ");
        // Lê uma linha do stdin (teclado)
        if (fgets(send_buffer, BUFFER_SIZE, stdin) == NULL) {
            // Aconteceu um EOF (Ctrl+D)
            printf("\n[INFO] Fim da entrada. Enviando QUIT...\n");
            strcpy(send_buffer, "QUIT\n");
        }

        // Envia a mensagem para o servidor
        if (send(sock_fd, send_buffer, strlen(send_buffer), 0) < 0) {
            perror("Erro ao enviar mensagem");
            break;
        }

        // Verifica se o comando é para sair ANTES de esperar a resposta
        if (strncmp(send_buffer, "QUIT", 4) == 0) {
             printf("[INFO] Desconectando.\n");
             break;
        }
        
        // Recebe a resposta do servidor
        memset(recv_buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(sock_fd, recv_buffer, BUFFER_SIZE - 1, 0);

        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                printf("[INFO] Servidor encerrou a conexão.\n");
            } else {
                perror("Erro ao receber resposta");
            }
            break;
        }
        
        // Imprime a resposta
        printf("%s", recv_buffer);
    }

    // 3. Fecha o socket
    close(sock_fd);
    return EXIT_SUCCESS;
}
