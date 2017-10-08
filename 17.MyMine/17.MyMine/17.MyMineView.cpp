
// 17.MyMineView.cpp : CMy17MyMineView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "17.MyMine.h"
#endif
#include "stdafx.h"

#include <cstring>
#include <cmath>
#include "time.h"
#include "17.MyMineDoc.h"
#include "17.MyMineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy17MyMineView

IMPLEMENT_DYNCREATE(CMy17MyMineView, CView)

BEGIN_MESSAGE_MAP(CMy17MyMineView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy17MyMineView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	//ON_WM_RBUTTONUP()
	/*----------自己加入的消息映射函数-------------------*/
	ON_WM_LBUTTONDOWN() //左键
	ON_WM_RBUTTONDOWN()  //右键
	ON_WM_TIMER()	//计时器
	ON_WM_CREATE() //窗口创建
	/*----------自己加入的消息映射函数-------------------*/
END_MESSAGE_MAP()


void CMy17MyMineView::landmineSet() {
	srand(time(0)); //初始化随机数种子
	int cnt = 0;
	int rndrow, rndcol;

	while (cnt < landmineNum) { //生成cnt个雷
		rndrow = rand() % m_RowCount + 1; //随机的x坐标
		rndcol = rand() % m_ColCount + 1; //随机的y坐标
		if (haveLandmine[rndrow][rndcol] == 0) {
			cnt++;
			haveLandmine[rndrow][rndcol] = 1;
		}
	}
	for (int i = 1; i <= m_RowCount; i++)
		for (int j = 1; j <= m_ColCount; j++)
			if (haveLandmine[i][j] == 0) {
				for (int w = -1; w <= 1; w++)
					for (int k = -1; k <= 1; k++) {
						landmineAround[i][j] += haveLandmine[i + w][j + k];//维护周围九宫格雷的数量
					}
			}

}
// CMy17MyMineView 构造/析构

CMy17MyMineView::CMy17MyMineView()
{
	// TODO: 在此处添加构造代码

	m_RowCount = 14; 	//行数
	m_ColCount = 25; //列数	
	landmineNum = 80;    //雷数
	leftNum = 80;
	seconds = 0;
	overflag = false;

	restart.LoadBitmap(IDB_BITMAP27);

	for (int i = 0; i<20; ++i)
		bitmap[i].LoadBitmap(IDB_BITMAP1 + i); //把位图加载进来
	for (int ii = 20; ii<30; ++ii)
		bitmap[ii].LoadBitmap(IDB_BITMAP1 + ii + 1);
	memset(haveLandmine, 0, sizeof(haveLandmine));
	memset(landmineAround, 0, sizeof(landmineAround));
	memset(status, 0, sizeof(status));
	landmineSet(); //设置类
}

CMy17MyMineView::~CMy17MyMineView()
{
}

BOOL CMy17MyMineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMy17MyMineView 绘制

void CMy17MyMineView::OnDraw(CDC* pDC)
{
	CMy17MyMineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	//背景
	CBrush mybrush1;                             // 画刷
	mybrush1.CreateSolidBrush(RGB(192, 192, 192)); // 灰色直刷 
	CRect myrect1(0, 0, 800, 800);                 //填充区域
	pDC->FillRect(myrect1, &mybrush1); //客户端，灰色背景

	//2个对称的黑框
	CBrush mybrush;
	mybrush.CreateSolidBrush(RGB(0, 0, 0)); //黑色画刷哦
	CRect myrect(40, 20, 140, 80);        //左边黑色区域，用来显示雷数
	pDC->FillRect(myrect, &mybrush);

	CRect myrect2(650, 20, 750, 80);    //右边黑色区域，用来显示所用时间
	pDC->FillRect(myrect2, &mybrush);


	//现在开始用CPen来划线
	CPen mypen;
	CPen*myoldPen;
	mypen.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));  //样式、 宽度、颜色(白色)
	myoldPen = pDC->SelectObject(&mypen);           //防止内存泄露,旧的pen

	//-----------------------------画黑框的白线
	pDC->MoveTo(40, 80);
	pDC->LineTo(140, 80);
	pDC->LineTo(140, 20);
	pDC->MoveTo(650, 80);
	pDC->LineTo(750, 80);
	pDC->LineTo(750, 20);

	//-----------------------------画雷区的边界线
	for (int i = 0; i<m_ColCount; i++)
		for (int j = 0; j<m_RowCount; j++) {
			pDC->MoveTo(20 + i * 30, 100 + j * 30 + 28);
			pDC->LineTo(20 + i * 30, 100 + j * 30);
			pDC->LineTo(20 + i * 30 + 28, 100 + j * 30);
		}

	pDC->SelectObject(myoldPen);

	/*----------------------把边界线加粗一点，形成叠影的效果----------------------*/
	CPen mypen2;
	CPen*myoldPen2;
	mypen2.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	myoldPen2 = pDC->SelectObject(&mypen2);

	for (int ii = 0; ii<m_ColCount; ii++)
		for (int jj = 0; jj<m_RowCount; jj++) {
			pDC->MoveTo(20 + ii * 30 + 28, 100 + jj * 30);
			pDC->LineTo(20 + ii * 30 + 28, 100 + jj * 30 + 28);
			pDC->LineTo(20 + ii * 30, 100 + jj * 30 + 28);
		}
	pDC->SelectObject(myoldPen2);
	/*----------------------把边界线加粗一点，形成叠影的效果----------------------*/
	/*----------------------*/
	/*----------------这部分画用来重启的笑脸----------------------*/
	CDC Dc;
	if (Dc.CreateCompatibleDC(pDC) == FALSE)
		AfxMessageBox(_T("Can't create DC"));
	Dc.SelectObject(restart);
	pDC->StretchBlt(365, 20, 60, 60, &Dc, 0, 0, 30, 30, SRCCOPY);
	/*----------------这部分画用来重启的笑脸----------------------*/

	
	/*--------------------加入逻辑控制------------------------------------*/
	for (int iii = 1; iii <= m_ColCount; iii++)
		for (int jjj = 1; jjj <= m_RowCount; jjj++) {
			if (status[jjj][iii] == 1 && haveLandmine[jjj][iii] == 1) {//点开了这个地方，而且是有雷的。
				Dc.SelectObject(bitmap[12]); //载入这个踩到雷的位图
				pDC->StretchBlt(20 + 30 * (iii - 1), 100 + 30 * (jjj - 1), 28, 28, &Dc, 0, 0, 14, 14, SRCCOPY);
			}
			else if (status[jjj][iii] == 1 && haveLandmine[jjj][iii] == 0) {
				Dc.SelectObject(bitmap[landmineAround[jjj][iii] + 14]);
				pDC->StretchBlt(20 + 30 * (iii - 1), 100 + 30 * (jjj - 1), 28, 28, &Dc, 0, 0, 14, 14, SRCCOPY);
			}
			else if (status[jjj][iii] == 2) {
				Dc.SelectObject(bitmap[22]);
				pDC->StretchBlt(20 + 30 * (iii - 1), 100 + 30 * (jjj - 1), 28, 28, &Dc, 0, 0, 14, 14, SRCCOPY);
			}
		}
	/*--------------------加入逻辑控制------------------------------------*/
	int nOldDC = pDC->SaveDC();
	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->SetBkColor(RGB(0, 0, 0));
	CFont font;

	pDC->SelectObject(&font);
	CString str, str1;
	//利用判断显示位数，不够三位前面加0
	if (leftNum<10)	str.Format(_T("00%d"), leftNum);
	else             str.Format(_T("0%d"), leftNum);
	if (seconds<10)	str1.Format(_T("00%d"), seconds);
	else if (seconds<100) str1.Format(_T("0%d"), seconds);
	else  str1.Format(_T("%d"), seconds);


	LOGFONT lf;
	CFont fontTemp, *pFontOld;
	pDC->GetCurrentFont()->GetLogFont(&lf);
	lstrcpy(lf.lfFaceName, _T("宋体"));
	lf.lfWidth = 30;
	lf.lfWeight = FW_HEAVY;
	lf.lfHeight = 59;
	fontTemp.CreateFontIndirect(&lf);      //创建字体
	pFontOld = pDC->SelectObject(&fontTemp);
	pDC->TextOut(45, 20, str);
	//使用当前选择的字体在指定位置输出文本。 参数x指定文本起始点的x坐标；参数y指定文本起始点的y坐标；
	//参数lpszString为要输出的文本字符串；参数nCount指定字符串中的字节个数；参数str为包含要输出的字符的CString对象
	pDC->TextOut(650, 20, str1);
	pDC->SelectObject(pFontOld);
}
void CMy17MyMineView::myRestart()
{

	overflag = false;
	landmineNum = landmineNum;
	leftNum = landmineNum;
	seconds = 0;

	restart.DeleteObject();
	restart.LoadBitmap(IDB_BITMAP27);

	memset(haveLandmine, 0, sizeof(haveLandmine));
	memset(landmineAround, 0, sizeof(landmineAround));
	memset(status, 0, sizeof(status));
	landmineSet();
	Invalidate();

}

