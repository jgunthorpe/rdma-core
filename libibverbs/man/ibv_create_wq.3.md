---
date: 2016-07-27
footer: libibverbs
header: "Libibverbs Programmer's Manual"
layout: page
license: 'Licensed under the OpenIB.org BSD license (FreeBSD Variant) - See COPYING.md'
section: 3
title: IBV_CREATE_WQ
---

# NAME

ibv_create_wq, ibv_destroy_wq - create or destroy a Work Queue (WQ).

# SYNOPSIS

```c
#include <infiniband/verbs_exp.h>

struct ibv_wq *
ibv_create_wq(struct ibv_context *context,
              struct ibv_wq_init_attr *wq_init_attr);

int ibv_destroy_wq(struct ibv_wq *wq);
```

# DESCRIPTION

**ibv_create_wq()** creates a WQ associated with the ibv_context *context*.
The argument *wq_init_attr* is an ibv_wq_init_attr struct, as defined in
<infiniband/verbs.h>.


```c
struct ibv_wq_init_attr {
        void *wq_context; /* Associated context of the WQ */
        enum ibv_wq_type wq_type; /* WQ type */
        uint32_t max_wr; /* Requested max number of outstanding
                            WRs in the WQ */
        uint32_t
            max_sge; /* Requested max number of scatter/gather
                        (s/g) elements per WR in the WQ */
        struct ibv_pd *pd; /* PD to be associated with the WQ */
        struct ibv_cq *cq; /* CQ to be associated with the WQ */
        uint32_t       comp_mask; /* Identifies valid fields. Use
                                     ibv_wq_init_attr_mask */
        uint32_t create_flags /* Creation flags for this WQ, use
                                 enum ibv_wq_flags */
};

enum ibv_wq_flags {
        IBV_WQ_FLAGS_CVLAN_STRIPPING = 1
                                       << 0, /* CVLAN field will
                                                be stripped from
                                                incoming packets
                                              */
        IBV_WQ_FLAGS_SCATTER_FCS =
            1
            << 1, /* FCS field will be scattered to host memory */
        IBV_WQ_FLAGS_DELAY_DROP = 1 << 2, /* Packets won't be
                                             dropped immediately
                                             if no receive WQEs */
        IBV_WQ_FLAGS_PCI_WRITE_END_PADDING =
            1 << 3, /* Incoming packets will be padded to
                       cacheline size */
        IBV_WQ_FLAGS_RESERVED = 1 << 4,
};
```


The function **ibv_create_wq()** will update the *wq_init_attr*->max_wr and
*wq_init_attr*->max_sge fields with the actual WQ values of the WQ that was
created; the values will be greater than or equal to the values requested.

**ibv_destroy_wq()** destroys the WQ *wq*.

# RETURN VALUE

**ibv_create_wq()** returns a pointer to the created WQ, or NULL if the
request fails.

**ibv_destroy_wq()** returns 0 on success, or the value of errno on failure
(which indicates the failure reason).

# SEE ALSO

**ibv_modify_wq**(3)

# AUTHOR

Yishai Hadas <yishaih@mellanox.com>
