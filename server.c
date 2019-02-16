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
	sin.sin_addr.s_addr = htonl(INADDR_ANY);//Correspond a l adresse 0.0.0.0 ou toutes les adresses ip de la machine
	if (!(proto = getprotobyname("tcp")))//Recuperation du protocole
		return (-1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	if ((bind(sock, (const struct sockaddr *)&sin, sizeof(sin))) == -1)
	{
		printf("BIND ERROR\n");
		exit (1);
	}
	listen(sock, 42);//Specifie la taille de la queue
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
	int					pid;

	if (ac != 0b00000010)
		return (usage(av[0]));
	port = atoi(av[1]);
	sock = create_server(port);
//	while (1)
//	{
		pid = fork();
		cs = accept(sock, (struct sockaddr *)&csin, &cslen);
		while ((r = read(cs, buff, 1023)) > 0)
		{
			buff[r - 1] = '\0';
			if (!strcmp(buff, "shutdown"))
				break;
			printf("sock: [%d] receveid %d bytes: [%s]\n", htonl(csin.sin_addr.s_addr), r, buff);//Recupere l ID du client avec select()
		}
		printf("Connexion interrompu\n");
		close(pid);
		close(cs);
//	}
	close(sock);
	return (0);
}
