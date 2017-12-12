---
date: 2017-06-27
footer: 1.0.0
layout: page
license: 'Licensed under the OpenIB.org (MIT) - See COPYING.md'
section: 3
title: MLX4DV_QUERY_DEVICE
---

# NAME

mlx4dv_query_device - Query device capabilities specific to mlx4

# SYNOPSIS

```c
#include <infiniband/mlx4dv.h>

int mlx4dv_query_device(struct ibv_context *ctx_in,
                        struct mlx4dv_context *attrs_out);
```

# DESCRIPTION

**mlx4dv_query_device()** Query mlx4 specific device information that is
usable via the direct verbs interface.

This function returns a version and compatibility mask. The version represents
the format of the internal hardware structures that mlx4dv.h exposes. Future
additions of new fields to the existing structures are handled by the
comp_mask field.


```c
struct mlx4dv_context {
        uint8_t  version;
        uint32_t max_inl_recv_sz; /* Maximum supported size of
                                     inline receive */
        uint64_t comp_mask;
};
```


# RETURN VALUE

0 on success or the value of errno on failure (which indicates the failure
reason).

# NOTES

* Compatibility mask (comp_mask) is an in/out field.

# SEE ALSO

**ibv_query_device**(3),
**mlx4dv**(7)

# AUTHOR

Maor Gottlieb <maorg@mellanox.com>
