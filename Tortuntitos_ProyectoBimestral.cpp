#include <iostream>
#include <chrono> // Librería para medir tiempo
using namespace std;

//************************* DECLARACIÓN DE VARIABLES Y STRUCTS ************************

struct Jugador {
    string nombre;
    int puntaje;
};

Jugador jugadores[2]; //Solo hay dos jugadores
int numeroJugadores=0;

struct Tortuntitos{
    int filas, cols;
    int bordesHorizontales[4][4]; //El valor máximo de la cuadrícula será 4x4
    int bordesVerticales[4][4];
    int cuadrados[4][4];
    int jugadorActual;      //Jugador que tiene el turno actual
    int puntajes[2];    //Puntos o cuadrados completados por cada jugador
};

//************************* DECLARACIÓN DE FUNCIONES ************************

// FUNCIONES DEL MENÚ
void mostrarMenu(); //Las funciones serán llamadas respecto a esta función

void jugar();
void mostrarPuntajes();
void mostrarCreditos();
void agregarJugador(const string &nombre);
void actualizarPuntaje(const string &nombre, int puntaje);

// FUNCIONES DEL FUNCIONAMIENTO DEL JUEGO
Tortuntitos inicializarJuego(int n);
void eliminarJuego(Tortuntitos &juego);
bool dibujarLinea(Tortuntitos &juego, char tipo, int fil, int col);
bool chequearCuadrado(Tortuntitos &juego, int fil, int col);
void cambiarJugador(Tortuntitos &juego);
void imprimirTablero(Tortuntitos &juego);
bool juegoTerminado(Tortuntitos &juego);
int determinarGanador(Tortuntitos &juego);
void imprimirPuntajes(Tortuntitos &juego);

//************************* COMIENZO DEL JUEGO ************************

int main() {
    mostrarMenu();
    return 0;
}

//*********************** CREACION DE FUNCIONES *************************

void mostrarMenu() {
    int opcion;
    do {
        cout<<"Menu:"<<endl;
        cout<<"1. Jugar"<<endl;
        cout<<"2. Puntajes"<<endl;
        cout<<"3. Salir"<<endl;
        cout<<"4. Creditos"<<endl;
        cout<<"Selecciona una opcion: ";
        cin>>opcion;

        switch(opcion){
            case 1:
                jugar();
                break;
            case 2:
                mostrarPuntajes();
                break;
            case 3:
                cout<<"Saliendo del juego..."<<endl;
                break;
            case 4:
                mostrarCreditos();
                break;
            default:
                cout<<"Opcion no valida, intenta nuevamente."<<endl;
        }
    } while(opcion!=3);
}

void jugar() {
    string nombre;
    for (int i=0; i<2; i++) {
        cout<<"Introduce el nombre del jugador "<< i+1 <<": ";
        cin>>nombre;
        agregarJugador(nombre);
    }

    int n;
    cout<<"Introduce el tamaño de la cuadrícula (n): ";
    cin>>n;

    Tortuntitos juego=inicializarJuego(n);

    char tipo;
    int fil,col;

    auto inicio = chrono::high_resolution_clock::now(); // Inicio del tiempo

    while (!juegoTerminado(juego)) {
        imprimirTablero(juego);
        imprimirPuntajes(juego);
        cout << jugadores[juego.jugadorActual].nombre
        <<", introduce tu movimiento (tipo fila columna): ";
        cin >> tipo >> fil >> col;
        if (!dibujarLinea(juego, tipo, fil, col)) {
            cout << "Movimiento invalido, intenta nuevamente." << endl;
        }
    }
    auto fin = chrono::high_resolution_clock::now(); // Fin del tiempo
    chrono::duration<double>duracion = fin - inicio;
    
    imprimirTablero(juego);
    imprimirPuntajes(juego);
    int ganador = determinarGanador(juego);
    if (ganador == -1) {
        cout << "¡Es un empate!"<<endl;
    } else {
        cout<<"¡"<<jugadores[ganador].nombre<<" gana!"<<endl;
        cout<<"Tiempo total: "<<duracion.count()<<" segundos."<<endl;
        actualizarPuntaje(jugadores[ganador].nombre, static_cast<int>(duracion.count()));
    }

    eliminarJuego(juego);
}

void mostrarPuntajes() {
    cout<<"Puntajes de los jugadores:\n";
    for (int i=0; i<numeroJugadores; i++) {
        cout<<"Nombre: "<<jugadores[i].nombre<<", Puntaje: "<< 
        jugadores[i].puntaje<<" segundos."<<endl;
    }
}

void mostrarCreditos() {
    cout << "Creditos:\n";
    cout << "Integrante 1: GARY MATEO DEFAS CONDOR\n";
    cout << "Integrante 2: LINDSAY VALENTINA GUZMAN QUILUMBA\n";
}

void agregarJugador(const string &nombre) {
    if (numeroJugadores < 2) {
        jugadores[numeroJugadores].nombre = nombre;
        jugadores[numeroJugadores].puntaje = 0;
        numeroJugadores++;
    }
}

