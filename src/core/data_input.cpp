#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <conio.h>
#include "../../include/excel_c.h"

// Función para input manual de datos con visualización en grid
void input_data_manually() {
    int x, y, i = 0;
    char input[50] = "";
    char temp[2] = "";
    double value;
    int current_page = 0;
    
    // Constantes para el grid - definidas dentro de la función pero accesibles para las subfunciones
    const int GRID_ROWS = 10;
    const int GRID_COLS = 10;
    const int CELL_WIDTH = 55;
    const int CELL_HEIGHT = 25;
    const int GRID_START_X = 50;
    const int GRID_START_Y = 200;
    int items_per_page = GRID_ROWS * GRID_COLS;
    
    clear_work_area();
    
    // Título y instrucciones
    setcolor(APP_COLOR_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(50, 60, (char*)"Ingrese los datos (un valor por linea)");
    outtextxy(50, 85, (char*)"Presione Enter despues de cada valor");
    outtextxy(50, 110, (char*)"Presione ESC para finalizar");
    
    // Cuadro de entrada
    setfillstyle(SOLID_FILL, WHITE);
    bar(100, 150, 400, 180);
    rectangle(100, 150, 400, 180);
    
    // Inicializar datos
    current_data.count = 0;
    strcpy(current_data.source, "Entrada Manual");
    
    // Posición inicial del cursor
    x = 110;
    y = 165;
    
    // Dibujar líneas de la cuadrícula inicial
    drawDataGrid(GRID_START_X, GRID_START_Y, GRID_ROWS, GRID_COLS, CELL_WIDTH, CELL_HEIGHT);
    
    // Bucle principal de entrada de datos
    while (1) {
        if (kbhit()) {
            char key = getch();
            
            if (key == 27) { // ESC - Finalizar entrada
                break;
            } else if (key == 13) { // Enter - Procesar valor
                if (strlen(input) > 0) {
                    value = atof(input);
                    current_data.data[current_data.count++] = value;
                    
                    // Calcular página actual basado en la cantidad de datos
                    current_page = (current_data.count - 1) / items_per_page;
                    
                    // Limpiar área de entrada
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(100, 150, 400, 180);
                    rectangle(100, 150, 400, 180);
                    x = 110;
                    
                    // Mostrar datos en el grid
                    displayDataInGrid(GRID_START_X, GRID_START_Y, GRID_ROWS, GRID_COLS, 
                                     CELL_WIDTH, CELL_HEIGHT, current_page);
                    
                    // Dibujar controles de navegación si hay más de una página
                    if ((current_data.count > items_per_page)) {
                        drawNavigationControls(current_page, 
                                              (current_data.count - 1) / items_per_page + 1,
                                              GRID_START_X, GRID_START_Y, CELL_HEIGHT);
                    }
                    
                    // Limpiar input para el próximo valor
                    input[0] = '\0';
                    i = 0;
                }
            } else if (key == 8) { // Backspace
                if (i > 0) {
                    i--;
                    input[i] = '\0';
                    
                    // Actualizar visualización
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(100, 150, 400, 180);
                    rectangle(100, 150, 400, 180);
                    x = 110;
                    outtextxy(x, y, input);
                }
            } else if ((key >= '0' && key <= '9') || key == '.' || key == '-') {
                // Solo permitir caracteres válidos para números
                temp[0] = key;
                temp[1] = '\0';
                strcat(input, temp);
                outtextxy(x, y, temp);
                x += textwidth(temp);
                i++;
            }
        }
        
        // Verificar clicks para navegación entre páginas
        if (ismouseclick(WM_LBUTTONDOWN) && current_data.count > items_per_page) {
            int clickX, clickY;
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            
            int navY = GRID_START_Y + GRID_ROWS * CELL_HEIGHT + 10;
            
            // Botón "Anterior"
            if (clickX >= GRID_START_X && clickX <= GRID_START_X + 100 &&
                clickY >= navY && clickY <= navY + 30) {
                
                if (current_page > 0) {
                    current_page--;
                    displayDataInGrid(GRID_START_X, GRID_START_Y, GRID_ROWS, GRID_COLS, 
                                    CELL_WIDTH, CELL_HEIGHT, current_page);
                    drawNavigationControls(current_page, 
                                          (current_data.count - 1) / items_per_page + 1,
                                          GRID_START_X, GRID_START_Y, CELL_HEIGHT);
                }
            }
            
            // Botón "Siguiente"
            if (clickX >= GRID_START_X + 350 && clickX <= GRID_START_X + 450 &&
                clickY >= navY && clickY <= navY + 30) {
                
                if (current_page < (current_data.count - 1) / items_per_page) {
                    current_page++;
                    displayDataInGrid(GRID_START_X, GRID_START_Y, GRID_ROWS, GRID_COLS, 
                                    CELL_WIDTH, CELL_HEIGHT, current_page);
                    drawNavigationControls(current_page, 
                                          (current_data.count - 1) / items_per_page + 1,
                                          GRID_START_X, GRID_START_Y, CELL_HEIGHT);
                }
            }
        }
        
        delay(50);
    }
    
    // Marcar datos como cargados
    current_data.is_loaded = 1;
    
    // Mostrar mensaje de confirmación
    setfillstyle(SOLID_FILL, COLOR(200, 255, 200));
    bar(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 50, WINDOW_WIDTH/2 + 200, WINDOW_HEIGHT/2 + 50);
    rectangle(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 50, WINDOW_WIDTH/2 + 200, WINDOW_HEIGHT/2 + 50);
    
    setcolor(GREEN); // Cambiado DARKGREEN a GREEN que es un color estándar
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 20, (char*)"Datos ingresados correctamente");
    
    char message[100];
    sprintf(message, "Total: %d valores", current_data.count);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 10, message);
    
    setcolor(BLACK);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 35, (char*)"Presione ENTER para volver al menu");
    
    // Limpiar cualquier tecla pendiente en el buffer
    while (kbhit()) getch();
    
    // Esperar a que se presione ENTER
    bool waiting = true;
    while (waiting) {
        if (kbhit()) {
            char key = getch();
            if (key == 13) { // 13 es el código ASCII para ENTER
                waiting = false;
            }
        }
        
        // También permitir hacer clic para continuar (como alternativa)
        if (ismouseclick(WM_LBUTTONDOWN)) {
            clearmouseclick(WM_LBUTTONDOWN);
            waiting = false;
        }
        
        delay(50);
    }
    
    // Limpiar pantalla y redibujar el menú principal
    clear_work_area();
    draw_menu();
}

