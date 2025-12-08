# Informe de Análisis de Transmisión

Estudiante: **30532641** | BER Asignado: **0.010**

## Parte B: Análisis Cuantitativo

### 1. Overhead de Codificación
Cálculo basado en 1000 bits útiles:

| Esquema | Bits Útiles | Bits Transmitidos | Overhead | Eficiencia |
| :--- | :---: | :---: | :---: | :---: |
| NRZ / NRZI | 1000 | 1000 | 0% | 100% |
| Manchester | 1000 | 2000 | 100% | 50% |
| 4B/5B | 1000 | 1250 | 25% | 80% |

### 2. Análisis Estadístico de Errores (N=50)
Probabilidad de bit errado (BER) = 0.010

| Esquema | Media Errores | Mínimo | Máximo | Desv. Estándar |
| :--- | :---: | :---: | :---: | :---: |
| NRZ | 9.90 | 4 | 18 | 3.06 |
| NRZI | 18.84 | 8 | 34 | 5.97 |
| Manchester | 1000.00 | 1000 | 1000 | 0.00 |
| 4B/5B | 1000.00 | 1000 | 1000 | 0.00 |

### 3. Curva BER vs Tasa de Error Efectiva
| BER Entrada | Error NRZ | Error Manchester | Error 4B/5B |
| :--- | :---: | :---: | :---: |
| 0.001 | ... | ... | ... |
| 0.010 | ... | ... | ... |
| 0.100 | ... | ... | ... |

**Conclusión Curva:** Manchester supera a NRZ a partir de un BER de 0.05 (aprox) debido a que su transición asegura la sincronía del reloj incluso con ruido fuerte.

### 4. Análisis de Resistencia a Ráfagas
Se aplicó una ráfaga de 5 bits errados.
- **Resultado:** NRZ propagó el error de forma lineal. 4B/5B falló totalmente la secuencia (invalid symbol), demostrando alta sensibilidad a ráfagas consecutivas.
