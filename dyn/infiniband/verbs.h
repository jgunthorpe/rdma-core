/*
 * This shim header replaces infiniband/verbs.h in the gcc search path, then
 * include_next lets us get the system header.
 */
#include_next <infiniband/verbs.h>

#ifndef _INFINIBAND_DYN_VERBS_H
#define _INFINIBAND_DYN_VERBS_H

#undef ibv_query_port

/*
 * Aided by:
 *  for I in `readelf -s --wide build/lib/libibverbs.so | grep @@IBVERBS | grep -v PRIVATE | sort -u |  awk '{split($8,a,"@@"); printf(" %s(\n",a[1])}' | sort -u`; do grep "$I" libibverbs/verbs.h; done
 */
struct dyn_verbs_ops {
	void (*ibv_ack_async_event)(struct ibv_async_event *event);
	void (*ibv_ack_cq_events)(struct ibv_cq *cq, unsigned int nevents);
	struct ibv_pd *(*ibv_alloc_pd)(struct ibv_context *context);
	int (*ibv_attach_mcast)(struct ibv_qp *qp, const union ibv_gid *gid,
				uint16_t lid);
	int (*ibv_close_device)(struct ibv_context *context);
	struct ibv_ah *(*ibv_create_ah_from_wc)(struct ibv_pd *pd,
						struct ibv_wc *wc,
						struct ibv_grh *grh,
						uint8_t port_num);
	struct ibv_ah *(*ibv_create_ah)(struct ibv_pd *pd,
					struct ibv_ah_attr *attr);
	struct ibv_comp_channel *(*ibv_create_comp_channel)(
		struct ibv_context *context);
	struct ibv_cq *(*ibv_create_cq)(struct ibv_context *context, int cqe,
					void *cq_context,
					struct ibv_comp_channel *channel,
					int comp_vector);
	struct ibv_qp *(*ibv_create_qp)(struct ibv_pd *pd,
					struct ibv_qp_init_attr *qp_init_attr);
	struct ibv_srq *(*ibv_create_srq)(
		struct ibv_pd *pd, struct ibv_srq_init_attr *srq_init_attr);
	int (*ibv_dealloc_pd)(struct ibv_pd *pd);
	int (*ibv_dereg_mr)(struct ibv_mr *mr);
	int (*ibv_destroy_ah)(struct ibv_ah *ah);
	int (*ibv_destroy_comp_channel)(struct ibv_comp_channel *channel);
	int (*ibv_destroy_cq)(struct ibv_cq *cq);
	int (*ibv_destroy_qp)(struct ibv_qp *qp);
	int (*ibv_destroy_srq)(struct ibv_srq *srq);
	int (*ibv_detach_mcast)(struct ibv_qp *qp, const union ibv_gid *gid,
				uint16_t lid);
	const char *(*ibv_event_type_str)(enum ibv_event_type event);
	int (*ibv_fork_init)(void);
	void (*ibv_free_device_list)(struct ibv_device **list);
	int (*ibv_get_async_event)(struct ibv_context *context,
				   struct ibv_async_event *event);
	int (*ibv_get_cq_event)(struct ibv_comp_channel *channel,
				struct ibv_cq **cq, void **cq_context);
	__be64 (*ibv_get_device_guid)(struct ibv_device *device);
	struct ibv_device **(*ibv_get_device_list)(int *num_devices);
	const char *(*ibv_get_device_name)(struct ibv_device *device);
	int (*ibv_init_ah_from_wc)(struct ibv_context *context,
				   uint8_t port_num, struct ibv_wc *wc,
				   struct ibv_grh *grh,
				   struct ibv_ah_attr *ah_attr);
	int (*ibv_modify_qp)(struct ibv_qp *qp, struct ibv_qp_attr *attr,
			     int attr_mask);
	int (*ibv_modify_srq)(struct ibv_srq *srq,
			      struct ibv_srq_attr *srq_attr, int srq_attr_mask);
	const char *(*ibv_node_type_str)(enum ibv_node_type node_type);
	struct ibv_context *(*ibv_open_device)(struct ibv_device *device);
	const char *(*ibv_port_state_str)(enum ibv_port_state port_state);
	int (*ibv_query_device)(struct ibv_context *context,
				struct ibv_device_attr *device_attr);
	int (*ibv_query_gid)(struct ibv_context *context, uint8_t port_num,
			     int index, union ibv_gid *gid);
	int (*ibv_query_pkey)(struct ibv_context *context, uint8_t port_num,
			      int index, __be16 *pkey);
	/* Compatability #define wrapper for query_port is safely ignored. */
	int (*ibv_query_port)(struct ibv_context *context, uint8_t port_num,
			      struct ibv_port_attr *port_attr);

