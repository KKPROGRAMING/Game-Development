#include <Windows.h>
#include <time.h>
#include <cmath>
#include <stdio.h>
#include "GL/glut.h"
#include "gl/glaux.h"


#pragma comment(lib,"winmm.lib") 
#pragma comment(lib,"gdi32.lib") 
#pragma comment(lib,"kernel32.lib") 
#pragma comment(lib,"user32.lib") 
#pragma comment(lib,"Advapi32.lib") 
#pragma comment(lib,"OpenGL32.lib") 
#pragma comment(lib,"glu32.lib") 
#pragma comment(lib,"glAux.lib") 
#pragma comment(lib,"legacy_stdio_definitions.lib") 

#define DIR_UP    0
#define DIR_RIGHT 1
#define DIR_DOWN  2
#define DIR_LEFT  3
#define BOARD_WIDTH  11
#define BOARD_HEIGHT 20

GLint   lvl      = 1;                //游戏Level
GLint   scores   = 0;                //游戏得分
GLint   cubeNumsOfSnake  = 0;        //蛇身数目
GLbyte  gameOver = false;            //游戏结束标志
 
GLfloat   corpoPos[2][100] = {{}};   //蛇身坐标
GLfloat   SnakeHead_X       = 0;              //蛇头坐标
GLfloat   SnakeHead_Y       = 0;

GLbyte  direction  = DIR_UP;         //前进方向
 
GLfloat   NewCube_X      = 0;
GLfloat   NewCube_Y      = 0;
 
GLint   _w         = 600;
GLint   _h         = 700;

GLfloat   Left_X     = - BOARD_WIDTH   / 2.0;
GLfloat   Right_X    =   BOARD_WIDTH   / 2.0;
GLfloat   Bottom_Y   = - BOARD_HEIGHT  / 2.0;
GLfloat   Top_Y      =   BOARD_HEIGHT  / 2.0;
 
GLfloat   box_R = 1.0, box_G = 0.0, box_B = 0.0;

DWORD   g_dwLastFPS = 0;
int		g_nFPS = 0, gFrames = 0;
int     gCurrentTime = 0, gStartTime = 0;
GLfloat g_Speed = 0.5f;
GLfloat eye[3], dir[3], up[3];

void drawSnakeCube(GLfloat x, GLfloat y, GLfloat z);
void glutSolidBox(GLfloat x, GLfloat y, GLfloat z);
 
void resetGame()
{
        SnakeHead_X         = 0;
        SnakeHead_Y         = 0;
        direction  = DIR_UP;
        lvl        = 1;
        scores   = 0;
        cubeNumsOfSnake  = 0;
        gameOver = false;
}
 
//绘制组成蛇身体的箱子
void drawSnakeCube(GLfloat x, GLfloat y, GLfloat z)
{
	glPushMatrix();//将当前矩阵推入矩阵堆栈
	glColor3f(169/255.0, 120/255.0, 27/255.0);//设置绘制的颜色
	glTranslatef(x, y, z);//将当前矩阵乘以转换矩阵，生成由 (x、y、z) 指定的转换
	glutSolidBox(0.45f, 0.45f, 0.45f);//这是自定义的函数，非OpenGL原装函数，绘制一个实心盒子
	glPopMatrix();//将矩阵从矩阵堆栈中弹出
}

void drawGenBox(GLfloat x, GLfloat y, GLfloat z)
{
	glPushMatrix();
	glColor3f((rand() % 10 ) * 0.1 , box_G  , box_B );
	glTranslatef(x, y, z);
	glutSolidBox(0.45f, 0.45f, 0.45f);
	glPopMatrix();
}

