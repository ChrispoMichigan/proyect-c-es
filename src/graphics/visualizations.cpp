#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../include/excel_c.h"

// Función para limpiar el área de trabajo
void clear_work_area() {
    // Fondo degradado suave - optimizado para 640x480
    for (int i = 40; i < WINDOW_HEIGHT; i++) {
        setcolor(COLOR(245, 245, 252 - (i/10)));  // Ajustado divisor para mejor degradado
        line(0, i, WINDOW_WIDTH, i);
    }
    
    // Borde sutil
    setcolor(LIGHTGRAY);
    rectangle(5, 45, WINDOW_WIDTH-5, WINDOW_HEIGHT-5);  // Reducido margen de 10 a 5
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

// Dibujar ejes de coordenadas - ajustado para resolución menor
void draw_axes(int x0, int y0, int width, int height, double xmin, double xmax, double ymin, double ymax, char* xlabel, char* ylabel, char* title) {
    // Dibujar un rectángulo suave de fondo para el área del gráfico
    setfillstyle(SOLID_FILL, COLOR(250, 250, 255));
    bar(x0-5, y0-height-5, x0+width+5, y0+5);  // Reducido margen de 10 a 5
    setcolor(LIGHTGRAY);
    rectangle(x0-5, y0-height-5, x0+width+5, y0+5);
    
    // Líneas de cuadrícula suaves
    setcolor(COLOR(220, 220, 230));
    setlinestyle(DOTTED_LINE, 0, NORM_WIDTH);
    
    // Líneas verticales - menos líneas para evitar saturación
    int numXTicks = 4;  // Reducido de 5 a 4
    int xPixelStep = width / numXTicks;
    for (int i = 0; i <= numXTicks; i++) {
        int x = x0 + i * xPixelStep;
        line(x, y0, x, y0 - height);
    }
    
    // Líneas horizontales - menos líneas para evitar saturación
    int numYTicks = 4;  // Reducido de 5 a 4
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
    
    // Marcas en eje X
    for (int i = 0; i <= numXTicks; i++) {
        int x = x0 + i * xPixelStep;
        line(x, y0, x, y0 + 3);  // Reducido tamaño de marcas
        
        char tickLabel[20];
        sprintf(tickLabel, "%.1f", xmin + i * ((xmax - xmin) / numXTicks));
        settextjustify(CENTER_TEXT, TOP_TEXT);
        settextstyle(SMALL_FONT, HORIZ_DIR, 5);  // Fuente más pequeña
        outtextxy(x, y0 + 4, tickLabel);
    }
    
    // Marcas en eje Y
    for (int i = 0; i <= numYTicks; i++) {
        int y = y0 - i * yPixelStep;
        line(x0 - 3, y, x0, y);  // Reducido tamaño de marcas
        
        char tickLabel[20];
        sprintf(tickLabel, "%.1f", ymin + i * ((ymax - ymin) / numYTicks));
        settextjustify(RIGHT_TEXT, CENTER_TEXT);
        settextstyle(SMALL_FONT, HORIZ_DIR, 5);  // Fuente más pequeña
        outtextxy(x0 - 4, y, tickLabel);
    }
    
    // Etiquetas
    settextjustify(CENTER_TEXT, TOP_TEXT);
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);
    outtextxy(x0 + width/2, y0 + 20, xlabel);  // Reducido espacio vertical
    
    settextjustify(CENTER_TEXT, BOTTOM_TEXT);
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);
    outtextxy(x0 - 25, y0 - height/2, ylabel);  // Reducido espacio horizontal
    
    // Título con mejor estilo
    settextjustify(CENTER_TEXT, TOP_TEXT);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);  // Tamaño reducido
    setcolor(BLUE);
    outtextxy(x0 + width/2, y0 - height - 15, title);  // Reducido espacio vertical
}

