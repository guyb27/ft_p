#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int		usage(char *str)
{
	printf("Usage: %s <addr> <port>\n", str);
	return (1);
}

int		create_client(char *addr, int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (!(proto = getprotobyname("tcp")))
		return (-1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	if ((connect(sock, (const struct sockaddr *)&sin, sizeof(sin))) == -1)
	{
		printf("CONNECT ERROR\n");
		exit (1);
	}
	return (sock);
}

int		main(int ac, char **av)
{
	int					port;
	int					sock;
	char				buff[1024];
	int					octets_read;

	if (ac != 0b00000011)//TEST BINAIRE (3)
		return (usage(av[0]));
	bzero(buff, sizeof(buff));
	port = atoi(av[2]);
	sock = create_client(av[1], port);
	while ((octets_read = read(0, buff, 1024)))
	{
		buff[octets_read - 1] = '\0';
		write(sock, buff, strlen(buff) + 1);
		if (!strcmp(buff, "quit"))
			break ;
	}
	close(sock);
	return (0);
}
