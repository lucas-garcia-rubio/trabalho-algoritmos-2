// trabalho de algoritmos 2
//Notas:
/*Para fazer o trabalho será necessário apenas um tipo de lista encadeada*/

# include <stdio.h>
# include <stdlib.h>
# include <locale.h>

typedef struct body
{
	int id;
	struct body *next;
	int peso; //o peso não será definido para a cabeça do vetor, somente para as células ligadas a ela

	//para o trabalho 2
	int d; //distância para a busca em largura, instante para a busca em profundidade, valor para Dijkstra, key para Prim
	int f; //instante para a busca em profundidade, verificado ou não para Dijkstra
	int pi;
	int cor; //1 - branco, 2 - cinza, 3 - preto
}body;


//------------Implementando fila para busca em largura no grafo------------//
//Fila estática
int u, t;
body fila[100];

void inicializaFila()
{
	u=0;
	t=0;
}

void enfila(body n)
{
	fila[t++] = n;
}

body desenfila()
{
	body r = fila[u++];
	return r;
}

int vazia()
{
	if(u == t)
		return 1;
	else
		return 0;
}

//-------------------------------------------------------------------------//

//-----------------------TRABALHO 2-----------------------//
void buscaLargura(int y, body *v, int t) //t = 1 se quer busca em largura, t = 2 se quer conexo
{
	body x; //x representa o u nos slides
	int s;
	int i;
	int numS;

	if(t == 1)
	{
		printf("Raíz da busca: ");
		scanf("%d", &s);
	}
	else
		s = 1;

	for(i=0; i<y; i++)
	{
		if(v[i].id != s)
		{
			v[i].cor = 1;
			v[i].d = -1; //representando infinito;
			v[i].pi = -1; //representando indefinição;
			body *p=v[i].next;
			while(p != NULL)
			{
				if(p->id != s)
				{
					p->cor = 1;
					p->d = -1;
					p->pi = -1;
					p = p->next;
				}
				else
				{
					p->cor = 2;
					p->d = 0;
					p->pi = -1;
					p = p->next;
				}
			}
		}
		else
		{
			v[i].cor = 2;
			v[i].d = 0;
			v[i].pi = -1;
			numS = i; //variável flag para depois enfilar a variável sem buscar novamente
			body *p = v[i].next;
			while(p != NULL)
			{
				p->cor = 1;
				p->d = -1;
				p->pi = -1;
				p = p->next;
			}
		}
	}
	inicializaFila();
	enfila(v[numS]);

	while(vazia() == 0) //enquanto a fila não estiver vazia
	{
		x = desenfila();
		body *p, *q; //p percorre os vizinhos de x; q percorre todo o grafo atualizando os vértices representantes de p;
		int j;
		i=0;
		while(v[i].id != x.id) //procurar a variável para trabalhar com seus vizinhos
			i++;
		p = v[i].next; //percorrendo os vizinhos na lista de adjacência
		while(p != NULL)
		{
			if(p->cor == 1)
			{
				p->cor = 2;
				p->d = (x.d) + 1;
				p->pi = x.id;
			
				for(j=0; j<y; j++)
				{
					if(j != i)
					{
						if(v[j].id == p->id)
						{
							v[j].cor = 2;
							v[j].d = (x.d) + 1;
							v[j].pi = x.id;
						}
						else
						{
							q = v[j].next;
							while(q != NULL)
							{
								if(q->id == p->id)
								{
									q->cor = 2;
									q->d = (x.d) + 1;
									q->pi = x.id;
								}
								q = q->next;
							}
						}
					}
				}

				body e;
				e.id = p->id;
				e.next = p->next;  //gambiarra feita pq não dá para enfilar o p direto, por ser um ponteiro;
				e.cor = p->cor;
				e.d = p->d;
				e.pi = p->pi;
				enfila(e);
			}
			p = p->next;
		}
		//agora precisa passar todos os representantes de x.id para a cor preta
		for(i=0; i<y; i++)
		{
			if(v[i].id != x.id)
			{
				p = v[i].next;
				while(p != NULL)
				{
					if(p->id == x.id)
						p->cor = 3;
					p = p->next;
				}
			}
			else
				v[i].cor = 3;
		}
	}
	if(t == 1)
	{
		//printar a tabela para a busca em largura
		for(i=0; i<y; i++)
		{
			printf("Vértice: %d  Pi: %d  Distância: %d\n", v[i].id, v[i].pi, v[i].d);
		}
	}
	else
	{
		int black = 1;
		for(i=0; i<y; i++)
			if(v[i].cor != 3)
				black = 0;

		if(black == 1)
			printf("É conexo\n\n");
		else
			printf("Não é conexo\n\n");
	}
}

