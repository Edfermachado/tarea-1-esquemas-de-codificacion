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
    if (!is_valid_bitstream(bitstream)) {
        fprintf(stderr, "Error: bitstream contiene caracteres inválidos\n");
        return NULL;
    }

    size_t length = strlen(bitstream);

    char *encoded = safe_malloc(length + 1);
    char current_level = 'H'; // Nivel inicial fijo para tu proyecto

    for (size_t i = 0; i < length; i++) {
        if (bitstream[i] == '1') {
            current_level = (current_level == 'H') ? 'L' : 'H';
        }
        encoded[i] = current_level;
    }

    encoded[length] = '\0';
    return encoded;
}


char *decode_nrzi(const char *encoded) {
    if (!encoded) {
        fprintf(stderr, "Error: encoded es NULL\n");
        return NULL;
    }

    size_t length = strlen(encoded);
    char *decoded = safe_malloc(length + 1);

    char prev = 'H'; // Nivel inicial ACORDADO

    for (size_t i = 0; i < length; i++) {
        char curr = toupper(encoded[i]);

        if (curr != 'H' && curr != 'L') {
            fprintf(stderr, "Error: encoded contiene caracteres inválidos\n");
            free(decoded);
            return NULL;
        }

        decoded[i] = (curr != prev) ? '1' : '0';
        prev = curr;
    }

    decoded[length] = '\0';
    return decoded;
}


// ============================================
// Manchester
// ============================================

char *encode_manchester(const char *bitstream) {
    // TODO: Implementar
    // En Manchester: '0' = transición bajo->alto, '1' = transición alto->bajo
    // (o viceversa según convención IEEE/Thomas)

    if (!bitstream) return NULL;

    size_t len = strlen(bitstream);
    char *out = malloc(len * 2 + 1);
    if (!out) return NULL;

    size_t j = 0;

    for (size_t i = 0; i < len; i++) {
        if (bitstream[i] == '0') {
            out[j++] = '0';
            out[j++] = '1';
        } else if (bitstream[i] == '1') {
            out[j++] = '1';
            out[j++] = '0';
        } else {
            // Carácter inválido
            free(out);
            return NULL;
        }
    }

    out[j] = '\0';
    return out;
}

char *decode_manchester(const char *encoded) {
    // TODO: Implementar
     if (!encoded) return NULL;

    size_t len = strlen(encoded);
    if (len % 2 != 0) {
        fprintf(stderr, "Manchester inválido\n");
        return NULL;
    }

    char *out = malloc(len / 2 + 1);
    if (!out) return NULL;

    size_t j = 0;

    for (size_t i = 0; i < len; i += 2) {
        char a = encoded[i];
        char b = encoded[i + 1];

        if (a == '0' && b == '1') {
            out[j++] = '0';
        } else if (a == '1' && b == '0') {
            out[j++] = '1';
        } else {
            // Secuencia inválida
            free(out);
            return NULL;
        }
    }

    out[j] = '\0';
    return out;
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

    switch (c) {
        case 'H': return 1;   // Alto
        case 'L': return 0;   // Bajo

        // Manchester y codificaciones binarias directas
        case '1': return 1;
        case '0': return 0;

        // Bipolar / AMI
        case '+': return 1;
        case '-': return -1;

        default:
            return -1; // Desconocido
    }
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

    // Modo append, así no sobrescribes el archivo
    FILE *f = fopen(filename, "a");
    if (!f) {
        fprintf(stderr, "Error: no se pudo abrir archivo '%s'\n", filename);
        return;
    }

    fprintf(f, "\n--------------------------------\n");
    fprintf(f, "Diagrama de señal codificada\n");
    fprintf(f, "Entrada: %s\n\n", encoded);

    //
    // Detectar si es Manchester (dos bits por símbolo y siempre 01 o 10)
    //
    int is_manchester = 1;
    if (length % 2 != 0) {
        is_manchester = 0;  // Manchester siempre tiene longitud par
    } else {
        for (size_t i = 0; i < length; i += 2) {
            char a = encoded[i];
            char b = encoded[i+1];
            if (!((a=='0' && b=='1') || (a=='1' && b=='0'))) {
                is_manchester = 0;
                break;
            }
        }
    }

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

        // Transición normal: comparar con anterior
        if (i > 0 && lvl != prev)
            fprintf(f, "|");
        else
            fprintf(f, " ");

        // Dibujar nivel
        if (lvl == 1)
            fprintf(f, "====");
        else if (lvl == 0)
            fprintf(f, "____");
        else
            fprintf(f, "????");

        // Si es Manchester, marcamos la transición en medio del bit
        if (is_manchester && (i % 2 == 0)) {
            // Entre encoded[i] y encoded[i+1]
            fprintf(f, "|");
        }

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