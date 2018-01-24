/*
 * This shim header replaces infiniband/mlx5dv.h in the gcc search path, then
 * include_next lets us get the system header.
 */
#include_next <infiniband/mlx5dv.h>

#ifndef _INFINIBAND_DYN_MLX5DV_H
#define _INFINIBAND_DYN_MLX5DV_H

/*
 * Aided by:
 *  for I in `readelf -s --wide build/lib/libmlx5.so | grep @@MLX5 | awk '{split($8,a,"@@"); printf(" %s(\n",a[1])}' | sort -u`; do grep -A2 "$I" providers/mlx5/mlx5dv.h ; done
 */
struct dyn_mlx5_ops {
	struct ibv_cq_ex *(*mlx5dv_create_cq)(
		struct ibv_context *context,
		struct ibv_cq_init_attr_ex *cq_attr,
		struct mlx5dv_cq_init_attr *mlx5_cq_attr);
	struct ibv_qp *(*mlx5dv_create_qp)(
		struct ibv_context *context,
		struct ibv_qp_init_attr_ex *qp_attr,
		struct mlx5dv_qp_init_attr *mlx5_qp_attr);
	struct ibv_wq *(*mlx5dv_create_wq)(
		struct ibv_context *context,
		struct ibv_wq_init_attr *wq_init_attr,
		struct mlx5dv_wq_init_attr *mlx5_wq_attr);
	int (*mlx5dv_get_clock_info)(struct ibv_context *context,
				     struct mlx5dv_clock_info *clock_info);
	int (*mlx5dv_init_obj)(struct mlx5dv_obj *obj, uint64_t obj_type);
	int (*mlx5dv_query_device)(struct ibv_context *ctx_in,
				   struct mlx5dv_context *attrs_out);
	int (*mlx5dv_set_context_attr)(struct ibv_context *context,
				       enum mlx5dv_set_ctx_attr_type type,
				       void *attr);
};

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
