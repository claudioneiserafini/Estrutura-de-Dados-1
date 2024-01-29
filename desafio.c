#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_JOGADORES 5
#define MAX_RESERVAS 3

typedef struct Evento {
  char jogador[50];
  char tipoEvento[50];
  time_t horarioEvento;
  struct Evento *proximo;
} Evento;

typedef struct Substituicao {
  int time;            
  int jogadorEntrando;
  int jogadorSaindo;   
  struct Substituicao *proximo;
} Substituicao;

typedef struct FilaSubstituicoes {
  Substituicao *inicio;
  Substituicao *fim;
} FilaSubstituicoes;

typedef struct Estatistica {
  char jogador[50];
  char tipoEstatistica[50]; 
  struct Estatistica *proximo;
} Estatistica;

typedef struct Time {
  char jogadoresAtivos[MAX_JOGADORES][50];
  char reservas[MAX_RESERVAS][50];
} Time;

Evento *inserirEvento(Evento *listaEventos, char jogador[], char tipoEvento[]) {
  Evento *novoEvento = (Evento *)malloc(sizeof(Evento));
  strcpy(novoEvento->jogador, jogador);
  strcpy(novoEvento->tipoEvento, tipoEvento);
  novoEvento->horarioEvento = time(NULL);
  novoEvento->proximo = NULL;

  if (listaEventos == NULL) {
    return novoEvento;
  } else {
    Evento *temp = listaEventos;
    while (temp->proximo != NULL) {
      temp = temp->proximo;
    }
    temp->proximo = novoEvento;
    return listaEventos;
  }
}

void exibirEventos(Evento *listaEventos) {
  Evento *temp = listaEventos;
  while (temp != NULL) {
    printf("|-------------------------------------------|\n");
    printf("| Jogador: %s, Evento: %s, Horário: %s", temp->jogador,
           temp->tipoEvento, ctime(&(temp->horarioEvento)));
    temp = temp->proximo;
  }
}

void empilharEstatistica(Estatistica **topo, char jogador[], char tipoEstatistica[]) {
  Estatistica *novaEstatistica = (Estatistica *)malloc(sizeof(Estatistica));
  strcpy(novaEstatistica->jogador, jogador);
  strcpy(novaEstatistica->tipoEstatistica, tipoEstatistica);
  novaEstatistica->proximo = *topo;
  *topo = novaEstatistica;
}

void desfazerUltimoEvento(Evento **listaEventos,
                          Estatistica *ultimaEstatistica) {
  if (*listaEventos == NULL || ultimaEstatistica == NULL) {
    printf("| Não há eventos para desfazer.\n");
    return;
  }

  Evento *temp = *listaEventos;
  Evento *anterior = NULL;

  // Verificar se o primeiro evento é o que deve ser desfeito
  if (strcmp(temp->jogador, ultimaEstatistica->jogador) == 0 &&
      strcmp(temp->tipoEvento, ultimaEstatistica->tipoEstatistica) == 0) {
    *listaEventos = temp->proximo;
    printf("| O evento ('%s' do jogador '%s') foi desfeito.\n", temp->tipoEvento, temp->jogador);
    free(temp);
    return;
  }

  // Encontrar o último evento correspondente
  while (temp->proximo != NULL) {
    if (strcmp(temp->proximo->jogador, ultimaEstatistica->jogador) == 0 &&
        strcmp(temp->proximo->tipoEvento, ultimaEstatistica->tipoEstatistica) == 0) {
      anterior = temp;
    }
    temp = temp->proximo;
  }

  // Se o último evento correspondente for encontrado
  if (anterior != NULL) {
    Evento *eventoParaDesfazer = anterior->proximo;
    anterior->proximo = eventoParaDesfazer->proximo;
    printf("| O último evento ('%s' do jogador '%s') foi desfeito.\n",
           eventoParaDesfazer->tipoEvento, eventoParaDesfazer->jogador);
    free(eventoParaDesfazer);
  } else {
    printf("| Não foi possível desfazer o evento. Nenhum evento correspondente "
           "encontrado.\n");
  }
}

