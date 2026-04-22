#include "index.h"
#include <stdio.h>

int object_write(ObjectType type, const void *data, size_t len, ObjectID *id_out);

int main(void) {
    printf("Step 1: loading index\n"); fflush(stdout);
    Index index;
    int r = index_load(&index);
    printf("Step 2: index_load returned %d, count=%d\n", r, index.count); fflush(stdout);
    r = index_add(&index, "hello.txt");
    printf("Step 3: index_add returned %d\n", r); fflush(stdout);
    return 0;
}
