#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>

// estrutura de registro em arquivo

typedef struct registro{
	char nome[20];
	int vit, der;
} Registro;

// define cores

#define ANSI_COLOR_PURPLE   "\e[0;35m" //cores em ANSI utilizadas 
#define ANSI_COLOR_GREEN     "\e[0;32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// my sleep =====================================================================================================

#if defined(_WIN32)
    #define WINDOWS_SYSTEM
    #include <Windows.h>
#else
    #define UNIX_SYSTEM
    #include <unistd.h>
#endif

//variáveis globais =============================================================================================

char tabuleiro[3][3];


//funcoes =======================================================================================================

void my_sleep(int sleepms);

int algarismos(int numero);

int diagonal(int i, int j, char tipo, char oponente);
int coluna(int j, char tipo, char oponente);
int linha(int i, char tipo, char oponente);

int probabilidade(int i, int j);
int modificadores(int *i, int *j, int rodada);
int BOT(int rodada);

int verifica_vitoria(int rodada);
int verifica_casa(int escolha);

void zerar_tabuleiro();
void imprimir_tabuleiro();

void ordenar_ranking(Registro ranking[], int tamanho);
void imprimir_ranking(Registro ranking[], int tamanho);

int moeda(char cara_coroa);
int anima_moeda(char cara_coroa);

//main ==========================================================================================================

