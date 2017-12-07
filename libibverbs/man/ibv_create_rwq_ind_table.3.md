---
date: ' 2016-07-27'
footer: libibverbs
header: "Libibverbs Programmer's Manual"
layout: page
license: 'Licensed under the OpenIB.org BSD license (FreeBSD Variant) - See COPYING.md'
section: 3
title: CREATE_RWQ_IND_TBL
---

# NAME

ibv_create_rwq_ind_table, ibv_destroy_rwq_ind_table  - create or destroy a
Receive Work Queue Indirection Table (RWQ IND TBL).

# SYNOPSIS

```c
#include <infiniband/verbs.h>

struct ibv_rwq_ind_table *ibv_create_rwq_ind_table(
    struct ibv_context *context,
    struct ibv_rwq_ind_table_init_attr *init_attr);

int ibv_destroy_rwq_ind_table(
    struct ibv_rwq_ind_table *rwq_ind_table);
```

# DESCRIPTION

**ibv_create_rwq_ind_table()** creates a RWQ IND TBL associated with the
ibv_context *context*. The argument *init_attr* is an
ibv_rwq_ind_table_init_attr struct, as defined in <infiniband/verbs.h>.


```c
struct ibv_rwq_ind_table_init_attr {
        uint32_t log_ind_tbl_size; /* Log, base 2, of Indirection
                                      table size */
        struct ibv_wq **ind_tbl;   /* Each entry is a pointer to
                                      Receive Work Queue */
        uint32_t comp_mask;        /* Identifies valid fields. Use
                                      ibv_ind_table_init_attr_mask */
};
```


The function **ibv_create_rwq_ind_table()** will create a RWQ IND TBL that
holds a table of Receive Work Queue. For further usage of the created object
see below *NOTES*.

**ibv_destroy_rwq_ind_table()** destroys the RWQ IND TBL *rwq_ind_table*.

# RETURN VALUE

**ibv_create_rwq_ind_table()** returns a pointer to the created RWQ IND TBL,
or NULL if the request fails.

**ibv_destroy_rwq_ind_table()** returns 0 on success, or the value of errno on
failure (which indicates the failure reason).

# NOTES

The created object should be used as part of *ibv_create_qp_ex()* to enable
dispatching of incoming packets based on some RX hash configuration.

# SEE ALSO

**ibv_create_qp_ex**(3),
**ibv_create_wq**(3),
**ibv_modify_wq**(3)

# AUTHOR

Yishai Hadas <yishaih@mellanox.com>
