//
//  Track.cpp
//  Fluxtracker
//
//  Created by Tiago Rezende on 3/23/13.
//
//

#include "Track.h"

using namespace std;
using namespace tr1;

Track::Track(int stepcount): ofxUIWidget() {
    steps = stepcount;
}

void Track::beat(int column, StepPtr step) {
    
}

void Track::drawFill() {
    if (draw_fill) {
        ofFill();
        ofSetColor(color_fill);
        rect->draw();
    }
    int column_count = columns.size();
    float xstart = 0, ystart = 0;
    float x = xstart;
    for (int c = 0; c < column_count; c++) {
        float y = ystart;
        StepPtr nextstep = columns[c].steps;
        for (int s = 0; s < steps; s++) {
            if (nextstep && (nextstep->time == s)) {
                font->drawString(columns[c].mediator->stepToString(nextstep), x, y);
                nextstep = nextstep->next;
            } else {
                font->drawString(columns[c].mediator->emptyStep(), x, y);
            }
        }
    }
}