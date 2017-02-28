#include "types.h"

#ifndef GENERATE_H
#define GENERATE_H

#ifdef __cplusplus
extern "C" {
#endif

    void generate_main(FILE *stream);
    void generate_headers(FILE *stream);
    void generate_rule_table(FILE *stream, RuleTable rules);
    void generate_rule_table_entry(FILE *stream, Rule rule);
    void generate_find_function(FILE *stream, RuleTable rules);
    void generate_find_case(FILE *stream, Rule rule);
    void generate_transform_functions(FILE *stream, RuleTable rules);
    void generate_transform_function(FILE *stream, Rule rule);
    void generate_transform_allocation(FILE *stream, Rule rule);
    void generate_transform_match(FILE *stream, Rule rule);
    void generate_transform_make(FILE *stream, Rule rule);


#ifdef __cplusplus
}
#endif

#endif /* GENERATE_H */

