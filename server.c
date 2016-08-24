#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <string.h>
#include <errno.h>

#define PORT 12345
#define IP_ADDR "192.168.149.3"
int main(void)
{
	int sock_fd = 0;
	int ret = 0;
	struct sockaddr_in addr = {0};
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0){
		printf("socket error\n");
		goto out;
	}
	int opt = 1;
	setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP_ADDR);

	ret = bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr));
	if(ret < 0){
		perror("bind error\n");
		goto out;
	}

	ret = listen(sock_fd, 3);
	if(ret < 0){
		perror("listen error\n");
		goto out;
	}

	int addr_len = 0;
	int fd = accept(sock_fd, (struct sockaddr *)&addr, &addr_len);
	if(fd < 0){
		printf("accept error\n");
		goto out;
	}

	char buf[256] = {0};
	while(1){
		memset(buf, 0, sizeof(buf));
		ret = read(fd, buf, 256);
		if(ret < 0){
			printf("read error\n");
			break;
		}
		printf("read :%s\n", buf);
		write(fd, buf, ret);
	}
	
	if(fd > 0){
		close(fd);
		fd = 0;
	}

	if(sock_fd > 0){
		close(sock_fd);
		sock_fd = 0;
	}
	
	return 0;
out:
	if(fd > 0){
		close(fd);
		fd = 0;
	}

	if(sock_fd > 0){
		close(sock_fd);
		sock_fd = 0;
	}

	return 0;
}
