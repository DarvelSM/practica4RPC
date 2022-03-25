
#include "sumar.h"

int *
suma_1_svc(dupla_int *argp, struct svc_req *rqstp)
{
	static int  result;
	
	printf("El procedimiento ha sido invocado remotamente\n");  //verifica que la conexión es correcta
	printf("Server is working \n");
	printf("parameters: %d, %d\n", argp->a, argp->b);    //imprime los parámetros recibidos del cliente 
	printf("Server response to client...\n");
	printf("parameters: %d, %d\n", argp->a, argp->b);   
	result = argp->a + argp->b;               //realiza la suma  
	printf("returning: %d\n", result);        //devuelve el resultdo al cliente
	return &result;			//regresa el resultado					
}
