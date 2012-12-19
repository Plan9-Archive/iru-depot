/* pad animation with OpenGL */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Point Point;
typedef struct Rect Rect;
struct Point {
	GLfloat x;
	GLfloat y;
};
struct Rect {
	Point	min;	/* top-left */
	Point	max;	/* bottom right */
	Point	v;
	GLfloat	drag;
};

Rect r0;
#define factor 0.3;
float diff = -factor;

void
printrect(Rect *r)
{
	printf("min(%.5f %.5f) max(%.5f %.5f) v(%.5f %.5f) drag %.5f\n", r->min.x, r->min.y, r->max.x, r->max.y, r->v.x, r->v.y, r->drag);
}

void
drawrect(Rect *r)
{
	glRectf(r->min.x, r->min.y, r->max.x, r->max.y);
}

Point
addpt(Point *p, Point *q)
{
	return (Point){p->x + q->x, p->y + q->y};
}

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	drawrect(&r0);
	glutSwapBuffers();
}

void
update(void)
{
	int bounce = 0;

	r0.min = addpt(&r0.min, &r0.v);
	r0.max = addpt(&r0.max, &r0.v);

	if (r0.min.x <= -1.0) {
		r0.min.x = -1.0;
		r0.max.x = -0.9;
		bounce = 1;
		r0.v.x = -r0.drag*r0.v.x;
	} else if (r0.max.x >= 1.0) {
		r0.max.x = 1.0;
		r0.min.x = 0.9;
		bounce = 1;
		r0.v.x = -r0.drag*r0.v.x;
	}

	if (r0.min.y <= -1.0) {
		r0.min.y = -1.0;
		r0.max.y = -0.9;
		bounce = 1;
		r0.v.y = -r0.drag*r0.v.y;
	} else if (r0.max.y >= 1.0) {
		r0.max.y = 1.0;
		r0.min.y = 0.9;
		bounce = 1;
		r0.v.y = -r0.drag*r0.v.y;
	}
	
	if (bounce) {
		printrect(&r0);
		if (r0.drag <= 0) {
			r0.drag = 1.1+factor;
			diff = -diff;
		} else if (r0.drag >= 2.0) {
			r0.drag = 2.1-factor;
			diff = -diff;
		}
		r0.drag += diff;
	}

	usleep(10000);
	glutPostRedisplay();
}

int
main(int argc, char *argv[])
{
	r0.min.x = r0.min.y = 0;
	r0.max.x = r0.max.y = 0.10;
	r0.v.x = 0.5;
	r0.v.y = 0.3;
	r0.drag = 1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutMainLoop();
	return 0;
}