void CMy17MyMineView::zeroHit(int x, int y)
{
	status[x][y] = 1;//点开该方格
	if (landmineAround[x][y] == 0) {
		for (int w = -1; w <= 1; w++)
			for (int k = -1; k <= 1; k++) {
				int nx = x + w;
				int ny = y + k;
				if (nx > 0 && nx <= m_RowCount &&
					ny > 0 && ny <= m_ColCount &&
					!status[nx][ny] && !haveLandmine[nx][ny])
					zeroHit(nx, ny);
			}
		//这个点开的放个周围的格子都没有地雷，
	}
}

void CMy17MyMineView::gameOver()
{
	overflag = true;
	for (int i = 1; i <= m_RowCount; i++)
		for (int j = 1; j <= m_ColCount; j++) {
			if (haveLandmine[i][j] == 1) status[i][j] = 1; //游戏结束，展现这个地雷分布图来看看
		}
	restart.DeleteObject();
	restart.LoadBitmap(IDB_BITMAP28);
}

// CMy17MyMineView 打印


void CMy17MyMineView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy17MyMineView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMy17MyMineView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMy17MyMineView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

/*
void CMy17MyMineView::OnRButtonUp(UINT  nFlags , CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}
*/

void CMy17MyMineView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy17MyMineView 诊断

