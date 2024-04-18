#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char marca[20];
    char modelo[10];
    char tipodevehiculo [25];
    int ano;
    float cilindraje;
    int cantidaddeacientos;
} vehiculo;

int main()
{
    int num_vehiculos;
    vehiculo *vehiculos;
    FILE *archivo;
    char nombre_archivo[30];

    printf("Ingrese el numero de vehiculos que desea almacenar: ");
    scanf("%d", &num_vehiculos);

    vehiculos = (vehiculo*)malloc(num_vehiculos * sizeof(vehiculo));
    if (vehiculos == NULL)
    {
        printf("No se pudo asignar memoria \n");
        return 1;
    }

    printf("Ingrese los detalles de cada vehiculo: \n");
    for (int i = 0; i < num_vehiculos; i++)
    {
        printf("vehiculo %d:\n", i + 1);
        printf("Ingrese la marca del vehiculo: ");
        scanf("%s", vehiculos[i].marca);
        printf("Ingrese el modelo: ");
        scanf("%s", vehiculos[i].modelo);
        printf("Ingrese el tipo de vehiculo: ");
        scanf("%s", vehiculos[i].tipodevehiculo);
        printf("Ingrese el año: ");
        scanf("%d", &vehiculos[i].ano);
        printf("Ingrese el cilidraje: ");
        scanf("%f", &vehiculos[i].cilindraje);
        printf("Ingrese el numero de acientos: ");
        scanf("%d", &vehiculos[i].cantidaddeacientos);
    }

    printf("Ingrese el nombre del archivo donde se guardarán los datos del vehiculo: ");
    scanf("%s", nombre_archivo);

    archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    for (int i = 0; i < num_vehiculos; i++)
    {
        fprintf(archivo, "vehiculo %d:\nmarca: %s\nmodelo: %s\ntipodevehiculo: %s\nano: %d\ncilindraje: %.2f\ncantidaddeacientos: %d\n\n", i + 1, vehiculos[i].marca, vehiculos[i].modelo, vehiculos[i].tipodevehiculo, vehiculos[i].ano, vehiculos[i].cilindraje, vehiculos[i].cantidaddeacientos);
    }

    fclose(archivo);

    free(vehiculos);

    printf("Los detalles de los vehiculos se han guardado exitosamente en el archivo %s.\n", nombre_archivo);

    return 0;
}
