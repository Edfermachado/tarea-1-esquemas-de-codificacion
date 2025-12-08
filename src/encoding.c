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

char *encode_nrz(const char *bitstream)
{
    // TODO: Implementar
    // En NRZ: '1' = nivel alto, '0' = nivel bajo

    if (bitstream == NULL)
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

char *decode_nrz(const char *encoded)
{

    if (encoded == NULL)
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
        else if (upper_c == 'L')
        {
            decoded[i] = '0';
        }
        else
        {
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

char *encode_nrzi(const char *bitstream)
{
    if (!is_valid_bitstream(bitstream))
    {
        fprintf(stderr, "Error: bitstream contiene caracteres inválidos\n");
        return NULL;
    }

    size_t length = strlen(bitstream);

    char *encoded = safe_malloc(length + 1);
    char current_level = 'H'; // Nivel inicial fijo para tu proyecto

    for (size_t i = 0; i < length; i++)
    {
        if (bitstream[i] == '1')
        {
            current_level = (current_level == 'H') ? 'L' : 'H';
        }
        encoded[i] = current_level;
    }

    encoded[length] = '\0';
    return encoded;
}

char *decode_nrzi(const char *encoded)
{
    if (!encoded)
    {
        fprintf(stderr, "Error: encoded es NULL\n");
        return NULL;
    }

    size_t length = strlen(encoded);
    char *decoded = safe_malloc(length + 1);

    char prev = 'H'; // Nivel inicial ACORDADO

    for (size_t i = 0; i < length; i++)
    {
        char curr = toupper(encoded[i]);

        if (curr != 'H' && curr != 'L')
        {
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

char *encode_manchester(const char *bitstream)
{
    // TODO: Implementar
    // En Manchester: '0' = transición bajo->alto, '1' = transición alto->bajo
    // (o viceversa según convención IEEE/Thomas)

    if (!bitstream)
        return NULL;

    size_t len = strlen(bitstream);
    char *out = malloc(len * 2 + 1);
    if (!out)
        return NULL;

    size_t j = 0;

    for (size_t i = 0; i < len; i++)
    {
        if (bitstream[i] == '0')
        {
            out[j++] = '0';
            out[j++] = '1';
        }
        else if (bitstream[i] == '1')
        {
            out[j++] = '1';
            out[j++] = '0';
        }
        else
        {
            // Carácter inválido
            free(out);
            return NULL;
        }
    }

    out[j] = '\0';
    return out;
}

char *decode_manchester(const char *encoded)
{
    // TODO: Implementar
    if (!encoded)
        return NULL;

    size_t len = strlen(encoded);
    if (len % 2 != 0)
    {
        fprintf(stderr, "Manchester inválido\n");
        return NULL;
    }

    char *out = malloc(len / 2 + 1);
    if (!out)
        return NULL;

    size_t j = 0;

    for (size_t i = 0; i < len; i += 2)
    {
        char a = encoded[i];
        char b = encoded[i + 1];

        if (a == '0' && b == '1')
        {
            out[j++] = '0';
        }
        else if (a == '1' && b == '0')
        {
            out[j++] = '1';
        }
        else
        {
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

typedef struct
{
    const char *b4;
    const char *b5;
} Code4B5B;

static const Code4B5B TABLE_4B5B[] = {
    {"0000", "11110"},
    {"0001", "01001"},
    {"0010", "10100"},
    {"0011", "10101"},
    {"0100", "01010"},
    {"0101", "01011"},
    {"0110", "01110"},
    {"0111", "01111"},
    {"1000", "10010"},
    {"1001", "10011"},
    {"1010", "10110"},
    {"1011", "10111"},
    {"1100", "11010"},
    {"1101", "11011"},
    {"1110", "11100"},
    {"1111", "11101"}};

char *encode_4b5b(const char *bitstream)
{
    // TODO: Implementar
    // Cada grupo de 4 bits se convierte en 5 bits según tabla estándar
    if (!is_valid_bitstream(bitstream))
    {
        fprintf(stderr, "Error: bitstream inválido en 4B5B\n");
        return NULL;
    }

    size_t len = strlen(bitstream);
    if (len % 4 != 0)
    {
        fprintf(stderr, "Error: longitud %zu no es múltiplo de 4\n", len);
        return NULL;
    }

    size_t groups = len / 4;
    char *encoded = safe_malloc(groups * 5 + 1);

    char chunk[5];
    chunk[4] = '\0';

    size_t pos = 0;

    for (size_t i = 0; i < groups; i++)
    {

        memcpy(chunk, &bitstream[i * 4], 4);

        int found = 0;

        for (int j = 0; j < 16; j++)
        {
            if (strcmp(chunk, TABLE_4B5B[j].b4) == 0)
            {
                memcpy(&encoded[pos], TABLE_4B5B[j].b5, 5);
                pos += 5;
                found = 1;
                break;
            }
        }

        if (!found)
        {
            fprintf(stderr, "Error: No se encontró codificación para %s\n", chunk);
            free(encoded);
            return NULL;
        }
    }

    encoded[pos] = '\0';
    return encoded;
}

char *decode_4b5b(const char *encoded)
{
    // TODO: Implementar
    if (!is_valid_bitstream(encoded))
    {
        fprintf(stderr, "Error: encoded inválido en 4B5B\n");
        return NULL;
    }

    size_t len = strlen(encoded);
    if (len % 5 != 0)
    {
        fprintf(stderr, "Error: longitud %zu no es múltiplo de 5\n", len);
        return NULL;
    }

    size_t groups = len / 5;
    char *decoded = safe_malloc(groups * 4 + 1);

    char chunk[6];
    chunk[5] = '\0';

    size_t pos = 0;

    for (size_t i = 0; i < groups; i++)
    {

        memcpy(chunk, &encoded[i * 5], 5);

        int found = 0;

        for (int j = 0; j < 16; j++)
        {
            if (strcmp(chunk, TABLE_4B5B[j].b5) == 0)
            {
                memcpy(&decoded[pos], TABLE_4B5B[j].b4, 4);
                pos += 4;
                found = 1;
                break;
            }
        }

        if (!found)
        {
            // fprintf(stderr, "Error: Secuencia 5B desconocida: %s\n", chunk);
            free(decoded);
            return NULL;
        }
    }

    decoded[pos] = '\0';
    return decoded;
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
static int level_from_char(char c)
{
    c = toupper(c);

    switch (c)
    {
    case 'H':
        return 1; // Alto
    case 'L':
        return 0; // Bajo
    case '1':
        return 1; // Manchester, 4B5B, binario
    case '0':
        return 0;
    case '+':
        return 1; // AMI, bipolar
    case '-':
        return -1;
    default:
        return -1; // desconocido
    }
}

/**
 * @brief Genera un diagrama de la señal codificada y lo guarda en un archivo de texto
 * @param encoded Cadena codificada (niveles 'H'/'L')
 * @param filename Nombre del archivo de salida
 */

void plot_signal(const char *encoded, const char *filename)
{
    if (!encoded || !filename)
        return;

    size_t len = strlen(encoded);
    if (len == 0)
        return;
    const char *cedula = "30532641";
    double ber = 0.01;
    FILE *f = fopen(filename, "a"); // Append
    if (!f)
        return;

    fprintf(f, "\n========================================\n");
    fprintf(f, "Cédula: %s | BER: %.2f\n", cedula, ber);
    fprintf(f, "Diagrama de señal codificada\n");
    fprintf(f, "Entrada: %s\n\n", encoded);

    // Detectar Manchester
    int is_manchester = 0;
    if (len % 2 == 0)
    {
        is_manchester = 1;
        for (size_t i = 0; i < len; i += 2)
        {
            char a = encoded[i];
            char b = encoded[i + 1];
            if (!((a == '0' && b == '1') || (a == '1' && b == '0')))
            {
                is_manchester = 0;
                break;
            }
        }
    }

    // Línea de tiempos
    fprintf(f, "Tiempo: ");
    for (size_t i = 0; i < len; i++)
        fprintf(f, "%3zu ", i);
    fprintf(f, "\n");

    // Línea de señal
    fprintf(f, "Señal:  ");
    int prev = level_from_char(encoded[0]);

    for (size_t i = 0; i < len; i++)
    {
        int lvl = level_from_char(encoded[i]);
        if (i > 0 && lvl != prev)
            fprintf(f, "|");
        else
            fprintf(f, " ");

        // Modificación sugerida dentro de plot_signal
        if (lvl == 1)
            fprintf(f, "----"); // Representa nivel alto
        else if (lvl == 0)
            fprintf(f, "____"); // Representa nivel bajo
        else
            fprintf(f, "????");

        // Para Manchester, marcar transición en medio
        if (is_manchester && (i % 2 == 0))
            fprintf(f, "|");

        prev = lvl;
    }

    fprintf(f, "\n");

    fclose(f);
}

// ============================================
// Simulación de ruido
// ============================================

void add_noise(char *bitstream, double ber)
{
    if (!bitstream)
        return;
    if (ber <= 0.0)
        return;
    if (ber > 1.0)
        ber = 1.0;

    static int seed_init = 0;
    if (!seed_init)
    {
        srand((unsigned)time(NULL));
        seed_init = 1;
    }

    for (size_t i = 0; bitstream[i]; i++)
    {
        double r = (double)rand() / RAND_MAX;
        if (r < ber)
        {
            bitstream[i] = (bitstream[i] == '0') ? '1' : '0';
        }
    }
}