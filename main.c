/**
 * @file main.c
 * @author ElNube
 * @brief 
 * @version 0.1
 * @date 2022-06-04
 * 
 * @copyright Copyright (c) 2022
 * 
 * TODO(1):
    - Hacer una busqueda por el lugar de salida y llegada para COMPRAR PASAJE
 */

/* Librerias */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Estructuras */

typedef struct s_fecha{
    int minuto, hora, dia, mes, anio;
} FECHA;

typedef struct s_bus{
    int disponibilidad[40], id;
    char Salida[40], Llegada[40];
    FECHA fsalida, fllegada;
} BUS;

typedef struct s_boleto{
    int id, monto, pagado, vuelto, asiento;
    char numero[50], correo[50];
    FECHA fcompra;
    BUS bus;
} BOLETO;

typedef struct s_elemento{
	BUS bus;
	struct s_elemento *siguiente;
} *ELEMENTO;

typedef struct s_fila_enlazada{
	ELEMENTO elementos;
	struct s_elemento *frente, *ultimo;
} FILA;

/* Prototipos Generales */

int interfaz();
int aleatoreo(int, int);
void mostrarBoleto(BOLETO);
void mostrarBus(BUS);
void mostrarFecha(FECHA);
BOLETO crearBoleto(int, int, int, int, int, char*, char*, FECHA, BUS);
FECHA crearFecha(int, int, int, int, int);
BUS crearBus(int, int*, char*, char*, FECHA, FECHA);
FILA cargarArchivo(char*);
void guardarArchivo(FILA, char*);
FECHA obtenerFecha(void);

/* Prototipos FILA */

FILA crearFila();
int esVacia(FILA);
FILA agregar(FILA, BUS);
FILA extraer(FILA);
BUS verFrente(FILA);

/* Funcion principal */
int main(){
    srand(time(0));
    int n;

    /* Menu */
    do
    {
        n = interfaz();
    } while(n != 0);
    printf("QUE TENGA BUEN DIA\n");
    system("pause");
    return 0;
}

/**
 * @brief Mostrar la interfaz por consola y operaciones principales, tales como menus.
 * 
 * @return Entero 0 si es que se desea salir.
 */
