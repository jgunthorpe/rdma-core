#define _GNU_SOURCE
#define _DYN_VERBS_NO_WRAPPERS 1
#include "infiniband/verbs.h"
#include "infiniband/mlx4dv.h"
#include "infiniband/mlx5dv.h"

#include <dlfcn.h>
#include <stdio.h>

#define stringify(expr) stringify_1(expr)
#define stringify_1(expr) #expr

struct dyn_verbs_ops _verbs_ops;
struct dyn_mlx4_ops _mlx4_ops;
struct dyn_mlx5_ops _mlx5_ops;

/*
 * Compiling this function, even if it isn't used, causes the compiler to type
 * check our function ops signatures against the API signature from the
 * header. This ensures we do not accidently call a function with the wrong
 * signature should the API change in some way.
 */
static inline void hard_link_init()
{
	_verbs_ops = (struct dyn_verbs_ops){
		.ibv_ack_async_event = &ibv_ack_async_event,
		.ibv_ack_cq_events = &ibv_ack_cq_events,
		.ibv_alloc_pd = &ibv_alloc_pd,
		.ibv_attach_mcast = &ibv_attach_mcast,
		.ibv_close_device = &ibv_close_device,
		.ibv_create_ah_from_wc = &ibv_create_ah_from_wc,
		.ibv_create_ah = &ibv_create_ah,
		.ibv_create_comp_channel = &ibv_create_comp_channel,
		.ibv_create_cq = &ibv_create_cq,
		.ibv_create_qp = &ibv_create_qp,
		.ibv_create_srq = &ibv_create_srq,
		.ibv_dealloc_pd = &ibv_dealloc_pd,
		.ibv_dereg_mr = &ibv_dereg_mr,
		.ibv_destroy_ah = &ibv_destroy_ah,
		.ibv_destroy_comp_channel = &ibv_destroy_comp_channel,
		.ibv_destroy_cq = &ibv_destroy_cq,
		.ibv_destroy_qp = &ibv_destroy_qp,
		.ibv_destroy_srq = &ibv_destroy_srq,
		.ibv_detach_mcast = &ibv_detach_mcast,
		.ibv_event_type_str = &ibv_event_type_str,
		.ibv_fork_init = &ibv_fork_init,
		.ibv_free_device_list = &ibv_free_device_list,
		.ibv_get_async_event = &ibv_get_async_event,
		.ibv_get_cq_event = &ibv_get_cq_event,
		.ibv_get_device_guid = &ibv_get_device_guid,
		.ibv_get_device_list = &ibv_get_device_list,
		.ibv_get_device_name = &ibv_get_device_name,
		.ibv_init_ah_from_wc = &ibv_init_ah_from_wc,
		.ibv_modify_qp = &ibv_modify_qp,
		.ibv_modify_srq = &ibv_modify_srq,
		.ibv_node_type_str = &ibv_node_type_str,
		.ibv_open_device = &ibv_open_device,
		.ibv_port_state_str = &ibv_port_state_str,
		.ibv_query_device = &ibv_query_device,
		.ibv_query_gid = &ibv_query_gid,
		.ibv_query_pkey = &ibv_query_pkey,
		.ibv_query_port = &ibv_query_port,
		.ibv_query_qp = &ibv_query_qp,
		.ibv_query_srq = &ibv_query_srq,
		.ibv_rate_to_mbps = &ibv_rate_to_mbps,
		.ibv_rate_to_mult = &ibv_rate_to_mult,
		.ibv_reg_mr = &ibv_reg_mr,
		.ibv_rereg_mr = &ibv_rereg_mr,
		.ibv_resize_cq = &ibv_resize_cq,
		.ibv_resolve_eth_l2_from_gid = &ibv_resolve_eth_l2_from_gid,
		.ibv_wc_status_str = &ibv_wc_status_str,
		.mbps_to_ibv_rate = &mbps_to_ibv_rate,
		.mult_to_ibv_rate = &mult_to_ibv_rate,
	};

	_mlx4_ops = (struct dyn_mlx4_ops){
		.mlx4dv_create_qp = &mlx4dv_create_qp,
		.mlx4dv_init_obj = &mlx4dv_init_obj,
		.mlx4dv_query_device = &mlx4dv_query_device,
		.mlx4dv_set_context_attr = &mlx4dv_set_context_attr,
	};

	_mlx5_ops = (struct dyn_mlx5_ops){
		.mlx5dv_create_cq = &mlx5dv_create_cq,
		.mlx5dv_create_qp = &mlx5dv_create_qp,
		.mlx5dv_create_wq = &mlx5dv_create_wq,
		.mlx5dv_get_clock_info = &mlx5dv_get_clock_info,
		.mlx5dv_init_obj = &mlx5dv_init_obj,
		.mlx5dv_query_device = &mlx5dv_query_device,
		.mlx5dv_set_context_attr = &mlx5dv_set_context_attr,
	};
}

#ifndef DYN_VERBS_NO_DLOPEN

#include "dyn-verbs-symver.h"

