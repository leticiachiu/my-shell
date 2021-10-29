# Shell Simulation
Simples implementacao do Shell desenvolvido na disciplina Infraestrutura de Software do 3 periodo. Obs.: Esse projeto foi desenvolvido e implementado no Fedora 33 (Linux).

# Como utilizar
Abra o terminal no repositorio local do arquivo <br>
Para compilar o codigo e gerar o arquivo executavel: 
> `make`


Em seguida, é possível continuar com dois comandos: <br>

Para executar interativamente: 
> `./sheLL`

Ou executando com um batchFile : 
> `./sheLL [batchFile]`

Escolha um modo da execucao:  (por default, o programa se inicia no sequencial e no terminal aparece ```ll seq>```)
> `style sequential`

> `style parallel`

escolhendo o modo paralelo o terminal mostrara `ll par>` na linha de comando

# Comandos:
Esse shell consegue executar os comandos nativos no Linux como por exemplo: 
> `ls, ps, sort, cd, mkdir , etc.` <br>

Diferentes comandos podem ser separados por um ponto virgula: 
> `ls; cd lucas; mkdir leticia`

Use o pipe (|), exemplo: 
> `ls | sort`

Use redirecionamento de entradas e saidas. (>), (<) ou (>>):
>`ls > filename` <br>
>`ls >> filename` <br>
>`test < filename`

Duvidas? Insira o comando de ajuda no terminal:
> `help`

Para interromper o processo e sair do shell:
> `exit` <br>
> `ctrl-d`

Para apagar o shell apos o uso:
> `make clean`
 
