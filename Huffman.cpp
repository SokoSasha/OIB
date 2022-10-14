#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <locale.h>
#include <time.h>

FILE* in, * out;

struct node
{
	char code[255];
	unsigned char ch;
	float probability;
	node* left;
	node* right;
};

void WriteBinFile(char* pb)
{
	int i, j;
	unsigned char uch[3] = { 0 };
	for (i = 0; i < 3; i++)
		for (j = 0; j < 8; j++)
			if (*pb++ == '1') uch[i] = uch[i] | (1 << (7 - j));
	fwrite(&uch[0], 3, 1, out);
	return;
}

int ReadBinFile(char* pb)
{
	int i, j, rd;
	unsigned char uch[3] = { 0 };
	char* pbtmp;

	pbtmp = pb;
	rd = fread(uch, 1, 3, in);
	for (i = 0; i < rd; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (uch[i] & (1 << (7 - j))) *pb++ = '1';
			else *pb++ = '0';
		}
	}
	*pb = 0;
	return rd;
}

node* BuildTreeHuff(int count_char, node* pnode[])
{
	int i, j;
	node* temp;

	temp = (node*)malloc(sizeof(node));
	temp->probability = pnode[count_char - 1]->probability + pnode[count_char - 2]->probability;
	temp->code[0] = 0;
	temp->left = pnode[count_char - 1];
	temp->right = pnode[count_char - 2];

	if (count_char == 2) return temp;
	else
	{
		for (i = 0; i < count_char; i++)
			if (temp->probability > pnode[i]->probability)
			{
				for (j = count_char - 1; j > i; j--) pnode[j] = pnode[j - 1];
				pnode[i] = temp;
				break;
			}
	}
	return BuildTreeHuff(count_char - 1, pnode);
}

void CodeAdd(node* root)
{
	if (root->left)
	{
		strcpy(root->left->code, root->code);
		strcat(root->left->code, "0");
		CodeAdd(root->left);
	}
	if (root->right)
	{
		strcpy(root->right->code, root->code);
		strcat(root->right->code, "1");
		CodeAdd(root->right);
	}
}