void drawText(char *string)
{
     while(*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
	 //将单个字符以位图形式绘制于屏幕上，第一个参数为字体，第二个参数为所要绘制的单个字符
}
 
//绘制背景及蛇身
void updateScene()
{
     int  i;
 
	 glColor3f(0.0, 0.7, 0.1);

	 glPushMatrix();
	 glTranslatef(0,0,-0.1);
 
	 glutSolidBox(BOARD_WIDTH  + 1.0f,
		          BOARD_HEIGHT + 1.0f,
				  0.15f);
     glPopMatrix();

	 // 绘制蛇头
     glPushMatrix();
     glTranslatef(SnakeHead_X, SnakeHead_Y, 0.0);
     glColor3f(1, 0, 0);
	 glutSolidBox(0.45f, 0.45f, 0.45f);

     glPopMatrix();
 
     // 绘制蛇身体
     for(i = 0; i < cubeNumsOfSnake; i++)
	 {
          drawSnakeCube(corpoPos[0][i],corpoPos[1][i],0.0);
     }

	 //绘制箱子
	 drawGenBox(NewCube_X, NewCube_Y, 0.0);
}
 
//显示信息
void showMessage()
{
     char tmp_str[40];
 
     //显示level和得分
     glColor3f(1, 1, 1);
     glRasterPos2f( Right_X + 0.8 , Bottom_Y - 0.5f );//指定开始绘制时的光栅位置，2f代表二维，参数为float类型
     sprintf(tmp_str, "Lvl: %d Pts: %d", lvl, scores);
     drawText(tmp_str);
}
 
//random 
int RandomNumber(int high, int low)
{
     return (rand() % (high-low))+low;
}

//adjust the position of the box
GLfloat adjust(GLfloat old)
{
	int times = old / g_Speed;
	GLfloat ret = g_Speed * times;

	return ret;
}

//生成箱子，位置在NewCube_X,NewCube_Y
void generateCube()
{
     time_t seconds;
     time(&seconds);
     srand((unsigned int) seconds);
     NewCube_X = RandomNumber(Right_X*2, Left_X*2);
     NewCube_X = adjust(NewCube_X / 2.0);
     NewCube_Y = RandomNumber(Top_Y*2, Bottom_Y*2);
     NewCube_Y = adjust(NewCube_Y / 2.0);

	 box_R = (rand() % 20) / 20.0;
	 box_G = (rand() % 20) / 20.0;
	 box_B = 1.0;
}
 
//判断蛇头是否碰到身体
bool isCollision()
{
     int i;

     for(i=0; i<cubeNumsOfSnake; i++)
	 {
            if(abs(corpoPos[0][i] - SnakeHead_X) < 0.5f && 
			   abs(corpoPos[1][i] - SnakeHead_Y) < 0.5f ) 
				return true;
     }
     return false;
}

//调整视角1
void resetEye0()
{
	eye[0] = 0;
	eye[1] = 0;
	eye[2] = 20;

	dir[0] = 0;
	dir[1] = 0;
	dir[2] = 0;

	up[0] = 0;
	up[1] = 1;
	up[2] = 0;
}

#define NONE 5
 
//更新Snake position
//通过定时器更新游戏逻辑
void go(int value)
{
     int i;
 
	 if (gameOver) {
		 direction = NONE;
	 }

	 //if(gameOver)
	//	 return;

	 //保留原来蛇头位置
		 double OldSnakeHead_X = SnakeHead_X;
		 double OldSnakeHead_Y = SnakeHead_Y;

		 //更新蛇头位置
		 switch (direction)
		 {
		 case DIR_RIGHT:
			 SnakeHead_X += g_Speed;
			 if (SnakeHead_X > Right_X) SnakeHead_X = Left_X;
			 break;
		 case DIR_LEFT:
			 SnakeHead_X -= g_Speed;
			 if (SnakeHead_X < Left_X) SnakeHead_X = Right_X;
			 break;
		 case DIR_UP:
			 SnakeHead_Y += g_Speed;
			 if (SnakeHead_Y > Top_Y) SnakeHead_Y = Bottom_Y;
			 break;
		 case DIR_DOWN:
			 SnakeHead_Y -= g_Speed;
			 if (SnakeHead_Y < Bottom_Y) SnakeHead_Y = Top_Y;
			 break;
		 case NONE:
			 SnakeHead_X += 0;
			 SnakeHead_Y += 0;
			 break;
		 }

		 //如果发生碰撞则游戏结束
		 if (isCollision()) {
			 gameOver = true;
		 }

		 //蛇头碰到物品
		 if (abs(SnakeHead_X - NewCube_X) < 0.5f &&
			 abs(SnakeHead_Y - NewCube_Y) < 0.5f)
		 {
			 scores++;
			 cubeNumsOfSnake++;
			 if (scores % 5 == 0 && lvl < 15) lvl++;
			 generateCube();
		 }

		 //================================================================================================
		 //设置蛇身新的位置
		 //   （SnakeHead_X,SnakeHead_Y）------------(corpoPos[0][0],corpoPos[1][0])---(corpoPos[0][1],corpoPos[1][1])......
		 //================================================================================================
		 for (i = cubeNumsOfSnake - 1; i > 0; i--)
		 {
			 //--------------------------//
			 //  move each node forward  //
			 //--------------------------//
			 corpoPos[0][i] = corpoPos[0][i - 1];
			 corpoPos[1][i] = corpoPos[1][i - 1];
		 }
		 //更新第一个蛇身体
		 corpoPos[0][0] = OldSnakeHead_X;
		 corpoPos[1][0] = OldSnakeHead_Y;
		
		 /*glutTimerFunc()函数相当于win32 api 中的timer 定时器，也是在delay毫秒后 放出一个定时器消息，
		 而这里的func 则为这个消息的处理函数， patameter为附加参数。
		 这里有一点要注意这个函数是一次性的， 如果要重复使用可以在func中继续调用glutTimerFunc，
		 而且这个功能是可以叠加的，在opengl 内部将他们看成许多个不同的定时器，*/
		 glutTimerFunc(300 - lvl * 8, go, 0);

		 /*在函数里改变和位置有关的变量，然后调用glutPostRedisplay() 进行重绘*/
		 glutPostRedisplay();	 

		 
		 
}

//显示游戏结束
double over_x = -7;
double over_y = 0;
void showGameOver()
{
	char* tmp_str = "Game Over!";

	//显示游戏结束提示信息
	glColor3f(1, 0, 0);

	//draw text,then move to a suitable position
	glRasterPos2f(over_x,over_y);
	if (over_x != -1.5)
		over_x += 0.5;

	//drawText
	while (*tmp_str)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *tmp_str++);
}
 