int profVisita(int y, body *v, int *tempo, int vert)
{
	int i, ind;

	for(i=0; i<y; i++) //procurando o índice do vértice passado por parâmetro
	{
		if(v[i].id == vert)
		{
			ind = i;
			break;
		}
	}
	
	v[ind].cor = 2;

	for(i=0; i<y; i++) //passando todos os representantes do vértice nas outras listas para a cor cinza
	{
		if(v[i].id != v[ind].id)
		{
			body *p;
			p = v[i].next;
			while(p != NULL)
			{
				if(p->id == v[ind].id)
					p->cor = 2;
				p = p->next;
			}
		}
	}

	(*tempo)++;

	v[ind].d = *tempo;
	for(i=0; i<y; i++)//atualizando todos os representantes do vértice nas outras listas para o tempo
	{
		if(v[i].id != v[ind].id)
		{
			body *p;
			p = v[i].next;
			while(p != NULL)
			{
				if(p->id == v[ind].id)
					p->d = *tempo;
				p = p->next;
			}
		}
	}
	body *p;
	p = v[ind].next;
	while(p != NULL)
	{
		if(p->cor == 1)
		{
			p->pi = v[ind].id;
			for(i=0; i<y; i++)
			{
				if(v[i].id == p->id)
				{
					v[i].pi = v[ind].id;
					break;
				}
			}
			profVisita(y, v, tempo, p->id);
		}
		p = p->next;
	}

	v[ind].cor = 3;
	for(i=0; i<y; i++) //passando todos os representantes daquele vértice para a cor preta
	{
		if(v[i].id != v[ind].id)
		{
			p = v[i].next;
			while(p != NULL)
			{
				if(p->id == v[ind].id)
					p->cor = 3;
				p = p->next;
			}
		}
	}

	(*tempo)++;
	v[ind].f = *tempo;
	for(i=0; i<y; i++) //atualizando todos os representantes daquele vértice para o tempo
	{
		if(v[i].id != v[ind].id)
		{
			p = v[i].next;
			while(p != NULL)
			{
				if(p->id == v[ind].id)
					p->f = *tempo;
				p = p->next;
			}
		}
	}

	return *tempo;
}

void buscaProfundidade(int y, body *v)
{
	body *p;
	int i, tempo;

	for(i=0; i<y; i++) //definindo todos os vértices para cor branca e pi NIL
	{
		v[i].cor = 1;
		v[i].pi = -1;
		p = v[i].next;
		while(p != NULL)
		{
			p->cor = 1;
			p->pi = -1;
			p = p->next;
		}
	}

	tempo=0;
	for(i=0; i<y; i++)
		if(v[i].cor == 1)
			tempo = profVisita(y, v, &tempo, v[i].id);

	for(i=0; i<y; i++) //printando o resultado
	{
		printf("Vértice: %d  D[v]: %d  F[v]: %d  Pi[v]:  %d\n", v[i].id, v[i].d, v[i].f, v[i].pi);
	}
}

int pertence(int y, int x, body *q) //retorna 1 se o elemento pertence ao conjunto
{
	int i;
	int ret = 0;
	for(i=0; i<y; i++)
		if(x == q[i].id)
			ret = 1;

	return ret;
}

