Calculadora Cliente-Servidor com Sockets em CAluno 1: [Seu Nome Completo]Matrícula: [Sua Matrícula]Aluno 2: [Nome Completo do Colega]Matrícula: [Matrícula do Colega]1. Visão GeralEste projeto implementa uma aplicação cliente-servidor de calculadora simples em C, utilizando sockets TCP sobre IPv4. O objetivo é demonstrar os conceitos fundamentais de programação de redes, incluindo a criação de sockets, comunicação baseada em texto, parsing de mensagens e tratamento de erros.Servidor (server): Aguarda conexões, recebe operações matemáticas, as processa e retorna o resultado.Cliente (client): Conecta-se ao servidor, envia operações lidas da linha de comando e exibe a resposta recebida.2. Como Compilar e ExecutarPré-requisitosCompilador C (GCC)Utilitário makeCompilaçãoPara compilar tanto o cliente quanto o servidor, execute o seguinte comando na raiz do projeto:make
ExecuçãoInicie o ServidorAbra um terminal e inicie o servidor na porta padrão 5050../server
Inicie o ClienteAbra outro terminal e conecte o cliente ao servidor../client 127.0.0.1 5050
3. Exemplos de UsoA seguir, um exemplo de sessão no terminal do cliente:> ADD 100 50
OK 150.000000

> DIV 10 0
ERR EZDV divisao_por_zero

> operacao_invalida 1 2
ERR EINV comando_invalido_ou_formato_incorreto

> QUIT
[INFO] Desconectando.
4. Decisões de Projeto e LimitaçõesServidor Iterativo: O servidor atende um cliente por vez. Para suportar múltiplos clientes simultaneamente, seria necessário usar fork() ou threads.Parsing com sscanf: sscanf foi utilizado pela simplicidade e adequação ao protocolo definido, com verificação de retorno para garantir robustez.Tratamento de Sinais: O servidor encerra de forma limpa ao receber SIGINT (Ctrl+C), fechando o socket corretamente.