//绘制函数
void display(void)
{
	//擦除背景
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//将缓冲区清除为预设值
 
	 glMatrixMode(GL_MODELVIEW);
	 /*指定哪个矩阵是当前矩阵，以下为可选参数
	 GL_MODELVIEW ：将后续矩阵操作应用于 modelview 矩阵堆栈。
	 GL_PROJECTION ：将后续矩阵操作应用于投影矩阵堆栈。
	 GL_TEXTURE ：将后续矩阵操作应用于纹理矩阵堆栈。*/

	 glLoadIdentity();//将当前矩阵替换为标识矩阵
	 gluLookAt(eye[0], eye[1], eye[2],
		       dir[0], dir[1], dir[2],
			   up[0],  up[1],  up[2]);
	 /*gluLookAt定义查看转换， 
	 函数创建一个从眼睛点派生的查看矩阵、指示场景中心以及向上向量的引用点。 
	 矩阵将引用点映射到负 z 轴和眼点到原点，以便在使用典型的投影矩阵时，场景中心映射到视区中心。 
	 同样， 投影到查看平面上的向上向量描述的方向映射到正 y 轴，使其指向视区向上。 
	 向上向量不得与从眼睛到引用点的视线平行。
	 gluLookAt 生成的矩阵会发布当前矩阵。*/
	 
	 updateScene();
	 showMessage();
	 if (isCollision()) {
		 showGameOver();
	 }

     glutSwapBuffers();//用于实现双缓冲技术的一个重要函数,该函数的功能是交换两个缓冲区指针
}

//键盘处理函数，处理上下左右箭头
void keyboard(int key, int x, int y)
{
	switch(key){
	case GLUT_KEY_RIGHT     :
		if(direction != DIR_LEFT)
		{
			direction = DIR_RIGHT;
		}
		break;
	case GLUT_KEY_LEFT      :
		if(direction != DIR_RIGHT)
		{
			direction = DIR_LEFT;
		}
		break;
	case GLUT_KEY_UP        :
		if(direction != DIR_DOWN)
		{
			direction = DIR_UP;
		}
		break;
	case GLUT_KEY_DOWN      :
		if(direction != DIR_UP)
		{
			direction = DIR_DOWN;
		}
		break;
	}
}

