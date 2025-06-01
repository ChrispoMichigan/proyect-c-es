#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../include/excel_c.h"

// Función para limpiar el área de trabajo
void clear_work_area() {
    // Fondo degradado suave
    for (int i = 40; i < WINDOW_HEIGHT; i++) {
        setcolor(COLOR(245, 245, 252 - (i/20)));
        line(0, i, WINDOW_WIDTH, i);
    }
    
    // Borde sutil
    setcolor(LIGHTGRAY);
    rectangle(10, 45, WINDOW_WIDTH-10, WINDOW_HEIGHT-10);
}

// Ordenar datos para visualizaciones
double* sort_data(DataSet* data) {
    double* sorted = (double*)malloc(data->count * sizeof(double));
    int i, j;
    double temp;
    
    // Copiar datos
    for (i = 0; i < data->count; i++) {
        sorted[i] = data->data[i];
    }
    
    // Ordenar (simple bubble sort)
    for (i = 0; i < data->count - 1; i++) {
        for (j = 0; j < data->count - i - 1; j++) {
            if (sorted[j] > sorted[j + 1]) {
                temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }
    
    return sorted;
}

// Encontrar valor mínimo
double find_min(DataSet* data) {
    if (data->count == 0) return 0;
    
    double min = data->data[0];
    for (int i = 1; i < data->count; i++) {
        if (data->data[i] < min) {
            min = data->data[i];
        }
    }
    return min;
}

// Encontrar valor máximo
double find_max(DataSet* data) {
    if (data->count == 0) return 0;
    
    double max = data->data[0];
    for (int i = 1; i < data->count; i++) {
        if (data->data[i] > max) {
            max = data->data[i];
        }
    }
    return max;
}

// Calcular media
double find_mean(DataSet* data) {
    if (data->count == 0) return 0;
    
    double sum = 0;
    for (int i = 0; i < data->count; i++) {
        sum += data->data[i];
    }
    return sum / data->count;
}

// Dibujar ejes de coordenadas
void draw_axes(int x0, int y0, int width, int height, double xmin, double xmax, double ymin, double ymax, char* xlabel, char* ylabel, char* title) {
    // Dibujar un rectángulo suave de fondo para el área del gráfico
    setfillstyle(SOLID_FILL, COLOR(250, 250, 255));
    bar(x0-10, y0-height-10, x0+width+10, y0+10);
    setcolor(LIGHTGRAY);
    rectangle(x0-10, y0-height-10, x0+width+10, y0+10);
    
    // Líneas de cuadrícula suaves
    setcolor(COLOR(220, 220, 230));
    setlinestyle(DOTTED_LINE, 0, NORM_WIDTH);
    
    // Líneas verticales
    int numXTicks = 5;
    int xPixelStep = width / numXTicks;
    for (int i = 0; i <= numXTicks; i++) {
        int x = x0 + i * xPixelStep;
        line(x, y0, x, y0 - height);
    }
    
    // Líneas horizontales
    int numYTicks = 5;
    int yPixelStep = height / numYTicks;
    for (int i = 0; i <= numYTicks; i++) {
        int y = y0 - i * yPixelStep;
        line(x0, y, x0 + width, y);
    }
    
    // Restaurar estilo de línea para los ejes
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    setcolor(APP_COLOR_AXIS);
    
    // Ejes X e Y
    line(x0, y0, x0 + width, y0);
    line(x0, y0, x0, y0 - height);
    
    // El resto del código para marcas y etiquetas se mantiene igual...
    
    // Título con mejor estilo
    settextjustify(CENTER_TEXT, TOP_TEXT);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    setcolor(BLUE);
    outtextxy(x0 + width/2, y0 - height - 30, title);
}

// Visualización de diagrama de tallos y hojas
void plot_stem_leaf(DataSet* data, int stem_digit, int leaf_digit) {
    if (data->count == 0 || !data->is_loaded) {
        setcolor(RED);
        outtextxy(100, 100, (char*)"Error: No hay datos cargados");
        return;
    }
    
    clear_work_area();
    
    // Título
    setcolor(APP_COLOR_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(WINDOW_WIDTH/2, 50, (char*)"Diagrama de Tallos y Hojas");
    
    // Obtener datos ordenados
    double* sorted = sort_data(data);
    
    // Determinar factor de escala para tallo y hoja
    double stem_factor = pow(10.0, stem_digit);
    double leaf_factor = pow(10.0, leaf_digit);
    
    // Encontrar rango de tallos
    int min_stem = (int)(sorted[0] / stem_factor);
    int max_stem = (int)(sorted[data->count - 1] / stem_factor);
    
    // Dibujar leyenda
    char legend[100];
    if (stem_digit == 1 && leaf_digit == 0) {
        sprintf(legend, "Tallo = Decenas | Hoja = Unidades");
    } else if (stem_digit == 0 && leaf_digit == -1) {
        sprintf(legend, "Tallo = Unidades | Hoja = Decimas");
    } else {
        sprintf(legend, "Tallo: 10^%d | Hoja: 10^%d", stem_digit, leaf_digit);
    }
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(WINDOW_WIDTH/2, 90, legend);
    
    // Dibujar diagrama
    settextjustify(RIGHT_TEXT, CENTER_TEXT);
    int y = 100;
    
    for (int stem = min_stem; stem <= max_stem; stem++) {
        char stem_str[20];
        sprintf(stem_str, "%d |", stem);
        outtextxy(300, y, stem_str);
        
        // Buscar todas las hojas para este tallo
        int x = 310;
        for (int i = 0; i < data->count; i++) {
            int curr_stem = (int)(sorted[i] / stem_factor);
            if (curr_stem == stem) {
                int leaf = (int)(fmod(sorted[i], stem_factor) / leaf_factor);
                if (leaf < 0) leaf = -leaf; // Manejar valores negativos
                
                char leaf_str[5];
                sprintf(leaf_str, "%d", leaf);
                settextjustify(LEFT_TEXT, CENTER_TEXT);
                outtextxy(x, y, leaf_str);
                
                x += textwidth(leaf_str) + 5;
            }
        }
        
        y += 30;
    }
    
    free(sorted);
    
    // Estadísticas descriptivas
    int stats_y = y + 50;
    settextjustify(LEFT_TEXT, TOP_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    
    char stats[100];
    sprintf(stats, "Numero de datos: %d", data->count);
    outtextxy(100, stats_y, stats);
    
    sprintf(stats, "Minimo: %.2f", find_min(data));
    outtextxy(100, stats_y + 25, stats);
    
    sprintf(stats, "Maximo: %.2f", find_max(data));
    outtextxy(100, stats_y + 50, stats);
    
    sprintf(stats, "Media: %.2f", find_mean(data));
    outtextxy(100, stats_y + 75, stats);
    
    // Esperar un clic para continuar
    settextjustify(LEFT_TEXT, TOP_TEXT);
    outtextxy(100, WINDOW_HEIGHT - 50, (char*)"Clic para continuar...");
    
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}

// Visualización de gráfica de puntos (dispersión)
void plot_scatter(DataSet* data) {
    if (data->count == 0 || !data->is_loaded) {
        setcolor(RED);
        outtextxy(100, 100, (char*)"Error: No hay datos cargados");
        return;
    }
    
    clear_work_area();
    
    // Área para el gráfico
    int margin = 80;
    int x0 = margin; // Origen X
    int y0 = WINDOW_HEIGHT - margin; // Origen Y
    int width = WINDOW_WIDTH - 2 * margin;
    int height = WINDOW_HEIGHT - 2 * margin - 40; // -40 para el menú superior
    
    // Encontrar valores mínimo y máximo
    double min_val = find_min(data);
    double max_val = find_max(data);
    
    // Ajustar rango para mejor visualización
    double range = max_val - min_val;
    min_val -= range * 0.1;
    max_val += range * 0.1;
    
    // Dibujar ejes
    draw_axes(x0, y0, width, height, 0, data->count, min_val, max_val, 
              (char*)"Indice de Dato", (char*)"Valor", (char*)"Grafica de Puntos");
    
    // Dibujar puntos
    setcolor(APP_COLOR_DATA_POINT);
    for (int i = 0; i < data->count; i++) {
        int x = x0 + (i * width) / data->count;
        int y = y0 - (int)(((data->data[i] - min_val) / (max_val - min_val)) * height);
        
        // Dibujar punto
        setfillstyle(SOLID_FILL, APP_COLOR_DATA_POINT);
        fillellipse(x, y, 5, 5);
    }
    
    // Mostrar estadísticas
    int stats_y = 70;
    settextjustify(LEFT_TEXT, TOP_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    setcolor(APP_COLOR_TEXT);
    
    char stats[100];
    sprintf(stats, "Numero de datos: %d", data->count);
    outtextxy(x0, stats_y, stats);
    
    sprintf(stats, "Minimo: %.2f", find_min(data));
    outtextxy(x0, stats_y + 25, stats);
    
    sprintf(stats, "Maximo: %.2f", find_max(data));
    outtextxy(x0, stats_y + 50, stats);
    
    sprintf(stats, "Media: %.2f", find_mean(data));
    outtextxy(x0, stats_y + 75, stats);
    
    // Esperar un clic para continuar
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    setcolor(APP_COLOR_TEXT);
    outtextxy(100, WINDOW_HEIGHT - 30, (char*)"Clic para continuar...");
    
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}

// Visualización de histograma
void plot_histogram(DataSet* data, int num_classes) {
    if (data->count == 0 || !data->is_loaded) {
        setcolor(RED);
        outtextxy(100, 100, (char*)"Error: No hay datos cargados");
        return;
    }
    
    clear_work_area();
    
    // Diálogo para número de clases
    setcolor(APP_COLOR_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(200, 100, (char*)"Numero de clases para el histograma:");
    
    // Mostrar el número actual
    char num_str[10];
    sprintf(num_str, "%d", num_classes);
    
    setfillstyle(SOLID_FILL, WHITE);
    bar(400, 150, 450, 180);
    rectangle(400, 150, 450, 180);
    
    setcolor(APP_COLOR_TEXT);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(425, 165, num_str);
    
    // Botones + y -
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(460, 150, 490, 180);
    rectangle(460, 150, 490, 180);
    outtextxy(475, 165, (char*)"+");
    
    bar(360, 150, 390, 180);
    rectangle(360, 150, 390, 180);
    outtextxy(375, 165, (char*)"-");
    
    // Botón Aceptar
    bar(400, 200, 500, 230);
    rectangle(400, 200, 500, 230);
    outtextxy(450, 215, (char*)"Aceptar");
    
    // Esperar selección
    int done = 0;
    while (!done) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            // Verificar si se hizo clic en +
            if (x >= 460 && x <= 490 && y >= 150 && y <= 180) {
                if (num_classes < 20) {
                    num_classes++;
                    sprintf(num_str, "%d", num_classes);
                    
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(401, 151, 449, 179);
                    
                    setcolor(APP_COLOR_TEXT);
                    outtextxy(425, 165, num_str);
                }
            }
            // Verificar si se hizo clic en -
            else if (x >= 360 && x <= 390 && y >= 150 && y <= 180) {
                if (num_classes > 2) {
                    num_classes--;
                    sprintf(num_str, "%d", num_classes);
                    
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(401, 151, 449, 179);
                    
                    setcolor(APP_COLOR_TEXT);
                    outtextxy(425, 165, num_str);
                }
            }
            // Verificar si se hizo clic en Aceptar
            else if (x >= 400 && x <= 500 && y >= 200 && y <= 230) {
                done = 1;
            }
        }
        delay(100);
    }
    
    clear_work_area();
    
    // Área para el gráfico
    int margin = 80;
    int x0 = margin; // Origen X
    int y0 = WINDOW_HEIGHT - margin; // Origen Y
    int width = WINDOW_WIDTH - 2 * margin;
    int height = WINDOW_HEIGHT - 2 * margin - 40; // -40 para el menú superior
    
    // Obtener datos ordenados
    double* sorted = sort_data(data);
    
    // Encontrar valores mínimo y máximo
    double min_val = sorted[0];
    double max_val = sorted[data->count - 1];
    
    // Calcular ancho de clase
    double class_width = (max_val - min_val) / num_classes;
    if (class_width == 0) { // Todos los valores son iguales
        class_width = 1.0;
        max_val = min_val + num_classes;
    }
    
    // Inicializar contador de frecuencias
    int* frequencies = (int*)calloc(num_classes, sizeof(int));
    
    // Contar frecuencias
    for (int i = 0; i < data->count; i++) {
        int class_index = (int)((sorted[i] - min_val) / class_width);
        if (class_index == num_classes) class_index--; // Para el valor máximo
        frequencies[class_index]++;
    }
    
    // Encontrar la frecuencia máxima
    int max_freq = 0;
    for (int i = 0; i < num_classes; i++) {
        if (frequencies[i] > max_freq) {
            max_freq = frequencies[i];
        }
    }
    
    // Dibujar ejes
    char title[50];
    sprintf(title, "Histograma (%d clases)", num_classes);
    draw_axes(x0, y0, width, height, min_val, max_val, 0, max_freq * 1.1, 
              (char*)"Valor", (char*)"Frecuencia", title);
    
    // Dibujar barras
    int bar_width = width / num_classes;
    
    for (int i = 0; i < num_classes; i++) {
        double class_start = min_val + i * class_width;
        int x = x0 + (i * width) / num_classes;
        int bar_height = (frequencies[i] * height) / (max_freq * 1.1);
        
        // Dibujar barra
        setfillstyle(SOLID_FILL, APP_COLOR_HISTOGRAM_BAR);
        bar(x, y0 - bar_height, x + bar_width, y0);
        
        // Borde de la barra
        setcolor(BLACK);
        rectangle(x, y0 - bar_height, x + bar_width, y0);
        
        // Mostrar frecuencia encima de la barra
        if (frequencies[i] > 0) {
            char freq_str[10];
            sprintf(freq_str, "%d", frequencies[i]);
            settextjustify(CENTER_TEXT, BOTTOM_TEXT);
            outtextxy(x + bar_width/2, y0 - bar_height - 5, freq_str);
        }
    }
    
    // Mostrar estadísticas
    int stats_y = 70;
    settextjustify(LEFT_TEXT, TOP_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    setcolor(APP_COLOR_TEXT);
    
    char stats[100];
    sprintf(stats, "Numero de datos: %d", data->count);
    outtextxy(x0, stats_y, stats);
    
    sprintf(stats, "Ancho de clase: %.2f", class_width);
    outtextxy(x0, stats_y + 25, stats);
    
    sprintf(stats, "Minimo: %.2f", min_val);
    outtextxy(x0, stats_y + 50, stats);
    
    sprintf(stats, "Maximo: %.2f", max_val);
    outtextxy(x0, stats_y + 75, stats);
    
    free(sorted);
    free(frequencies);
    
    // Esperar un clic para continuar
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    setcolor(APP_COLOR_TEXT);
    outtextxy(100, WINDOW_HEIGHT - 30, (char*)"Clic para continuar...");
    
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}