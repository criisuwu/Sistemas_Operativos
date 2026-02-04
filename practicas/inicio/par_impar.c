#include <stdio.h>
#include <stdlib.h> // No se si puedo usar esta libreria

int main(int argc, char **argv)
{
    if(argc != 1)
    {
        int rest = 0;
        int num = atoi(argv[1]); // En caso de no poder usar el atoi lo crearia.
        rest = num % 2;
        if (rest != 0)
            printf("El numero %d es impar!\n", num);
        else
            printf("El numero %d es par!\n", num);
    }
    printf("\n");
    return(0);
}