#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_assert.h"
#include "aterm1.h"
#include "aterm2.h"
#include "generate_aterms.h"
#include "generate.h"
#include "logging.h"

void test_generate_premise() {
    char *buffer;
    size_t buffer_size;
    FILE* fd = open_memstream(&buffer, &buffer_size);
    Premise premise = (Premise) { .type = EQUALITY, .left = ATmake("a(b, c)"), .right = ATmake("d(b)")};

    generate_premise(fd, &premise, 2);
    fputc(0, fd); // to end the list string
    fclose(fd);
    
    log_info("Generated the premise: \n%s", buffer);    
    assert(strncmp(buffer, 
            "\t// premise #2\n"
            "\tATerm p2 = ATmakeTerm(rule.premises[2].left, b, c);\n"
            "\tif(!ATisEqual(p2, rule.premises[2].right)) return NULL;\n", 1024) == 0);
}

int main(int argc, char** argv) {
    ATerm bottom_of_stack;
    ATinit(argc, argv, &bottom_of_stack);
    printf("%%SUITE_STARTING%% generate\n");
    printf("%%SUITE_STARTED%%\n");

    // TODO need a better way to test this
    // test(test_generate_premise);

    printf("%%SUITE_FINISHED%% time=0\n");
    return (EXIT_SUCCESS);
}
