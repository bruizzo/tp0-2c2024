#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

#define PUERTO "4444"

typedef enum {
	HANDSHAKE = 100,
	MENSAJE = 200,
	PERSONA = 300,
	DESCONEXION = 999
}op_code;

typedef struct {
	int nombre_length;
	char* nombre;
	int casa_real_length;
	char* casa_real;
	int poder;
} t_persona;

// extern pthread_mutex_t log_mutex;

extern t_log* logger_server;

int iniciar_servidor(void);

int esperar_cliente(int socket_escucha);
int recibir_cod_op(int socket_conexion);
void* recibir_buffer(int socket_conexion, int* tamanio_payload);
int deserializar_handshake(void* payload);
char* deserializar_mensaje(void* payload, int tamanio_payload);
t_persona* deserializar_persona(void* payload);


// int esperar_cliente_old(int);
// t_list* recibir_paquete_old(int);
// void recibir_mensaje_old(int);
// int recibir_operacion_old(int);
// void* recibir_buffer_old(int*, int);

#endif /* UTILS_H_ */
