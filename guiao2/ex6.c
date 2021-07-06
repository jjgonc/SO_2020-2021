#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

//objetivo deste ex é passar um inteiro como argumento e verificar se ele existe na matriz
int main (int argc, char* argv[]) {
    if (argc < 2) {
        printf("Argument needed...\n");
        return -1;
    }

    pid_t pid;
    int needle = atoi(argv[1]);
    int rows = 10;
    int columns = 10000;
    int rand_max = 10000;
    int status;
    int **matrix;   //matriz é um array de arrays

    //Alocar espaço e povoar a matriz com numeros random
    printf ("Generating numbers from 0 to %d...\n", rand_max);
    matrix = malloc(sizeof(int*) * rows);   //alocar cada uma das linhas
    for (int i=0; i<rows;i++) {
        matrix[i] = (int*) malloc(sizeof (int) * columns); //aux[1] = malloc(sizeof (int) * x); => aux[1] == *(aux+1)
        for (int j=0; j<columns;j++) {
            matrix[i][j] = rand() % rand_max;
        }
    }
    printf("Done!\n\n");

    for (int i=0; i<rows; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            for (int j=0; j<columns; j++) {
                if (matrix[i][j] == needle){
                    _exit(i);
                } 
            }
            _exit(-1);
        } 
    }

    for (int i = 0; i < rows; i++) {
        pid_t terminated_pid = wait(&status); //ao fazer apenas wait nao garantimos que os filhos apareçam pela ordem que foram criados, por isso igualamos ao pid
        if (WEXITSTATUS(status)) {  //se WIFExITSTATUS der true é pq o processo retornou corretamente, caso contrario da false
            if (WEXITSTATUS(status) < 255) {
                printf ("[pai] process with pid %d found number at row %d\n", terminated_pid, WEXITSTATUS(status));
            }
            else {  //quando devolve 255 é pq nao encontrou o valor nessa linha da matriz
                printf ("[pai] process with pid %d didn't find anything...\n", terminated_pid);
            }
        }
        else {
            printf ("[pai] process with pid %d exited. Something went wrong\n", terminated_pid);
        }
    }
    
    return 0;
}
