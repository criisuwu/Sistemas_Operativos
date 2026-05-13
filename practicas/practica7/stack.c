/*
*   Ejercicio 7. stack
*
*   El objetivo de este programa es implementar una pila generica usando
*   usando un array dinamico de void *. Esta protegida por un mutex de
*   pthreads para permitir el acceso concurrente desde multiples hilos.
*
*   Una de las funcionalidades es que ademas de ser thread-safe debe de
*   ampliarse la pila si el array se llena. Y las pilas deben poder usarse
*   concurrentemente.
*
*   Si al acabar los hilos, el hilo principal comprueba que hay 6000
*   elementos en la pila y extraera todos los valores, comprobando que los
*   ids son ordenados decrecientemente. Si la comprobacion falla, imprime
*   el error, el estado de la pila y terminar con fallo. Si todo
*   es correcto termina con exito.
*
*   Creado por: Cristina Homobono Fernández
*   Fecha: 13 mayo 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "stack.h"

struct Stack {
    void          **data;
    int             top;
    int             cap;
    pthread_mutex_t lock;
};

/* Crea una pila vacía con capacidad inicial sz.
*/
Stack *newstack(int sz)
{
    if (sz <= 0)
        return NULL;

    Stack *s = malloc(sizeof(Stack));
    if (s == NULL)
        return NULL;

    s->data = malloc((size_t)sz * sizeof(void *));
    if (s->data == NULL) {
        free(s);
        return NULL;
    }

    s->top = 0;
    s->cap = sz;

    if (pthread_mutex_init(&s->lock, NULL) != 0) {
        free(s->data);
        free(s);
        return NULL;
    }

    return s;
}

/* Verificación de si la pila esta vacia.
*/
int isempty(Stack *s)
{
    pthread_mutex_lock(&s->lock);
    int empty = (s->top == 0);
    pthread_mutex_unlock(&s->lock);
    return empty;
}

/* Esta funcion inserta un elemento en la cima de la pila. Si la pila
*  está llena, dobla el tamaño del array con realloc.
*/
void push(Stack *s, void *elem)
{
    pthread_mutex_lock(&s->lock);

    if (s->top == s->cap) {
        int newcap = s->cap * 2;
        void **newdata = realloc(s->data, (size_t)newcap * sizeof(void *));
        if (newdata == NULL) {
            pthread_mutex_unlock(&s->lock);
            fprintf(stderr, "push: realloc failed, element not inserted\n");
            return;
        }
        s->data = newdata;
        s->cap  = newcap;
    }

    s->data[s->top++] = elem;

    pthread_mutex_unlock(&s->lock);
}

/* Esta función extrae y devuelve el elemento de la cima. En caso de
*  que este vacia devuelve NULL.
*/
void *pop(Stack *s)
{
    pthread_mutex_lock(&s->lock);

    if (s->top == 0) {
        pthread_mutex_unlock(&s->lock);
        return NULL;
    }

    void *elem = s->data[--s->top];

    pthread_mutex_unlock(&s->lock);
    return elem;
}

/* Esta función devuelve el número de elementos almacenados en la pila.
*/
int nelems(Stack *s)
{
    pthread_mutex_lock(&s->lock);
    int n = s->top;
    pthread_mutex_unlock(&s->lock);
    return n;
}

/* Esta función imprime por la salida de errores la capacidad, el
*  número de elementos y los punteros almacenados.
*/
void dumpstack(Stack *s)
{
    pthread_mutex_lock(&s->lock);

    fprintf(stderr, "Stack@%p  cap=%d  nelems=%d\n", (void *)s, s->cap, s->top);
    for (int i = s->top - 1; i >= 0; i--)
        fprintf(stderr, "  [%d] %p\n", i, s->data[i]);

    pthread_mutex_unlock(&s->lock);
}

/* Esta función libera la memoria de la pila.
*/
void freestack(Stack *s)
{
    if (s == NULL)
        return;
    pthread_mutex_destroy(&s->lock);
    free(s->data);
    free(s);
}