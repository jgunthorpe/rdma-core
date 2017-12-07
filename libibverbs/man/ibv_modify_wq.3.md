---
date: 2016-07-27
footer: libibverbs
header: "Libibverbs Programmer's Manual"
layout: page
license: 'Licensed under the OpenIB.org BSD license (FreeBSD Variant) - See COPYING.md'
section: 3
title: IBV_MODIFY_WQ
---

# NAME

ibv_modify_wq - Modify a Work Queue (WQ).

# SYNOPSIS

```c
#include <infiniband/verbs.h>

struct int ibv_modify_wq(struct ibv_wq *wq,
                         struct ibv_wq_attr *wq_attr);
```

# DESCRIPTION

**ibv_modify_wq()** modifys a WQ *wq*. The argument *wq_attr* is an
ibv_wq_attr struct, as defined in <infiniband/verbs.h>.


```c
struct ibv_wq_attr {
        uint32_t attr_mask; /* Use enum ibv_wq_attr_mask */
        enum ibv_wq_state wq_state; /* Move to this state */
        enum ibv_wq_state
                 curr_wq_state; /* Assume this is the current state */
        uint32_t flags;      /* Flags values to modify, use enum
                                ibv_wq_flags */
        uint32_t flags_mask; /* Which flags to modify, use enum
                                ibv_wq_flags */
};
```


The function **ibv_modify_wq()** will modify the WQ based on the given
*wq_attr*->attr_mask

# RETURN VALUE

returns 0 on success, or the value of errno on failure (which indicates the
failure reason).

# SEE ALSO

**ibv_create_wq**(3),
**ibv_destroy_wq**(3)

# AUTHOR

Yishai Hadas <yishaih@mellanox.com>