int w(int id1, int id2, int y, body *v) //retorna o peso de uma aresta
{
	int i;
	for(i=0; i<y; i++)
	{
		if(v[i].id == id1 || v[i].id == id2)
		{
			body *p;
			p = v[i].next;
			while(p != NULL)
			{
				if(p->id == id1 || p->id == id2)
					return p->peso;
				p = p->next;
			}
		}
	}
}

int min(body *q, int n) //retorna o índice do conjunto onde está o menor
{
	int i;
	int menor=999, indMenor = 0;

	for(i=0; i<n; i++)
	{	
		if(q[i].d < menor)
		{
			menor = q[i].d;
			indMenor = i;
		}
	}
	
	return indMenor;
}


void dijkstra(int y, body *v)
{
	int i, s;
	printf("Raíz:\n");	
	scanf("%d", &s);

	for(i=0; i<y; i++)
	{
		v[i].d = 999;
		v[i].pi = -1;
		if(v[i].id == s)
			v[i].d = 0;

		body *p = v[i].next;
		while(p != NULL)
		{
			if(p->id == s)
			{
				p->d = 0;
				p->pi = -1;
			}
			else
			{
				p->d = 999;
				p->pi = -1;
			}
			p = p->next;
		}
	
	}

	body q[y];
	int n = y;
	for(i=0; i<y; i++)
		q[i] = v[i];

	while(n>0)
	{
		body x;
		int b = min(q, n);
		x = q[b];

		for(i=b; i<n; i++)
			q[i] = q[i+1];
		n--;

		for(i=0; i<y; i++)
			if(v[i].id == x.id)
				b = i;

		body *z = v[b].next;
		while(z != NULL)
		{
			if(pertence(n, z->id, q) == 1 && (w(v[b].id, z->id, y, v) + v[b].d) < z->d)
			{
				z->d = w(v[b].id, z->id, y, v) + v[b].d;
				z->pi = v[b].id;
				for(i=0; i<y; i++)
				{
					if(v[i].id == z->id)
					{
						v[i].d = z->d;
						v[i].pi = z->pi;
					}
					else
					{
						body *w = v[i].next;
						while(w != NULL)
						{
							if(w->id == z->id)
							{
								w->d = z->d;
								w->pi = z->pi;
							}
							w = w->next;
						}
					}
				}
				for(i=0; i<n; i++)
				{
					if(q[i].id == z->id)
					{
						q[i].d = z->d;
						q[i].pi = z->pi;
					}
				}
			}
			z = z->next;
		}
	}

	for(i=0; i<y; i++)
		printf("Vértice: %d  Pi: %d  d: %d\n", v[i].id, v[i].pi, v[i].d);
} //dica: tentar oq foi feito com prim, zerar os valores de s mesmo dentro da lista. Antes tinha sido zerado só para o vetor v

void visualiza(int y, body *v);

void prim(int y, body *v)
{
	int i, r;

	printf("Raíz:\n");
	scanf("%d", &r);

	for(i=0; i<y; i++)
	{
		v[i].d = 999;
		v[i].pi = -1;
		if(v[i].id == r)
			v[i].d = 0;

		body *p;
		p = v[i].next;
		while(p != NULL)
		{
			p->d = 999;
			p->pi = -1;
			if(p->id == r)
				p->d = 0;
			p = p->next;
		}
	}

	body q[y];
	int n = y;

	for(i=0; i<y; i++)
		q[i] = v[i];

	while(n>0)
	{
		for(i=0; i<n; i++)
			printf("%d: (%d) - ",q[i].id, q[i].d);
		printf("\n");
		visualiza(y, v);
		getchar();
		body x;
		int b = min(q, n);
		x = q[b];

		for(i=b; i<n; i++)
			q[i] = q[i+1];
		n--;

		int ind;

		for(i=0; i<y; i++)
			if(v[i].id == x.id)
				ind = i;
		
		body *p;
		p = v[ind].next;

		while(p != NULL)
		{
			if(pertence(n, p->id, q) == 1 && w(v[ind].id, p->id, y, v) < p->d)
			{
				p->pi = v[ind].id;
				p->d = w(v[ind].id, p->id, y, v);
				for(i=0; i<y; i++)
				{
					if(v[i].id == p->id)
					{
						v[i].pi = p->pi;
						v[i].d = p->d;
					}
					else
					{
						body *w;
						w = v[i].next;
						while(w != NULL)
						{
							if(w->id == p->id)
							{
								w->pi = p->pi;
								w->d = p->d;
							}
							w = w->next;
						}
					}
				}
				for(i=0; i<n; i++)
				{
					if(q[i].id == p->id)
					{
						q[i].d = p->d;
						q[i].pi = p->pi;
					}
				}
			}
			p = p->next;
		}
	}
	for(i=0; i<y; i++)
		printf("Vértice: %d  Pi: %d  Key: %d\n", v[i].id, v[i].pi, v[i].d);
}

