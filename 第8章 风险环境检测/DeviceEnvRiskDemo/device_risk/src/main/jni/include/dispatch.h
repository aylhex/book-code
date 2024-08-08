#ifndef DISPATCH_H
#define DISPATCH_H

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

namespace devicerisk{


class Dispatch
{
public:

    static jbyteArray GetUploadData(JNIEnv *env, char* origin_params);
};
}




#endif //DISPATCH_H
