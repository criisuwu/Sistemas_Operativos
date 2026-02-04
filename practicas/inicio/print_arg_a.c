#include <unistd.h>

// ! Preguntar en caso de que sean en argumentos distintos.
int main(int argc, char **argv)
{
	if(argc != 1)
	{
		int i = 0;
		while(argv[1][i]) //Interpreto que todos los argumentos son pasados en un solo argumento
		{
			while(argv[1][i] != ' ' && argv[1][i])
			{
				write(1, &argv[1][i], 1);
				i++;
			}
			if(argv[1][i] == ' ')
			{
				i++;
				write(1, "\n", 1);
			}
		}
		write(1, "\n", 1);        
	}
	return(0);
}
