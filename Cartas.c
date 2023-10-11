#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Max_Cartas 60
#define Num_Cartas_Iniciales 3
#define Num_Cartas_A_Mostrar 15

struct Guardian {
    char nombre[50];
    int PV;
    int PA;
    int PD;
    char tipo[20];
};

struct Carta {
    struct Guardian guardian;
    int enCampoBatalla;
};

struct Jugador {
    int puntosVida ;
    struct Carta mano[Num_Cartas_Iniciales];
    int numCartasEnMano;
    struct Guardian mazoPropio[Num_Cartas_A_Mostrar];
    struct Carta campoBatalla[Max_Cartas];
};

void cargarCartasDesdeArchivo(struct Guardian cartas[], int *numCartas);
void mostrarMenu();
void revolverCartas(struct Guardian cartas[], int numCartas);
void seleccionarCartasIniciales(struct Jugador *jugador, int numCartasRestantes);
void mostrarCartasEnMano(struct Jugador jugador);
void iniciarCombate(struct Jugador *jugador1, struct Jugador *jugador2);

int main() {
    struct Guardian cartas[Max_Cartas];
    int numCartas = 0;

    cargarCartasDesdeArchivo(cartas, &numCartas);
    revolverCartas(cartas, numCartas);

    int opcion;
    do {
        mostrarMenu();
        printf("Selecciona una opcion: \n");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                // Implementar inicio del juego
                struct Jugador jugador1;
                struct Jugador jugador2;

                // Copiar las cartas al mazo propio de cada jugador
                memcpy(jugador1.mazoPropio, cartas, sizeof(struct Guardian) * Num_Cartas_A_Mostrar);
                memcpy(jugador2.mazoPropio, cartas, sizeof(struct Guardian) * Num_Cartas_A_Mostrar);

                revolverCartas(jugador1.mazoPropio, Num_Cartas_A_Mostrar);
                revolverCartas(jugador2.mazoPropio, Num_Cartas_A_Mostrar);

                // Inicializar el número de cartas en mano
                jugador1.numCartasEnMano = 0;
                jugador2.numCartasEnMano = 0;

                // Jugadores seleccionan sus cartas iniciales
                seleccionarCartasIniciales(&jugador1, Num_Cartas_A_Mostrar);
                seleccionarCartasIniciales(&jugador2, Num_Cartas_A_Mostrar);

                // Iniciar el combate
                iniciarCombate(&jugador1, &jugador2);
                break;
            case 2:
                break;
            case 3:
                printf("Saliendo del juego...\n");
                break;
            default:
                printf("Opcion no valida. Intenta de nuevo.\n");
        }
    } while (opcion != 3);

    return 0;
}

void cargarCartasDesdeArchivo(struct Guardian cartas[], int *numCartas) {
    FILE *archivo = fopen("Cartas.txt", "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    char linea[100];

    // Leer la primera línea (encabezado) y descartarla
    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        // Utilizar sscanf para leer los valores directamente en la estructura Guardian
        if (sscanf(linea, "%49[^,],%19[^,],%d,%d,%d\n",
                   cartas[*numCartas].nombre, cartas[*numCartas].tipo,
                   &cartas[*numCartas].PV, &cartas[*numCartas].PA, &cartas[*numCartas].PD) == 5) {
            (*numCartas)++;
        } else {
            fprintf(stderr, "Error al leer la línea: %s", linea);
        }
    }

    fclose(archivo);
}

void mostrarMenu() {
    printf("---- The Clash of the Guardians ----\n");
    printf("1. Comenzar juego\n");
    printf("2. Historial de la partida\n");
    printf("3. Salir\n");
}

void revolverCartas(struct Guardian cartas[], int numCartas) {
    srand(time(NULL));
    printf("Se están revolviendo las cartas....\n");
    printf("....\n");
    printf("...\n");
    printf("..\n");
    printf(".\n");
    printf(".\n");
    printf(".\n");

    for (int i = numCartas - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        struct Guardian temp = cartas[i];
        cartas[i] = cartas[j];
        cartas[j] = temp;
    }
}

