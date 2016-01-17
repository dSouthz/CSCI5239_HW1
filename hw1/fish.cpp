
#include "fish.h"
#include "vec.h"
#include <math.h>

#define MAX_VELOCITY 20
#define SWIM_ANGLE_DELTA_MAG 4
#define SWIM_ANGLE_MAX 30
#define FOV_RADIUS 2*radius
#define FOV_ANGLE 110 
#define MAX_PRIORITY_CONTROL 1 

Fish::Fish(Vec pos, Vec vel, Vec dir, double m, double r, double h)
: Particle(pos, vel, m, r), direction(dir) {
    normalize(direction);
    height = h;
    colour = Vec((rand() / (float) RAND_MAX), (rand() / (float) RAND_MAX), (rand() / (float) RAND_MAX));
    swimAngle = (2 * SWIM_ANGLE_MAX) * (rand() / (float) RAND_MAX) - SWIM_ANGLE_MAX;
    swimAngleDelta = SWIM_ANGLE_DELTA_MAG;
}

Fish::~Fish() {
}

const Vec & Fish::getColour() const {
    return colour;
}

void Fish::setColour(const Vec &col) {
    colour = col;
}

void Fish::animate(float dt, unsigned int schoolID, vector< Fish* > &school, Vec globalGoal) {
    // Global Goal
    lineToGoal = globalGoal - position;

    goalSeekDir = globalGoal - position;
    normalize(goalSeekDir);
//    goalSeekDir *= 1/dt;

    // Local Neighbourhood

    int numNeighbours = 0;
    float smallestCloseNeighDist = 2*radius;
    separationDir = Vec();
    avgNeighbourVel = Vec();
    avgNeighbourPos = Vec();
    velMatchDir = Vec();
    centeringDir = Vec();

    for (unsigned int i = 0; i < school.size(); i++) {
        if (schoolID != i) { // Don't influence yourself


            Vec neighbourDir = school[i]->position - position;
            float neighbourDist = magnitud(neighbourDir);

            if ((neighbourDist < FOV_RADIUS) &&
                    (rad2deg(acos(dot(norm(neighbourDir), direction))) < FOV_ANGLE)) { // Is it a neighbour?

                numNeighbours++;

                // Work for Separation
                if (neighbourDist < 2*radius) {
                    separationDir -= norm(neighbourDir) * pow(1 - (neighbourDist / 2*radius), 2);
                    if (neighbourDist < smallestCloseNeighDist) smallestCloseNeighDist = neighbourDist;
                }

                // Work for Velocity Matching
                avgNeighbourVel += school[i]->velocity;

                // Work for Flock Centering
                avgNeighbourPos += school[i]->position;

            }

        }
    }
    

    // Separation Influence
    float priorityControl = 0;
    float centeringPriority = 0;
    float separationPriority = 0;
    float velMatchPriority = 0;


    if ((numNeighbours > 0)) {
        separationPriority = pow(1 - (smallestCloseNeighDist / radius), 2);
        priorityControl += separationPriority;
    }

    // Velocity Matching Influence
    if ((numNeighbours > 0) &&
            (priorityControl < MAX_PRIORITY_CONTROL)) {

        Vec avgVel = avgNeighbourVel / numNeighbours;
        float diffAngle = rad2deg(acos(dot(norm(avgVel), norm(velocity))));
        velMatchDir = (avgNeighbourVel / numNeighbours) - velocity;
        velMatchPriority = pow(diffAngle / 180.0, 2);

        if (velMatchPriority + priorityControl > MAX_PRIORITY_CONTROL) {
            velMatchPriority = MAX_PRIORITY_CONTROL - priorityControl;
            priorityControl = MAX_PRIORITY_CONTROL;
        } else {
            priorityControl += velMatchPriority;
        }
    }

    // Centering Influence
    if ((numNeighbours > 0) &&
            (priorityControl < MAX_PRIORITY_CONTROL)) {
        centeringDir = (avgNeighbourPos / numNeighbours) - position;
        centeringPriority = pow(magnitud(centeringDir) / 20, 2);
        if (centeringPriority + priorityControl > MAX_PRIORITY_CONTROL) {
            centeringPriority = MAX_PRIORITY_CONTROL - priorityControl;
            priorityControl = MAX_PRIORITY_CONTROL;
        } else {
            priorityControl += centeringPriority;
        }
    }

    // Final Target Direction
    if (schoolID == 0) {
        targetDir = (MAX_PRIORITY_CONTROL - priorityControl) * goalSeekDir;
    } else {
        targetDir = separationPriority * separationDir +
                velMatchPriority * velMatchDir +
                centeringPriority * centeringDir +
                (MAX_PRIORITY_CONTROL - priorityControl) * goalSeekDir;
    }

    if (magnitud(targetDir) > 1) targetDir *= (1 / magnitud(targetDir));
    targetDir *= 1/dt;

    // Arrival
    float modMax;
    if (magnitud(lineToGoal) < 2*radius) {
        modMax = pow((magnitud(lineToGoal) / 2*radius) * MAX_VELOCITY, 2);
    } else {
        modMax = MAX_VELOCITY;
    }

    Vec oldVel = velocity;
    incrVelocity(targetDir * dt);
    if (magnitud(velocity) > modMax) velocity *= (modMax / magnitud(velocity)); // Truncate velocity
    incrPosition(oldVel * dt);

    if (velocity[0] != 0 ||
            velocity[1] != 0 ||
            velocity[2] != 0) {
        direction = oldVel * dt;
        normalize(direction);
    }

    // Update Swim Angle
    if (swimAngle >= SWIM_ANGLE_MAX) swimAngleDelta *= -1;
    if (swimAngle <= -SWIM_ANGLE_MAX) swimAngleDelta *= -1;
    swimAngle += swimAngleDelta;

}

void Fish::draw() const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);

    // Rotate to point in direction
    float xzLen = sqrt(direction[0] * direction[0] + direction[2] * direction[2]);
    float yRot, xRot;
    if (xzLen == 0) {

        if (direction[0] > 0)
            yRot = 90;
        else
            yRot = -90;

    } else {

        yRot = rad2deg(acos(direction[2] / xzLen));

    }
    xRot = rad2deg(acos(xzLen));
    if (direction[1] > 0) xRot *= -1;
    if (direction[0] < 0) yRot *= -1;

    glRotatef(yRot, 0, 1, 0);
    glRotatef(xRot, 1, 0, 0);

    // Draw "Fish"
    glColor3f(colour[0], colour[1], colour[2]);
    float velRatio = magnitud(velocity) / MAX_VELOCITY;
    glPushMatrix();
    
    // Head
    glRotatef(velRatio * 0.8 * swimAngle, 0, 1, 0);
    glutSolidCone(radius/2, 2*height/5, 5, 1);

    // Body
    glRotatef(180 - velRatio * swimAngle, 0, 1, 0);
    glutSolidCone(radius/4, height/2, 5, 1);
    glPushMatrix();
    glTranslatef(0, 0, 0.1);
    glRotatef(-65, 1, 0, 0);
    glutSolidCone(radius/6, 2*height/7, 5, 1);
    glPopMatrix();

    // Tail
    glTranslatef(0, 0, height/2);
    glRotatef(180 + velRatio * 1.2 * swimAngle, 0, 1, 0);
    glTranslatef(0, 0, -height/5);
    glScalef(0.5, 1, 1);
    glutSolidCone(radius/2, height/5, 5, 1);
    glPopMatrix();

    glPopMatrix();






}