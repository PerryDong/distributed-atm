#include <stdio.h> 
#include <sys/param.h>  
#include <sys/types.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include<fcntl.h>
#include <pthread.h>

void printids(const char *s)
{
    pid_t      pid;
    pthread_t  tid;

    pid = getpid();
    tid = pthread_self();
}

struct Account
{
    char name[256];//帐号
    char password[256]; //密码
    char money[256]; //金额
}; 

int myatoi(char s[])  
{  
    int i,n=0;  
    for (i=0;s[i]>='0'&&s[i]<='9';i++)  
    {  
        n=10*n+(s[i]-'0');    /*(s[i]-'0')能够计算出s[i]中存储的字符所对应的数字值*/  
    }  
    return n;  
}  

int createUser(struct Account acc)
{
    char filename[300] = {0};
    strcpy(filename,strcat(acc.name,".txt"));
	int fd = open(filename,O_WRONLY|O_CREAT|O_EXCL,0600);
    if(fd == -1)
    {
        return -1;
    }
    if(write(fd,&acc,sizeof(acc)) < 0)
    {
        return -1;
    }
    close(fd);
    return 0;
}

//查找帐号，并进行销户
int destoryUser(struct Account acc)
{
    char filename[300] = {0};
    strcpy(filename,strcat(acc.name,".txt"));
    int fd = open(filename,O_RDONLY,0600);
   if(fd == -1)
   {
        perror("failed:");
        printf("此帐户不存在\n");
        return -1;
   }
    //删除文件的函数
    remove(filename);
    return 0;
}

//查找帐户，并进行存钱
struct Account saveMoney(struct Account acc)
{
    char filename[300] = {0};
    strcpy(filename,strcat(acc.name,".txt"));
	int sum=0;
	int fd = open(filename,O_RDWR,0600);    
    if(fd == -1)
    {
        printf("fileopen error");
        printf("no user\n");
        return;
    }
    struct Account accTemp;
    read(fd,&accTemp,sizeof(accTemp));
	sum=myatoi(accTemp.money)+myatoi(acc.money);
    sprintf(accTemp.money, "%d", sum);
    lseek(fd,0,SEEK_SET);
    write(fd,&accTemp,sizeof(accTemp));
    return accTemp;
}
//取钱的处理
struct Account getMoney(struct Account acc)
{
	int sum=0;
    char filename[300] = {0};
    strcpy(filename,strcat(acc.name,".txt"));
    //O_WRONLY为只写 O_RDONLY为只读
    int fd = open(filename,O_RDWR,0600);
    if(fd == -1)
    {
        perror("文件打开失败");
        return ;
    }
    struct Account accTemp;
    read(fd,&accTemp,sizeof(accTemp));
	 sum=myatoi(accTemp.money)-myatoi(acc.money);
    sprintf(accTemp.money, "%d", sum);
    lseek(fd,0,SEEK_SET);
    write(fd,&accTemp,sizeof(accTemp));
    return accTemp;
}
//查询余额
struct Account checkMoney(struct Account acc)
{
    char filename[300] = {0};
    strcpy(filename,strcat(acc.name,".txt"));
    int fd = open(filename,O_RDONLY,0600);
    if(fd == -1)
    {
        perror("文件打开失败");
        return ;
    }
    struct Account accTemp;
    read(fd,&accTemp,sizeof(accTemp));
    return accTemp;
}

struct sockaddr_in init_sockaddr(unsigned int serv)
{
	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(serv);
	return servAddr;
}

int make_socket(int type,unsigned int server)
{
	int sock;
	struct sockaddr_in name;
	if((sock=socket(AF_INET,type,0))<0)
		printf("socket error\n");
	name=init_sockaddr(server);
	if(bind(sock,(struct sockaddr *)&name,(socklen_t)sizeof(name))<0)
		printf("bind\n");
	return sock;
}

