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
	int size;
	int offset;
	void* stream;
} t_buffer;

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

void esperar_cliente_multihilos(int socket_escucha);
void* atender_cliente_multihilos(void* socket);
void atender_cliente(int socket_conexion);

int esperar_cliente(int socket_escucha);
int recibir_cod_op(int socket_conexion);
t_buffer* recibir_buffer(int socket_conexion, int* tamanio_payload);
int deserializar_handshake(void* payload);
int deserializar_handshake_new(t_buffer* buffer);
char* deserializar_mensaje(void* payload, int tamanio_payload);
char* deserializar_mensaje_new(t_buffer* buffer);
t_persona* deserializar_persona(void* payload);
t_persona* deserializar_persona_new(t_buffer* buffer);

int buffer_read_int(t_buffer* buffer);
uint8_t buffer_read_uint8(t_buffer* buffer);
uint16_t buffer_read_uint16(t_buffer* buffer);
uint32_t buffer_read_uint32(t_buffer* buffer);
char* buffer_read_string(t_buffer* buffer, uint32_t length);
char* buffer_read_string_int_length(t_buffer* buffer, int length);


// int esperar_cliente_old(int);
// t_list* recibir_paquete_old(int);
// void recibir_mensaje_old(int);
// int recibir_operacion_old(int);
// void* recibir_buffer_old(int*, int);

#endif /* UTILS_H_ */
