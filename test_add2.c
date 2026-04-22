#include "index.h"
#include <stdio.h>

int object_write(ObjectType type, const void *data, size_t len, ObjectID *id_out);

int main(void) {
    Index index;
    index_load(&index);
    
    printf("Testing object_write\n"); fflush(stdout);
    ObjectID hash;
    const char *data = "hello world\n";
    int r = object_write(OBJ_BLOB, data, 12, &hash);
    printf("object_write returned %d\n", r); fflush(stdout);

    printf("Testing index_save\n"); fflush(stdout);
    r = index_save(&index);
    printf("index_save returned %d\n", r); fflush(stdout);
    return 0;
}
