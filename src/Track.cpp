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

void Track::patternEnter() {
    
}

void Track::patternExit() {
    
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
    if(stepdata != NULL) {
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
                return 1;
            }
            
            prev = step;
            step = step->next;
        }
        prev->next = stepdata;
        return 1;
    } else {
        while(step!=NULL) {
            if (step->time >= stepnum) {
                while (step!=NULL) {
                        step->time += 1;
                        step=step->next;
                }
                return 1;
            }
        }
        return 0;
    }
}

int Track::replace(int column, int stepnum, StepPtr stepdata) {
    StepPtr step = columns[column].steps, prev = NULL;
    if(stepdata != NULL) {
        stepdata->time = stepnum;
        if (step==NULL) {
            columns[column].steps = stepdata;
            return 1;
        }
        while (step != NULL) {
            if (step->time > stepnum) {
                if(prev == NULL) {
                    stepdata->next = columns[column].steps;
                    columns[column].steps = stepdata;
                } else {
                    prev->next = stepdata;
                }
                return 1;
            } else if (step->time == stepnum) {
                if(prev == NULL) {
                    stepdata->next = step->next;
                    columns[column].steps = stepdata;
                } else {
                    stepdata->next = step->next;
                    prev->next = stepdata;
                }
                return 1;
            }
            
            prev = step;
            step = step->next;
        }
        prev->next = stepdata;
        return 1;
    } else {
        while(step!=NULL) {
            if (step->time == stepnum) {
                if (prev==NULL) {
                    columns[column].steps = step->next;
                } else {
                    prev->next = step->next;
                }
                return 1;
            }
        }
        return 0;
    }
}

void Track::setStepCount(int steps) {
    this->steps = steps;
}

void Track::tick(float timedelta) {
    if (time <=0.0) {
        if (time + timedelta > 0.0) {
            patternEnter();
            for (vector<Column>::iterator i = columns.begin(); i != columns.end(); i++) {
                i->curstep = i->steps;
            }
        }
    }
    if (time < steps) {
        if (time + timedelta > steps) {
            patternExit()
        } else {
            time += steps;
            int itime = time;
            int colcount = columns.size();
            for (int i = 0; i < colcount; i++) {
                StepPtr current = columns[i].curstep;
                if (current != NULL) {
                    if (current->time <=itime) {
                        beat(i, current);
                        columns[i].curstep = current->next;
                    }
                }
            }
        }
    }
}