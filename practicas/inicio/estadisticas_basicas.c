#include <stdio.h>

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
        int sum = 0;

        while(argv[i])
        {
            int num = ft_atoi(argv[i]);
            sum += num;

            if (num > max_num)
                max_num = num;
            if (num < min_num)
                min_num = num;

            i++;
        }
        int media = sum / (argc - 1);

        printf("El numero mas alto es %d\n", max_num);
        printf("El numero mas bajo es %d\n", min_num);
        printf("La media es %d\n", media);
    }
    return(0);
}