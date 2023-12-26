/*
 *  libssh2 提供了远程登录 server 的方法
 *
 *  环境 :
 *    # 依赖库 
        apt-get install libssl-dev zlib1g-dev
 *    # libssh2 依赖 openssl
 *        ./config --prefix=/usr/local/openssl --openssldir=/usr/local/openssl/ssl
 *        make
 *        make install
 *    # 编译 libssh2
 *        ./configure
 *        make
 *        make install
 *
 *  编译 : 
 *     gcc -o libssh2 libssh2.c -lssh2
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libssh2.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    /* 1. 创建 socket 连接 */
    struct sockaddr_in sin;
    const char *remote_host = "10.10.10.250";
    const char *username = "cen-mngr";
    const char *passwd   = "root";

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) 
    {
        printf("创建套接字失败\n");
        exit(EXIT_FAILURE);
    }
    
    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = inet_addr(remote_host);
    if(connect(sockfd, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in))) 
    {
        printf("连接失败\n");
        goto shutdown;
    }

    /* 初始化 libssh2 库 */
    int rc = libssh2_init(0);
    if (rc != 0)
    {
        printf("初始化 libssh2 失败, 错误代码: %d\n", rc);
        goto shutdown;
    }

    /* 创建会话 */
    LIBSSH2_SESSION *session;
    session = libssh2_session_init();
    if (NULL == session)
    {
        printf("创建会话失败\n");
        goto shutdown;
    }

    /* 设置会话阻塞模式 */
    libssh2_session_set_blocking(session, 1);

    rc = libssh2_session_handshake(session, sockfd);
    if (rc != 0)
    {
        printf("会话握手失败\n");
        goto shutdown;
    }

    /* 身份验证 */
    rc = libssh2_userauth_password(session, username, passwd);
    if (rc != 0)
    {
        printf("用户认证失败\n");
        goto shutdown;
    }

    /* 执行远程命令 */
    LIBSSH2_CHANNEL *channel = libssh2_channel_open_session(session);
    if (NULL == channel)
    {
        printf("打开隧道失败\n");
        goto shutdown;
    }

    /* 执行远程命令 */
    rc = libssh2_channel_exec(channel, "ls");
    if (rc != 0)
    {
        printf("执行远程命令失败\n");
        goto shutdown;
    }

    /* 读取并打印远程命令输出 */
    char buffer[1024];
    int nbytes;
    while ((nbytes = libssh2_channel_read(channel, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[nbytes] = '\0';
        printf("%s", buffer);
    }

shutdown:
    close(sockfd);
    libssh2_channel_close(channel);
    libssh2_channel_free(channel);
    libssh2_session_disconnect(session, "Goodbye");
    libssh2_session_free(session);
    libssh2_exit();
    return 0;
}
