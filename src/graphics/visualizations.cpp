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

// Visualización mejorada de diagrama de tallos y hojas con grid y desplazamiento vertical
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
    
    // Calcular número total de tallos
    int total_stems = max_stem - min_stem + 1;
    
    // Dibujar leyenda
    char legend[100];
    if (stem_digit == 1 && leaf_digit == 0) {
        sprintf(legend, "Tallo = Decenas | Hoja = Unidades");
    } else if (stem_digit == 0 && leaf_digit == -1) {
        sprintf(legend, "Tallo = Unidades | Hoja = Décimas");
    } else {
        sprintf(legend, "Tallo: 10^%d | Hoja: 10^%d", stem_digit, leaf_digit);
    }
    
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    outtextxy(WINDOW_WIDTH/2, 80, legend);
    
    // Definir área visible para el diagrama
    int grid_top = 100;
    int grid_bottom = WINDOW_HEIGHT - 80;
    int grid_height = grid_bottom - grid_top;
    
    // Establecer exactamente 10 filas visibles por página
    int visible_rows = 10;
    int row_height = 24;
    
    // Variables para la paginación
    int current_page = 0;
    int total_pages = (total_stems + visible_rows - 1) / visible_rows;
    
    // Crear una estructura para almacenar las hojas por tallo
    typedef struct {
        int stem;
        int* leaves;
        int count;
    } StemData;
    
    StemData* stems = (StemData*)malloc(total_stems * sizeof(StemData));
    
    // Inicializar la estructura
    for (int i = 0; i < total_stems; i++) {
        stems[i].stem = min_stem + i;
        stems[i].leaves = (int*)malloc(data->count * sizeof(int)); // Máximo posible de hojas
        stems[i].count = 0;
    }
    
    // Clasificar los datos en la estructura
    for (int i = 0; i < data->count; i++) {
        int stem_value = (int)(sorted[i] / stem_factor);
        int leaf_value = (int)(fmod(sorted[i], stem_factor) / leaf_factor);
        if (leaf_value < 0) leaf_value = -leaf_value; // Manejar valores negativos
        
        int stem_index = stem_value - min_stem;
        stems[stem_index].leaves[stems[stem_index].count++] = leaf_value;
    }
    
    // Función para dibujar la cuadrícula y los datos
    auto drawStemLeafPage = [&](int page) {
        // Limpiar área de la cuadrícula
        setfillstyle(SOLID_FILL, WHITE);
        bar(10, grid_top, WINDOW_WIDTH - 10, grid_bottom);
        
        // Dibujar encabezados de tabla
        setfillstyle(SOLID_FILL, COLOR(220, 230, 240));
        bar(10, grid_top, WINDOW_WIDTH - 10, grid_top + 26);
        
        setcolor(DARKGRAY);
        line(10, grid_top + 26, WINDOW_WIDTH - 10, grid_top + 26);
        line(100, grid_top, 100, grid_bottom);
        
        // Encabezados
        setcolor(BLUE);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        outtextxy(55, grid_top + 18, (char*)"Tallo");
        outtextxy(WINDOW_WIDTH/2, grid_top + 18, (char*)"Hojas");
        
        // Calcular qué filas mostrar en esta página (exactamente 10 por página)
        int start_stem = page * 10;
        int end_stem = start_stem + 10;
        if (end_stem > total_stems) end_stem = total_stems;
        
        // Dibujar filas de datos
        setcolor(APP_COLOR_TEXT);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        
        for (int i = start_stem; i < end_stem; i++) {
            int y = grid_top + 26 + (i - start_stem) * row_height + row_height/2;
            
            // Dibujar línea divisoria horizontal
            setcolor(LIGHTGRAY);
            line(10, y + row_height/2, WINDOW_WIDTH - 10, y + row_height/2);
            
            // Dibujar tallo
            setcolor(APP_COLOR_TEXT);
            char stem_str[20];
            sprintf(stem_str, "%d", stems[i].stem);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            outtextxy(55, y + 5, stem_str);
            
            // Dibujar hojas con formato ordenado
            setcolor(APP_COLOR_DATA_POINT);
            settextjustify(LEFT_TEXT, CENTER_TEXT);
            
            int x = 110;
            // Ordenar las hojas
            for (int j = 0; j < stems[i].count - 1; j++) {
                for (int k = 0; k < stems[i].count - j - 1; k++) {
                    if (stems[i].leaves[k] > stems[i].leaves[k+1]) {
                        int temp = stems[i].leaves[k];
                        stems[i].leaves[k] = stems[i].leaves[k+1];
                        stems[i].leaves[k+1] = temp;
                    }
                }
            }
            
            // Mostrar las hojas ordenadas con espaciado uniforme
            for (int j = 0; j < stems[i].count; j++) {
                char leaf_str[5];
                sprintf(leaf_str, "%d", stems[i].leaves[j]);
                
                if (x + textwidth(leaf_str) + 10 < WINDOW_WIDTH - 20) {
                    outtextxy(x, y + 5, leaf_str);
                    x += textwidth(leaf_str) + 10;  // Espaciado uniforme
                } else {
                    // Si no caben más, poner elipsis y terminar
                    outtextxy(x, y + 5, (char*)"...");
                    break;
                }
            }
        }
        
        // Mostrar información de paginación
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        settextjustify(CENTER_TEXT, TOP_TEXT);
        setcolor(BLUE);
        
        char page_info[50];
        sprintf(page_info, "Pag %d de %d", page + 1, total_pages);
        outtextxy(WINDOW_WIDTH/2, grid_bottom + 5, page_info);
        
        // Dibujar botones de navegación
        // Botón "Anterior" - solo activo si hay página anterior
        if (page > 0) {
            setfillstyle(SOLID_FILL, LIGHTGRAY);
            setcolor(BLACK);
        } else {
            setfillstyle(SOLID_FILL, COLOR(230,230,230));
            setcolor(DARKGRAY);
        }
        bar(50, grid_bottom + 30, 150, grid_bottom + 60);
        rectangle(50, grid_bottom + 30, 150, grid_bottom + 60);
        outtextxy(100, grid_bottom + 35, (char*)"<- Anterior");
        
        // Botón "Siguiente" - solo activo si hay página siguiente
        if (page < total_pages - 1) {
            setfillstyle(SOLID_FILL, LIGHTGRAY);
            setcolor(BLACK);
        } else {
            setfillstyle(SOLID_FILL, COLOR(230,230,230));
            setcolor(DARKGRAY);
        }
        bar(WINDOW_WIDTH - 150, grid_bottom + 30, WINDOW_WIDTH - 50, grid_bottom + 60);
        rectangle(WINDOW_WIDTH - 150, grid_bottom + 30, WINDOW_WIDTH - 50, grid_bottom + 60);
        outtextxy(WINDOW_WIDTH - 100, grid_bottom + 35, (char*)"Siguiente ->");
    };
    
    // Dibujar la primera página
    drawStemLeafPage(current_page);
    
    // Bucle principal para manejar la paginación
    bool done = false;
    while (!done) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            // Botón "Anterior" - solo procesar el clic si el botón está activo
            if (x >= 50 && x <= 150 && y >= grid_bottom + 30 && y <= grid_bottom + 60) {
                if (current_page > 0) {
                    current_page--;
                    drawStemLeafPage(current_page);
                }
            }
            
            // Botón "Siguiente" - solo procesar el clic si el botón está activo
            else if (x >= WINDOW_WIDTH - 150 && x <= WINDOW_WIDTH - 50 && 
                     y >= grid_bottom + 30 && y <= grid_bottom + 60) {
                if (current_page < total_pages - 1) {
                    current_page++;
                    drawStemLeafPage(current_page);
                }
            }
            
            // Salir con cualquier otro clic
            else {
                done = true;
            }
        }
        
        // También permitir salir con tecla Enter o Escape
        if (kbhit()) {
            char key = getch();
            if (key == 13 || key == 27) { // Enter o Escape
                done = true;
            }
        }
        
        delay(50);
    }
    
    // Liberar memoria
    for (int i = 0; i < total_stems; i++) {
        free(stems[i].leaves);
    }
    free(stems);
    free(sorted);
}

