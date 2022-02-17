#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>

#include"json.h"

int main(int argc, char **aggv)
{

    const char *str = "{\"name\":\"jack\",\"age\":22,\"sex\":\"male\"}";

    /* 把符合 json 格式的字符串转换成 json 对象 */
    struct json_object *obj = json_tokener_parse(str);

    /* json 对象转换为字符串 */
    printf("%s\n", json_object_to_json_string(obj));


    return 0;
}
