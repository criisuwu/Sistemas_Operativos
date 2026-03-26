La segunda practica de la asignatura de _Sistemas Operativos_ de la carrera *Ingenieria de Robotica software* de la universidad *Rey Juan Carlos*

-Enunciado:

        Escribe un programa llamado timecmds.c que reciba un número indeterminado de programas a ejecutar con sus argumentos. Para indicar cada programa a ejecutar, se pasará su ruta. El programa debe ejecutar todos los procesos, indicando cuánto tiempo han tardado en ejecutar y si han terminado correctamente o no. Para indicar esto, debe escribir por su salida una línea por programa que se le ha pasado, el PID, el estatus de salida y el tiempo que ha tardado en ejecutar, en segundos.
        
        Por ejemplo:
        
        
        $> timecmds '/bin/sleep 10' '/bin/sleep 10' '/bin/false'
        cmd: /bin/false, pid: 3434, time: 0 seconds, status: failure
        cmd: /bin/sleep 10, pid: 3432, time: 10 seconds, status: success
        cmd: /bin/sleep 10, pid: 3433, time: 10 seconds, status: success
        $>
        
        El programa debe ser lo más rápido que se pueda, por lo que los programas a ejecutar deberían ser concurrentes.
        
        El formato de la salida del programa se tiene que ajustar al del ejemplo.Es obligatorio usar las funciones fork(2), execv(2) y  wait(3) para realizar este ejercicio. Para conseguir el tiempo actual del sistema (en segundos), se debe usar la función time(2). Para tokenizar las strings, se debe usar la función strtok_r(3).
        
        No se puede ejecutar ningún otro programa externo que no sean los indicados en los argumentos.
        
        Si los programas que se ejecutan escriben cosas por su salida, se mezclará con la salida del programa. Por ahora no podemos hacer nada para evitar esto, por lo que no hay que controlar ese detalle (si se mezclan las salidas, no pasa nada).
        
        
        El programa tiene que acabar con éxito si todos los programas terminan con éxito. En otro caso, debe terminar con fallo. El programa no puede acabar hasta que no hayan salido todos los procesos creados.
        
        