void enfileirarSubstituicao(FilaSubstituicoes *fila, int time, int jogadorEntrando, int jogadorSaindo) {
  Substituicao *novaSubstituicao = (Substituicao *)malloc(sizeof(Substituicao));
  novaSubstituicao->time = time;
  novaSubstituicao->jogadorEntrando = jogadorEntrando;
  novaSubstituicao->jogadorSaindo = jogadorSaindo;
  novaSubstituicao->proximo = NULL;

  if (fila->inicio == NULL) {
    fila->inicio = novaSubstituicao;
    fila->fim = novaSubstituicao;
  } else {
    fila->fim->proximo = novaSubstituicao;
    fila->fim = novaSubstituicao;
  }
}

Substituicao *realizarSubstituicao(FilaSubstituicoes *fila, Time times[2], Evento **listaEventos) {
  if (fila->inicio == NULL) {
    return NULL;
  }

  Substituicao *substituicaoRealizada = fila->inicio;
  fila->inicio = fila->inicio->proximo;
  if (fila->inicio == NULL) {
    fila->fim = NULL;
  }

  int timeIndex = substituicaoRealizada->time;
  int entrandoIndex = substituicaoRealizada->jogadorEntrando;
  int saindoIndex = substituicaoRealizada->jogadorSaindo;

  char jogadorSaindo[50];
  strcpy(jogadorSaindo, times[timeIndex].jogadoresAtivos[saindoIndex]);
  strcpy(times[timeIndex].jogadoresAtivos[saindoIndex], times[timeIndex].reservas[entrandoIndex]);
  strcpy(times[timeIndex].reservas[entrandoIndex], jogadorSaindo);

  *listaEventos = inserirEvento(*listaEventos, jogadorSaindo, "substituído");
  *listaEventos = inserirEvento(*listaEventos, times[timeIndex].jogadoresAtivos[saindoIndex], "entrou");

  return substituicaoRealizada;
}

void liberarMemoriaEventos(Evento *listaEventos) {
  Evento *temp;
  while (listaEventos != NULL) {
    temp = listaEventos;
    listaEventos = listaEventos->proximo;
    free(temp);
  }
}

void liberarMemoriaSubstituicoes(FilaSubstituicoes *fila) {
  Substituicao *temp;
  while (fila->inicio != NULL) {
    temp = fila->inicio;
    fila->inicio = fila->inicio->proximo;
    free(temp);
  }
}

void liberarMemoriaEstatisticas(Estatistica *topo) {
  Estatistica *temp;
  while (topo != NULL) {
    temp = topo;
    topo = topo->proximo;
    free(temp);
  }
}

int escolherJogador(Time time, char tipo[]) {
  printf("|-------------------------------------------|\n");
  printf("| Escolha um jogador %s:\n", tipo);
  if (strcmp(tipo, "ativo") == 0) {
    for (int i = 0; i < MAX_JOGADORES; i++) {
      printf("| %d - %s\n", i + 1, time.jogadoresAtivos[i]);
    }
  } else if (strcmp(tipo, "reserva") == 0) {
    for (int i = 0; i < MAX_RESERVAS; i++) {
      printf("| %d - %s\n", i + 1, time.reservas[i]);
    }
  }
  int escolha;
  printf("|-------------------------------------------|\n");
  printf("| Digite o número do jogador: ");
  scanf("%d", &escolha);
  getchar();       
  return escolha - 1; 
}

void escolherEvento(char *tipoEvento) {
  printf("|-------------------------------------------|\n");
  printf("| Escolha o tipo de evento:\n");
  printf("| 1 - Ponto\n");
  printf("| 2 - Falta\n");
  printf("|-------------------------------------------|\n");
  printf("| Digite a opção: ");
  int opcao;
  scanf("%d", &opcao);
  getchar(); 
  if (opcao == 1) {
    strcpy(tipoEvento, "Ponto");
  } else if (opcao == 2) {
    strcpy(tipoEvento, "Falta");
  }
}

