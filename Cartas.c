#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Max_Cartas 30

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
    struct Carta mano[15];
    int numCartasEnMano;
};

void cargarCartasDesdeArchivo(struct Guardian cartas[], int *numCartas);
void mostrarMenu();

int main() {
    struct Guardian cartas[Max_Cartas];
    int numCartas = 0;

    cargarCartasDesdeArchivo(cartas, &numCartas);

    int opcion;
    do {
        mostrarMenu();
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                break;
            case 2:
                break;
            case 3:

                break;
            case 4:
                printf("Saliendo del juego.\n");
                break;
            default:
                printf("Opcion no válida. Intenta de nuevo.\n");
        }
    } while (opcion != 4);

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
        strcpy(cartas[*numCartas].tipo, token);
        token = strtok(NULL, ",");
        strcpy(cartas[*numCartas].nombre, token);
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
    printf("1. Crear nueva carta\n");
    printf("2. Comenzar juego\n");
    printf("3. Historial de la partida\n");
    printf("4. Salir\n");
}
