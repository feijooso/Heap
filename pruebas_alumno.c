#include "heap.h"
#include "testing.h"
#include <string.h>

int comparar(const void* a, const void* b) {
    int c = *(int*) a;
    int d = *(int*) b;
    if(c == d)  return 0;
    if(c > d)   return 1;
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

void pruebas_heap_alumno() {
    prueba_crear();
}