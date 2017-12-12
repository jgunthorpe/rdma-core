---
date: 2017-04-19
footer: 1.0.0
layout: page
license: 'Licensed under the OpenIB.org (MIT) - See COPYING.md'
section: 7
title: MLX4DV
---

# NAME

mlx4dv - Direct verbs for mlx4 devices

This is low level access to mlx4 devices to perform data path operations,
without general branching performed by **ibv_post_send**(3).

# DESCRIPTION

The libibverbs API is an abstract one. It is agnostic to any underlying
provider specific implementation. While this abstraction has the advantage of
user applications portability it has a performance penalty. For some
applications optimizing performance is more important than portability.

The mlx4 direct verbs API is intended for such applications. It exposes mlx4
specific low level data path (send/receive/completion) operations, allowing
the application to bypass the libibverbs data path API.

This interface consists from one hardware specific header file with relevant
inline functions and conversion logic from ibverbs structures to mlx4 specific
structures.

The direct include of mlx4dv.h together with linkage to mlx4 library will
allow usage of this new interface.

Once an application uses the direct flow the locking scheme is fully managed
by itself. There is an expectation that no mixed flows in the data path for
both direct/non-direct access will be by same application.

# NOTES


# SEE ALSO

**ibv_post_send**(3),
**verbs**(7)

# AUTHOR

Maor Gottlieb <maorg@mellanox.com>
