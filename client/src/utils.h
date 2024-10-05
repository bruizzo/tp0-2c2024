#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>

typedef enum {
	HANDSHAKE = 100,
	MENSAJE = 200,
	PERSONA = 300,
	PERSONA_DAENERYS = 301,
	PERSONA_CERSEI = 302,
	PERSONA_ROBB = 303,
	DESCONEXION = 999
} op_code;

typedef struct {
	int size;
	int offset;
	void* stream;
} t_buffer;

typedef struct {
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef struct {
	int nombre_length;
	char* nombre;
	int casa_real_length;
	char* casa_real;
	int poder;
} t_persona;

extern t_log* logger_client;

int crear_conexion(char *ip, char* puerto);
t_log* iniciar_logger();
t_config* iniciar_config();

void buffer_add_int(t_buffer* buffer, int data);
void buffer_add_uint8(t_buffer* buffer, uint8_t data);
void buffer_add_uint16(t_buffer* buffer, uint16_t data);
void buffer_add_uint32(t_buffer* buffer, uint32_t data);
void buffer_add_string(t_buffer* buffer, uint32_t length, char* string);


// void leer_consola(t_log* logger);
// void paquete(int conexion);
// void terminar_programa(int conexion, t_log* logger, t_config* config);


// int crear_conexion_old(char* ip, char* puerto, t_log* logger);
// void enviar_mensaje_old(char* mensaje, int socket_cliente);
// void enviar_mensaje_multihilos_old(char* mensaje, int socket_cliente, int iteracion);
// t_paquete* crear_paquete(void);
// void agregar_a_paquete_old(t_paquete* paquete, void* valor, int tamanio);
// void enviar_paquete_old(t_paquete* paquete, int socket_cliente);
// void liberar_conexion(int socket_cliente);
// void eliminar_paquete_old(t_paquete* paquete);

#endif /* UTILS_H_ */
