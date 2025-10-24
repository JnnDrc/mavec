#ifndef _MAVEC_H
#define _MAVEC_H
/* ============================================================================\
 *  __  __     __     __        
 * |  \/  | __ \ \   / /__  ___ 
 * | |\/| |/ _` \ \ / / _ \/ __|
 * | |  | | (_| |\ V /  __/ (__ 
 * |_|  |_|\__,_| \_/ \___|\___|
 * * MAcro VECtor
 * - a stupid dynamic array made with a hell of macros
 * ============================================================================|
 *
 * * How to use:
 *  - The lib is header only so just include it where you want to use
 *
 *  - To create a vector of a certain type you just create a pointer to the type
 *  and call vec_new(type), like:
 *  >> int* nums = vec_new(int);
 *
 *  - To add numbers use vec_push(vec,val), like:
 *  >> vec_push(nums,69);
 *
 *  - To get a value, you can just index the pointer or use vec_at(vec,i)
 *  >> int x = nums[3];
 *  >> int y = vec_at(vec,4);
 *
 *  - To remove values, use vec_pop(vec) or vec_remove(vec,i)
 *  >> vec_pop(nums);
 *  >> vec_remove(nums,3);
 *
 *  - You can easily iterate over a vector using vec_foreach(item, vec)
 *  >> vec_foreach(data, vec){
 *  >>     print_val(data)
 *  >> }
 *
 *  - Remember to free the vector after using it with vec_free(vec)!
 *  >> vec_free(nums);
 *
 * * Docs:
 *
 * * vec_new(T): create a new vector of type T
 * * vec_free(vec): free the vector
 *
 * * vec_push(vec,val): push a value to the end of the vector
 * * vec_pop(vec): pop the last value from the vector (and return it)
 * * vec_insert(vec,i,val): insert value val at index i in vector
 * * vec_remove(vec,i): remove value at index i in vector
 * * vec_fill(vec,i,n,val): fill the vector with #n vals, starting from j
 * * vec_swap(vec,i,j): swap the values of indexes i and j (STDC >= C23 ONLY)
 *
 * * vec_reserve(items): reserve space on the vector for AT LEAST more #tems
 * * vec_resize(cap): resize vector to desire size if it's bigger
 *
 * * vec_clear(vec): clear vector
 *
 * * vec_front(vec): get value at front (0)
 * * vec_back(vec): get value at back (len-1)
 *
 * * vec_at(vec,i): get value at index i with bounds checking (STDC >= 23 ONLY)
 * - no typeof version: vec_at(item, vec,fallback)
 * - aliases: vec_get
 * * vec_set(vec,i,val): set index i to val
 *
 * * vec_cap(vec): get the capacity of the vector
 * * vec_len(vec): get the length(used size) of the vector
 * * vec_empty(vec): check if the vector is empty(len == 0)
 *
 * * vec_foreach(item,vec): iterate over each item of vector
 * - no typeof version: vec_foreach(T, item, vec)
 * * vec_foreach_enum(item, idx, vec): iterate over each item of vector and
 * expose index
 * - no typeof version: vec_foreach_enum(T, item, idx, vec)
 *
 *
 * * Notes:
 * 
 * * Some macros rely on C23 features (specifically typeof) to work, users can
 * use versions without typeof if they want (or are using an older C standard)
 * * To disable the macros that use typeof, you can define MAVEC_NO_TYPEOF
 * to use the versions with no typeof
 * * The macro vec_swap relies on typeof and doesn't have a version without it
 * * typeof is also available on older gcc and clang as extensions
 *
 * * if you want, you can change the initial size of vectors, just define
 * VEC_INIT_CAP (number)
 * * it's highly recomende to the cap be an multiple of 2, as the vector
 * capacity is always rounded to the nearest multiple of VEC_INIT_CAP
 *
 *
 * by Jayy :3
 * */

