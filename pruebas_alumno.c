#include "heap.h"
#include "testing.h"
#include <string.h>
#include <stdio.h>

int comparar(const void* a, const void* b) {
    int c = *(int*) a;
    int d = *(int*) b;
    if(c == d)  return 0;
    if(c > d)   return 1;   //la idea es que al cambiar el sentido de esta operacion, puedo pasar un heap de minimos a mayores.
    return -1;
}

void prueba_crear() {
    heap_t* heap = heap_crear(comparar);
    print_test("esta vacio", heap_esta_vacio(heap));
    print_test("cantidad == 0", heap_cantidad(heap) == 0);
    print_test("ver maximo == NULL", heap_ver_max(heap) == NULL);
    print_test("desencolar == NULL", heap_desencolar(heap) == NULL);
    heap_destruir(heap, NULL);
}

void prueba_heap_encolar() {
    heap_t* heap = heap_crear(comparar);
    int numero[] = {1,2,3,4,5,6,7,8,9};
    int tam = 9; 
    for(int i=0; i<tam; i++) {
        heap_encolar(heap, &numero[i]);
        printf("nuevo maximo es %i", numero[i]);
        print_test("" , heap_ver_max(heap) == &numero[i]);
    }
    heap_destruir(heap, NULL);
}

void pruebas_heap_alumno() {
    //prueba_crear();
    prueba_heap_encolar();
}