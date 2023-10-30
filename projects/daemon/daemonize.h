#ifndef _DAEMONIZE_H_
#define _DAEMONIZE_H_

void daemonize(const char *app);
int already_running(const char *pidfile);

#endif
