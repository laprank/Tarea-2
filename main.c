#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char title[100];
  List *genres;
  char director[300];
  float rating;
  int year;
  int Decada;
} Film;
// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por id");
  puts("3) Buscar por director");
  puts("4) Buscar por género");
  puts("5) Buscar por década");
  puts("6) Buscar por rango de calificaciones");
  puts("7) Buscar por década y género");
  puts("8) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}
// Función para aproximar un año a la década más cercana siempre hacia abajo
int aproximar_a_decada_abajo(int year) {
    return (year / 10) * 10; // Dividir entre 10, truncar y luego multiplicar de nuevo por 10
}
int convertir_cadena_a_anio(char *cadena) {
    // Obtener la longitud de la cadena
    int longitud = strlen(cadena);

    // Verificar si la cadena tiene al menos tres caracteres y termina con 's'
    if (longitud >= 3 && cadena[longitud - 1] == 's') {
        // Crear una subcadena para extraer los dígitos del año
        char anio[5]; // máximo de 4 dígitos para el año + 1 para el carácter nulo
        strncpy(anio, cadena, longitud - 1); // Copiar todos los caracteres excepto el último 's'
        anio[longitud - 1] = '\0'; // Agregar el carácter nulo al final

        // Convertir la subcadena a un entero y devolverlo
        return atoi(anio);
    } else {
        // Si la cadena no tiene el formato esperado, devolver -1 (indicando un error)
        return -1;
    }
}
void cargar_peliculas2(Map *pelis_byid) {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // Crea una nueva estructura Film y almacena los datos de cada película
    Film *peli = (Film *)malloc(sizeof(Film));
    strcpy(peli->id, campos[1]);        // Asigna ID
    strcpy(peli->title, campos[5]);     // Asigna título
    strcpy(peli->director, campos[14]); // Asigna director
    peli->genres = list_create();       // Inicializa la lista de géneros
    peli->year =
        atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero
    peli-> rating = atof(campos[8]);
    char *token = strtok(campos[11], ",");
    while (token != NULL){
      list_pushBack(peli->genres, strdup(token));
      token = strtok(NULL, ",");
    }


    while (peli){

    }

  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas
}

/**
 * Carga películas desde un archivo CSV y las almacena en un mapa por ID.
 */
void cargar_peliculas(Map *pelis_byid) {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // Crea una nueva estructura Film y almacena los datos de cada película
    Film *peli = (Film *)malloc(sizeof(Film));
    strcpy(peli->id, campos[1]);        // Asigna ID
    strcpy(peli->title, campos[5]);     // Asigna título
    strcpy(peli->director, campos[14]); // Asigna director
    peli->genres = list_create();       // Inicializa la lista de géneros
    peli->year =
        atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero
    peli-> rating = atof(campos[8]);
    char *token = strtok(campos[11], ",");
    while (token != NULL){
      list_pushBack(peli->genres, strdup(token));
      token = strtok(NULL, ",");
    }


    // Inserta la película en el mapa usando el ID como clave
    map_insert(pelis_byid, peli->id, peli);
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas

  // Itera sobre el mapa para mostrar las películas cargadas
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    printf("ID: %s, Título: %s, Director: %s, Año: %d\n", peli->id, peli->title,
           peli->director, peli->year);
    peli->genres->current = peli->genres->head;
    while (peli->genres->current != NULL){
      printf("Género: %s\n", (char *)peli->genres->current->data);
      peli->genres->current = peli->genres->current->next;
    }
    pair = map_next(pelis_byid); // Avanza al siguiente par en el mapa
  }
}

/**
 * Busca y muestra la información de una película por su ID en un mapa.
 */
void buscar_por_id(Map *pelis_byid) {
  char id[10]; // Buffer para almacenar el ID de la película

  // Solicita al usuario el ID de la película
  printf("Ingrese el id de la película: ");
  scanf("%s", id); // Lee el ID del teclado
  printf("ID ingresado: %s\n", id);

  // Busca el par clave-valor en el mapa usando el ID proporcionado
  MapPair *pair = map_search(pelis_byid, id);

  // Si se encontró el par clave-valor, se extrae y muestra la información de la
  // película
  if (pair != NULL) {
    Film *peli =
        pair->value; // Obtiene el puntero a la estructura de la película
    // Muestra el título y el año de la película
    printf("Título: %s, Año: %d \n", peli->title, peli->year);
    // Muestra los géneros de la película
    peli->genres->current = peli->genres->head;
    while (peli->genres->current != NULL){
      printf("Género: %s\n", (char *)peli->genres->current->data);
      peli->genres->current = peli->genres->current->next;
    }
    // Muestra el director de la película
    printf("Director: %s\n", peli->director);
    // Muestra la calificación de la película
    printf("Calificación: %.2f\n", peli->rating);
  } else {
    // Si no se encuentra la película, informa al usuario
    printf("La película con id %s no existe\n", id);
  }
}

