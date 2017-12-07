---
date: 2016-2-20
footer: libibverbs
header: "Libibverbs Programmer's Manual"
layout: page
license: 'Licensed under the OpenIB.org BSD license (FreeBSD Variant) - See COPYING.md'
section: 3
title: IBV_QUERY_RT_VALUES_EX
---

# NAME

ibv_query_rt_values_ex - query an RDMA device for some real time values

# SYNOPSIS

```c
#include <infiniband/verbs.h>

int ibv_query_rt_values_ex(struct ibv_context *context,
                           struct ibv_values_ex *values);
```

# DESCRIPTION

**ibv_query_rt_values_ex()** returns certain real time values of a device
*context*. The argument *attr* is a pointer to an ibv_device_attr_ex struct,
as defined in <infiniband/verbs.h>.


```c
struct ibv_values_ex {
        uint32_t
            comp_mask; /* Compatibility mask that defines the
                          query/queried fields [in/out] */
        struct timespec raw_clock; /* HW raw clock */
};

enum ibv_values_mask {
        IBV_VALUES_MASK_RAW_CLOCK = 1 << 0, /* HW raw clock */
};
```


# RETURN VALUE

**ibv_query_rt_values_ex()** returns 0 on success, or the value of errno on
failure (which indicates the failure reason).

# NOTES

This extension verb only calls the provider, the provider has to query this
value somehow and mark the queried values in the comp_mask field.

# SEE ALSO

**ibv_open_device**(3),
**ibv_query_device**(3),
**ibv_query_gid**(3),
**ibv_query_pkey**(3),
**ibv_query_port**(3)

# AUTHORS

Matan Barak <matanb@mellanox.com>,
Yishai Hadas <yishaih@mellanox.com>
