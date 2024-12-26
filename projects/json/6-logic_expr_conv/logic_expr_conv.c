/*

   逻辑表达式转换成 json 格式

       识别每一个子表达式, 将其转换成 json 对象进行缓存;

       逻辑表达式转换, 由很多子表达式组成的复杂表达式化简成简单表达式;

       每一个括号就表示一个子表达式, 例如 (5 or 6);

       最终转换的是一个转换后的简单表达式

   示例 :
       
      逻辑表达式  
      
          (1 or 2) and ((3 or 4) and (5 or 6) and 7)

      json 逻辑表达

          {
              "windows": {
                  "op": "and",
                  "rules": [{
                      "op": "or",
                      "rules": [{
                          "id": 1
                      }, {
                          "id": 2
                      }]
                  }, {
                      "op": "and",
                      "rules": [{
                          "op": "or",
                          "rules": [{
                              "id": 3
                          }, {
                              "id": 4
                          }]
                      }, {
                          "op": "or",
                          "rules": [{
                              "id": 5
                          }, {
                              "id": 6
                          }]
                      }, {
                          "id": 7
                      }]
                  }]
              }
          }

 */

#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>

#include"json.h"

#define  LOGICAL_OPERATOR_OR "or"
#define  LOGICAL_OPERATOR_AND "and"

/* 缓存子表达式构成的 json 对象 */
struct json_object *jbuff = NULL;

static int __subexpr_to_jsonstr(const char *expr, char jsonstr[], int jsonstrlen) {

    char sub_expr[1024] = { 0 };
    char buff[1024] = { 0 };
    const char *op = NULL;
    char *pstr = NULL;

    char *token = NULL;
    const char *delim = " ";
    int id = 0;

    struct json_object *jlogic = NULL;
    struct json_object *jrules = NULL;
    struct json_object *jobj = NULL;
    struct json_object *jtmp = NULL;
    

    snprintf(sub_expr, sizeof(sub_expr),
            "%s", expr);

    /* 逻辑运算符 */
    op = strstr(sub_expr, " ");
    if ( NULL == op ) {
        printf("子表达式格式错误\n");
        return -1;
    }

    while (' ' == *op) op++;

    if ( 0 == strncmp(op, LOGICAL_OPERATOR_OR, strlen(LOGICAL_OPERATOR_OR))) {
        op = LOGICAL_OPERATOR_OR;

    } else {
        op = LOGICAL_OPERATOR_AND;
    }

    /* 子表达式格式转换 */

    while (1) {
        if ( pstr = strstr(sub_expr, "(") ) {
            *pstr = ' ';
        } else if ( pstr = strstr(sub_expr, ")") ) {
            *pstr = ' ';
        } else if ( pstr = strstr(sub_expr, " or ") ) {
            memset(pstr, ' ', 3);
        } else if (pstr = strstr(sub_expr, " and ") ) {
            memset(pstr, ' ', 4);
        } else {
            break;
        }
    }

//    printf("sub_expr = %s\n", sub_expr);

    jlogic = json_object_new_object();
    jrules = json_object_new_array();

    json_object_object_add(jlogic, "op", json_object_new_string(op));

    token = strtok(sub_expr, delim);
    while (token) {

        if ( strstr(token, "_") ) {

#if 0
            jtmp = json_object_object_get(jbuff, token);

            snprintf(buff, sizeof(buff),
                    "%s", json_object_get_string(jtmp));

            jobj = json_tokener_parse(buff);
#endif
            
            jobj = json_object_object_get(jbuff, token);
            json_object_get(jobj);

        } else {

            id = atoi(token);
            jobj = json_object_new_object();
            json_object_object_add( jobj, "id", json_object_new_int(id));
        }

        json_object_array_add(jrules, jobj);

        token = strtok(NULL, delim);
    }

    json_object_object_add(jlogic, "rules", jrules);

    snprintf(jsonstr, jsonstrlen, 
            "%s", json_object_get_string(jlogic));

    json_object_put(jlogic);

    return 0;
}
/* 处理子表达式 */
static int __deal_subexpr( char expr[] ) {

    char sub_expr[1024] = { 0 };
    char *p_expr_start = NULL, *p_expr_end = NULL, *p = NULL;

    char key[1024] = { 0 };
    char jsonstr[1024] = { 0 };

    p_expr_start = strstr(expr, "(");
    if (NULL == p_expr_start) {
        return 0;
    }

    p_expr_end = p_expr_start;
    do {
        p_expr_end++;
        if ('(' == *p_expr_end) {
            p_expr_start = p_expr_end;
        }
    } while ( *p_expr_end != ')' );

    /* 获取子表达式 */
    snprintf(sub_expr, p_expr_end - p_expr_start + 2, 
            "%s", p_expr_start);

    /* 逻辑表达式整体转换 */
    for ( p = p_expr_start;  p <= p_expr_end ; p++ ) {

        if ( ' ' == *p 
                || '(' == *p 
                || ')' == *p ) {
            *p = '_';
        }
    }

    snprintf(key, p_expr_end - p_expr_start + 2, 
            "%s", p_expr_start);

    if ( __subexpr_to_jsonstr(sub_expr, jsonstr, sizeof(jsonstr)) < 0) {
        printf("子表达式转 json 对象失败");
        return -1;
    }

    struct json_object *jnode = json_tokener_parse(jsonstr);

    json_object_object_add(jbuff, key, jnode);

    return 1;
}

int logic_expr_conv_to_json( const char *expr, char jsonstr[], int jsonstrlen ) {


    char buff[1024] = { 0 };
    struct json_object *jlogic = NULL;

    char expr_buff[1024] = { 0 };
    snprintf(expr_buff, sizeof(expr_buff),
            "%s", expr);

    jbuff = json_object_new_object();
    if ( NULL == jbuff ) {
        return -1;
    }

    while ( 0 != __deal_subexpr(expr_buff) );

    __subexpr_to_jsonstr(expr_buff, buff, sizeof(buff));

    jlogic = json_object_new_object();
    struct json_object *jobject = json_tokener_parse(buff);
    json_object_object_add(jlogic, "windows", jobject);
    snprintf(jsonstr, jsonstrlen, 
            "%s", json_object_get_string(jlogic));

    json_object_put(jbuff);

    return 0;
}

int main(int argc, char *argv[]) {

    /* 逻辑表达式 */
    const char *expr = "(1 or 2) and ((3 or 4) and (5 or 6) and 7)";

    /* 输出 json 字符串 */
    char jsonstr[1024] = { 0 };

    if ( logic_expr_conv_to_json(expr, jsonstr, sizeof(jsonstr) ) < 0 ) {
        printf("格式转换失败!\n");
        exit(EXIT_FAILURE);
    }

    printf("输入表达式 : %s\n", expr);
    printf("输出字符串 : %s\n", jsonstr);

    exit(EXIT_SUCCESS);
}
