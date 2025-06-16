#pragma once

class OutputLogger {
  public:
    //@ FUnções para printar logs de eventos
    //@ @param time Tempo do evento
    //@ @param pkgId ID do pacote
    //@ @param whId ID do armazém
    //@ @param sectionId ID da seção do armazém
    void logPackageStored(int time, int pkgId, int whId, int sectionId);
    void logPackageInTransit(int time, int pkgId, int originId, int destId);
    void logPackageDelivered(int time, int pkgId, int whId);
    void logPackageRemoved(int time, int pkgId, int whId, int sectionId);

  private:
    void printFormattedTime(int time);
};