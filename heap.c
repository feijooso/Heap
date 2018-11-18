#include <stdlib.h>
#include "heap.h"

#define TAMANIO_INICIAL 10

/*
 * Implementación de un TAD cola de prioridad, usando un max-heap.
 *
 * Notar que al ser un max-heap el elemento mas grande será el de mejor
 * prioridad. Si se desea un min-heap, alcanza con invertir la función de
 * comparación.
 */

/* Tipo utilizado para el heap. */
struct heap {
    void** datos;
    size_t cantidad;
    size_t tamanio;
    cmp_func_t cmp;
};

size_t pos_hijo_der(size_t pos) {
    return pos*2 + 2;
}
size_t pos_hijo_izq(size_t pos) {
    return pos*2 + 1;
}
size_t pos_padre(size_t pos) {
    return (pos-1)/2;
}

void swap(void** datos, size_t pos1, size_t pos2) {
    void* aux = datos[pos1];
    datos[pos1] = datos[pos2];
    datos[pos2] = aux;
}

bool redimensionar(heap_t* heap, size_t nuevo_tamanio) {
    void** pointer = realloc(heap->datos, nuevo_tamanio * sizeof(void*));
    if(pointer == NULL) return false;
    heap->datos = pointer;
    heap->tamanio = nuevo_tamanio;
    return true;
}

bool verificar_redimension(heap_t* heap) {
    if(heap->cantidad == heap->tamanio) {
        return redimensionar(heap, heap->tamanio*2);
    }
    size_t nuevo_tamanio = heap->tamanio / 2;
    if(heap->cantidad <= heap->tamanio/4 && nuevo_tamanio >= TAMANIO_INICIAL) {
        return redimensionar(heap, nuevo_tamanio);
    }
    return true;
}

size_t pos_hijo_prioritario(heap_t* heap, size_t pos_hijo_izq, size_t pos_hijo_der) {
    void* hijo_izq = heap->datos[pos_hijo_izq];
    void* hijo_der = heap->datos[pos_hijo_der];
    if(hijo_izq == NULL)
        return pos_hijo_der;
    if(hijo_der == NULL)
        return pos_hijo_izq;
    if(heap->cmp(hijo_izq, hijo_der) > 0)
        return pos_hijo_izq;
    return pos_hijo_der;
}

void down_heap(heap_t* heap, size_t pos, size_t fin) {
    if(pos > fin) return;
    size_t hijo_prioritario = pos_hijo_prioritario(heap, pos_hijo_izq(pos), pos_hijo_der(pos));
    if(heap->cmp(heap->datos[pos], heap->datos[hijo_prioritario]) < 0) {
        swap(heap->datos, pos, hijo_prioritario);
    }
    down_heap(heap, pos+1, fin);
}

void up_heap(heap_t* heap, size_t pos) {
    if(pos == 0) return;
    size_t posicion_padre = pos_padre(pos);
    if(heap->cmp(heap->datos[pos], heap->datos[posicion_padre]) > 0) {
        swap(heap->datos, pos, posicion_padre);
    }
    up_heap(heap, pos-1);
}

/* Función de heapsort genérica. Esta función ordena mediante heap_sort
 * un arreglo de punteros opacos, para lo cual requiere que se
 * le pase una función de comparación. Modifica el arreglo "in-place".
 * Nótese que esta función NO es formalmente parte del TAD Heap.
 */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {

}

/* Crea un heap. Recibe como único parámetro la función de comparación a
 * utilizar. Devuelve un puntero al heap, el cual debe ser destruido con
 * heap_destruir().
 */
heap_t* heap_crear(cmp_func_t cmp) {
    heap_t* heap = malloc(sizeof(heap_t));
    if(heap == NULL) return NULL;
    heap->datos = malloc(sizeof(void*) * TAMANIO_INICIAL);
    if(heap->datos == NULL) return NULL;
    heap->tamanio = TAMANIO_INICIAL;
    heap->cantidad = 0;
    heap->cmp = cmp;
    return heap;
}

/*
 * Constructor alternativo del heap. Además de la función de comparación,
 * recibe un arreglo de valores con que inicializar el heap. Complejidad
 * O(n).
 *
 * Excepto por la complejidad, es equivalente a crear un heap vacío y encolar
 * los valores de uno en uno
*/
heap_t* heap_crear_arr(void* arreglo[], size_t n, cmp_func_t cmp) {
    return NULL;
}

/* Elimina el heap, llamando a la función dada para cada elemento del mismo.
 * El puntero a la función puede ser NULL, en cuyo caso no se llamará.
 * Post: se llamó a la función indicada con cada elemento del heap. El heap
 * dejó de ser válido. */
void heap_destruir(heap_t* heap, void destruir_elemento(void* e)) {
    free(heap->datos);
    free(heap);
}

/* Devuelve la cantidad de elementos que hay en el heap. */
size_t heap_cantidad(const heap_t* heap) {
    return heap->cantidad;
}

/* Devuelve true si la cantidad de elementos que hay en el heap es 0, false en
 * caso contrario. */
bool heap_esta_vacio(const heap_t* heap) {
    return heap->cantidad == 0;
}

/* Agrega un elemento al heap. El elemento no puede ser NULL.
 * Devuelve true si fue una operación exitosa, o false en caso de error.
 * Pre: el heap fue creado.
 * Post: se agregó un nuevo elemento al heap.
 */
bool heap_encolar(heap_t* heap, void* elem) {
    if(elem == NULL || !verificar_redimension(heap)) return false;
    heap->datos[heap->cantidad] = elem;
    if(heap->cantidad > 0)
        up_heap(heap, heap->cantidad);
    heap->cantidad++;
    return true;
}

/* Devuelve el elemento con máxima prioridad. Si el heap esta vacío, devuelve
 * NULL.
 * Pre: el heap fue creado.
 */
void* heap_ver_max(const heap_t* heap) {
    if(heap_esta_vacio(heap)) return NULL;
    return heap->datos[0];
}

/* Elimina el elemento con máxima prioridad, y lo devuelve.
 * Si el heap esta vacío, devuelve NULL.
 * Pre: el heap fue creado.
 * Post: el elemento desencolado ya no se encuentra en el heap.
 */
void* heap_desencolar(heap_t* heap) {
    if(heap_esta_vacio(heap) || !verificar_redimension(heap)) return NULL;
    heap->cantidad--;
    void* aux = heap->datos[0];
    swap(heap->datos, 0, heap->cantidad);
    heap->datos[heap->cantidad] = NULL;
    if(heap->cantidad > 1)
        down_heap(heap, 0, heap->cantidad/2-1);
    return aux;
}