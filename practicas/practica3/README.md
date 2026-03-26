La tercera practica de la asignatura de _Sistemas Operativos_ de la carrera *Ingenieria de Robotica software* de la universidad *Rey Juan Carlos*

-Enunciado:

        Escriba un programa llamado and.c en C para GNU/Linux que ejecute los comandos que se le pasan como argumentos. Cada argumento es en realidad un comando y sus argumentos.
        
        El programa debe ejecutar un comando tras otro siempre que terminen con estatus de éxito. El programa debe acabar con el status del último comando ejecutado. Esto es, esta
        ejecución:
        
        $> ./and cmd1 cmd2 cmd3
        
        ejecutaría el comando cmd1, y si termina con éxito, el comando cmd2, y si termina con éxito, el comando cmd3. El estatus de salida sería el del comando cmd3. Esto es, sería similar a ejecutar esto en la shell:
        
        $> cmd1 && cmd2 && cmd3
        
        El programa debe buscar los comandos en el directorio /bin.
        
        Si no se le pasan al menos dos argumentos, tiene que fallar indicando la forma de uso.
        
        Por ejemplo:
        
        $> ./and
        usage: and cmd cmd [cmd ...]
        $> echo $?
        1
        $> ./and 'echo hola'  'echo adios'
        hola
        adios
        $> echo $?
        0
        $> ./and 'echo hola' 'ls /noexisto' 'echo adios'
        hola
        ls: cannot access '/noexisto': No such file or directory
        $> echo $?
        2
        $> ./and 'echo hola' 'ls /etc/resolv.conf' 'echo adios'
        hola
        /etc/resolv.conf
        adios
        $> echo $?
        0
        $>
        
        El programa en C debe ejecutar los programas con la llamada al sistema execv(2). No está permitido ejecutar una shell o usar funciones como system(3).
        
        
        
        Para tokenizar las cadenas, se debe usar la función strtok_r(3).