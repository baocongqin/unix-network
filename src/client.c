#include <apue.h>

#define TIME_SERVER "quan.suning.com"
#define TIME_SERVER_URL "http://quan.suning.com/getSysTime.do"

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

//	puts(recv_buffer);

	/*
	//解析字符串 拼多多
	char * srvtime = strstr(recv_buffer,"server_time");
	puts(srvtime);
	// time_t tt = time(NULL);
	unsigned long long ts=0xFFFFFFFF;
	sscanf(srvtime,"%*[^0-9]%llu",&ts);
	printf("%llu\n",ts);
	*/


	//{"sysTime2":"2024-07-22 09:31:12","sysTime1":"20240722093112"}
	char buf[1024] = {0};
	sscanf(recv_buffer,"%*[^{]%*[^:]:\"%[^\"]",buf);
	printf("%s\n",buf);
	
	
	close(lfd);


	return 0;
}
