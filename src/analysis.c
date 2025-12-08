#include "analysis.h"
#include "encoding.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

// -------------------------------
// Generación de bits aleatorios
// -------------------------------
char *generate_random_bits(size_t n)
{
    char *bits = malloc(n + 1);
    if (!bits)
        return NULL;
    for (size_t i = 0; i < n; i++)
    {
        bits[i] = (rand() % 2) ? '1' : '0';
    }
    bits[n] = '\0';
    return bits;
}
// -------------------------------------------------
// Contar errores entre dos bitstreams
// -------------------------------------------------
size_t count_bit_errors(const char *original, const char *received)
{
    size_t len = strlen(original);
    size_t errors = 0;
    for (size_t i = 0; i < len && received[i] != '\0'; i++)
        if (original[i] != received[i])
            errors++;
    return errors;
}

// -------------------------------------------------
// Obtener longitud de la señal codificada
// -------------------------------------------------
size_t get_encoded_length(const char *bitstream,
                          char *(*encode_fn)(const char *))
{
    char *enc = encode_fn(bitstream);
    size_t len = strlen(enc);
    free(enc);
    return len;
}

// -------------------------------------------------
// Simulación de ráfagas de errores
// -------------------------------------------------
void simulate_burst_errors(char *bitstream, double prob_inicio, size_t burst_len) {
    size_t len = strlen(bitstream);
    for (size_t i = 0; i < len; i++) {
        if ((double)rand() / RAND_MAX < prob_inicio) {
            for (size_t j = 0; j < burst_len && (i + j) < len; j++) {
                bitstream[i + j] = (bitstream[i + j] == '0') ? '1' : '0';
            }
            i += burst_len; // Saltar la ráfaga
        }
    }
}

void add_noise_encoded(char *encoded, double ber, const char *scheme)
{
    if (!encoded || ber <= 0.0)
        return;
    if (ber > 1.0)
        ber = 1.0;

    static int seed_init = 0;
    if (!seed_init)
    {
        srand((unsigned)time(NULL));
        seed_init = 1;
    }

    size_t len = strlen(encoded);

    if (strcmp(scheme, "NRZ") == 0 || strcmp(scheme, "NRZI") == 0)
    {
        // Invertir 'H' <-> 'L'
        for (size_t i = 0; i < len; i++)
        {
            double r = (double)rand() / RAND_MAX;
            if (r < ber)
            {
                encoded[i] = (encoded[i] == 'H') ? 'L' : 'H';
            }
        }
    }
    else if (strcmp(scheme, "Manchester") == 0 || strcmp(scheme, "4B5B") == 0)
    {
        // Invertir '0' <-> '1', siempre sobre la señal codificada
        for (size_t i = 0; i < len; i++)
        {
            double r = (double)rand() / RAND_MAX;
            if (r < ber)
            {
                if (encoded[i] == '0')
                    encoded[i] = '1';
                else if (encoded[i] == '1')
                    encoded[i] = '0';
            }
        }
    }
    else
    {
        // Por defecto, invertir '0'/'1'
        for (size_t i = 0; i < len; i++)
        {
            double r = (double)rand() / RAND_MAX;
            if (r < ber)
            {
                if (encoded[i] == '0')
                    encoded[i] = '1';
                else if (encoded[i] == '1')
                    encoded[i] = '0';
            }
        }
    }
}



void add_noise_4b5b(char *encoded, double ber)
{
    if (!encoded || ber <= 0.0) return;
    size_t len = strlen(encoded);

    for (size_t i = 0; i < len; i += 5) // Procesar bloques de 5 bits
    {
        double r = (double)rand() / RAND_MAX;
        if (r < ber)
        {
            // Invertir un bit aleatorio dentro del bloque
            size_t bit_pos = i + rand() % 5;
            encoded[bit_pos] = (encoded[bit_pos] == '0') ? '1' : '0';
        }
    }
}


// -------------------------------------------------
// Ejecutar N simulaciones con ruido
// -------------------------------------------------
void run_simulations(FILE *md, const char *bitstream, double ber, int N,
                     const char *name,
                     char *(*encode_fn)(const char *),
                     char *(*decode_fn)(const char *))
{
    size_t len = strlen(bitstream);
    int total_errors = 0, min_err = (int)len, max_err = 0;

    for (int i = 0; i < N; i++) {
        char *enc = encode_fn(bitstream);
        if (!enc) continue;

        char *enc_noisy = malloc(strlen(enc) + 1);
        strcpy(enc_noisy, enc);

        // Aplicamos ruido a la señal codificada
        add_noise_encoded(enc_noisy, ber, name);

        char *dec = decode_fn(enc_noisy);
        
        size_t errors;
        if (!dec) {
            // Si el ruido rompió la estructura (4B5B), asumimos error total en ese bloque
            errors = len; 
        } else {
            errors = count_bit_errors(bitstream, dec);
            free(dec);
        }

        total_errors += (int)errors;
        if ((int)errors < min_err) min_err = (int)errors;
        if ((int)errors > max_err) max_err = (int)errors;

        free(enc);
        free(enc_noisy);
    }

    double avg = (double)total_errors / N;
    fprintf(md, "| %s | %.2f | %d | %d |\n", name, avg, min_err, max_err);
}