#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <syslog.h>

#include "daemonize.h"
#include "error.h"

#define Multithread 1

#if Multithread
sigset_t mask;

void
reread(void)
{
	/* ... */
}

void *
thr_fn(void *arg)
{
	int err, signo;

	for (;;)
	{
		err = sigwait(&mask, &signo);
		if (err != 0)
		{
			syslog(LOG_ERR, "sigwait failed");
			exit(1);
		}

		switch (signo)
		{
			case SIGHUP:
				syslog(LOG_INFO, "Re-reading confiuration file");
				reread();
				break;

			case SIGTERM:
				syslog(LOG_INFO, "got SIGTERM; exiting");
				exit(0);

			default:
				syslog(LOG_INFO, "unexpected signal %d\n", signo);
		}
	}
}

int
main(int argc, char *argv[])
{

	int              err;
	pthread_t        tid;
	char             *cmd;
	struct sigaction sa;

	if ((cmd = strrchr(argv[0], '/')) ==  NULL)
		cmd = argv[0];
	else
		cmd++;

	/* init daemon */ 
	daemonize(cmd);

	/* Make sure only one copy of the daemon is running */
	if (already_running())
	{
		syslog(LOG_ERR, "daemon already running");
		exit(1);
	}

	/*
	 * Restore SIGHUP default and block all signals
	 */
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) <0)
		err_quit("can't restore SIGHUP default");
	sigfillset(&mask);

	if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)
		err_exit(err, "SIG_BLOCK error");

	/*
	 * Create a thread to handle SIGHUP and SIGTREM
	 */
	err = pthread_create(&tid, NULL, thr_fn, 0);
	if (err != 0)
		err_exit(err, "can't create thread");


	/*
	   Proceed with the rest of the daemon
	 */
	/*...*/
	for (;;)
	{
		syslog(LOG_INFO, "Hello world!");
		sleep(5);
	}

	exit(0);
}

#else

void
reread(void)
{
	/* ... */
}

void
sigterm(int signo)
{
	syslog(LOG_INFO, "got SIGTERM;exiting");
	exit(0);
}

void
sighup(int signo)
{
	syslog(LOG_INFO, "Re-reading configuration file");
	relead();
}

int
main(int argc, char *argv[])
{

	char             *cmd;
	struct sigaction sa;

	if ((cmd = strrchr(argv[0], '/')) ==  NULL)
		cmd = argv[0];
	else
		cmd++;

	/* init daemon */ 
	daemonize(cmd);

	/* Make sure only one copy of the daemon is running */
	if (already_running())
	{
		syslog(LOG_ERR, "daemon already running");
		exit(1);
	}

	/*
	 * Handle signale of interest 
	 */
	sa.sa_handler = sigterm;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGHUP);
	sa.sa_flags = 0;
	if (sigaction(SIGTERM, &sa, NULL) <0)
	{
		syslog(LOG_ERR, "can't catch SIGTREM:%s", strerror(errno));
		exit(1);
	}

	sa.sa_handler = sighup;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGTERM);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) <0)
	{
		syslog(LOG_ERR, "can't catch SIGHUP:%s", strerror(errno));
		exit(1);
	}


	/*
	   Proceed with the rest of the daemon
	 */
	/*...*/
	for (;;)
	{
		syslog(LOG_INFO, "Hello world!");
		sleep(5);
	}

	exit(0);
}

#endif
