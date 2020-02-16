#include <stdio.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
    int D; //Tamanho vector interno
    int R; //Elementos lista, numero de estruturas.

    typedef struct {
        struct s *p;
        long int data[D];
    } s;

    //Creamos a lista e reservamos memoria
    s *punteirosLista = malloc (sizeof(s) * R);

    for (int i = 0; i < R-1; i++) {
        punteirosLista[i].p = &punteirosLista[i+1];
        /* Non incializaremos o vector de datos dado que
         * consideramos q ue podería contaminar as probas
        */
    }

    //Circular
    punteirosLista[R].p = &punteirosLista[0];

    


    return 0;
}