	int (*ibv_query_qp)(struct ibv_qp *qp, struct ibv_qp_attr *attr,
			    int attr_mask, struct ibv_qp_init_attr *init_attr);

	int (*ibv_query_srq)(struct ibv_srq *srq,
			     struct ibv_srq_attr *srq_attr);
	int (*ibv_rate_to_mbps)(enum ibv_rate rate);
	int (*ibv_rate_to_mult)(enum ibv_rate rate);
	struct ibv_mr *(*ibv_reg_mr)(struct ibv_pd *pd, void *addr,
				     size_t length, int access);
	int (*ibv_rereg_mr)(struct ibv_mr *mr, int flags, struct ibv_pd *pd,
			    void *addr, size_t length, int access);
	int (*ibv_resize_cq)(struct ibv_cq *cq, int cqe);
	int (*ibv_resolve_eth_l2_from_gid)(struct ibv_context *context,
					   struct ibv_ah_attr *attr,
					   uint8_t eth_mac[ETHERNET_LL_SIZE],
					   uint16_t *vid);
	const char *(*ibv_wc_status_str)(enum ibv_wc_status status);
	enum ibv_rate (*mbps_to_ibv_rate)(int mbps);
	enum ibv_rate (*mult_to_ibv_rate)(int mult);
};

extern struct dyn_verbs_ops _verbs_ops;

/*
 * Work around the fallback call to query_device. We know mlx4/5 implement
 * query_device_ex so just call it with no fallback.
 */
static inline int
_dyn_ibv_query_device_ex(struct ibv_context *context,
			 const struct ibv_query_device_ex_input *input,
			 struct ibv_device_attr_ex *attr)
{
	struct verbs_context *vctx;

	vctx = verbs_get_ctx_op(context, query_device_ex);
	if (!vctx)
		return ENOSYS;

	return vctx->query_device_ex(context, input, attr, sizeof(*attr));
}

/*
 * Generated with:
 *   readelf -s --wide build/lib/libibverbs.so | grep @@IBVERBS | grep -v PRIVATE | awk '{split($8,a,"@@"); printf("#define %s(...) (_verbs_ops.%s(__VA_ARGS__))\n",a[1],a[1])}' | sort -u
 */
