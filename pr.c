#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int leerEnteroPositivo(const char *mensaje) {
    int valor;
    int resultado;
    do {
        printf("%s", mensaje);
        resultado = scanf("%d", &valor);
        if (resultado != 1) {
            printf("\t[ERROR] Entrada invalida. Por favor, ingrese un numero entero.\n");
            limpiarBuffer();
        } else if (valor < 0) {
            printf("\t[ERROR] El valor no puede ser negativo. Intente de nuevo.\n");
        }
    } while (resultado != 1 || valor < 0);
    limpiarBuffer();
    return valor;
}

int main() {
    int idSistema = 0;
    char nombreSistema[50] = "No registrado";
    int maxIntentosPermitidos = 0;
    int maxIntentosSospechosos = 0;
    int sistemaRegistrado = 0; 

    int totalAccesos = 0;
    int accesosNormales = 0;
    int accesosSospechosos = 0;
    int accesosBloqueados = 0;

    char usuarioAcceso[50] = "Ninguno";
    int intentosRealizados = 0;
    int contrasenasCorrectas = 0;
    int contrasenasIncorrectas = 0;
    int puntajeRiesgoUltimo = 0;
    char nivelRiesgoUltimo[20] = "Sin registros";
    int ultimoAccesoRegistrado = 0;

    int opcion = 0;

    do {
        printf("\n==================================================\n");
        printf("\tSISTEMA DE MONITOREO DE ACCESOS\n");
        printf("==================================================\n");
        printf("1. Registrar sistema\n");
        printf("2. Registrar intento de acceso\n");
        printf("3. Consultar informacion\n");
        printf("4. Mostrar estadisticas\n");
        printf("5. Salir\n");
        printf("--------------------------------------------------\n");
        
        printf("Seleccione una opcion: ");
        if (scanf("%d", &opcion) != 1) {
            printf("\t[ERROR] Opcion invalida. Ingrese un numero entre 1 y 5.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();

        switch (opcion) {
            case 1:
                printf("\n--- REGISTRO DEL SISTEMA ---\n");
                idSistema = leerEnteroPositivo("Ingrese el ID del sistema: ");
                
                printf("Ingrese el Nombre del sistema: ");
                fgets(nombreSistema, sizeof(nombreSistema), stdin);
                nombreSistema[strcspn(nombreSistema, "\n")] = '\0'; 

                maxIntentosPermitidos = leerEnteroPositivo("Ingrese el numero maximo de intentos permitidos: ");
                maxIntentosSospechosos = leerEnteroPositivo("Ingrese el numero maximo de intentos sospechosos permitidos: ");

                sistemaRegistrado = 1;
                printf("\t[OK] Sistema registrado correctamente.\n");
                break;

            case 2:
                if (!sistemaRegistrado) {
                    printf("\t[ALERTA] Primero debe registrar los datos del sistema (Opcion 1).\n");
                    break;
                }

                printf("\n--- REGISTRO DE INTENTO DE ACCESO ---\n");
                printf("Ingrese el Nombre de usuario: ");
                fgets(usuarioAcceso, sizeof(usuarioAcceso), stdin);
                usuarioAcceso[strcspn(usuarioAcceso, "\n")] = '\0';

                intentosRealizados = leerEnteroPositivo("Ingrese el numero total de intentos realizados: ");

                contrasenasCorrectas = 0;
                contrasenasIncorrectas = 0;

                printf("\n--- Verificacion de contraseña por cada intento ---\n");
                for (int i = 1; i <= intentosRealizados; i++) {
                    int respuesta = -1;
                    do {
                        printf("Intento %d/%d - ¿La contraseña fue correcta? (1 = Si, 0 = No): ", i, intentosRealizados);
                        if (scanf("%d", &respuesta) != 1 || (respuesta != 0 && respuesta != 1)) {
                            printf("\t[ERROR] Entrada invalida. Ingrese 1 (Si) o 0 (No).\n");
                            limpiarBuffer();
                            respuesta = -1;
                        }
                    } while (respuesta != 0 && respuesta != 1);
                    limpiarBuffer();

                    if (respuesta == 1) {
                        contrasenasCorrectas++;
                    } else {
                        contrasenasIncorrectas++;
                    }
                }

                puntajeRiesgoUltimo = 0;
                if (contrasenasIncorrectas > 0) {
                    puntajeRiesgoUltimo += 30;
                }
                if (intentosRealizados > 5) {
                    puntajeRiesgoUltimo += 30;
                } else if (intentosRealizados > 3) {
                    puntajeRiesgoUltimo += 20;
                }
                if (strcmp(usuarioAcceso, "desconocido") == 0 || strcmp(usuarioAcceso, "unknown") == 0) {
                    puntajeRiesgoUltimo += 40;
                }

                if (puntajeRiesgoUltimo > 100) puntajeRiesgoUltimo = 100;

                if (puntajeRiesgoUltimo <= 30) {
                    strcpy(nivelRiesgoUltimo, "Bajo");
                } else if (puntajeRiesgoUltimo <= 70) {
                    strcpy(nivelRiesgoUltimo, "Medio");
                } else {
                    strcpy(nivelRiesgoUltimo, "Alto");
                }

                totalAccesos++;
                ultimoAccesoRegistrado = 1;

                printf("\n==================================================\n");
                printf("\tRESUMEN DEL ACCESO EVALUADO\n");
                printf("==================================================\n");
                printf("\tIntentos Totales:\t\t%d\n", intentosRealizados);
                printf("\tContraseñas Correctas:\t\t%d\n", contrasenasCorrectas);
                printf("\tContraseñas Incorrectas:\t%d\n", contrasenasIncorrectas);

                if (intentosRealizados > maxIntentosPermitidos) {
                    printf("\tEstado Final:\t\t\t[ACCESO BLOQUEADO]\n");
                    printf("\tMotivo:\t\t\t\tSupero el limite maximo de intentos permitidos (%d).\n", maxIntentosPermitidos);
                    accesosBloqueados++;
                } else if (contrasenasIncorrectas >= maxIntentosSospechosos || puntajeRiesgoUltimo > 30) {
                    printf("\tEstado Final:\t\t\t[ACCESO SOSPECHOSO]\n");
                    printf("\tMotivo:\t\t\t\tErrores en contraseña alcanzan/superan el limite sospechoso (%d).\n", maxIntentosSospechosos);
                    accesosSospechosos++;
                } else {
                    printf("\tEstado Final:\t\t\t[ACCESO NORMAL]\n");
                    accesosNormales++;
                }

                printf("\tNivel de Riesgo:\t\t%s (%d pts)\n", nivelRiesgoUltimo, puntajeRiesgoUltimo);
                break;

            case 3:
                if (!sistemaRegistrado) {
                    printf("\t[ALERTA] No hay un sistema registrado todavia.\n");
                    break;
                }

                printf("\n==================================================\n");
                printf("\tCONSULTA DE INFORMACION DEL SISTEMA\n");
                printf("==================================================\n");
                printf("\tID del Sistema:\t\t\t\t%d\n", idSistema);
                printf("\tNombre del Sistema:\t\t\t%s\n", nombreSistema);
                printf("--------------------------------------------------\n");
                printf("\tTotal de accesos registrados:\t\t%d\n", totalAccesos);
                printf("\t  - Accesos Normales:\t\t\t%d\n", accesosNormales);
                printf("\t  - Accesos Sospechosos:\t\t%d\n", accesosSospechosos);
                printf("\t  - Accesos Bloqueados:\t\t\t%d\n", accesosBloqueados);
                
                if (ultimoAccesoRegistrado) {
                    printf("\tRiesgo del ultimo acceso:\t\t%s (%d pts)\n", nivelRiesgoUltimo, puntajeRiesgoUltimo);
                } else {
                    printf("\tRiesgo del ultimo acceso:\t\tSin registros\n");
                }
                
                printf("--------------------------------------------------\n");
                printf("\tMax. intentos permitidos en sistema:\t%d\n", maxIntentosPermitidos);
                printf("\tMax. intentos sospechosos en sistema:\t%d\n", maxIntentosSospechosos);
                break;

            case 4:
                if (totalAccesos == 0) {
                    printf("\t[ALERTA] No hay accesos registrados para calcular estadisticas.\n");
                } else {
                    float pctNormales = ((float)accesosNormales / totalAccesos) * 100.0f;
                    float pctSospechosos = ((float)accesosSospechosos / totalAccesos) * 100.0f;
                    float pctBloqueados = ((float)accesosBloqueados / totalAccesos) * 100.0f;

                    printf("\n==================================================\n");
                    printf("\tESTADISTICAS DE ACCESO\n");
                    printf("==================================================\n");
                    printf("\tTotal de eventos procesados:\t%d\n", totalAccesos);
                    printf("\t%% Accesos Normales:\t\t%.2f%%\n", pctNormales);
                    printf("\t%% Accesos Sospechosos:\t\t%.2f%%\n", pctSospechosos);
                    printf("\t%% Accesos Bloqueados:\t\t%.2f%%\n", pctBloqueados);
                }
                break;

            case 5:
                printf("\nSaliendo del sistema de monitoreo... ¡Hasta luego!\n");
                break;

            default:
                printf("\t[ERROR] Opcion fuera de rango. Seleccione entre 1 y 5.\n");
                break;
        }

    } while (opcion != 5);

    return 0;
}