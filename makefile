Compilador C a ser usado
CC = gcc

# Flags de compilação:
# -Wall: Habilita a maioria dos warnings
# -Wextra: Habilita warnings extras
# -std=c11: Usa o padrão C11
# -O2: Otimização de nível 2
# -Iinclude: Adiciona o diretório 'include' para busca de headers
# -g: Gera informações de debug
CFLAGS = -Wall -Wextra -std=c11 -O2 -g

# Diretórios
SRC_DIR = src
BIN_DIR = .

# Arquivos fonte
SERVER_SRC = $(SRC_DIR)/server.c
CLIENT_SRC = $(SRC_DIR)/client.c

# Executáveis
SERVER_BIN = $(BIN_DIR)/server
CLIENT_BIN = $(BIN_DIR)/client

# Target padrão: compila tudo
all: $(SERVER_BIN) $(CLIENT_BIN)

# Target para compilar o servidor
server: $(SERVER_BIN)

# Target para compilar o cliente
client: $(CLIENT_BIN)

# Regra para criar o executável do servidor
$(SERVER_BIN): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_SRC)

# Regra para criar o executável do cliente
$(CLIENT_BIN): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC)

# Target para limpar os arquivos compilados e executáveis
clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)

# Phony targets não correspondem a nomes de arquivos
.PHONY: all server client clean
