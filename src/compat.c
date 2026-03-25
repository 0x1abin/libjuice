/**
 * Compatibility functions for ESP-IDF platform.
 */

#ifdef ESP_PLATFORM

#include "socket.h"
#include <stdio.h>

int getnameinfo(const struct sockaddr *sa, socklen_t salen,
                char *host, socklen_t hostlen,
                char *serv, socklen_t servlen, int flags) {
	(void)salen;
	(void)flags;
	if (sa->sa_family == AF_INET) {
		const struct sockaddr_in *sin = (const struct sockaddr_in *)sa;
		if (host && hostlen > 0)
			inet_ntop(AF_INET, &sin->sin_addr, host, hostlen);
		if (serv && servlen > 0)
			snprintf(serv, servlen, "%u", ntohs(sin->sin_port));
		return 0;
	} else if (sa->sa_family == AF_INET6) {
		const struct sockaddr_in6 *sin6 = (const struct sockaddr_in6 *)sa;
		if (host && hostlen > 0)
			inet_ntop(AF_INET6, &sin6->sin6_addr, host, hostlen);
		if (serv && servlen > 0)
			snprintf(serv, servlen, "%u", ntohs(sin6->sin6_port));
		return 0;
	}
	return -1;
}

#endif /* ESP_PLATFORM */
