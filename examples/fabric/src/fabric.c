

////////////////////////////////////////////////
////////////     DELICATE FABRIC     ///////////
////////////////////////////////////////////////

// Fork from: https://codepen.io/matthewmain/pen/oyyadr

#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "fabric.h"

double canvasWidth = 0;
double canvasHeight = 0;

// fabric components
int pointCount = 0;
int spanCount = 0;
Point *points = NULL;
Span *spans = NULL;

// settings

// (iterations of position-accuracy refinement)
int rigidity = 15;

// (rate of y-velocity increase per frame)
double gravity = 0.5;

// (proportion of previous velocity after frame refresh)
double friction = 0.999;

// (proportion of previous velocity after bouncing)
double wallBounceLoss = 0.9;

// (proportion of previous velocity if touching the ground)
double skidLoss = 0.8;

// interaction
double grabRadius = 0;

// (number of times a span's length can stretch before breaking)
double fabricStrength = 25;

// converts percentage to canvas x value
double xPct(double pct)
{
        return pct * canvasWidth / 100;
}

// converts percentage to canvas y value
double yPct(double pct)
{
        return pct * canvasHeight / 100;
}

// point constructor
Point createPoint(double current_x, double current_y)
{
        Point pt = malloc(sizeof(PointRec));
        if (pt == NULL) {
                return NULL;
        }
        pt->cx = current_x;
        pt->cy = current_y;
        pt->px = pt->cx;    // previous x value
        pt->py = pt->cy;    // previous y value
        pt->pinned = false;
        pt->grabbed = false;
        pt->mxd = 0;    // mouse x distance (upon grab)
        pt->myd = 0;    // mouse y distance (upon grab)
        pt->id = pointCount;
        pointCount += 1;
        return pt;
}

// creates a point object instance
void addPt(double xp, double yp)
{
        Point pt = createPoint(xPct(xp), yPct(yp));
        Point *newPoints = realloc(points, sizeof(Point) * pointCount);
        if (newPoints == NULL || pt == NULL) {
                return;
        }
        newPoints[pointCount - 1] = pt;
        points = newPoints;
}

void Fabric_resize(double width, double height)
{
        canvasWidth = width;
        canvasHeight = height;
}

///---OBJECTS---///

// gets distance between two points (pythogorian theorum)
double distanceBetween(Point p1, Point p2)
{
        double x_difference = p2->cx - p1->cx;
        double y_difference = p2->cy - p1->cy;
        return sqrt(x_difference * x_difference + y_difference * y_difference);
}

// span constructor
Span createSpan(Point p1, Point p2)
{
        Span span = malloc(sizeof(SpanRec));
        if (span == NULL) {
                return NULL;
        }
        span->p1 = p1;
        span->p2 = p2;
        span->l = distanceBetween(span->p1, span->p2);    // length
        spanCount++;
        return span;
}

///---FUNCTIONS---///

// gets a point by id number
Point getPt(int id)
{
        int i;
        for (i = 0; i < pointCount; i++) {
                if (points[i]->id == id) {
                        return points[i];
                }
        }
        return NULL;
}

// generates random number between a minimum and maximum value
int randNumBetween(int min, int max)
{
        return (int)(rand() % (max - min + 1)) + min;
}

// creates a span object instance
void addSp(int p1, int p2)
{
        Span span = createSpan(getPt(p1), getPt(p2));
        Span *newSpans = realloc(spans, sizeof(Span) * spanCount);
        if (newSpans == NULL || span == NULL) {
                return;
        }
        newSpans[spanCount - 1] = span;
        spans = newSpans;
}

// updates points based on verlet velocity (i.e., current coord minus previous
// coord)
void Fabric_updatePoints(void)
{
        int i;
        for (i = 0; i < pointCount; i++) {
                Point p = points[i];    // point object
                if (!p->pinned) {
                        double xv = (p->cx - p->px) * friction;    // x velocity
                        double yv = (p->cy - p->py) * friction;    // y velocity
                        if (p->py >= canvasHeight - 1 &&
                            p->py <= canvasHeight) {
                                xv *= skidLoss;
                        }
                        p->px = p->cx;    // updates previous x as current x
                        p->py = p->cy;    // updates previous y as current y
                        p->cx += xv;      // updates current x with new velocity
                        p->cy += yv;      // updates current y with new velocity
                        p->cy += gravity;    // add gravity to y
                }
        }
}

// inverts velocity for bounce if a point reaches a wall
void Fabric_wallBounce(void)
{
        int i;
        for (i = 0; i < pointCount; i++) {
                Point p = points[i];
                if (p->cx > canvasWidth) {
                        p->cx = canvasWidth;
                        p->px = p->cx + (p->cx - p->px) * wallBounceLoss;
                }
                if (p->cx < 0) {
                        p->cx = 0;
                        p->px = p->cx + (p->cx - p->px) * wallBounceLoss;
                }
                if (p->cy > canvasHeight) {
                        p->cy = canvasHeight;
                        p->py = p->cy + (p->cy - p->py) * wallBounceLoss;
                }
                if (p->cy < 0) {
                        p->cy = 0;
                        p->py = p->cy + (p->cy - p->py) * wallBounceLoss;
                }
        }
}

