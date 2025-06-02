#include "../../include/excel_c.h"
#include "../../include/statistical.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>  // Para QueryPerformanceCounter
#endif

// Redefinir la función para medir el tiempo con mayor precisión
double get_time_ms() {
#ifdef _WIN32
    // En Windows, usar QueryPerformanceCounter para mayor precisión
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    return (double)start.QuadPart * 1000.0 / (double)frequency.QuadPart;
#else
    // En otros sistemas, usar clock() pero con un factor de escala
    return (double)clock() * 1000.0 / CLOCKS_PER_SEC;
#endif
}

// Función para calcular la diferencia de tiempo
double time_diff_ms(double start_time) {
#ifdef _WIN32
    LARGE_INTEGER frequency;
    LARGE_INTEGER end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&end);
    double end_time = (double)end.QuadPart * 1000.0 / (double)frequency.QuadPart;
    return end_time - start_time;
#else
    double end_time = (double)clock() * 1000.0 / CLOCKS_PER_SEC;
    return end_time - start_time;
#endif
}

// Función para obtener una copia ordenada de los datos
double* get_sorted_data(DataSet* data) {
    // Verificar que hay datos
    if (!data || data->count <= 0) {
        return NULL;
    }

    // Crear una copia del array de datos
    double* sorted = (double*)malloc(data->count * sizeof(double));
    if (!sorted) {
        return NULL;  // Error de memoria
    }

    // Copiar datos
    for (int i = 0; i < data->count; i++) {
        sorted[i] = data->data[i];
    }

    // Ordenar utilizando bubble sort (simple pero no eficiente)
    for (int i = 0; i < data->count - 1; i++) {
        for (int j = 0; j < data->count - i - 1; j++) {
            if (sorted[j] > sorted[j + 1]) {
                double temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    return sorted;
}

// Cálculo de la media aritmética
StatResult calc_mean(DataSet* data) {
    StatResult result = {0.0, 0.0, 0, "Media"};
    
    if (!data || data->count <= 0) {
        strcpy(result.description, "Error: No hay datos disponibles");
        return result;
    }

    // Iniciar medición de tiempo
    double start_time = get_time_ms();
    int operations = 0;

    // Calcular suma
    double sum = 0.0;
    for (int i = 0; i < data->count; i++) {
        sum += data->data[i];
        operations++;
    }

    // Calcular media
    result.result = sum / data->count;
    operations++;

    // Finalizar medición de tiempo
    result.processing_time = time_diff_ms(start_time);
    result.operations_count = operations;

    sprintf(result.description, "Media (n=%d)", data->count);
    return result;
}

// Cálculo de la mediana
StatResult calc_median(DataSet* data) {
    StatResult result = {0.0, 0.0, 0, "Mediana"};
    
    if (!data || data->count <= 0) {
        strcpy(result.description, "Error: No hay datos disponibles");
        return result;
    }

    // Iniciar medición de tiempo
    double start_time = get_time_ms();
    int operations = 0;

    // Obtener datos ordenados
    double* sorted = get_sorted_data(data);
    if (!sorted) {
        strcpy(result.description, "Error: No se pudieron ordenar los datos");
        return result;
    }

    // Calcular mediana
    if (data->count % 2 == 0) {
        // Si el número de datos es par, promedio de los dos del medio
        int mid_idx = data->count / 2;
        result.result = (sorted[mid_idx - 1] + sorted[mid_idx]) / 2.0;
        operations += 3; // Operaciones para calcular promedio
        sprintf(result.description, "Mediana par (n=%d)", data->count);
    } else {
        // Si el número de datos es impar, valor del medio
        int mid_idx = data->count / 2;
        result.result = sorted[mid_idx];
        operations++;
        sprintf(result.description, "Mediana impar (n=%d)", data->count);
    }

    // Liberar memoria
    free(sorted);

    // Finalizar medición de tiempo
    result.processing_time = time_diff_ms(start_time);
    result.operations_count = operations;

    return result;
}

// Cálculo de la moda
StatResult calc_mode(DataSet* data) {
    StatResult result = {0.0, 0.0, 0, "Moda"};
    
    if (!data || data->count <= 0) {
        strcpy(result.description, "Error: No hay datos disponibles");
        return result;
    }

    // Iniciar medición de tiempo
    double start_time = get_time_ms();
    int operations = 0;

    // Obtener datos ordenados para facilitar el cálculo
    double* sorted = get_sorted_data(data);
    if (!sorted) {
        strcpy(result.description, "Error: No se pudieron ordenar los datos");
        return result;
    }

    // Variables para encontrar la moda
    double mode_value = sorted[0];
    int max_frequency = 1;
    int current_frequency = 1;
    int modal_count = 1; // Número de modas (para detectar casos multimodales)

    // Recorrer el array ordenado
    for (int i = 1; i < data->count; i++) {
        operations++;
        
        if (sorted[i] == sorted[i-1]) {
            // Incrementar frecuencia del valor actual
            current_frequency++;
        } else {
            // Revisar si encontramos una nueva moda
            if (current_frequency > max_frequency) {
                max_frequency = current_frequency;
                mode_value = sorted[i-1];
                modal_count = 1;
            } else if (current_frequency == max_frequency) {
                // Caso multimodal - aumentar contador
                modal_count++;
            }
            
            // Reiniciar frecuencia para el nuevo valor
            current_frequency = 1;
        }
    }
    
    // Verificar el último grupo
    if (current_frequency > max_frequency) {
        mode_value = sorted[data->count - 1];
        max_frequency = current_frequency;
        modal_count = 1;
    } else if (current_frequency == max_frequency) {
        modal_count++;
    }

    // Si todos los valores aparecen la misma cantidad de veces, no hay moda clara
    if (max_frequency == 1) {
        sprintf(result.description, "No hay moda clara (frecuencia=%d)", max_frequency);
    } else if (modal_count > 1) {
        sprintf(result.description, "Multimodal (frec=%d, modas=%d)", max_frequency, modal_count);
    } else {
        sprintf(result.description, "Moda (n=%d, frecuencia=%d)", data->count, max_frequency);
    }

    result.result = mode_value;

    // Liberar memoria
    free(sorted);

    // Finalizar medición de tiempo
    result.processing_time = time_diff_ms(start_time);
    result.operations_count = operations;

    return result;
}

// Cálculo de la media recortada por porcentaje
StatResult calc_trimmed_mean(DataSet* data, double percentage) {
    StatResult result = {0.0, 0.0, 0, "Media recortada"};
    
    if (!data || data->count <= 0) {
        strcpy(result.description, "Error: No hay datos disponibles");
        return result;
    }
    
    if (percentage < 0 || percentage >= 50.0) {
        sprintf(result.description, "Error: Porcentaje fuera de rango (0-50)");
        return result;
    }

    // Iniciar medición de tiempo
    double start_time = get_time_ms();
    int operations = 0;

    // Obtener datos ordenados
    double* sorted = get_sorted_data(data);
    if (!sorted) {
        strcpy(result.description, "Error: No se pudieron ordenar los datos");
        return result;
    }

    // Calcular cuántos valores recortar de cada extremo
    int trim_count = (int)((percentage / 100.0) * data->count);
    operations++;

    // Calcular suma de los valores tras recorte
    double sum = 0.0;
    int valid_count = 0;
    
    for (int i = trim_count; i < data->count - trim_count; i++) {
        sum += sorted[i];
        valid_count++;
        operations++;
    }

    // Calcular media recortada
    if (valid_count > 0) {
        result.result = sum / valid_count;
        operations++;
    } else {
        result.result = 0.0;
        strcpy(result.description, "Error: No hay datos suficientes tras recorte");
    }

    // Liberar memoria
    free(sorted);

    // Finalizar medición de tiempo
    result.processing_time = time_diff_ms(start_time);
    result.operations_count = operations;

    sprintf(result.description, "Media recortada (%.1f%%, n=%d)", percentage, data->count);
    return result;
}

// Cálculo de la media recortada por cantidad de valores
StatResult calc_trimmed_mean_by_count(DataSet* data, int count) {
    StatResult result = {0.0, 0.0, 0, "Media recortada por cantidad"};
    
    if (!data || data->count <= 0) {
        strcpy(result.description, "Error: No hay datos disponibles");
        return result;
    }
    
    if (count < 0 || count >= data->count / 2) {
        sprintf(result.description, "Error: Cantidad fuera de rango (0-%d)", data->count/2 - 1);
        return result;
    }

    // Iniciar medición de tiempo
    double start_time = get_time_ms();
    int operations = 0;

    // Obtener datos ordenados
    double* sorted = get_sorted_data(data);
    if (!sorted) {
        strcpy(result.description, "Error: No se pudieron ordenar los datos");
        return result;
    }

    // Calcular suma de los valores tras recorte
    double sum = 0.0;
    int valid_count = 0;
    
    for (int i = count; i < data->count - count; i++) {
        sum += sorted[i];
        valid_count++;
        operations++;
    }

    // Calcular media recortada
    if (valid_count > 0) {
        result.result = sum / valid_count;
        operations++;
    } else {
        result.result = 0.0;
        strcpy(result.description, "Error: No hay datos suficientes tras recorte");
    }

    // Liberar memoria
    free(sorted);

    // Finalizar medición de tiempo
    result.processing_time = time_diff_ms(start_time);
    result.operations_count = operations;

    sprintf(result.description, "Media recortada (%d valores, n=%d)", count, data->count);
    return result;
}

// Cálculo de la varianza con fórmula de muestras (n-1 en denominador)
StatResult calc_variance_sample(DataSet* data) {
    StatResult result = {0.0, 0.0, 0, "Varianza (muestra)"};
    
    if (!data || data->count <= 1) { // Se necesitan al menos 2 valores para varianza muestral
        strcpy(result.description, "Error: Se necesitan al menos 2 datos");
        return result;
    }

    // Iniciar medición de tiempo
    double start_time = get_time_ms();
    int operations = 0;

    // Primero calculamos la media
    double mean = 0.0;
    for (int i = 0; i < data->count; i++) {
        mean += data->data[i];
        operations++;
    }
    mean /= data->count;
    operations++;

    // Después calculamos la suma de cuadrados de diferencias
    double sum_squares = 0.0;
    for (int i = 0; i < data->count; i++) {
        double diff = data->data[i] - mean;
        sum_squares += diff * diff;
        operations += 3; // Resta, multiplicación y suma
    }

    // Calculamos la varianza (fórmula para muestras)
    result.result = sum_squares / (data->count - 1);
    operations++;

    // Finalizar medición de tiempo
    result.processing_time = time_diff_ms(start_time);
    result.operations_count = operations;

    sprintf(result.description, "Varianza muestral (n=%d)", data->count);
    return result;
}

// Cálculo de la desviación estándar muestral
StatResult calc_stddev_sample(DataSet* data) {
    // Calculamos varianza primero
    double start_total = get_time_ms();
    
    StatResult var_result = calc_variance_sample(data);
    
    // Inicializamos resultado para desviación
    StatResult result = {0.0, 0.0, var_result.operations_count + 1, "Desviacion estandar (muestra)"};
    strcpy(result.description, var_result.description);
    
    // Si hubo error en el cálculo de la varianza
    if (var_result.result == 0.0 && strstr(var_result.description, "Error") != NULL) {
        strcpy(result.description, var_result.description);
        return result;
    }
    
    // Tomar raíz cuadrada de la varianza
    double start_time = get_time_ms();
    result.result = sqrt(var_result.result);
    
    // Actualizar tiempo y operaciones
    double sqrt_time = time_diff_ms(start_time);
    result.processing_time = time_diff_ms(start_total);
    result.operations_count += 1; // Una operación extra (raíz cuadrada)
    
    // Cambiar descripción
    strcpy(result.description, "Desviacion estandar muestral");
    sprintf(result.description, "Desviacion estandar muestral (n=%d)", data->count);
    
    return result;
}

// Cálculo de la varianza con fórmula de población (n en denominador)
StatResult calc_variance_population(DataSet* data) {
    StatResult result = {0.0, 0.0, 0, "Varianza (poblacion)"};
    
    if (!data || data->count <= 0) {
        strcpy(result.description, "Error: No hay datos disponibles");
        return result;
    }

    // Iniciar medición de tiempo
    double start_time = get_time_ms();
    int operations = 0;

    // Primero calculamos la media
    double mean = 0.0;
    for (int i = 0; i < data->count; i++) {
        mean += data->data[i];
        operations++;
    }
    mean /= data->count;
    operations++;

    // Después calculamos la suma de cuadrados de diferencias
    double sum_squares = 0.0;
    for (int i = 0; i < data->count; i++) {
        double diff = data->data[i] - mean;
        sum_squares += diff * diff;
        operations += 3; // Resta, multiplicación y suma
    }

    // Calculamos la varianza (fórmula para población)
    result.result = sum_squares / data->count;
    operations++;

    // Finalizar medición de tiempo
    result.processing_time = time_diff_ms(start_time);
    result.operations_count = operations;

    sprintf(result.description, "Varianza poblacional (N=%d)", data->count);
    return result;
}

// Cálculo de la desviación estándar poblacional
StatResult calc_stddev_population(DataSet* data) {
    // Calculamos varianza primero
    double start_total = get_time_ms();
    
    StatResult var_result = calc_variance_population(data);
    
    // Inicializamos resultado para desviación
    StatResult result = {0.0, 0.0, var_result.operations_count + 1, "Desviacion estandar (poblacion)"};
    strcpy(result.description, var_result.description);
    
    // Si hubo error en el cálculo de la varianza
    if (var_result.result == 0.0 && strstr(var_result.description, "Error") != NULL) {
        strcpy(result.description, var_result.description);
        return result;
    }
    
    // Tomar raíz cuadrada de la varianza
    double start_time = get_time_ms();
    result.result = sqrt(var_result.result);
    
    // Actualizar tiempo y operaciones
    double sqrt_time = time_diff_ms(start_time);
    result.processing_time = time_diff_ms(start_total);
    result.operations_count += 1; // Una operación extra (raíz cuadrada)
    
    // Cambiar descripción
    strcpy(result.description, "Desviacion estandar poblacional");
    sprintf(result.description, "Desviacion estandar poblacional (N=%d)", data->count);
    
    return result;
}

void display_stat_result(StatResult result, int y_position) {
    // Colores
    const int COLOR_TITLE = BLUE;
    const int COLOR_VALUE = BLACK;
    const int COLOR_INFO = DARKGRAY;
    
    // Formato de muestra - 4 DECIMALES
    char value_str[50];
    sprintf(value_str, "%.4f", result.result);
    
    // Mostrar más decimales para el tiempo (6 en lugar de 3)
    char time_str[50];
    sprintf(time_str, "Tiempo: %.6f ms", result.processing_time);
    
    char ops_str[50];
    sprintf(ops_str, "Operaciones: %d", result.operations_count);
    
    // Dibujar fondo claro para el resultado
    setfillstyle(SOLID_FILL, COLOR(245, 245, 255));
    bar(50, y_position, WINDOW_WIDTH-50, y_position+60);
    rectangle(50, y_position, WINDOW_WIDTH-50, y_position+60);
    
    // Mostrar título
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    setcolor(COLOR_TITLE);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    outtextxy(60, y_position+10, result.description);
    
    // Mostrar valor resultado
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    setcolor(COLOR_VALUE);
    settextjustify(RIGHT_TEXT, TOP_TEXT);
    outtextxy(WINDOW_WIDTH-60, y_position+10, value_str);
    
    // Mostrar información de rendimiento
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    setcolor(COLOR_INFO);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    outtextxy(60, y_position+35, time_str);
    
    settextjustify(RIGHT_TEXT, TOP_TEXT);
    outtextxy(WINDOW_WIDTH-60, y_position+35, ops_str);
}