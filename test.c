#include <stdio.h>

#include "mavec.h"

int main(void){
    int* vec = vec_new(int);

    for(int i = 0; i <= 20; i++){
        vec_push(vec,i*i);
    }

    vec_insert(vec, 4, 69);
    
    vec_foreach_enum(num, i, vec){
        printf("%llu:%d\n",i, num);
    }

    printf("len is %zu\n", vec_len(vec));
    printf("cap is %zu\n", vec_cap(vec));

    vec_remove(vec,4);
    printf("removed 4, now len is %zu\n", vec_len(vec));
    
    vec_free(vec);
    
    return 0;
}
