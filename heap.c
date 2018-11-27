#include <stdlib.h>
#include <stdio.h>
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

size_t pos_hijo_izq(size_t pos) {
    return pos*2 + 1;
}
size_t pos_hijo_der(size_t pos) {
    return pos_hijo_izq(pos) + 1;
}
size_t pos_padre(size_t pos) {
    if (pos == 0) return 0;
    return (pos-1)/2;
}
size_t pos_ultimo_padre(size_t tam) {
    if(tam<=1) return 0;
    return tam/2-1;
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

void up_heap(void** datos, size_t pos, cmp_func_t cmp) {

    if(pos == 0) return;

    size_t posicion_padre = pos_padre(pos);

    if(cmp(datos[pos], datos[posicion_padre]) > 0) {

        swap(datos, pos, posicion_padre);

    }

    up_heap(datos, posicion_padre, cmp);

}

void down_heap(void* arreglo[], size_t tam, size_t pos, cmp_func_t cmp) {

    while(pos<tam-1) {
        size_t posicion_hijo_izq = pos_hijo_izq(pos);
        size_t posicion_hijo_der = pos_hijo_der(pos);
        size_t actual = pos;

        if(posicion_hijo_izq < tam && cmp(arreglo[actual], arreglo[posicion_hijo_izq]) < 0)
            actual = posicion_hijo_izq;
        if(posicion_hijo_der < tam && cmp(arreglo[actual], arreglo[posicion_hijo_der]) < 0)
            actual = posicion_hijo_der;
        if(actual == pos)
            return;
        swap(arreglo, actual, pos);
        pos = actual;
    }
}

void heapify(void* arreglo[], size_t tam, cmp_func_t cmp) {
    size_t pos = pos_ultimo_padre(tam);

    for (int i = (int)pos; i >= 0 ; i--){
        down_heap(arreglo, tam, i, cmp);
    }

}

/* Función de heapsort genérica. Esta función ordena mediante heap_sort
 * un arreglo de punteros opacos, para lo cual requiere que se
 * le pase una función de comparación. Modifica el arreglo "in-place".
 * Nótese que esta función NO es formalmente parte del TAD Heap.
 */
void heap_sort(void* elementos[], size_t cant, cmp_func_t cmp) {
    heapify(elementos, cant, cmp);
    size_t pos_ultimo_elemento = cant-1;
    while(pos_ultimo_elemento > 0) {
        swap(elementos, 0, pos_ultimo_elemento);
        down_heap(elementos, pos_ultimo_elemento, 0, cmp);
        pos_ultimo_elemento--;
    }
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
    heap_t* heap = malloc(sizeof(heap_t));
    if(heap == NULL) return NULL;
    if (n > TAMANIO_INICIAL) heap->tamanio = n;
    else heap->tamanio = TAMANIO_INICIAL;
    heap->cantidad = n;
    heap->cmp = cmp;
    void** datos_nuevo = malloc(sizeof(void*)* heap->tamanio);
    for (int i = 0; i < n; i++) {
        datos_nuevo[i] = arreglo[i];
    }
    heap->datos = datos_nuevo;
    heapify(heap->datos, n, cmp);
    return heap;
}

/* Elimina el heap, llamando a la función dada para cada elemento del mismo.
 * El puntero a la función puede ser NULL, en cuyo caso no se llamará.
 * Post: se llamó a la función indicada con cada elemento del heap. El heap
 * dejó de ser válido. */
void heap_destruir(heap_t* heap, void destruir_elemento(void* e)) {
    if(destruir_elemento != NULL) {
        for(int i = 0; i < heap->cantidad ; i++) {
            destruir_elemento(heap->datos[i]);
        }
    }
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
    heap->cantidad++;
    up_heap(heap->datos, heap->cantidad-1, heap->cmp);
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
    down_heap(heap->datos, heap->cantidad, 0, heap->cmp);
    return aux;
}