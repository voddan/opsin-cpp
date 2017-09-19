#include "AnnotatorState.h"


AnnotatorState::AnnotatorState(int state, wchar_t annot, int posInName, bool isCaseSensitive,
                               AnnotatorState *previousAs) : state(state), annot(annot), posInName(posInName),
                                                             isCaseSensitive(), previousAs(previousAs) {
}

int AnnotatorState::getState() {
    return state;
}

wchar_t AnnotatorState::getAnnot() {
    return annot;
}

int AnnotatorState::getPosInName() {
    return posInName;
}

bool AnnotatorState::isCaseSensitive() {
    return isCaseSensitive_Renamed;
}

AnnotatorState *AnnotatorState::getPreviousAs() {
    return previousAs;
}
