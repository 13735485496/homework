#include<Gl/glut.h>
#include <process.h>//这里面包含了，过程控制有关的函数。

class wcPt2D {
public:
	float x, y;//定义一个类
};

GLubyte encode(wcPt2D pt, wcPt2D winMin, wcPt2D winMax);//undefined byte未定义的字节

void swapPts(wcPt2D* p1, wcPt2D* p2);//坐标函数

void swapCodes(GLubyte* c1, GLubyte* c2);//码值函数

void lineClipCohSuth(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2);//切割函数

void myKeyBoard(unsigned char key, int x, int y);//声明添加键盘回调函数

const GLint winLeftBitCode = 0x1;//表示0001由上下右左原则可知为窗口左边线（16进制转化为2进制0x1=0001）下方同理16进制前缀为0x
const GLint winRightBitCode = 0x2;//表示0010由上下右左原则可知为窗口右边线
const GLint winBottomBitCode = 0x4;//表示0100由上下右左原则可知为窗口下边线
const GLint winTopBitCode = 0x8;//表示1000由上下右左原则可知为窗口上边线

inline GLint inside(GLint code) {//这里的GLint可能表示一种int型的数据类型
	return GLint(!code);//判断点是否在裁剪区内,若在(代码为0000)则返回真（1）。(!0000=1）
}

inline GLint reject(GLint code1, GLint code2) {
	return GLint(code1 & code2);//判断直线明显在裁剪窗口外，若code1＆code2=1则说明线在窗口的同一侧（只要有一位编码相同则在同一侧）
}

inline GLint accept(GLint code1, GLint code2) {
	return GLint(!(code1 | code2));//判断直线明显在裁剪窗口内，若在，则！（code1|code2）=1，说明两个点都是0000     （只要有一个编码有一位是1则或运算=1）
}

GLubyte encode(wcPt2D pt, wcPt2D winMin, wcPt2D winMax) {
	GLubyte code = 0x00;
	if (pt.x < winMin.x) code = code | winLeftBitCode;//对照书上算法left=0001 right=0010 bottom=0100 top=1000
	if (pt.x > winMax.x) code = code | winRightBitCode;
	if (pt.y < winMin.y) code = code | winBottomBitCode;
	if (pt.y > winMax.y) code = code | winTopBitCode;
	return (code);////编码函数,将每个点转换成四位二进制数,函数返回点的编码
}

void swapPts(wcPt2D* p1, wcPt2D* p2) {
	wcPt2D tmp;
	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}//交换两个点的坐标

void swapCodes(GLubyte* c1, GLubyte* c2) {
	GLubyte tmp;
	tmp = *c1;
	*c1 = *c2;
	*c2 = tmp;
}//交换两个点的码值

void lineClipCohSuth(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2) {
	//切割函数,当直线有一部分在窗口内时应求出线与窗口的交点,再用交点进行运算,直到所有点都在窗口内为止。
	GLubyte code1, code2;//切割函数
	GLint done = false, plotLine = false;
	GLfloat m;
	while (!done) {
		code1 = encode(p1, winMin, winMax);//为p1编码
		code2 = encode(p2, winMin, winMax);//为p2编码
		if (accept(code1, code2)) {//表示两个点连的线都在区域内
			done = true;
			plotLine = true;
		}
		else if (reject(code1, code2)) {//表示两个点练的线都不在区域内
			done = true;
		}
		else {
			if (inside(code1)) {//交换坐标值和编码
				swapPts(&p1, &p2);
				swapCodes(&code1, &code2);
			}
			if (p2.x != p1.x) m = (p2.y - p1.y) / (p2.x - p1.x);//m为斜率，计算斜率
			if (code1 & winLeftBitCode) {//开始裁剪，以下与运算若结果为真，则p1在边界外，此时将p1移向与改变界的交点
				p1.y += (winMin.x - p1.x) * m;
				p1.x = winMin.x;
			}
			else if (code1 & winRightBitCode) {
				p1.y += (winMax.x - p1.x) * m;
				p1.x = winMax.x;
			}
			else if (code1 & winBottomBitCode) {
				if (p2.x != p1.x)
					p1.x += (winMin.y - p1.y) / m;
				p1.y = winMin.y;
			}
			else if (code1 & winTopBitCode) {
				if (p2.x != p1.x)
					p1.x = (winMax.y - p1.y) / m;
				p1.y = winMax.y;
			}
		}
	}
	if (plotLine)
		glBegin(GL_LINES);//GL_LINES：把每一个顶点作为一个独立的线段，顶点2n－1和2n之间共定义了n条线段，总共绘制N/2条线段
	glLineWidth(10);
	glColor3f(1, 0, 0);//红色
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
}

