#include <apue.h>

#define MAXEPOLLSIZE 1024


// 为EPOLL添加事件
int epoll_add_event(int _epfd, int _fd, uint32_t _events = EPOLLIN)
{

    EPOLLEVENT ev;
    ev.data.fd = _fd;
    ev.events = _events;
    return epoll_ctl(_epfd, EPOLL_CTL_ADD, _fd, &ev);
}

int epoll_remove_event(int _epfd, int _fd){


    return epoll_ctl(_epfd, EPOLL_CTL_DEL, _fd,NULL);


}



int main(int argc, char **argv)
{

    char *srvip = NULL;
    char read_buf[MINIBUFSIZ];
    in_port_t port;
    // 判断参数
    if (argc == 3)
    {
        srvip = argv[1];
        sscanf(argv[2], "%hu", &port);
    }
    else if (argc==2)
    {
        sscanf(argv[1], "%hu", &port);
    }
    else
    {
        printf("参数出错:usage %s ip port\n", argv[0]);
        exit(1); // 退出程序
    }



    
    
    int ret, num_of_ret;

    // 创建监听用的socket描述符lfd
    int lfd = socket_bind_listen(srvip, port);

    // 创建epoll根结点描述符
    int epfd = epoll_create(MAXEPOLLSIZE);

    HANDLE_ERROR_N1_EXIT(epfd, "epoll_create");

    // 为epoll树添加监听
    epoll_add_event(epfd, lfd);

    // 开始监听
    EPOLLEVENT events[MAXEPOLLSIZE];
    while (1)
    {

        num_of_ret = epoll_wait(epfd, events, MAXEPOLLSIZE, -1);
        HANDLE_ERROR_N1_EXIT(epfd, "epoll_wait");
        for (int i = 0; i < num_of_ret; i++)
        {
            //获取fd以供后续用
            int fd = events[i].data.fd;

            if (fd == lfd) // 监听socket返回 有连接
            {
                SOCKADDRIN addr = {0};
                socklen_t addrlen = sizeof(addr);
                int cfd = accept(lfd, (PSOCKADDR)&addr, &addrlen);
                printf("%s@%d上线\n", ip_from_sockaddr(&addr),
                       ntohs(addr.sin_port));

                // 把cfd挂到树上
                epoll_add_event(epfd,cfd);
            }
            else
            {
                // 开始读写数据
                int nread = read(fd,read_buf,MINIBUFSIZ);
                if (nread > 0)
                    printf("读取到%d个字节\n",nread);

                if (nread>0){

                        write(STDOUT_FILENO,read_buf,nread);
                }
                else if(nread == 0){

                        //有客户端下线
                        printf("有客户端下线\n");
                        //把事件从树上摘下
                        ret = epoll_remove_event(epfd,fd);
                        HANDLE_ERROR_N1_EXIT(epfd, "epoll_del_event");
                        //关闭客户端套接字
                        close(fd);

                }
                else 
                {

                    //出错
                }
                

            }
        }
    }

    return 0;
}