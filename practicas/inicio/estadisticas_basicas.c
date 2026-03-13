#include <stdio.h>
#include <stdlib.h> //Libreria del malloc para reservar memoria

int ft_atoi(char *str)
{
    int rest = 0;
    int sign = 1;
    int i = 0;

    while (str[i] == ' ')
        i++;
    if(str[i] == '-')
    {
        sign *= -1;
        i++;
    }
    while (str[i] >= 48 && str[i] <= 57)
    {
        rest *= 10;
        rest += str[i] - 48;
        i++;
    }
    return(rest * sign);
}

int main(int argc, char **argv)
{
    if(argc >= 2)
    {
        int i = 1;
        int max_num = 0;
        int min_num = 0;
        int j = 0;
        int *lista = malloc(sizeof(int) * argc); //El size of es para el tipo de dato, Hago uso del argc sin necesidad de sumar porque argc cuenta con el ejecutable por lo que no hace falta que añada una posicion mas para el null
        if(!lista)
            return (free(lista), '\0'); //Si no se guarda bien liberamos y devolvemos NULL

        lista = ft_ordenar(argc, argv[0]);
        while(argv[i])
        {
            int num = ft_atoi(argv[i]);
            lista[j] = num;

            if (num > max_num)
                max_num = num;
            if (num < min_num) 
                min_num = num;

            i++;
            j++;
        }

        printf("El numero mas alto es %d\n", max_num);
        printf("El numero mas bajo es %d\n", min_num);
    }
    return(0);
}