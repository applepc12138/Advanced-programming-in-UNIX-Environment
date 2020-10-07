#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_EVENTS 1024 /*��������*/
#define BUFLEN  4096    /*��������С*/
#define SERV_PORT 6666  /*�˿ں�*/

void recvdata(int fd,int events,void *arg);
void senddata(int fd,int events,void *arg);

/*���������ļ��������������Ϣ*/
struct myevent_s
{
    int fd;             //Ҫ�������ļ�������
    int events;         //��Ӧ�ļ����¼���EPOLLIN��EPLLOUT
    void *arg;          //ָ���Լ��ṹ��ָ��
    void (*call_back)(int fd,int events,void *arg); //�ص�����,��epoll_wait�������غ󱻵���
    int status;         //�Ƿ��ڼ���:1->�ں������(����), 0->����(������)
    char buf[BUFLEN];   
    int len;
    long last_active;   //��¼ÿ�μ������� g_efd ��ʱ��ֵ
};

int g_efd;      //ȫ�ֱ�������Ϊ�������
struct myevent_s g_events[MAX_EVENTS+1];    //�Զ���ṹ����������. +1-->listen fd


/*
 * ����(��װ)һ���Զ����¼�������fd�����fd�Ļص�����������һ������Ĳ�����
 * ע�⣺�ڷ�װ����¼���ʱ��Ϊ����¼�ָ���˻ص�������һ����˵��һ��fdֻ��һ���ض����¼�
 * ����Ȥ��������¼�������ʱ�򣬾͵�������ص�����
 */
void eventset(struct myevent_s *ev, int fd, void (*call_back)(int fd,int events,void *arg), void *arg)
{
    ev->fd = fd;
    ev->call_back = call_back;
    ev->events = 0;
    ev->arg = arg;
    ev->status = 0;
    if(ev->len <= 0) {
        memset(ev->buf, 0, sizeof(ev->buf));
        ev->len = 0;
    }
    ev->last_active = time(NULL); //����eventset������ʱ��
    return;
}

/* �� epoll�����ĺ���� ���һ���ļ������� */
void eventadd(int efd, int events, struct myevent_s *ev)
{
    struct epoll_event epv={0, {0}};
    int op = 0;
    epv.data.ptr = ev; // ptrָ��һ���ṹ�壨֮ǰ��epollģ�ͺ�����Ϲ��ص����ļ�������confd��lfd��������ptrָ�룩
    epv.events = ev->events = events; //EPOLLIN �� EPOLLOUT
    if(ev->status == 0) {      //status ˵���ļ��������Ƿ��ں������ 0���ڣ�1 �� 
        op = EPOLL_CTL_ADD; //����������� g_efd, ����status��1
        ev->status = 1;
    }
    if(epoll_ctl(efd, op, ev->fd, &epv) < 0) // ���һ���ڵ�
        printf("event add failed [fd=%d],events[%d]\n", ev->fd, events);
    else
        printf("event add OK [fd=%d],events[%0X]\n", ev->fd, events);
    return;
}

/* ��epoll ������ �������ɾ��һ���ļ�������*/
void eventdel(int efd,struct myevent_s *ev)
{
    struct epoll_event epv = {0, {0}};
    if(ev->status != 1) //���fdû����ӵ��������ϣ��Ͳ���ɾ����ֱ�ӷ���
        return;
    epv.data.ptr = NULL;
    ev->status = 0;
    epoll_ctl(efd, EPOLL_CTL_DEL, ev->fd, &epv);
    return;
}

//listenfd������Ļص�����
/*  �����ļ�����������, epoll_wait����, ���øú�����ͻ��˽������� */
void acceptconn(int lfd,int events,void *arg)
{
    struct sockaddr_in cin;
    socklen_t len = sizeof(cin);
    int confd, i;
    if((confd = accept(lfd, (struct sockaddr *)&cin, &len)) == -1) {
        if(errno != EAGAIN && errno != EINTR) {
            sleep(1);
        }
        printf("%s:accept,%s\n",__func__, strerror(errno));
        return;
    }
    do {
        for(i = 0; i < MAX_EVENTS; i++) {//��ȫ������g_events����һ������Ԫ�أ�������select����ֵΪ-1��Ԫ��
            if(g_events[i].status == 0)
                break;
        }
        if(i == MAX_EVENTS) {// ��������������
            printf("%s: max connect limit[%d]\n", __func__, MAX_EVENTS);
            break;
        }
        int flag = 0;
        if((flag = fcntl(confd, F_SETFL, O_NONBLOCK)) < 0) {//��confdҲ����Ϊ������
            printf("%s: fcntl nonblocking failed, %s\n", __func__, strerror(errno));
            break;
        }
        eventset(&g_events[i], confd, recvdata, &g_events[i]); //�ҵ����ʵĽڵ�֮�󣬽�����ӵ��������У����������¼�
        eventadd(g_efd, EPOLLIN, &g_events[i]);
    }while(0);

    printf("new connect[%s:%d],[time:%ld],pos[%d]",inet_ntoa(cin.sin_addr), ntohs(cin.sin_port), g_events[i].last_active, i);
    return;
}