/*
 * TODO:
 *
 * * vec_push_front(vec,val): push value to the start of vector
 * * vec_pop_front(vec): pop the value at the start of vector
 *
 * */

// ----------------------------------------------------------------------------|

#include <stdlib.h>

// 202000L := C23 initial scratch; 202311L := C23 release
#if (__STDC_VERSION__ >= 202000L || defined(__GNUC__)) || defined(__clang__)
#define __TYPEOF_AVAILABLE__ 1
#else
#define __TYPEOF_AVAILABLE__ 0
#endif /* __TYPEOF_AVAILABLE__ */

#if __TYPEOF_AVAILABLE__ && !defined(MAVEC_NO_TYPEOF)
#define __MAVEC_USE_TYPEOF__
#endif /* __MAVEC_USE_TYPEOF__ */


#ifndef VEC_INIT_CAP
#define VEC_INIT_CAP 16
#endif  /* VEC_INIT_CAP */

struct vec_header {
    size_t cap;
    size_t len;
};

#define _vec_header(vec) ((struct vec_header*)vec - 1)

// create new vector
#define vec_new(T) calloc(1,sizeof(struct vec_header) + VEC_INIT_CAP * sizeof(T)) + sizeof(struct vec_header)

// free vector 
#define vec_free(vec) free((char*)vec - sizeof(struct vec_header))

// push val to vector
#define vec_push(vec, val)                                                                                      \
do {                                                                                                            \
    struct vec_header* vh = _vec_header(vec);                                                                   \
    if(vh->len + 1 >= vh->cap){                                                                                 \
        if(vh->cap == 0) vh->cap = VEC_INIT_CAP;                                                                \
        else vh->cap = (((vh->len + VEC_INIT_CAP)/VEC_INIT_CAP) * VEC_INIT_CAP);                                \
        vec = realloc(vh,sizeof(struct vec_header) + vh->cap * sizeof(*vec)) + sizeof(struct vec_header);       \
    }                                                                                                           \
    vec[vh->len++] = val;                                                                                       \
} while (0)

// pop last value from vector (and return it, like a stack)
#define vec_pop(vec) vec[--_vec_header(vec)->len]

// insert value at index i
#define vec_insert(vec,i,val)                                                                                   \
do {                                                                                                            \
    struct vec_header* vh = _vec_header(vec);                                                                   \
    if(vh->len + 1 >= vh->cap){                                                                                 \
        if(vh->cap == 0) vh->cap = VEC_INIT_CAP;                                                                \
        else vh->cap = (((vh->len + VEC_INIT_CAP)/VEC_INIT_CAP) * VEC_INIT_CAP);                                \
        vec = realloc(vh,sizeof(struct vec_header) + vh->cap * sizeof(*vec)) + sizeof(struct vec_header);       \
    }                                                                                                           \
    for(size_t j = vh->len-1; j >= i; j--){                                                                     \
        vec[j+1] = vec[j];                                                                                      \
    }                                                                                                           \
    vec[i] =  val;                                                                                              \
    vh->len++;                                                                                                  \
}while (0)                                                                                                     

// remove value from index i
#define vec_remove(vec,i)                       \
do {                                            \
    struct vec_header* vh = _vec_header(vec);   \
    for (size_t j = i; j < vh->len; j++) {      \
        vec[j] = vec[j+1];                      \
    }                                           \
    vh->len--;                                  \
}while (0)

// fils the vector of n vals starting by index i
#define vec_fill(vec,i,n,val)                   \
do {                                            \
    struct vec_header* vh = _vec_header(vec);   \
    for (size_t j = i; j < i + n; j++) {        \
        vec[j] = val;                           \
    }                                           \
}while (0)

