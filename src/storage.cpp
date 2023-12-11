#include "dartClient.h"

void printDirectory(File dir, int numTabs)
{
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            // no more files
            return;
        }
        for (uint8_t i = 0; i < numTabs; i++) { Serial.print('\t'); }
        Serial.print(entry.name());
        if (entry.isDirectory()) {
            Serial.println("/");
            printDirectory(entry, numTabs + 1);
        } else {
            // files have sizes, directories do not
            Serial.print("\t\t");
            Serial.print(entry.size(), DEC);
            time_t cr = entry.getCreationTime();
            time_t lw = entry.getLastWrite();
            struct tm* tmstruct = localtime(&cr);
            Serial.printf("\tCREATION: %d-%02d-%02d %02d:%02d:%02d", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
            tmstruct = localtime(&lw);
            Serial.printf("\tLAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
        }
        entry.close();
    }
}
