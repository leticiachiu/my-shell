#include <stdio.h>
#include <unistd.h>     
#include <string.h>
#include <fcntl.h>      
#include <sys/wait.h>   
#include <stdlib.h>         

int executar = 1;
int sequencial = 1;

void cd(char **args) {

    if(args[1] != NULL) {
        chdir(args[1]);        
    } else {
        printf("argumento esperado\n");
    }

}


void help(char **args)
{

    printf("                       ▄▀▄     ▄▀▄\n");
    printf("                      ▄█  ▀▀▀▀▀  █▄\n");
    printf("                  ▄▄  █           █  ▄▄\n");
    printf("                 █▄▄█ █  ▀  ┬  ▀  █ █▄▄█\n");
    printf(" --------------------------------------------------------\n");
    printf("|               PROMPT DE LETICIA E LUCAS                |\n");
    printf("| Digite nomes de programas e argumentos, e aperte enter |\n");
    printf("| as seguintes funcoes estao implementadas:              |\n");
    printf("|     cd                                                 |\n");                       
    printf("|     help                                               |\n");
    printf("|     exit                                               |\n");
    printf("| Use '|' para criar pipes, '<', '>' e \">>\" para         |\n");
    printf("| redirecionar entradas e saidas                         |\n");
    printf(" --------------------------------------------------------\n");
}


void redirecionar_entrada(char *fileName)
{
    int in = open(fileName, O_RDONLY);
    dup2(in, 0);
    close(in);
}

void redirecionar_saida(char *nome_arquivo, char tipo)
{

    if (tipo == 'w'){
        int out = open(nome_arquivo, O_WRONLY | O_TRUNC | O_CREAT, 0600);
        dup2(out, 1);
        close(out);
    } else {
        int out = open(nome_arquivo, 'a', 0600);
        dup2(out, 1);
        close(out);
    }
    
    
}


void execute(char *args[])
{
    pid_t pid;


	if (strcmp(args[0], "cd") == 0){
			cd(args);

	} else if (strcmp(args[0], "help") == 0){
			help(args);

	} else if (strcmp(args[0], "exit") == 0){

		printf("                  ▄▀▄     ▄▀▄\n");
		printf("                 ▄█  ▀▀▀▀▀  █▄     BYE!\n");
		printf("             ▄▄  █           █  ▄▄\n");
		printf("            █▄▄█ █  ▀  ┬  ▀  █ █▄▄█\n");
		
		executar = 0;

	} else {

		pid = fork();

		if (pid == 0) {

			execvp(args[0], args);

		}
		else { /* parent process */
			if (sequencial) {
				waitpid(pid, NULL, 0);
			} else {
				sequencial = 0;
			}
		}
		redirecionar_entrada("/dev/tty");
		redirecionar_saida("/dev/tty", 'w');
	}
}


void criar_pipe(char *args[])
{
    int fd[2];
    pipe(fd);

    dup2(fd[1], 1);
    close(fd[1]);

    execute(args);

    dup2(fd[0], 0);
    close(fd[0]);
}


char * formatar_entrada(char *entrada, int tamanho)
{

    int j = 0;
    char *formatado = (char *)malloc((1024) * sizeof(char));

    for (int i = 0; i < tamanho; i++) {
        if (entrada[i] != '>' && entrada[i] != '<' && entrada[i] != '|' && entrada[i] != ';') {
            formatado[j++] = entrada[i];
        } else {
            formatado[j++] = ' ';
            formatado[j++] = entrada[i];
            if (entrada[i+1] == '>'){
                formatado[j++] = entrada[i+1];
                i++;
            }
            formatado[j++] = ' ';
        }
    }
    formatado[j++] = '\0';
    formatado[j++] = '\0';

    return formatado;
}

int main(int argc, char *argv[])
{
    char *args[512];
    char *comandos;
    char *style = "ll seq> ";
	int batch = 0;
    char *nome_arquivo;
    FILE *batch_file;

    if (argc == 2) {
        batch = 1;	        
        nome_arquivo = argv[1];

        batch_file = fopen(argv[1], "r");

        if (batch_file == NULL){
            printf("arquivo nao encontrado\n");
            return 0;
        }
    } else if (argc > 2) {
        printf("entrada invalida\n");
        return 0;
    }


    
    while (executar) {
		char entrada[512];

		if (!batch){
            printf("%s", style);
        	if(fgets(entrada, 512, stdin) == NULL){
                args[0] = "exit";
				execute(args);
            }
		} else {

			if (fgets(entrada, 512, batch_file) == NULL){
                break;
            }
            printf("\n%s\n", entrada);
		}

		
		if (strncmp(entrada, "style sequential", 16) == 0){
            sequencial = 1;
            style = "ll seq> ";
            continue;
        } else if (strncmp(entrada, "style parallel", 14) == 0){
            sequencial = 0;
            style = "ll par> ";
            continue;
        } else if (strcmp(entrada, "\n") == 0){
            
            continue;
        }

        comandos = formatar_entrada(entrada, strlen(entrada));

        char *end;
        end = comandos + strlen(comandos) - 1;
        end--;
        *(end + 1) = '\0';

        char *arg = strtok(comandos, " ");
        int i = 0;

        while (arg) {
            if (*arg == '<') {
                redirecionar_entrada(strtok(NULL, " "));
            } else if (strcmp(arg, ">>") == 0) {
                redirecionar_saida(strtok(NULL, " ") ,'a');
            } else if (*arg == '>') {
                redirecionar_saida(strtok(NULL, " "), 'w');               
            } else if (*arg == ';') {
                args[i] = NULL;
                execute(args);               
                i = 0;
            } else if (*arg == '|') {
                args[i] = NULL;
                criar_pipe(args);
                i = 0;
            } else {
                args[i] = arg;
                i++;
            }
            arg = strtok(NULL, " ");
        }

        args[i] = NULL;

        execute(args);
    }
    return 0;
}