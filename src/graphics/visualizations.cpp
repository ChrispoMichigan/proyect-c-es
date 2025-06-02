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

// Función mejorada para dibujar ejes con texto más grande
void draw_axes(int x0, int y0, int width, int height, double xmin, double xmax, double ymin, double ymax, char* xlabel, char* ylabel, char* title) {
    // Dibujar un rectángulo suave de fondo para el área del gráfico
    setfillstyle(SOLID_FILL, COLOR(250, 250, 255));
    bar(x0-5, y0-height-5, x0+width+5, y0+5);
    setcolor(LIGHTGRAY);
    rectangle(x0-5, y0-height-5, x0+width+5, y0+5);
    
    // Líneas de cuadrícula suaves
    setcolor(COLOR(220, 220, 230));
    setlinestyle(DOTTED_LINE, 0, NORM_WIDTH);
    
    // Líneas verticales
    int numXTicks = 4;
    int xPixelStep = width / numXTicks;
    for (int i = 0; i <= numXTicks; i++) {
        int x = x0 + i * xPixelStep;
        line(x, y0, x, y0 - height);
    }
    
    // Líneas horizontales
    int numYTicks = 4;
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
    
    // MEJORA: Texto más grande para los ejes
    
    // Marcas en eje X
    for (int i = 0; i <= numXTicks; i++) {
        int x = x0 + i * xPixelStep;
        line(x, y0, x, y0 + 3);
        
        char tickLabel[20];
        sprintf(tickLabel, "%.1f", xmin + i * ((xmax - xmin) / numXTicks));
        settextjustify(CENTER_TEXT, TOP_TEXT);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1); // Aumentado de SMALL_FONT a SANS_SERIF_FONT
        setcolor(COLOR(180, 0, 0)); // Color de texto más visible (rojo oscuro)
        outtextxy(x, y0 + 4, tickLabel);
    }
    
    // Marcas en eje Y
    for (int i = 0; i <= numYTicks; i++) {
        int y = y0 - i * yPixelStep;
        line(x0 - 3, y, x0, y);
        
        char tickLabel[20];
        sprintf(tickLabel, "%.1f", ymin + i * ((ymax - ymin) / numYTicks));
        settextjustify(RIGHT_TEXT, CENTER_TEXT);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1); // Aumentado de SMALL_FONT a SANS_SERIF_FONT
        setcolor(COLOR(180, 0, 0)); // Color de texto más visible (rojo oscuro)
        outtextxy(x0 - 6, y, tickLabel);
    }
    
    // Etiquetas de ejes con mayor tamaño
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1); // Tamaño aumentado
    settextjustify(CENTER_TEXT, TOP_TEXT);
    setcolor(COLOR(180, 0, 0));
    outtextxy(x0 + width/2, y0 + 25, xlabel);
    
    settextjustify(RIGHT_TEXT, CENTER_TEXT);
    setlinestyle(0, 0, 1);
    settextstyle(SANS_SERIF_FONT, VERT_DIR, 1); // Tamaño aumentado
    outtextxy(x0 - 35, y0 - height/2, ylabel);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    
    // Título con mejor estilo y tamaño
    settextjustify(CENTER_TEXT, TOP_TEXT);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2); // Título más grande
    setcolor(BLUE);
    outtextxy(x0 + width/2, y0 - height - 20, title);
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

