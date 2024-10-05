#include "server.h"

// pthread_mutex_t log_mutex;
t_log* logger_server;

int main(void) {

	// Inicializar el mutex
    // pthread_mutex_init(&log_mutex, NULL);

	logger_server = log_create("logServidor.log", "SERVER-TEST", 1, LOG_LEVEL_INFO);
	log_info(logger_server, "Bienvenidos al módulo Server");

	int socket_escucha = iniciar_servidor();
	
	// Crear un hilo para cada conexión entrante, para poder manejar múltiples conexiones; cada hilo se encarga de cerrar su conexión
	esperar_cliente_multihilos(socket_escucha);

	// Para cuando el Server acepta una única conexión, por lo cual sólo hay un hilo; la función se encarga de cerrar la conexión
	// int socket_conexion = esperar_cliente(socket_escucha);
	// atender_cliente(socket_conexion);

	log_info(logger_server, "Terminando el programa");
	close(socket_escucha);
	log_destroy(logger_server);

	return 0;
}
	
/*
	int cant_conexiones = 0;
	
	pthread_t conexion_a_filesystem;
	pthread_mutex_lock(&log_mutex);
	log_info(logger, "Hilo Main esperando aceptar una nueva conexión");
	pthread_mutex_unlock(&log_mutex);
	int* socket_conexion = malloc(sizeof(int));
	(*socket_conexion) = accept(server_fd, NULL, NULL);
	pthread_mutex_lock(&log_mutex);
	log_info(logger, "Se conectó el cliente!");
	pthread_mutex_unlock(&log_mutex);
	pthread_create(&conexion_a_filesystem, NULL, (void*) atender_cliente_remix, (void*) socket_conexion);
	pthread_mutex_lock(&log_mutex);
	log_info(logger, "Hilo creado para la conexión nro %d!", cant_conexiones);
	pthread_mutex_unlock(&log_mutex);
	
	pthread_join(conexion_a_filesystem, NULL);
	
	
	while(true){
		pthread_t conexion_a_filesystem;
		sleep(2);
		pthread_mutex_lock(&log_mutex);
		log_info(logger, "Hilo Main esperando aceptar una nueva conexión");
		pthread_mutex_unlock(&log_mutex);
		int* socket_conexion = malloc(sizeof(int));
		(*socket_conexion) = accept(server_fd, NULL, NULL);
		if(*socket_conexion == -1){
			close(server_fd);
			log_error(logger, "Programa terminado, se cerró conexión");
			break;
		}
		cant_conexiones++;
		pthread_mutex_lock(&log_mutex);
		log_info(logger, "Se conectó el cliente nro %d!", cant_conexiones);
		pthread_mutex_unlock(&log_mutex);
		pthread_create(&conexion_a_filesystem, NULL, (void*) atender_cliente_remix, (void*) socket_conexion);
		pthread_mutex_lock(&log_mutex);
		log_info(logger, "Hilo creado para la conexión nro %d!", cant_conexiones);
		pthread_mutex_unlock(&log_mutex);
		pthread_detach(conexion_a_filesystem);
	}
	

	pthread_mutex_destroy(&log_mutex);
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void* atender_cliente(int* socket_conexion){
	//t_list* lista;
	int continuar = 1;
	while (continuar) {
		int cod_op = recibir_operacion(*socket_conexion);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(*socket_conexion);
			break;
		case PAQUETE:
			lista = recibir_paquete(*socket_conexion);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "El cliente se desconectó. Terminando servidor. El cod de operación recibido fue %d", cod_op);
			continuar = 0;
			break;
		default:
			log_warning(logger, "Operación desconocida. No quieras meter la pata");
			break;
		}
	}
	return NULL;
}

void* atender_cliente_remix(int* socket_conexion){
	//t_list* lista;
	int cod_op = recibir_operacion(*socket_conexion);
	int
	switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(*socket_conexion);
			break;
		case PAQUETE:
			lista = recibir_paquete(*socket_conexion);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "El cliente se desconectó. Terminando servidor. El cod de operación recibido fue %d", cod_op);
			break;
		default:
			log_warning(logger, "Operación desconocida. No quieras meter la pata");
			break;
	}
	close(*socket_conexion);
	return NULL;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void* atender_cliente(int* socket_conexion){
	//t_list* lista;
	int continuar = 1;
	while (continuar) {
		int cod_op = recibir_operacion(*socket_conexion);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(*socket_conexion);
			break;
		case PAQUETE:
			lista = recibir_paquete(*socket_conexion);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "El cliente se desconectó. Terminando servidor. El cod de operación recibido fue %d", cod_op);
			continuar = 0;
			break;
		default:
			log_warning(logger, "Operación desconocida. No quieras meter la pata");
			break;
		}
	}
	return NULL;
}
*/