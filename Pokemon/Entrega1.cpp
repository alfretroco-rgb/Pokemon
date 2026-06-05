#include <iostream>
#include <string>

using namespace std;

const int TAM_EQUIPO = 3;
const int TAM_MOCHILA = 3;

enum Tipo {
    FUEGO,
    AGUA,
    PLANTA,
    ELECTRICO,
    ROCA
};

struct Pokemon {
    string nombre;
    string especie;
    Tipo tipo;
    int vidaMaxima;
    int vida;
    int ataque;
    int defensa;
};

struct Objeto {
    string nombre;
    int cura;
    int cantidad;
};

struct Entrenador {
    string nombre;
    Pokemon* equipo[TAM_EQUIPO];
    Objeto mochila[TAM_MOCHILA];
    int activo;
};

string tipoTexto(Tipo tipo) {
    if (tipo == FUEGO) return "Fuego";
    if (tipo == AGUA) return "Agua";
    if (tipo == PLANTA) return "Planta";
    if (tipo == ELECTRICO) return "Electrico";
    return "Roca";
}

Pokemon* crearPokemon(string nombre, string especie, Tipo tipo, int vida, int ataque, int defensa) {
    Pokemon* p = new Pokemon;
    p->nombre = nombre;
    p->especie = especie;
    p->tipo = tipo;
    p->vidaMaxima = vida;
    p->vida = vida;
    p->ataque = ataque;
    p->defensa = defensa;
    return p;
}

void liberarEntrenador(Entrenador& entrenador) {
    for (int i = 0; i < TAM_EQUIPO; i++) {
        delete entrenador.equipo[i];
        entrenador.equipo[i] = nullptr;
    }
}

bool estaVivo(Pokemon* p) {
    return p->vida > 0;
}

int encontrarVivo(Entrenador& entrenador) {
    for (int i = 0; i < TAM_EQUIPO; i++) {
        if (estaVivo(entrenador.equipo[i])) {
            return i;
        }
    }
    return -1;
}

void mostrarPokemon(const Pokemon* p, int numero, bool activo) {
    cout << "  [" << numero + 1 << "] " << p->nombre << " (" << p->especie << ")";
    cout << " - Tipo: " << tipoTexto(p->tipo);
    cout << " - Vida: " << p->vida << "/" << p->vidaMaxima;
    cout << " - Ataque: " << p->ataque;
    cout << " - Defensa: " << p->defensa;
    if (activo) {
        cout << " <-- activo";
    }
    cout << endl;
}

void mostrarEquipo(const Entrenador& entrenador) {
    cout << endl << "Equipo de " << entrenador.nombre << endl;
    for (int i = 0; i < TAM_EQUIPO; i++) {
        mostrarPokemon(entrenador.equipo[i], i, i == entrenador.activo);
    }
}

void mostrarMochila(const Entrenador& entrenador) {
    cout << endl << "Mochila de " << entrenador.nombre << endl;
    for (int i = 0; i < TAM_MOCHILA; i++) {
        cout << "  [" << i + 1 << "] " << entrenador.mochila[i].nombre;
        cout << " - cura " << entrenador.mochila[i].cura;
        cout << " - cantidad " << entrenador.mochila[i].cantidad << endl;
    }
}

bool tipoEfectivo(Tipo atacante, Tipo defensor) {
    if ((atacante == FUEGO && defensor == PLANTA) ||
        (atacante == AGUA && defensor == FUEGO) ||
        (atacante == PLANTA && defensor == AGUA) ||
        (atacante == ELECTRICO && defensor == AGUA) ||
        (atacante == ROCA && defensor == FUEGO)) {
        return true;
    }
    return false;
}

int calcularDano(Pokemon* atacante, Pokemon* defensor) {
    int dano = atacante->ataque - defensor->defensa / 2;
    if (dano < 1) {
        dano = 1;
    }

    if (tipoEfectivo(atacante->tipo, defensor->tipo)) {
        dano += 5;
    }

    return dano;
}

void atacar(Pokemon* atacante, Pokemon* defensor) {
    int dano = calcularDano(atacante, defensor);
    defensor->vida -= dano;

    if (defensor->vida < 0) {
        defensor->vida = 0;
    }

    cout << atacante->nombre << " ataca y hace " << dano << " de dano." << endl;
    if (defensor->vida == 0) {
        cout << defensor->nombre << " se debilito." << endl;
    }
}

void curar(Pokemon* p, int cantidad) {
    p->vida += cantidad;
    if (p->vida > p->vidaMaxima) {
        p->vida = p->vidaMaxima;
    }
}

void usarObjeto(Entrenador& entrenador) {
    mostrarMochila(entrenador);
    cout << "Elige el numero del objeto: ";

    int opcion;
    cin >> opcion;

    if (opcion < 1 || opcion > TAM_MOCHILA) {
        cout << "Objeto invalido." << endl;
        return;
    }

    Objeto& obj = entrenador.mochila[opcion - 1];
    if (obj.cantidad <= 0) {
        cout << "No quedan objetos." << endl;
        return;
    }

    curar(entrenador.equipo[entrenador.activo], obj.cura);
    obj.cantidad--;
    cout << entrenador.nombre << " usa " << obj.nombre << " y recupera vida." << endl;
}

