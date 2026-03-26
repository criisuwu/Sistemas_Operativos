La primera practica de la asignatura de _Sistemas Operativos_ de la carrera *Ingenieria de Robotica software* de la universidad *Rey Juan Carlos*

-Enunciado:

        Escribe un programa en C llamado myfgrep.c que admita un mínimo de dos argumentos. De los N argumentos recibidos, los primeros N-1 serán palabras a buscar en un fichero. El último argumento es una ruta a un fichero.

        El programa escribirá, una única vez, las líneas que contengan en cualquier posición alguna de las palabras como subcadena en el formato del ejemplo (número de línea, dos puntos, y la línea).
        Realizará su labor ejecutando el comando fgrep con los parámetros adecuados.
        
        En caso de haber cualquier error (permisos, argumentos...) saldrá con estado fallido y escribirá un mensaje de error por la salida estándar de error.
        
        Como no puedes esperar a la salida de los comandos, una vez lanzados (ya veremos wait y similares), no te preocupes por esto. Además puede suceder que el prompt se escriba mezclado con la salida del programa. No te preocupes de esto.
        
        No se puede usar system ni ejecutar un programa externo que no sea fgrep.
        
        Un ejemplo de ejecución:
        
        $> echo 'hola adios
        bla ble bli
        xholap adios' > /tmp/a
        $> myfgrep hola zz /tmp/a
        1:hola adios
        3:xholap adios
        $> myfgrep hola adios /tmp/a
        1:hola adios
        3:xholap adios
        $> echo $?
        0
        $> myfgrep rr zz /tmp/a
        $> echo $?
        0
        $> myfgrep rr
        usage: myfgrep word [word]... file
        $> echo $?
        1
        