int main(){
	int novo_jogo, rodada=0, opcao, voltar, primeiro, escolha, quant_jogadores=0, k; // variáveis da main
	char nome_temp[20], cara_coroa;
	Registro jogador, temp, temporario;
	
	setlocale(LC_ALL, "");
	
	srand(time(NULL));

    //recebe nome do jogador
    printf("===== Bem-Vindo ao Jogo da Velha Invencível =====");
    printf("\n\n>>>>> Insira seu nome: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]s", nome_temp);
    
    FILE * f_hist = fopen("historico.txt","r+");
    
    if(f_hist == NULL){
    	f_hist = fopen("historico.txt","w+");
		if(f_hist == NULL){
			printf("Erro ao recuperar o historico de partidas\n");
			return 0;
		}
 	}
	
	rewind(f_hist);
    
	bool flag = false; //parto do principio que nao foi encontrado no historico
    while(!flag && fscanf(f_hist, "%20[^,], %d, %d\n", jogador.nome, &jogador.vit, &jogador.der) != EOF){
    	if(!strcmp(jogador.nome, nome_temp))
    		flag = true;
	}
	
	if(!flag){ 
		strcpy(jogador.nome, nome_temp);
		jogador.vit = jogador.der = 0;
		printf("Parece que e a sua primeira vez aqui! Esperamos que se divirta\n A aventura começará em... ");
		int i;
		for(i=5; i>1; i--){
			printf("%d, ", i);
			my_sleep(1000);
		}
	}
    
    rewind(f_hist);
    
    system("cls"); // limpa o terminal

   	printf(" ====== Olá, %s =====", jogador.nome);
   	printf("\n\nInstruções - Para jogar, escolha entre as opções apresentadas digitando o número correspondente, em seguida aperte a tecla enter!");
   	printf(ANSI_COLOR_GREEN "\n>>>>>> Você é X" );
	printf(ANSI_COLOR_RESET);
   	printf("\n\nAguarde, o jogo iniciará em breve ;)");
   	my_sleep(5000);
    
    do{
    	rewind(f_hist);
    	
    	while(fscanf(f_hist, "%20[^,], %d, %d\n", temporario.nome, &temporario.vit, &temporario.der) != EOF){
			quant_jogadores++;
		}
		Registro ranking[quant_jogadores];
		
		system("cls");
	   	printf("\n =================== MENU ===================");
		printf("\n\n                  1 - Jogar");
	    printf("\n                 2 - Ranking");
	    printf("\n                  3 - Sobre");
	    printf("\n                  0 - Sair\n");
	
	    printf("\nEscolha uma opção: ");
	    setbuf(stdin, NULL);
	    scanf("%d", &opcao);
		
	    switch(opcao){
	    //jogar
	    case 1:
	    	do{
				system("cls"); // limpa o terminal
				
				do{
			    	printf("Para decidir quem começa, vamos rodar uma moeda. \nEscolha: cara ou coroa? (digite 1 para Cara 2 para Coroa): ");
			    	scanf(" %c", &cara_coroa);
			    	
			    	primeiro = anima_moeda(cara_coroa);
			    	my_sleep(3000);
			    	system("color 07");
			    	
			    	rodada = 0; //zera as rodadas
					zerar_tabuleiro(); // limpa o tabuleiro
			    	system("cls"); // limpa o terminal
			    	
			    	if(!primeiro){
			    		system("cls");
			    		printf("\nEscolha inválida. Escolha novamente!");
					}
					
				}while(!primeiro);
				
				if(primeiro==2){
					//rodada valida iniciada
					rodada++;
					//jogo da IA
					BOT(rodada);
				}
				
				//jogo comecou
				do{
					//escolha de casa	
					do{
						rodada++;
						system("cls"); // limpa o terminal
						printf("\n ========== Rodada %d ==========\n", rodada);
						printf("\nEscolha a casa na qual deseja jogar (1-9)\n");
						
						imprimir_tabuleiro();
						
						printf("\nCasa escolhida: ");
						setbuf(stdin, NULL);
						scanf(" %d", &escolha);
					}while(!verifica_casa(escolha)); //caso a casa escolhida nao exista ou esteja ocupada, volta para que o jogador escolha novamente
					
					if(verifica_vitoria(rodada)==0){
						//rodada valida iniciada
						rodada++;
						//jogo da IA
						BOT(rodada);
					}
					
				}while(verifica_vitoria(rodada)==0);
				//jogo acabou
			
				system("cls");
				imprimir_tabuleiro();
				
				switch (verifica_vitoria(rodada)){
					case 1: //vitoria da IA
						system("color 05");
						printf("\n\n            Vitória do computador!\n");
						jogador.der++;
						break;
						
					case 2: //vitoria do Jogador
						system("color 02");
						printf("\n\n            Vitória do jogador!\n");
						jogador.vit++;
						break;
						
					default: //velha
						system("color 06");
						printf("\n\n            Velha!\n");
						break;	
				}
				
				my_sleep(3000);
					
				// FIM DA PARTIDA
				printf("\nDeseja jogar novamente?\n");
				printf("1 - Sim\n");
				printf("0 - Não\n");
				scanf("%d", &novo_jogo);
				system("color 07");
				
			}while(novo_jogo);
			
			//atualização do arquivo histórico
		
			rewind(f_hist);
			
			flag = false;
			while(!flag && fscanf(f_hist, "%20[^,], %d, %d\n", temp.nome, &temp.vit, &temp.der) != EOF){
				if(!strcmp(jogador.nome, temp.nome)){
					flag=true;
				}
			}
			if(flag){
				fseek(f_hist,-(strlen(jogador.nome)+algarismos(jogador.vit)+algarismos(jogador.der)+5), SEEK_CUR);	
			}
			
			fprintf(f_hist,"%s,%d,%d\n",jogador.nome, jogador.vit, jogador.der);
			
			rewind(f_hist);
			
			break;
		
		//Ranking
	    case 2:
	        k=0;
	        
 			rewind(f_hist);
 			
			while(fscanf(f_hist, "%20[^,], %d, %d\n", temporario.nome, &temporario.vit, &temporario.der) != EOF){
				strcpy(ranking[k].nome, temporario.nome);
				ranking[k].vit = temporario.vit;
				ranking[k].der = temporario.der;
				k++;
			}
			
			ordenar_ranking(ranking, k);
			imprimir_ranking(ranking, k);
			my_sleep(2000);
			
			printf("\n\nDigite qualquer número inteiro para voltar ao menu: ");
	        scanf("%d", &voltar);
	        system("cls"); // limpa o terminal
	        break;
			    
	    //Sobre
	    case 3:
	        system("cls"); // limpa o terminal
	        printf("===== Informações do Jogo =====");
	        printf("\n\nNome: Jogo da Velha Invencível");
	        printf("\nVersão: 1.0");
	        printf("\nCriadores: Ana Laura Neuber de Castro Carvalho");
	        printf("\n           Bianca Mitie Menezes de Azeredo\n");
	        printf("\nObjetivo: Este jogo foi criado com o objetivo de cumprir, da melhor forma possível, o Trabalho de Conclusão de Disciplina de\nProgramação Procendimental.Foi solicitado pelos Prof. Me. Claudiney R. Tinoco e Prof. Dra Amanda C. Davi Resende.\n");
	        printf("\n===== Sobre o jogo da velha =====");
	        printf("\n O jogo da velha ou jogo do galo ou três em linha é um jogo e/ou passatempo popular. É um jogo de regras extremamente simples, que não traz grandes dificuldades para seus jogadores e é facilmente aprendido. A origem é desconhecida, com indicações de que pode ter começado no antigo Egito. O objetivo do jogo da velha é fazer uma sequência de três símbolos iguais, seja em linha vertical, horizontal ou diagonal, enquanto tenta impedir que seu adversário faça o mesmo. Quando um dos participantes faz uma linha, ganha o jogo.\n");
	
	        printf("\n\nDigite qualquer número inteiro para voltar ao menu: ");
	        scanf("%d", &voltar);
	        system("cls"); // limpa o terminal
	        break;
	        
	    //Sair 
	    case 0:
			system("cls"); // limpa o terminal
	        printf("\nObrigada por Jogar!\n");
	        break;
	        
	    //Erro
	    default:
	        printf("\nOpção inválida, escolha novamente!\n");
	    	my_sleep(5000);
	        system("cls"); // limpa o terminal
	        break;
	    }
	    
	}while(opcao);
	
	fclose(f_hist);
	
	return 0;
}

