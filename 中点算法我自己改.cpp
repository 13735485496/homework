//绘制斜率大于0且小于1的线段
#include <GL/glut.h>
#include <iostream>//输入输出流
#include <cmath>//包含了数学函数
using namespace std;

void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);//设置清除颜色
	glMatrixMode(GL_PROJECTION);////将当前矩阵指定为投影矩阵
	glLoadIdentity();//功能是重置当前指定的矩阵为单位矩阵
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);//定义了一个二维图像投影矩阵
}

void display()
//算法条件：x1>x0,y1>y0,x1-x0>y1-y0,,斜率大于0小于1
{
	int x0, y0, x1, y1;
	cout << "请输入起点和终点坐标:x1,y1,x2,y2:" << endl;
	cin >> x0 >> y0 >> x1 >> y1;
	int a = y1 - y0, b = x1 - x0;//▲y=a ▲x=b
	int d = 2 * a - b;//dm=2*a-b
	int x = x0, y = y0;

	glClear(GL_COLOR_BUFFER_BIT);//深度清除缓冲区
	glColor3f(1.0, 0.0, 0.0);//设置为红色
	glBegin(GL_POINTS);//开始画点
	glVertex2i(x, y);
	while (x <= x1) {
		glVertex2i(x, y);
		if (d <= 0) {//dm<=0
			d = d + 2 * a;//dm=dm+2*a
			
		}
		else {
			d =d+ 2 * a-2 * b;//dm=dm+2*a-2*b
			y=y+1;
		}
		x++;
	}






	for (y0 == y1; x < x1; x++)
		x = x + 1;



	glEnd();

	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//函数原型void glutInitDisplayMode(unsigned int mode)，函数功能为设置初始显示模式。
	glutInitWindowPosition(50, 100); //设置初始窗口的位置(窗口左上角相对于桌面坐标(x, y))
	glutInitWindowSize(640, 480);//窗口尺寸
	glutCreateWindow("Mid-point line drawing");//显示exe的名称
	init();
	glutDisplayFunc(display);//glutDisplayFunc函数用于注册一个绘图函数， 这样操作系统在必要时刻就会对窗体进行重新绘制操作
	glutMainLoop();//glutMainLoop进入GLUT事件处理循环，让所有的与“事件”有关的函数调用无限循环。 
	return 0;
}