#ifndef _DYN_VERBS_NO_WRAPPERS
#define ibv_ack_async_event(...) (_verbs_ops.ibv_ack_async_event(__VA_ARGS__))
#define ibv_ack_cq_events(...) (_verbs_ops.ibv_ack_cq_events(__VA_ARGS__))
#define ibv_alloc_pd(...) (_verbs_ops.ibv_alloc_pd(__VA_ARGS__))
#define ibv_attach_mcast(...) (_verbs_ops.ibv_attach_mcast(__VA_ARGS__))
#define ibv_close_device(...) (_verbs_ops.ibv_close_device(__VA_ARGS__))
#define ibv_create_ah_from_wc(...) (_verbs_ops.ibv_create_ah_from_wc(__VA_ARGS__))
#define ibv_create_ah(...) (_verbs_ops.ibv_create_ah(__VA_ARGS__))
#define ibv_create_comp_channel(...) (_verbs_ops.ibv_create_comp_channel(__VA_ARGS__))
#define ibv_create_cq(...) (_verbs_ops.ibv_create_cq(__VA_ARGS__))
#define ibv_create_qp(...) (_verbs_ops.ibv_create_qp(__VA_ARGS__))
#define ibv_create_srq(...) (_verbs_ops.ibv_create_srq(__VA_ARGS__))
#define ibv_dealloc_pd(...) (_verbs_ops.ibv_dealloc_pd(__VA_ARGS__))
#define ibv_dereg_mr(...) (_verbs_ops.ibv_dereg_mr(__VA_ARGS__))
#define ibv_destroy_ah(...) (_verbs_ops.ibv_destroy_ah(__VA_ARGS__))
#define ibv_destroy_comp_channel(...) (_verbs_ops.ibv_destroy_comp_channel(__VA_ARGS__))
#define ibv_destroy_cq(...) (_verbs_ops.ibv_destroy_cq(__VA_ARGS__))
#define ibv_destroy_qp(...) (_verbs_ops.ibv_destroy_qp(__VA_ARGS__))
#define ibv_destroy_srq(...) (_verbs_ops.ibv_destroy_srq(__VA_ARGS__))
#define ibv_detach_mcast(...) (_verbs_ops.ibv_detach_mcast(__VA_ARGS__))
#define ibv_event_type_str(...) (_verbs_ops.ibv_event_type_str(__VA_ARGS__))
#define ibv_fork_init(...) (_verbs_ops.ibv_fork_init(__VA_ARGS__))
#define ibv_free_device_list(...) (_verbs_ops.ibv_free_device_list(__VA_ARGS__))
#define ibv_get_async_event(...) (_verbs_ops.ibv_get_async_event(__VA_ARGS__))
#define ibv_get_cq_event(...) (_verbs_ops.ibv_get_cq_event(__VA_ARGS__))
#define ibv_get_device_guid(...) (_verbs_ops.ibv_get_device_guid(__VA_ARGS__))
#define ibv_get_device_list(...) (_verbs_ops.ibv_get_device_list(__VA_ARGS__))
#define ibv_get_device_name(...) (_verbs_ops.ibv_get_device_name(__VA_ARGS__))
#define ibv_init_ah_from_wc(...) (_verbs_ops.ibv_init_ah_from_wc(__VA_ARGS__))
#define ibv_modify_qp(...) (_verbs_ops.ibv_modify_qp(__VA_ARGS__))
#define ibv_modify_srq(...) (_verbs_ops.ibv_modify_srq(__VA_ARGS__))
#define ibv_node_type_str(...) (_verbs_ops.ibv_node_type_str(__VA_ARGS__))
#define ibv_open_device(...) (_verbs_ops.ibv_open_device(__VA_ARGS__))
#define ibv_port_state_str(...) (_verbs_ops.ibv_port_state_str(__VA_ARGS__))
#define ibv_query_device(...) (_verbs_ops.ibv_query_device(__VA_ARGS__))
#define ibv_query_device_ex(...) (_dyn_ibv_query_device_ex(__VA_ARGS__))
#define ibv_query_gid(...) (_verbs_ops.ibv_query_gid(__VA_ARGS__))
#define ibv_query_pkey(...) (_verbs_ops.ibv_query_pkey(__VA_ARGS__))
#define ibv_query_port(...) (_verbs_ops.ibv_query_port(__VA_ARGS__))
#define ibv_query_qp(...) (_verbs_ops.ibv_query_qp(__VA_ARGS__))
#define ibv_query_srq(...) (_verbs_ops.ibv_query_srq(__VA_ARGS__))
#define ibv_rate_to_mbps(...) (_verbs_ops.ibv_rate_to_mbps(__VA_ARGS__))
#define ibv_rate_to_mult(...) (_verbs_ops.ibv_rate_to_mult(__VA_ARGS__))
#define ibv_reg_mr(...) (_verbs_ops.ibv_reg_mr(__VA_ARGS__))
#define ibv_rereg_mr(...) (_verbs_ops.ibv_rereg_mr(__VA_ARGS__))
#define ibv_resize_cq(...) (_verbs_ops.ibv_resize_cq(__VA_ARGS__))
#define ibv_resolve_eth_l2_from_gid(...) (_verbs_ops.ibv_resolve_eth_l2_from_gid(__VA_ARGS__))
#define ibv_wc_status_str(...) (_verbs_ops.ibv_wc_status_str(__VA_ARGS__))
#define mbps_to_ibv_rate(...) (_verbs_ops.mbps_to_ibv_rate(__VA_ARGS__))
#define mult_to_ibv_rate(...) (_verbs_ops.mult_to_ibv_rate(__VA_ARGS__))
#endif

int dl_link_init(void);

#endif
