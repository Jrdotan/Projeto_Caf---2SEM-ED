#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct participante {
    int id;
    char nome[120];
    char curso[120];
    int ano;
    struct participante *next;
} Participante;

typedef struct pagante{
  int id;
  int mes;
  int ano;
  float valor;
  struct pagante *next;
} Pagante;

// inicializando frente e trÃ¡s das filas
Participante *front = NULL;
Participante *rear = NULL;
Pagante *frontP = NULL;
Pagante *rearP = NULL;
int next_id = 1;

//protótipós
void inserir_participante();
void editar_participante();
void ler_participantes();
void ler_contribuintes();
void salvar_participantes();
void limpar_buffer();
void inserir_contribuinte();
void salvar_contribuintes_por_curso();
void liberar_memoria();
void carregar_participantes();

int main() {
    char escolha;

    do {
        printf("\nPressione 'a' para inserir novo participante, 'e' para editar participante");
        printf(" 'l' para ler todos os participantes, 'i' para inserir contribuintes, 'g' para carregar contribuintes, 'c' para ler contribuintes ");
         printf(" ou 'q' para sair: ");
        scanf("%c", &escolha);

        switch (escolha) {
            case 'a':
                inserir_participante();
                break;
            case 'e':
                editar_participante();
                break;
            case 'l':
                ler_participantes();
                break;
            case 'c':
                ler_contribuintes();
                break;
            case 'i':
                inserir_contribuinte();
                break;
            case 'g':
            	liberar_memoria();
            	carregar_participantes();
            	break;
            case 'q':
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (escolha != 'q');

    liberar_memoria(); //chamada de função que libera memória restante
    return 0;
}

void salvar_participantes() // salva dados dos participantes em um .txt
{
    FILE *participantes_file = fopen("participantes.txt", "wt");
    if (participantes_file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    Participante *curr = front;
    while (curr != NULL) {
        fprintf(participantes_file, "%d,%s,%s,%d\n", curr->id, curr->nome, curr->curso, curr->ano);
        curr = curr->next;
    }

    fclose(participantes_file);
    printf("Dados salvos com sucesso\n");
}

void salvar_contribuintes() //salva dados dos contribuintes em .txt
{
    FILE *participantes_file = fopen("contribuintes.txt", "wt");
    if (participantes_file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    Pagante *curr = frontP;
    while (curr != NULL) {
        fprintf(participantes_file, "%d,%0.2f,%d,%d\n", curr->id, curr->valor, curr->mes, curr->ano);
        curr = curr->next;
    }

    fclose(participantes_file);
    printf("Dados salvos com sucesso\n");
}

void salvar_contribuintes_por_curso() //salva dados de contribuintes em uma .txt separadas por curso
{
    Pagante *curr = frontP;
    
   //abre .txt de todos os cursos
    FILE *fileDSM = fopen("contribuintes_DSM.txt", "wt");
    FILE *fileSI = fopen("contribuintes_SI.txt", "wt");
    FILE *fileGE = fopen("contribuintes_GE.txt", "wt");
   
  //verifica se arquivos foram abertos com sucesso
    if (fileDSM == NULL || fileSI == NULL || fileGE == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    while (curr != NULL) {
     
        Participante *participante = front;
        while (participante != NULL) {
            if (participante->id == curr->id) {
                // Escrever os dados de acordo com o curso em que o contribuinte cursa
                switch (participante->curso[0]) {
                    case 'D':
                        fprintf(fileDSM, "%d,%0.2f,%d,%d\n", curr->id, curr->valor, curr->mes, curr->ano);
                        break;
                    case 'S':
                        fprintf(fileSI, "%d,%0.2f,%d,%d\n", curr->id, curr->valor, curr->mes, curr->ano);
                        break;
                    case 'G':
                        fprintf(fileGE, "%d,%0.2f,%d,%d\n", curr->id, curr->valor, curr->mes, curr->ano);
                        break;
                }
                break; 
            }
            participante = participante->next;
        }
        curr = curr->next;
    }

    fclose(fileDSM);
    fclose(fileSI);
    fclose(fileGE);

    printf("Dados dos contribuintes por curso salvos com sucesso.\n");
}


void inserir_participante() {
    char nome[120];
    char curso[120];
    int ano;
    

    Participante *this = malloc(sizeof(Participante));
    if (this == NULL) {
        printf("Erro ao alocar memória para o participante.\n");
        exit(1);
    }
    this->next = NULL;

  

    printf("Digite o primeiro nome do participante: ");
    scanf("%s", this->nome);

  //seleciona curso a partir de switch case
    printf("Escolha o curso, digite 'a' para DSM, 'b' para SI ou 'c' para GE: ");
    char escolha;
    scanf(" %c", &escolha);
    limpar_buffer(); 

    switch (escolha) {
        case 'a':
            strcpy(this->curso, "DSM");
            break;
        case 'b':
            strcpy(this->curso, "SI");
            break;
        case 'c':
            strcpy(this->curso, "GE");
            break;
        default:
            printf("Opção de curso inválida.\n");
            free(this);
            return;
    }

    printf("Qual o ano em que foi cursado? ");
    scanf("%d", &ano);
    while(ano > 2024){
    printf("\nAno invalido, digite novamente o ano em que foi cursado: ");
    scanf("%d", &ano);
  }
    limpar_buffer();

    this->ano = ano;

    if (rear == NULL) {
        front = this;
        rear = this;
        this->id = 1;
    } else {
        this->id = rear->id + 1; //sistema de auto increment de ID
        rear->next = this;
        rear = this;
        
    }

    salvar_participantes();
    printf("Participante inserido com sucesso.\n");
}

void inserir_contribuinte() {
    int id;
    int ano;
    int mes;
    float valor;

    printf("Digite o ID do participante a ser editado: ");
    scanf("%d", &id);
    limpar_buffer();

    Participante *curr = front;
    Pagante *this = malloc(sizeof(Pagante));
    if (!this) {
        printf("Erro de alocação de memória");
        exit(1);
    }
    while (curr != NULL) {
        if (curr->id == id) {
            this->id = id;
            printf("Qual o mês da contribuição? ");
            scanf("%d", &mes);
           while(mes < 0 || mes > 12){
            printf("\nMês invalido, digite novamente o mês da contribuição: ");
            scanf("%d", &mes);
            }

            limpar_buffer();
            this->mes = mes;

            printf("Qual o ano em da contribuição? ");
            scanf("%d", &ano);
              while(ano > 2024){
                printf("\nAno invalido, digite novamente o ano da contribuição: ");
                scanf("%d", &ano);
              }
            limpar_buffer();
            this->ano = ano;

            printf("Qual o valor em reais da contribuição? ");
            scanf("%f", &valor);
            limpar_buffer();
            this->valor = valor;

            this->next = NULL; // Gambiarra feita pra verificar se a próxima estrutura esta vazia

            if (rearP == NULL) {
                frontP = this;
                rearP = this;
            } else {
                rearP->next = this;
                rearP = this;
            }
            salvar_contribuintes();
            salvar_contribuintes_por_curso();
            printf("Contribuição registrada com sucesso!");
            return;
        }
        curr = curr->next;
    }
    printf("ID não encontrado!");
}


void editar_participante() {
    int id;
    printf("Digite o ID do participante a ser editado: ");
    scanf("%d", &id);
    limpar_buffer();

    Participante *curr = front;
    while (curr != NULL) {
        if (curr->id == id) {
            printf("Digite o primeiro nome do participante: ");
            scanf("%s", curr->nome);
            printf("Escolha o novo curso, digite 'a' para DSM, 'b' para SI ou 'c' para GE: ");
            char escolha;
            scanf(" %c", &escolha);
            limpar_buffer(); 

            switch (escolha) {
                case 'a':
                    strcpy(curr->curso, "DSM");
                    break;
                case 'b':
                    strcpy(curr->curso, "SI");
                    break;
                case 'c':
                    strcpy(curr->curso, "GE");
                    break;
                default:
                    printf("Opção de curso invalida.\n");
                    return;
            }

            printf("Digite o ano em que foi cursado: ");
            scanf("%d", &curr->ano);
              while(curr->ano > 2024){
              printf("\nAno invalido, digite novamente o ano em que foi cursado: ");
              scanf("%d", &curr->ano);
            }
            limpar_buffer();

            salvar_participantes();
            printf("Participante editado com sucesso.\n");
            return;
        }
        curr = curr->next;
    }

    printf("Participante com o ID %d nÃ£o encontrado.\n", id);
}

void ler_participantes() {
    FILE *handler = fopen("participantes.txt", "rt");
    if (handler == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("\nLista de participantes:\n");
    int id, ano;
    char nome[120], curso[120];
    while (fscanf(handler, "%d,%[^,],%[^,],%d\n", &id, nome, curso, &ano) == 4) // Gambiarra nº 2 que precisei fazer para verificar se os 4 inputs pedidos estão sendo passados por participantes
  {
        printf("ID: %d, Nome: %s, Curso: %s, Ano: %d\n", id, nome, curso, ano);
    }

    fclose(handler);
}

void ler_contribuintes() {
    FILE *handler = fopen("contribuintes.txt", "rt");
    if (handler == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("\nLista de participantes:\n");
    int id, mes, ano;
    float valor;
    
    while (fscanf(handler, "%d,%f,%d,%d\n", &id, &valor, &mes, &ano) == 4) // Gambiarra número 2.5 que precisei fazer para verificar se são os 4 inputs pedidos estão sendo passados por participantes
  {
        printf("Membro de ID: %d,Contribuiu com valor de: R$%0.2f no Mês: %d, durante o ano de %d\n", id, valor, mes, ano);
    }

    fclose(handler);
}


void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void liberar_memoria() {
    // Liberar memória em participante 
    Participante *curr_participante = front;
    while (curr_participante != NULL) {
        Participante *temp_participante = curr_participante;
        curr_participante = curr_participante->next;
        free(temp_participante);
    }
    
    // Liberar memória em pagante
    Pagante *curr_contribuinte = frontP;
    while (curr_contribuinte != NULL) {
        Pagante *temp_contribuinte = curr_contribuinte;
        curr_contribuinte = curr_contribuinte->next;
        free(temp_contribuinte);
    }

    // aqui eu nulifico e limpo todas as structs de front e rear do programa 
    front = NULL;
    rear = NULL;
    frontP = NULL;
    rearP = NULL;
}

void carregar_participantes() {
    FILE *handler = fopen("participantes.txt", "rt");
    if (handler == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }


    int id, ano;
    char nome[120], curso[120];

    while (fscanf(handler, "%d,%[^,],%[^,],%d\n", &id, nome, curso, &ano) == 4) {
        Participante *novo_participante = malloc(sizeof(Participante));
        if (novo_participante == NULL) {
            printf("Erro ao alocar memória para participante.\n");
            fclose(handler);
            liberar_memoria();
            exit(1);
        }

        
        novo_participante->id = id;
        strcpy(novo_participante->nome, nome);
        strcpy(novo_participante->curso, curso);
        novo_participante->ano = ano;
        novo_participante->next = NULL;

  
        if (front == NULL) {
            front = novo_participante;
            rear = novo_participante;
        } else {
            rear->next = novo_participante;
            rear = novo_participante;
        }
    }

    fclose(handler);
    printf("Participantes carregados com sucesso.\n");
}