void cambiarPokemon(Entrenador& entrenador) {
    mostrarEquipo(entrenador);
    cout << "Elige el numero del Pokemon: ";

    int opcion;
    cin >> opcion;

    if (opcion < 1 || opcion > TAM_EQUIPO) {
        cout << "Pokemon invalido." << endl;
        return;
    }

    if (!estaVivo(entrenador.equipo[opcion - 1])) {
        cout << "Ese Pokemon esta debilitado." << endl;
        return;
    }

    entrenador.activo = opcion - 1;
    cout << "Ahora usa a " << entrenador.equipo[entrenador.activo]->nombre << endl;
}

void crearJugador(Entrenador& jugador) {
    jugador.nombre = "Jugador";
    jugador.activo = 0;

    jugador.mochila[0] = {"Pocion", 30, 3};
    jugador.mochila[1] = {"Super Pocion", 50, 1};
    jugador.mochila[2] = {"Baya", 20, 2};

    jugador.equipo[0] = crearPokemon("Blaze", "Charmander", FUEGO, 55, 18, 10);
    jugador.equipo[1] = crearPokemon("Splash", "Squirtle", AGUA, 60, 16, 12);
    jugador.equipo[2] = crearPokemon("Leafy", "Bulbasaur", PLANTA, 58, 17, 11);
}

void crearCPU(Entrenador& cpu) {
    cpu.nombre = "CPU";
    cpu.activo = 0;

    cpu.mochila[0] = {"Pocion", 30, 2};
    cpu.mochila[1] = {"Super Pocion", 50, 1};
    cpu.mochila[2] = {"Baya", 20, 1};

    cpu.equipo[0] = crearPokemon("Spark", "Pikachu", ELECTRICO, 50, 19, 9);
    cpu.equipo[1] = crearPokemon("Stone", "Geodude", ROCA, 65, 17, 14);
    cpu.equipo[2] = crearPokemon("Aqua", "Poliwag", AGUA, 57, 16, 10);
}

void autoCambiar(Entrenador& entrenador) {
    if (entrenador.activo >= 0 && estaVivo(entrenador.equipo[entrenador.activo])) {
        return;
    }

    entrenador.activo = encontrarVivo(entrenador);
    if (entrenador.activo != -1) {
        cout << entrenador.nombre << " envia a " << entrenador.equipo[entrenador.activo]->nombre << endl;
    }
}

void batalla(Entrenador& jugador, Entrenador& cpu) {
    while (encontrarVivo(jugador) != -1 && encontrarVivo(cpu) != -1) {
        autoCambiar(jugador);
        autoCambiar(cpu);

        Pokemon* pJugador = jugador.equipo[jugador.activo];
        Pokemon* pCPU = cpu.equipo[cpu.activo];

        cout << endl << "----------------------------------------" << endl;
        cout << "Tu Pokemon: " << pJugador->nombre << " | Vida " << pJugador->vida << "/" << pJugador->vidaMaxima << endl;
        cout << "Pokemon CPU: " << pCPU->nombre << " | Vida " << pCPU->vida << "/" << pCPU->vidaMaxima << endl;
        cout << "----------------------------------------" << endl;

        cout << "1. Atacar" << endl;
        cout << "2. Usar objeto" << endl;
        cout << "3. Cambiar Pokemon" << endl;
        cout << "Elige una opcion: ";

        int opcion;
        cin >> opcion;

        if (opcion == 2) {
            usarObjeto(jugador);
        } else if (opcion == 3) {
            cambiarPokemon(jugador);
        } else {
            atacar(pJugador, pCPU);
        }

        if (encontrarVivo(cpu) == -1) {
            break;
        }

        autoCambiar(jugador);
        autoCambiar(cpu);

        if (encontrarVivo(jugador) != -1 && encontrarVivo(cpu) != -1) {
            atacar(cpu.equipo[cpu.activo], jugador.equipo[jugador.activo]);
        }
    }

    if (encontrarVivo(jugador) != -1) {
        cout << endl << "Ganaste la batalla." << endl;
    } else {
        cout << endl << "La CPU gano la batalla." << endl;
    }
}

// (Se eliminó la funcionalidad de "Datos iniciales" por simplicidad)

void mostrarMenu() {
    cout << endl << "========================================" << endl;
    cout << "  SIMULADOR DE BATALLA POKEMON" << endl;
    cout << "========================================" << endl;
    cout << "1. Batalla" << endl;
    cout << "2. Mochila" << endl;
    cout << "3. Ver equipos" << endl;
    cout << "4. Salir" << endl;
    cout << "Elige una opcion: ";
}

int main() {
    Entrenador jugador;
    Entrenador cpu;

    crearJugador(jugador);
    crearCPU(cpu);

    int opcion = 0;

    do {
        mostrarMenu();
        cin >> opcion;

        if (opcion == 1) {
            batalla(jugador, cpu);
        } else if (opcion == 2) {
            mostrarMochila(jugador);
        } else if (opcion == 3) {
            mostrarEquipo(jugador);
            mostrarEquipo(cpu);
        } else if (opcion != 4) {
            cout << "Opcion invalida." << endl;
        }
    } while (opcion != 4);

    liberarEntrenador(jugador);
    liberarEntrenador(cpu);
    cout << "Saliendo del juego." << endl;
    return 0;
}
