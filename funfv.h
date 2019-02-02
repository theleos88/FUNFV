#include <rte_ring.h>

/* DEFINITIONS */
#define DEFAULT_RING_SIZE	(2>>20)


/* DATA STRUCTURES */

/**
Type is a char:
't': transmitter		(Producer only, no rx ring, 1 tx ring
'i': input interface	(many rx rings, 1 tx ring)	!!!Not implemented yet!!!
'd': dispatcher			(1 rx ring, many tx rings)
'f': VNF function		(many rx rings, many tx rings)
*/
typedef struct thread_info{
	int 			id;
	char			type;
	struct rte_ring	*tx_ring;
	struct rte_ring	*rx_ring;
}	thread_t;





/**
Functions: implementation and code everything here... for the moment
*/
int init_single_shared_ring(struct rte_ring* ring);


int init_single_shared_ring(struct rte_ring* ring)
{
	//socket_io = rte_lcore_to_socket_id(rx_conf->conf.lcore_id);
	ring = rte_ring_create("RX_RING", DEFAULT_RING_SIZE, 1,	RING_F_SP_ENQ | RING_F_SC_DEQ);		//TODO: fixed size, name and socket id. Change.
	if (ring == NULL)
		rte_panic("Cannot create ring\n");

	return 0;
}
