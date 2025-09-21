# Calculadora Cliente-Servidor com Sockets em C

**Aluno 1:** [Seu Nome Completo]  
**Matrícula:** [Sua Matrícula]  
**Aluno 2:** [Nome Completo do Colega]  
**Matrícula:** [Matrícula do Colega]

## Visão Geral

Este projeto implementa uma aplicação cliente-servidor de calculadora simples em C, utilizando sockets TCP sobre IPv4. O objetivo é demonstrar os conceitos fundamentais de programação de redes, incluindo a criação de sockets, comunicação baseada em texto, parsing de mensagens e tratamento de erros.

### Servidor (server):  
Aguarda conexões, recebe operações matemáticas, as processa e retorna o resultado.

### Cliente (client):  
Conecta-se ao servidor, envia operações lidas da linha de comando e exibe a resposta recebida.

## Como Compilar e Executar

### Pré-requisitos

- Compilador C (GCC)
- Utilitário `make`

### Compilação

Para compilar tanto o cliente quanto o servidor, execute o seguinte comando na raiz do projeto:

```bash
make

# Execução

## Inicie o Servidor

Abra um terminal e inicie o servidor na porta padrão 5050:
./server


## Inicie o Cliente

Abra outro terminal e conecte o cliente ao servidor, passando o endereço IP e a porta como parâmetros:

./client 127.0.0.1 5050


# Exemplos de Uso

A seguir, um exemplo de sessão no terminal do cliente, com as respectivas operações e respostas:

ADD 100 50
OK 150.000000

DIV 10 0
ERR EZDV divisao_por_zero

operacao_invalida 1 2
ERR EINV comando_invalido_ou_formato_incorreto

QUIT
[INFO] Desconectando.


## Explicação das Operações

- **ADD 100 50**: Executa a soma de 100 e 50, retornando o valor `150.000000`.
- **DIV 10 0**: Tenta dividir por zero, gerando um erro com o código `EZDV` e a mensagem `divisao_por_zero`.
- **operacao_invalida 1 2**: Comando inválido, retorna erro `EINV` e a mensagem `comando_invalido_ou_formato_incorreto`.
- **QUIT**: Encerra a conexão do cliente com o servidor e imprime a mensagem `[INFO] Desconectando`.

# Decisões de Projeto e Limitações

## Servidor Iterativo

O servidor atende um cliente por vez. Para suportar múltiplos clientes simultaneamente, seria necessário usar `fork()` ou threads, o que não foi implementado nesta versão.

## Parsing com `sscanf`

Utilizamos `sscanf` para interpretar as operações enviadas pelo cliente, devido à sua simplicidade e adequação ao protocolo. Há uma verificação de retorno para garantir que o parsing seja feito corretamente e de forma robusta.

## Tratamento de Sinais

O servidor encerra de forma limpa ao receber o sinal `SIGINT` (Ctrl+C), fechando corretamente o socket e garantindo que os recursos sejam liberados de maneira adequada.

# Limitações Conhecidas

- O sistema não implementa validação de entrada além do parsing de operações simples.
- A comunicação é limitada ao protocolo simples de strings, sem suporte a formatação complexa ou criptografia.
- O servidor não lida com múltiplos clientes de forma concorrente, o que significa que apenas um cliente pode ser atendido de cada vez.

