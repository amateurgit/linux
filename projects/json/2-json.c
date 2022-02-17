#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>

#include"json.h"

int main(int argc, char **aggv)
{
    /* 创建空 json 对象 */
    struct json_object *obj = json_object_new_object();

    /* 往 json 对象添加键值对 */
    json_object_object_add(obj, "name", json_object_new_string("jack"));
    json_object_object_add(obj, "age", json_object_new_int(11));
    json_object_object_add(obj, "sex", json_object_new_string("male"));

    /* 打印 */
    printf("%s\n", json_object_to_json_string(obj));

    /* 解析 json */
    /* 第一步 根据键名解析出对应的 json 对象 */
    struct json_object *json;
    json_object_object_get_ex(obj, "name", &json);
    /* 第二步 根据 json 类型转换成对应的数据 */
    json_type type = json_object_get_type(json);
    if (json_type_string == type)
    {
	printf("name : %s\n", json_object_get_string(json));
    }

    json_object_object_get_ex(obj, "age", &json);
    printf("age : %d\n", json_object_get_int(json));

    json_object_object_get_ex(obj, "sex", &json);
    printf("sex : %s\n", json_object_get_string(json));


    return 0;
}