void grau(int y, body *v)
{
	int i, x, cont=0;

	printf("Vértice:\n");
	scanf("%d", &x);

	for(i=0; i<y; i++)
	{
		if(v[i].id == x)
		{
			body *p;
			p = v[i].next;
			while(p != NULL)
			{
				cont++;
				p = p->next;
			}
		}
	}
	printf("Grau do vértice: %d\n", cont);
}

void matriz(int y, body *v)
{
	int cont=0, i, j; //i = linhas  j = colunas

	for(i=0; i<y; i++)
	{
		body *p = v[i].next;
		cont = 0;
		while(p != NULL)
		{
			for(j=cont; j<y; j++)
			{
				if(j+1 == p->id)
				{
					printf("   %d", p->peso);
					cont = j+1;
					break;
				}
				else
					printf("   0");
			}
			p = p->next;
		}
		for(j = cont; j<y; j++)
			printf("   0");
		printf("\n");
	}
}
//--------------------------------------------------------//

//Funções parametrizadas
void visualiza(int y, body *v)
{
	int i;
	body *p; //variável que será usada para percorrer o grafo e ser printada com o índice correspondente
	for(i=0; i<y; i++)
	{
		p = v[i].next;
		printf("%d->", v[i].id); //printa a cabeça das listas (o primeiro vértice)
		while(p != NULL) //se existir arestas pra esse vértice...
		{
			printf("%d->", p->id); //...então ele a percorre e printa enquanto houver vértices conectados à cabeça
			p = p->next;
		}
		printf("\n");
	}
}

void insereAres(int y, body *v) //adiciona as arestas
{
	int id1, id2, weight; //0 se não existe, 1 se existe --> previne que o usuário adicione uma aresta já existente
	printf("Primeiro vértice:\n");
	scanf("%d", &id1);
	printf("Segundo vértice:\n");
	scanf("%d", &id2);
	printf("Peso:\n");
	scanf("%d", &weight);
	int i, existe=0;

	for(i=0; i<y; i++) //laço para verificar se a aresta já existe
	{
		if(v[i].id == id1 || v[i].id == id2)
		{
			body* p;
			p = v[i].next;
			while(p != NULL)
			{
				if(p->id == id1 || p->id == id2)
				{
					existe = 1; //muda o valor da variável existe de 0 para 1 se a aresta existe
					break; //o programa sai, então, do while
				}
				p = p->next;
			}
		}
		if(existe == 1) //se já foi verificado que a aresta existe, não é necessário percorrer o resto do vetor
		{
			printf("\nA aresta já existe!\n");
			break;
		}
	}

	if(existe == 0 && id1 != id2) //se a aresta não existe, chama a função e a adiciona
	{
		body *p; //variável de busca;
		int i;
		int existe=0; //0 quando não existe nenhuma, 1 quando só existe uma, 2 se existe as duas

		for(i=0; i<y; i++) //laço para verificar se os dois vértices existem
			if(v[i].id == id1 || v[i].id == id2)
				existe++;

		if(existe == 2) //se os dois vértices existem, então...
		{
			for(i=0; i<y; i++)
			{
				if(v[i].id == id1)
				{
					if(v[i].next == NULL) //se esse vértice não tiver nenhuma aresta, a nova aresta simplesmente é adicionada
					{
						body *t;
						t = malloc(sizeof(body));
						t->id = id2;
						t->next = NULL;
						t->peso = weight;
						v[i].next = t;
					}
					else //caso contrário...
					{
						p = v[i].next;
						while(p->next != NULL) //a variável de busca percorre até que encontre o último vértice conectado
							p = p->next;
						body *t;
						t = malloc(sizeof(body));
						p->next = t;
						t->id = id2;
						t->next = NULL;
						t->peso = weight;
					}
				}
				else if(v[i].id == id2) //mesmo modelo anteriormente explicado, mas buscando agora para o outro vértice
				{
					if(v[i].next == NULL)
					{
						body *t;
						t = malloc(sizeof(body));
						t->id = id1;
						t->next = NULL;
						t->peso = weight;
						v[i].next = t;
					}
					else
					{
						p = v[i].next;
						while(p->next != NULL)
							p = p->next;
						body *t;
						t = malloc(sizeof(body));
						p->next = t;
						t->id = id1;
						t->next = NULL;
						t->peso = weight;
					}	
				}
			}
		}
		else
			printf("Os dois vértices precisam existir!\n");
	}
	if(id1 == id2)
		printf("Não são permitidos criar laços!\n");
}

