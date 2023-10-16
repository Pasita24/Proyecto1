#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Max_Cartas 60
#define Num_Cartas_Iniciales 3
#define Num_Cartas_A_Mostrar 15
#define Max_Guardianes_Creados 5

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
    struct Guardian mazoPropio[Num_Cartas_A_Mostrar];
    struct Carta campoBatalla[Max_Cartas];
};

struct JugadorCreador {
    int numGuardianesCreados;
    struct Guardian guardianCreado[Max_Guardianes_Creados];
};
int guardianAgregadoPorTurno = 0;
void cargarCartasDesdeArchivo(struct Guardian cartas[], int *numCartas);
void mostrarMenu();
void revolverCartas(struct Guardian cartas[], int numCartas);
void seleccionarCartasIniciales(struct Jugador *jugador, int numCartasRestantes);
void mostrarCartasEnMano(struct Jugador jugador);
void iniciarCombate(struct Jugador *jugador1, struct Jugador *jugador2);
void seleccionarCartaCampoBatalla(struct Jugador *jugador);
void atacarOponente(struct Jugador *atacante, struct Jugador *oponente, struct Jugador *jugador1, struct Jugador *jugador2);
void moverCartaDeMazoAMano(struct Jugador *jugador);
void crearGuardian(struct JugadorCreador *jugadorCreador);
void mostrarGuardianesCreados(struct JugadorCreador jugadorCreador);

