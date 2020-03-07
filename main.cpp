#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <windows.h>
#include <glut.h>
#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;


float posx=250;
float posy=250;
float posz=0;
float lx=-1/sqrt(2);
float ly=-1/sqrt(2);
float lz=0;
float rx=-1/sqrt(2);
float ry=-1/sqrt(2);
float rz=0;
float ux=0;
float uy=0;
float uz=1;

struct point
{
    double x,y,z;
};
void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);

            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);

            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }
        glEnd();
    }
}

void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);	//grey
        glBegin(GL_LINES);
        {
            for(i=-8; i<=8; i++)
            {

                if(i==0)
                    continue;	//SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
                //lines parallal to z axis

                //glVertex3f( 0, 90, i*10);
                //glVertex3f(0, -90, i*10);


            }
        }
        glEnd();
    }
}
void drawCube(double a)
{
    glBegin(GL_QUADS);
    {

        glVertex3f( a, a, a);
        glVertex3f( a,-a, a);
        glVertex3f(-a,-a, a);
        glVertex3f(-a, a, a);

        glVertex3f( a, a,-a);
        glVertex3f( a,-a,-a);
        glVertex3f(-a,-a,-a);
        glVertex3f(-a, a,-a);


        glVertex3f( a, a, a);
        glVertex3f( a, a,-a);
        glVertex3f( a,-a,-a);
        glVertex3f( a,-a, a);

        glVertex3f(-a, a, a);
        glVertex3f(-a, a,-a);
        glVertex3f(-a,-a,-a);
        glVertex3f(-a,-a, a);


        glVertex3f( a, a, a);
        glVertex3f( a, a,-a);
        glVertex3f(-a, a,-a);
        glVertex3f(-a, a, a);

        glVertex3f( a,-a, a);
        glVertex3f( a,-a,-a);
        glVertex3f(-a,-a,-a);
        glVertex3f(-a,-a, a);
    }
    glEnd();
}
//draws a cylinder of given radius and height
void drawCylinder(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw quads using generated points
    for(i=0; i<segments; i++)
    {

        glBegin(GL_QUADS);
        {
            glVertex3f(points[i].x,points[i].y,height/2);
            glVertex3f(points[i+1].x,points[i+1].y,height/2);
            glVertex3f(points[i].x,points[i].y,-height/2);
            glVertex3f(points[i+1].x,points[i+1].y,-height/2);
        }
        glEnd();
    }
}
void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0; i<segments; i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}
void drawSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            }
            glEnd();
        }
    }
}
double m= 0;
double n= 0;


void camel1()
{
                                    //Ground
   glPushMatrix();
    {

        glColor3f(1,0,1);
        glScalef(5,.01,5);
        glTranslatef(0,-25,0);
        drawCube(25);
   }
   glPopMatrix();
                                    // belly
   glPushMatrix();
    {

        glColor3f(0,0,1);
        glScalef(1.5,.6,1);
        glTranslatef(0,100,0);
        drawCube(25);
   }
   glPopMatrix();

                            //head
        glPushMatrix();
    {

            glColor3f(1,1,0);
            glTranslatef(-100,100,0);
            drawSphere(15,75,75);
       }
       glPopMatrix();

                            //mouth
    glPushMatrix();
    {

            glColor3f(1,1,0);
            glTranslatef(-110,85,0);
            drawCube(7);
       }
       glPopMatrix();
                                //goose bump

        glPushMatrix();
    {
            glColor3f(.5,.5,.5);
            glTranslatef(-10,75,0);
            drawSphere(15,10,75);
       }
       glPopMatrix();

}

