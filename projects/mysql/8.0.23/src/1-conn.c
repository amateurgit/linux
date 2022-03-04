#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

int main(int agrc, char *argv[])
{
    MYSQL *conn;

    /* 初始化 */
    conn = mysql_init(NULL);
    if (NULL == conn)
    {
	printf("mysql_init() error\n");
	return -1;
    }

    /* 连接数据库 */
    if (!mysql_real_connect(conn, "localhost", "root", "mysql", NULL, 0, NULL, 0))
    {
	printf("%s\n", mysql_error(conn));
	mysql_close(conn);
    }
    else
    {
	printf("连接成功！\n");
    }

    mysql_close(conn);

    return 0;
}