int main() {
  Evento *listaEventos = NULL;
  FilaSubstituicoes filaSubstituicoes = {NULL, NULL};
  Estatistica *pilhaEstatisticas = NULL;
  Time times[2]; 

  // Inicialização dos times e reservas
  strcpy(times[0].jogadoresAtivos[0], "Manasses");
  strcpy(times[0].jogadoresAtivos[1], "Fabio");
  strcpy(times[0].jogadoresAtivos[2], "Rossangela");
  strcpy(times[0].jogadoresAtivos[3], "Claudionei");
  strcpy(times[0].jogadoresAtivos[4], "Wagner");
  strcpy(times[0].reservas[0], "reserva1");
  strcpy(times[0].reservas[1], "reserva2");
  strcpy(times[0].reservas[2], "reserva3");

  strcpy(times[1].jogadoresAtivos[0], "Wanderson");
  strcpy(times[1].jogadoresAtivos[1], "Tiago");
  strcpy(times[1].jogadoresAtivos[2], "Angelite");
  strcpy(times[1].jogadoresAtivos[3], "Ryan");
  strcpy(times[1].jogadoresAtivos[4], "Sergio");
  strcpy(times[1].reservas[0], "1reserva");
  strcpy(times[1].reservas[1], "2reserva");
  strcpy(times[1].reservas[2], "3reserva");

  int opcao, timeIndex, jogadorIndex, reservaIndex;
  char tipoEvento[50], jogador[50];

  do {
    printf("|-------------------------------------------|\n");
    printf("|      SISTEMA DE GERENCIAMENTO DA NBA      |\n");
    printf("|-------------------------------------------|\n");
    printf("| 1 - Registrar Evento                      |\n");
    printf("| 2 - Gerenciar Substituições               |\n");
    printf("| 3 - Gerenciar Estatísticas                |\n");
    printf("| 4 - Desfazer Última Estatística           |\n");
    printf("| 5 - Exibir Eventos                        |\n");
    printf("| 0 - Sair                                  |\n");
    printf("|-------------------------------------------|\n");
    printf("| Escolha uma opção: ");
    scanf("%d", &opcao);
    getchar(); 

    switch (opcao) {
    case 1: {
      printf("| Escolha o time (0 ou 1): ");
      scanf("%d", &timeIndex);
      getchar();
      int jogadorIndex = escolherJogador(times[timeIndex], "ativo");
      escolherEvento(tipoEvento);

      // Registrar o evento
      listaEventos = inserirEvento(
          listaEventos, times[timeIndex].jogadoresAtivos[jogadorIndex],
          tipoEvento);

      // Empilhar a estatística correspondente ao evento
      empilharEstatistica(&pilhaEstatisticas,
                          times[timeIndex].jogadoresAtivos[jogadorIndex],
                          tipoEvento);
      break;
    }
    case 2: {
      printf("| Escolha o time (0 ou 1): ");
      scanf("%d", &timeIndex);
      getchar();
      int jogadorSaindoIndex = escolherJogador(times[timeIndex], "ativo");
      int jogadorEntrandoIndex = escolherJogador(times[timeIndex], "reserva");
      enfileirarSubstituicao(&filaSubstituicoes, timeIndex, jogadorEntrandoIndex, jogadorSaindoIndex);
      realizarSubstituicao(&filaSubstituicoes, times, &listaEventos);
      break;
    }
    case 3: {
      printf("| Escolha o time (0 ou 1): ");
      scanf("%d", &timeIndex);
      getchar();
      int jogadorIndex = escolherJogador(times[timeIndex], "ativo");
      escolherEvento(tipoEvento);
      empilharEstatistica(&pilhaEstatisticas,
                          times[timeIndex].jogadoresAtivos[jogadorIndex],
                          tipoEvento);
      break;
    }
    case 4: {
      if (pilhaEstatisticas != NULL) {
        Estatistica *ultimaEstatistica = pilhaEstatisticas;
        pilhaEstatisticas = pilhaEstatisticas->proximo;

        // Desfazer o último evento correspondente à estatística
        desfazerUltimoEvento(&listaEventos, ultimaEstatistica);

        // Liberar a estatística removida da pilha
        free(ultimaEstatistica);
      } else {
        printf("| Não há estatísticas para desfazer.\n");
      }
      break;
    }

    case 5:
      exibirEventos(listaEventos);
      break;
    }
  } while (opcao != 0);
  printf("| Jogo encerrado!\n");

  liberarMemoriaEventos(listaEventos);
  liberarMemoriaSubstituicoes(&filaSubstituicoes);
  liberarMemoriaEstatisticas(pilhaEstatisticas);

  return 0;
}
