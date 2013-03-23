//
//  Track.h
//  Fluxtracker
//
//  Created by Tiago Rezende on 3/23/13.
//
//

#ifndef __Fluxtracker__Track__
#define __Fluxtracker__Track__

#include <vector>
#include <tr1/memory>
#include <string>
#include "ofMain.h"
#include "ofxUI.h"

class Step {
    friend class Track;
    tr1::shared_ptr<Step> next;
    int time;
public:
    std::vector<int> idata;
    std::vector<float> fdata;
    std::string str;
};

typedef tr1::shared_ptr<Step> StepPtr;

class StepMediator {
public:
    virtual ~StepMediator() {}
    virtual std::string stepToString(StepPtr step);
    virtual bool checkString(std::string str);
    virtual StepPtr stringToStep(std::string str);
    virtual std::string emptyStep();
    virtual bool stepValueUp(StepPtr step, int cursorpos);
    virtual bool stepValueDown(StepPtr step, int cursorpos);
    virtual bool stepValueLeft(StepPtr step, int cursorpos);
    virtual bool stepValueRight(StepPtr step, int cursorpos);
};

/**
 * The main extension class.
 * override the constructor to create the columns, 
 * and void beat(column, step) to implement custom tracks.
 */
class Track: public ofxUIWidget {
    float charwidth, charheight;
    int cursor_column, cursor_subcolumn, cursor_step;

    int steps;
    float time;
    struct Column {
        std::string name;
        float r, g, b;
        StepPtr steps;
        int width;
        tr1::shared_ptr<StepMediator> mediator;
    };
    std::vector<Column> columns;
public:
    Track(int stepcount);
    virtual ~Track() {}
    // where the implementation goes
    virtual void beat(int column, StepPtr step);
    void addColumn(std::string name,
                   float r, float g, float b,
                   int width,
                   tr1::shared_ptr<StepMediator> mediator);
    int insert(int column, int stepnum, StepPtr stepdata);
    int replace(int column, int stepnum, StepPtr stepdata);
    void setStepCount(int steps);
    void tick(float timedelta);
    void reset();
    
    virtual void drawFill();
};

class TrackPattern {
public:
    std::string name;
    std::string comment;
    typedef tr1::shared_ptr<Track> TrackPtr;
    std::vector<TrackPtr> tracks;
    void tick(float timedelta);
    void addTrack(int column, TrackPtr track);
};

class Module {
    
};

class Tracker {
public:
    typedef tr1::shared_ptr<TrackPattern> PatternPtr;
    typedef std::vector<PatternPtr> PatternRow;
    std::vector<PatternPtr> patterns;
    std::vector<PatternRow> rows;
    
    void tick(float timedelta);
};

#endif /* defined(__Fluxtracker__Track__) */
