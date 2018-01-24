/*
 * This shim header replaces infiniband/mlx5dv.h in the gcc search path, then
 * include_next lets us get the system header.
 */
#include_next <infiniband/mlx5dv.h>

#ifndef _INFINIBAND_DYN_MLX5DV_H
#define _INFINIBAND_DYN_MLX5DV_H

#define DECLARE_FNP(_fn) typeof(&_fn) _fn
struct dyn_mlx5_ops {
	DECLARE_FNP(mlx5dv_create_cq);
	DECLARE_FNP(mlx5dv_create_qp);
	DECLARE_FNP(mlx5dv_create_wq);
	DECLARE_FNP(mlx5dv_get_clock_info);
	DECLARE_FNP(mlx5dv_init_obj);
	DECLARE_FNP(mlx5dv_query_device);
	DECLARE_FNP(mlx5dv_set_context_attr);
};
#undef DECLARE_FNP

extern struct dyn_mlx5_ops _mlx5_ops;

/*
 * Generated with:
 *   readelf -s --wide build/lib/libmlx5.so | grep @@MLX5 | awk '{split($8,a,"@@"); printf("#define %s(...) (_mlx5_ops.%s(__VA_ARGS__))\n",a[1],a[1])}' | sort -u
 */
#ifndef _DYN_VERBS_NO_WRAPPERS
#define mlx5dv_create_cq(...) (_mlx5_ops.mlx5dv_create_cq(__VA_ARGS__))
#define mlx5dv_create_qp(...) (_mlx5_ops.mlx5dv_create_qp(__VA_ARGS__))
#define mlx5dv_create_wq(...) (_mlx5_ops.mlx5dv_create_wq(__VA_ARGS__))
#define mlx5dv_get_clock_info(...) (_mlx5_ops.mlx5dv_get_clock_info(__VA_ARGS__))
#define mlx5dv_init_obj(...) (_mlx5_ops.mlx5dv_init_obj(__VA_ARGS__))
#define mlx5dv_query_device(...) (_mlx5_ops.mlx5dv_query_device(__VA_ARGS__))
#define mlx5dv_set_context_attr(...) (_mlx5_ops.mlx5dv_set_context_attr(__VA_ARGS__))
#endif

#endif