// sets spans between points
void Fabric_updateSpans(void)
{
        int i;
        for (i = 0; i < spanCount; i++) {
                Span s = spans[i];
                if (s == NULL) {
                        continue;
                }
                // distance between x values
                double dx = s->p2->cx - s->p1->cx;
                // distance between y values
                double dy = s->p2->cy - s->p1->cy;
                // distance between the points
                double d = sqrt(dx * dx + dy * dy);
                // tear if over-stretched
                if (d > s->l * fabricStrength) {
                        spans[i] = NULL;
                }
                // ratio (span length over distance between points) midpoint
                // between x values
                double r = s->l / d;
                double mx = s->p1->cx + dx / 2;
                // midpoint between y values
                double my = s->p1->cy + dy / 2;
                // offset of each x value (compared to span length)
                double ox = dx / 2 * r;
                // offset of each y value (compared to span length)
                double oy = dy / 2 * r;
                if (!s->p1->pinned) {
                        // updates span's first point x value
                        s->p1->cx = mx - ox;
                        // updates span's first point y value
                        s->p1->cy = my - oy;
                }
                if (!s->p2->pinned) {
                        // updates span's second point x value
                        s->p2->cx = mx + ox;
                        // updates span's second point y value
                        s->p2->cy = my + oy;
                }
                if (spans[i] == NULL) {
                        free(s);
                }
        }
}

void Fabric_update(void)
{
        int i;

        Fabric_updatePoints();
        //(refines point positions for position accuracy & shape rigidity)
        for (i = 0; i < rigidity; i++) {
                Fabric_wallBounce();
                Fabric_updateSpans();
        }
}

// grabs fabric on click
void Fabric_grab(double mouseCanvasX, double mouseCanvasY)
{
        int i;
        for (i = 0; i < pointCount; i++) {
                double x_diff = points[i]->cx - mouseCanvasX;
                double y_diff = points[i]->cy - mouseCanvasY;
                double dist = sqrt(x_diff * x_diff + y_diff * y_diff);
                if (dist <= grabRadius) {
                        points[i]->grabbed = true;
                        points[i]->mxd = x_diff;
                        points[i]->myd = y_diff;
                }
        }
}

// moves fabric
void Fabric_move(double mouseCanvasX, double mouseCanvasY)
{
        int i;
        //(drops fabric if mouse leaves canvas)
        if (mouseCanvasX < 0 || mouseCanvasX > canvasWidth ||
            mouseCanvasY < 0 || mouseCanvasY > canvasHeight) {
                Fabric_drop();
        }
        // updates grabbed points according to mouse position
        for (i = 0; i < pointCount; i++) {
                if (points[i]->grabbed && !points[i]->pinned) {
                        points[i]->cx = points[i]->px =
                            mouseCanvasX + points[i]->mxd;
                        points[i]->cy = points[i]->py =
                            mouseCanvasY + points[i]->myd;
                }
        }
}

// drops fabric
void Fabric_drop(void)
{
        int i;
        for (i = 0; i < pointCount; i++) {
                points[i]->grabbed = false;
        }
}

void Fabric_getSpans(Span **outSpans, int *count)
{
        *outSpans = spans;
        *count = spanCount;
}

void Fabric_getPoints(Point **outPoints, int *count)
{
        *outPoints = points;
        *count = spanCount;
}

/**
 * @brief init fabirc
 *
 * @param fw fabric width (as percentage of canvas width)
 * @param fh fabric height (as percentage of canvas height)
 */
void Fabric_init(int fw, int fh, int canvaswidth, int canvasHeight)
{
        int i, j, x, y;
        int htc = fw;    // fabric horizontal thread count
        int vtc = fh;    // fabric vertical thread count

        srand((unsigned)time(NULL));
        Fabric_resize(canvaswidth, canvasHeight);
        grabRadius = canvasWidth / 25;

        // creates points
        for (i = 0; i < vtc; i++) {
                //(assigns y values so top margin matches l/r margins)
                y = (i * fh / (vtc - 1)) + (100 - fw) / 2;
                for (j = 0; j < htc; j++) {
                        //(assigns x values so fabric is centered horizontally)
                        x = (j * fw / (htc - 1)) + (100 - fw) / 2;
                        addPt(x, y);
                }
        }
        //(pin top row)
        for (i = 0; i < htc; i++) {
                points[i]->pinned = true;
        }

        // create spans
        for (i = 0; i < pointCount; i++) {
                if ((i + 1) % htc != 0) {
                        addSp(i, i + 1);
                }    // horizontal spans
                if (i < pointCount - htc) {
                        addSp(i, i + htc);
                }    // vertical spans
        }

        // initial unfurl
        double rx = randNumBetween(-50, 50);
        for (i = 0; i < pointCount / 4; i++) {
                int rp = randNumBetween(pointCount / 3, pointCount - 1);
                points[rp]->px += rx;
                points[rp]->py += randNumBetween(10, 30);
        }
        for (i = vtc * htc - vtc - 1; i < vtc * htc - 1; i++) {
                points[i]->px += rx;
                points[i]->py += randNumBetween(100, 300);
        }
}