int interfaz(){
    /**
     * @param n Numero de eleccion del menu
     * @param verdad Logica para usar en bucles 1 : true; 0 : false
     * 
     */
    int n, asiento, id, pagado, contadorBuses=0, valorPasaje = 3000, verdad = 0;
    char numero[50], correo[50];
    FILA x_f = crearFila();
    BOLETO x_boleto;
    do
    {
        printf("0)SALIR\n1)CONSULTAR BUSES\n2)COMPRAR PASAJE\n");
        scanf("%d", &n);
        switch (n)
        {
            case 1:
                x_f = cargarArchivo("misdatos.txt");
                while (esVacia(x_f) != 1)
                {
                    printf("ID: %d\n", x_f.frente->bus.id);
                    printf("DESDE: %s\t", x_f.frente->bus.Salida);
                    mostrarFecha(x_f.frente->bus.fsalida);
                    printf("HASTA: %s\t", x_f.frente->bus.Llegada);
                    mostrarFecha(x_f.frente->bus.fllegada);
                    contadorBuses++;
                    x_f = extraer(x_f);
                }
                printf("\n");
                /* Elegir bus para mostrar su disponibilidad */
                do
                {
                    printf("Identificador: ");
                    scanf("%d", &id);
                    if (id < 1 || id > contadorBuses)
                        printf("Ingrese un identificador valido.\n");
                } while (id < 1 || id > contadorBuses);
                x_f.frente = x_f.elementos;
                contadorBuses = 1;
                while (verdad == 0)
                {
                    if (id == contadorBuses)
                    {
                        verdad = 1;
                        mostrarBus(x_f.frente->bus);
                        break;
                    }
                    x_f = extraer(x_f);
                    contadorBuses++;
                }
                system("pause");
                break;
            case 2:
                x_f = cargarArchivo("misdatos.txt");
                
                /* Mostrar datos */
                while (esVacia(x_f) != 1)
                {
                    printf("ID: %d\n", x_f.frente->bus.id);
                    printf("DESDE: %s\t", x_f.frente->bus.Salida);
                    mostrarFecha(x_f.frente->bus.fsalida);
                    printf("HASTA: %s\t", x_f.frente->bus.Llegada);
                    mostrarFecha(x_f.frente->bus.fllegada);
                    contadorBuses++;
                    x_f = extraer(x_f);
                }

                /* Elegir bus */
                do
                {
                    printf("Identificador: ");
                    scanf("%d", &id);
                    if (id < 1 || id > contadorBuses)
                        printf("Ingrese un identificador valido.\n");
                } while (id < 1 || id > contadorBuses);
                x_f.frente = x_f.elementos;
                contadorBuses = 1;
                while (verdad == 0)
                {
                    if (id == contadorBuses)
                    {
                        verdad = 1;
                        break;
                    }
                    x_f = extraer(x_f);
                    contadorBuses++;
                }

                /* Elegir asiento */
                do
                {
                    printf("# Asiento : ");
                    scanf("%d", &asiento);
                    if (asiento < 0 || 1 == x_f.frente->bus.disponibilidad[asiento-1])
                        printf("Ya esta ocupado.\n");
                } while (asiento < 0 || 1 == x_f.frente->bus.disponibilidad[asiento-1]);
                x_f.frente->bus.disponibilidad[asiento-1] = 1;

                /* Pagar */
                printf("El pasaje vale $%d\n", valorPasaje);
                do
                {
                    printf("Paga con: $");
                    scanf("%d", &pagado);
                    if (pagado<0 || pagado < valorPasaje)
                        printf("Ingrese una cantidad valida.\n.");
                } while (pagado<0 || pagado < valorPasaje);
                
                /* Ingreso datos extras */
                printf("Numero de telefono : ");
                scanf("%49s", numero);
                printf("Correo electronico : ");
                scanf("%49s", correo);

                /* Crear boleta, mostrar datos por consola y guardar los cambios */
                x_boleto = crearBoleto(aleatoreo(1,100), valorPasaje, pagado, pagado-valorPasaje, asiento, numero, correo, obtenerFecha(), x_f.frente->bus);
                x_f.frente = x_f.elementos;
                guardarArchivo(x_f, "misdatos.txt");
                mostrarBoleto(x_boleto);
                system("pause");
                break;
        }
    } while (n!=0);
    return 0;
}

/**
 * @brief Numero seudo-aletoreo
 * 
 * @param min Numero minimo incluido
 * @param max Numero maximo incluido
 * @return int [min, max]
 */
int aleatoreo(int min, int max){
	//srand(time(NULL));
	return rand () % (max-min+1) + min;
}

/**
 * @brief Mostrar los datos de un boleto
 * 
 * @param b boleto
 */
void mostrarBoleto(BOLETO b){
    printf("----------------------------\n");
    printf("-----------BOLETO-----------\n");
    printf("----------------------------\n");
    printf("ID: %d\t", b.id);
    printf("MONTO: $%d\n", b.monto);
    printf("PAGADO: $%d\n", b.pagado);
    printf("VUELTO: $%d\n", b.vuelto);
    printf("DESDE: %s\t", b.bus.Salida);
    mostrarFecha(b.bus.fsalida);
    printf("HASTA: %s\t", b.bus.Llegada);
    mostrarFecha(b.bus.fllegada);
    printf("ASIENTO: %d\n", b.asiento);
    printf("NUMERO: %s\n", b.numero);
    printf("CORREO: %s\n", b.correo);
    printf("----------------------------\n");
}

/**
 * @brief Mostrar los datos de un bus
 * 
 * @param b Bus 
 */
void mostrarBus(BUS b){
    printf("----------------------------\n");
    printf("-------DISPONIBILIDAD-------\n");
    printf("X = ocupado   O = disponible\n");
    printf("----------------------------\n");
    printf("ID: %d\n", b.id);
    for (int i = 1; i <= 40; i++)
    {
        if (b.disponibilidad[i-1] == 0)
        {
            printf("%d.O ", i);
        }
        else
        {
            printf("%d.X ", i);
            
        }
        if (i % 2 == 0 && i % 4 != 0)
            printf("\t");
        if (i % 4 == 0)
            printf("\n");
    }
    printf("----------------------------\n");
    printf("Desde: %s\t", b.Salida);
    mostrarFecha(b.fsalida);

    printf("Hasta: %s\t", b.Llegada);
    mostrarFecha(b.fllegada);
}

