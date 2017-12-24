/*
* at91 watchdog timer
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <time.h>

#include <linux/watchdog.h>

#include "wdogd.h"

#define DAEMON_NAME "wdogd"

/* default values in ms */
int t_val = 500;
int T_val = 60000;
int S_val = 60 * 60 * 24 * 1000; // 1 day in ms

int run_fg = 0;
int verb = 0;

char wd[64] = "/dev/watchdog";

int open_wd(char *wd, int T_val, int varb, int run_fg){
  int timeout = T_val / 1000;

  int fd=open(wd, O_WRONLY);

  if(fd == -1) {
    if (run_fg)
      fprintf(stderr, "Can not open %s : %s\n", wd, strerror(errno));
    else {
      syslog(LOG_ERR, "Can not open %s : %s\n", wd, strerror(errno));
    }
    exit(1);
  }

  ioctl(fd, WDIOC_SETTIMEOUT, &timeout);
  if (verb){
    ioctl(fd, WDIOC_GETTIMEOUT, &timeout);
    fprintf(stderr, "The timeout was is %d seconds\n", timeout);
  }

  return fd;
}

int wd_loop(int fd, int t_val, int T_val, int S_val, int run_fg, int verb){
  time_t tcur = time(NULL);
  time_t tstop = tcur + S_val / 1000;
  int tcurmin = tcur / 60;
  int tprevmin = tcurmin;
  int timeout = T_val / 1000;
  useconds_t t_uval = t_val * 1000; 

  while (tcur < tstop){

#if 0
    ioctl(fd, WDIOC_SETTIMEOUT, &timeout);
#else
    if (ioctl(fd, WDIOC_KEEPALIVE, 0) == -1){
      if (run_fg)
        fprintf(stderr, "Error WDIOC_KEEPALIVE : %s\n", strerror(errno));
      else {
        syslog(LOG_ERR, "Error WDIOC_KEEPALIVE : %s\n", strerror(errno));
      }
    }
#endif
    if (verb){

      if (verb == 2){
        if (run_fg)
          fprintf(stderr, "keepalive\n");
        else {
          syslog(LOG_NOTICE, "keepalive\n");
        }
      }

      if (tcurmin - tprevmin > 0){
        tprevmin = tcurmin;
        if (run_fg)
          fprintf(stderr, "%d minutes before restart\n", (int)((tstop - tcur) / 60));
        else {
          syslog(LOG_NOTICE, "%d minutes before restart\n", (int)((tstop - tcur) / 60));
        }
      }
    }
    usleep(t_uval);

    tcur = time(NULL);
    tcurmin = tcur / 60;
  }

  if (run_fg)
    fprintf(stderr, "REBOOT\n");
  else {
    syslog(LOG_NOTICE, "REBOOT");
  }
  if (run_fg && verb == 2){
    fprintf(stderr, "perform /bin/reboot if no debug mode\n");
  }else{
    sleep(2);
    execl("/bin/reboot", "/bin/reboot", (char*)NULL);
  }

  return 0;
}

int main(int argc, char *argv[]){

  int opt;
  pid_t pid, sid;
  int fd = -1;

  while ((opt = getopt(argc, argv, "t:T:S:Fv:")) != -1) {
    switch (opt) {
      case 't':
        t_val = atoi(optarg);
        if (strlen(optarg) < 2 || strncmp(&optarg[strlen(optarg) - 2], "ms", 2)){
          t_val *= 1000;
        }
      break;
      case 'T':
        T_val = atoi(optarg) * 1000;
      break;
      case 'S':
        S_val = atoi(optarg) * 60 * 1000;
      break;

      case 'F':
        run_fg = 1;
      break;
      case 'v':
        verb = atoi(optarg);
      break;

      case 'h':
      default: /* '?' */
        fprintf(stderr, "Usage: wdogd [-t N[ms]] [-T N] [-S N] [-F] [-v] DEV\n\n");
        fprintf(stderr, "\t-t N    Reset every N seconds (default 30)\n");
        fprintf(stderr, "\t-T N    Reboot after N seconds if not reset (default 60)\n");
        fprintf(stderr, "\t-S N    Forced system reboot every N minutes\n");
        fprintf(stderr, "\t-F      Run in foreground\n");
        fprintf(stderr, "\t-v {0,1,2} Run verbosely 0 - silent, 1 - verb, 2 - debug\n\n");
        fprintf(stderr, "Use 500ms to specify period in milliseconds\n");
        exit(1);
    }
  }

  if (optind >= argc) {
    fprintf(stderr, "Expected argument after options\n");
    exit(1);
  }

  strncpy(wd, argv[optind], sizeof(wd));

  if (verb){
    fprintf(stderr, "wdogd -t %d -T %d -S %d -F=%d -v %s\n", t_val, T_val, S_val, run_fg, wd);
  }

  fd = open_wd(wd, T_val, verb, run_fg);

  if (!run_fg){

    openlog(DAEMON_NAME, LOG_PID, LOG_LOCAL5);
    syslog(LOG_INFO, "wdogd starting: -t %d -T %d -S %d -F=%d -v %s\n", t_val, T_val, S_val, run_fg, wd);

    /*http://www.itp.uzh.ch/~dpotter/howto/daemonize*/
    if((pid = fork()) == 0){ /* child */

      signal(SIGCHLD,SIG_DFL); /* A child process dies */
      signal(SIGTSTP,SIG_IGN); /* Various TTY signals */
      signal(SIGTTOU,SIG_IGN);
      signal(SIGTTIN,SIG_IGN);
      signal(SIGHUP, SIG_IGN); /* Ignore hangup signal */
      signal(SIGTERM,SIG_DFL); /* Die on SIGTERM */

      /* Change the file mode mask */
      umask(0);

      /* Create a new SID for the child process */
      sid = setsid();
      if (sid < 0) {
        syslog(LOG_ERR, "unable to create a new session, %s", strerror(errno) );
        exit(1);
      }

      /* Change the current working directory.  This prevents the current
         directory from being locked; hence not being able to remove it. */
      if ((chdir("/")) < 0) {
        syslog(LOG_ERR, "unable to chroot, %s", strerror(errno));
        exit(1);
      }

      /* Redirect standard files to /dev/null */
      freopen( "/dev/null", "r", stdin);
      freopen( "/dev/null", "w", stdout);
      freopen( "/dev/null", "w", stderr);

      wd_loop(fd, t_val, T_val, S_val, run_fg, verb);

      syslog(LOG_NOTICE, "terminated");
      exit(0);

    } else if (pid != -1) {
      /* exit parent */
      exit(0);

    } else{
      fprintf(stderr,"Failed to fork off child process %s\n", strerror(errno));
      exit(1);
    }
  }else{
    wd_loop(fd, t_val, T_val, S_val, run_fg, verb);
  }
  close(fd);
  return 0;
}
