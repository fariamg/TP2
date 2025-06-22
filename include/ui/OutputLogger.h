#pragma once

class OutputLogger {
  public:
    //@ FUnções para printar logs de eventos
    //@ @param time Tempo do evento
    //@ @param pkgId ID do pacote
    //@ @param whId ID do armazém
    //@ @param sectionId ID da seção do armazém
    static void logPackageStored(int time, int pkgId, int whId, int sectionId);
    static void logPackageInTransit(int time, int pkgId, int originId, int destId);
    static void logPackageDelivered(int time, int pkgId, int whId);
    static void logPackageRemoved(int time, int pkgId, int whId, int sectionId);
    static void logPackageRestored(int time, int pkgId, int whId, int sectionId);

  private:
    static void printFormattedTime(int time);
};