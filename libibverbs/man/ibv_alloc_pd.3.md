---
date: 2006-10-31
footer: libibverbs
header: "Libibverbs Programmer's Manual"
layout: page
license: 'Licensed under the OpenIB.org BSD license (FreeBSD Variant) - See COPYING.md'
section: 3
title: IBV_ALLOC_PD
---

# NAME

ibv_alloc_pd, ibv_dealloc_pd - allocate or deallocate a protection domain
(PDs)

# SYNOPSIS

```c
#include <infiniband/verbs.h>

struct ibv_pd *ibv_alloc_pd(struct ibv_context *context);

int ibv_dealloc_pd(struct ibv_pd *pd);
```

# DESCRIPTION

**ibv_alloc_pd()** allocates a PD for the RDMA device context *context*.

**ibv_dealloc_pd()** deallocates the PD *pd*.

# RETURN VALUE

**ibv_alloc_pd()** returns a pointer to the allocated PD, or NULL if the
request fails.

**ibv_dealloc_pd()** returns 0 on success, or the value of errno on failure
(which indicates the failure reason).

# NOTES

**ibv_dealloc_pd()** may fail if any other resource is still associated with
the PD being freed.

# SEE ALSO

**ibv_create_ah**(3),
**ibv_create_ah_from_wc**(3),
**ibv_create_qp**(3),
**ibv_create_srq**(3),
**ibv_reg_mr**(3)

# AUTHOR

Dotan Barak <dotanba@gmail.com>
