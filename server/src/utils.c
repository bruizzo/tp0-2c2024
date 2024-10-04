#include"utils.h"

int iniciar_servidor(){
	int socket_servidor;
	int err;

	struct addrinfo hints, *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(NULL, PUERTO, &hints, &server_info);
	if(err != 0){
		log_error(logger_server, "Error en getadrrinfo");
		return -1;
	}

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(server_info->ai_family,
							 server_info->ai_socktype,
							 server_info->ai_protocol);

	if(socket_servidor == -1){
		log_error(logger_server, "Error en socket");
		return -1;
	}

	// Asociamos el socket a un puerto
	err = setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));
	if(err == -1){
		log_error(logger_server, "Error en setsockopt");
		return -1;
	}

	err = bind(socket_servidor, server_info->ai_addr, server_info->ai_addrlen);
	if(err == -1){
		log_error(logger_server, "Error en bind");
		return -1;
	}

	// Escuchamos las conexiones entrantes
	err = listen(socket_servidor, SOMAXCONN);
	if(err == -1){
		log_error(logger_server, "Error en listen");
		return -1;
	}

	freeaddrinfo(server_info);
	log_info(logger_server, "¡Estoy listo para escuchar a mi cliente!");

	return socket_servidor;
}

int esperar_cliente(int socket_escucha){
		int socket_conexion = accept(socket_escucha, NULL, NULL);
		if(socket_conexion == -1){
			log_error(logger_server, "Error al intentar aceptar una conexión entrante");
			return -1;
		} else {
			log_info(logger_server, "¡Se ha iniciado una conexión! Socket de conexión: %d", socket_conexion);
		}

		return socket_conexion;
}

int recibir_cod_op(int socket_conexion){
	
	int cod_op;
	ssize_t bytes_leidos;
	
	bytes_leidos = recv(socket_conexion, &cod_op, sizeof(int), MSG_WAITALL);
	log_info(logger_server, "Se han leído %d bytes", (int) bytes_leidos);
	if(bytes_leidos <= 0){
		log_error(logger_server, "Hubo un error en la recepción del código de operación");
		return -1;
	}

	return cod_op;
}

void* recibir_buffer(int socket_conexion, int* tamanio_payload){
	ssize_t bytes_leidos;

	bytes_leidos = recv(socket_conexion, tamanio_payload, sizeof(int), MSG_WAITALL);
	log_info(logger_server, "Se han leído %d bytes; el payload tiene un tamaño de %d bytes ", (int) bytes_leidos, *tamanio_payload);
	if(bytes_leidos <= 0){
		log_error(logger_server, "Hubo un error en la recepción del tamño del payload");
		return NULL;
	}

	void* payload = malloc(*tamanio_payload);
	bytes_leidos = recv(socket_conexion, payload, *tamanio_payload, MSG_WAITALL);
	log_info(logger_server, "Se han leído %d bytes (payload)", (int) bytes_leidos);
	if(bytes_leidos <= 0){
		log_error(logger_server, "Hubo un error en la recepción del tamño del payload");
		return NULL;
	}

	return payload;
}

int deserializar_handshake(void* payload){

	int handshake;
	int offset = 0;
	memcpy(&handshake, payload + offset, sizeof(int));
	offset += sizeof(int);

	return handshake;
}

char* deserializar_mensaje(void* payload, int tamanio_payload){
	
	char* mensaje = malloc(tamanio_payload);
	int offset = 0;
	memcpy(mensaje, payload + offset, tamanio_payload);
	offset += tamanio_payload;
	
	return mensaje;
}

t_persona* deserializar_persona(void* payload){

	t_persona* persona = malloc(sizeof(t_persona));
	int offset = 0;
	memcpy(&(persona->nombre_length), payload + offset, sizeof(int));
	offset += sizeof(int);
	persona->nombre = malloc(persona->nombre_length);
	memcpy(persona->nombre, payload + offset, persona->nombre_length);
	offset += persona->nombre_length;
	memcpy(&(persona->casa_real_length), payload + offset, sizeof(int));
	offset += sizeof(int);
	persona->casa_real = malloc(persona->casa_real_length);
	memcpy(persona->casa_real, payload + offset, persona->casa_real_length);
	offset += persona->casa_real_length;
	memcpy(&(persona->poder), payload + offset, sizeof(int));
	offset += sizeof(int);

	return persona;
}

/*
// Viejas

int esperar_cliente_old(int socket_servidor)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	// assert(!"no implementado!");
	log_info(logger, "Por hacer el accept...");
	// Aceptamos un nuevo cliente
	int socket_cliente;
	socket_cliente = accept(socket_servidor, NULL, NULL);
	log_info(logger, "Se conectó un cliente!");

	return socket_cliente;
}

int recibir_operacion_old(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer_old(int* size, int socket_cliente){
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje_old(int socket_cliente){
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	pthread_mutex_lock(&log_mutex);
	log_info(logger, "Me llego el mensaje %s en el socket nro %d", buffer, socket_cliente);
	pthread_mutex_unlock(&log_mutex);
	free(buffer);
}

t_list* recibir_paquete_old(int socket_cliente){
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
*/