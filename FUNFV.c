/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <signal.h>
#include <sys/queue.h>


#include <rte_ring.h>		// For memory ring
#include <rte_cycles.h>		// For rte_delay
#include <rte_memory.h>
#include <rte_launch.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_lcore.h>
#include <rte_debug.h>

#include "funfv.h"

/* Parameters */
#define BURST_SIZE	(32)
#define N_TX	1	//Number of transmitters (i.e. number of input queues)
#define N_DX	1	//Number of dispatchers (i.e. classifiers and VNF schedulers)
#define N_VNF	0	//Number of VNFS


/* Global variables*/ 
thread_t		vnf[2];		//TODO: Add parameter here.
struct rte_ring *ring;		//TODO: add parametrized array here.



static void handler(int sig){
	unsigned lcore_id = rte_lcore_id();
	printf("Received SigINT %d: exiting At core %d\n", sig, lcore_id);
	exit(0);
}

static int
lcore_hello(__attribute__((unused)) void *arg)
{
	unsigned lcore_id;
	lcore_id = rte_lcore_id();
	printf("hello from core %u\n", lcore_id);
	rte_delay_ms(2000);
	printf("hello again from core %u\n", lcore_id);
	return 0;
}

#ifdef IGNORE

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

	signal(SIGINT, handler);

	init_single_shared_ring(ring);

	/* HARDCODED TODO: change*/
	vnf[0].id=9;
	vnf[0].type='t';
	vnf[0].tx_ring=ring;
	vnf[0].rx_ring=NULL;

	vnf[1].id=10;
	vnf[1].type='d';
	vnf[1].tx_ring=NULL;
	vnf[1].rx_ring=ring;


	rte_eal_mp_remote_launch(lcore_hello, NULL, CALL_MASTER);
	printf("This is main at core %d\n", lcore_id);
//	nfv_block_function();

	rte_eal_mp_wait_lcore();
	return 0;
}

