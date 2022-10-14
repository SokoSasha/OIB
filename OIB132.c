#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789*-+/.?,=_)(&^%$#@!~}{><;:[]";

int CheckCapital(char passwd)
{
	for (int i = 0; i < 26; i++)
	{
		if (table[i] == passwd)
			return 1;
	}
	return 0;
}

int CheckLowercase(char passwd)
{
	for (int i = 26; i < 52; i++)
	{
		if (table[i] == passwd)
			return 1;
	}
	return 0;
}

int CheckDigits(char passwd)
{
	for (int i = 52; i < 62; i++)
	{
		if (table[i] == passwd)
			return 1;
	}
	return 0;
}

int CheckSymbols(char passwd)
{
	for (int i = 62; i < 90; i++)
	{
		if (table[i] == passwd)
			return 1;
	}
	return 0;
}

void NewUser(char* login, char* password)
{
	/*system("G:");
	system("cd G:\\web\\apache\\bin");*/
	FILE* file;
	char cmd[100] = { '\0' };
	if ((file = fopen("G:/web/apache/bin/.htpasswd", "r")))
	{
		fclose(file);
		strcat(cmd, "htpasswd -b G:\\web\\apache\\bin\\.htpasswd \0");
	}
	else strcat(cmd, "htpasswd -cb G:\\web\\apache\\bin\\.htpasswd \0");
	strcat(cmd, login);
	strcat(cmd, " ");
	strcat(cmd, password);
	if (!system(NULL))
		return;
	system(cmd);
}

int main()
{
	char login[20];
	char password[20];
	int cap = 0;
	int low = 0;
	int sym = 0;
	int dig = 0;
	printf("Enter the name of a new user\n");
	scanf("%s", login);
	while (cap + low + dig + sym != 4)
	{
		printf("Enter the password for the new user\n");
		scanf("%s", password);
		if (strlen(password) < 10)
		{
			printf("Password should be more than 10 symbols\n");
			printf("Enter the password for the new user\n");
			scanf("%s", password);
		}

		for (int i = 0; i < strlen(password); i++)
		{
			if (CheckCapital(password[i]))
				cap = 1;
			if (CheckLowercase(password[i]))
				low = 1;
			if (CheckDigits(password[i]))
				dig = 1;
			if (CheckSymbols(password[i]))
				sym = 1;
			if (cap + low + dig + sym == 4)
			{
				NewUser(login, password);
				return 0;
			}
		}
		printf("Password should include capital letters, lowercase letters, digits and symbols!\n");
	}
	return 0;

}