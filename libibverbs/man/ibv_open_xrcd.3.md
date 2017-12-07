---
date: 2011-06-17
footer: libibverbs
header: "Libibverbs Programmer's Manual"
layout: page
license: 'Licensed under the OpenIB.org BSD license (FreeBSD Variant) - See COPYING.md'
section: 3
title: IBV_OPEN_XRCD
---

# NAME

ibv_open_xrcd, ibv_close_xrcd - open or close an XRC protection domain (XRCDs)

# SYNOPSIS

```c
#include <infiniband/verbs.h>

struct ibv_xrcd *
ibv_open_xrcd(struct ibv_context *context,
              struct ibv_xrcd_init_attr *xrcd_init_attr);

int ibv_close_xrcd(struct ibv_xrcd *xrcd);
```

# DESCRIPTION

**ibv_open_xrcd()** open an XRC domain for the RDMA device context *context*
*xrcd_init_attr* is an ibv_xrcd_init_attr struct, as defined in
<infiniband/verbs.h>.


```c
struct ibv_xrcd_init_attr {
        uint32_t comp_mask; /* Identifies valid fields */
        int      fd;
        int      oflag;
};
```


*fd* is the file descriptor to associate with the XRCD. *oflag* describes the
desired creation attributes.  It is a bitwise OR of zero or more of the
following flags:

**O_CREAT**
:	Indicates that an XRCD should be created and associated with the inode
	referenced by the given fd.  If the XRCD exists, this flag has no
	effect except as noted under **O_EXCL** below.

**O_EXCL**
:	If **O_EXCL** and **O_CREAT** are set, open will fail if an XRCD
	associated with the inode exists.

If *fd* equals -1, no inode is associated with the XRCD. To indicate that XRCD
should be created, use *oflag* = **O_CREAT**.

**ibv_close_xrcd()** closes the XRCD *xrcd*. If this is the last reference,
the XRCD will be destroyed.

# RETURN VALUE

**ibv_open_xrcd()** returns a pointer to the opened XRCD, or NULL if the
request fails.

**ibv_close_xrcd()** returns 0 on success, or the value of errno on failure
(which indicates the failure reason).

# NOTES

**ibv_close_xrcd()** may fail if any other resource is still associated with
the XRCD being closed.

# SEE ALSO

**ibv_create_qp_ex**(3),
**ibv_create_srq_ex**(3)

# AUTHOR

Sean Hefty <sean.hefty@intel.com>
