/*
 * This shim header replaces infiniband/mlx4dv.h in the gcc search path, then
 * include_next lets us get the system header.
 */
#include_next <infiniband/mlx4dv.h>

#ifndef _INFINIBAND_DYN_MLX4DV_H
#define _INFINIBAND_DYN_MLX4DV_H

/*
 * Aided by:
 *  for I in `readelf -s --wide build/lib/libmlx4.so | grep @@MLX4 | awk '{split($8,a,"@@"); printf(" %s(\n",a[1])}' | sort -u`; do grep -A2 "$I" providers/mlx4/mlx4dv.h ; done
 */
struct dyn_mlx4_ops {
	struct ibv_qp *(*mlx4dv_create_qp)(
		struct ibv_context *context, struct ibv_qp_init_attr_ex *attr,
		struct mlx4dv_qp_init_attr *mlx4_qp_attr);
	int (*mlx4dv_init_obj)(struct mlx4dv_obj *obj, uint64_t obj_type);
	int (*mlx4dv_query_device)(struct ibv_context *ctx_in,
				   struct mlx4dv_context *attrs_out);
	int (*mlx4dv_set_context_attr)(struct ibv_context *context,
				       enum mlx4dv_set_ctx_attr_type attr_type,
				       void *attr);
};

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
