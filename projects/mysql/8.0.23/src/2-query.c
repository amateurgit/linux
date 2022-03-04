#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql.h>

int main(int agrc, char *argv[])
{
    MYSQL     *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[] = "select host, user from mysql.user;";
    int i;

    /* 初始化 */
    conn = mysql_init(NULL);
    if (NULL == conn)
    {
	printf("mysql_init() error\n");
	goto err;
    }

    /* 连接数据库 */
    if (!mysql_real_connect(conn, "localhost", "root", "mysql", NULL, 0, NULL, 0))
    {
	printf("%s\n", mysql_error(conn));
	goto err;
    }

    /* 查询 */
    if (mysql_real_query(conn, query, strlen(query)))
    {
	printf("mysql_real_query error\n");
	goto err;
    }

    /* 获取查询数据 */
    res = mysql_store_result(conn); 
    if (res)
    {
	while (row = mysql_fetch_row(res))
	{
	    for (i = 0; i < mysql_num_fields(res); i++)
	    {
		printf("%s\t", row[i]);
	    }
	    printf("\n");
	}

	mysql_free_result(res);
	res = NULL;
    }

    mysql_close(conn);
    return 0;

err:
    mysql_close(conn);

    return -1;
}