int main()
{
	node param[256] = { 0 }, * pnode[256], * root, * tmp_node;
	int code_char, count_tab = 0, unique_char[256] = { 0 }, i, j = 0, flag, tab_code[256] = { 0 };
	float count_out = 0, count_char = 0;
	char buf_in[64] = { 0 }, buf_out[64] = { 0 }, tmp_buf[64] = { 0 };
	unsigned char cu;
	double start, finish, sec;

	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int menu;
	printf("1 : Кодирование\n"
		"2 : Декодирование\n"
		">");
	scanf("%d", &menu);
	system("cls");

	if (menu == 1)
	{
		char namein[20], nameout[20];
		printf("Введите входной и выходной файлы\n");
		scanf("%s", namein);
		scanf("%s", nameout);

		if ((in = fopen(namein, "rb")) == NULL)
		{
			printf("Ошибка открытия входного файла!\n");
			return 1;
		}
		if ((out = fopen(nameout, "wb")) == NULL)
		{
			printf("Ошибка открытия выходного файла!\n");
			return 1;
		}

		start = clock();
		while ((code_char = fgetc(in)) != EOF)
		{
			unique_char[code_char]++;
			count_char++;
			if (unique_char[code_char] == 1) param[count_tab++].ch = (unsigned char)code_char;
		}
		if (count_tab == 1)
		{
			fwrite(&count_char, sizeof(int), 1, out);
			fwrite(&count_tab, sizeof(int), 1, out);
			cu = param[0].ch;
			fwrite(&cu, sizeof(char), 1, out);
			fclose(in);
			fclose(out);
			finish = clock();
			sec = (finish - start) / (double)CLOCKS_PER_SEC;
			float a = (float)sizeof(int) * 2 + 1;
			printf("\nВремя кодирования: %.3f сек\nРазмер входного файла: %.0f байт(а/ов)\nРазмер выходного файла: %.0f байт(а/ов)\nКоэффициент сжатия: %f\n", sec, count_char, a, count_char / a);
			return 0;
		}

		for (i = 0; i < count_tab; i++)
		{
			pnode[i] = &param[i];
			param[i].probability = (float)unique_char[param[i].ch] / (float)count_char;
		}

		node tempp;
		for (i = 1; i < count_tab; i++)
		{
			flag = 0;
			for (j = 0; j < count_tab - 1; j++)
				if (param[j].probability < param[j + 1].probability)
				{
					tempp = param[j];
					param[j] = param[j + 1];
					param[j + 1] = tempp;
					flag++;
				}
			if (flag == 0) break;
		}

		root = BuildTreeHuff(count_tab, pnode);
		CodeAdd(root);

		for (i = 0; i < count_tab; i++) tab_code[param[i].ch] = i;

		fwrite(&count_char, sizeof(int), 1, out);
		fwrite(&count_tab, sizeof(int), 1, out);
		for (i = 0; i < count_tab; i++)
		{
			fwrite(&param[i].ch, sizeof(char), 1, out);
			fwrite(&param[i].probability, sizeof(float), 1, out);
		}
		count_out = sizeof(int) * 2 + count_tab * (sizeof(char) + sizeof(float));
		fseek(in, 0, SEEK_SET);
		while (fread(&cu, 1, 1, in))
		{
			i = tab_code[cu];
			strcat(&buf_out[0], param[i].code);
			if (strlen(&buf_out[0]) >= 24)
			{
				WriteBinFile(&buf_out[0]);
				strcpy(&tmp_buf[0], &buf_out[24]);
				strcpy(&buf_out[0], &tmp_buf[0]);
				count_out += 3;
			}
		}
		if (strlen(&buf_out[0]) < 24)
		{
			strncat(&buf_out[0], "00000000000000000000000", 24 - strlen(&buf_out[0]));
			WriteBinFile(&buf_out[0]);
			count_out += 3;
		}
		fclose(in);
		fclose(out);
		finish = clock();
		sec = (finish - start) / (double)CLOCKS_PER_SEC;
		printf("\nВремя кодирования: %.3f сек\nРазмер входного файла: %.0f байт(а/ов)\nРазмер выходного файла: %.0f байт(а/ов)\nКоэффициент сжатия: %f\n", sec, count_char, count_out, count_char / count_out);
	}

	if (menu == 2)
	{
		char namein[20], nameout[20];
		printf("Введите входной и выходной файлы\n");
		scanf("%s", nameout);
		scanf("%s", namein);

		if ((in = fopen(nameout, "rb")) == NULL)
		{
			printf("Ошибка открытия входного файла!\n");
			return 1;
		}
		if ((out = fopen(namein, "wb")) == NULL)
		{
			printf("Ошибка открытия выходного файла!\n");
			return 1;
		}

		start = clock();

		fread(&count_char, sizeof(int), 1, in);
		fread(&count_tab, sizeof(int), 1, in);

		if (count_tab == 1)
		{
			fread(&cu, sizeof(char), 1, in);
			while (count_char--) fwrite(&cu, 1, 1, out);
			fclose(in);
			fclose(out);
			finish = clock();
			sec = (finish - start) / (double)CLOCKS_PER_SEC;
			printf("\nВремя декодирования: %.3f сек\n", sec);
			return 0;
		}

		for (i = 0; i < count_tab; i++) pnode[i] = &param[i];
		for (i = 0; i < count_tab; i++)
		{
			fread(&param[i].ch, sizeof(char), 1, in);
			fread(&param[i].probability, sizeof(float), 1, in);
		}

		node tempp;
		for (i = 1; i < count_tab; i++)
		{
			flag = 0;
			for (j = 0; j < count_tab - 1; j++)
				if (param[j].probability < param[j + 1].probability)
				{
					tempp = param[j];
					param[j] = param[j + 1];
					param[j + 1] = tempp;
					flag++;
				}
			if (flag == 0) break;
		}

		root = BuildTreeHuff(count_tab, pnode);
		CodeAdd(root);

		tmp_buf[0] = 0;
		while (1)
		{
			i = ReadBinFile(&buf_in[0]);
			strcat(&tmp_buf[0], &buf_in[0]);
			strcpy(&buf_out[0], &tmp_buf[0]);

			i = 0;
			j = 0;
			tmp_node = root;
			while ((code_char = buf_out[i]))
			{
				i++;
				if (code_char == '0') tmp_node = tmp_node->left;
				else tmp_node = tmp_node->right;
				if (tmp_node->right == 0 && tmp_node->left == 0)
				{
					fwrite(&tmp_node->ch, 1, 1, out);
					tmp_node = root;
					j = i;
					if (--count_char == 0)
					{
						j = 0;
						break;
					}
				}
			}
			if (j == 0) break;
			strcpy(&tmp_buf[0], &buf_out[j]);
		}

		fclose(in);
		fclose(out);
		finish = clock();
		sec = (finish - start) / (double)CLOCKS_PER_SEC;
		printf("\nВремя декодирования: %.3f сек\n", sec);
	}
	return 0;
}