#include "encoding.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ============================================
// NRZ (Non-Return to Zero)
// ============================================

char *encode_nrz(const char *bitstream) {
    // TODO: Implementar
    // En NRZ: '1' = nivel alto, '0' = nivel bajo

    if(bitstream == NULL)
    {
        fprintf(stderr, "Error: bitstream es NULL\n");
        return NULL;
    }

    if (!is_valid_bitstream(bitstream))
    {
        fprintf(stderr, "Error: bitstream contiene caracteres inválidos\n");
        return NULL;
    }

    size_t length = strlen(bitstream);
    char *encoded = safe_malloc(length + 1);

    // Codificamos bit a bit

    for (size_t i = 0; i < length; i++)
    {
        encoded[i] = (bitstream[i] == '1') ? 'H' : 'L'; // 'H' de High, 'L' para Low xd
    }
    
    // Null-terminamos la cadena
    encoded[length] = '\0';
    return encoded;
}

char *decode_nrz(const char *encoded) {
    
    if(encoded == NULL)
    {
        fprintf(stderr, "Error: encoded es NULL\n");
        return NULL;
    }

    size_t length = strlen(encoded);
    char *decoded = safe_malloc(length + 1);

    for (size_t i = 0; i < length; i++)
    {
        char c = encoded[i];
        char upper_c = toupper(c);

        if (upper_c == 'H')
        {
            decoded[i] = '1';
        }
        else if(upper_c == 'L'){
            decoded[i] = '0';
        }else{
            fprintf(stderr, "Error: Carácter inválido '%c' en posición %zu\n", c, i);
            free(decoded);
            return NULL;
        }
    }

    decoded[length] = '\0';
    return decoded;
}

// ============================================
// NRZI (Non-Return to Zero Inverted)
// ============================================

char *encode_nrzi(const char *bitstream) {
    // TODO: Implementar
    // En NRZI: '1' = transición, '0' = sin transición
    return NULL;
}

char *decode_nrzi(const char *encoded) {
    // TODO: Implementar
    return NULL;
}

// ============================================
// Manchester
// ============================================

char *encode_manchester(const char *bitstream) {
    // TODO: Implementar
    // En Manchester: '0' = transición bajo->alto, '1' = transición alto->bajo
    // (o viceversa según convención IEEE/Thomas)
    return NULL;
}

char *decode_manchester(const char *encoded) {
    // TODO: Implementar
    return NULL;
}

// ============================================
// 4B/5B
// ============================================

char *encode_4b5b(const char *bitstream) {
    // TODO: Implementar
    // Cada grupo de 4 bits se convierte en 5 bits según tabla estándar
    return NULL;
}

char *decode_4b5b(const char *encoded) {
    // TODO: Implementar
    return NULL;
}

// ============================================
// Visualización de señales
// ============================================


/**
 * Convierte un carácter de nivel a su equivalencia:
 *  - 'H' → 1 (alto)
 *  - 'L' → 0 (bajo)
 *  - cualquier otro → -1 (nivel desconocido)
 */
static int level_from_char(char c) {
    c = toupper(c);
    if (c == 'H') return 1;
    if (c == 'L') return 0;
    return -1;
}


/**
 * @brief Genera un diagrama de la señal codificada y lo guarda en un archivo de texto
 * @param encoded Cadena codificada (niveles 'H'/'L')
 * @param filename Nombre del archivo de salida
 */

void plot_signal(const char *encoded, const char *filename) {

    if (encoded == NULL || filename == NULL) {
        fprintf(stderr, "Error: plot_signal recibió argumentos NULL\n");
        return;
    }

    size_t length = strlen(encoded);
    if (length == 0) {
        fprintf(stderr, "Error: plot_signal recibió cadena vacía\n");
        return;
    }

    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Error: no se pudo abrir archivo '%s'\n", filename);
        return;
    }

    fprintf(f, "Diagrama de señal codificada\n");
    fprintf(f, "Entrada: %s\n\n", encoded);

    //
    // 1) Línea de tiempos
    //
    fprintf(f, "Tiempo: ");
    for (size_t i = 0; i < length; i++) {
        fprintf(f, "%3zu ", i);
    }
    fprintf(f, "\n");

    //
    // 2) Línea de señal
    //
    fprintf(f, "Señal:  ");

    int prev = level_from_char(encoded[0]);

    for (size_t i = 0; i < length; i++) {
        int lvl = level_from_char(encoded[i]);

        // Añadimos una barra vertical para mostrar la transición
        if (i > 0 && lvl != prev)
            fprintf(f, "|");
        else
            fprintf(f, " ");

        // Dibujamos el nivel
        if (lvl == 1)
            fprintf(f, "====");   // Nivel alto
        else if (lvl == 0)
            fprintf(f, "____");   // Nivel bajo
        else
            fprintf(f, "????");   // Nivel desconocido (para esquemas nuevos)

        prev = lvl;
    }

    fprintf(f, "\n");
    fclose(f);
}

// ============================================
// Simulación de ruido
// ============================================

void add_noise(char *bitstream, double ber) {
    // TODO: Implementar
    // Invierte cada bit con probabilidad 'ber'
    (void)bitstream;
    (void)ber;
}