int main() {
    struct Guardian cartas[Max_Cartas];
    int numCartas = 0;

    cargarCartasDesdeArchivo(cartas, &numCartas);
    revolverCartas(cartas, numCartas);

    int opcion;
    struct JugadorCreador jugadorCreador;
    jugadorCreador.numGuardianesCreados = 0;

    do {
        mostrarMenu();
        printf("Selecciona una opcion: \n");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                // Implementar inicio del juego
                struct Jugador jugador1;
                struct Jugador jugador2;

                jugador1.puntosVida = 3;
                jugador2.puntosVida = 3;

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
                printf("\n");
                seleccionarCartasIniciales(&jugador2, Num_Cartas_A_Mostrar);

                printf("\n\n");
                // Iniciar el combate
                printf("----------------Empieza el juego-----------------\n");
                iniciarCombate(&jugador1, &jugador2);
                break;
            case 2:
                mostrarGuardianesCreados(jugadorCreador);
                break;
            case 3:
                crearGuardian(&jugadorCreador);
                break;
            case 4:
                printf("Saliendo del juego...\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 4);

    return 0;
}
//<---------------------- Funcion para leer los guardianes------------------------------>
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
            fprintf(stderr, "Error al leer la linea: %s", linea);
        }
    }

    fclose(archivo);
}
//<------------------------------ Fin de funcion para leer los guardianes------------------------------------>
//<------------Funcion mostrar menu principal------------->
void mostrarMenu() {
    printf("---- The Clash of the Guardians ----\n");
    printf("1. Comenzar juego\n");
    printf("2. Historial de la partida\n");
    printf("3. Crear Nuevo guardian\n");
    printf("4. Salir del juego\n");
}
//<----------------Fin de funcion mostrar menu principal ------------------>
//<-------------- Funcion para revolver las Guardianes -------------------------------->
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
// <--------------------------FIN  de funcion para mezclar los guardianes---------------->
//<------------------- Funcion para poder seleccionar las cartas inciales------------------------->
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
            printf("Seleccion no valida. Elige una carta valida del 1 al %d.\n", Num_Cartas_A_Mostrar);
        }
    }

    printf("\n\nTus cartas iniciales son:\n");
    mostrarCartasEnMano(*jugador);
}
//<-------------------------------FIN Funcion para poder seleccionar las cartas inciales-------------------------->
//<----------------------Funcion para crear Guardian -------------------------->
void crearGuardian(struct JugadorCreador *jugadorCreador) {
    if (jugadorCreador->numGuardianesCreados >= Max_Guardianes_Creados) {
        printf("Has alcanzado el limite de guardianes creados (%d).\n", Max_Guardianes_Creados);
        return;
    }

    printf("Creacion de un nuevo guardian:\n");
    struct Guardian nuevoGuardian;

    printf("Nombre: ");
    scanf("%s", nuevoGuardian.nombre);

    printf("Selecciona el tipo de guardian:\n");
    printf("1. Mago\n");
    printf("2. Vikingo\n");
    printf("3. Nigromante\n");
    printf("4. Bestia\n");
    int tipoSeleccionado;
    scanf("%d", &tipoSeleccionado);

    switch (tipoSeleccionado) {
        case 1:
            strcpy(nuevoGuardian.tipo, "Mago");
            break;
        case 2:
            strcpy(nuevoGuardian.tipo, "Vikingo");
            break;
        case 3:
            strcpy(nuevoGuardian.tipo, "Nigromante");
            break;
        case 4:
            strcpy(nuevoGuardian.tipo, "Bestia");
            break;
        default:
            printf("Tipo no valido. Selecciona un tipo valido (1-4).\n");
            return;
    }

    printf("Asigna puntos (total de 1000) a los atributos:\n");
    printf("Puntos de Ataque (PA): ");
    scanf("%d", &nuevoGuardian.PA);

    printf("Puntos de Defensa (PD): ");
    scanf("%d", &nuevoGuardian.PD);

    printf("Puntos de Vida (PV): ");
    scanf("%d", &nuevoGuardian.PV);

    if (nuevoGuardian.PA + nuevoGuardian.PD + nuevoGuardian.PV != 1000) {
        printf("La suma de los puntos de atributos debe ser 1000. No se ha creado el guardian.\n");
        return;
    }

    jugadorCreador->guardianCreado[jugadorCreador->numGuardianesCreados] = nuevoGuardian;
    jugadorCreador->numGuardianesCreados++;

    printf("Guardian creado y agregado a tu lista de guardianes creados.\n");
}
//<----------------------------------- FIN de Funcion para crear Guardian ------------------------------------------>
//<------------------------------Funciones para mostrar cartas de distintos mazos------------------------------->
void mostrarCartasEnMano(struct Jugador jugador) {
    for (int i = 0; i < Num_Cartas_Iniciales; i++) {
        printf("%d) %s (%s, PV:%d, PA:%d, PD:%d)\n", i+1,jugador.mano[i].guardian.nombre, jugador.mano[i].guardian.tipo, jugador.mano[i].guardian.PV, jugador.mano[i].guardian.PA, jugador.mano[i].guardian.PD);
    }
}
void mostrarCartasElegidas(struct Jugador jugador) {
    printf("Cartas elegidas del jugador %d:\n", jugador.puntosVida);
    for (int i = 0; i < jugador.numCartasEnMano; i++) {
        printf("%d. %s (%s, PV:%d, PA:%d, PD:%d)\n", i + 1, jugador.campoBatalla[i].guardian.nombre,
               jugador.campoBatalla[i].guardian.tipo, jugador.campoBatalla[i].guardian.PV,
               jugador.campoBatalla[i].guardian.PA, jugador.campoBatalla[i].guardian.PD);
    }
}
void mostrarGuardianesCreados(struct JugadorCreador jugadorCreador) {
    printf("Guardianes creados por el Jugador:\n");
    for (int i = 0; i < jugadorCreador.numGuardianesCreados; i++) {
        printf("Guardián %d:\n", i + 1);
        printf("Nombre: %s\n", jugadorCreador.guardianCreado[i].nombre);
        printf("Tipo: %s\n", jugadorCreador.guardianCreado[i].tipo);
        printf("Puntos de Vida (PV): %d\n", jugadorCreador.guardianCreado[i].PV);
        printf("Puntos de Ataque (PA): %d\n", jugadorCreador.guardianCreado[i].PA);
        printf("Puntos de Defensa (PD): %d\n", jugadorCreador.guardianCreado[i].PD);
    }
}
//<-------------------------- FIN DE FUNCIONES PARA MOSTRAR CARTAS ------------------------------>
//<---------------------------------Funcion Agregar cartas ------------------------------->
void agregarPrimeraCartaMazoElegidas(struct Jugador *jugador) {
    // Verificar si hay cartas restantes en el mazo propio
    if (jugador->numCartasEnMano > 0) {
        // Agregar la primera carta del mazo propio al mazo de cartas elegidas
        jugador->campoBatalla[jugador->numCartasEnMano].guardian = jugador->mazoPropio[0];
        jugador->numCartasEnMano++;

        // Eliminar el primer guardián del mazo propio y desplazar los demás
        for (int i = 0; i < jugador->numCartasEnMano - 1; i++) {
            jugador->mazoPropio[i] = jugador->mazoPropio[i + 1];
        }
        jugador->numCartasEnMano--;
    } else {
        printf("No quedan mas cartas en el mazo propio.\n");
    }
}
//<-----------------------------FIN de funcion para agregar cartas--------------------------->
//<---------------------------Funcion Principal para el combate--------------------------------->
void iniciarCombate(struct Jugador *jugador1, struct Jugador *jugador2) {
    int turno = 1; // Variable para llevar el seguimiento del turno
    struct Jugador *jugadorEnTurno = jugador1;
    // Mostrar cartas iniciales en mano al comienzo del combate
    /*printf("Cartas iniciales del Jugador 1:\n");
    mostrarCartasEnMano(*jugador1);
    printf("Cartas iniciales del Jugador 2:\n");
    mostrarCartasEnMano(*jugador2);*/

    // Permitir a ambos jugadores seleccionar la primera carta en el campo de batalla
    printf("------------JUGADOR 1----------\n");
    printf("Selecciona tu PRIMERA para dejarlo en el campo de batalla:\n");
    seleccionarCartaCampoBatalla(jugador1);
    printf("------------JUGADOR 2----------\n");
    printf("Selecciona tu PRIMERA para dejarlo en el campo de batalla:\n");
    seleccionarCartaCampoBatalla(jugador2);

   while (jugador1->puntosVida > 0 && jugador2->puntosVida > 0) {
        printf("--------------GUARIDANES ELEGIDOS--------------\n");

        // Mostrar las cartas en mano al inicio de cada turno
        printf("Cartas en mano del Jugador %d:\n", turno);
        mostrarCartasEnMano(turno == 1 ? *jugador1 : *jugador2);
        printf("-------------------------------------------\n");

        // Cambiar el turno antes de que el jugador realice una acción
        turno = (turno == 1) ? 2 : 1;


        int opcion;
        printf("<---Turno del Jugador %d--->\n", turno);
        printf("Vida del jugador: %d\n", jugadorEnTurno->puntosVida);
        printf("---- Menu de Combate ----\n");
        printf("1. Atacar al oponente\n");
        printf("2. Seleccionar una carta y dejarla en el campo de batalla\n");
        printf("3. Mostrar guardianes disponibles\n");
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                atacarOponente(turno == 1 ? jugador1 : jugador2, turno == 1 ? jugador2 : jugador1, jugador1, jugador2);
                break;
            case 2:
                seleccionarCartaCampoBatalla(turno == 1 ? jugador1 : jugador2);
                break;
            case 3:
                printf("Mostrando las cartas elegidas del Jugador %d:\n", turno);
                printf("------------------------------\n");
                mostrarCartasElegidas(turno == 1 ? *jugador1 : *jugador2);
                printf("------------------------------\n");
                break;
            default:
                printf("Opcion no valida. Elige una opción valida.\n");
        }
    }

    // Verificar quién ha ganado al final del combate
    if (jugador1->puntosVida <= 0) {
        printf("El Jugador 2 ha ganado el juego.\n");
    } else if (jugador2->puntosVida <= 0) {
        printf("El Jugador 1 ha ganado el juego.\n");
    }
}
//<----------------------------Fin de la funcion de combate------------------------------->
//<----------------------- Funcion para seleccionar los guardianes que van a luchar------------------------>
void seleccionarCartaCampoBatalla(struct Jugador *jugador) {
    if (jugador->numCartasEnMano > 0) {
        // Mostrar las cartas disponibles en el mazo de cartas elegidas
        printf("Cartas en el mazo de cartas elegidas:\n");
        for (int i = 0; i < jugador->numCartasEnMano; i++) {
            printf("%d. %s (%s, PV:%d, PA:%d, PD:%d)\n", i + 1, jugador->mano[i].guardian.nombre,
                   jugador->mano[i].guardian.tipo, jugador->mano[i].guardian.PV,
                   jugador->mano[i].guardian.PA, jugador->mano[i].guardian.PD);
        }

        int cartaElegida;
        printf("Elige una carta para dejarla en el campo de batalla (1-%d): ", jugador->numCartasEnMano);
        scanf("%d", &cartaElegida);

        if (cartaElegida >= 1 && cartaElegida <= jugador->numCartasEnMano) {
            // Copia la carta seleccionada al campo de batalla
            jugador->campoBatalla[0] = jugador->mano[cartaElegida - 1];
            jugador->campoBatalla[0].enCampoBatalla = 1;

            // Elimina la carta seleccionada de la mano y desplaza las demás
            for (int i = cartaElegida - 1; i < jugador->numCartasEnMano - 1; i++) {
                jugador->mano[i] = jugador->mano[i + 1];
            }
            jugador->numCartasEnMano--;

            printf("Carta seleccionada y dejada en el campo de batalla.\n");
        } else {
            printf("Selección no válida. Elige una carta válida del 1 al %d.\n", jugador->numCartasEnMano);
        }
    } else {
        printf("No tienes cartas en la mano para seleccionar.\n");
    }
}
//----------------------------- FIN de funcion para seleccionar cartas de combate---------------------------------------->
// Función para que un jugador ataque al oponente seleccionando una carta
void atacarOponente(struct Jugador *atacante, struct Jugador *oponente, struct Jugador *jugador1, struct Jugador *jugador2) {
    if (atacante->campoBatalla[0].enCampoBatalla) {
        printf("\n");
        printf("---TUS GUARDIANES---\n");
        printf("Carta en el campo de batalla del Jugador %d:\n", atacante == jugador1 ? 1 : 2);
        printf("%s (%s, PV:%d, PA:%d, PD:%d)\t\t", atacante->campoBatalla[0].guardian.nombre,
               atacante->campoBatalla[0].guardian.tipo, atacante->campoBatalla[0].guardian.PV,
               atacante->campoBatalla[0].guardian.PA, atacante->campoBatalla[0].guardian.PD);

        if (oponente->campoBatalla[0].enCampoBatalla) {
            printf("-----ENEMIGO------\n");
            printf("--\t\t\t\t\t\t");
            printf("Carta en el campo de batalla del Jugador %d:\n", oponente == jugador1 ? 1 : 2);
            printf("\t\t\t\t\t\t");
            printf("%s (%s, PV:%d, PA:%d, PD:%d)\n", oponente->campoBatalla[0].guardian.nombre,
                   oponente->campoBatalla[0].guardian.tipo, oponente->campoBatalla[0].guardian.PV,
                   oponente->campoBatalla[0].guardian.PA, oponente->campoBatalla[0].guardian.PD);

            printf("Elige una carta para atacar : ");
            int cartaElegida;
            scanf("%d", &cartaElegida);

            if (cartaElegida == 1) {
                // Calcula el daño realizado por el atacante
                int danio = atacante->campoBatalla[0].guardian.PA - oponente->campoBatalla[0].guardian.PD;
                if (danio > 0) {
                    // Restar puntos de vida al oponente
                    oponente->campoBatalla[0].guardian.PV -= danio;
                    if (oponente->campoBatalla[0].guardian.PV <= 0) {
                        // Si los puntos de vida del oponente llegan a cero o menos, retira al oponente del campo de batalla
                        printf("El ataque ha derrotado al oponente.\n");
                        oponente->campoBatalla[0].enCampoBatalla = 0;
                        
                        // Restar un punto de vida al jugador oponente
                        oponente->puntosVida--;
                    } else {
                        printf("El ataque ha infligido %d puntos de vida al oponente.\n", danio);
                    }
                } else {
                    printf("El ataque no hizo efecto ya que la defensa es mayor que el ataque.\n");
                }
            } else {
                printf("Selección no valida. Elige la carta 1 para atacar.\n");
            }
        } else {
            printf("No hay cartas en el campo de batalla del Jugador %d.\n", oponente == jugador1 ? 1 : 2);
        }
    } else {
        printf("No hay cartas en el campo de batalla del Jugador %d.\n", atacante == jugador1 ? 1 : 2);
    }
}
//<----------------- Funcion para mover automaticamente los guardianes ---------------------->
void moverCartaDeMazoAMano(struct Jugador *jugador) {
    // Verificar si hay cartas restantes en el mazo propio
    if (jugador->numCartasEnMano > 0) {
        // Crear una instancia de Carta y asignar el Guardian
        struct Carta nuevaCarta;
        nuevaCarta.guardian = jugador->mazoPropio[0];

        // Agregar la nueva Carta a la mano
        jugador->mano[jugador->numCartasEnMano] = nuevaCarta;
        jugador->numCartasEnMano++;

        // Eliminar el primer guardián del mazo propio y desplazar los demás
        for (int i = 0; i < jugador->numCartasEnMano - 1; i++) {
            jugador->mazoPropio[i] = jugador->mazoPropio[i + 1];
        }
        jugador->numCartasEnMano--;
    } else {
        printf("No quedan mas cartas en el mazo propio.\n");
    }
}
