#include "test_assert.h"

void __test_assert_test(void (*test_case)(), const char *test_name, const char *module_name){
    clock_t t;
    printf("%%TEST_STARTED%% %s (%s)\n", test_name, module_name); 
    
    t = clock();
    test_case(); 
    t = clock() - t;
    
    double sec = ((double)t) / CLOCKS_PER_SEC;
    printf("%%TEST_FINISHED%% time=%f %s (%s) \n", sec, test_name, module_name);
}

void __test_assert_fail(const char *file, int line, const char *func, const char *expr) {
    printf("%%TEST_FAILED%% time=0 testname=%s (engine) message=failed %s at %s in %s:%d.\n", func, expr, func, file, line);
}