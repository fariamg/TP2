#pragma once

//@ Classe responsável por orquestrar tempo das movimentações
class Timer {
  private:
    //@ Atributos que guardam inicio e fim
    int time;

  public:
    //@ Contrutor padrão
    Timer();

    //@ Função que retorna o tempo atual acumulado
    int getTime() const noexcept;

    //@ Função que define o tempo atual do timer
    void setTime(int newTime);

    //@ Função que adiciona tempo ao timer
    //@ @param time Tempo a ser adicionado
    void addTime(int time);
};