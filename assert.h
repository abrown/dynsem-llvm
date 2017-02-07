#include <stdio.h>
#include <time.h>

#ifndef ASSERT_H
#define ASSERT_H

// macro for asserting a condition: assert(2 == 2);
void fail(const char *file, int line, const char *func, const char *expr) {
    printf("%%TEST_FAILED%% time=0 testname=%s (engine) message=failed %s at %s in %s:%d.\n", func, expr, func, file, line);
}
#define __ASSERT_FUNC __PRETTY_FUNCTION__
#define assert(__e) ((__e) ? (void)0 : fail(__FILE__, __LINE__, __ASSERT_FUNC, #__e))

// macro for running and timing a simple test case: test(my_test);
void _test(void (*test)(), const char *test_name, const char *module_name){
    clock_t t;
    printf("%%TEST_STARTED%% %s (%s)\n", test_name, module_name); 
    
    t = clock();
    test(); 
    t = clock() - t;
    
    double sec = ((double)t) / CLOCKS_PER_SEC;
    printf("%%TEST_FINISHED%% time=%f %s (%s) \n", sec, test_name, module_name);
}
#define test(f) _test(f, #f, __FILE__)

#endif /* ASSERT_H */