#ifdef _DEBUG
void CMy17MyMineView::AssertValid() const
{
	CView::AssertValid();
}

void CMy17MyMineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy17MyMineDoc* CMy17MyMineView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy17MyMineDoc)));
	return (CMy17MyMineDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy17MyMineView 消息处理程序
void CMy17MyMineView::OnLButtonDown(UINT nFlags, CPoint point)  //左键单击触发的消息处理函数
{
	// TODO: Add your message handler code here and/or call default
	CRect rectlei;
	rectlei.left = 20;
	rectlei.right = 770;
	rectlei.top = 100;
	rectlei.bottom = 580;
	if (point.x < 425 && point.x >= 365 && point.y < 80 && point.y >= 20) {
		myRestart(); //点击了该控件，表示重新开始游戏
	}
	else if (overflag) return; //已经失败了，怎么点都没反应滴
	if (point.x < 770 && point.x >= 20 && point.y < 580 && point.y >= 100) {
		int hitx = (point.x - 20) / 30 + 1;
		int hity = (point.y - 100) / 30 + 1; //点击的格子坐标
		if (status[hity][hitx] == 0) { //没有点击过
			if (haveLandmine[hity][hitx] == 1) { //点了一个雷，游戏结束
				gameOver();
				status[hity][hitx] = 1;
				Invalidate();
			}
			else if (landmineAround[hity][hitx] > 0) { //显示周围八个格子的雷数量
												  // 部分重画
				status[hity][hitx] = 1;
				CRect rect;
				rect.left = hitx * 30 + 20 - 30;
				rect.right = hitx * 30 + 20;
				rect.top = hity * 30 + 100 - 30;
				rect.bottom = hity * 30 + 100;
				//Invalidate();
				InvalidateRect(&rect);//重新绘制
			}
			else {
				zeroHit(hity, hitx); //开阔疆土
				InvalidateRect(&rectlei);
			}
		}
		else if (status[hity][hitx] == 1) { //已经点开过了
			int cnt = 0; //统计这个旁边的九宫格的个数
			for (int w = -1; w <= 1; w++)
			{
				for (int k = -1; k <= 1; k++) {
					int nx = hity + w;
					int ny = hitx + k;
					if (nx > 0 && nx <= m_RowCount &&
						ny > 0 && ny <= m_ColCount &&
						status[nx][ny] == 2 && haveLandmine[nx][ny])
						cnt++;
				}
			}

			if (cnt == landmineAround[hity][hitx]) {
				bool allflag = false;
				for (int w = -1; w <= 1; w++)
					for (int k = -1; k <= 1; k++)
					{
						int nx = hity + w;
						int ny = hitx + k;
						if (nx > 0 && nx <= m_RowCount && ny > 0 && ny <= m_ColCount && !status[nx][ny])
						{
							if (landmineAround[nx][ny]>0)
								status[nx][ny] = 1;
							else
							{
								allflag = true;
								zeroHit(nx, ny);
							}
						}

					}

				if (allflag) InvalidateRect(&rectlei);
				else {
					CRect rect;
					rect.left = hitx * 30 + 20 - 30 - 30;
					rect.right = hitx * 30 + 20 + 30;
					rect.top = hity * 30 + 100 - 30 - 30;
					rect.bottom = hity * 30 + 100 + 30;
					InvalidateRect(&rect);
				}
			}


		}

	}
	CView::OnLButtonDown(nFlags, point);
}

void CMy17MyMineView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (overflag) return; //已经输了
	if (point.x < 770 && point.x >= 20 && point.y < 580 && point.y >= 100) { //表示在客户区里面
		int hitx = (point.x - 20) / 30 + 1; //得到在格子的下标
		int hity = (point.y - 100) / 30 + 1;
		if (status[hity][hitx] == 0) {
			if (leftNum == 0) return; //没有雷了
			status[hity][hitx] = 2; //标记为雷
			leftNum--;//少了一个雷了
			CRect rect;
			rect.left = hitx * 30 + 20 - 30;
			rect.right = hitx * 30 + 20;
			rect.top = hity * 30 + 100 - 30;
			rect.bottom = hity * 30 + 100;
			InvalidateRect(&rect); //点的这块区域更新啦！~

			rect.left = 40;
			rect.right = 140;
			rect.top = 20;
			rect.bottom = 80;
			InvalidateRect(&rect);
		}
		else if (status[hity][hitx] == 2) { //取消标记为雷区
			status[hity][hitx] = 0;
			leftNum++;
			CRect rect;
			rect.left = hitx * 30 + 20 - 30;
			rect.right = hitx * 30 + 20;
			rect.top = hity * 30 + 100 - 30;
			rect.bottom = hity * 30 + 100;
			InvalidateRect(&rect);

			rect.left = 40;
			rect.right = 140;
			rect.top = 20;
			rect.bottom = 80;
			InvalidateRect(&rect); //更新这部分矩形
		}

	}
	//CView::OnRButtonDown(nFlags, point);
}

void CMy17MyMineView::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (overflag) return;//已经输了，时间不会再有提示
	if (seconds < 999) seconds++;
	CRect rect;
	rect.left = 650;
	rect.right = 750;
	rect.top = 20;
	rect.bottom = 80;
	InvalidateRect(&rect); //更新这部分区域
	CView::OnTimer(nIDEvent);
}

int CMy17MyMineView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	SetTimer(1, 1000, NULL); //设置定时器
	return 0;
}