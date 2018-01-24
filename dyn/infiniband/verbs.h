/*
 * This shim header replaces infiniband/verbs.h in the gcc search path, then
 * include_next lets us get the system header.
 */
#include_next <infiniband/verbs.h>

#ifndef _INFINIBAND_DYN_VERBS_H
#define _INFINIBAND_DYN_VERBS_H

#undef ibv_query_port

#define DECLARE_FNP(_fn) __typeof__(&_fn) _fn
struct dyn_verbs_ops {
	DECLARE_FNP(ibv_ack_async_event);
	DECLARE_FNP(ibv_ack_cq_events);
	DECLARE_FNP(ibv_alloc_pd);
	DECLARE_FNP(ibv_attach_mcast);
	DECLARE_FNP(ibv_close_device);
	DECLARE_FNP(ibv_create_ah_from_wc);
	DECLARE_FNP(ibv_create_ah);
	DECLARE_FNP(ibv_create_comp_channel);
	DECLARE_FNP(ibv_create_cq);
	DECLARE_FNP(ibv_create_qp);
	DECLARE_FNP(ibv_create_srq);
	DECLARE_FNP(ibv_dealloc_pd);
	DECLARE_FNP(ibv_dereg_mr);
	DECLARE_FNP(ibv_destroy_ah);
	DECLARE_FNP(ibv_destroy_comp_channel);
	DECLARE_FNP(ibv_destroy_cq);
	DECLARE_FNP(ibv_destroy_qp);
	DECLARE_FNP(ibv_destroy_srq);
	DECLARE_FNP(ibv_detach_mcast);
	DECLARE_FNP(ibv_event_type_str);
	DECLARE_FNP(ibv_fork_init);
	DECLARE_FNP(ibv_free_device_list);
	DECLARE_FNP(ibv_get_async_event);
	DECLARE_FNP(ibv_get_cq_event);
	DECLARE_FNP(ibv_get_device_guid);
	DECLARE_FNP(ibv_get_device_list);
	DECLARE_FNP(ibv_get_device_name);
	DECLARE_FNP(ibv_init_ah_from_wc);
	DECLARE_FNP(ibv_modify_qp);
	DECLARE_FNP(ibv_modify_srq);
	DECLARE_FNP(ibv_node_type_str);
	DECLARE_FNP(ibv_open_device);
	DECLARE_FNP(ibv_port_state_str);
	DECLARE_FNP(ibv_query_device);
	DECLARE_FNP(ibv_query_gid);
	DECLARE_FNP(ibv_query_pkey);
	DECLARE_FNP(ibv_query_port);
	DECLARE_FNP(ibv_query_qp);
	DECLARE_FNP(ibv_query_srq);
	DECLARE_FNP(ibv_rate_to_mbps);
	DECLARE_FNP(ibv_rate_to_mult);
	DECLARE_FNP(ibv_reg_mr);
	DECLARE_FNP(ibv_rereg_mr);
	DECLARE_FNP(ibv_resize_cq);
	DECLARE_FNP(ibv_resolve_eth_l2_from_gid);
	DECLARE_FNP(ibv_wc_status_str);
	DECLARE_FNP(mbps_to_ibv_rate);
	DECLARE_FNP(mult_to_ibv_rate);
};
#undef DECLARE_FNP

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
