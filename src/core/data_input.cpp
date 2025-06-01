#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <conio.h>
#include "../../include/excel_c.h"

// Función para input manual de datos
void input_data_manually() {
    int x, y, i = 0;
    char input[50] = "";
    char temp[2] = "";
    double value;
    
    clear_work_area();
    
    setcolor(APP_COLOR_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(100, 100, (char*)"Ingrese los datos (un valor por linea)");
    outtextxy(100, 130, (char*)"Presione Enter despues de cada valor");
    outtextxy(100, 160, (char*)"Presione ESC para finalizar");
    
    setfillstyle(SOLID_FILL, WHITE);
    bar(100, 200, 400, 230);
    rectangle(100, 200, 400, 230);
    
    current_data.count = 0;
    strcpy(current_data.source, "Entrada Manual");
    
    x = 110;
    y = 215;
    
    while (1) {
        if (kbhit()) {
            char key = getch();
            
            if (key == 27) { // ESC
                break;
            } else if (key == 13) { // Enter
                if (strlen(input) > 0) {
                    value = atof(input);
                    current_data.data[current_data.count++] = value;
                    
                    // Limpiar área de entrada
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(100, 200, 400, 230);
                    rectangle(100, 200, 400, 230);
                    x = 110;
                    
                    // Mostrar el valor ingresado en la lista
                    char valueStr[50];
                    sprintf(valueStr, "%.2f", value);
                    outtextxy(450, 200 + (current_data.count - 1) * 25, valueStr);
                    
                    // Limpiar input
                    input[0] = '\0';
                    i = 0;
                }
            } else if (key == 8) { // Backspace
                if (i > 0) {
                    i--;
                    input[i] = '\0';
                    
                    // Actualizar visualización
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(100, 200, 400, 230);
                    rectangle(100, 200, 400, 230);
                    x = 110;
                    outtextxy(x, y, input);
                }
            } else if ((key >= '0' && key <= '9') || key == '.' || key == '-') {
                temp[0] = key;
                temp[1] = '\0';
                strcat(input, temp);
                outtextxy(x, y, temp);
                x += textwidth(temp);
                i++;
            }
        }
    }
    
    current_data.is_loaded = 1;
    
    // Mostrar mensaje con el número de datos ingresados
    char message[100];
    sprintf(message, "Se han ingresado %d datos correctamente", current_data.count);
    
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(100, 500, 500, 530);
    setcolor(APP_COLOR_TEXT);
    outtextxy(110, 510, message);
    
    // Esperar un clic para continuar
    outtextxy(100, 540, (char*)"Clic para continuar...");
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
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