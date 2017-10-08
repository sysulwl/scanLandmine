
// 17.MyMineView.h : CMy17MyMineView 类的接口
//

#pragma once


class CMy17MyMineView : public CView
{
protected: // 仅从序列化创建
	CMy17MyMineView();
	DECLARE_DYNCREATE(CMy17MyMineView)

// 特性
public:
	CMy17MyMineDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMy17MyMineView();
/*-------------------自己所加的成员如下--------------------*/
public:
	void myRestart();//重新开盘
	void gameOver(); //游戏结束了
	void zeroHit(int x, int y);
	int m_RowCount; // 行数
	int m_ColCount; // 列数
	int landmineNum;     // 雷数
	int leftNum;    // 剩余雷数
	int overflag;   // 表示是否结束的标志
	int seconds;    //已经过去的时间
	CString m_MousePoint;
	CBitmap restart;			//点击之后重新开始新一盘的游戏的位图
	CBitmap test1;
	CBitmap bitmap[31];        // 位图映射
	int haveLandmine[100][100];    //  雷区
	int landmineAround[100][100]; // 周围雷数
	int status[100][100];   // 0-未点开 1-点开 2-标记 3-？

	void landmineSet();         //生成雷区 设置周围雷数
/*-------------------自己所加的成员如下--------------------*/
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	//afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	/*----------------自己加入的消息映射函数---------------------*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	/*----------------自己加入的消息映射函数---------------------*/
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // 17.MyMineView.cpp 中的调试版本
inline CMy17MyMineDoc* CMy17MyMineView::GetDocument() const
   { return reinterpret_cast<CMy17MyMineDoc*>(m_pDocument); }
#endif

