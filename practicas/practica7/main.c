/*
*   Ejercicio 7. stack
*
*   Es el archivo main del trabajo de stack.c
*
*   Creado por: Cristina Homobono Fernández
*   Fecha: 13 mayo 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "stack.h"

/* Son los valores que almacenamos en la pila*/
typedef struct {
    int v;
    int id;
} Value;

/* Son los argumentos que se pasan a cada hilo al crearlo. */
typedef struct {
    int    id;
    int    push_per_thr;
    int    pop_per_thr;
    Stack *stack;
} ThreadArg;

/* Esta función cuenta e imprime los elementos extraídos que no pertenecen
*  al hilo actual. Ademas devuelve el array de Values para que el hilo
*  principal pueda liberarlo.
*/
static void *thread_func(void *arg)
{
    ThreadArg *a = (ThreadArg *)arg;

    int    id           = a->id;
    int    push_per_thr = a->push_per_thr;
    int    pop_per_thr  = a->pop_per_thr;
    Stack *stack        = a->stack;
    free(a);

    Value *vals = malloc((size_t)push_per_thr * sizeof(Value));
    if (vals == NULL) {
        fprintf(stderr, "thread %d: malloc failed\n", id);
        return NULL;
    }

    for (int i = 0; i < push_per_thr; i++) {
        vals[i].v  = i;
        vals[i].id = id;
        push(stack, &vals[i]);
    }

    Value **popped = malloc((size_t)pop_per_thr * sizeof(Value *));
    if (popped == NULL) {
        fprintf(stderr, "thread %d: malloc for popped failed\n", id);
        return vals;
    }

    int foreign = 0;
    for (int i = 0; i < pop_per_thr; i++) {
        popped[i] = (Value *)pop(stack);
        if (popped[i] != NULL && popped[i]->id != id)
            foreign++;
    }
    free(popped);

    if (foreign > 0)
        printf("Thread %d: popped %d foreign element(s)\n", id, foreign);

    return vals;
}

/* En el main se crea la pila y los hilos, espera a que terminen y verifica
*  que el número de elementos restantes es correcto y ademas que cada hilo
*  los dejó en orden.
*/
int main(void)
{
    int nthreads     = 100;
    int push_per_thr = 100;
    int pop_per_thr  = 40;
    int stack_cap    = 100;

    Stack *stack = newstack(stack_cap);
    if (stack == NULL) {
        fprintf(stderr, "newstack failed\n");
        return 1;
    }

    pthread_t *tids    = malloc((size_t)nthreads * sizeof(pthread_t));
    void     **retvals = malloc((size_t)nthreads * sizeof(void *));
    if (tids == NULL || retvals == NULL) {
        fprintf(stderr, "malloc failed\n");
        freestack(stack);
        return 1;
    }

    for (int i = 0; i < nthreads; i++) {
        ThreadArg *a = malloc(sizeof(ThreadArg));
        if (a == NULL) {
            fprintf(stderr, "malloc for thread arg failed\n");
            freestack(stack);
            return 1;
        }
        a->id           = i;
        a->push_per_thr = push_per_thr;
        a->pop_per_thr  = pop_per_thr;
        a->stack        = stack;

        if (pthread_create(&tids[i], NULL, thread_func, a) != 0) {
            fprintf(stderr, "pthread_create failed for thread %d\n", i);
            free(a);
            freestack(stack);
            return 1;
        }
    }

    for (int i = 0; i < nthreads; i++)
        pthread_join(tids[i], &retvals[i]);

    free(tids);

    int expected = (push_per_thr - pop_per_thr) * nthreads;
    int actual   = nelems(stack);

    if (actual != expected) {
        fprintf(stderr,
                "ERROR: expected %d elements in stack, found %d\n",
                expected, actual);
        dumpstack(stack);
        for (int i = 0; i < nthreads; i++)
            free(retvals[i]);
        free(retvals);
        freestack(stack);
        return 1;
    }

    int *last_v = malloc((size_t)nthreads * sizeof(int));
    if (last_v == NULL) {
        fprintf(stderr, "malloc for last_v failed\n");
        freestack(stack);
        return 1;
    }
    for (int i = 0; i < nthreads; i++)
        last_v[i] = push_per_thr;

    int order_ok = 1;
    Value *v;
    while ((v = (Value *)pop(stack)) != NULL) {
        int id = v->id;
        if (v->v >= last_v[id]) {
            fprintf(stderr,
                    "ERROR: order violation for id=%d: got v=%d after v=%d\n",
                    id, v->v, last_v[id]);
            order_ok = 0;
        }
        last_v[id] = v->v;
    }

    free(last_v);

    if (!order_ok) {
        fprintf(stderr, "Order check failed.\n");
        dumpstack(stack);
        for (int i = 0; i < nthreads; i++)
            free(retvals[i]);
        free(retvals);
        freestack(stack);
        return 1;
    }

    printf("All checks passed: %d elements correctly ordered.\n", expected);

    for (int i = 0; i < nthreads; i++)
        free(retvals[i]);
    free(retvals);
    freestack(stack);
    return 0;
}