static inline void *_dl_get(void *handle, const char *sym, const char *ver,
			   int *res)
{
	void *ret;
	ret = dlvsym(handle, sym, ver);
	if (!ret)
		*res = -1;

	return ret;
}

static int dl_verbs_init(void *verbs_handle, struct dyn_verbs_ops *ops)
{
	int res = 0;

#define set_sym(name) ops->name = _dl_get(verbs_handle, stringify(name), VERSION_ ## name, &res)

	set_sym(ibv_ack_async_event);
	set_sym(ibv_ack_cq_events);
	set_sym(ibv_alloc_pd);
	set_sym(ibv_attach_mcast);
	set_sym(ibv_close_device);
	set_sym(ibv_create_ah_from_wc);
	set_sym(ibv_create_ah);
	set_sym(ibv_create_comp_channel);
	set_sym(ibv_create_cq);
	set_sym(ibv_create_qp);
	set_sym(ibv_create_srq);
	set_sym(ibv_dealloc_pd);
	set_sym(ibv_dereg_mr);
	set_sym(ibv_destroy_ah);
	set_sym(ibv_destroy_comp_channel);
	set_sym(ibv_destroy_cq);
	set_sym(ibv_destroy_qp);
	set_sym(ibv_destroy_srq);
	set_sym(ibv_detach_mcast);
	set_sym(ibv_event_type_str);
	set_sym(ibv_fork_init);
	set_sym(ibv_free_device_list);
	set_sym(ibv_get_async_event);
	set_sym(ibv_get_cq_event);
	set_sym(ibv_get_device_guid);
	set_sym(ibv_get_device_list);
	set_sym(ibv_get_device_name);
	set_sym(ibv_init_ah_from_wc);
	set_sym(ibv_modify_qp);
	set_sym(ibv_modify_srq);
	set_sym(ibv_node_type_str);
	set_sym(ibv_open_device);
	set_sym(ibv_port_state_str);
	set_sym(ibv_query_device);
	set_sym(ibv_query_gid);
	set_sym(ibv_query_pkey);
	set_sym(ibv_query_port);
	set_sym(ibv_query_qp);
	set_sym(ibv_query_srq);
	set_sym(ibv_rate_to_mbps);
	set_sym(ibv_rate_to_mult);
	set_sym(ibv_reg_mr);
	set_sym(ibv_rereg_mr);
	set_sym(ibv_resize_cq);
	set_sym(ibv_resolve_eth_l2_from_gid);
	set_sym(ibv_wc_status_str);
	set_sym(mbps_to_ibv_rate);
	set_sym(mult_to_ibv_rate);

#undef set_sym

	return res;
}

static int dl_mlx4_init(void *mlx4_handle, struct dyn_mlx4_ops *ops)
{
	int res = 0;

#define set_sym(name) ops->name = _dl_get(mlx4_handle, stringify(name), VERSION_ ## name, &res)

	set_sym(mlx4dv_create_qp);
	set_sym(mlx4dv_init_obj);
	set_sym(mlx4dv_query_device);
	set_sym(mlx4dv_set_context_attr);

#undef set_sym

	return res;
}

static int dl_mlx5_init(void *mlx5_handle, struct dyn_mlx5_ops *ops)
{
	int res = 0;

#define set_sym(name) ops->name = _dl_get(mlx5_handle, stringify(name), VERSION_ ## name, &res)

	set_sym(mlx5dv_create_cq);
	set_sym(mlx5dv_create_qp);
	set_sym(mlx5dv_create_wq);
	set_sym(mlx5dv_get_clock_info);
	set_sym(mlx5dv_init_obj);
	set_sym(mlx5dv_query_device);
	set_sym(mlx5dv_set_context_attr);

#undef set_sym

	return res;
}

//__attribute__((constructor))
int dl_link_init(void)
{
	void *verbs;
	void *mlx5;
	void *mlx4;

	verbs = dlopen(SONAME_libibverbs, RTLD_NOW);
	if (!verbs) {
		fprintf(stderr, "Could not open library: %s\n", dlerror());
		return -1;
	}

	if (dl_verbs_init(verbs, &_verbs_ops)) {
		fprintf(stderr, "Could not find libibverbs symbols\n");
		return -1;
	}

	mlx4 = dlopen(SONAME_libmlx4, RTLD_NOW);
	if (!mlx4) {
		fprintf(stderr, "Could not open library: %s\n", dlerror());
		return -1;
	}

	if (dl_mlx4_init(mlx4, &_mlx4_ops)) {
		fprintf(stderr, "Could not find libmlx4 symbols\n");
		return -1;
	}


	mlx5 = dlopen(SONAME_libmlx5, RTLD_NOW);
	if (!mlx5) {
		fprintf(stderr, "Could not open library: %s\n", dlerror());
		return -1;
	}

	if (dl_mlx5_init(mlx5, &_mlx5_ops)) {
		fprintf(stderr, "Could not find libmlx5 symbols\n");
		return -1;
	}

	return 0;
}

#endif
