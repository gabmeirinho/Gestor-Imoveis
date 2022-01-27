#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>

#define MAX_NOME 50
#define MAX_MORADA 100
#define MAX_DADOS 100
#define FILENAME "C:\\temp\\imoveis.dat"

typedef enum { T0 = 0, T1 = 1, T2 = 2, T3 = 3, T4 = 4 } TTipologia;

typedef struct {
	int dia, mes, ano;
}TData;
typedef struct SImoveis {
	char nome[MAX_NOME];
	char morada[MAX_MORADA];
	float renda;
	TData data_escr;
	TTipologia tipologia;
	int nr_imov;
}TImoveis;

TImoveis Insere_Imoveis(int nr_imov) {
	TImoveis imov; int i, a;
	printf("Nome do Proprietário: ");
	fseek(stdin, 0, SEEK_END);
	gets(imov.nome);
	printf("Morada: ");
	gets(imov.morada);

	do {
		printf("Renda: ");
		scanf("%f", &imov.renda);
		if (imov.renda < 0) {
			printf("A renda não pode ser negativa\n");
		}
	} while (imov.renda < 0);

	do {
		i = 0; printf("Data [dd/mm/aaaa]:");
		scanf("%d/%d/%d",
			&imov.data_escr.dia,
			&imov.data_escr.mes,
			&imov.data_escr.ano);
		switch (imov.data_escr.mes) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12: {
			if (imov.data_escr.dia > 31 || imov.data_escr.dia <= 0) {
				i = 1;
				printf("Data invalida\n");
			}break; }
		case 4:
		case 6:
		case 9:
		case 11:
			if (imov.data_escr.dia > 30 || imov.data_escr.dia <= 0) {
				i = 1;
				printf("Data invalida\n");
			}break;
		case 2: if (imov.data_escr.ano < 0) { a = (imov.data_escr.ano * -1) % 4; }
			  else { a = imov.data_escr.ano % 4; }
			  switch (a) {
			  case 0:
				  if (imov.data_escr.dia > 29 || imov.data_escr.dia <= 0) {
					  i = 1;
					  printf("Data invalida\n");
				  }break;
			  case 1:
			  case 2:
			  case 3:
				  if (imov.data_escr.dia > 28 || imov.data_escr.dia <= 0) {
					  i = 1;
					  printf("Data invalida\n");
				  }break;
			  }break;
		default: i = 1; printf("Data invalida\n");
		}
	} while (i == 1);

	do {
		printf("Tipologia: [0,1,2,3] ");
		scanf("%d", &imov.tipologia);
		if (imov.tipologia < 0 || imov.tipologia > 3) { printf("Tipologia invalida\n"); }
	} while (imov.tipologia < 0 || imov.tipologia > 3);

	imov.nr_imov = nr_imov;
	return imov;
}

void MostrarImovel(TImoveis imov) {
	printf("\nEstrutura Número: %d\n", imov.nr_imov);
	printf("Nome do Proprietário: %s\n", imov.nome);
	printf("Morada: %s\n", imov.morada);
	printf("Renda: %f\n", imov.renda);
	printf("Data: %2d/%2d/%2d\n", imov.data_escr.dia, imov.data_escr.mes, imov.data_escr.ano);
	printf("Tipologia: T[%d]\n", imov.tipologia);
}

int menu() {
	char opc;
	printf("\t\tMENU\n[1] Inserir Informação dos Imóveis\n[2] Gravar Informação dos Imóveis no Ficheiro\n[3] Ler do Ficheiro e Mostrar\n[4] Listar todos os Imóveis Inseridos\n[5] Listar Imóveis pelo Nome do Proprietário\n[6] Remover Imóvel por Data da Escritura\n[7] Ler do Ficheiro, Ordenar, Gravar e Mostrar\n[8] Calcular o Valor Total de Rendas a Receber\n[0] Sair\n");
	fseek(stdin, 0, SEEK_END);
	scanf("%c", &opc);

	if (opc >= '0' && opc <= '8') {
		return opc;
	}
	else {
		system("cls");
		printf("Inseriu uma opção inválida, tente novamente!!\n");
		getch();
		system("cls");
		return menu();
	}
}

void GravarImoveis(TImoveis* imov, int n_imov) {
	FILE* fp;
	if ((fp = fopen(FILENAME, "wb")) == NULL) {
		printf("ERRO: falhou a abertura do ficheiro '%s' para escrita\n", FILENAME);
		return;
	}
	fwrite(imov, sizeof(TImoveis), n_imov, fp);
	fclose(fp);
}

TImoveis* LerImoveis(TImoveis* imov, int* nr_imov) {
	FILE* fp;
	if ((fp = fopen(FILENAME, "rb")) == NULL) {
		printf("AVISO: o ficheiro '%s' não existe!\n", FILENAME);
		*nr_imov = 0;
		return imov;
	}
	*nr_imov = 0;
	TImoveis* ptr = imov;
	while (fread(ptr, sizeof(TImoveis), 1, fp) == 1) {
		*nr_imov = ptr->nr_imov;
		ptr++;	
	}
	fclose(fp);
	return ptr;
}

