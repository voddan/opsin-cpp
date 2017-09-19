#include "IDManager.h"


int IDManager::getCurrentID() {
    return currentID;
}

IDManager::IDManager() {
    currentID = 0;
}

int IDManager::getNextID() {
    currentID += 1;
    return currentID;
}
