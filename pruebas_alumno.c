#include "heap.h"
#include "testing.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int mayor(const void* a, const void* b) {
    int c = *(int*) a;
    int d = *(int*) b;
    if(c == d)  return 0;
    if(c > d)   return 1;   //la idea es que al cambiar el sentido de esta operacion, puedo pasar un heap de minimos a mayores.
    return -1;
}

int menor(const void* a, const void* b) {
    int c = *(int*) a;
    int d = *(int*) b;
    if(c == d)  return 0;
    if(c < d)   return 1;   //la idea es que al cambiar el sentido de esta operacion, puedo pasar un heap de minimos a mayores.
    return -1;
}

void prueba_crear() {
    heap_t* heap = heap_crear(mayor);
    print_test("esta vacio", heap_esta_vacio(heap));
    print_test("cantidad == 0", heap_cantidad(heap) == 0);
    print_test("ver maximo == NULL", heap_ver_max(heap) == NULL);
    print_test("desencolar == NULL", heap_desencolar(heap) == NULL);
    heap_destruir(heap, NULL);
}

void prueba_heap_mayores_encolar() {
    heap_t* heap = heap_crear(mayor);
    int numero[] = {1,2,3,4,5,6,7,8,9};
    int tam = 9; 
    for(int i=0; i<tam; i++) {
        heap_encolar(heap, &numero[i]);
        printf("nuevo maximo es %i", numero[i]);
        print_test("" , heap_ver_max(heap) == &numero[i]);
    }
    printf("cantidad = %i", tam);
    print_test("", heap_cantidad(heap) == tam);
    heap_destruir(heap, NULL);
}

void prueba_heap_menores_encolar() {
    heap_t* heap = heap_crear(menor);
    int numero[] = {1,2,3,4,5,6,7,8,9};
    int tam = 9; 
    for(int i=tam-1; i>=0; i--) {
        heap_encolar(heap, &numero[i]);
        printf("nuevo minimo es %i", numero[i]);
        print_test("" , heap_ver_max(heap) == &numero[i]);
    }
    printf("cantidad = %i", tam);
    print_test("", heap_cantidad(heap) == tam);
    heap_destruir(heap, NULL);
}

void prueba_heap_mayores_desencolar() {
    heap_t* heap = heap_crear(mayor);
    int numero[] = {1,2,3,4,5,6,7,8,9};
    int tam = 9; 
    for(int i=0; i<tam; i++) {
        heap_encolar(heap, &numero[i]);
    }
    for(int i=tam-1; i>=0; i--) {
        printf("%i desencolado", numero[i]);
        print_test("", heap_desencolar(heap) == &numero[i]);
    }
    print_test("cantidad = 0", heap_cantidad(heap) == 0);
    heap_destruir(heap, NULL);
}

void prueba_heap_menores_desencolar() {
    heap_t* heap = heap_crear(menor);
    int numero[] = {1,2,3,4,5,6,7,8,9};
    int tam = 9;
    for(int i=0; i< tam; i++) {
        heap_encolar(heap, &numero[i]);
    }
    for(int i=0; i<tam; i++) {
        printf("%i desencolado", numero[i]);
        print_test("", heap_desencolar(heap) == &numero[i]);
    }
    print_test("cantidad = 0", heap_cantidad(heap) == 0);
    heap_destruir(heap, NULL);
}

void prueba_heap_elementos_repetidos() {
    heap_t* heap = heap_crear(mayor);
    int numero[] = {1,1,2,2,3,3,4,4,5};
    int tam = 9;
    for(int i=0; i<tam; i++) {
        heap_encolar(heap, &numero[i]);
        printf("nuevo maximo es %i", numero[i]);
        print_test("" , *(int*)heap_ver_max(heap) == numero[i]);
    }
    for(int i=tam-1; i>=0; i--) {
        printf("%i desencolado", numero[i]);
        print_test("", *(int*)heap_desencolar(heap) == numero[i]);
    }
    heap_destruir(heap, NULL);
}

void** crear_valores(int tamanio, int* numero) {
    void** valores = malloc(sizeof(void*) * tamanio);
    if(valores == NULL) return NULL;
    for(int i=0; i<tamanio; i++) {
        int* valor = malloc(sizeof(int));
        if(valor == NULL) {
            for(int n=0; n<i; n++) {
                free(valores[n]);
            }
            free(valores);
            return NULL;
        }
        *valor = numero[i];
        valores[i] = valor;
    }
    return valores;
}

void destruir_valores(void** valores, size_t tam) {
    for(int i=0; i<tam; i++) {
        free(valores[i]);
    }
    free(valores);
}

void prueba_heap_sort() {
    heap_t* heap = heap_crear(mayor);
    int numero[] = {3,4,1,2,8,6,5,9,7};
    int ordenado[] = {1,2,3,4,5,6,7,8,9};
    int tam = 9;
    void** parametro = crear_valores(tam, numero);
    heap_sort(parametro, tam, mayor);
    bool esta_ordenado = true;
    for(int i=0; i<tam; i++) {
        if(ordenado[i] != *(int*)parametro[i]) {
            esta_ordenado = false;
            break;
        }
    }
    print_test("esta ordenado", esta_ordenado);
    destruir_valores(parametro, tam);
    heap_destruir(heap, NULL);
}

void pruebas_heap_alumno() {
    //prueba_crear();
    //prueba_heap_mayores_encolar();
    //prueba_heap_menores_encolar();
    //prueba_heap_mayores_desencolar();
    //prueba_heap_menores_desencolar();
    //prueba_heap_elementos_repetidos();
    prueba_heap_sort();
}