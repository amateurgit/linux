#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <unistd.h>

#include "json.h"

#define PORT 8888

void process_conn_client(int s)
{
    struct json_object *json = json_object_new_object();
    json_object_object_add(json, "name", json_object_new_string("jack"));
    json_object_object_add(json, "age", json_object_new_int(11));
    json_object_object_add(json, "sex", json_object_new_string("male"));

    const char *buf = json_object_to_json_string(json);

    int ret = send(s, buf, strlen(buf), 0);
    if (-1 == ret)
    {
	perror("send");
	exit(1);
    }
    printf("字符串 %s 发送成功 长度 %ld\n", buf, strlen(buf));

}


int main(int argc, char *argv[])
{
	int s;
	struct sockaddr_in server_addr;
	int err;

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		printf("socket error\n");
		return -1;
	}

	/* 设置服务器地址 */
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	/* 将用户输入的字符串类型的IP地址转为整形 */
	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
	/* 链接服务器 */
	connect(s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));

	process_conn_client(s);
	close(s);
}


