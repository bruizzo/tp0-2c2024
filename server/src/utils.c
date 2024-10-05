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

void esperar_cliente_multihilos(int socket_escucha){

	while (1) {
    	pthread_t thread_conexion;
    	int* socket_conexion = malloc(sizeof(int));
    	*socket_conexion = accept(socket_escucha, NULL, NULL);
		if(*socket_conexion == -1){
			log_error(logger_server, "Error al intentar aceptar una conexión entrante");
		} else {
			log_info(logger_server, "¡Se ha iniciado una conexión! Socket de conexión: %d", *socket_conexion);
		}
		int err = pthread_create(&thread_conexion, NULL, atender_cliente_multihilos, socket_conexion);
		if (err != 0) {
        	log_error(logger_server, "Error al crear el hilo para manejar la conexión");
    	} else {
			log_info(logger_server, "Hilo creado exitosamente para manejar la conexión");
			pthread_detach(thread_conexion);
		}	
	}
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

void* atender_cliente_multihilos(void* socket){

	int socket_conexion = *(int*)socket;
	free(socket);

	int continuar = 1;
	while(continuar){
		int cod_op = recibir_cod_op(socket_conexion);
		int tamanio_payload;
		// void* payload;
		t_buffer* buffer;
		switch(cod_op){
			case HANDSHAKE:
				// payload = recibir_buffer(socket_conexion, &tamanio_payload);
				buffer = recibir_buffer(socket_conexion, &tamanio_payload);
				// int handshake = deserializar_handshake(payload);
				int handshake = deserializar_handshake_new(buffer);
				log_info(logger_server, "El valor recibido del handshake fue %d", handshake);
				// free(payload);
				free(buffer);
				break;
			case MENSAJE:
				// payload = recibir_buffer(socket_conexion, &tamanio_payload);
				buffer = recibir_buffer(socket_conexion, &tamanio_payload);
				// char* mensaje = deserializar_mensaje(payload, tamanio_payload);
				char* mensaje = deserializar_mensaje_new(buffer);
				log_info(logger_server, "El mensaje recibido fue: %s", mensaje);
				// free(payload);
				free(buffer);
				free(mensaje);
				break;
			case PERSONA:
				// payload = recibir_buffer(socket_conexion, &tamanio_payload);
				buffer = recibir_buffer(socket_conexion, &tamanio_payload);
				// t_persona* persona = deserializar_persona(payload);
				t_persona* persona = deserializar_persona_new(buffer);
				log_info(logger_server, "La persona recibida fue %s, de la casa %s, con un poder %d", persona->nombre, persona->casa_real, persona->poder);
				// free(payload);
				free(buffer);
				free(persona->nombre);
				free(persona->casa_real);
				free(persona);
				break;
			case DESCONEXION:
				log_warning(logger_server, "El cliente se ha desconectado. Cerrando conexión");
				continuar = 0;
				break;
			case -1:
				log_error(logger_server, "Se produjo un error. Cerrando conexión");
				continuar = 0;
				break;
			default:
				log_warning(logger_server, "No se conoce otra operación, intente nuevamente");
				break;
		}
	}

	close(socket_conexion);
	return NULL;
}

void atender_cliente(int socket_conexion){

	int continuar = 1;
	while(continuar){
		int cod_op = recibir_cod_op(socket_conexion);
		int tamanio_payload;
		// void* payload;
		t_buffer* buffer;
		switch(cod_op){
			case HANDSHAKE:
				// payload = recibir_buffer(socket_conexion, &tamanio_payload);
				buffer = recibir_buffer(socket_conexion, &tamanio_payload);
				// int handshake = deserializar_handshake(payload);
				int handshake = deserializar_handshake_new(buffer);
				log_info(logger_server, "El valor recibido del handshake fue %d", handshake);
				// free(payload);
				free(buffer);
				break;
			case MENSAJE:
				// payload = recibir_buffer(socket_conexion, &tamanio_payload);
				buffer = recibir_buffer(socket_conexion, &tamanio_payload);
				// char* mensaje = deserializar_mensaje(payload, tamanio_payload);
				char* mensaje = deserializar_mensaje_new(buffer);
				log_info(logger_server, "El mensaje recibido fue: %s", mensaje);
				// free(payload);
				free(buffer);
				free(mensaje);
				break;
			case PERSONA:
				// payload = recibir_buffer(socket_conexion, &tamanio_payload);
				buffer = recibir_buffer(socket_conexion, &tamanio_payload);
				// t_persona* persona = deserializar_persona(payload);
				t_persona* persona = deserializar_persona_new(buffer);
				log_info(logger_server, "La persona recibida fue %s, de la casa %s, con un poder %d", persona->nombre, persona->casa_real, persona->poder);
				// free(payload);
				free(buffer);
				free(persona->nombre);
				free(persona->casa_real);
				free(persona);
				break;
			case DESCONEXION:
				log_warning(logger_server, "El cliente se ha desconectado. Cerrando conexión");
				continuar = 0;
				break;
			case -1:
				log_error(logger_server, "Se produjo un error. Cerrando conexión");
				continuar = 0;
				break;
			default:
				log_warning(logger_server, "No se conoce otra operación, intente nuevamente");
				break;
		}
	}

	close(socket_conexion);
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

t_buffer* recibir_buffer(int socket_conexion, int* tamanio_payload){
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = 0;
	buffer->offset = 0;
	buffer->stream = NULL;
	
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

	buffer->size = *tamanio_payload;
	buffer->stream = payload;

	return buffer;
}

int deserializar_handshake(void* payload){

	int handshake;
	int offset = 0;
	memcpy(&handshake, payload + offset, sizeof(int));
	offset += sizeof(int);

	return handshake;
}

int deserializar_handshake_new(t_buffer* buffer){

	int handshake;
	handshake = buffer_read_int(buffer);
	return handshake;
}

int buffer_read_int(t_buffer* buffer){
	int data;
	memcpy(&data, buffer->stream + buffer->offset, sizeof(int));
	buffer->offset += sizeof(int);
	return data;
}

uint8_t buffer_read_uint8(t_buffer* buffer){
	uint8_t data;
	memcpy(&data, buffer->stream + buffer->offset, sizeof(uint8_t));
	buffer->offset += sizeof(uint8_t);
	return data;
}

uint16_t buffer_read_uint16(t_buffer* buffer){
	uint16_t data;
	memcpy(&data, buffer->stream + buffer->offset, sizeof(uint16_t));
	buffer->offset += sizeof(uint16_t);
	return data;
}

uint32_t buffer_read_uint32(t_buffer* buffer){
	uint16_t data;
	memcpy(&data, buffer->stream + buffer->offset, sizeof(uint32_t));
	buffer->offset += sizeof(uint32_t);
	return data;
}

char* buffer_read_string(t_buffer* buffer, uint32_t length){
	char* data = malloc(length);
	memcpy(data, buffer->stream + buffer->offset, length);
	buffer->offset += length;
	return data;
}

char* buffer_read_string_int_length(t_buffer* buffer, int length){
	char* data = malloc(length);
	memcpy(data, buffer->stream + buffer->offset, length);
	buffer->offset += length;
	return data;
}

char* deserializar_mensaje(void* payload, int tamanio_payload){
	
	char* mensaje = malloc(tamanio_payload);
	int offset = 0;
	memcpy(mensaje, payload + offset, tamanio_payload);
	offset += tamanio_payload;
	
	return mensaje;
}

char* deserializar_mensaje_new(t_buffer* buffer){
	
	char* mensaje = buffer_read_string(buffer, (uint32_t)(buffer->size));
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

t_persona* deserializar_persona_new(t_buffer* buffer){

	t_persona* persona = malloc(sizeof(t_persona));
	persona->nombre_length = buffer_read_int(buffer);
	persona->nombre = buffer_read_string(buffer, (uint32_t)(persona->nombre_length));
	persona->casa_real_length = buffer_read_int(buffer);
	persona->casa_real = buffer_read_string(buffer, (uint32_t)(persona->casa_real_length));
	persona->poder = buffer_read_int(buffer);

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