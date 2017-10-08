
// 17.MyMineView.h : CMy17MyMineView ��Ľӿ�
//

#pragma once


class CMy17MyMineView : public CView
{
protected: // �������л�����
	CMy17MyMineView();
	DECLARE_DYNCREATE(CMy17MyMineView)

// ����
public:
	CMy17MyMineDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMy17MyMineView();
/*-------------------�Լ����ӵĳ�Ա����--------------------*/
public:
	void myRestart();//���¿���
	void gameOver(); //��Ϸ������
	void zeroHit(int x, int y);
	int m_RowCount; // ����
	int m_ColCount; // ����
	int landmineNum;     // ����
	int leftNum;    // ʣ������
	int overflag;   // ��ʾ�Ƿ�����ı�־
	int seconds;    //�Ѿ���ȥ��ʱ��
	CString m_MousePoint;
	CBitmap restart;			//���֮�����¿�ʼ��һ�̵���Ϸ��λͼ
	CBitmap test1;
	CBitmap bitmap[31];        // λͼӳ��
	int haveLandmine[100][100];    //  ����
	int landmineAround[100][100]; // ��Χ����
	int status[100][100];   // 0-δ�㿪 1-�㿪 2-��� 3-��

	void landmineSet();         //�������� ������Χ����
/*-------------------�Լ����ӵĳ�Ա����--------------------*/
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	//afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	/*----------------�Լ��������Ϣӳ�亯��---------------------*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	/*----------------�Լ��������Ϣӳ�亯��---------------------*/
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // 17.MyMineView.cpp �еĵ��԰汾
inline CMy17MyMineDoc* CMy17MyMineView::GetDocument() const
   { return reinterpret_cast<CMy17MyMineDoc*>(m_pDocument); }
#endif

