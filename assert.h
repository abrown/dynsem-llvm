#include <stdio.h>

#ifndef ASSERT_H
#define ASSERT_H

void fail(const char *file, int line, const char *func, const char *expr) {
    printf("%%TEST_FAILED%% time=0 testname=%s (engine) message=failed %s at %s in %s:%d.\n", func, expr, func, file, line);
}
#define __ASSERT_FUNC __PRETTY_FUNCTION__
#define assert(__e) ((__e) ? (void)0 : fail(__FILE__, __LINE__, __ASSERT_FUNC, #__e))

#endif /* ASSERT_H */

