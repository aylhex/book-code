#include <mjson.h>

namespace devicerisk
{

Document MJson::NewJsonObject()
{
    Document document;
    document.SetObject();
    return document;
}

string MJson::GetString(const string &_json_string, const string &_key)
{
    Document document;
    if (document.Parse(_json_string.c_str()).HasParseError() || !document.HasMember(_key.c_str()))
    {
        return "";
    }
    const rapidjson::Value &jv = document[_key.c_str()];
    return jv.GetString();
}

int MJson::GetInt(const string &_json_string, const string &_key)
{
    Document document;
    if (document.Parse(_json_string.c_str()).HasParseError() || !document.HasMember(_key.c_str()))
    {
        return 6666;
    }
    const rapidjson::Value &jv = document[_key.c_str()];
    return jv.GetInt();
}


string MJson::GetString(const char* _json_string, const string &_key)
{
    Document document;
    if (document.Parse(_json_string).HasParseError() || !document.HasMember(_key.c_str()))
    {
        return "";
    }
    const rapidjson::Value &jv = document[_key.c_str()];
    return jv.GetString();
}

int MJson::GetInt(const char* _json_string, const string &_key)
{
    Document document;
    if (document.Parse(_json_string).HasParseError() || !document.HasMember(_key.c_str()))
    {
        return 6666;
    }
    const rapidjson::Value &jv = document[_key.c_str()];
    return jv.GetInt();
}


void MJson::AddString(Document &d, const string &_key, const string &_value)
{
    Value json_key(kStringType);
    Value json_value(kStringType);
    json_key.SetString(_key.c_str(), _key.size(), d.GetAllocator());
    json_value.SetString(_value.c_str(), _value.size(), d.GetAllocator());
    d.AddMember(json_key, json_value, d.GetAllocator());
}

void MJson::AddInt(Document &d, const string &_key, const int _value)
{
    Value json_key(kStringType);
    Value json_value(kStringType);
    json_key.SetString(_key.c_str(), _key.size(), d.GetAllocator());
    d.AddMember(json_key, _value, d.GetAllocator());
}

string MJson::ToJsonString(Document &_document)
{
    // StringBuffer是一个简单的输出流。它分配一个内存缓冲区，供写入整个JSON。
    // 可使用GetString()来获取该缓冲区。
    StringBuffer json_buf;
    Writer<StringBuffer> writer(json_buf);
    _document.Accept(writer);

    return json_buf.GetString();
}

}
