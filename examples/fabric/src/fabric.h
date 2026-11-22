#include <stdbool.h>

// The fabric simulation always runs in this fixed coordinate space. The
// viewport scales it to fit the window like CSS `object-fit: contain`.
#define FABRIC_WORLD_WIDTH 800
#define FABRIC_WORLD_HEIGHT 600

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