// Función auxiliar para dibujar la cuadrícula de datos
void drawDataGrid(int startX, int startY, int rows, int cols, int cellWidth, int cellHeight) {
    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    
    // Dibujar líneas horizontales
    for (int r = 0; r <= rows; r++) {
        line(startX, startY + r * cellHeight, 
             startX + cols * cellWidth, startY + r * cellHeight);
    }
    
    // Dibujar líneas verticales
    for (int c = 0; c <= cols; c++) {
        line(startX + c * cellWidth, startY, 
             startX + c * cellWidth, startY + rows * cellHeight);
    }
}

// Función auxiliar para mostrar datos en la cuadrícula
void displayDataInGrid(int startX, int startY, int rows, int cols, 
                       int cellWidth, int cellHeight, int page) {
    // Limpiar área de la cuadrícula
    setfillstyle(SOLID_FILL, WHITE);
    bar(startX + 1, startY + 1, 
        startX + cols * cellWidth - 1, startY + rows * cellHeight - 1);
    
    // Redibujar las líneas de la cuadrícula
    drawDataGrid(startX, startY, rows, cols, cellWidth, cellHeight);
    
    // Calcular elementos por página e índice inicial
    int itemsPerPage = rows * cols;
    int startIndex = page * itemsPerPage;
    int endIndex = startIndex + itemsPerPage;
    
    if (endIndex > current_data.count) {
        endIndex = current_data.count;
    }
    
    // Mostrar datos en la cuadrícula
    setcolor(APP_COLOR_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    
    for (int i = startIndex; i < endIndex; i++) {
        // Calcular posición en la cuadrícula
        int relativeIndex = i - startIndex;
        int row = relativeIndex / cols;
        int col = relativeIndex % cols;
        
        // Coordenadas centrales de la celda
        int cellX = startX + col * cellWidth + cellWidth/2;
        int cellY = startY + row * cellHeight + cellHeight/2;
        
        // Mostrar valor formateado
        char valueStr[20];
        sprintf(valueStr, "%.2f", current_data.data[i]);
        outtextxy(cellX, cellY, valueStr);
    }
    
    // Mostrar número de página y total de datos
    char pageInfo[100];
    sprintf(pageInfo, "Pagina %d de %d - Total: %d valores", 
            page + 1, (current_data.count - 1) / itemsPerPage + 1, current_data.count);
    
    settextjustify(CENTER_TEXT, TOP_TEXT);
    setcolor(BLUE);
    outtextxy(startX + cols * cellWidth / 2, startY - 20, pageInfo);
}

// Función auxiliar para dibujar los controles de navegación
// Modificada para aceptar las coordenadas del grid como parámetros
void drawNavigationControls(int currentPage, int totalPages, int gridStartX, int gridStartY, int cellHeight) {
    int navY = gridStartY + 10 * cellHeight + 10;
    
    // Botón "Anterior"
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(gridStartX, navY, gridStartX + 100, navY + 30);
    rectangle(gridStartX, navY, gridStartX + 100, navY + 30);
    
    setcolor(currentPage > 0 ? BLACK : DARKGRAY);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(gridStartX + 50, navY + 15, (char*)"< Anterior");
    
    // Botón "Siguiente"
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(gridStartX + 350, navY, gridStartX + 450, navY + 30);
    rectangle(gridStartX + 350, navY, gridStartX + 450, navY + 30);
    
    setcolor(currentPage < totalPages - 1 ? BLACK : DARKGRAY);
    outtextxy(gridStartX + 400, navY + 15, (char*)"Siguiente >");
    
    // Información de página
    char pageInfo[50];
    sprintf(pageInfo, "Pagina %d de %d", currentPage + 1, totalPages);
    
    setcolor(BLUE);
    outtextxy(gridStartX + 225, navY + 15, pageInfo);
}

// Diálogo para seleccionar archivo
int file_dialog(char* filename) {
    DIR *dir;
    struct dirent *ent;
    int numFiles = 0;
    char *files[100];
    char currentPath[MAX_FILENAME_LEN] = "."; // Comenzamos en el directorio actual
    int selected = -1;
    
    clear_work_area();
    
    setcolor(APP_COLOR_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(100, 60, (char*)"Seleccione el archivo de datos");
    
    // Obtener archivos del directorio actual
    if ((dir = opendir(currentPath)) != NULL) {
        // Dibujar área de lista de archivos
        setfillstyle(SOLID_FILL, WHITE);
        bar(100, 100, 600, 500);
        rectangle(100, 100, 600, 500);
        
        // Listar archivos
        while ((ent = readdir(dir)) != NULL && numFiles < 100) {
            // Solo mostrar archivos .txt
            char *ext = strrchr(ent->d_name, '.');
            if (ext && strcmp(ext, ".txt") == 0) {
                files[numFiles] = strdup(ent->d_name);
                outtextxy(120, 120 + numFiles * 30, files[numFiles]);
                numFiles++;
            }
        }
        closedir(dir);
        
        // Si no hay archivos .txt
        if (numFiles == 0) {
            outtextxy(150, 250, (char*)"No se encontraron archivos .txt");
            outtextxy(150, 280, (char*)"Coloque archivos .txt en el directorio actual");
        }
        
        // Botones
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(400, 520, 500, 550);
        rectangle(400, 520, 500, 550);
        outtextxy(420, 530, (char*)"Aceptar");
        
        bar(250, 520, 350, 550);
        rectangle(250, 520, 350, 550);
        outtextxy(270, 530, (char*)"Cancelar");
        
        // Esperar selección
        int done = 0;
        while (!done) {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                int x, y;
                getmouseclick(WM_LBUTTONDOWN, x, y);
                
                // Verificar si se hizo clic en un archivo
                if (x >= 100 && x <= 600 && y >= 100 && y <= 500) {
                    int fileIndex = (y - 120) / 30;
                    if (fileIndex >= 0 && fileIndex < numFiles) {
                        // Resaltar archivo seleccionado
                        setfillstyle(SOLID_FILL, LIGHTBLUE);
                        bar(101, 120 + fileIndex * 30 - 10, 599, 120 + fileIndex * 30 + 20);
                        setcolor(APP_COLOR_TEXT);
                        outtextxy(120, 120 + fileIndex * 30, files[fileIndex]);
                        selected = fileIndex;
                    }
                }
                // Verificar si se hizo clic en Aceptar
                else if (x >= 400 && x <= 500 && y >= 520 && y <= 550) {
                    if (selected >= 0) {
                        strcpy(filename, files[selected]);
                        done = 1;
                    }
                }
                // Verificar si se hizo clic en Cancelar
                else if (x >= 250 && x <= 350 && y >= 520 && y <= 550) {
                    done = 1;
                    selected = -1;
                }
            }
            delay(100);
        }
        
        // Liberar memoria
        for (int i = 0; i < numFiles; i++) {
            free(files[i]);
        }
    }
    
    return (selected >= 0);
}

// Cargar datos desde archivo
void load_data_from_file() {
    char filename[MAX_FILENAME_LEN];
    FILE *file;
    
    // Mostrar diálogo de selección de archivo
    if (!file_dialog(filename)) {
        return; // Usuario canceló
    }
    
    file = fopen(filename, "r");
    if (!file) {
        // Mostrar error
        clear_work_area();
        setcolor(RED);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(100, 100, (char*)"Error: No se pudo abrir el archivo");
        delay(2000);
        return;
    }
    
    // Limpiar conjunto de datos actual
    current_data.count = 0;
    strcpy(current_data.source, filename);
    
    // Leer datos del archivo
    double value;
    while (fscanf(file, "%lf", &value) == 1 && current_data.count < MAX_DATA_POINTS) {
        current_data.data[current_data.count++] = value;
    }
    
    fclose(file);
    
    current_data.is_loaded = 1;
    
    // Mostrar mensaje con el número de datos cargados
    clear_work_area();
    char message[100];
    sprintf(message, "Se cargaron %d datos desde %s", current_data.count, filename);
    
    setcolor(APP_COLOR_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(100, 100, message);
    
    // Mostrar algunos datos de ejemplo
    int showCount = current_data.count < 10 ? current_data.count : 10;
    for (int i = 0; i < showCount; i++) {
        char valueStr[50];
        sprintf(valueStr, "%.2f", current_data.data[i]);
        outtextxy(100, 150 + i * 30, valueStr);
    }
    
    if (current_data.count > 10) {
        outtextxy(100, 150 + 10 * 30, (char*)"...");
    }
    
    // Esperar un clic para continuar
    outtextxy(100, 450, (char*)"Clic para continuar...");
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
    
    clear_work_area();
    
    // Mostrar el grid de datos
    display_data_grid(&current_data);
}

// Mostrar datos en formato de tabla
void display_data_grid(DataSet* data) {
    if (data->count == 0 || !data->is_loaded) {
        return;
    }
    
    int cols = 5; // Número de columnas
    int rows = (data->count + cols - 1) / cols; // Número de filas necesarias
    
    // Dibujar título
    setcolor(APP_COLOR_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    char title[100];
    sprintf(title, "Datos (%s)", data->source);
    outtextxy(50, 50, title);
    
    // Dibujar cuadrícula
    int cell_width = 100;
    int cell_height = 30;
    int start_x = 50;
    int start_y = 100;
    
    // Dibujar encabezados
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    for (int i = 0; i < cols; i++) {
        bar(start_x + i * cell_width, start_y - cell_height, 
            start_x + (i+1) * cell_width, start_y);
        rectangle(start_x + i * cell_width, start_y - cell_height, 
                 start_x + (i+1) * cell_width, start_y);
                 
        char header[10];
        sprintf(header, "Col %d", i+1);
        outtextxy(start_x + i * cell_width + 10, start_y - cell_height + 10, header);
    }
    
    // Dibujar datos
    int data_index = 0;
    for (int row = 0; row < rows && data_index < data->count; row++) {
        for (int col = 0; col < cols && data_index < data->count; col++) {
            // Celda
            rectangle(start_x + col * cell_width, start_y + row * cell_height, 
                     start_x + (col+1) * cell_width, start_y + (row+1) * cell_height);
            
            // Valor
            char value_str[20];
            sprintf(value_str, "%.2f", data->data[data_index]);
            outtextxy(start_x + col * cell_width + 10, start_y + row * cell_height + 10, value_str);
            
            data_index++;
        }
    }
}