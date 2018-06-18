/*
 * Copyright (c) 2006-2016 Chelsio, Inc. All rights reserved.
 * Copyright (c) 2005 Topspin Communications.  All rights reserved.
 * Copyright (c) 2005, 2006 Cisco Systems.  All rights reserved.
 * Copyright (c) 2005 PathScale, Inc.  All rights reserved.
 * Copyright (c) 2006 Mellanox Technologies.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _RDMA_CORE_COMPAT_ABI_H
#define _RDMA_CORE_COMPAT_ABI_H

/*
 * Kernel commit c6d7b26791a2 ("RDMA/cxgb4: Support on-chip SQs")
 * changed the cxgb4 ABI from 0 to 1, this struct is from
 * c6d7b26791a2^:drivers/infiniband/hw/cxgb4/user.h
 */
struct c4iw_create_qp_resp_v0 {
	__u64 sq_key;
	__u64 rq_key;
	__u64 sq_db_gts_key;
	__u64 rq_db_gts_key;
	__u64 sq_memsize;
	__u64 rq_memsize;
	__u32 sqid;
	__u32 rqid;
	__u32 sq_size;
	__u32 rq_size;
	__u32 qid_mask;
};

/*
 * Kernel commit ea88fd16d6e8 ("IB/uverbs: Return actual capacity from create
 * SRQ operation") changed the uverbs ABI from 5 to 6, this struct is from
 * ea88fd16d6e8^:include/rdma/ib_user_verbs.h
 */
struct ib_uverbs_create_srq_resp_v5 {
	__u32 srq_handle;
};

/*
 * Kernel commit 4d9781c5ce1a ("IB/uverbs: Fix alignment of struct
 * ib_uverbs_create_qp_resp") changed the uverbs ABI from 4 to 5, this struct
 * is from 4d9781c5ce1a^:include/rdma/ib_user_verbs.h
 */
struct ib_uverbs_create_qp_resp_v4 {
	__u32 qp_handle;
	__u32 qpn;
	__u32 max_send_wr;
	__u32 max_recv_wr;
	__u32 max_send_sge;
	__u32 max_recv_sge;
	__u32 max_inline_data;
};

/*
 * Kernel commit 77369ed31daa ("[IB] uverbs: have kernel return QP
 * capabilities") changed the uverbs ABI from 3 to 4, these structs are from
 * 77369ed31daa^:include/rdma/ib_user_verbs.h
 */
struct ib_uverbs_create_qp_resp_v3 {
	__u32 qp_handle;
	__u32 qpn;
};

/*
 * WARNING: This structure is *smaller* than our current standard
 * structure!
 */
struct ib_uverbs_modify_srq_v3 {
	__u32 srq_handle;
	__u32 attr_mask;
	__u32 max_wr;
	__u32 max_sge;
	__u32 srq_limit;
	__u32 reserved;
	__u64 driver_data[0];
};

#endif
