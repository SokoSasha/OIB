#include <stdio.h>
#include <string.h>
char Base64[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
void selection(char* passwd)
{
	if (strlen(passwd) == 1)
	{
		char guess[1];
		for (int i1 = 0; i1 < 64; i1++)
		{
			guess[0] = Base64[i1];
			int flag = 1;
			for (int i = 0; i < 1; i++)
			{
				if (guess[i] != passwd[i])
					flag = 0;
			}
			if (flag == 1)
			{
				printf_s("Guessed password: %c", guess[0]);
				return 0;
			}

		}
		printf("Password include unacceptable characters!");
		return 0;
	}
	if (strlen(passwd) == 2)
	{
		char guess[2];
		for (int i1 = 0; i1 < 64; i1++)
		{
			guess[0] = Base64[i1];
			for (int i2 = 0; i2 < 64; i2++)
			{
				guess[1] = Base64[i2];
				int flag = 1;
				for (int i = 0; i < 2; i++)
				{
					if (guess[i] != passwd[i])
						flag = 0;
				}
				if (flag == 1)
				{
					printf_s("Guessed password: %c%c", guess[0], guess[1]);
					return 0;
				}
			}
		}
		printf("Password include unacceptable characters!");
		return 0;
	}
	if (strlen(passwd) == 3)
	{
		char guess[3];
		for (int i1 = 0; i1 < 64; i1++)
		{
			guess[0] = Base64[i1];
			for (int i2 = 0; i2 < 64; i2++)
			{
				guess[1] = Base64[i2];
				for (int i3 = 0; i3 < 64; i3++)
				{
					guess[2] = Base64[i3];
					int flag = 1;
					for (int i = 0; i < 3; i++)
					{
						if (guess[i] != passwd[i])
							flag = 0;
					}
					if (flag == 1)
					{
						printf_s("Guessed password: %c%c%c", guess[0], guess[1], guess[2]);
						return 0;
					}

				}
			}
		}
		printf("Password include unacceptable characters!");
		return 0;
	}
	if (strlen(passwd) == 4)
	{
		char guess[4];
		for (int i1 = 0; i1 < 64; i1++)
		{
			guess[0] = Base64[i1];
			for (int i2 = 0; i2 < 64; i2++)
			{
				guess[1] = Base64[i2];
				for (int i3 = 0; i3 < 64; i3++)
				{
					guess[2] = Base64[i3];
					for (int i4 = 0; i4 < 64; i4++)
					{
						int flag = 1;
						guess[3] = Base64[i4];
						for (int i = 0; i < 4; i++)
						{
							if (guess[i] != passwd[i])
								flag = 0;
						}
						if (flag == 1)
						{
							printf_s("Guessed password: %c%c%c%c", guess[0], guess[1], guess[2], guess[3]);
							return 0;
						}
					}
				}
			}
		}
		printf("Password include unacceptable characters!");
		return 0;
	}
}
int main()
{
	char passwd[5];
	printf("Enter the password: ");
	gets_s(passwd, 5);
	selection(passwd);
	return 0;
}
