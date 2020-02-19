#include <stdio.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{


    int D; //Tamanho vector interno
    int R; //Elementos lista, numero de estruturas.
    long int k; //Elemento auxiliar para asignacion a unha variable

    printf("Antes de atoi\n");
    //Asinacion de D e R por linha de comandos
    if(argc == 3){
        D = atoi(argv[1]);
        R = atoi(argv[2]);
        printf("despois de atoi\n");

    } else{
        printf("ERRO: O formato debe ser: ./executable -D -R\n");
        //return 0;
    }

    typedef struct s {
        struct s *p;
        long int data[20000];
    } s;

    R = 1000;

    printf("hfosaf\n");

    //Creamos a lista e reservamos memoria
    s *punteirosLista = malloc (sizeof(s) * R);

    for (int i = 0; i < R-1; i++) {
        punteirosLista[i].p = &punteirosLista[i+1];
        printf(" %d\n", i);
        /*
         * Non incializaremos o vector de datos dado que
         * consideramos q ue podería contaminar as probas
        */
    }
    printf("hfosaf3\n");


    //Circular
    punteirosLista[R -1].p = &punteirosLista[0];


    //Accedemos cun bucle
    for (int j = 0; j < R; j++) {
        k = punteirosLista[j].p->data[0];
        printf("Esto anda funsionando webon\n");
    }

printf("Las mejores cosas siempre tienen un fin, pero el recuerdo siempre perduraran en nuestros corasones\n");

    return 0;
}