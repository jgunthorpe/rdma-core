/*
 * This shim header replaces infiniband/mlx4dv.h in the gcc search path, then
 * include_next lets us get the system header.
 */
#include_next <infiniband/mlx4dv.h>

#ifndef _INFINIBAND_DYN_MLX4DV_H
#define _INFINIBAND_DYN_MLX4DV_H

#define DECLARE_FNP(_fn) __typeof__(&_fn) _fn
struct dyn_mlx4_ops {
	DECLARE_FNP(mlx4dv_create_qp);
	DECLARE_FNP(mlx4dv_init_obj);
	DECLARE_FNP(mlx4dv_query_device);
	DECLARE_FNP(mlx4dv_set_context_attr);
};
#undef DECLARE_FNP

extern struct dyn_mlx4_ops _mlx4_ops;

/*
 * Generated with:
 *   readelf -s --wide build/lib/libmlx4.so | grep @@MLX | awk '{split($8,a,"@@"); printf("#define %s(...) (_mlx4_ops.%s(__VA_ARGS__))\n",a[1],a[1])}' | sort -u
 */
#ifndef _DYN_VERBS_NO_WRAPPERS
#define mlx4dv_create_qp(...) (_mlx4_ops.mlx4dv_create_qp(__VA_ARGS__))
#define mlx4dv_init_obj(...) (_mlx4_ops.mlx4dv_init_obj(__VA_ARGS__))
#define mlx4dv_query_device(...) (_mlx4_ops.mlx4dv_query_device(__VA_ARGS__))
#define mlx4dv_set_context_attr(...) (_mlx4_ops.mlx4dv_set_context_attr(__VA_ARGS__))
#endif

#endif
