---
date: 2006-10-31
footer: libibverbs
header: "Libibverbs Programmer's Manual"
layout: page
license: 'Licensed under the OpenIB.org BSD license (FreeBSD Variant) - See COPYING.md'
section: 3
title: IBV_POLL_CQ
---

# NAME

ibv_poll_cq - poll a completion queue (CQ)

# SYNOPSIS

```c
#include <infiniband/verbs.h>

int ibv_poll_cq(struct ibv_cq *cq,
                int num_entries,
                struct ibv_wc *wc);
```

# DESCRIPTION

**ibv_poll_cq()** polls the CQ *cq* for work completions and returns the first
*num_entries* (or all available completions if the CQ contains fewer than this
number) in the array *wc*. The argument *wc* is a pointer to an array of
ibv_wc structs, as defined in <infiniband/verbs.h>.


```c
struct ibv_wc {
        uint64_t
                           wr_id; /* ID of the completed Work Request (WR) */
        enum ibv_wc_status status; /* Status of the operation */
        enum ibv_wc_opcode
            opcode; /* Operation type specified in the completed
                       WR */
        uint32_t vendor_err; /* Vendor error syndrome */
        uint32_t byte_len;   /* Number of bytes transferred */
        union {
                __be32 imm_data; /* Immediate data (in network
                                    byte order) */
                uint32_t
                    invalidated_rkey; /* Local RKey that was
                                         invalidated */
        };
        uint32_t qp_num; /* Local QP number of completed WR */
        uint32_t
            src_qp;   /* Source QP number (remote QP number) of
                         completed WR (valid only for UD QPs) */
        int wc_flags; /* Flags of the completed WR */
        uint16_t
                 pkey_index; /* P_Key index (valid only for GSI QPs) */
        uint16_t slid;       /* Source LID */
        uint8_t  sl;         /* Service Level */
        uint8_t  dlid_path_bits; /* DLID path bits (not applicable
                                    for multicast messages) */
};
```


The attribute wc_flags describes the properties of the work completion. It is
either 0 or the bitwise OR of one or more of the following flags:

**IBV_WC_GRH **
:	GRH is present (valid only for UD QPs)

**IBV_WC_WITH_IMM **
:	Immediate data value is valid

**IBV_WC_WITH_INV **
:	Invalidated RKey data value is valid (cannot be combined with
	IBV_WC_WITH_IMM)

**IBV_WC_IP_CSUM_OK **
:	TCP/UDP checksum over IPv4 and IPv4 header checksum are verified.
	Valid only when **device_cap_flags** in device_attr indicates current
	QP is supported by checksum offload.

Not all *wc* attributes are always valid. If the completion status is other
than **IBV_WC_SUCCESS**, only the following attributes are valid: wr_id,
status, qp_num, and vendor_err.

# RETURN VALUE

On success, **ibv_poll_cq()** returns a non-negative value equal to the number
of completions found.  On failure, a negative value is returned.

# NOTES


Each polled completion is removed from the CQ and cannot be returned to it.

The user should consume work completions at a rate that prevents CQ overrun
from occurrence.  In case of a CQ overrun, the async event
**IBV_EVENT_CQ_ERR** will be triggered, and the CQ cannot be used.

# SEE ALSO

**ibv_post_recv**(3),
**ibv_post_send**(3)

# AUTHOR

Dotan Barak <dotanba@gmail.com>
