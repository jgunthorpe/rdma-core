---
date: 2006-10-31
footer: libibverbs
header: "Libibverbs Programmer's Manual"
layout: page
license: 'Licensed under the OpenIB.org BSD license (FreeBSD Variant) - See COPYING.md'
section: 3
title: IBV_CREATE_COMP_CHANNEL
---

# NAME

ibv_create_comp_channel, ibv_destroy_comp_channel - create or destroy a
completion event channel

# SYNOPSIS

```c
#include <infiniband/verbs.h>

struct ibv_comp_channel *ibv_create_comp_channel(struct ibv_context *context);

int ibv_destroy_comp_channel(struct ibv_comp_channel *channel);
```

# DESCRIPTION

**ibv_create_comp_channel()** creates a completion event channel for the RDMA
device context *context*.

**ibv_destroy_comp_channel()** destroys the completion event channel
*channel*.

# RETURN VALUE

**ibv_create_comp_channel()** returns a pointer to the created completion
event channel, or NULL if the request fails.

**ibv_destroy_comp_channel()** returns 0 on success, or the value of errno on
failure (which indicates the failure reason).

# NOTES

A "completion channel" is an abstraction introduced by libibverbs that does
not exist in the InfiniBand Architecture verbs specification or RDMA Protocol
Verbs Specification.  A completion channel is essentially file descriptor that
is used to deliver completion notifications to a userspace process.  When a
completion event is generated for a completion queue (CQ), the event is
delivered via the completion channel attached to that CQ.  This may be useful
to steer completion events to different threads by using multiple completion
channels.

**ibv_destroy_comp_channel()** fails if any CQs are still associated with the
completion event channel being destroyed.

# SEE ALSO

**ibv_create_cq**(3),
**ibv_get_cq_event**(3),
**ibv_open_device**(3)

# AUTHOR

Dotan Barak <dotanba@gmail.com>