//funcoes =============================================================================================

int diagonal(int i, int j, char tipo, char oponente){
	int k, l, contador=0, contador_oponente=0;
	
	if(i==j){
		for(k=0; k<3; k++){
			for(l=0; l<3; l++){
				if(k==l && tabuleiro[k][l]==tipo){
					contador++;
				}
				else if(k==l && tabuleiro[k][l]==oponente){
					contador_oponente++;
				}
				
			}
		}	
	}
	if((i+j)==2){
		for(k=0; k<3; k++){
			for(l=0; l<3; l++){
				if((k+l)==2 && tabuleiro[k][l]==tipo){
					contador++;
				}
				else if((k+l)==2 && tabuleiro[k][l]==oponente){
					contador_oponente++;
				}
			}
		}
	}
	
	if(contador == 2){
		contador = 4;
	}
	
	return contador;
}

int coluna(int j, char tipo, char oponente){
	int k, contador=0, contador_oponente=0;
	
	for(k=0; k<3; k++){
		if(tabuleiro[k][j] == tipo){
			contador++;
		}
		else if(tabuleiro[k][j]==oponente){
			contador_oponente++;
		}
	}
	
	if(contador == 2 && tipo=='o'){
		contador = 10;
	}
	else if(contador == 2 && tipo=='x'){
		contador = 4;
	}
	if(contador == 1 && contador_oponente == 1){
		contador = 0;
	}
	
	return contador;
}

int linha(int i, char tipo, char oponente){
	
	int k, contador=0, contador_oponente=0;
	
	for(k=0; k<3; k++){
		if(tabuleiro[i][k] == tipo){
			contador++;
		}
		else if(tabuleiro[i][k]==oponente){
			contador_oponente++;
		}
	}
	
	if(contador == 2){
		contador = 4;
	}
	if(contador == 1 && contador_oponente == 1){
		contador = 0;
	}
	
	return contador;
}

int probabilidade(int i, int j){
	int prob_x=0, prob_o=0;
	if(tabuleiro[i][j]!= 'o' && tabuleiro[i][j]!='x'){
		prob_x= linha(i, 'x', 'o')+coluna(j, 'x', 'o')+diagonal(i, j, 'x', 'o');
		prob_o= linha(i, 'o', 'x')+coluna(j, 'o', 'x')+diagonal(i, j, 'o', 'x');
	}		
	else{			
		prob_x=-10;
		prob_o=-10;
	}
	return prob_x+prob_o;
}

int modificadores (int *i, int *j, int rodada){
	
	if((rodada==1 || rodada==2) && tabuleiro[1][1]!='x' && tabuleiro[1][1]!='o'){
		*i = 1;
		*j = 1;
		return 1;
	}
	if(rodada==3 && diagonal(0,0,'x', 'o')==1){
		int j_x=0, i_o=0, i_x=0, k, l;
		bool flag = true;
		for(k=0; flag && k<3; k++){
			for(l=0; flag && l<3; l++){
				if(tabuleiro[k][l]=='x'){
					if(k==l){
						*i = 2-k;
						*j = 2-l;
					}
					else{
						*i = k;
						*i = l;
					}
					flag = false;
				}
			}
		}
		return 1;
	}
	if(rodada==4 && ((diagonal(0,0,'o','x')==1 && diagonal(0,0,'x', 'o')==4) || (diagonal(0,2,'o', 'x')==1 && diagonal(0,2,'x', 'o')==4 ))){
		int i_x=0, j_o=0, k, l;
		for(k=0; k<3; k++){
			for(l=0; l<3; l++){
				if(k!=1 || l!=1){
					if(tabuleiro[k][l]=='x'){
						i_x=k;	
					}
				}
				if(tabuleiro[k][l]=='o'){
					j_o=l;
				}	
			}
		}
		*i = i_x;
		*j = j_o;
		return 1;
	}
	return 0;
}

