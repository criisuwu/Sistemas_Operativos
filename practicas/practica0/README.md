La practica cero de la asignatura de _Sistemas Operativos_ de la carrera *Ingenieria de Robotica software* de la universidad *Rey Juan Carlos*

-Enunciado:

        Escribe un programa sortargs.c en C para Linux que reciba como argumentos un número indeterminado de cadenas de texto.
        
        El programa debe pasar todas las cadenas a mayúsculas (ignorando los caracteres que no sean letras) e imprimir las cadenas en orden lexicográfico creciente, una por línea, eliminando cadenas duplicadas.
        
        Si no se proporciona ningún argumento, el programa debe fallar.
        
        Para ordenar, se debe implementar el algoritmo insertion sort o el algoritmo selection sort.
        
        No se debe usar una lista enlazada para hacer el ejercicio.
        
        Por ejemplo:
        
        $ ./sortargs hola que tal estas tal amigo tal hola
        AMIGO
        ESTAS
        HOLA
        QUE
        TAL
        $ ./sortargs
        error: no arguments
        $
        
        
        El programa no puede ejecutar ningún otro programa externo, y debe implementar el algoritmo de ordenación (no se puede usar una biblioteca para ordenar).