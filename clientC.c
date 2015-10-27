#include <stdio.h> 
#include <sys/param.h>  
#include <sys/types.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include<fcntl.h>

struct Account
{
    char name[256];//帐号
    char password[256]; //密码
    char money[256]; //金额
};

struct sockaddr_in init_sockaddr(char *addr,unsigned int serv)
{
	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(addr);
	servAddr.sin_port = htons(serv);
	return servAddr;
}

int socket_connect(char *addr,unsigned int port)
{
	int sockfd;
	struct sockaddr_in name;
	name=init_sockaddr(addr,port);
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
		printf("socket error");
	if((connect(sockfd,(struct sockaddr *)&name,sizeof(name)))<0)
		printf("connect error");
	return sockfd;
}

void display()
{
       printf("---------menu-------------\n");
       printf("[1]open amount\n");
       printf("[2]drop amount\n");
       printf("[3]save money\n");
       printf("[4]draw money\n");
       printf("[5]check money\n");
       printf("-------------------------\n");
	   printf("enter your choice:\n");
}

int main()
{
	int sockfd,sockfd2,n;
	char *ip;
	char sen[256];
	char recvbuff[256];
	char choose;
	memset(sen, '\0', 256);
	memset(recvbuff, '\0', 256);

	sockfd=socket_connect("10.10.169.51",5013);
//******************获得跳转ip***************************
	printf("Please choose the amount A,B,C\n");
	scanf("%s",&sen);
	write(sockfd,sen,sizeof(sen));
	printf("you choose the %s amount\n",sen);
//*******************用户操作***********************************
	if(strcmp(sen,"A")!=0)
	{
		n=read(sockfd,recvbuff,sizeof(recvbuff));
		recvbuff[n]=0;
		ip=recvbuff;
		printf("%s\n",ip);
		sockfd2=socket_connect(ip,5015);
	}
	else
		sockfd2=sockfd;
	while(1)
	{
		display();
		scanf("%c",&choose);
		memset(recvbuff, '\0', 256);
		memset(sen, '\0', 256);
		sen[0]=choose;
		write(sockfd2,sen,sizeof(sen));
		struct Account acc;
		if(choose=='1')
		{
			
			printf("please enter the amount name:\n");
			scanf("%s",acc.name);
			memset(sen, '\0', 256);
			strcpy(sen,acc.name);
			write(sockfd2,sen,sizeof(sen));

			printf("please enter the amount password:\n");
			scanf("%s",acc.password);
			memset(sen, '\0', 256);
			strcpy(sen,acc.password);
			write(sockfd2,sen,sizeof(sen));

			printf("please enter the amount money:\n");
			scanf("%s",&acc.money);
			memset(sen, '\0', 256);
			strcpy(sen,acc.money);
			write(sockfd2,sen,sizeof(sen));

			memset(recvbuff, '\0', 256);
			n=read(sockfd2,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			printf("%s\n",recvbuff);
		}

		if(choose=='2')
		{
			
			printf("please enter the amount name:\n");
			scanf("%s",acc.name);
			memset(sen, '\0', 256);
			strcpy(sen,acc.name);
			write(sockfd2,sen,sizeof(sen));

			printf("please enter the amount password:\n");
			scanf("%s",acc.password);
			memset(sen, '\0', 256);
			strcpy(sen,acc.password);
			write(sockfd2,sen,sizeof(sen));

			memset(recvbuff, '\0', 256);
			n=read(sockfd2,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			printf("%s\n",recvbuff);
		}
		if(choose=='3')
		{
			printf("please enter the amount name:\n");
			scanf("%s",acc.name);
			memset(sen, '\0', 256);
			strcpy(sen,acc.name);
			write(sockfd2,sen,sizeof(sen));

			printf("please enter the amount password:\n");
			scanf("%s",acc.password);
			memset(sen, '\0', 256);
			strcpy(sen,acc.password);
			write(sockfd2,sen,sizeof(sen));

			printf("please enter the saving money:\n");
			scanf("%s",&acc.money);
			memset(sen, '\0', 256);
			strcpy(sen,acc.money);
			write(sockfd2,sen,sizeof(sen));

			memset(recvbuff, '\0', 256);
			n=read(sockfd2,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			printf("counting money,please wait........\n");
			printf("the sum money is %s\n",recvbuff);

		}
		if(choose=='4')
		{
			printf("please enter the amount name:\n");
			scanf("%s",acc.name);
			memset(sen, '\0', 256);
			strcpy(sen,acc.name);
			write(sockfd2,sen,sizeof(sen));

			printf("please enter the amount password:\n");
			scanf("%s",acc.password);
			memset(sen, '\0', 256);
			strcpy(sen,acc.password);
			write(sockfd2,sen,sizeof(sen));

			printf("please enter the saving money:\n");
			scanf("%s",&acc.money);
			memset(sen, '\0', 256);
			strcpy(sen,acc.money);
			write(sockfd2,sen,sizeof(sen));

			memset(recvbuff, '\0', 256);
			n=read(sockfd2,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			printf("counting money,please wait........\n");
			printf("the remained money is %s\n",recvbuff);

		}
		if(choose=='5')
		{
			printf("please enter the amount name:\n");
			scanf("%s",acc.name);
			memset(sen, '\0', 256);
			strcpy(sen,acc.name);
			write(sockfd2,sen,sizeof(sen));

			printf("please enter the amount password:\n");
			scanf("%s",acc.password);
			memset(sen, '\0', 256);
			strcpy(sen,acc.password);
			write(sockfd2,sen,sizeof(sen));

			memset(recvbuff, '\0', 256);
			n=read(sockfd2,recvbuff,sizeof(recvbuff));
			recvbuff[n]=0;
			printf("the remained money is \n%s\n",recvbuff);
		}
	}

//*****************************************************	
	return 1;
}