// c23/gnuc exclusive (currently)
#ifdef __MAVEC_USE_TYPEOF__
// swap elements i and j
#define vec_swap(vec, i, j)                     \
do {                                            \
    struct vec_header* vh = _vec_header(vec);   \
    typeof(*vec) tmp = vec[i];                  \
    vec[i] = vec[j];                            \
    vec[j] = tmp;                               \
}while (0)
#endif /* __MAVEC_USE_TYPEOF__ */

// reserve space on the vector for AT LEAST more #items
#define vec_reserve(items) \
do{                                                                                                             \
    struct vec_header* vh = _vec_header(vec);                                                                   \
    if(vh->len + items >= vh->cap){                                                                             \
        if(vh->cap == 0) vh->cap = VEC_INIT_CAP;                                                                \
        else vh->cap = (((vh->len + items + VEC_INIT_CAP)/VEC_INIT_CAP) * VEC_INIT_CAP);                        \
        vec = realloc(vh,sizeof(struct vec_header) + vh->cap * sizeof(*vec)) + sizeof(struct vec_header);       \
    }                                                                                                           \
}while (0)

// resize the vector to the desired capacity if it's bigger than the current one
#define vec_resize(cap) \
do{                                                                                                         \
    struct vec_header* vh = _vec_header(vec);                                                               \
    if(cap >= vh->cap){                                                                                     \
        if(vh->cap == 0) vh->cap = VEC_INIT_CAP;                                                            \
        else vh->cap = (((cap + VEC_INIT_CAP)/VEC_INIT_CAP) * VEC_INIT_CAP);                                \
        vec = realloc(vh,sizeof(struct vec_header) + vh->cap * sizeof(*vec)) + sizeof(struct vec_header);   \
    }                                                                                                       \
}while (0);

#define vec_clear(vec) _vec_header(vec)->len = 0

#define vec_front(vec) vec[0]
#define vec_back(vec)  vec[_vec_header(vec)->len - 1]

// get item at index i in vector with type=safe bound checking (STDC >= C23 ONLY)
#ifdef __MAVEC_USE_TYPEOF__
#define vec_at(vec,i) (i < 0 || i > _vec_header(vec)->len) ? (typeof(*vec)){0} : vec[i]
#define vec_get(vec,i) vec_at(vec,i)
#else
#define vec_at(vec,i,fallback) (i < 0 || i > _vec_header(vec)->len) ? fallback : vec[i]
#define vec_get(vec,i,fallback) vec_at(vec,i,fallback)
#endif /* __MAVEC_USE_TYPEOF__ */
// set value at index i in vector with bounds checking
#define vec_set(vec,i,val) if(i > 0 && i < _vec_header(vec)->len) vec[i] = val

// get vector capacity
#define vec_cap(vec) _vec_header(vec)->cap

// get vector length
#define vec_len(vec) _vec_header(vec)->len
// check if vector is empty
#define vec_empty(vec) (_vec_header(vec)->len == 0)

// iterate over each item of vector (statement, needs a block)
#ifdef __MAVEC_USE_TYPEOF__
#define vec_foreach(item, vec) for(typeof(*(vec)) item, *__vec = (vec); __vec < (vec) + _vec_header(vec)->len && ((item = *__vec),1); __vec++)

#define vec_foreach_enum(item, idx, vec)                    \
    for(size_t idx = 0; idx < _vec_header(vec)->len; i++)   \
        for(char __once = 1; __once; __once = 0)            \
            for(typeof(*(vec)) item = (vec)[idx]; __once; __once = 0)
#else

#define vec_foreach(T, item, vec) for(T item, *__vec = (vec); __vec < (vec) + _vec_header(vec)->len && ((item = *__vec),1); __vec++)

#define vec_foreach_enum(T, item, idx, vec)                 \
    for(size_t idx = 0; idx < _vec_header(vec)->len; i++)   \
        for(char __once = 1; __once; __once = 0)            \
            for(T item = (vec)[idx]; __once; __once = 0)
#endif /* __MAVEC_USE_TYPEOF__ */

#endif /* _MAVEC_H */
