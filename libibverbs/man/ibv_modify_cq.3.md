---
date: 2017-10-20
footer: libibverbs
header: "Libibverbs Programmer's Manual"
layout: page
license: 'Licensed under the OpenIB.org BSD license (FreeBSD Variant) - See COPYING.md'
section: 3
title: IBV_MODIFY_CQ
---

# NAME

ibv_modify_cq - modify a completion queue (CQ)

# SYNOPSIS

```c
#include <infiniband/verbs.h>

int ibv_modify_cq(struct ibv_cq *cq,
                  struct ibv_modify_cq_attr *cq_attr);
```

# DESCRIPTION

**ibv_modify_cq()** modify a CQ *cq*. The argument *cq_attr* is an
ibv_modify_cq_attr struct, as defined in <infiniband/verbs.h>.


```c
struct ibv_moderate_cq {
        uint16_t cq_count;  /* number of completions per event */
        uint16_t cq_period; /* in micro seconds */
};

struct ibv_modify_cq_attr {
        uint32_t               attr_mask;
        struct ibv_moderate_cq moderate;
};
```


The function **ibv_modify_cq()** will modify the CQ, based on the given
*cq_attr*->attr_mask

# RETURN VALUE

returns 0 on success, or the value of errno on failure (which indicates the
failure reason).

# SEE ALSO

**ibv_create_cq**(3)

# AUTHOR

Yonatan Cohen <yonatanc@mellanox.com>
