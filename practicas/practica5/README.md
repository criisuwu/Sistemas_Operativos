La quinta practica de la asignatura de _Sistemas Operativos_ de la carrera *Ingenieria de Robotica software* de la universidad *Rey Juan Carlos*

-Enunciado:
    Escribe un programa llamado curler.c que lea líneas, que contendrán URLs. El programa deberá comprobar si se pueden descargar dichas URLs. Si se pueden descargar todas, saldrá con éxito. Si no, saldrá con fallo.
    
    El programa puede admitir un argumento, que es la ruta de un fichero con las líneas. Si no se le da ningún argumento, debe leer de la entrada estándar. En caso de tener más de un argumento, deberá fallar.
    
    Las líneas deben tener como máximo una longitud de 512 caracteres, y deben empezar con "http://" o "https://". Si una línea no cumple este requisito, el programa debe mostrar un error y terminar con fallo inmediatamente.
    
    Las URLs se deberán comprobar de forma secuencial, según se van leyendo líneas de la entrada. El estado de salida del proceso será el número de URLs que no se han podido descargar (o 255 si hay más de 255 fallos). Si hay un error que no tenga que ver con la descarga, el estado de salida deberá ser 255 (p. ej. si se le ha pasado más de un argumento al programa o la línea es inválida).
    
    Se debe ejecutar el comando curl para descargar las URLs. Las ejecuciones de curl no deben escribir nada (ni datos ni errores). Se debe de invocar con un timeout de 5 segundos, por ejemplo:
    
    $> curl --connect-timeout 5 https://www.urjc.es
    
    
    Por ejemplo, el programa se debería comportar así:
    
    $> cat urls
    https://google.es
    https://www.google.com
    $> ./curler < urls
    $> echo $?
    0
    $> ./curler urls
    $> echo $?
    0
    $> echo patata | ./curler
    error: invalid line "patata"
    $> echo $?
    255
    $> cat urls2
    https://google.es
    https://patatainfame.com
    https://www.google.com
    $> ./curler urls2
    $> echo $?
    1
    $> 
    
    Solo se podrá ejecutar el programa externo curl, que deberá ser ejecutado con execv(2) o execl(2). Para leer líneas se debe usar fgets(3).