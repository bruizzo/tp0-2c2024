#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "utils.h"

void iterator(char* value);
void* atender_cliente(int* socket_conexion);
void* atender_cliente_remix(int* socket_conexion);

#endif /* SERVER_H_ */
