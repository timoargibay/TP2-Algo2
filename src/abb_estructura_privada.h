#ifndef ABB_ESTRUCTURA_PRIVADA_H_
#define ABB_ESTRUCTURA_PRIVADA_H_

#include <stdlib.h>

//ESTE ARCHIVO NO ES PARA EL USUARIO
//
//NO TE QUIERO VER INCLUYENDO ESTO EN TU PROGRAMA

typedef struct nodo {
	void *dato;
	struct nodo *izq;
	struct nodo *der;
} nodo_t;

struct abb {
	size_t cantidad;
	nodo_t *raiz;
	int (*comparador)(const void *, const void *);
};

#endif // ABB_ESTRUCTURA_PRIVADA_H_
