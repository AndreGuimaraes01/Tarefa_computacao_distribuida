# Tarefa_computacao_distribuida

Calculadora Cliente-Servidor com Sockets em C
Visão Geral
Este projeto implementa uma aplicação cliente-servidor de calculadora simples em C, utilizando sockets TCP sobre IPv4. O objetivo é demonstrar os conceitos fundamentais de programação de redes, incluindo a criação de sockets, comunicação baseada em texto, parsing de mensagens e tratamento de erros.

Servidor (server): Aguarda conexões, recebe operações matemáticas, as processa e retorna o resultado.

Cliente (client): Conecta-se ao servidor, envia operações lidas da linha de comando e exibe a resposta recebida.

Como Compilar e Executar
Pré-requisitos
Compilador C (GCC)

Utilitário make

Compilação
Para compilar tanto o cliente quanto o servidor, execute o seguinte comando na raiz do projeto:

make

Isso gerará dois executáveis: server e client.

Você também pode compilar os programas individualmente:

make server
make client

Para limpar os arquivos compilados:

make clean

Execução
Inicie o Servidor
Abra um terminal e inicie o servidor. Por padrão, ele escutará na porta 5050.

./server

Para especificar uma porta diferente (ex: 6000):

./server 6000

Inicie o Cliente
Abra outro terminal e conecte o cliente ao servidor, especificando o endereço IP e a porta.

./client 127.0.0.1 5050

Após a conexão, você pode começar a digitar as operações.

Formato do Protocolo
A comunicação entre cliente e servidor segue um protocolo de texto simples, baseado em linhas.

Requisição do Cliente
O cliente envia uma mensagem no formato OP A B\n, onde:

OP: A operação a ser realizada. Valores possíveis: ADD, SUB, MUL, DIV.

A, B: Operandos numéricos (ponto flutuante).

\n: Caractere de nova linha, que finaliza a mensagem.

Resposta do Servidor
Sucesso: OK R\n

R: O resultado da operação, formatado com 6 casas decimais.

Erro: ERR <COD> <mensagem>\n

<COD>: Um código de erro.

<mensagem>: Uma breve descrição do erro.

Códigos de Erro
EINV: Entrada inválida (formato incorreto, operação desconhecida).

EZDV: Divisão por zero.

ESRV: Erro interno do servidor.

Exemplos de Uso
A seguir, um exemplo de sessão no terminal do cliente:

# Exemplo de sessão no cliente
> ADD 10 2
OK 12.000000

> SUB 7.5 9
OK -1.500000

> MUL -3 3.5
OK -10.500000

> DIV 5 0
ERR EZDV divisao_por_zero

> DIV 10 3
OK 3.333333

> operacao_invalida 1 2
ERR EINV comando_invalido_ou_formato_incorreto

> QUIT
[INFO] Desconectando.

Para encerrar a sessão, digite QUIT.

Decisões de Projeto e Limitações
Servidor Iterativo: O servidor implementado é "iterativo", ou seja, ele atende um cliente por vez. Uma nova conexão só será aceita após a anterior ser encerrada. Esta é uma abordagem mais simples, ideal para fins didáticos, mas não é adequada para ambientes com muitos acessos simultâneos.

Buffer de Tamanho Fixo: A leitura e escrita de dados utilizam um buffer de tamanho fixo (1024 bytes). Mensagens maiores que isso não serão tratadas corretamente. Para este protocolo simples, o tamanho é mais que suficiente.

Parsing: O parsing das mensagens de requisição é feito com sscanf, que é uma forma simples e eficaz de extrair tokens de uma string formatada. A robustez é garantida pela verificação do valor de retorno de sscanf.

Tratamento de Sinais: O servidor foi configurado para encerrar de forma limpa ao receber um sinal SIGINT (Ctrl+C), garantindo que o socket de escuta seja fechado corretamente.
