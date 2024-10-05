#include "client.h"

t_log* logger_client;

int main(){
	char* ip;
	char* puerto;

	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger_client = iniciar_logger();
	log_info(logger_client, "Bienvenidos al módulo Client");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	// valor = config_get_string_value(config, "CLAVE");

	/* ---------------- LEER DE CONSOLA ---------------- */

	// leer_consola(logger);

	/*
	int conexiones[5];

	for (int i = 1; i<4; i++){
		conexiones[i] = crear_conexion(ip, puerto, logger);
		enviar_mensaje_multihilos(valor, conexiones[i], i);
		sleep(2);
	}
	*/

	// Creamos una conexión hacia el servidor
	int socket_conexion = crear_conexion(ip, puerto);

	int operacion_ingresada = 0;

	while(operacion_ingresada != DESCONEXION){

    	printf("Ingresa un número para ejecutar una operación: ");
    	scanf("%d", &operacion_ingresada);

		switch (operacion_ingresada) {
    	    case HANDSHAKE: {
    	        log_info(logger_client, "Operación 1: Ejecutando HANDSHAKE");
				int valor_handshake = 5;

				t_paquete* paquete = crear_paquete(HANDSHAKE);
				buffer_add_int(paquete->buffer, valor_handshake);
				enviar_paquete(socket_conexion, paquete);

				/*
				// Armo el paquete con el código de op
				t_paquete* paquete = malloc(sizeof(t_paquete));
				paquete->codigo_operacion = HANDSHAKE;

				// Creo el buffer y lo agrego al paquete
				t_buffer* buffer = malloc(sizeof(t_buffer));
				buffer->size = sizeof(int);
				buffer->stream = malloc(buffer->size);

				memcpy(buffer->stream, &valor_handshake, sizeof(int));
				paquete->buffer = buffer;

				size_t bytes_a_enviar = sizeof(int)					// cód de op
										+ sizeof(int)				// entero para guardar el tamaño del buffer
										+ paquete->buffer->size;	// tamaño real del buffer

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
				*/
    	        break;
			}
    	    case MENSAJE: {
    	        log_info(logger_client, "Operación 2: Ejecutando MENSAJE");
				char* mensaje = "dracarys";

				t_paquete* paquete = crear_paquete(MENSAJE);
				buffer_add_string(paquete->buffer, (int) (strlen(mensaje) + 1), mensaje);
				enviar_paquete(socket_conexion, paquete);

				/*
				// Armo el paquete con el código de op
				t_paquete* paquete = malloc(sizeof(t_paquete));
				paquete->codigo_operacion = MENSAJE;

				// Creo el buffer y lo agrego al paquete
				t_buffer* buffer = malloc(sizeof(t_buffer));
				buffer->size = strlen(mensaje) + 1;
				buffer->stream = malloc(buffer->size);

				memcpy(buffer->stream, mensaje, strlen(mensaje) + 1);
				paquete->buffer = buffer;

				size_t bytes_a_enviar = sizeof(int)					// cód de op
										+ sizeof(int)				// entero para guardar el tamaño del buffer
										+ paquete->buffer->size;	// tamaño real del buffer

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
				*/
    	        break;
			}
    	    case PERSONA_DAENERYS: {
    	        log_info(logger_client, "Operación 3: Ejecutando DAENERYS");;
    	        t_persona* persona = malloc(sizeof(t_persona));
				persona->nombre = "Daenerys";
				persona->nombre_length = strlen(persona->nombre) + 1;
				persona->casa_real = "Targaryen";
				persona->casa_real_length = strlen(persona->casa_real) + 1;
				persona->poder = 1250;

				t_paquete* paquete = crear_paquete(PERSONA);
				buffer_add_int(paquete->buffer, persona->nombre_length);
				buffer_add_string(paquete->buffer, persona->nombre_length, persona->nombre);
				buffer_add_int(paquete->buffer, persona->casa_real_length);
				buffer_add_string(paquete->buffer, persona->casa_real_length, persona->casa_real);
				buffer_add_int(paquete->buffer, persona->poder);
				enviar_paquete(socket_conexion, paquete);

				/*
				// Armo el paquete con el código de op
				t_paquete* paquete = malloc(sizeof(t_paquete));
				paquete->codigo_operacion = PERSONA;

				// Creo el buffer y lo agrego al paquete
				t_buffer* buffer = malloc(sizeof(t_buffer));
				buffer->size = sizeof(int) * 3
							   + persona->nombre_length
							   + persona->casa_real_length;
				buffer->stream = malloc(buffer->size);

				int offset_buffer_temp = 0;
				memcpy(buffer->stream + offset_buffer_temp, &(persona->nombre_length), sizeof(int));
				offset_buffer_temp += sizeof(int);
				memcpy(buffer->stream + offset_buffer_temp, persona->nombre, persona->nombre_length);
				offset_buffer_temp += persona->nombre_length;
				memcpy(buffer->stream + offset_buffer_temp, &(persona->casa_real_length), sizeof(int));
				offset_buffer_temp += sizeof(int);
				memcpy(buffer->stream + offset_buffer_temp, persona->casa_real, persona->casa_real_length);
				offset_buffer_temp += persona->casa_real_length;
				memcpy(buffer->stream + offset_buffer_temp, &(persona->poder), sizeof(int));
				offset_buffer_temp += sizeof(int);
				paquete->buffer = buffer;

				size_t bytes_a_enviar = sizeof(int)					// cód de op
										+ sizeof(int)				// entero para guardar el tamaño del buffer
										+ paquete->buffer->size;	// tamaño real del buffer

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
				*/
    	        break;
			}
			case PERSONA_CERSEI: {
    	        log_info(logger_client, "Operación 301: Ejecutando CERSEI");;
    	        t_persona* persona = malloc(sizeof(t_persona));
				persona->nombre = "Cersei";
				persona->nombre_length = strlen(persona->nombre) + 1;
				persona->casa_real = "Lannister";
				persona->casa_real_length = strlen(persona->casa_real) + 1;
				persona->poder = 985;

				t_paquete* paquete = crear_paquete(PERSONA);
				buffer_add_int(paquete->buffer, persona->nombre_length);
				buffer_add_string(paquete->buffer, persona->nombre_length, persona->nombre);
				buffer_add_int(paquete->buffer, persona->casa_real_length);
				buffer_add_string(paquete->buffer, persona->casa_real_length, persona->casa_real);
				buffer_add_int(paquete->buffer, persona->poder);
				enviar_paquete(socket_conexion, paquete);

				/*
				// Armo el paquete con el código de op
				t_paquete* paquete = malloc(sizeof(t_paquete));
				paquete->codigo_operacion = PERSONA;

				// Creo el buffer y lo agrego al paquete
				t_buffer* buffer = malloc(sizeof(t_buffer));
				buffer->size = sizeof(int) * 3
							   + persona->nombre_length
							   + persona->casa_real_length;
				buffer->stream = malloc(buffer->size);

				int offset_buffer_temp = 0;
				memcpy(buffer->stream + offset_buffer_temp, &(persona->nombre_length), sizeof(int));
				offset_buffer_temp += sizeof(int);
				memcpy(buffer->stream + offset_buffer_temp, persona->nombre, persona->nombre_length);
				offset_buffer_temp += persona->nombre_length;
				memcpy(buffer->stream + offset_buffer_temp, &(persona->casa_real_length), sizeof(int));
				offset_buffer_temp += sizeof(int);
				memcpy(buffer->stream + offset_buffer_temp, persona->casa_real, persona->casa_real_length);
				offset_buffer_temp += persona->casa_real_length;
				memcpy(buffer->stream + offset_buffer_temp, &(persona->poder), sizeof(int));
				offset_buffer_temp += sizeof(int);
				paquete->buffer = buffer;

				size_t bytes_a_enviar = sizeof(int)					// cód de op
										+ sizeof(int)				// entero para guardar el tamaño del buffer
										+ paquete->buffer->size;	// tamaño real del buffer

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
				*/
    	        break;

			}
			case PERSONA_ROBB: {
				log_info(logger_client, "Operación 3: Ejecutando ROBB");;
    	        t_persona* persona = malloc(sizeof(t_persona));
				persona->nombre = "Robb";
				persona->nombre_length = strlen(persona->nombre) + 1;
				persona->casa_real = "Stark";
				persona->casa_real_length = strlen(persona->casa_real) + 1;
				persona->poder = 530;

				t_paquete* paquete = crear_paquete(PERSONA);
				buffer_add_int(paquete->buffer, persona->nombre_length);
				buffer_add_string(paquete->buffer, persona->nombre_length, persona->nombre);
				buffer_add_int(paquete->buffer, persona->casa_real_length);
				buffer_add_string(paquete->buffer, persona->casa_real_length, persona->casa_real);
				buffer_add_int(paquete->buffer, persona->poder);
				enviar_paquete(socket_conexion, paquete);

				/*
				// Armo el paquete con el código de op
				t_paquete* paquete = malloc(sizeof(t_paquete));
				paquete->codigo_operacion = PERSONA;

				// Creo el buffer y lo agrego al paquete
				t_buffer* buffer = malloc(sizeof(t_buffer));
				buffer->size = sizeof(int) * 3
							   + persona->nombre_length
							   + persona->casa_real_length;
				buffer->stream = malloc(buffer->size);

				int offset_buffer_temp = 0;
				memcpy(buffer->stream + offset_buffer_temp, &(persona->nombre_length), sizeof(int));
				offset_buffer_temp += sizeof(int);
				memcpy(buffer->stream + offset_buffer_temp, persona->nombre, persona->nombre_length);
				offset_buffer_temp += persona->nombre_length;
				memcpy(buffer->stream + offset_buffer_temp, &(persona->casa_real_length), sizeof(int));
				offset_buffer_temp += sizeof(int);
				memcpy(buffer->stream + offset_buffer_temp, persona->casa_real, persona->casa_real_length);
				offset_buffer_temp += persona->casa_real_length;
				memcpy(buffer->stream + offset_buffer_temp, &(persona->poder), sizeof(int));
				offset_buffer_temp += sizeof(int);
				paquete->buffer = buffer;

				size_t bytes_a_enviar = sizeof(int)					// cód de op
										+ sizeof(int)				// entero para guardar el tamaño del buffer
										+ paquete->buffer->size;	// tamaño real del buffer

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
				*/
    	        break;
			}
			case DESCONEXION: {
				log_warning(logger_client, "Iniciando desconexión...");
				break;
			}
    	    default:
    	        log_error(logger_client, "Operación inválida");
    	        break;
    	}
	}

	close(socket_conexion);
	log_destroy(logger_client);
	config_destroy(config);
	return 0;
}

/*
	// Armamos y enviamos el paquete
	// paquete(conexion);

	close(conexiones[1]);
	log_info(logger, "Se cerró la conexión 1");
	close(conexiones[2]);
	log_info(logger, "Se cerró la conexión 2");
	close(conexiones[3]);
	log_info(logger, "Se cerró la conexión 3");

	log_destroy(logger);
	config_destroy(config);

	terminar_programa(conexion, logger, config);

}
*/
