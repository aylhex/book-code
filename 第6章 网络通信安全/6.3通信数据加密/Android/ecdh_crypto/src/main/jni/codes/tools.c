//
//  Created by AYL on 19/8/23.
//

#include <tools.h>

// 安全关闭指针函数
void mm_safe_free(void **pp) {
    if (pp != NULL && *pp != NULL) 
    {
        free(*pp);
        *pp = NULL;
    }
}