void *handle(void *arg)
{
	char recvbuff[256];
	char sendbuff[256];
	int n;
	memset(recvbuff, '\0', 256);
	memset(sendbuff, '\0', 256);

	int connfd = *(int *)arg;
	struct Account acc;
	struct Account p;
	while(1)
	{
		n=read(connfd,recvbuff,sizeof(recvbuff));
		recvbuff[n]=0;
		printf("%s\n",recvbuff);

		
		if(strcmp(recvbuff,"1")==0)
		{
			
			memset(recvbuff, '\0', 256);
			n=read(connfd,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			strcpy(acc.name,recvbuff);
			printf("%s\n",acc.name);

			memset(recvbuff, '\0', 256);
			n=read(connfd,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			strcpy(acc.password,recvbuff);
			printf("%s\n",acc.password);

			memset(recvbuff, '\0', 256);
			n=read(connfd,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			strcpy(acc.money,recvbuff);
			printf("%s\n",acc.money);
			createUser(acc);
			strcpy(sendbuff,"creat a new user success");
			write(connfd,sendbuff,sizeof(sendbuff));
		}

		if(strcmp(recvbuff,"2")==0)
		{
			memset(recvbuff, '\0', 256);
			n=read(connfd,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			strcpy(acc.name,recvbuff);
			printf("%s\n",acc.name);

			memset(recvbuff, '\0', 256);
			n=read(connfd,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			strcpy(acc.password,recvbuff);
			printf("%s\n",acc.password);
			destoryUser(acc);
			
			memset(sendbuff, '\0', 256);
			strcpy(sendbuff,"destroyUser success");
			write(connfd,sendbuff,sizeof(sendbuff));
		}
		if(strcmp(recvbuff,"3")==0)
		{
			memset(recvbuff, '\0', 256);
			n=read(connfd,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			strcpy(acc.name,recvbuff);
			printf("%s\n",acc.name);

			memset(recvbuff, '\0', 256);
			n=read(connfd,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			strcpy(acc.password,recvbuff);
			printf("%s\n",acc.password);

			memset(recvbuff, '\0', 256);
			n=read(connfd,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			strcpy(acc.money,recvbuff);
			printf("%s\n",acc.money);
			
			p=saveMoney(acc);
			if(strcmp(p.password,acc.password)==0)
			{
				memset(sendbuff, '\0', 256);
				strcpy(sendbuff,p.money);
				write(connfd,sendbuff,sizeof(sendbuff));
			}
			else
			{
				memset(sendbuff, '\0', 256);
				strcpy(sendbuff,"password wrong");
				write(connfd,sendbuff,sizeof(sendbuff));
			}
		}

		if(strcmp(recvbuff,"4")==0)
		{
			memset(recvbuff, '\0', 256);
			n=read(connfd,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			strcpy(acc.name,recvbuff);
			printf("%s\n",acc.name);

			memset(recvbuff, '\0', 256);
			n=read(connfd,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			strcpy(acc.password,recvbuff);
			printf("%s\n",acc.password);

			memset(recvbuff, '\0', 256);
			n=read(connfd,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			strcpy(acc.money,recvbuff);
			printf("%s\n",acc.money);
			
			p=getMoney(acc);
			if(strcmp(p.password,acc.password)==0)
			{
				memset(sendbuff, '\0', 256);
				strcpy(sendbuff,p.money);
				write(connfd,sendbuff,sizeof(sendbuff));
			}
			else
			{
				memset(sendbuff, '\0', 256);
				strcpy(sendbuff,"password wrong");
				write(connfd,sendbuff,sizeof(sendbuff));
			}
		}
		if(strcmp(recvbuff,"5")==0)
		{
			memset(recvbuff, '\0', 256);
			n=read(connfd,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			strcpy(acc.name,recvbuff);
			printf("%s\n",acc.name);

			memset(recvbuff, '\0', 256);
			n=read(connfd,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			strcpy(acc.password,recvbuff);
			printf("%s\n",acc.password);
			p=checkMoney(acc);
			if(strcmp(p.password,acc.password)==0)
			{
				memset(sendbuff, '\0', 256);
				strcpy(sendbuff,p.money);
				write(connfd,sendbuff,sizeof(sendbuff));
			}
			else
			{
				memset(sendbuff, '\0', 256);
				strcpy(sendbuff,"password wrong");
				write(connfd,sendbuff,sizeof(sendbuff));
			}
		}
	}
}

int main()
{
	int listenfd,n,err,i=0;
	int connfd;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t len;
	char recvbuff[256];
	char sendbuff[256];

	pthread_t  tid[5];
	
	memset(recvbuff, '\0', 256);
	memset(sendbuff, '\0', 256);

	listenfd=make_socket(SOCK_STREAM,5015);
	listen(listenfd,5);
	len=sizeof(cliaddr);

	while(1)
	{
		connfd=accept(listenfd,(struct sockaddr *)&cliaddr,&len);
		i++;
		printf("connect from %s,port %d\n",inet_ntoa((struct in_addr)cliaddr.sin_addr),ntohs(cliaddr.sin_port));
		err=pthread_create(&tid[i], NULL, handle, &connfd);
		if(err!=0)
			printf("new thread error");
	}
	return 1;
}

