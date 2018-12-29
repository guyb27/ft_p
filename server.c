#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int		usage(char *str)
{
	printf("Usage: %s <port>\n", str);
	return (1);
}

int		create_server(int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (!(proto = getprotobyname("tcp")))
		return (-1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	if ((bind(sock, (const struct sockaddr *)&sin, sizeof(sin))) == -1)
	{
		printf("BIND ERROR\n");
		exit (1);
	}
	listen(sock, 42);
	return (sock);
}

int		main(int ac, char **av)
{
	int					port;
	int					sock;
	int					cs;
	unsigned int		cslen;
	struct sockaddr_in	csin;
	int					r;
	char				buff[1024];

	if (ac != 2)
		return (usage(av[0]));
	port = atoi(av[1]);
	sock = create_server(port);
	cs = accept(sock, (struct sockaddr *)&csin, &cslen);
	while ((r = read(cs, buff, 1023)) > 0)
	{
		buff[r - 1] = '\0';
		if (!strcmp(buff, "quit"))
			break;
		printf("receveid %d bytes: [%s]\n", r, buff);
	}
	close(cs);
	close(sock);
	return (0);
}