// Visualización de diagrama de tallos y hojas - ajustado para resolución menor
void plot_stem_leaf(DataSet* data, int stem_digit, int leaf_digit) {
    if (data->count == 0 || !data->is_loaded) {
        setcolor(RED);
        outtextxy(100, 100, (char*)"Error: No hay datos cargados");
        return;
    }
    
    clear_work_area();
    
    // Título
    setcolor(APP_COLOR_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);  // Tamaño reducido
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
    
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);  // Fuente más pequeña
    outtextxy(WINDOW_WIDTH/2, 70, legend);  // Posición vertical reducida
    
    // Dibujar diagrama
    settextjustify(RIGHT_TEXT, CENTER_TEXT);
    int y = 90;  // Posición inicial más arriba
    int spacing = 20;  // Espacio vertical reducido entre filas
    
    for (int stem = min_stem; stem <= max_stem; stem++) {
        char stem_str[20];
        sprintf(stem_str, "%d |", stem);
        outtextxy(200, y, stem_str);  // Posición X reducida
        
        // Buscar todas las hojas para este tallo
        int x = 210;  // Posición X reducida
        for (int i = 0; i < data->count; i++) {
            int curr_stem = (int)(sorted[i] / stem_factor);
            if (curr_stem == stem) {
                int leaf = (int)(fmod(sorted[i], stem_factor) / leaf_factor);
                if (leaf < 0) leaf = -leaf; // Manejar valores negativos
                
                char leaf_str[5];
                sprintf(leaf_str, "%d", leaf);
                settextjustify(LEFT_TEXT, CENTER_TEXT);
                outtextxy(x, y, leaf_str);
                
                x += textwidth(leaf_str) + 3;  // Espacio horizontal reducido
            }
        }
        
        y += spacing;  // Menos espacio entre líneas
        
        // Si se está saliendo de la pantalla, ajustar
        if (y > WINDOW_HEIGHT - 100) {
            spacing = 15;  // Reducir aún más el espaciado si hay muchos tallos
        }
    }
    
    free(sorted);
    
    // Estadísticas descriptivas - compactas para pantalla pequeña
    int stats_y = WINDOW_HEIGHT - 95;  // Posicionadas más abajo
    settextjustify(LEFT_TEXT, TOP_TEXT);
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);  // Fuente pequeña
    
    char stats[100];
    sprintf(stats, "Datos: %d | Min: %.2f | Max: %.2f | Media: %.2f", 
            data->count, find_min(data), find_max(data), find_mean(data));
    outtextxy(10, stats_y, stats);  // Una sola línea con toda la info
    
    // Esperar un clic para continuar
    settextjustify(LEFT_TEXT, TOP_TEXT);
    outtextxy(10, WINDOW_HEIGHT - 20, (char*)"Clic para continuar...");
    
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}

// Visualización de gráfica de puntos (dispersión) - ajustada para resolución menor
void plot_scatter(DataSet* data) {
    if (data->count == 0 || !data->is_loaded) {
        setcolor(RED);
        outtextxy(100, 100, (char*)"Error: No hay datos cargados");
        return;
    }
    
    clear_work_area();
    
    // Área para el gráfico - márgenes reducidos para pantalla pequeña
    int margin = 50;  // Reducido de 80 a 50
    int x0 = margin; // Origen X
    int y0 = WINDOW_HEIGHT - margin; // Origen Y
    int width = WINDOW_WIDTH - 2 * margin;
    int height = WINDOW_HEIGHT - 2 * margin - 40; // -40 para el menú superior
    
    // Encontrar valores mínimo y máximo
    double min_val = find_min(data);
    double max_val = find_max(data);
    
    // Ajustar rango para mejor visualización
    double range = max_val - min_val;
    min_val -= range * 0.05;  // Reducido margen
    max_val += range * 0.05;  // Reducido margen
    
    // Dibujar ejes
    draw_axes(x0, y0, width, height, 0, data->count, min_val, max_val, 
              (char*)"Indice", (char*)"Valor", (char*)"Grafica de Puntos");
    
    // Dibujar puntos - tamaño reducido para pantallas pequeñas
    setcolor(APP_COLOR_DATA_POINT);
    for (int i = 0; i < data->count; i++) {
        int x = x0 + (i * width) / data->count;
        int y = y0 - (int)(((data->data[i] - min_val) / (max_val - min_val)) * height);
        
        // Dibujar punto
        setfillstyle(SOLID_FILL, APP_COLOR_DATA_POINT);
        fillellipse(x, y, 3, 3);  // Tamaño reducido de 5,5 a 3,3
    }
    
    // Mostrar estadísticas - compactas y en la parte superior
    settextjustify(LEFT_TEXT, TOP_TEXT);
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);  // Fuente pequeña
    setcolor(APP_COLOR_TEXT);
    
    char stats[100];
    sprintf(stats, "Datos:%d Min:%.2f Max:%.2f Media:%.2f", 
            data->count, find_min(data), find_max(data), find_mean(data));
    outtextxy(50, 50, stats);  // Una sola línea con toda la info
    
    // Esperar un clic para continuar
    settextjustify(LEFT_TEXT, TOP_TEXT);
    setcolor(APP_COLOR_TEXT);
    outtextxy(10, WINDOW_HEIGHT - 20, (char*)"Clic para continuar...");
    
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}