int BOT(int rodada){
	int prob[3][3] = {{0,0,0},{0,0,0},{0,0,0}}, i, j, maior_linha, maior_coluna, maior;
	
	//probabilidade das casas
	
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			prob[i][j]=probabilidade(i,j);
		}
	}
	
	//jogada dicionário 
	
	if(rodada<=4){
		int mod_i, mod_j, *p_i, *p_j;
		p_i=&mod_i;
		p_j=&mod_j;
		if(modificadores(p_i, p_j, rodada)){
			prob[mod_i][mod_j]=20;
		}
	}

	//analisar casa de maior probabilidade
	
	maior = prob[0][0];
	maior_linha=0;
	maior_coluna=0;
	
	for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			if(prob[i][j]>maior){
				maior_linha = i;
				maior_coluna = j;
				maior = prob[i][j];
			}
		}
	}
	
	tabuleiro[maior_linha][maior_coluna]='o';
	
	return 0;
}

int verifica_vitoria(int rodada){
	int i, j, pontos_o_linha[3] = {0,0,0}, pontos_o_coluna[3] = {0,0,0}, pontos_o_diagonal[2] = {0,0}, pontos_x_linha[3] = {0,0,0}, pontos_x_coluna[3] = {0,0,0}, pontos_x_diagonal[2] = {0,0};
	//verifica linhas
	for(i=0; i<3;i++){
		for(j=0; j<3; j++){
			if(tabuleiro[i][j] == 'o'){
				if(++pontos_o_linha[i]==3) return 1;
				
				if(++pontos_o_coluna[j]==3)	return 1;
				
				if(i==j)
					if(++pontos_o_diagonal[0]==3) return 1;
				
				if(i+j==2)
					if(++pontos_o_diagonal[1]==3) return 1;
			}
			else if(tabuleiro[i][j] == 'x'){
				if(++pontos_x_linha[i]==3) return 2;
				
				if(++pontos_x_coluna[j]==3)	return 2;
				
				if(i==j)
					if(++pontos_x_diagonal[0]==3) return 2;
				
				if(i+j==2)
					if(++pontos_x_diagonal[1]==3) return 2;
			}
		}
	}
	
	if(rodada == 9)	return 3;
	
	return 0;
}

void imprimir_tabuleiro(){
	int i, j;
	printf("           \n");
	for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			if(j==0){
				printf("          ");
			}
			if(tabuleiro[i][j]=='o'){
				printf(ANSI_COLOR_PURPLE "  O  " );
				printf(ANSI_COLOR_RESET);
			}
			else if(tabuleiro[i][j]=='x'){
				printf(ANSI_COLOR_GREEN "  X  " );
				printf(ANSI_COLOR_RESET);
			}
			else{
				printf("  %c  ", tabuleiro[i][j]);
			}
			
			if(j<2){
				printf("|");
			}
		}
		if(i==0){
			printf("\n          _____|_____|_____\n               |     |    \n");	
		}
		else if(i==1){
			printf("\n          _____|_____|____");
			printf("\n               |     |    \n");
		}
		else{
			printf("\n              ");
		}		
	}
}

void zerar_tabuleiro(){
	int i, j, k=0;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			tabuleiro[i][j] = 49 + (k);
			k++;
		}
	}
}

int verifica_casa(int escolha){
	escolha--;
	
	if(tabuleiro[escolha/3][escolha%3] != 'x' && tabuleiro[escolha/3][escolha%3] != 'o'){
		tabuleiro[escolha/3][escolha%3] = 'x';
		return 1;
	}
		
	printf("Casa invalida! escolha novamente.\n");
	my_sleep(2000);
	return 0;
}

