## Readme_第十三次作业说明（OpenGL贪吃蛇）

​	我在原代码的基础上添加了一项功能，即：当蛇身发生碰撞时，游戏结束，对这一状态生成“Game Over!“的提示信息。

​	对此，我创建了一个提示游戏结束的函数 showGameOver() ，并对原来代码中规定小蛇移动逻辑的函数 go() 进行了一定的改动。

```c++
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
```

​	为了让提示信息在屏幕上动态呈现，我使用 over_x 和 over_y 两个全局变量保存这一提示信息的光栅位置，并让 over_x 变量在达到最终的合适位置前不断改变，在画面的重绘之下产生”移动“的效果。由于提示信息只在水平位置上移动，所以 over_y 不需要做改变。

​	而在 go() 函数中，最初的设计是在发生碰撞之后函数立即返回，不再根据移动逻辑进行画面的的更新，这会产生一个问题：我希望呈现的”Game Over!“提示信息也无法在画面上显示，更无法达到移动的效果。

```c++
	 if(gameOver)
		 return;
```

​	所以，我取消了函数立即返回的做法，并通过宏定义添加了一个新的”方向“NONE，当小蛇发生身体碰撞后，会将自身移动方向变为NONE；而这一方向下，蛇头位置不会再发生改变，但画面重绘仍旧会进行，不会妨碍提示信息的动态呈现。

​	但这也存在一个缺陷：虽然蛇头不会再移动，但蛇身依旧会随原有方向移动，直到小蛇只剩下蛇头为止。

```c++
#define NONE 5
```

```c++
if (gameOver) {
		 direction = NONE;
	 }
```

```c++
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
         //NONE方向下，蛇头位置不会再发生改变，但画面依旧重绘
		 case NONE:
			 SnakeHead_X += 0;
			 SnakeHead_Y += 0;
			 break;
		 }
```

​	此外，我还对绘制函数 display() 进行了适当的改变。当检测到小蛇的身体碰撞发生时，即绘制”Game Over!“的提示信息。

```c++
void display(void){
	...

	if (isCollision()) {
		 showGameOver();
	}
	 
	...
}
```

​	具体效果见附件”贪吃蛇改进动画.mp4“，代码注释见附件”SOpenGL.CPP“。
