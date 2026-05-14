#include "global.h"

global::global() {}


std::function<void(QWidget *)> repolish = [](QWidget *w) {
    w->style()->unpolish(w);
    w->style()->polish(w);
};