int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)

  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *pelis_byid = map_create(is_equal_str);
  Map *pelis_bygenre = map_create(is_equal_str);
  Map *pelis_byyear = map_create(is_equal_int);

  // Recuerda usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(pelis_byid);
      break;
    case '2':
      buscar_por_id(pelis_byid);
      break;
    case '3':
      break;
    case '4':
      {
        char genero[100];
        printf("Ingrese el género: ");
        scanf("%s", genero);
        FILE *archivo = fopen("data/Top1500.csv", "r");
        if (archivo == NULL) {
          perror(
              "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
        }

        char **campos;
        // Leer y parsear una línea del archivo CSV. La función devuelve un array de
        // strings, donde cada elemento representa un campo de la línea CSV procesada.
        campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

        // Lee cada línea del archivo CSV hasta el final
        while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
          // Crea una nueva estructura Film y almacena los datos de cada película
          Film *peli = (Film *)malloc(sizeof(Film));
          strcpy(peli->id, campos[1]);        // Asigna ID
          strcpy(peli->title, campos[5]);     // Asigna título
          strcpy(peli->director, campos[14]); // Asigna director
          peli->genres = list_create();       // Inicializa la lista de géneros
          peli->year =
              atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero
          peli-> rating = atof(campos[8]);
          char *token = strtok(campos[11], ",");
          while (token != NULL){
            list_pushBack(peli->genres, strdup(token));
            token = strtok(NULL, ",");
          }
          peli->genres->current = peli->genres->head;
          while(peli->genres->current != NULL){
            if(map_search(pelis_bygenre, peli->genres->current->data) == NULL){
                List *new_genero = list_create();
                list_pushBack(new_genero, peli);
                map_insert(pelis_bygenre, peli->genres->current->data, new_genero);
            }
            else{
              MapPair *genero_existente = map_search(pelis_bygenre, peli->genres->current->data);
              List *genero_list = genero_existente->value;
              list_pushBack(genero_list, peli);
            }
            peli->genres->current = peli->genres->current->next;
          }
          peli->genres->current = peli->genres->head;
        }
        MapPair *genero_pair = map_search(pelis_bygenre, genero);
        if (genero_pair != NULL){
          List *genero_list = genero_pair->value;
          genero_list->current = genero_list->head;
          while (genero_list->current != NULL){
            Film *peli = (Film *)genero_list->current->data;
            printf("ID: %s, Título: %s, Director: %s, Año: %d\n", peli->id, peli->title, peli->director, peli->year);
            genero_list->current = genero_list->current->next;
          }
          
        }
        else{
          printf("No se encontraron películas con ese género\n");
        }
        fclose(archivo);
      }
      break;
    case '5':
      {
        char decada[10];
        int decadanum;
        printf("Ingrese la década (ej:1980s): ");
        scanf("%s", decada);
        decadanum =convertir_cadena_a_anio(decada);
        FILE *archivo = fopen("data/Top1500.csv", "r");
        if (archivo == NULL) {
          perror(
              "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
        }

        char **campos;
        // Leer y parsear una línea del archivo CSV. La función devuelve un array de
        // strings, donde cada elemento representa un campo de la línea CSV procesada.
        campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

        // Lee cada línea del archivo CSV hasta el final
        while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
          // Crea una nueva estructura Film y almacena los datos de cada película
          Film *peli = (Film *)malloc(sizeof(Film));
          strcpy(peli->id, campos[1]);        // Asigna ID
          strcpy(peli->title, campos[5]);     // Asigna título
          strcpy(peli->director, campos[14]); // Asigna director
          peli->genres = list_create();       // Inicializa la lista de géneros
          peli->year = atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero
          peli-> rating = atof(campos[8]);
          char *token = strtok(campos[11], ",");
          while (token != NULL){
            list_pushBack(peli->genres, strdup(token));
            token = strtok(NULL, ",");
          }
          peli->Decada = aproximar_a_decada_abajo(peli->year);
          if(map_search(pelis_byyear, &(peli->Decada)) == NULL){
            List *new_decada = list_create();
            list_pushBack(new_decada, peli);
            map_insert(pelis_byyear, &(peli->Decada), new_decada);
          }
          else{
            MapPair *decada_existente = map_search(pelis_byyear, &(peli->Decada));
            List *decada_list = decada_existente->value;
            list_pushBack(decada_list, peli);
          }
        }
        MapPair *decada_pair = map_search(pelis_byyear, &decadanum);
        if (decada_pair != NULL){
          List *decada_list = decada_pair->value;
          decada_list->current = decada_list->head;
          while (decada_list->current != NULL){
            Film *peli = (Film *)decada_list->current->data;
            printf("ID: %s, Título: %s, Director: %s, Año: %d\n", peli->id, peli->title, peli->director, peli->year);
            decada_list->current = decada_list->current->next;
          }
        }
      }
      break;
    case '6':
      break;
    case '7':
      break;
    default:
      {
        printf("Opción inválida. Intente nuevamente.\n");
      }
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}