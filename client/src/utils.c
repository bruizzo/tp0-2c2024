#include "utils.h"

t_log* iniciar_logger() {
	t_log* nuevo_logger;

	// Seteo los parámetros necesarios para iniciar el log
	char* archivo_de_log = "logCliente.log";
	char* nombre_del_proceso = "CLIENT-TEST";
	bool consola_activa = 1;
	t_log_level nivel_de_log = LOG_LEVEL_INFO;

	nuevo_logger = log_create(archivo_de_log, nombre_del_proceso, consola_activa, nivel_de_log);
	
	// Libero el espacio reservado a los parámetros, que ya no se necesitan
	// free(archivo_de_log);
	// free(nombre_del_proceso);

	return nuevo_logger;
}

t_config* iniciar_config(){
	t_config* nuevo_config;
	//char* ruta_del_config = getcwd(NULL, 0);
	nuevo_config = config_create("cliente.config");
	return nuevo_config;
}

int crear_conexion(char *ip, char* puerto){

	int err;

	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(ip, puerto, &hints, &server_info);
	if(err != 0){
		log_error(logger_client, "Error en getadrrinfo");
		return -1;
	}

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family,
								server_info->ai_socktype,
								server_info->ai_protocol);
	if(socket_cliente == -1){
		log_error(logger_client, "Error en socket");
		return -1;
	}

	// Ahora que tenemos el socket, vamos a conectarlo
	err = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);
	if(err == -1){
		log_error(logger_client, "Error en connect");
		return -1;
	}
	
	log_info(logger_client, "¡Se ha establecido conexión exitosa con el servidor!");
	
	freeaddrinfo(server_info);

	return socket_cliente;
}


// ---------------------------------------------------------------
/*
void* serializar_paquete_old(t_paquete* paquete, int bytes) {
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

int crear_conexion_old(char *ip, char* puerto, t_log* logger) {

	int err;

	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = 0;
	socket_cliente = socket(server_info->ai_family,
							server_info->ai_socktype,
							server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
	err = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	if(err == 0){
		log_info(logger, "Se ha establecido conexión exitosa con el servidor!");
	} else {
		log_info(logger, "El intento de conexión ha fallado");
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}

void enviar_mensaje_old(char* mensaje, int socket_cliente) {
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}

void enviar_mensaje_multihilos_old(char* mensaje, int socket_cliente, int iteracion) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	string_append_with_format(&mensaje, " nro %d", iteracion);
	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}


void crear_buffer_old(t_paquete* paquete) {
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_paquete_old(void) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}
*/

t_paquete* crear_paquete(int codigo_operacion){
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = codigo_operacion;
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = 0;
	buffer->offset = 0;
	buffer->stream = NULL;
	paquete->buffer = buffer;
	return paquete;
}

void buffer_add_int(t_buffer* buffer, int data){
	buffer->stream = realloc(buffer->stream, buffer->size + sizeof(int));
	buffer->size += sizeof(int);
	memcpy(buffer->stream + buffer->offset, &data, sizeof(int));
	buffer->offset += sizeof(int);
}

void buffer_add_uint8(t_buffer* buffer, uint8_t data){
	buffer->stream = realloc(buffer->stream, buffer->size + sizeof(uint8_t));
	buffer->size += sizeof(uint8_t);
	memcpy(buffer->stream + buffer->offset, &data, sizeof(uint8_t));
	buffer->offset += sizeof(uint8_t);
}

void buffer_add_uint16(t_buffer* buffer, uint16_t data){
	buffer->stream = realloc(buffer->stream, buffer->size + sizeof(uint16_t));
	buffer->size += sizeof(uint16_t);
	memcpy(buffer->stream + buffer->offset, &data, sizeof(uint16_t));
	buffer->offset += sizeof(uint16_t);
}

void buffer_add_uint32(t_buffer* buffer, uint32_t data){
	buffer->stream = realloc(buffer->stream, buffer->size + sizeof(uint32_t));
	buffer->size += sizeof(uint32_t);
	memcpy(buffer->stream + buffer->offset, &data, sizeof(uint32_t));
	buffer->offset += sizeof(uint32_t);
}

// Agrega string al buffer con un uint32_t adelante indicando su longitud
void buffer_add_string(t_buffer* buffer, uint32_t length, char* string){
	buffer->stream = realloc(buffer->stream, buffer->size + sizeof(uint32_t) + length);
	buffer->size += sizeof(uint32_t) + length;
	memcpy(buffer->stream + buffer->offset, &length, sizeof(uint32_t));
	buffer->offset += sizeof(uint32_t);
	memcpy(buffer->stream + buffer->offset, string, length);
	buffer->offset += length;
}

void enviar_paquete(int socket_conexion, t_paquete* paquete){
	size_t bytes_a_enviar = sizeof(int)					// cód de op
							+ sizeof(int)				// entero para guardar el tamaño del payload
							+ paquete->buffer->size;	// tamaño real del payload

	void* a_enviar = malloc(bytes_a_enviar);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);
	offset += paquete->buffer->size;

	ssize_t bytes_enviados;
	bytes_enviados = send(socket_conexion, a_enviar, bytes_a_enviar, 0);
	log_info(logger_client, "Se han enviado %d bytes de %d bytes", (int) bytes_enviados, (int) bytes_a_enviar);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

/*
void agregar_a_paquete_old(t_paquete* paquete, void* valor, int tamanio) {
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete_old(t_paquete* paquete, int socket_cliente) {
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete_old(t_paquete* paquete) {
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente) {
	close(socket_cliente);
}


// Sin tocar
void leer_consola(t_log* logger) {
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido, "") != 0){
		log_info(logger, leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion) {
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Creo el paquete, con cód de operación PAQUETE
	paquete = crear_paquete();

	leido = readline("> ");

	// Voy leyendo y agregando al paquete cada línea leída
	while(strcmp(leido, "") != 0){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		leido = readline("> ");
	}
	
	// Envío el paquete
	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config) {
	// Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	// con las funciones de las commons y del TP mencionadas en el enunciado
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
*/