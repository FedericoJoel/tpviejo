#ifndef PROTOCOLOS_H_
#define PROTOCOLOS_H_

typedef enum {
	YM_MS_OKCONN,
	MS_YM_DESCONECTAR,
	YM_MS_ERRORCONN,
	MS_YM_INICIO_JOB,
	YM_MS_TRANSFORMACION,
	FIN_TRANSF_NODO,
	FIN_TRANSFORMACION,
	INICIO_TRANF_WORKER,
	DN_OKCONN,
	DN_SETBLOQUE,
	DN_GETBLOQUE,
	DN_GETBLOQUEANSW,
	DN_KEEPALIVE,
	DN_ALIVE,
	FS_YM_ERRORCONN,
	YM_FS_RUTA,
	TRANSFORMACION_OK,
	TRANSFORMACION_ERROR,
	YAMA_OKCONN
}t_proto;

typedef enum {
	TRANSFORMACION,
	REDUC_LOCAL,
	REDUC_GLOBAL
}t_etapa;

typedef enum { //DISTINTOS CLIENTES QUE PUEDEN LLEGAR A UN FILESYSTEM
	DATANODE,
	WORKER,
	YAMA,
	MASTER
}t_clientes_fs;


#endif
