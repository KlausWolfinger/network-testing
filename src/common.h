/* -*- c-file-style: "linux" -*-
 * Author: Jesper Dangaard Brouer <netoptimizer@brouer.com>, (C)2014
 * License: GPLv2
 * From: https://github.com/netoptimizer/network-testing
 *
 * Common/shared helper functions
 */
#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

extern int verbose;

#define PKTGEN_MAGIC 0xbe9be955

struct pktgen_hdr {
	uint32_t pgh_magic;
	uint32_t seq_num;
	uint32_t tv_sec;
	uint32_t tv_usec;
};

struct time_bench_record
{
	/* Stats */
	int64_t  packets;
	uint64_t bytes;
	uint64_t tsc_start;
	uint64_t tsc_stop;
	uint64_t time_start;
	uint64_t time_stop;
	uint64_t try_again; /* EAGAIN include/uapi/asm-generic/errno-base.h */

	/* Calculated stats */
	uint64_t tsc_interval;
	uint64_t time_interval;
	uint64_t tsc_cycles;
	uint64_t payload_pktsz;

	double pps, ns_per_pkt, timesec;

	/* Settings */
	int ip_early_demux;
};

struct params_common {
	int connect;
};

static inline uint64_t rdtsc()
{
	uint32_t low, high;
	asm volatile("rdtsc" : "=a" (low), "=d" (high));
	return low  | (((uint64_t )high ) << 32);
}

uint64_t gettime(void);
void time_bench_start(struct time_bench_record *r);
void time_bench_stop(struct time_bench_record *r);
void time_bench_calc_stats(struct time_bench_record *r);
void time_bench_print_stats(struct time_bench_record *r,
			    struct params_common *c);
void time_bench_record_setting(struct time_bench_record *r);

char *malloc_payload_buffer(int msg_sz);
void print_result(uint64_t tsc_cycles, double ns_per_pkt, double pps,
		  double timesec, int cnt_send, uint64_t tsc_interval);
void print_header(const char *fct, int batch);

/* Using __builtin_constant_p(x) to ignore cases where the return
 * value is always the same.
 */
# ifndef likely
#  define likely(x)	(__builtin_constant_p(x) ? !!(x) : __builtin_expect((x),1))
# endif
# ifndef unlikely
#  define unlikely(x)	(__builtin_constant_p(x) ? !!(x) : __builtin_expect((x),0))
# endif

int time_func(int loops,
	      int (*func)(int loops, uint64_t* tsc_begin, uint64_t* tsc_end,
			  uint64_t* time_begin, uint64_t* time_end)
	);

#endif /* COMMON_H */
