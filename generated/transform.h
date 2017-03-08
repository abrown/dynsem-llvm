#include <aterm1.h>
#include "types.h"

#ifndef TRANSFORM_H
#define TRANSFORM_H

#ifdef __cplusplus
extern "C" {
#endif

    void build_rules();
    ATerm match_and_transform(ATerm before);

#ifdef __cplusplus
}
#endif

#endif /* TRANSFORM_H */

