#ifndef MJSON_H
#define MJSON_H

#include <iostream>
#include <string>
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/memorystream.h"

using namespace rapidjson;

using namespace std;

namespace devicerisk
{
class MJson
{
public:

    static Document NewJsonObject();

    static void AddString(Document &d, const string &_key, const string &_value);
    static void AddInt(Document &d, const string &_key, const int _value);
    static string ToJsonString(Document &_document);

    static string GetString(const char* _json_string, const string &_key);
    static int GetInt(const char* _json_string, const string &_key);

    static string GetString(const string &_json_string, const string &_key);
    static int GetInt(const string &_json_string, const string &_key);
};

}

#endif //MJSON_H
