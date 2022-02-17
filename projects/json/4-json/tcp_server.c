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
#define BACKLOG 2

/* 服务器对客户端的处理 */
void process_conn_server(int s)
{
        char buf[1024];

	int ret = recv(s, buf, sizeof(buf), 0);
	if (-1 == ret)
	{
	    perror("recv");
	    exit(1);
	}

	/* 字符串转 json */
	struct json_object *obj = json_tokener_parse(buf);
	struct json_object *json;

	json_object_object_get_ex(obj, "name", &json);
	printf("name : %s\n", json_object_get_string(json));

	json_object_object_get_ex(obj, "age", &json);
	printf("age : %d\n", json_object_get_int(json));

	json_object_object_get_ex(obj, "sex", &json);
	printf("sex : %s\n", json_object_get_string(json));
}

int main(int argc, char *argv[])
{
	int ss, sc;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int err;
	pid_t pid;

	/* 建立一个流式套接字 */
	ss = socket(AF_INET, SOCK_STREAM, 0);
	if(ss < 0)
	{
		printf("socket error\n");
		return -1;
	}

	/* 设置服务器地址 */
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	/* 绑定地址结构到套接字描述符 */
	err = bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err < 0)
	{
		printf("bind error\n");
		return -1;
	}

	/* 设置侦听队列 */
	err = listen(ss, BACKLOG);
	if(err < 0)
	{
		printf("listen error\n");
		return -1;
	}

	/* 主循环过程 */
	for(;;)
	{
		int addrlen = sizeof(struct sockaddr);

		sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
		/* 接收客户端连接 */
		if(sc < 0)
		{
			continue;
		}

		/* 建立一个新的进程处理到来的连接 */
		pid = fork();
		if(pid == 0)	/*在子进程中*/
		{
			close(ss);
			process_conn_server(sc);
		}
		else   /* 在父进程中关闭客户端的连接 */
		{
			close(sc);
		}
	}
}