void seleccionarCartasIniciales(struct Jugador *jugador, int numCartasRestantes) {
    printf("Selecciona tus 3 cartas iniciales:\n");

    while (jugador->numCartasEnMano < Num_Cartas_Iniciales && numCartasRestantes > 0) {
        for (int i = 0; i < Num_Cartas_A_Mostrar && jugador->numCartasEnMano < Num_Cartas_Iniciales; i++) {
            if (jugador->mazoPropio[i].nombre[0] != '-' && jugador->mazoPropio[i].nombre[1] != '-') {
                printf("%d. %s (%s, PV:%d, PA:%d, PD:%d)\n", i + 1, jugador->mazoPropio[i].nombre, jugador->mazoPropio[i].tipo, jugador->mazoPropio[i].PV, jugador->mazoPropio[i].PA, jugador->mazoPropio[i].PD);
            } else {
                printf("%d. --\n", i + 1);
            }
        }

        printf("Elige una carta (1-%d): ", Num_Cartas_A_Mostrar);
        int eleccion;
        scanf("%d", &eleccion);

        if (eleccion >= 1 && eleccion <= Num_Cartas_A_Mostrar && jugador->mazoPropio[eleccion - 1].nombre[0] != '-' && jugador->mazoPropio[eleccion - 1].nombre[1] != '-') {
            jugador->mano[jugador->numCartasEnMano].guardian = jugador->mazoPropio[eleccion - 1];
            jugador->numCartasEnMano++;

            jugador->mazoPropio[eleccion - 1].nombre[0] = '-';
            jugador->mazoPropio[eleccion - 1].nombre[13] = '-';

            printf("Carta seleccionada.\n\n");
        } else {
            printf("Selección no válida. Elige una carta válida del 1 al %d.\n", Num_Cartas_A_Mostrar);
        }
    }

    printf("\n\nTus cartas iniciales son:\n");
    mostrarCartasEnMano(*jugador);
}

void mostrarCartasEnMano(struct Jugador jugador) {
    for (int i = 0; i < Num_Cartas_Iniciales; i++) {
        printf("%s (%s, PV:%d, PA:%d, PD:%d)\n", jugador.mano[i].guardian.nombre, jugador.mano[i].guardian.tipo, jugador.mano[i].guardian.PV, jugador.mano[i].guardian.PA, jugador.mano[i].guardian.PD);
    }
}

void iniciarCombate(struct Jugador *jugador1, struct Jugador *jugador2) {
    int turno = 1; 
    int cartaElegida;

    while (1) {
        printf("\n----- Turno del Jugador %d -----\n", turno);

        if (turno == 1) {
            printf("Cartas del Jugador 1:\n");
            mostrarCartasEnMano(*jugador1);
            printf("Elige una carta para poner en el campo de batalla (1-%d): ", jugador1->numCartasEnMano);
            scanf("%d", &cartaElegida);

            if (cartaElegida >= 1 && cartaElegida <= jugador1->numCartasEnMano) {
                jugador1->campoBatalla[0] = jugador1->mano[cartaElegida - 1];
                jugador1->campoBatalla[0].enCampoBatalla = 1;

                jugador1->mano[cartaElegida - 1].enCampoBatalla = 1;
                jugador1->numCartasEnMano--;

                // Mostrar la carta que está en combate y sus atributos
                printf("La carta %s está en combate. Atributos: PV:%d, PA:%d, PD:%d\n", jugador1->campoBatalla[0].guardian.nombre, jugador1->campoBatalla[0].guardian.PV, jugador1->campoBatalla[0].guardian.PA, jugador1->campoBatalla[0].guardian.PD);
            } else {
                printf("Selección no válida. Elige una carta válida del 1 al %d.\n", jugador1->numCartasEnMano);
                continue;
            }
        } else if (turno == 2) {
            printf("Cartas del Jugador 2:\n");
            mostrarCartasEnMano(*jugador2);
            printf("Elige una carta para poner en el campo de batalla (1-%d): ", jugador2->numCartasEnMano);
            scanf("%d", &cartaElegida);

            if (cartaElegida >= 1 && cartaElegida <= jugador2->numCartasEnMano) {
                jugador2->campoBatalla[0] = jugador2->mano[cartaElegida - 1];
                jugador2->campoBatalla[0].enCampoBatalla = 1;

                jugador2->mano[cartaElegida - 1].enCampoBatalla = 1;
                jugador2->numCartasEnMano--;

                printf("La carta %s está en combate. Atributos: PV:%d, PA:%d, PD:%d\n", jugador2->campoBatalla[0].guardian.nombre, jugador2->campoBatalla[0].guardian.PV, jugador2->campoBatalla[0].guardian.PA, jugador2->campoBatalla[0].guardian.PD);
            } else {
                printf("Selección no válida. Elige una carta válida del 1 al %d.\n", jugador2->numCartasEnMano);
                continue;
            }
        }
        turno = 3 - turno;
    }
}



