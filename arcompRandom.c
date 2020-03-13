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
    D = atof(argv[1]);
    /*
     * Como con D=40 y D=80 el tamanho de la estructura no ocupa un número de
     * lineas exacto, redondeamos hacia arriba cogiendo una estructura entera
     * aunque luego toquemos más lineas de las necesarias con el fin de, tocar
     * al menos esas
     */
    if (atoi(argv[2]) == 49 || atoi(argv[2]) == 149 || atoi(argv[2]) == 399 ||
        atoi(argv[2]) == 1198 || atoi(argv[2]) == 9590 ||
        atoi(argv[2]) == 14385 || atoi(argv[2]) == 38362 ||
        atoi(argv[2]) == 153450 || atoi(argv[2]) == 25 || atoi(argv[2]) == 75 ||
        atoi(argv[2]) == 202 || atoi(argv[2]) == 606 || atoi(argv[2]) == 4854 ||
        atoi(argv[2]) == 7281 || atoi(argv[2]) == 19418 ||
        atoi(argv[2]) == 77672 || atoi(argv[2]) == 196608 ||
        atoi(argv[2]) == 786432) {
      R = atoi(argv[2]) + 1;
    } else {
      R = atoi(argv[2]);
    }
  } else {
    perror("ERRO: O formato debe ser: ./executable -D -R\n");
  }

  FILE *fichero;
  // En funcion del valor de D anhadiremos los datos de la prueba a un archivo
  switch (D) {
  case 1:
    fichero = fopen("prueba1a.csv", "a+");
    break;
  case 3:
    fichero = fopen("prueba3a.csv", "a+");
    break;
  case 15:
    fichero = fopen("prueba15a.csv", "a+");
    break;
  case 40:
    fichero = fopen("prueba40a.csv", "a+");
    break;
  case 80:
    fichero = fopen("prueba80a.csv", "a+");
    break;
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
  // Tomamos la primera posicion de la lista
  s *pos = &punterosLista[0];
  // Accedemos con un bucle a los elementos de la lista
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < R; j++) {
      k = pos->data[0];
      pos = pos->p;
    }
  }
  ck = get_counter();
  /*-----------Fin codigo a medir-----------*/

  printf("\n Clocks=%1.10lf, %ld\n", ck / (10 * R), (sizeof(s) * R) / 64);
  fprintf(fichero, "%1.10lf, %ld\n", ck / (10 * R), (sizeof(s) * R) / 64);

  fclose(fichero);

  /* Esta rutina imprime a frecuencia de reloxo estimada coas rutinas
   * start_counter/get_counter */
  mhz(1, 1);

  // Liberar memoria
  _mm_free(punterosLista);

  return 0;
}