/**
 * @brief Mostrar los datos de una fecha HH:MM DD/MM/AA
 * 
 * @param f Fecha
 */
void mostrarFecha(FECHA f){
    if (f.hora == 0)
        printf("00:");
    else
        printf("%d:",f.hora);
    if (f.minuto == 0)
        printf("00 ");
    else
        printf("%d ", f.minuto);
    if (f.dia <10)
        printf("0");
    printf("%d/", f.dia);
    if (f.mes <10)
        printf("0");
    printf("%d/", f.dia);
    printf("%d\n", f.anio);
}
/**
 * @brief 
 * 
 * @param id Identificador
 * @param m Monto
 * @param p Pagado por el cliente
 * @param v Vuelto
 * @param a Asiento
 * @param n Numero del cliente
 * @param c Correo del cliente
 * @param fc Fecha de compra
 * @param x_bus Bus en el cual viaja
 * @return BOLETO 
 */
BOLETO crearBoleto(int id, int m, int p, int v, int a, char *n, char *c, FECHA fc, BUS x_bus){
    BOLETO b;
    b.id = id;
    b.monto = m;
    b.pagado = p;
    b.vuelto = v;
    b.asiento = a;
    for (int i=0; i<50; i++)
    {
        b.numero[i] = n[i];
        b.correo[i] = c[i];
    }
    b.fcompra = fc;
    b.bus = x_bus;
    return b;
}

/**
 * @brief 
 * 
 * @param mi Minuto
 * @param h Hora
 * @param d Dia
 * @param me Mes
 * @param a Año
 * @return FECHA 
 */
FECHA crearFecha(int mi, int h, int d, int me, int a){
    FECHA f;
    f.minuto = mi;
    f.hora = h;
    f.dia = d;
    f.mes = me;
    f.anio = a;
    return f;
}

/**
 * @brief 
 * 
 * @param i identificador
 * @param d array de disponibilidad
 * @param ls Lugar salida
 * @param ll Lugar Llegada
 * @param fs Fecha salida
 * @param fl Fecha llegada
 * @return BUS 
 */
BUS crearBus(int i, int *d, char *ls, char *ll, FECHA fs, FECHA fl){
    BUS b;
    b.id = i;
    for (int i=0; i<40; i++){
        b.disponibilidad[i] = d[i];
        b.Salida[i] = ls[i];
        b.Llegada[i] = ll[i];
    }
    b.fsalida = fs;
    b.fllegada = fl;
    return b;
}

/**
 * @brief Leer los datos de un archivo
 * 
 * @param direccion del Archivo
 * @return FILA con los datos del archivo
 */
FILA cargarArchivo(char *direccion){
    int datoExtraido[11], dispo[40];
    char auxSalida[50], auxLlegada[50], aux[50];
    FILE* fichero;
    FILA x_fila = crearFila();
    BUS x_bus;
    FECHA x_fechas, x_fechal;
    
    //LECTURA
    
    fichero = fopen(direccion, "rt");
    for (int i =0; i < 2; i++)
    {
        fscanf (fichero, "%d", &datoExtraido[0]);
        for (int j = 1; j < 11; j++)
            fscanf (fichero, "%d", &datoExtraido[j]);
        fgets(aux, 50, fichero);
        fgets(auxSalida, 50, fichero);
        fgets(auxLlegada, 50, fichero);
        for (int j = 0; j < 40; j++)
            fscanf (fichero, "%d", &dispo[j]);
        x_fechas = crearFecha(datoExtraido[1], datoExtraido[2], datoExtraido[3], datoExtraido[4], datoExtraido[5]);
        x_fechal = crearFecha(datoExtraido[6], datoExtraido[7], datoExtraido[8], datoExtraido[9], datoExtraido[10]);
        x_bus = crearBus(datoExtraido[0], dispo, auxSalida, auxLlegada, x_fechas, x_fechal);
        x_fila = agregar(x_fila, x_bus);
    }
    fclose(fichero);
    //printf(">>>Proceso de lectura completado\n");
    //system("pause");
    return x_fila;
}

