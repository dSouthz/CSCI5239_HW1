/* 
 * File:   fish.h
 * Author: diana
 *
 * Created on January 16, 2016, 10:36 PM
 */

#ifndef FISH_H
#define	FISH_H

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <QGLViewer/vec.h>
using namespace qglviewer;
#include <vector>
using namespace std;

#include "particle.h"

class Fish : public Particle {
private:
    double height;
    Vec colour;
    Vec direction;
    float swimAngle;
    float swimAngleDelta;
    Vec lineToGoal;
    Vec goalSeekDir;
    Vec avgNeighbourVel;
    Vec avgNeighbourPos;
    Vec separationDir;
    Vec velMatchDir;
    Vec centeringDir;
    Vec targetDir;

public:
    Fish(Vec pos, Vec vel, Vec dir, double m, double r, double h);
    virtual ~Fish();
    void draw() const;
    void animate(float dt, unsigned int schoolID, vector< Fish * > &school, Vec globalGoal);
    const Vec & getColour() const;
    void setColour(const Vec &col);
};

#endif	/* FISH_H */