// Visualización mejorada de gráfica de puntos (estilo horizontal con apilamiento)
void plot_scatter(DataSet* data) {
    if (data->count == 0 || !data->is_loaded) {
        setcolor(RED);
        outtextxy(100, 100, (char*)"Error: No hay datos cargados");
        return;
    }
    
    clear_work_area();
    
    // Título
    setcolor(RED);  // Título en rojo como en la imagen de referencia
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    outtextxy(50, 50, (char*)"Graficas de puntos");
    
    // Área para el gráfico - ajustada para maximizar el área visible
    int margin_left = 60;    // Reducido para más espacio horizontal
    int margin_right = 30;   // Reducido para más espacio horizontal
    int margin_top = 100;    // Ajustado para dar más espacio arriba
    int margin_bottom = 80;  // Ajustado para dar más espacio abajo
    
    int x0 = margin_left; // Origen X (izquierda)
    int y0 = WINDOW_HEIGHT - margin_bottom; // Origen Y (abajo)
    int width = WINDOW_WIDTH - margin_left - margin_right;
    int height = 150; // Altura aumentada considerablemente para el área del gráfico
    
    // Encontrar valores mínimo y máximo
    double min_val = find_min(data);
    double max_val = find_max(data);
    
    // Ajustar rango para mejor visualización
    double range = max_val - min_val;
    min_val = floor(min_val - range * 0.05);
    max_val = ceil(max_val + range * 0.05);
    
    if (range == 0) { // Si todos los valores son iguales
        min_val -= 1;
        max_val += 1;
    }
    
    // Dibujar fondo del gráfico - MUCHO MÁS GRANDE
    setfillstyle(SOLID_FILL, WHITE);
    bar(x0-10, y0-height-10, x0+width+10, y0+10);
    setcolor(BLACK);
    rectangle(x0-10, y0-height-10, x0+width+10, y0+10);
    
    // Dibujar eje horizontal
    line(x0, y0, x0 + width, y0);
    
    // Marcas y etiquetas en el eje X - TAMAÑO DE FUENTE AUMENTADO
    int numXTicks = 6;  // Número de marcas en el eje X
    int xPixelStep = width / numXTicks;
    
    // Crear marcas más visibles
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1); // Fuente más grande y clara
    for (int i = 0; i <= numXTicks; i++) {
        int x = x0 + i * xPixelStep;
        line(x, y0, x, y0 + 5);  // Marca
        
        char tickLabel[20];
        double value = min_val + i * ((max_val - min_val) / numXTicks);
        sprintf(tickLabel, "%.0f", value);
        settextjustify(CENTER_TEXT, TOP_TEXT);
        outtextxy(x, y0 + 10, tickLabel); // Mayor separación para los números
    }
    
    // Estructura para contar la frecuencia de cada valor
    int num_bins = 200;  // Número de bins para discretización
    int* frequencies = (int*)calloc(num_bins, sizeof(int));
    double bin_width = (max_val - min_val) / num_bins;
    
    // Contar frecuencias por bin
    for (int i = 0; i < data->count; i++) {
        int bin = (int)((data->data[i] - min_val) / bin_width);
        if (bin >= num_bins) bin = num_bins - 1; 
        if (bin < 0) bin = 0;  // Para valores que puedan estar justo por debajo del mínimo ajustado
        frequencies[bin]++;
    }
    
    // Encontrar la frecuencia máxima para escalar la altura de apilamiento
    int max_freq = 0;
    for (int i = 0; i < num_bins; i++) {
        if (frequencies[i] > max_freq) {
            max_freq = frequencies[i];
        }
    }
    
    int max_stack_height = height - 10; // Casi toda la altura disponible
    
    // Dibujar puntos agrupados por valor
    setcolor(BLACK);
    for (int b = 0; b < num_bins; b++) {
        if (frequencies[b] > 0) {
            // Calcular la posición X correspondiente al bin
            double bin_value = min_val + (b + 0.5) * bin_width;
            int x = x0 + (int)((bin_value - min_val) * width / (max_val - min_val));
            
            // Dibujar puntos apilados según su frecuencia
            for (int f = 0; f < frequencies[b]; f++) {
                // Calcular posición Y del punto - ajustado para más espacio vertical
                int y = y0 - 5 - (f * max_stack_height / (max_freq * 1.2)); // Ajuste para no llegar al límite superior
                
                // Dibujar punto
                setfillstyle(SOLID_FILL, BLACK);
                fillellipse(x, y, 2, 2);
            }
        }
    }
    
    // Liberar memoria
    free(frequencies);
    
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