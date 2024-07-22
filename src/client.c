#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/epoll.h>


#include <netdb.h>
#define TIME_SERVER "api.pinduoduo.com"
#define TIME_SERVER_URL "http://api.pinduoduo.com/api/server/_stm"

extern int h_errno;

#define BUFFERSIZE 1024*2
//http://quan.suning.com/getSysTime.do

typedef int sockfd_t; // 定义一个socket文件描述符类型

int main()
{

    //解析域名
    struct hostent* p_hostent = gethostbyname(TIME_SERVER);
    if (NULL == p_hostent)
    {
        puts("gethostbyname error!\n");
        exit(1);
    }

    //创建socket
    sockfd_t lfd = socket(AF_INET,SOCK_STREAM,0);

    if (lfd < 0 )
    {
        perror("socket");
        exit(1);
    }


    //绑定IP和端口
    struct sockaddr_in si;
    si.sin_family = AF_INET;
    si.sin_port = htons(80);
    si.sin_addr.s_addr = *((uint32_t*)(p_hostent->h_addr_list[0]));

 
    //连接服务器
    int ret = connect(lfd,(struct sockaddr*)&si,sizeof(si));
    
     if (ret == -1 )
    {
        perror("connect");
        exit(1);
    }
    
   
    //发送HTTP请求
    char send_buffer[BUFFERSIZE] = {0};
    char recv_buffer[BUFFERSIZE] = {0};


    sprintf(send_buffer,"GET %s HTTP/1.1\r\nHOST: %s\r\n\r\n",TIME_SERVER_URL,TIME_SERVER);

    //发送数据
    ret = write(lfd,send_buffer,strlen(send_buffer));
    if (ret <=0 )
    {
        perror("write");
        exit(1);
    }

    //接收数据
    ret = read(lfd,recv_buffer,sizeof(recv_buffer));
    if (ret <=0 )
    {
        perror("read");
        exit(1);
    }

    puts(recv_buffer);

    //解析字符串
    
    close(lfd);


        return 0;
}