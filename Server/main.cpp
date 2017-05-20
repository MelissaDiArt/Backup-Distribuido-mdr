#include "mainwindow.h"
#include <QApplication>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>

void go_Demonio(char *argv[])
{
    pid_t pid;
    umask(0);
    pid = fork();

    if (pid < 0) {
        std::cerr << strerror(errno) << '\n';
        exit(10);
    }

    if (pid > 0) {
        exit(0);
    }

    openlog(argv[0], LOG_NOWAIT | LOG_PID, LOG_USER);
    syslog(LOG_NOTICE, "Demonio iniciado con éxito\n");

    pid = setsid();
    if (pid < 0) {
        syslog(LOG_ERR, "No fue posible crear una nueva sesión\n");
        exit(11);
    }

    if ((chdir("/")) < 0) {
        syslog(LOG_ERR, "No fue posible cambiar el directorio de trabajo a /\n");
        exit(12);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int fd0 = open("/dev/null", O_RDONLY);
    int fd1 = open("/dev/null", O_WRONLY);
    int fd2 = open("/dev/null", O_WRONLY);

    QByteArray ug = qgetenv("USER");
    passwd* user = getpwnam(ug);
    if (user == NULL) qDebug() << errno << strerror(errno);
    seteuid(user->pw_uid);

    group* group = getgrnam(ug);
    setegid(group->gr_gid);
}

int main(int argc, char *argv[])
{
    go_Demonio(argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

