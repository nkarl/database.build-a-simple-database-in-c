#include "test_query_statements.h"

#include "macros.h"
#include <assert.h>
#include <stdio.h>

void test_query_statements_01(void) {
    char const *q = "select * from database;";
    debug("%s", q);
}