/**
 * @brief Escribir los datos de un archivo
 * 
 * @param f fila con los datos
 * @param direccion del archivo
 */
void guardarArchivo(FILA f, char *direccion){
    FILE* fichero;
    //ESCRITURA

    fichero = fopen(direccion, "wt");
    while (esVacia(f) != 1)
    {
        fprintf (fichero, "%d ", f.frente->bus.id);
        fprintf (fichero, "%d ", f.frente->bus.fsalida.minuto);
        fprintf (fichero, "%d ", f.frente->bus.fsalida.hora);
        fprintf (fichero, "%d ", f.frente->bus.fsalida.dia);
        fprintf (fichero, "%d ", f.frente->bus.fsalida.mes);
        fprintf (fichero, "%d ", f.frente->bus.fsalida.anio);
        fprintf (fichero, "%d ", f.frente->bus.fllegada.minuto);
        fprintf (fichero, "%d ", f.frente->bus.fllegada.hora);
        fprintf (fichero, "%d ", f.frente->bus.fllegada.dia);
        fprintf (fichero, "%d ", f.frente->bus.fllegada.mes);
        fprintf (fichero, "%d ", f.frente->bus.fllegada.anio);
        fprintf (fichero, "%c", '\n');
        fprintf (fichero, "%s", f.frente->bus.Salida);
        fprintf (fichero, "%s", f.frente->bus.Llegada);
        for (int i=0; i<39; i++)
            fprintf (fichero, "%d ", f.frente->bus.disponibilidad[i]);
            //fprintf (fichero, "%d ", aleatoreo(0,1));
        fprintf (fichero, "%d %c", f.frente->bus.disponibilidad[39], '\n');
        f = extraer(f);
    }
    fclose(fichero);
    //printf("<<<Proceso de escritura completado \n");
    //system("pause");
}

FECHA obtenerFecha(void){
    FECHA f;
    // Tiempo actual
    time_t t = time(NULL);
    struct tm tiempoLocal = *localtime(&t);
    f = crearFecha(tiempoLocal.tm_min, tiempoLocal.tm_hour, tiempoLocal.tm_mday, tiempoLocal.tm_mon+1, tiempoLocal.tm_year+1900);
    return f;
}
//!! -- agregar funciones aqui
/**
 * @brief Crea una fila vacia
 * 
 * @return FILA vacia
 */
FILA crearFila(){
	FILA nueva;
	nueva.elementos = NULL;
	nueva.frente = NULL;
	nueva.ultimo = NULL;
	return nueva;
}

/**
 * @brief Determina si la fila es vacia
 * 
 * @param f fila
 * @return 1: en caso de que la fila esta vacia y 0 eoc
 */
int esVacia(FILA f){
	return (f.frente == NULL)?1:0;
}

/**
 * @brief Agrega un nuevo elemento a la fila. Regla LIFO
 * 
 * @param f fila de elementos
 * @param bus nuevo elemento que sera agregado
 * @return FILA actualizada
 */
FILA agregar(FILA f, BUS bus){
	ELEMENTO nuevo, auxiliar;

	nuevo = (ELEMENTO) malloc(sizeof(struct s_elemento));
	nuevo->bus = bus;
	nuevo->siguiente = NULL;	
	if (f.ultimo != NULL){
		auxiliar = f.frente;
		while (auxiliar != f.ultimo){
			auxiliar = auxiliar->siguiente;
		}
		auxiliar->siguiente = nuevo;
		f.ultimo = nuevo;
	}
	else{
		f.elementos = nuevo;
		f.frente = nuevo; f.ultimo = nuevo;
	}
	return f;
}

/**
 * @brief Extrae elemento de la fila. Regla LIFO
 * 
 * @param f fila de elementos
 * @return FILA actualizada
 */
FILA extraer(FILA f){	
	f.frente = f.frente->siguiente;
	return f;		
}

/**
 * @brief Retorna (sin extraer) elemento al frente de la fila
 * 
 * @param f fila de elementos
 * @return primer elemento de la fila
 */
BUS verFrente(FILA f){
	return f.frente->bus;
}