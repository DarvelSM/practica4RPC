#include "sumar.h"    //incluye la biblioteca donde se definen funciones y librerias


void
suma_prog_1(char *host, int a, int b)        //define los argumentos que va a recibir para la conexión con el servidor
{
	CLIENT *clnt;
	int  *result_1;
	dupla_int  suma_1_arg;          //variables para registrar dos números 

#ifndef	DEBUG
	clnt = clnt_create (host, SUMA_PROG, SUMA_VERS, "udp"); 
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
  suma_1_arg.a = a;
  suma_1_arg.b = b;

	result_1 = suma_1(&suma_1_arg, clnt);     
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");             //si hay un error imprime un mensaje que informa sobre este y se desconecta del servidor 
	} else {
		printf("result = %d\n", *result_1);    //recibe el resultado por parte del servidor
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	int a, b;
	if (argc != 4) {
		printf ("usage: %s server_host num1 num2\n", argv[0]); //recibe como argumento la dirección del servidor y recibe los parámetros para realizar la suma 
	exit(1);
	}
	host = argv[1];
	if ((a = atoi(argv[2])) == 0 && *argv[2] != '0') {
		fprintf(stderr, "invalid value: %s\n", argv[2]);   //verifica que los argumentos sean válidos
	exit(1);
	}
	if ((b = atoi(argv[3])) == 0 && *argv[3] != '0') {
		fprintf(stderr, "invalid value: %s\n", argv[3]);   //verifica que los arguimentos sean válidos
	exit(1);
	}
	suma_prog_1(host, a, b);
}
