#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/excel_c.h"

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char*)"");
    
    // Inicializar la aplicación
    app_init();
    
    // Ejecutar el bucle principal
    main_loop();
    
    // Limpieza y finalización
    app_cleanup();
    
    closegraph();
    return 0;
}