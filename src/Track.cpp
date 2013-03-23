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

void Track::addColumn(std::string name,
                      float r, float g, float b,
                      int width,
                      tr1::shared_ptr<StepMediator> mediator) {
    Track::Column newcolumn = {
        name,
        r, g, b,
        width,
        mediator
    };
    columns.push_back(newcolumn);
    rect->width += font->stringWidth("0")*width;
    
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
            y += charheight;
        }
        x += charwidth*(columns[c].width+1)
    }
}

int Track::insert(int column, int stepnum, StepPtr stepdata) {
    StepPtr step = columns[column].steps, prev = NULL;
    stepdata->time = stepnum;
    if (step==NULL) {
        columns[column].steps = stepdata;
        return 1;
    }
    while (step != NULL) {
        if (step->time >= stepnum) {
            if(prev == NULL) {
                stepdata->next = columns[column].steps;
                columns[column].steps = stepdata;
            } else {
                prev->next = stepdata;
            }
            while (step!=NULL) {
                step->time += 1;
                step = step->next;
            }
        }
        
        prev = step;
        step = step->next;
    }
}

int Track::replace(int column, int stepnum, StepPtr stepdata) {
    
}