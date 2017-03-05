/*
 * For APIs specific to aterms implementation
 */
#include <aterm1.h>
#include <aterm2.h>

#ifndef GENERATE_ATERM_H
#define GENERATE_ATERM_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef ATerm(*free_variables_cb)(ATerm current, void *data);
    ATerm iterate_free_variables(ATerm term, free_variables_cb callback, void *callback_data);
    ATerm replace_free_variables(ATerm term);
    ATermList find_free_variables(ATerm term, ATermList start);
    void generate_variable_list(FILE *stream, ATermList vars, char *prefix, char *delimiter, char *suffix);
    void generate_transform_allocation_with_aterm(FILE *stream, ATerm rule); // TODO use rules

#ifdef __cplusplus
}
#endif

#endif /* GENERATE_ATERM_H */

