//gcc lab2.c -g  => //valgrind ./a.out -g	//sprawdzanie wyciekow pamieci
//gcc lab2.c -ldl  //kompilacja z dolaczeniem biblioteki wspoldzielonej
#include <stdlib.h>
#include <stdio.h>
#include <utmpx.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <dlfcn.h>

void (*PobierzId)(char*);
void (*PobierzGrup)(char*);

int main(int argc, char **argv)
{
	int ret, flagId = 0, flagGr = 0;
	struct passwd *pwd;
	gid_t * idGrupy;
	struct group *grupa;
	void * uchwyt;

	while((ret = getopt(argc, argv, "ig")) != -1)
	{
		switch (ret)
		{
			case 'i': flagId = 1; break;
			case 'g': flagGr = 1; break;
			default: printf ("nieprawidlowe przelaczniki"); return (0);
		}
	}

	if (flagId || flagGr) uchwyt = dlopen("./biblioBezId.so", RTLD_LAZY);
	if (!uchwyt)
	{
		printf("Nieudane otwarcie biblioteki: %s\n", dlerror());
		flagGr = 0;
		flagId = 0;
	}
	else
	{
		if (!(PobierzId = dlsym(uchwyt, "pobierzId")))
		{
			printf("Brak funkcji pobierzId: %s\n", dlerror());
			flagId = 0;
		}
		if (!(PobierzGrup = dlsym(uchwyt, "pobierzGrupy")))
		{
			printf("Brak funkcji pobierzGrupy: %s\n", dlerror());
			flagGr = 0;
		}
	}

	struct utmpx *a = getutxent();
	while (a)
	{
		if (a->ut_type == 7)
		{
			if (flagId) PobierzId(a->ut_user);

			printf("%s ", a->ut_user);			//wyswietlenie loginu uzytkownika

			if (flagGr) PobierzGrup(a->ut_user);
			printf("\n");
		}
		a = getutxent();
	}

	if (uchwyt)
	dlclose(uchwyt);

	return(0);
}
