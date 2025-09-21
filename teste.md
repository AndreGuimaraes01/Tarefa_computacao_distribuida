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
