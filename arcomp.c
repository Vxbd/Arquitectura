#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void start_counter();
double get_counter();
double mhz();

/* Initialize the cycle counter */
static unsigned cyc_hi = 0;
static unsigned cyc_lo = 0;

/* Set *hi and *lo to the high and low order bits of the cycle counter.
Implementation requires assembly code to use the rdtsc instruction. */
void access_counter(unsigned *hi, unsigned *lo) {
  asm("rdtsc; movl %%edx,%0; movl %%eax,%1" /* Read cycle counter */
      : "=r"(*hi), "=r"(*lo)                /* and move results to */
      : /* No input */                      /* the two outputs */
      : "%edx", "%eax");
}

/* Record the current value of the cycle counter. */
void start_counter() { access_counter(&cyc_hi, &cyc_lo); }

/* Return the number of cycles since the last call to start_counter. */
double get_counter() {
  unsigned ncyc_hi, ncyc_lo;
  unsigned hi, lo, borrow;
  double result;

  /* Get cycle counter */
  access_counter(&ncyc_hi, &ncyc_lo);

  /* Do double precision subtraction */
  lo = ncyc_lo - cyc_lo;
  borrow = lo > ncyc_lo;
  hi = ncyc_hi - cyc_hi - borrow;
  result = (double)hi * (1 << 30) * 4 + lo;
  if (result < 0) {
    fprintf(stderr, "Error: counter returns neg value: %.0f\n", result);
  }
  return result;
}

double mhz(int verbose, int sleeptime) {
  double rate;

  start_counter();
  sleep(sleeptime);
  rate = get_counter() / (1e6 * sleeptime);
  if (verbose)
    printf("\n Processor clock rate = %.1f MHz\n", rate);
  return rate;
}

int main(int argc, char const *argv[]) {
  double ck;
  int D;      // Tamanho vector interno
  int R;      // Elementos lista, numero de estruturas.
  long int k; // Elemento auxiliar para la asignacion a una variable

  // Asinacion de D y R por linea de comandos
  if (argc == 3) {
    D = atoi(argv[1]);
    R = atoi(argv[2]);

  } else {
    printf("ERRO: O formato debe ser: ./executable -D -R\n");
    return 0;
  }

  typedef struct s {
    struct s *p;
    long int data[D];
  } s;

  // Creamos la lista y reservamos memoria
  s *punterosLista = (s *)malloc(sizeof(s) * R);

  for (int i = 0; i < R - 1; i++) {
    punterosLista[i].p = &punterosLista[i + 1];
    /*
     * No incializaremos el vector de datos dado que
     * consideramos que podría contaminar las pruebas
     */
  }

  // Hacemos que la lista sea enlazada(cerrada)
  punterosLista[R - 1].p = &punterosLista[0];

  start_counter();
  /*---------Inicio codigo a medir---------*/
  // Accedemos con un bucle a los elementos de la lista
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < R * 10; j++) {
      k = punterosLista[j].p->data[0];
    }
  }
  /*-----------Fin codigo a medir-----------*/


// Liberar memoria
  ck = get_counter();

  printf("\n Clocks=%1.10lf \n", ck);

  /* Esta rutina imprime a frecuencia de reloxo estimada coas rutinas
   * start_counter/get_counter */
  mhz(1, 1);

  return 0;
}
