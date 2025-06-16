#pragma once

class OutputLogger {
  public:
    void logPackageStored(int time, int pkgId, int whId, int sectionId);
    void logPackageInTransit(int time, int pkgId, int originId, int destId);
    void logPackageDelivered(int time, int pkgId, int whId);
    void logPackageRemoved(int time, int pkgId, int whId, int sectionId);

  private:
    void printFormattedTime(int time);
};