#include <unistd.h>

//Otra version pero con la libreria unistd y con write
int main()
{
    write(1, "Hola mundo!\n", 12); // El primer numero es la salida, depues el texto y el ultimo es el numero de caracteres + el null
    return(0);
}