int removeVert(int y, body *v)
{
	int x, i;
	body *p, *q;
	printf("Vértice:\n");
	scanf("%d", &x);
	int existe=0; //0 se o vértice não existe, 1 se existe

	for(i=0; i<y; i++) //laço para verificar se o vértice existe
		if(v[i].id == x)
			existe=1;

	if(existe == 1) //se o vértice existe, então é possível removê-lo
	{
		for(i=0; i<y; i++) //esse laço vai tirar o vértice de cada cabeça
		{
			if(v[i].next != NULL)
			{
				p = v[i].next;
				if(p->id == x)
				{
					q = p;
					p = p->next;
					v[i].next = p;
					free(q);
				}
				else
				{
					q = p->next;
					while(q != NULL)
					{
						if(q->id == x)
						{
							p->next = q->next;
							free(q);
						}
						p = q;
						q = q->next;
					}
				}
			}
		}
		for(i=0; i<y; i++) // esse laço vai retirar a cabeça correspondente àquele vértice
		{
			if(v[i].id == x)
			{
				for(; i<y; i++)
				{
					v[i] = v[i+1];
				}
			}
		}
		y--;
		v = realloc(v, y*sizeof(body));
		return y;
	}
	else //se o vértice não existir, emitir o alarme
		printf("O vértice não existe!\n");
	return y;
}

void removeArest(int y, body *v)
{
	int id1, id2, i, existe=0; //0 se a aresta não existe, 1 se existe 
	body *p, *q;
	printf("Vértice 1:\n");
	scanf("%d", &id1);
	printf("Vértice 2:\n");
	scanf("%d", &id2);

	for(i=0; i<y; i++)
	{
		if(v[i].id == id1)
		{
			p = v[i].next;
			while(p != NULL)
			{
				if(p->id == id2)
				{
					existe = 1;
					break;
				}
				p = p->next;
			}
		}
	}

	if(existe == 1) //se a aresta existe, é possível removê-la
	{
		for(i=0; i<y; i++)
		{
			if(v[i].id == id1)
			{
				p = v[i].next;
				if(p->id == id2)
				{
					q = p->next;
					v[i].next = q;
					free(p);
				}
				else
					while(1<2)
					{
						q = p->next;
						if(q->id == id2)
						{
							p->next = q->next;
							free(q);
							break;
						}
						p = p->next;
						}

			}
			if(v[i].id == id2)
			{
				p = v[i].next;
				if(p->id == id1)
				{
					q = p->next;
					v[i].next = q;
					free(p);
				}
				else
					while(1<2)
					{
						q = p->next;
						if(q->id == id1)
						{
							p->next = q->next;
							free(q);
							break;
						}
						p = p->next;
					}
			
			}
		}
	}
	else //se a aresta não existir, emitir o alarme
		printf("A aresta não existe!\n");
}

