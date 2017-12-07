---
date: 2006-10-31
footer: libibverbs
header: "Libibverbs Programmer's Manual"
layout: page
license: 'Licensed under the OpenIB.org BSD license (FreeBSD Variant) - See COPYING.md'
section: 3
title: IBV_CREATE_CQ
---

# NAME

ibv_create_cq, ibv_destroy_cq - create or destroy a completion queue (CQ)

# SYNOPSIS

```c
#include <infiniband/verbs.h>

struct ibv_cq *ibv_create_cq(struct ibv_context *context,
                             int cqe,
                             void *cq_context,
                             struct ibv_comp_channel *channel,
                             int comp_vector);

int ibv_destroy_cq(struct ibv_cq *cq);
```

# DESCRIPTION

**ibv_create_cq()** creates a completion queue (CQ) with at least *cqe*
entries for the RDMA device context *context*. The pointer *cq_context* will
be used to set user context pointer of the CQ structure. The argument
*channel* is optional; if not NULL, the completion channel *channel* will be
used to return completion events.  The CQ will use the completion vector
*comp_vector* for signaling completion events; it must be at least zero and
less than *context*->num_comp_vectors.

**ibv_destroy_cq()** destroys the CQ *cq*.

# RETURN VALUE

**ibv_create_cq()** returns a pointer to the CQ, or NULL if the request fails.

**ibv_destroy_cq()** returns 0 on success, or the value of errno on failure
(which indicates the failure reason).

# NOTES

**ibv_create_cq()** may create a CQ with size greater than or equal to the
requested size. Check the cqe attribute in the returned CQ for the actual
size.

**ibv_destroy_cq()** fails if any queue pair is still associated with this CQ.

# SEE ALSO

**ibv_ack_cq_events**(3),
**ibv_create_qp**(3),
**ibv_req_notify_cq**(3),
**ibv_resize_cq**(3)

# AUTHOR

Dotan Barak <dotanba@gmail.com>
