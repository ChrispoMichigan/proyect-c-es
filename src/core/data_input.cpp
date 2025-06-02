#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <conio.h>
#include "../../include/excel_c.h"

// Incluir los headers necesarios para el diálogo de Windows
#include <windows.h>
#include <commdlg.h>

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
        
        // También permitir hacer clic para continuar
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

// Versión mejorada con el diálogo nativo de Windows
int file_dialog(char* filename) {
    OPENFILENAME ofn;                // Estructura para el diálogo de Windows
    char szFile[MAX_FILENAME_LEN] = "";   // Buffer para el nombre de archivo inicializado vacío
    
    // Inicializar la estructura OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetForegroundWindow(); // Ventana actual como propietaria
    ofn.lpstrFile = szFile;                // Buffer donde se guarda el nombre de archivo
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Archivos de texto\0*.txt\0Todos los archivos\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    // Mostrar el diálogo de selección de archivo
    BOOL result = GetOpenFileName(&ofn);
    
    if (result) {
        // Copiar el nombre de archivo seleccionado (incluida la ruta)
        strcpy(filename, ofn.lpstrFile);
        return 1; // Éxito
    } else {
        DWORD error = CommDlgExtendedError();
        if (error) {
        } else {
        }
        return 0; // Cancelado o error
    }
}

// Función para cargar datos desde un archivo con diagnóstico ampliado
void load_data_from_file() {
    char filename[MAX_FILENAME_LEN];
    FILE *file;
    
    // Mostrar diálogo nativo de selección de archivo
    if (!file_dialog(filename)) {
        return; // Usuario canceló
    }
    
    // Restaurar el contexto gráfico después del diálogo
    clear_work_area();
    
    // Mostrar información de carga
    setcolor(APP_COLOR_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, (char*)"Cargando datos...");

    
    // Usar el modo "rb" (binary read) para evitar problemas de codificación
    file = fopen(filename, "rb");
    if (!file) {
        clear_work_area();
        setcolor(RED);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 20, (char*)"Error: No se pudo abrir el archivo");
        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 20, (char*)filename);
        settextjustify(LEFT_TEXT, TOP_TEXT); // Restaurar justificación de texto
        delay(2000);
        return;
    }
    
    // Leer y mostrar el contenido bruto del archivo para diagnóstico
    char buffer[1024];
    int bytes_read;
    int total_bytes = 0;
    
    // Leer hasta 1024 bytes del archivo
    while ((bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminar
        total_bytes += bytes_read;
    }
    
    // Volver al inicio del archivo
    rewind(file);
    
    // Limpiar conjunto de datos actual
    current_data.count = 0;
    strcpy(current_data.source, filename);
    
    // Leer el archivo línea por línea para mejor diagnóstico
    char line[1024];
    int line_number = 0;
    
    // Intentar otra técnica de lectura: leer todo el archivo en memoria
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    
    char* file_contents = (char*)malloc(file_size + 1);
    if (!file_contents) {
        fclose(file);
        return;
    }
    
    fread(file_contents, 1, file_size, file);
    file_contents[file_size] = '\0';
    
    // Ahora procesamos los números utilizando strtod para mayor robustez
    char* end_ptr;
    double value;
    char* current_ptr = file_contents;
    
    // Omitir espacios iniciales
    while (*current_ptr && isspace((unsigned char)*current_ptr)) {
        current_ptr++;
    }
    
    while (*current_ptr) {
        // Intentar convertir a número
        value = strtod(current_ptr, &end_ptr);
        
        if (current_ptr == end_ptr) {
            // No se pudo convertir, avanzar hasta el siguiente carácter
            current_ptr++;
            continue;
        }
        
        // Añadir valor al conjunto de datos
        if (current_data.count < MAX_DATA_POINTS) {
            current_data.data[current_data.count++] = value;
        } else {
            break;
        }
        
        // Avanzar al siguiente token
        current_ptr = end_ptr;
        
        // Omitir espacios
        while (*current_ptr && isspace((unsigned char)*current_ptr)) {
            current_ptr++;
        }
    }
    
    free(file_contents);
    fclose(file);
    
    // Si no se leyó ningún dato, mostrar error
    if (current_data.count == 0) {
        clear_work_area();
        setcolor(RED);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 20, 
                 (char*)"Error: No se encontraron datos numericos validos");
        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 20, (char*)filename);
        settextjustify(LEFT_TEXT, TOP_TEXT);
        delay(2000);
        return;
    }
    
    current_data.is_loaded = 1;
    
    // Mostrar un mensaje de éxito
    clear_work_area();
    setcolor(GREEN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    char message[100];
    sprintf(message, "Se cargaron %d datos correctamente", current_data.count);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, message);
    delay(1500);
    
    // Mostrar datos en el grid
    displayDataGrid();
}

