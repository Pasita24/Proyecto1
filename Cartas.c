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
    int puntosVida;
    struct Carta mano[Num_Cartas_Iniciales];
    int numCartasEnMano;
};

void cargarCartasDesdeArchivo(struct Guardian cartas[], int *numCartas);
void mostrarMenu();
void revolverCartas(struct Guardian cartas[], int numCartas);
void seleccionarCartasIniciales(struct Jugador *jugador, struct Guardian lote[], int *numCartasEnLote);
void eliminarCartaDelLote(struct Guardian lote[], int *numCartasEnLote, int eleccion);

int main() {
    struct Guardian cartas[Max_Cartas];
    int numCartas = 0;
    
    cargarCartasDesdeArchivo(cartas, &numCartas);
    revolverCartas(cartas, numCartas);

    int opcion;
    int numCartasRestantes = Num_Cartas_A_Mostrar;
    do {
        mostrarMenu();
        printf("Selecciona una opcion: \n");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                // Implementar inicio del juego
                struct Jugador jugador1;
                struct Jugador jugador2;

                // Crear mazo completo
                struct Guardian mazoCompleto[Max_Cartas];
                memcpy(mazoCompleto, cartas, sizeof(struct Guardian) * Max_Cartas);
                revolverCartas(mazoCompleto, Max_Cartas);

                // Jugador 1 selecciona sus cartas
                seleccionarCartasIniciales(&jugador1, mazoCompleto, &numCartasRestantes);

                printf("\n\nTurno del segundo jugador\n");
                
                // Jugador 2 selecciona sus cartas del mazo restante
                seleccionarCartasIniciales(&jugador2, mazoCompleto, &numCartasRestantes);
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
    char *token;

    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        token = strtok(linea, ",");
        strcpy(cartas[*numCartas].nombre, token);
        token = strtok(NULL, ",");
        strcpy(cartas[*numCartas].tipo, token);
        token = strtok(NULL, ",");
        cartas[*numCartas].PV = atoi(token);
        token = strtok(NULL, ",");
        cartas[*numCartas].PA = atoi(token);
        token = strtok(NULL, ",");
        cartas[*numCartas].PD = atoi(token);

        (*numCartas)++;
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
    printf("Se estan revolviendo las cartas....\n");
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

void seleccionarCartasIniciales(struct Jugador *jugador, struct Guardian lote[], int *numCartasEnLote) {
    jugador->numCartasEnMano = 0; // Inicializa la mano del jugador
    printf("Selecciona tus 3 cartas iniciales:\n");

    while (jugador->numCartasEnMano < Num_Cartas_Iniciales && *numCartasEnLote > 0) {
        for (int i = 0; i < *numCartasEnLote && jugador->numCartasEnMano < Num_Cartas_Iniciales; i++) {
            if (lote[i].nombre[0] != '-' && lote[i].nombre[1] != '-') {
                printf("%d. %s (%s, PV:%d, PA:%d, PD:%d)\n", i + 1, lote[i].nombre, lote[i].tipo, lote[i].PV, lote[i].PA, lote[i].PD);
            } else {
                printf("%d. --\n", i + 1);
            }
        }

        printf("Elige una carta (1-%d): ", *numCartasEnLote);
        int eleccion;
        scanf("%d", &eleccion);

        if (eleccion >= 1 && eleccion <= *numCartasEnLote && lote[eleccion - 1].nombre[0] != '-' && lote[eleccion - 1].nombre[1] != '-') {
            // Agrega la carta seleccionada a la mano del jugador
            jugador->mano[jugador->numCartasEnMano].guardian = lote[eleccion - 1];
            jugador->numCartasEnMano++;

            // Deja la carta seleccionada como ("--") en el lote
            lote[eleccion - 1].nombre[0] = '-';
            lote[eleccion - 1].nombre[1] = 'S';
            lote[eleccion - 1].nombre[2] = 'E';
            lote[eleccion - 1].nombre[3] = 'L';
            lote[eleccion - 1].nombre[4] = 'E';
            lote[eleccion - 1].nombre[5] = 'C';
            lote[eleccion - 1].nombre[6] = 'C';
            lote[eleccion - 1].nombre[7] = 'I';
            lote[eleccion - 1].nombre[8] = 'O';
            lote[eleccion - 1].nombre[9] = 'N';
            lote[eleccion - 1].nombre[10] = 'A';
            lote[eleccion - 1].nombre[11] = 'D';
            lote[eleccion - 1].nombre[12] = 'A';
            lote[eleccion - 1].nombre[13] = '-';
            
            printf("Carta seleccionada.\n\n");
        } else {
            printf("Selección no valida. Elige una carta válida del 1 al %d.\n", *numCartasEnLote);
        }
    }

    printf("\n\nTus cartas iniciales son:\n");

    for (int i = 0; i < Num_Cartas_Iniciales; i++) {
        printf("%s (%s, PV:%d, PA:%d, PD:%d)\n", jugador->mano[i].guardian.nombre, jugador->mano[i].guardian.tipo, jugador->mano[i].guardian.PV, jugador->mano[i].guardian.PA, jugador->mano[i].guardian.PD);
    }
}







