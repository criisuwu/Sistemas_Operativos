La cuarta practica de la asignatura de _Sistemas Operativos_ de la carrera *Ingenieria de Robotica software* de la universidad *Rey Juan Carlos*

-Enunciado:
    Escriba un programa mysplit.c  en C para Linux similar al comando split(1), que reciba como argumentos obligatorios un tamaño N en bytes y un fichero. El programa cortará el fichero en varios ficheros que contengan como mucho N bytes y cuyos nombres que sean el nombre del fichero original precedido por un número de 3 dígitos que comienza en 000 y representa el orden en el que aparecen los datos de ese fichero en el original.

    Si alguno de los ficheros que se va a crear existen, se truncarán y sobrescribirán (no será un error). Si no hubiese suficientes argumentos o fuesen incorrectos (no existe el fichero, etc.), escribirá su uso por la salida de error y saldrá con estado erróneo.
    
    Por ejemplo:
    
    $> seq 0 9 | tr -d '\n' > fich
    $> mysplit 3 fich
    $> ls -l
    total 20
    -rw-rw-r-- 1 paurea paurea  3 nov 11 10:55 000fich
    -rw-rw-r-- 1 paurea paurea  3 nov 11 10:55 001fich
    -rw-rw-r-- 1 paurea paurea  3 nov 11 10:55 002fich
    -rw-rw-r-- 1 paurea paurea  1 nov 11 10:55 003fich
    -rw-rw-r-- 1 paurea paurea 10 nov 11 10:55 fich
    $> cat 000fich
    012$> cat 001fich
    345$> cat 002fich
    678$> cat 003fich
    9$>
    $> mysplit 7
    usage: mysplit N file
    $>
    
    Nota: asegúrate de probarlo con ficheros grandes (de texto y binarios), por ejemplo:
    
    $> cp /usr/bin/ls .
    $> mysplit 30000 ls
    $> cat 0*ls > newls
    $> cmp ls newls    #no dice nada, mi split funciona en este caso
    $>
    
    Es obligatorio implementar el programa usando las llamadas al sistema read(2) y write(2) directamente, no se permiten otras operaciones para leer o escribir los ficheros.
    
    El programa no puede ejecutar ningún otro programa externo (ni una shell). Se recomienda usar las funciones strtol(3) y snprintf(3).