int main()
{
	int option;

	setlocale(LC_ALL, "Portuguese");
	system("clear"); //limpa a tela do terminal assim que o programa é executado

	printf("Bem vindo ao Graph's Maker!\n");

	printf("1 - Iniciar\n");
	printf("0 - Sair\n");
	scanf("%d", &option);
	system("clear");

	if(option == 1) //programa inicia aqui e roda todo dentro desse if
	{
		int y=0; //número atual de vértices - necessário devido a escolha do programador de nomear automaticamente os vértices em
		body *v; //v de vetor 																					ordem crescente
		body *b;

		while(option != 0)
		{
			printf("1 - Inserir vértice\n");	    //
			printf("2 - Inserir aresta\n");		    //
			printf("3 - Visualizar grafo\n");       //		Menu do programa
			printf("4 - Remover vértice\n");	    //
			printf("5 - Remover aresta\n");	        //
			printf("6 - Busca em largura\n");	    //
			printf("7 - Busca em profundidade\n");  //
			printf("8 - Caminho mínimo\n");		    //
			printf("9 - Árvore Geradora Mínima\n"); //
			printf("10 - Grau de um vértice\n");	//
			printf("11 - Matriz de Adjacência\n");  //
			printf("12 - É conexo?\n");			    //
			printf("0 - Sair\n");				    //

			scanf("%d", &option);

			if(option == 1) //insere vértice
			{
				int x; //parâmetro para o id do vértice
				int i;
				int existe=0; //0 se o vértice não existe, 1 se existe

				printf("Id do vértice que deseja colocar:\n");
				scanf("%d", &x);

				for(i=0; i<y; i++) //laço para verificar se o vértice já existe
					if(v[i].id == x)
					{
						existe=1;
						break;
					}


				if(existe == 0) //se o vértice não existe, então é possível inserí-lo
				{
					if(y == 0) //como não existem vértices, usa-se apenas o malloc
					{	
						v = malloc((++y)*sizeof(body));
						v[y-1].id = x;
						v[y-1].next = NULL;
					}
					else if(y != 0) //como já existem vértices, usa-se o realloc
					{
						v = realloc(v, (++y)*sizeof(body));
						v[y-1].id = x;
						v[y-1].next = NULL;
					}
				}
				else //se o vértice já existe, emitir a alerta
					printf("O vértice já existe!\n");
			}

			else if(option == 2) //insere arestas
			{
				insereAres(y, v);
			}

			else if(option == 3) //visualiza o grafo
				visualiza(y, v);

			else if(option == 4) //remove um vértice
			{
				y = removeVert(y, v);
			}

			else if(option == 5) //remove arestas - basicamente consiste em percorrer a lista com duas variáveis 
			{					 // e quando for encontrado a aresta, a variável anterior tem seu ponteiro alterado
								//são necessários duas variáveis devido ao fato da lista não ser duplamente encadeada
				removeArest(y, v);
			}

			else if(option == 6)
			{
				buscaLargura(y, v, 1);
			}

			else if(option == 7)
			{
				buscaProfundidade(y, v);
			}

			else if(option == 8)
			{
				dijkstra(y, v);
			}

			else if(option == 9)
			{
				prim(y, v);
			}

			else if(option == 10)
			{
				grau(y, v);
			}

			else if(option == 11)
			{
				matriz(y, v);
			}

			else if(option == 12)
			{
				buscaLargura(y, v, 2);
			}

			else if(option == 0) //sai do programa
			{
				int i;
				body *p, *q;
				for(i=0; i<y; i++)
				{
					q = v[i].next;
					while(q != NULL)
					{
						v[i].next = q->next;
						free(q);
						q = v[i].next;
					}
				}
				free(v);
				return EXIT_SUCCESS;
			}
		}
	}
	else
		return EXIT_SUCCESS;
}