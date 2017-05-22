//gcc -c -Wall -Werror -fPIC shared.c
//gcc -shared -o libshared.so shared.o

#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <grp.h>
#include <string.h>

//pobieranie grup

void pobierzGrupy(char * login)
{
    gid_t * idGrupy;
    struct group *grupa;
    struct passwd *pwd;
    pwd = getpwnam(login);
    int il_grup=0;
    getgrouplist(login, pwd->pw_gid, NULL, &il_grup);
    idGrupy = malloc (sizeof(gid_t)*il_grup);
    getgrouplist(login, pwd->pw_gid, idGrupy, &il_grup);

    printf("[ ");
    for (int i=0; i<il_grup; i++)
    {
        grupa = getgrgid(idGrupy[i]);
        printf ("%s ", grupa->gr_name);
    }
    free(idGrupy);
    printf("]");
}

//pobieranie id

void pobierzId(char * login)
{
    struct passwd *pwd;
    pwd = getpwnam(login);
    printf("%d ", pwd->pw_uid);
}