int moeda(char cara_coroa){
	int numero;
	
	numero = rand();
	
	if(numero%2==0){
		if(cara_coroa == '1'){
			return 1;
		}
		else{
			return 2;
		}
	}
	else{
		if(cara_coroa == '2'){
			return 1; //jogador comeca
		}
		else{
			return 2; // computador comeca
		}
	}
	
	return 0;
}

int anima_moeda(char cara_coroa){
	int resultado=0, subir=0, funcao=moeda(cara_coroa), aux=0;
	
	system("color 06");
	
	if(cara_coroa=='1'){
		if(funcao==1){
			resultado =1;
		}
		else{
			resultado = 2;
		}
	}
	else{
		if(funcao==1){
			resultado = 2;
		}
		else{
			resultado = 1;
		}
 }
	
	for(aux=0; aux<3; aux++){
		for(subir=3; subir>aux && aux == 0; subir--){
			printf("\n");
		}
		printf("                    ____ ");
		printf("\n                   /    \\     ");
		printf("\n                  | cara |    ");
		printf("\n                   \\____/ ");
		my_sleep(200);
		system("cls");
		printf("                   _____");
		printf("\n                  /     \\    ");
		printf("\n                 |\\_____/| ");
		printf("\n                  \\_____/ ");
		my_sleep(200);
		system("cls");
		printf("                   _____  ");
		printf("\n                  |_____|    ");
		my_sleep(200);
		system("cls");
		printf("                   _____");
		printf("\n                  /     \\    ");
		printf("\n                 |\\_____/| ");
		printf("\n                  \\_____/ ");
		my_sleep(200);
		system("cls");
		printf("                    ____ ");
		printf("\n                   /    \\     ");
		printf("\n                  |coroa |    ");
		printf("\n                   \\____/ ");
		my_sleep(200);
		system("cls");
		printf("                   _____  ");
		printf("\n                  |_____|    ");
		my_sleep(200);
		system("cls");
		printf("                   _____");
		printf("\n                  /     \\    ");
		printf("\n                 |\\_____/| ");
		printf("\n                  \\_____/ ");
		my_sleep(200);
		system("cls");
	}
	
	printf("                    ____ ");
	printf("\n                   /    \\     ");
	
	if(resultado == 1){
		printf("\n                  | cara |    ");	
	}
	else if(resultado == 2){
		printf("\n                  | coroa|    ");	
	}
	
	printf("\n                   \\____/ \n\n");
	
	if(resultado == 1){
		printf("\n                  CARA!    ");	
	}
	else if(resultado == 2){
		printf("\n                  COROA!    ");	
	}
	
	if(funcao==1){
		printf("\n                  Você começa!", funcao, resultado);
	}
	else{
		printf("\n                  O computador começa!", funcao, resultado);
	}

	return funcao;
	
}

void ordenar_ranking(Registro ranking[], int tamanho){
	int menor, i, j;
	Registro auxiliar;
	
	for(i=0; i<tamanho; i++){
		menor=i;
		for(j=i;j<tamanho;j++){
			if(ranking[j].der<ranking[menor].der){
				menor = j;
			}
		}
		
		auxiliar = ranking[i];
		ranking[i] = ranking[menor];
		ranking[menor] = auxiliar;
	}
}

void imprimir_ranking(Registro ranking[], int tamanho){
	int l, tamanho_nome, aux;
	system("cls");
	printf("\n          ===== RANKING =====");
	printf("\n * |        NOME        | Vitórias | Derrotas |\n");
	
	for(l=0; l < tamanho; l++){
		tamanho_nome = 20 - strlen(ranking[l].nome);
		tamanho_nome /= 2;
		printf("%dº |", l+1);
		for(aux = 0; aux<tamanho_nome; aux++){
			printf(" ");
		}
		printf("%s",ranking[l].nome);
		for(aux = 0; aux<tamanho_nome; aux++){
			printf(" ");
		}
		if((20 - strlen(ranking[l].nome))%2){
			printf(" ");
		}
		printf("|    %d     |    %d     |\n",ranking[l].vit, ranking[l].der);
	}
}

void my_sleep(int sleepms){
    #ifdef WINDOWS_SYSTEM
        Sleep(sleepms);
    #else
        usleep(sleepms * 1000);
    #endif
}

int algarismos(int numero){
	int i;
	for(i=0; numero>0; i++){
		numero/=10;
	}
	return i;
}