//键盘处理函数，处理t，s，q
void keyboardchar(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'q'      :
	case 'Q'      :   //游戏退出

		exit(0);
		break;
	}

}
 
void initGL (void)
{   
     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//指定颜色缓冲区的清除值

	 GLfloat lightPosition[]  = {0 , 0, 10, 1};
	 GLfloat lightAmbient[]   = {0.3, 0.3, 0.3, 1.0f}; 
	 GLfloat lightDiffuse[]   = {0.7, 0.7, 0.7, 1.0f}; 
	 GLfloat lightSpecular[]  = {1.0, 1.0, 1.0, 1.0f}; 

	 /*glLightfv返回光源参数值，参数包含 glLightfv(light, pname, params)
	 * light：光线的标识符。可能的灯数取决于实现，但至少支持 8 个灯。 
		它们由表单的符号名称标识GL_LIGHT i ，其中 i 是一个值：0 到 GL_MAX_LIGHTS - 1。
	   pname：浅色的光源参数。 接受多种符号名称。
	   paarams：指定将设置为光源光的参数 pname 的值。
	 */
	 glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	 glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	 glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	 glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	 /*glEnable函数启用OpenGL中的功能。参数为指示 OpenGL 功能的符号常量。*/
	 glEnable(GL_LIGHTING);
	 glEnable(GL_LIGHT0);
	 glEnable(GL_DEPTH_TEST);

	 /*glDepthFunc 函数指定用于深度缓冲区比较的值。
	 参数指定深度比较函数，接受多种符号常量。
		GL_NEVER：永不通过。
		GL_LESS：传入 z 值是否小于存储的 z 值。 这是默认值。
		GL_LEQUAL：传入 z 值小于或等于存储的 z 值时传递。
		GL_EQUAL：传入 z 值是否等于存储的 z 值。
		GL_GREATER：传入 z 值是否大于存储的 z 值。
		GL_NOTEQUAL：传入 z 值是否不等于存储的 z 值时传递。
		GL_GEQUAL：传入 z 值大于或等于存储的 z 值时传递。
		GL_ALWAYS：始终通过。*/
	 glDepthFunc(GL_LEQUAL);

	 /*glShadeModel 函数选择平底纹或平滑底纹。
	 参数mode表示底纹技术的符号值。 接受的值是GL_FLAT和GL_SMOOTH。 默认值为GL_SMOOTH。*/
	 glShadeModel(GL_SMOOTH);	 

	 glEnable(GL_COLOR_MATERIAL);

	 /*glColorMaterial 函数导致材料颜色跟踪当前颜色。glColorMaterial(GLenum face, GLenum mode)
	 face:指定前、后或后方材料参数是否应跟踪当前颜色。 
	 接受的值是GL_FRONT、GL_BACK和GL_FRONT_AND_BACK。 默认值为GL_FRONT_AND_BACK。
	 mode:指定哪些材料参数跟踪当前颜色。 
	 接受的值是GL_EMISSION、GL_AMBIENT、GL_DIFFUSE、GL_SPECULAR和GL_AMBIENT_AND_DIFFUSE。 
	 默认值为GL_AMBIENT_AND_DIFFUSE。*/
	 glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	 resetEye0();
}
 
void resize(GLsizei w, GLsizei h)
{
	/*glViewport 函数设置视区。*/
     glViewport(0, 0, _w, _h);

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();

	 /*gluPerspective 函数设置透视投影矩阵。*/
	 gluPerspective(60, w * 1.0 / h, 0.1, 1000);

	 glMatrixMode(GL_MODELVIEW);
}
 