void actualizarPuntaje(const string &nombre, int puntaje) {
    for (int i=0; i<numeroJugadores; i++) {
        if (jugadores[i].nombre == nombre) {
            jugadores[i].puntaje = puntaje;
            break;
        }
    }
}

Tortuntitos inicializarJuego(int n){
    //creo una variable de tipo tortuntitos 
    Tortuntitos juego;
    
    juego.filas=n;
    juego.cols=n;
    juego.jugadorActual= 0;

    //dando valores a los arrays de los bordes verticales, horizontales y cuadrados
    for (int i=0; i<=n; i++) {
        for (int j=0; j<n; j++) {
            juego.bordesHorizontales[i][j] = 0;
        }
    }

    for (int i=0; i<n; i++) {
        for (int j=0; j<=n; j++) {
            juego.bordesVerticales[i][j] = 0;
        }
    }

    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            juego.cuadrados[i][j] = -1;
        }
    }

    //Tanto el jugado 0 como el jugador 1 incian con 0 puntos.
    juego.puntajes[0] = 0;
    juego.puntajes[1] = 0;

    return juego;
}

void eliminarJuego(Tortuntitos &juego) {
    //No es necesario liberar memoria ya que no se usan punteros dinámicos
}



bool dibujarLinea(Tortuntitos &juego, char tipo, int fil, int col) {
    if (tipo == 'H') {
        if (fil >= 0 && fil <= juego.filas && col >= 0 && col < juego.cols && 
        juego.bordesHorizontales[fil][col] == 0) {
            juego.bordesHorizontales[fil][col] = 1;
            if (!chequearCuadrado(juego, fil - 1, col) && 
            !chequearCuadrado(juego, fil, col)) {
                cambiarJugador(juego);
            }
            return true;
        }
    } else if (tipo == 'V'){
        if (fil >= 0 && fil < juego.filas && col >= 0 && col <= juego.cols && 
        juego.bordesVerticales[fil][col] == 0) {
            juego.bordesVerticales[fil][col] = 1;
            if (!chequearCuadrado(juego, fil, col - 1) && !chequearCuadrado(juego, fil, col)) {
                cambiarJugador(juego);
            }
            return true;
        }
    }
    return false;
}

bool chequearCuadrado(Tortuntitos &juego, int fil, int col){
    if (fil >= 0&&col >= 0 && juego.bordesHorizontales[fil][col] && 
        juego.bordesHorizontales[fil + 1][col] &&
        juego.bordesVerticales[fil][col] && juego.bordesVerticales[fil][col + 1]) {
        juego.cuadrados[fil][col] = juego.jugadorActual;
        juego.puntajes[juego.jugadorActual]++;
        return true;
    }
    return false;
}

void cambiarJugador(Tortuntitos &juego) {
    juego.jugadorActual = 1 - juego.jugadorActual;
}

void imprimirTablero(Tortuntitos &juego) {
    for (int i = 0; i < juego.filas; i++) {
        for (int j = 0; j < juego.cols; j++) {
            cout<<"+";
            if(juego.bordesHorizontales[i][j]) {
                cout<<"---";
            }else {
                cout<<"   ";
            }
        }
        cout<<"+"<<endl;
        for (int j = 0; j < juego.cols; j++) {
            if(juego.bordesVerticales[i][j]) {
                cout<< "|";
            }else{
                cout<<" ";
            }
            if(juego.cuadrados[i][j] != -1){
                cout<<" "<<juego.cuadrados[i][j]<<" ";
            }else{
                cout<<"   ";
            }
        }
        if (juego.bordesVerticales[i][juego.cols]) {
            cout<<"|"<<endl;
        } else {
            cout<<" "<<endl;
        }
    }
    for (int j=0; j<juego.cols; j++) {
        cout<<"+";
        if (juego.bordesHorizontales[juego.filas][j]) {
            cout<<"---";
        } else {
            cout<<"   ";
        }
    }
    cout << "+" << endl;
}

bool juegoTerminado(Tortuntitos &juego) {
    // Verifica si un jugador tiene una diferencia de 2 puntos
    if (abs(juego.puntajes[0] - juego.puntajes[1])>=2){ 
        return true;
    }

    // Verifica si todos los cuadrados están completados
    for (int i=0; i<juego.filas; i++){
        for (int j=0; j<juego.cols; j++){
            if (juego.cuadrados[i][j]==-1){ 
            return false;
            }
        }
    }
    return true;
}

int determinarGanador(Tortuntitos &juego) {
    if (juego.puntajes[0]>juego.puntajes[1]){ 
        return 0;
    }
    if (juego.puntajes[1]>juego.puntajes[0]){ 
        return 1;
    }
    return -1; // Empate
}

void imprimirPuntajes(Tortuntitos &juego) {
    cout<<"Puntajes: "<<jugadores[0].nombre<< ": "<<juego.puntajes[0]<<", "<<
    jugadores[1].nombre<<": "<<juego.puntajes[1]<<endl;
}

