#include "logging.h"

void AEMessageBox(const char* message, const char* caption){
    MessageBoxA(0, message, caption, 0);
}