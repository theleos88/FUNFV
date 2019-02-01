/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/queue.h>

#include <rte_memory.h>
#include <rte_launch.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_lcore.h>
#include <rte_debug.h>

/* Parameters */
#define BURST_SIZE	(32)

/* Data structure */
struct nfv_conf {
	int function_type;
	struct rte_ring *ring;
};


static int
lcore_hello(__attribute__((unused)) void *arg)
{
	unsigned lcore_id;
	lcore_id = rte_lcore_id();
	printf("hello from core %u\n", lcore_id);
	return 0;
}

#ifdef READY
static void nfv_block_function( __attribute__((unused)) void *dummy)
{
	unsigned lcore_id = rte_lcore_id();
	int nb_rx = 0, burst_size=BURST_SIZE;
	struct rte_mbuf *bufs[burst_size];
	struct nfv_conf *conf = (struct thread_tx_conf *)dummy;

	printf("hello from core %u\n", lcore_id);

	/*Main loop forever*/
	for (;;) {
		nb_rx = rte_ring_dequeue_burst(conf->ring, (void *)bufs, burst_size);
		if (unlikely(nb_rx == 0))
			continue;
	}
}
#endif


int
main(int argc, char **argv)
{
	int ret;
	unsigned lcore_id;

	ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_panic("Cannot init EAL\n");

	/* call lcore_hello() on every slave lcore */
	rte_eal_mp_remote_launch(lcore_hello, NULL, CALL_MASTER);
	printf("This is main at core %d\n", lcore_id);
//	nfv_block_function();

	rte_eal_mp_wait_lcore();
	return 0;
}

