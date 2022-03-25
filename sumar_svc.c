#include "sumar.h"              //incluye la biblioteca donde se definen funciones y librerias
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
suma_prog_1(struct svc_req *rqstp, register SVCXPRT *transp)           //funcion para 
{
	union {
		dupla_int suma_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:								//procedimiento nulo
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case SUMA:								//procedimientos remotos soportados
		_xdr_argument = (xdrproc_t) xdr_dupla_int;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) suma_1_svc;    //apuntador a función (local) a la función de servidor
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");           //si faltan argumentos para realizar la operación deseada desconecta al cliente y espera uno nuevo 
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;             //registra los procedimientos

	pmap_unset (SUMA_PROG, SUMA_VERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");             //verificación del funcionamiento del servicio udp
		exit(1);
	}
	if (!svc_register(transp, SUMA_PROG, SUMA_VERS, suma_prog_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (SUMA_PROG, SUMA_VERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");      //virificación de la conexión mediante tcp
		exit(1);
	}
	if (!svc_register(transp, SUMA_PROG, SUMA_VERS, suma_prog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (SUMA_PROG, SUMA_VERS, tcp).");    
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");    //devuelve el resultado al cliente
	exit (1);
	/* NOTREACHED */
}