// Función para mostrar los datos cargados en una cuadrícula paginada
void displayDataGrid() {
    clear_work_area();
    
    // Constantes para el grid
    const int GRID_ROWS = 10;
    const int GRID_COLS = 10;
    const int CELL_WIDTH = 55;
    const int CELL_HEIGHT = 25;
    const int GRID_START_X = 50;
    const int GRID_START_Y = 150;
    int items_per_page = GRID_ROWS * GRID_COLS;
    int current_page = 0;
    int total_pages = (current_data.count - 1) / items_per_page + 1;
    
    // Título con información del archivo
    setcolor(APP_COLOR_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    char title[100];
    sprintf(title, "Datos cargados de: %s", current_data.source);
    outtextxy(270, 60, title);
    
    // Dibujar la cuadrícula inicial
    drawDataGrid(GRID_START_X, GRID_START_Y, GRID_ROWS, GRID_COLS, CELL_WIDTH, CELL_HEIGHT);
    
    // Mostrar datos en la cuadrícula
    displayDataInGrid(GRID_START_X, GRID_START_Y, GRID_ROWS, GRID_COLS, 
                     CELL_WIDTH, CELL_HEIGHT, current_page);
    
    // Dibujar controles de navegación si hay más de una página
    if (current_data.count > items_per_page) {
        drawNavigationControls(current_page, total_pages,
                              GRID_START_X, GRID_START_Y, CELL_HEIGHT);
    }
    
    // Mensaje para volver al menú principal
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(BLUE);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT - 20, (char*)"Presione ENTER para volver al menu");
    settextjustify(LEFT_TEXT, TOP_TEXT); // Restaurar justificación
    
    // Esperar entrada para continuar
    bool done = false;
    
    while (!done) {
        // Verificar teclas
        if (kbhit()) {
            char key = getch();
            if (key == 13) { // ENTER
                done = true;
            }
        }
        
        // Verificar clics para navegación entre páginas
        if (ismouseclick(WM_LBUTTONDOWN) && current_data.count > items_per_page) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            int navY = GRID_START_Y + GRID_ROWS * CELL_HEIGHT + 10;
            
            // Botón "Anterior"
            if (x >= GRID_START_X && x <= GRID_START_X + 100 &&
                y >= navY && y <= navY + 30) {
                
                if (current_page > 0) {
                    current_page--;
                    displayDataInGrid(GRID_START_X, GRID_START_Y, GRID_ROWS, GRID_COLS, 
                                    CELL_WIDTH, CELL_HEIGHT, current_page);
                    drawNavigationControls(current_page, total_pages,
                                          GRID_START_X, GRID_START_Y, CELL_HEIGHT);
                }
            }
            
            // Botón "Siguiente"
            if (x >= GRID_START_X + 350 && x <= GRID_START_X + 450 &&
                y >= navY && y <= navY + 30) {
                
                if (current_page < total_pages - 1) {
                    current_page++;
                    displayDataInGrid(GRID_START_X, GRID_START_Y, GRID_ROWS, GRID_COLS, 
                                    CELL_WIDTH, CELL_HEIGHT, current_page);
                    drawNavigationControls(current_page, total_pages,
                                          GRID_START_X, GRID_START_Y, CELL_HEIGHT);
                }
            }
            
        }
        
        delay(50);
    }
    
    clear_work_area();
    draw_menu();
}