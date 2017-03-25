#include "types.h"

#ifndef GENERATE_H
#define GENERATE_H

#ifdef __cplusplus
extern "C" {
#endif

    void assign_indices(Specification *spec);
    void generate_headers(FILE *stream);
    void generate_rule_table(FILE *stream, Specification *spec);
    void generate_rule_table_entry(FILE *stream, Rule *rule);
    void generate_match_function(FILE *stream, Specification *spec);
    void generate_native_functions(FILE *stream, Specification *spec);
    void generate_native_function(FILE *stream, Native *spec);
    void generate_rule_functions(FILE *stream, Specification *spec);
    void generate_rule_function(FILE *stream, Rule *rule);
    void generate_premise(FILE *stream, Rule *rule, Premise *premise);


#ifdef __cplusplus
}
#endif

#endif /* GENERATE_H */