void drawBox(GLfloat x, GLfloat y, GLfloat z, GLenum type)
{
	static GLfloat n[6][3] =
	{
		{-1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{1.0, 0.0, 0.0},
		{0.0, -1.0, 0.0},
		{0.0, 0.0, 1.0},
		{0.0, 0.0, -1.0}
	};
	static GLint faces[6][4] =
	{
		{0, 1, 2, 3},
		{3, 2, 6, 7},
		{7, 6, 5, 4},
		{4, 5, 1, 0},
		{5, 6, 2, 1},
		{7, 4, 0, 3}
	};
	GLfloat v[8][3];
	GLfloat t[8][2];
	GLint i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -x / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = x / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -y / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = y / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -z / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = z / 2;

	for (i = 5; i >= 0; i--) {
		/*glBegin 和 glend 函数分隔基元或一组类似基元的顶点。*/
		glBegin(type);

		/*glNormal3fv设置当前正向量。参数v指向三个元素数组的指针：新当前法线的 x、y 和 z 坐标。*/
		glNormal3fv(&n[i][0]);

		/*glTexCoord2f设置当前纹理坐标。参数分别指定 s 和 t 纹理坐标。*/
		/*glVertex3fv指定顶点。参数v指向三个元素数组的指针。 元素是顶点的 x、y 和 z 坐标*/
		glTexCoord2f(0.0f, 0.0f);glVertex3fv(&v[faces[i][0]][0]);
		glTexCoord2f(0.0f, x);   glVertex3fv(&v[faces[i][1]][0]);
		glTexCoord2f(x, x);      glVertex3fv(&v[faces[i][2]][0]);
		glTexCoord2f(x, 0.0f);   glVertex3fv(&v[faces[i][3]][0]);

		glEnd();
	}
}

void glutSolidBox(GLfloat x, GLfloat y, GLfloat z)
{
	drawBox(x, y, z, GL_QUADS);
}

int main(void)
{
	/*glutInitDisplayMode()函数的作用主要是在创建窗口的时候，指定其显示模式的类型。
	* 
	颜色模式：GLUT_RGBA表示颜色模式，另外还有GLUT_RGB 和 GLUT_INDEX模式。
	其中GLUT_RGBA为默认的颜色模式。
	
	缓冲区类型：GLUT_DOUBLE表示使用双缓冲窗口，与之对应的是GLUT_SINGLE单缓冲模式。

	其它的缓冲模式还有很多例如：
	GLUT_ACCUM；GLUT_ALPHA；GLUT_DEPTH；GLUT_STENCIL；GLUT_MULTISAMPLE；GLUT_STEREO；GLUT_LUMINANCE。
	这里的GLUT_DEPTH是指窗口使用深度缓存
	*/
     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	 /*指定窗口大小*/
     glutInitWindowSize(_w,_h);

	 /*指定窗口在显示屏幕上的位置*/
     glutInitWindowPosition(50,40);

	 /*创建窗口，参数为窗口名（title）*/
     glutCreateWindow("3D Snake");

	 /*OpenGL的glut中使用glutMouseFunc函数注册鼠标响应事件，
	 使用glutKeyboardFunc函数注册键盘响应事件，
	 对键盘上特殊的4个方向按键的响应函数是glutSpecialFunc。
	 
	 这个函数与glutKeyboardFunc的区别在于前者是用来响应键盘上的特殊按键，
	 如方向键和控制键等。而后者则是用来响应键盘上的字符按键*/
	 glutSpecialFunc(keyboard);
	 glutKeyboardFunc(keyboardchar);

	 /*glutDisplayFunc(&display)在程序运行时是自动调用的，即程序会自动调用display函数重绘窗口，
		调用的时机有这么几个：
		1.  窗口内容绘制
		2.  窗口大小改变
		3.  窗口重绘
	*/
	 glutDisplayFunc(display);

	 /*当然，glutDisplayFunc(&display)函数不会单独出现，比如 ：
		glutReshapeFunc(&reshape)函数先控制窗口大小改变时的操作，
		完成上述操作之后，glut才会调用glutDisplayFunc(&display)绘制窗口。*/
	 glutReshapeFunc(resize);
 
     generateCube();
     go(0);
 
     initGL();//此处开始对OpenGL进行所有设置
     glutMainLoop();//glutMainLoop进入GLUT事件处理循环，让所有的与“事件”有关的函数调用无限循环。
}

