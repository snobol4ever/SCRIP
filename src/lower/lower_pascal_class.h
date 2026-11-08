#ifndef LOWER_PASCAL_CLASS_H
#define LOWER_PASCAL_CLASS_H
tree_t *pas_classes_prepass(tree_t *prog);
static inline long long pas_hash_stable(const char *nm) {
    unsigned long long h = 1469598103934665603ULL;
    for (const char *p = nm; *p; p++) { h ^= (unsigned char)*p; h *= 1099511628211ULL; }
    unsigned long long v = (1000000000ULL + (h % 1000000000ULL)) & ~0xFULL;
    return (long long)v;
}
#endif
