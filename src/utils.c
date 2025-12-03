#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Asignación de memoria segura
 * @param size Tamaño a asignar
 * @return Puntero a memoria asignada, o NULL si falla
 */
void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL && size > 0) {
        fprintf(stderr, "Error: No se pudo asignar %zu bytes\n", size);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

/**
 * @brief Duplica una cadena de forma segura
 * @param src Cadena original
 * @return Nueva cadena en memoria dinámica
 */
char *string_duplicate(const char *src) {
    if (src == NULL) return NULL;
    
    size_t len = strlen(src);
    char *dest = (char *)safe_malloc(len + 1);
    strcpy(dest, src);
    return dest;
}

/**
 * @brief Valida que una cadena contenga solo '0' y '1'
 * @param str Cadena a validar
 * @return 1 si es válida, 0 si no
 */
int is_valid_bitstream(const char *str) {
    if (str == NULL) return 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != '0' && str[i] != '1') {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Imprime un byte en formato binario
 * @param byte Byte a imprimir
 */
void print_binary(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (byte >> i) & 1);
    }
}


