#include <iostream>
#include <string>
#include <chrono> // Librería para medir tiempo

using namespace std;

struct Jugador {
    string nombre;
    int puntaje;
};

Jugador jugadores[2]; // Solo hay dos jugadores
int numeroJugadores = 0;

// La realización de estas funciones estás al final del código
void mostrarMenu();
void jugar();
void mostrarPuntajes();
void mostrarCreditos();
void agregarJugador(const string& nombre);
void actualizarPuntaje(const string& nombre, int puntaje);

class Turtuntitos {
private:
    int filas, cols;
    int** horizontalBordes; // Puntero de un puntero (hacer arrays bidimensionales dinámicas)
    int** verticalBordes;
    int** cuadrados;
    int PlayerJugando; // Qué jugador tiene el turno actual en el juego
    int scores[2]; // Máximo de puntos o cuadrados hechos (2)

    bool checkAndMarkSquare(int fil, int col) {
        if (fil >= 0 && col >= 0 && horizontalBordes[fil][col] && horizontalBordes[fil + 1][col] &&
            verticalBordes[fil][col] && verticalBordes[fil][col + 1]) {
            cuadrados[fil][col] = PlayerJugando;
            scores[PlayerJugando]++;
            return true;
        }
        return false;
    }

    void switchPlayer() {
        PlayerJugando = 1 - PlayerJugando;
    }

public:
    Turtuntitos(int n) : filas(n), cols(n), PlayerJugando(0) {
        // Después del ':' se definen la lista de inicialización del constructor
        horizontalBordes = new int*[n + 1];
        for (int i = 0; i <= n; i++) {
            horizontalBordes[i] = new int[n]{0};
        }

        verticalBordes = new int*[n];
        for (int i = 0; i < n; i++) {
            verticalBordes[i] = new int[n + 1]{0};
        }

        cuadrados = new int*[n];
        for (int i = 0; i < n; i++) {
            cuadrados[i] = new int[n];
            for (int j = 0; j < n; j++) {
                cuadrados[i][j] = -1;
            }
        }

        scores[0] = 0;
        scores[1] = 0;
    }

    ~Turtuntitos() {
        for (int i = 0; i <= filas; i++) {
            delete[] horizontalBordes[i];
        }
        delete[] horizontalBordes;

        for (int i = 0; i < filas; i++) {
            delete[] verticalBordes[i];
        }
        delete[] verticalBordes;

        for (int i = 0; i < filas; i++) {
            delete[] cuadrados[i];
        }
        delete[] cuadrados;
    }

    bool dibujarEsquinas(char tipo, int fil, int col) {
        if (tipo == 'H') {
            if (fil >= 0 && fil <= filas && col >= 0 && col < cols && horizontalBordes[fil][col] == 0) {
                horizontalBordes[fil][col] = 1;
                if (!checkAndMarkSquare(fil - 1, col) && !checkAndMarkSquare(fil, col)) {
                    switchPlayer();
                }
                return true;
            }
        } else if (tipo == 'V') {
            if (fil >= 0 && fil < filas && col >= 0 && col <= cols && verticalBordes[fil][col] == 0) {
                verticalBordes[fil][col] = 1;
                if (!checkAndMarkSquare(fil, col - 1) && !checkAndMarkSquare(fil, col)) {
                    switchPlayer();
                }
                return true;
            }
        }
        return false;
    }