// Visualización de histograma mejorada con números más grandes
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
    outtextxy(100, 80, (char*)"Numero de clases:");
    
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
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
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
                if (num_classes < 15) {
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
    
    // Área para el gráfico - ajustada para mejor visualización
    int margin_top = 60;
    int margin_bottom = 60;
    int margin_left = 60;
    int margin_right = 40;
    
    int x0 = margin_left; // Origen X
    int y0 = WINDOW_HEIGHT - margin_bottom; // Origen Y
    int width = WINDOW_WIDTH - margin_left - margin_right;
    int height = WINDOW_HEIGHT - margin_top - margin_bottom;
    
    // Cálculos de datos para el histograma
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
    
    // Preparar área para el gráfico
    setfillstyle(SOLID_FILL, COLOR(248, 248, 255)); // Fondo suave
    bar(x0-10, y0-height-10, x0+width+10, y0+40); // Área extendida para las etiquetas
    setcolor(LIGHTGRAY);
    rectangle(x0-10, y0-height-10, x0+width+10, y0+40);
    
    // Dibujar título
    char title[50];
    sprintf(title, "Histograma (%d clases)", num_classes);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(BLUE);
    outtextxy(x0 + width/2, y0-height-20, title);
    
    // Dibujar líneas de cuadrícula suaves
    setlinestyle(DOTTED_LINE, 0, NORM_WIDTH);
    setcolor(LIGHTGRAY);
    
    // Líneas horizontales
    int numYTicks = 5;
    int yPixelStep = height / numYTicks;
    for (int i = 0; i <= numYTicks; i++) {
        int y = y0 - i * yPixelStep;
        line(x0, y, x0 + width, y);
        
        // Etiquetas del eje Y (frecuencia)
        if (i > 0) { // Evitar el 0 para evitar superposición
            char tickLabel[20];
            sprintf(tickLabel, "%.1f", (max_freq * 1.1) * i / numYTicks);
            settextjustify(RIGHT_TEXT, CENTER_TEXT);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            setcolor(DARKGRAY);
            outtextxy(x0 - 5, y, tickLabel);
        }
    }
    
    // Líneas verticales más sutiles entre las barras
    for (int i = 0; i <= num_classes; i++) {
        int x = x0 + (i * width) / num_classes;
        line(x, y0, x, y0 - height);
    }
    
    // Restaurar estilo de línea
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    
    // Ejes principales
    setcolor(BLACK);
    line(x0, y0, x0 + width, y0); // Eje X
    line(x0, y0, x0, y0 - height); // Eje Y
    
    // Dibujar barras
    int bar_width = (width / num_classes) - 2; // Espacio para separación
    
    for (int i = 0; i < num_classes; i++) {
        double class_start = min_val + i * class_width;
        int x = x0 + (i * width) / num_classes + 1; // +1 para separación
        int bar_height = (frequencies[i] * height) / (max_freq * 1.1);
        
        // Dibujar barra
        setfillstyle(SOLID_FILL, APP_COLOR_HISTOGRAM_BAR);
        bar(x, y0 - bar_height, x + bar_width, y0);
        
        // Borde de la barra
        setcolor(BLACK);
        rectangle(x, y0 - bar_height, x + bar_width, y0);
        
        // Mostrar frecuencia encima de la barra 
        if (frequencies[i] > 0 && bar_height > 15) {
            char freq_str[10];
            sprintf(freq_str, "%d", frequencies[i]);
            settextjustify(CENTER_TEXT, BOTTOM_TEXT);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            setcolor(BLACK);
            outtextxy(x + bar_width/2, y0 - bar_height - 2, freq_str);
        }
        
        // Etiqueta del eje X 
        if (i % (num_classes > 7 ? 2 : 1) == 0) { // Mostrar cada etiqueta o cada 2 si hay muchas
            char label[20];
            sprintf(label, "%.1f", class_start);
            settextjustify(CENTER_TEXT, TOP_TEXT);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            setcolor(BLACK);
            outtextxy(x + bar_width/2, y0 + 5, label);
        }
    }
    
    // Etiqueta del eje X
    settextjustify(CENTER_TEXT, TOP_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(BLACK);
    outtextxy(x0 + width/2, y0 + 35, (char*)"Valor");
    
    free(sorted);
    free(frequencies);
    
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}

// Función mejorada para regresión lineal con datos divididos
void plot_regression(DataSet* data) {
    if (data->count < 2 || !data->is_loaded) {
        setcolor(RED);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, (char*)"Error: Se necesitan al menos 2 datos para la regresion");
        delay(2000);
        return;
    }
    
    // Verificar que hay suficientes datos para formar pares X,Y
    if (data->count % 2 != 0) {
        setcolor(RED);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, (char*)"Error: Se necesita un numero par de datos");
        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 40, (char*)"La primera mitad = X, Segunda mitad = Y");
        delay(3000);
        return;
    }
    
    clear_work_area();
    
    // Definir área para el gráfico
    int margin = 60;
    int x0 = margin;
    int y0 = WINDOW_HEIGHT - margin;
    int width = WINDOW_WIDTH - 2 * margin;
    int height = WINDOW_HEIGHT - 2 * margin - 40;
    
    // Dividir los datos en valores X e Y
    int pair_count = data->count / 2;
    double x_values[MAX_DATA_POINTS/2];
    double y_values[MAX_DATA_POINTS/2];
    
    for (int i = 0; i < pair_count; i++) {
        x_values[i] = data->data[i];              // Primera mitad = X
        y_values[i] = data->data[i + pair_count]; // Segunda mitad = Y
    }
    
    // Calcular estadísticas para regresión lineal
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    double min_x = x_values[0], max_x = x_values[0];
    double min_y = y_values[0], max_y = y_values[0];
    
    for (int i = 0; i < pair_count; i++) {
        double x = x_values[i];
        double y = y_values[i];
        
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x2 += x * x;
        
        min_x = (x < min_x) ? x : min_x;
        max_x = (x > max_x) ? x : max_x;
        min_y = (y < min_y) ? y : min_y;
        max_y = (y > max_y) ? y : max_y;
    }
    
    // Ajustar el rango para mejor visualización
    double range_x = max_x - min_x;
    double range_y = max_y - min_y;
    
    min_x -= range_x * 0.05;
    max_x += range_x * 0.05;
    min_y -= range_y * 0.1;
    max_y += range_y * 0.1;
    
    // Calcular coeficientes de regresión lineal
    double n = pair_count;
    double mean_x = sum_x / n;
    double mean_y = sum_y / n;
    
    double sxx = sum_x2 - (sum_x * sum_x) / n;
    double sxy = sum_xy - (sum_x * sum_y) / n;
    
    double b = sxy / sxx;
    double a = mean_y - b * mean_x;
    
    // Dibujar ejes y cuadrícula
    char title[50];
    sprintf(title, "Regresion Lineal (n=%d)", pair_count);
    draw_axes(x0, y0, width, height, min_x, max_x, min_y, max_y, 
              (char*)"X", (char*)"Y", title);
    
    // Dibujar puntos de datos
    setcolor(APP_COLOR_DATA_POINT);
    setfillstyle(SOLID_FILL, APP_COLOR_DATA_POINT);
    
    for (int i = 0; i < pair_count; i++) {
        double x = x_values[i];
        double y = y_values[i];
        
        // Convertir coordenadas de datos a coordenadas de pantalla
        int screen_x = x0 + ((x - min_x) * width) / (max_x - min_x);
        int screen_y = y0 - ((y - min_y) * height) / (max_y - min_y);
        
        // Dibujar punto
        fillellipse(screen_x, screen_y, 4, 4); // Puntos más grandes
    }
    
    // Dibujar línea de regresión
    setlinestyle(SOLID_LINE, 0, 3);
    setcolor(BLUE);
    
    double x_start = min_x;
    double y_start = a + b * x_start;
    double x_end = max_x;
    double y_end = a + b * x_end;
    
    int start_x = x0 + ((x_start - min_x) * width) / (max_x - min_x);
    int start_y = y0 - ((y_start - min_y) * height) / (max_y - min_y);
    int end_x = x0 + ((x_end - min_x) * width) / (max_x - min_x);
    int end_y = y0 - ((y_end - min_y) * height) / (max_y - min_y);
    
    line(start_x, start_y, end_x, end_y);
    
    setlinestyle(SOLID_LINE, 0, 1);
    
    // Mostrar ecuación de regresión y estadísticas
    char equation[100];
    sprintf(equation, "Ecuacion: y = %.4f + %.4fx", a, b);
    
    char r_squared[50];
    // Calcular R²
    double sum_residual_squared = 0;
    double sum_total_squared = 0;
    
    for (int i = 0; i < pair_count; i++) {
        double x = x_values[i];
        double y = y_values[i];
        double y_pred = a + b * x;
        
        sum_residual_squared += (y - y_pred) * (y - y_pred);
        sum_total_squared += (y - mean_y) * (y - mean_y);
    }
    
    double r2 = 1 - (sum_residual_squared / sum_total_squared);
    
    // Dibujar cuadro para la información
    setfillstyle(SOLID_FILL, WHITE);
    bar(x0 + 10, y0 - height + 10, x0 + 280, y0 - height + 80);
    setcolor(BLUE);
    rectangle(x0 + 10, y0 - height + 10, x0 + 280, y0 - height + 80);
    
    // Texto más grande para la ecuación
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1); // Texto más grande
    settextjustify(LEFT_TEXT, TOP_TEXT);
    setcolor(BLUE);
    outtextxy(x0 + 20, y0 - height + 20, equation);
    outtextxy(x0 + 20, y0 - height + 45, r_squared);
    
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}

