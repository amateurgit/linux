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

    /* 创建 json 数组对象 */
    struct json_object *array = json_object_new_array();
    json_object_array_add(array, json_object_new_int(100));
    json_object_array_add(array, json_object_new_int(90));
    json_object_array_add(array, json_object_new_int(80));

    /* 把数组对象添加到 json 对象中 */
    json_object_object_add(obj, "score", array);

    /* 打印 */
    printf("%s\n", json_object_to_json_string(obj));


    /* 解析 */
    struct json_object *json;
    json_object_object_get_ex(obj, "score", &json);
    if (json_type_array == json_object_get_type(json))
    {
	int i;
	int size = json_object_array_length(json);	/* 获取数组长度 */
	for (i = 0; i < size; i++)
	{
	    struct json_object *j = json_object_array_get_idx(json, i);
	    if (json_type_int == json_object_get_type(j))
	    {
		printf("%d\n", json_object_get_int(j));
	    }
	}
    }

    return 0;
}
