/* pad animation with OpenGL */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct point point;
typedef struct rect rect;
struct point {
	GLfloat x;
	GLfloat y;
};
struct rect {
	point min; /* top-left */
	point max; /* bottom right */
	point v;
	point drag;
};

rect r, win;

void
drawrect(rect *r)
{
	glRectf(r->min.x, r->min.y, r->max.x, r->max.y);
}

void
addpt(point *p, GLfloat n)
{
	p->x += n;
	p->y += n;
}

void
addrect(rect *r, GLfloat n)
{
	addpt(&r->min, n);
	addpt(&r->max, n);
}

void
init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	drawrect(&r);
	glutSwapBuffers();
}

void
update(void)
{
	float diff = 0.2;

	if (r.max.x + r.v.x > 1.0 || r.min.x + r.v.x < -1.0) {
		if (r.drag.x - diff >= 0)
			r.drag.x -= diff; 
		else
			r.drag.x = 0;
		r.v.x = -r.v.x;
	}
	if (r.max.y + r.v.y > 1.0 || r.min.y + r.v.y < -1.0) {
		if (r.drag.y - diff >= 0.0)
			r.drag.y -= diff;
		else
			r.drag.y = 0;
		r.v.y = -r.v.y;
	}
	

	r.min.x += r.drag.x * r.v.x;
	r.max.x += r.drag.x * r.v.x;
	r.min.y += r.drag.y * r.v.y;
	r.max.y += r.drag.y * r.v.y;

	usleep(10000);
	glutPostRedisplay();
	printf("drag.x = %f drag.y = %f\n", r.drag.x, r.drag.y);
}
int
main(int argc, char *argv[])
{
	r.min.x = r.min.y = 0;
	r.max.x = r.max.y = 0.10;
	r.v.x = 0.05;
	r.v.y = 0.03;
	r.drag.x = r.drag.y = 1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutMainLoop();
	return 0;
}