void camel2()
{                           //leg
    glPushMatrix();
    {
        glTranslatef(0,25,0);
       glPushMatrix();
        {
            glScalef(1,5,1);
            glColor3f(0,1,0);
            glTranslatef(-25,0,15);
            drawCube(5);
       }
       glPopMatrix();
                                   //leg
       glPushMatrix();
        {
            glScalef(1,5,1);
            glColor3f(0,1,0);
            glTranslatef(25,0,15);
            drawCube(5);
       }
       glPopMatrix();
                                   //leg
       glPushMatrix();
        {
            glScalef(1,5,1);
            glColor3f(0,1,0);
            glTranslatef(-25,0,-15);
            drawCube(5);
       }
       glPopMatrix();
                                   //leg
       glPushMatrix();
        {
            glScalef(1,5,1);
            glColor3f(0,1,0);
            glTranslatef(25,0,-15);
            drawCube(5);
       }
       glPopMatrix();

    }
   glPopMatrix();


}
void camel3()
{
                                //tail
      glPushMatrix();
        {
            glTranslatef(40,70,0);
            glScalef(1,.2,.5);
            glRotated(50,.5,1,0);
            glColor3f(0,1,1);
            drawCone(10,50,25);
    }
       glPopMatrix();

                                //neck
        glPushMatrix();
        {

            glTranslatef(-50,90,0);
            glColor3f(1,0,0);
            glRotated(90,.5,1,0);
            glScalef(2.5,2,10);
            drawCylinder(3,10,50);
       }
       glPopMatrix();

}
void camel4()
{

    glPushMatrix();
    {
        glRotatef(90,0,1,0);
        glTranslatef(0,70,25);
        glScalef(3,3,3);
                glPushMatrix();
                {
                    //head
                    glScalef(1.5,1.5,1);
                    glTranslatef(0,12,0);
                  drawSphere(2,50,50);
                }
               glPopMatrix();

                glPushMatrix();
                {//belly
                    glColor3f(.5,.7,.5);
                    glScalef(1.5,1.5,1.5);
                    glTranslatef(0,5,0);
                  drawSphere(5,50,50);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    //neck
                    glColor3f(1,1,0);
                    glRotatef(90,1,0,0);
                    glTranslatef(0,0,-15);
                    drawCylinder(1,5,75);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    //leg
                    glScalef(.8,1.5,.8);
                    glColor3f(1,0,0);
                    glRotatef(90,1,0,0);
                    glTranslatef(-10,0,0);
                    drawCylinder(2,5,75);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    //leg
                    glScalef(.8,1.5,.8);
                    glColor3f(1,0,0);
                    glRotatef(90,1,0,0);
                    glTranslatef(10,0,0);
                    drawCylinder(2,5,75);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    //neck
                    glColor3f(1,0,0);
                    glScalef(4,.5,.5);
                    glTranslatef(0,20,0);
                    drawCube(2.5);
                }
               glPopMatrix();

    }
    glPopMatrix();
}
void camel5()
{


        glPushMatrix();
                {
                    glTranslated(-75,0,75);

        glPushMatrix();
                {
                    //tree
                    glColor3f(0,1,0);
                    glScalef(1,5,1);
                    glRotatef(90,1,0,0);

                    drawCylinder(4,25,75);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    glScalef(3,3,3);
                    glColor3f(0.5,.2,.3);
                    glTranslatef(0,26,0);
                    drawSphere(3,50,50);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    glScalef(3,3,3);
                    glColor3f(0.2,.2,.3);
                    glTranslatef(4,26,0);
                    drawSphere(2,50,50);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    glScalef(3,3,3);
                    glColor3f(0.4,.2,.3);
                    glTranslatef(-4,26,0);
                    drawSphere(2,50,50);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    glScalef(3,3,3);
                    glColor3f(0.4,.4,.1);
                    glTranslatef(0,26,4);
                    drawSphere(2,50,50);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    glScalef(3,3,3);
                    glColor3f(0.1,.2,.3);
                    glTranslatef(0,26,-4);
                    drawSphere(2,50,50);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    glScalef(3,3,3);
                    glColor3f(0.5,.2,.5);
                    glTranslatef(0,20,0);
                    drawSphere(3,50,50);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    glScalef(3,3,3);
                    glColor3f(1,0,0);
                    glTranslatef(8,20,0);
                    drawSphere(3,50,50);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    glScalef(3,3,3);
                    glColor3f(1,1,0);
                    glTranslatef(-8,20,0);
                    drawSphere(3,50,50);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    glScalef(3,3,3);
                    glColor3f(1,0,1);
                    glTranslatef(0,20,8);
                    drawSphere(3,50,50);
                }
               glPopMatrix();

               glPushMatrix();
                {
                    glScalef(3,3,3);
                    glColor3f(0,1,1);
                    glTranslatef(0,20,-8);
                    drawSphere(3,50,50);
                }
               glPopMatrix();


                }
               glPopMatrix();

}
void keyboardListener(unsigned char key, int x,int y)
{

    switch(key)
    {

    case '1':
        drawgrid=1-drawgrid;
        break;

    case 'w':
        posx+=lx;
        posy+=ly;
        posz+=lz;
        break;

    case 's':
        posx-=lx;
        posy-=ly;
        posz-=lz;
        break;

    case 'n':
        posx+=rx;
        posy+=ry;
        posz+=rz;
        break;

    case 'u':
        posx-=rx;
        posy-=ry;
        posz-=rz;
        break;

    case 'a':
        posx+=ux;
        posy+=uy;
        posz+=uz;
        break;

    case 'd':
        posx-=ux;
        posy-=uy;
        posz-=uz;
        break;

    default:
        break;
    }
}

void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:		//down arrow key
        cameraHeight -= 3.0;
        break;
    case GLUT_KEY_UP:		// up arrow key
        cameraHeight += 3.0;
        break;

    case GLUT_KEY_RIGHT:
        cameraAngle += 0.03;
        break;
    case GLUT_KEY_LEFT:
        cameraAngle -= 0.03;
        break;

    case GLUT_KEY_PAGE_UP:
        break;
    case GLUT_KEY_PAGE_DOWN:
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;
    case GLUT_KEY_END:
        break;

    default:
        break;
    }
}
void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            drawaxes=1-drawaxes;
        }
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}
void display()
{
    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?
    //gluLookAt(200,200,300,	1,0,0,	0,1,0);
    //gluLookAt(200,100,200,	1,0,0,	0,1,0);
    gluLookAt(posx,posy,posz, posx+lx, posy+ly, posz+lz,	0,1,0);

    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();
    drawGrid();
   // rocket();
   // ground();
   // switchbox();
   // man();

    camel1();
    camel2();
  camel3();
    camel4();
    camel5();
    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}
void animate()
{

    angle+=0.04;
    glutPostRedisplay();
}
void init()
{
    //codes for initialization
    //drawgrid=1;
    drawaxes=1;
    cameraHeight=100.0;
    cameraAngle=1.0;
    angle=0;

    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}