void pesquisa(TImoveis* imov, TImoveis* ultimo) {
	TImoveis* cur;
	char pesq[MAX_NOME];
	int nores = 0;

	printf("Qual o nome do proprietário a pesquisar? ");
	fseek(stdin, 0, SEEK_END);
	gets(pesq);
	system("cls");
	for (cur = imov; cur < ultimo; cur++) {
		if (strstr(cur->nome, pesq) != NULL) {
			MostrarImovel(*cur);
			nores = 1;
		}
	}if (nores == 0) { printf("Não foi encontrado nenhum imóvel em nome do proprietário inserido\n"); }
	return;
}

TImoveis* remover_imovel(TImoveis* imov, TImoveis* ultimo) {
	if (ultimo == imov) {
		system("cls");
		printf("Não existem imóveis na lista\n");
		getch();
		return 0;
	}
	int d, m, a;
	printf("Qual a data da escritura a remover? [dd/mm/aa] ");
	scanf("%d/%d/%d", &d, &m, &a);
	TImoveis* ptr;
	TImoveis* alvo = NULL;
	for (ptr = imov; ptr < ultimo; ptr++) {
		if (ptr->data_escr.dia == d && ptr->data_escr.mes == m && ptr->data_escr.ano == a) {
			alvo = ptr;
			break;
		}
	}
	if (alvo == NULL) {
		system("cls");
		printf("O imóvel com essa data não existe!\n");
		getch();
		return 0;
	}
	for (ptr = alvo + 1; ptr < ultimo; ptr++)
		for (ptr = alvo + 1; ptr < ultimo; ptr++) {
			ptr->nr_imov--;
		}
	for (ptr = alvo + 1; ptr < ultimo; ptr++) {
		*(ptr - 1) = *ptr;
	}
	return ptr - 1;
}

void Ordenar(TImoveis v[], int n) {
	int i, j, tem, troca = 1;
	TImoveis temp;
	for (i = n - 1; i >= 1 && troca; i--) {
		troca = 0;
		for (j = 1; j <= i; j++)
			if (v[j - 1].renda > v[j].renda) {
				tem = v[j - 1].nr_imov;
				v[j - 1].nr_imov = v[j].nr_imov;
				v[j].nr_imov = tem;
				temp = v[j - 1];
				v[j - 1] = v[j];
				v[j] = temp;
				troca = 1;
			}
	}
}

void TotalRenda(TImoveis* imov, TImoveis* ultimo) {
	TImoveis* cur; int v = 0, m, tempo[MAX_DADOS], tem, e = 0, j, i, n = 0, houvetroca = 1;

	for (cur = imov; cur < ultimo; cur++) {
		v = cur->renda + v;
	}
	printf("O valor total a receber de renda é de %d", v);
}

void ListarImoveis(TImoveis v[], int n) {
	TImoveis* cur;
	TImoveis* atual = LerImoveis(v, &n);
	for (cur = v; cur < atual; cur++)
		MostrarImovel(*cur);
}

int main() {
	TImoveis imoveis[MAX_DADOS];
	int nr_imov = 0;
	TImoveis* atual = LerImoveis(imoveis, &nr_imov);
	TImoveis* cur;
	int men, resprem;

	setlocale(LC_ALL, "Portuguese");
	while (1) {
		men = menu();
		fseek(stdin, 0, SEEK_END);
		system("cls");
		switch (men) {
		case '1':
			nr_imov++;
			*atual = Insere_Imoveis(nr_imov);
			atual++;
			system("cls");
			break;
		case '2':
			GravarImoveis(imoveis, atual - imoveis);
			system("cls");
			break;
		case '3': {
			ListarImoveis(imoveis, nr_imov);
			getch();
			system("cls");
			break;
		}
		case '4':
			for (cur = imoveis; cur < atual; cur++)
			MostrarImovel(*cur);
			getch();
			system("cls");
			break;
		case '5':
			pesquisa(imoveis, atual);
			getch();
			system("cls");
			break;
		case '6':resprem = remover_imovel(imoveis, atual);
			if (resprem != 0) { atual = resprem; nr_imov--; }
			system("cls");
			break;
		case '7':
			LerImoveis(imoveis, &nr_imov);
			Ordenar(imoveis, nr_imov);
			GravarImoveis(imoveis, nr_imov);
			ListarImoveis(imoveis, nr_imov);
			getch();
			system("cls");
			break;
		case '8':
			TotalRenda(imoveis, atual);
			getch();
			system("cls");
			break;
		case '0': printf("Obrigado por utilizar o nosso programa!!"); return 0;
		}
	}
}