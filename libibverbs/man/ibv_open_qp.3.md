---
date: 2011-08-12
footer: libibverbs
header: "Libibverbs Programmer's Manual"
layout: page
license: 'Licensed under the OpenIB.org BSD license (FreeBSD Variant) - See COPYING.md'
section: 3
title: IBV_OPEN_QP
---

# NAME

ibv_open_qp - open a shareable queue pair (QP)

# SYNOPSIS

```c
#include <infiniband/verbs.h>

struct ibv_qp *ibv_open_qp(struct ibv_context *context,
                           struct ibv_qp_open_attr *qp_open_attr);
```

# DESCRIPTION

**ibv_open_qp()** opens an existing queue pair (QP) associated with the
extended protection domain *xrcd*. The argument *qp_open_attr* is an
ibv_qp_open_attr struct, as defined in <infiniband/verbs.h>.


```c
struct ibv_qp_open_attr {
        uint32_t comp_mask;       /* Identifies valid fields */
        uint32_t qp_num;          /* QP number */
        struct * ibv_xrcd;        /* XRC domain */
        void *   qp_context;      /* User defined opaque value */
        enum ibv_qp_type qp_type; /* QP transport service type */
};
```


**ibv_destroy_qp()** closes the opened QP and destroys the underlying QP if it
has no other references. *qp*.

# RETURN VALUE

**ibv_open_qp()** returns a pointer to the opened QP, or NULL if the request
fails. Check the QP number (**qp_num**) in the returned QP.

# NOTES

**ibv_open_qp()** will fail if a it is asked to open a QP that does not exist
within the xrcd with the specified qp_num and qp_type.

# SEE ALSO

**ibv_alloc_pd**(3),
**ibv_create_qp**(3),
**ibv_create_qp_ex**(3),
**ibv_modify_qp**(3),
**ibv_query_qp**(3)

# AUTHOR

Sean Hefty <sean.hefty@intel.com>
