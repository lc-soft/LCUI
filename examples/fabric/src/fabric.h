#include <stdbool.h>

typedef struct PointRec_ {
        double cx;
        double cy;
        double px;
        double py;
        bool pinned;
        bool grabbed;
        double mxd;
        double myd;
        int id;
} PointRec, *Point;

typedef struct SpanRec_ {
        Point p1;
        Point p2;
        double l;
} SpanRec, *Span;

void Fabric_getSpans(Span **outSpans, int *count);

void Fabric_getPoints(Point **outPoints, int *count);

void Fabric_resize(double width, double height);

void Fabric_update(void);

// grabs fabric on click
void Fabric_grab(double mouseCanvasX, double mouseCanvasY);

// moves fabric
void Fabric_move(double mouseCanvasX, double mouseCanvasY);

// drops fabric
void Fabric_drop(void);

/**
 * @brief init fabirc
 *
 * @param fw fabric width (as percentage of canvas width)
 * @param fh fabric height (as percentage of canvas height)
 */
void Fabric_init(int fw, int fh, int canvaswidth, int canvasHeight);
