#include "aterm2.h"
#include "types.h"

ATbool addI(Rule rule, ATerm before, ATerm *after) {
    int a, b;
    // initial match
    ATbool rc = ATmatch(before, "addI(<int>, <int>)", &a, &b);
    if(!rc) return ATfalse;
    // final transform
    *after = (ATerm) ATmakeInt(a + b);
    return ATtrue;
}

ATbool subI(Rule rule, ATerm before, ATerm *after) {
    int a, b;
    // initial match
    ATbool rc = ATmatch(before, "subI(<int>, <int>)", &a, &b);
    if(!rc) return ATfalse;
    // final transform
    *after = (ATerm) ATmakeInt(a - b);
    return ATtrue;
}
