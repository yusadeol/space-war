#include "array.h"

int ArrayCompareIntegerAscending(const void *a, const void *b) {
    const int x = *(const int *)a;
    const int y = *(const int *)b;

    if (x < y) {
        return -1;
    }

    if (x > y) {
        return 1;
    }

    return 0;
}

int ArrayDeduplicateInteger(int *array, const int count) {
    int new_count = 0;

    for (int i = 0; i < count; i++) {
        bool already_exists = false;

        for (int j = 0; j < new_count; j++) {
            if (array[i] == array[j]) {
                already_exists = true;

                break;
            }
        }

        if (!already_exists) {
            array[new_count++] = array[i];
        }
    }

    return new_count;
}
