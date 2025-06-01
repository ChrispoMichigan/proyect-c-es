#include <graphics.h>
#include <stdio.h>
#include "../../include/excel_c.h"

// Variables globales 
DataSet current_data = { {0}, 0, "", 0 };
GraphType current_graph = GRAPH_NONE;

// Inicialización de la aplicación
void app_init() {
    // Establecer colores y estilos predeterminados
    setbkcolor(APP_COLOR_BACKGROUND);
    cleardevice();
    
    // Dibujar menú inicial
    draw_menu();
    
    // Mensaje de bienvenida
    setcolor(APP_COLOR_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 50, (char*)"Sistema de Visualizacion Estadistica");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 50, (char*)"Seleccione una opcion del menu para comenzar");
}

// Limpieza antes de cerrar la aplicación
void app_cleanup() {
    // Por ahora no hay nada que limpiar específicamente
}

// Bucle principal de la aplicación
void main_loop() {
    int running = 1;
    
    while (running) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            // Manejar clics en el menú
            int result = handle_menu_click(x, y);
            
            if (result == -1) {
                running = 0; // Código para salir
            }
            
            // Redibujar menú después de las acciones
            draw_menu();
        }
        
        delay(100); // Pequeña pausa para evitar uso excesivo de CPU
    }
}