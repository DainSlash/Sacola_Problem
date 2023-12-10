//Includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

//Defines
#define tamanho_carrinho 8
#define tamanho_sacola 30
#define area_sacola tamanho_sacola*tamanho_sacola
#define valor_maximo 100


//Variaveis globais
typedef struct{
    unsigned int preco : 8;
    unsigned int tam_x : 8, tam_y : 8;
}item_type;

item_type item_list[] = {
    {3,15,15},
    {5,10,10},
    {6,7,4},
    {8,7,2},
    {10,6,4},
    {12,5,5},
    {20,3,1},
    {30,1,1}
};

FILE *arquivo_log;


//Escopo de funções
void gravar_Log(const char*);
void carrinho_Gerar(unsigned int []);
bool carrinho_Validar(unsigned int []);
int carrinho_Preco(unsigned int []);
int carrinho_Tamanho(unsigned int []);
void carrinho_Print(unsigned int []);
void carrinho_Print_Log(unsigned int []);
void melhor_Solucao(unsigned int []);
bool sacola_Coube(unsigned int [], int [tamanho_sacola][tamanho_sacola]);
bool sacola_Valida_Posiciona(unsigned short int, int [tamanho_sacola][tamanho_sacola]);
bool sacola_Valida(unsigned short int, int [tamanho_sacola][tamanho_sacola], unsigned short int i, unsigned short int j);
void sacola_Posiciona(unsigned short int, int [tamanho_sacola][tamanho_sacola], unsigned short int i, unsigned short int j);
int sacola_Area(int [tamanho_sacola][tamanho_sacola]);

void sacola_Print(int [tamanho_sacola][tamanho_sacola]);
void sacola_Print_Log(int [tamanho_sacola][tamanho_sacola]);


//Funções
int main(){
    unsigned int carrinho[tamanho_carrinho];
    //  melhor_solucao[tamanho_carrinho];
    
    //LIMPA LOG.
    arquivo_log = fopen("log.txt","w");
    fclose(arquivo_log);
    
    carrinho_Gerar(carrinho);
    melhor_Solucao(carrinho);

    return 0;
}


// Geração de carrinho //

void carrinho_Gerar(unsigned int carrinho[]){
    int i, qtd=0;
    do{
        srand(time(NULL)+(++qtd));
        for(i=0;i<tamanho_carrinho;i++) carrinho[i] = (rand()%8)+1;

    }while(carrinho_Validar(carrinho));
}

bool carrinho_Validar(unsigned int carrinho[]){
    int preco = carrinho_Preco(carrinho), tamanho = carrinho_Tamanho(carrinho);
    bool valor = preco > valor_maximo, area = tamanho > area_sacola;
    char log[30] = "";

    gravar_Log("\nSolucao Atual: ");
    carrinho_Print_Log(carrinho);
    if(valor){
        sprintf(log,"EXCEDEU O VALOR\nVALOR: %i\n\n",preco);    
        gravar_Log(log);
    } 
    if(area){
        sprintf(log,"EXCEDEU A AREA\nAREA: %i\n\n",tamanho);
        gravar_Log(log);
    } 

    return(valor || area);
}

int carrinho_Preco(unsigned int carrinho[]){
    int i, total = 0;
    for(i=0;i<tamanho_carrinho;i++) total+= item_list[carrinho[i]-1].preco;
    return total;
}

int carrinho_Tamanho(unsigned int carrinho[]){
    int i, total = 0;
    for(i=0;i<tamanho_carrinho;i++) total+= (item_list[carrinho[i]-1].tam_x) * (item_list[carrinho[i]-1].tam_y);
    return total;
}

void carrinho_Print(unsigned int carrinho[]){
    int i;
    for(i=0;i<tamanho_carrinho;i++) printf("%i ", carrinho[i]);
}

// Geração de carrinho //


// Organizar na sacola //

void melhor_Solucao(unsigned int carrinho[]){
    int sacola[tamanho_sacola][tamanho_sacola], area;
    memset(sacola, 0, tamanho_sacola * tamanho_sacola * sizeof(int));



    if(!sacola_Coube(carrinho, sacola)) gravar_Log("\nNão foi possivel encaixar todos os itens na sacola.\n");
    else gravar_Log("\nCoube tudo na sacola! :D\n");
    sacola_Print_Log(sacola);
    area = sacola_Area(sacola);



}


