---
date: 2006-10-31
footer: libibverbs
header: "Libibverbs Programmer's Manual"
layout: page
license: 'Licensed under the OpenIB.org BSD license (FreeBSD Variant) - See COPYING.md'
section: 3
title: IBV_QUERY_SRQ
---

# NAME

ibv_query_srq - get the attributes of a shared receive queue (SRQ)

# SYNOPSIS

```c
#include <infiniband/verbs.h>

int ibv_query_srq(struct ibv_srq *srq,
                  struct ibv_srq_attr *srq_attr);
```

# DESCRIPTION

**ibv_query_srq()** gets the attributes of the SRQ *srq* and returns them
through the pointer *srq_attr*. The argument *srq_attr* is an ibv_srq_attr
struct, as defined in <infiniband/verbs.h>.


```c
struct ibv_srq_attr {
        uint32_t max_wr;    /* maximum number of outstanding work
                               requests (WRs) in the SRQ */
        uint32_t max_sge;   /* maximum number of scatter elements
                               per WR */
        uint32_t srq_limit; /* the limit value of the SRQ */
};
```


# RETURN VALUE

**ibv_query_srq()** returns 0 on success, or the value of errno on failure
(which indicates the failure reason).

# NOTES

If the value returned for srq_limit is 0, then the SRQ limit reached ("low
watermark") event is not (or no longer) armed, and no asynchronous events will
be generated until the event is rearmed.

# SEE ALSO

**ibv_create_srq**(3),
**ibv_destroy_srq**(3),
**ibv_modify_srq**(3)

# AUTHOR

Dotan Barak <dotanba@gmail.com>
