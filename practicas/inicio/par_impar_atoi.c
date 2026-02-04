#include <stdio.h>

int ft_atoi(char *str)
{
    int res = 0;
    int i = 0;
    int s = 1;

    while (str[i] == ' ')
        i++;

    if(str[i] == '-')
    {
        s = -1;
        i++;
    }    

    while(str[i] >= 48 && str[i] <= 57)
    {
        res *= 10;
        res += str[i] - 48;
        i++;
    }
    return (res *= s);
}

int main(int argc, char **argv)
{
    if(argc != 1)
    {
        int rest = 0;
        int num = ft_atoi(argv[1]);
        rest = num % 2;
        if (rest != 0)
            printf("El numero %d es impar!\n", num);
        else
            printf("El numero %d es par!\n", num);
    }
    else
        printf("\n");
    return(0);
}