// Visualización de histograma - ajustada para resolución menor
void plot_histogram(DataSet* data, int num_classes) {
    if (data->count == 0 || !data->is_loaded) {
        setcolor(RED);
        outtextxy(100, 100, (char*)"Error: No hay datos cargados");
        return;
    }
    
    clear_work_area();
    
    // Diálogo para número de clases - ajustado para pantalla pequeña
    setcolor(APP_COLOR_TEXT);
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);  // Fuente más pequeña
    outtextxy(100, 80, (char*)"Numero de clases:");  // Posición X e Y reducidas
    
    // Mostrar el número actual
    char num_str[10];
    sprintf(num_str, "%d", num_classes);
    
    // Posiciones ajustadas para pantalla pequeña
    int btnX = WINDOW_WIDTH/2;
    int btnY = 120;
    
    setfillstyle(SOLID_FILL, WHITE);
    bar(btnX-25, btnY, btnX+25, btnY+30);
    rectangle(btnX-25, btnY, btnX+25, btnY+30);
    
    setcolor(APP_COLOR_TEXT);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(btnX, btnY+15, num_str);
    
    // Botones + y - con posiciones relativas
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(btnX+35, btnY, btnX+65, btnY+30);
    rectangle(btnX+35, btnY, btnX+65, btnY+30);
    outtextxy(btnX+50, btnY+15, (char*)"+");
    
    bar(btnX-65, btnY, btnX-35, btnY+30);
    rectangle(btnX-65, btnY, btnX-35, btnY+30);
    outtextxy(btnX-50, btnY+15, (char*)"-");
    
    // Botón Aceptar
    bar(btnX-50, btnY+50, btnX+50, btnY+80);
    rectangle(btnX-50, btnY+50, btnX+50, btnY+80);
    outtextxy(btnX, btnY+65, (char*)"Aceptar");
    
    // Esperar selección
    int done = 0;
    while (!done) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            // Verificar si se hizo clic en +
            if (x >= btnX+35 && x <= btnX+65 && y >= btnY && y <= btnY+30) {
                if (num_classes < 15) {  // Máximo reducido para pantalla pequeña
                    num_classes++;
                    sprintf(num_str, "%d", num_classes);
                    
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(btnX-24, btnY+1, btnX+24, btnY+29);
                    
                    setcolor(APP_COLOR_TEXT);
                    outtextxy(btnX, btnY+15, num_str);
                }
            }
            // Verificar si se hizo clic en -
            else if (x >= btnX-65 && x <= btnX-35 && y >= btnY && y <= btnY+30) {
                if (num_classes > 2) {
                    num_classes--;
                    sprintf(num_str, "%d", num_classes);
                    
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(btnX-24, btnY+1, btnX+24, btnY+29);
                    
                    setcolor(APP_COLOR_TEXT);
                    outtextxy(btnX, btnY+15, num_str);
                }
            }
            // Verificar si se hizo clic en Aceptar
            else if (x >= btnX-50 && x <= btnX+50 && y >= btnY+50 && y <= btnY+80) {
                done = 1;
            }
        }
        delay(100);
    }
    
    clear_work_area();
    
    // Área para el gráfico - márgenes reducidos para pantalla pequeña
    int margin = 50;  // Reducido de 80 a 50
    int x0 = margin; // Origen X
    int y0 = WINDOW_HEIGHT - margin; // Origen Y
    int width = WINDOW_WIDTH - 2 * margin;
    int height = WINDOW_HEIGHT - 2 * margin - 40; // -40 para el menú superior
    
    // Cálculos de datos para el histograma (sin cambios)
    double* sorted = sort_data(data);
    double min_val = sorted[0];
    double max_val = sorted[data->count - 1];
    double class_width = (max_val - min_val) / num_classes;
    if (class_width == 0) { // Todos los valores son iguales
        class_width = 1.0;
        max_val = min_val + num_classes;
    }
    
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
    
    // Dibujar barras - ajustar ancho para pantalla pequeña
    int bar_width = (width / num_classes) - 1;  // -1 para separación
    
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
        
        // Mostrar frecuencia encima de la barra solo si hay espacio
        if (frequencies[i] > 0 && bar_height > 15) {
            char freq_str[10];
            sprintf(freq_str, "%d", frequencies[i]);
            settextjustify(CENTER_TEXT, BOTTOM_TEXT);
            settextstyle(SMALL_FONT, HORIZ_DIR, 5);  // Fuente pequeña
            outtextxy(x + bar_width/2, y0 - bar_height - 2, freq_str);
        }
    }
    
    // Mostrar estadísticas - compactas para pantalla pequeña
    settextjustify(LEFT_TEXT, TOP_TEXT);
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);
    setcolor(APP_COLOR_TEXT);
    
    char stats[100];
    sprintf(stats, "Datos:%d Ancho:%.2f Min:%.2f Max:%.2f", 
            data->count, class_width, min_val, max_val);
    outtextxy(50, 50, stats);
    
    free(sorted);
    free(frequencies);
    
    // Esperar un clic para continuar
    settextjustify(LEFT_TEXT, TOP_TEXT);
    setcolor(APP_COLOR_TEXT);
    outtextxy(10, WINDOW_HEIGHT - 20, (char*)"Clic para continuar...");
    
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}