bool sacola_Coube(unsigned int carrinho[], int sacola[tamanho_sacola][tamanho_sacola]){
    int i;
    bool coube;
    for(i=0;i<tamanho_carrinho;i++){
        coube = sacola_Valida_Posiciona(carrinho[i], sacola);
        if(!coube) return false;
    }
    return true;
}


bool sacola_Valida_Posiciona(unsigned short int index, int sacola[tamanho_sacola][tamanho_sacola]){
    index-=1;
    int i, j;
    bool vazio, valido;

    for(i=0;i<tamanho_sacola;i++){
        for(j=0;j<tamanho_sacola;j++){
            vazio = sacola[i][j] == 0;
            if(vazio){
                valido = sacola_Valida(index, sacola, i, j);
                if(valido){
                    sacola_Posiciona(index ,sacola, i, j);
                    return true; 
                }else break;
            }
        }
    }
    return false;
}

bool sacola_Valida(unsigned short index, int sacola[tamanho_sacola][tamanho_sacola], unsigned short int i, unsigned short int j){
    unsigned short tam_x = item_list[index].tam_x, tam_y = item_list[index].tam_y, x, y;
    bool valido;
    if(tam_x+j<=30 && tam_y+i<=30){
        for(y=0;y<tam_y;y++)
            for(x=0;x<tam_x;x++){
                valido = sacola[i+y][j+x] == 0;
                if (!valido){
                    return false;
                };
            }
        return true;
    }else return false;
}

void sacola_Posiciona(unsigned short int index, int sacola[tamanho_sacola][tamanho_sacola], unsigned short i, unsigned short j){
    unsigned short tam_x = item_list[index].tam_x, tam_y = item_list[index].tam_y, x, y;
    for(y=0;y<tam_y;y++){
        for(x=0;x<tam_x;x++){
            sacola[i+y][j+x] = index+1;
        }
    }
}

int sacola_Area(int sacola[tamanho_sacola][tamanho_sacola]){
    int i, j, y = 0, x = 0;
    char log[10] = "";

    for(i=0;i<tamanho_sacola;i++) for(j=0;j<tamanho_sacola;j++)
        if(sacola[i][j]!=0){
                y = (y>i+1) ? y : i+1;
                x = (x>j+1) ? x : j+1; 
    }

    sprintf(log, "\nArea Ocupada: %i", (x*y));
    gravar_Log(log);

    return x*y;
}

void sacola_Print(int sacola[tamanho_sacola][tamanho_sacola]){
    int i, j;
    for(i=0;i<tamanho_sacola;i++){ 
        for(j=0;j<tamanho_sacola;j++){
            printf("%i ",sacola[i][j]);
        }
        printf("\n");
    }
}


// Organizar na sacola //


// Log funcs //

void gravar_Log(const char *log){
    arquivo_log = fopen("log.txt","a");
    
    if(arquivo_log == NULL){
        printf("Erro ao abrir o arquivo log.txt");
        return;
    }

    fprintf(arquivo_log, log);
    fclose(arquivo_log);
} 


void carrinho_Print_Log(unsigned int carrinho[]){
    arquivo_log = fopen("log.txt","a");

    if(arquivo_log == NULL){
        printf("Erro ao abrir o arquivo log.txt");
        return;
    }

    int i;
    for(i=0;i<tamanho_carrinho;i++) fprintf(arquivo_log, "%i ", carrinho[i]);
    fprintf(arquivo_log, "\n");
    fclose(arquivo_log);
}

void sacola_Print_Log(int sacola[tamanho_sacola][tamanho_sacola]){
    arquivo_log = fopen("log.txt","a");

    if(arquivo_log == NULL){
        printf("Erro ao abrir o arquivo log.txt");
        return;
    }

    fprintf(arquivo_log,"\n");
    
    int i, j;
    for(i=0;i<tamanho_sacola;i++){ 
        for(j=0;j<tamanho_sacola;j++){
            fprintf(arquivo_log,"%i ",sacola[i][j]);
        }
        fprintf(arquivo_log,"\n");
    }
}


// Log funcs //