/*��ȡ�ͻ��˷����������ݵĺ���*/
void recvdata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;

    len = recv(fd, ev->buf, sizeof(ev->buf), 0);    //��ȡ�ͻ��˷�����������

    eventdel(g_efd, ev);                            //���ýڵ�Ӻ������ժ��

    if (len > 0) {
        ev->len = len;
        ev->buf[len] = '\0';                        //�ֶ�����ַ����������

		//���������ݴ������
        printf("C[%d]:%s\n", fd, ev->buf);                  

		//�����������ֱ�ӵ���write������ͻ��˷����ݿ��ܻᵼ�·���������
        eventset(ev, fd, senddata, ev);             //���ø�fd��Ӧ�Ļص�����Ϊsenddata    
        eventadd(g_efd, EPOLLOUT, ev);              //��fd��������g_efd��,������д�¼�    

    } 
    else if (len == 0) { 
        close(ev->fd);
        /* ev-g_events ��ַ����õ�ƫ��Ԫ��λ�� */
        printf("[fd=%d] pos[%ld], closed\n", fd, ev-g_events);
    } 
    else {
        close(ev->fd);
        printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));
    }   
    return;
}

/*���͸��ͻ�������*/
void senddata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;

    len = send(fd, ev->buf, ev->len, 0);    //ֱ�ӽ����ݻ�����ͻ���

    eventdel(g_efd, ev);                    //�Ӻ����g_efd���Ƴ�

    if (len > 0) {
        printf("send[fd=%d], [%d]%s\n", fd, len, ev->buf);
        eventset(ev, fd, recvdata, ev);     //����fd�Ļص�������Ϊrecvdata
        eventadd(g_efd, EPOLLIN, ev);       //������ӵ�������ϣ���Ϊ�������¼�
    }
    else {
        close(ev->fd);                      //�ر�����
        printf("send[fd=%d] error %s\n", fd, strerror(errno));
    }
    return ;
}

/*���� socket, ��ʼ��lfd */

void initlistensocket(int efd, short port)
{
    struct sockaddr_in sin;

    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(lfd, F_SETFL, O_NONBLOCK);                //��socket��Ϊ������

    memset(&sin, 0, sizeof(sin));               //bzero(&sin, sizeof(sin))
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);

    bind(lfd, (struct sockaddr *)&sin, sizeof(sin));

    listen(lfd, 20);

    /* void eventset(struct myevent_s *ev, int fd, void (*call_back)(int, int, void *), void *arg);  */
    eventset(&g_events[MAX_EVENTS], lfd, acceptconn, &g_events[MAX_EVENTS]);    

    /* void eventadd(int efd, int events, struct myevent_s *ev) */
    eventadd(efd, EPOLLIN, &g_events[MAX_EVENTS]);  //��lfd��ӵ��������ϣ��������¼�

    return;
}

int main()
{
    int port=SERV_PORT;

    g_efd = epoll_create(MAX_EVENTS + 1); //���������,���ظ�ȫ�� g_efd
    if(g_efd <= 0)
            printf("create efd in %s err %s\n", __func__, strerror(errno));
    
    initlistensocket(g_efd, port); //��ʼ������socket
    
    struct epoll_event events[MAX_EVENTS + 1];  //��������ṹ�����飬��������epoll_wait��������������¼���fd�ṹ��
    printf("server running:port[%d]\n", port);

    int checkpos = 0;
    int i;
    while(1){
    /*    long now = time(NULL);
        for(i=0; i < 100; i++, checkpos++) {
            if(checkpos == MAX_EVENTS);
                checkpos = 0;
            if(g_events[checkpos].status != 1)
                continue;
            long duration = now -g_events[checkpos].last_active;
            if(duration >= 60)
            {
                close(g_events[checkpos].fd);
                printf("[fd=%d] timeout\n", g_events[checkpos].fd);
                eventdel(g_efd, &g_events[checkpos]);
            }
        } */
        //����epoll_wait�ȴ�����Ŀͻ����¼�,epoll_wait�����������������������Щfd��struct epoll_event����
        int nfd = epoll_wait(g_efd, events, MAX_EVENTS+1, 1000);
        if (nfd < 0) {
            printf("epoll_wait error, exit\n");
            exit(-1);
        }
        for(i = 0; i < nfd; i++) {
		    //evtAdd()�����У���ӵ��������м����¼���ʱ��myevents_t�ṹ�����͸���ptrָ��
            //����epoll_wait���ص�ʱ��ͬ���᷵�ض�Ӧfd��myevents_t���͵�ָ��
            struct myevent_s *ev = (struct myevent_s *)events[i].data.ptr;
            //����������Ƕ��¼��������ص��Ƕ��¼�
            if((events[i].events & EPOLLIN) &&(ev->events & EPOLLIN)) {
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
            //�����������д�¼��������ص���д�¼�
            if((events[i].events & EPOLLOUT) && (ev->events & EPOLLOUT)) {
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
        }
    }
    return 0;
}
