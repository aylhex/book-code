#ifndef _ANTI_FRAUD_H_
#define _ANTI_FRAUD_H_
#include <stdio.h>
#include <stdlib.h>
#include <anti_fraud.h>
#include <sys/system_properties.h>

namespace devicerisk
{
class MainFraud
{
public:
    /*
    返回值为0 说明没有hook信息
    返回值为非0 说明获取到风险信息
     */
    static void AddAntiFraudDataToJson(JNIEnv *env, Document &_json);

};

}
#endif