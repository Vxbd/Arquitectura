#include <pmmintrin.h>
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
    perror("ERRO: O formato debe ser: ./executable -D -R\n");
  }

  typedef struct s {
    struct s *p;
    long int data[D];
  } s;

  // Creamos la lista y reservamos memoria
  s *punterosLista = (s *)_mm_malloc(sizeof(s) * R, 64);
  // Variables auxiliares que utilizaremos para conseguir la aleatoriedad
  int array[R], pos1, pos2, aux;

  // Asignamos los valores al array de posiciones
  for (int i = 0; i < R; i++) {
    array[i] = i;
  }

  // Hacemos que haya cierta aleatoriedad en el array
  for (int i = 0; i < R * 2; i++) {
    // Generamos dos posiciones aleatorias
    pos1 = rand() % R;
    pos2 = rand() % R;
    // Hacemos swap entre dichas posiciones
    aux = array[pos1];
    array[pos1] = array[pos2];
    array[pos2] = array[pos1];
  }

  // Asignamos los punteros
  for (int i = 0; i < R; i++) {
    punterosLista[i].p = &punterosLista[array[i]];
    /*
     * No incializaremos el vector de datos dado que
     * consideramos que podría contaminar las pruebas
     */
  }

  start_counter();
  /*---------Inicio codigo a medir---------*/
  // Accedemos con un bucle a los elementos de la lista
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < R; j++) {
      k = punterosLista[j].p->data[0];
    }
  }
  /*-----------Fin codigo a medir-----------*/

  ck = get_counter();

  printf("\n Clocks=%1.10lf \n", ck);

  /* Esta rutina imprime a frecuencia de reloxo estimada coas rutinas
   * start_counter/get_counter */
  mhz(1, 1);

  // Liberar memoria
  _mm_free(punterosLista);

  return 0;
}