void drawpolygon(double cd[]) {//绘制函数
	glBegin(GL_LINE_LOOP);//绘制从第一个顶点到最后一个顶点依次相连的一组线段，然后最后一个顶点和第一个顶点相连，第n和n+1个顶点定义了线段n，总共绘制n条线段
	glLineWidth(10);
	for (int i = 0; i < 8; i = i + 2) {
		glVertex2f(cd[i], cd[i + 1]);
	}
	glEnd();
}

void drawline(double cd[]) {//绘制函数
	glBegin(GL_LINES);
	glLineWidth(10);
	for (int i = 0; i < 8; i = i + 2) {
		glVertex2f(cd[i], cd[i + 1]);
	}
	glEnd();
}

void myKeyBoard(unsigned char key, int x, int y) {//键盘控制函数切割
	wcPt2D winMin = { 100, 100 };
	wcPt2D winMax = { 400, 400 };
	wcPt2D p1 = { 0, 0 };
	wcPt2D p2 = { 500, 500 };


	wcPt2D p11 = { 200, 300 };
	wcPt2D p21 = { 300, 200 };

	wcPt2D p12 = { 10, 50 };
	wcPt2D p22 = { 50, 10 };
	if (key == 13) {
		lineClipCohSuth(winMin, winMax, p1, p2);
		lineClipCohSuth(winMin, winMax, p11, p21);
		lineClipCohSuth(winMin, winMax, p12, p22);

	}
	glFlush();
	if (key == 27)
		exit(0);
}

void display(void) {//绘制图像
	double cd[8] = { 100, 50, 100, 500, 400, 400, 350, 200 };
	double line[4] = { 20, 20, 480, 480 };
	double line1[4] = { 200, 300, 300, 200 };
	double line2[4] = { 20, 80, 40, 10 };
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 500, 500);//可视区域与窗口大小一样，调用glViewPort函数来决定视见区域,告诉OpenGL应把渲染之后的图形绘制在窗体的哪个部位。当视见区域是整个窗体时,OpenGL将把渲染结果绘制到整个窗口
	glColor3f(0, 0, 0);//黑色
	drawpolygon(cd);
	drawline(line);
	drawline(line1);
	drawline(line2);
	glFlush();
}

int main(int argc, char** argv) {//主函数
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RED);
	glutInitWindowSize(500, 500);//窗口尺寸
	glutInitWindowPosition(100, 100);//窗口位置
	glutCreateWindow("Cohen-Sutherland"); //显示输出
	glClearColor(1, 1, 1, 0.0);//清除颜色
	glMatrixMode(GL_PROJECTION);//投影矩阵
	gluOrtho2D(0.0, 500.0, 0.0, 500.0);//gluOrtho2D定义了一个二维图像投影矩阵，参数 left,right指明平面的左边和右边的垂直坐标，bottom,top指明平面底部和顶部的水平坐标。
	glutKeyboardFunc(myKeyBoard);//调用键盘回调函数
	glutDisplayFunc(display);//glutDisplayFunc函数用于注册一个绘图函数， 这样操作系统在必要时刻就会对窗体进行重新绘制操作
	glutMainLoop();//glutMainLoop进入GLUT事件处理循环，让所有的与“事件”有关的函数调用无限循环。
	return 0;

}