    void imprimirTablero() {
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < cols; j++) {
                cout << "+";
                if (horizontalBordes[i][j]) {
                    cout << "---";
                } else {
                    cout << "   ";
                }
            }
            cout << "+" << endl;
            for (int j = 0; j < cols; j++) {
                if (verticalBordes[i][j]) {
                    cout << "|";
                } else {
                    cout << " ";
                }
                if (cuadrados[i][j] != -1) {
                    cout << " " << cuadrados[i][j] << " ";
                } else {
                    cout << "   ";
                }
            }
            if (verticalBordes[i][cols]) {
                cout << "|" << endl;
            } else {
                cout << " " << endl;
            }
        }
        for (int j = 0; j < cols; j++) {
            cout << "+";
            if (horizontalBordes[filas][j]) {
                cout << "---";
            } else {
                cout << "   ";
            }
        }
        cout << "+" << endl;
    }

    bool isGameOver() {
        // Verifica si un jugador tiene una diferencia de 2 puntos
        if (abs(scores[0] - scores[1]) >= 2) return true;

        // Verifica si todos los cuadrados están completados
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < cols; j++) {
                if (cuadrados[i][j] == -1) return false;
            }
        }
        return true;
    }

    int Ganador() {
        if (scores[0] > scores[1]) return 0;
        if (scores[1] > scores[0]) return 1;
        return -1; // Empate
    }

    void imprimirScores() {
        cout << "Scores: " << jugadores[0].nombre << ": " << scores[0] << ", " << jugadores[1].nombre << ": " << scores[1] << endl;
    }

    int getPlayerJugando() const {
        return PlayerJugando;
    }
};

int main() {
    mostrarMenu();
    return 0;
}

void mostrarMenu() {
    int opcion;
    do {
        cout << "Menu:\n";
        cout << "1. Jugar\n";
        cout << "2. Puntajes\n";
        cout << "3. Salir\n";
        cout << "4. Creditos\n";
        cout << "Selecciona una opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1:
                jugar();
                break;
            case 2:
                mostrarPuntajes();
                break;
            case 3:
                cout << "Saliendo del juego...\n";
                break;
            case 4:
                mostrarCreditos();
                break;
            default:
                cout << "Opcion no valida, intenta nuevamente.\n";
        }
    } while(opcion != 3);
}

void jugar() {
    string nombre;
    for (int i = 0; i < 2; i++) {
        cout << "Introduce el nombre del jugador " << i << ": ";
        cin >> nombre;
        agregarJugador(nombre);
    }

    int n;
    cout << "Introduce el tamaño de la cuadrícula (n): ";
    cin >> n;

    Turtuntitos game(n);
    char tipo;
    int fil, col;

    auto start = chrono::high_resolution_clock::now(); // Inicio del tiempo

    while (!game.isGameOver()) {
        game.imprimirTablero();
        game.imprimirScores();
        cout << jugadores[game.getPlayerJugando()].nombre << ", introduce tu movimiento (tipo fila columna): ";
        cin >> tipo >> fil >> col;
        if (!game.dibujarEsquinas(tipo, fil, col)) {
            cout << "Movimiento invalido, intenta nuevamente." << endl;
        }
    }

    auto end = chrono::high_resolution_clock::now(); // Fin del tiempo
    chrono::duration<double> duracion = end - start;

    game.imprimirTablero();
    game.imprimirScores();
    int winner = game.Ganador();
    if (winner == -1) {
        cout << "¡Es un empate!" << endl;
    } else {
        cout << "¡" << jugadores[winner].nombre << " gana!" << endl;
        cout << "Tiempo total: " << duracion.count() << " segundos." << endl;
        actualizarPuntaje(jugadores[winner].nombre, static_cast<int>(duracion.count()));
    }
}

void mostrarPuntajes() {
    cout << "Puntajes de los jugadores:\n";
    for (int i = 0; i < numeroJugadores; i++) {
        cout << "Nombre: " << jugadores[i].nombre << ", Puntaje: " << jugadores[i].puntaje << " segundos." << endl;
    }
}

void mostrarCreditos() {
    cout << "Creditos:\n";
    cout << "Integrante 1: GARY MATEO DEFAS CONDOR\n";
    cout << "Integrante 2: LINDSAY VALENTINA GUZMAN QUILUMBA\n";
}

void agregarJugador(const string& nombre) {
    if (numeroJugadores < 2) {
        jugadores[numeroJugadores].nombre = nombre;
        jugadores[numeroJugadores].puntaje = 0;
        numeroJugadores++;
    }
}

void actualizarPuntaje(const string& nombre, int puntaje) {
    for (int i = 0; i < numeroJugadores; i++) {
        if (jugadores[i].nombre == nombre) {
            jugadores[i].puntaje = puntaje;
            break;
        }
    }
}
