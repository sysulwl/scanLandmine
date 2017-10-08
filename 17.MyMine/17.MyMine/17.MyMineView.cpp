
// 17.MyMineView.cpp : CMy17MyMineView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy17MyMineView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	//ON_WM_RBUTTONUP()
	/*----------�Լ��������Ϣӳ�亯��-------------------*/
	ON_WM_LBUTTONDOWN() //���
	ON_WM_RBUTTONDOWN()  //�Ҽ�
	ON_WM_TIMER()	//��ʱ��
	ON_WM_CREATE() //���ڴ���
	/*----------�Լ��������Ϣӳ�亯��-------------------*/
END_MESSAGE_MAP()


void CMy17MyMineView::landmineSet() {
	srand(time(0)); //��ʼ�����������
	int cnt = 0;
	int rndrow, rndcol;

	while (cnt < landmineNum) { //����cnt����
		rndrow = rand() % m_RowCount + 1; //�����x����
		rndcol = rand() % m_ColCount + 1; //�����y����
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
						landmineAround[i][j] += haveLandmine[i + w][j + k];//ά����Χ�Ź����׵�����
					}
			}

}
// CMy17MyMineView ����/����

CMy17MyMineView::CMy17MyMineView()
{
	// TODO: �ڴ˴���ӹ������

	m_RowCount = 14; 	//����
	m_ColCount = 25; //����	
	landmineNum = 80;    //����
	leftNum = 80;
	seconds = 0;
	overflag = false;

	restart.LoadBitmap(IDB_BITMAP27);

	for (int i = 0; i<20; ++i)
		bitmap[i].LoadBitmap(IDB_BITMAP1 + i); //��λͼ���ؽ���
	for (int ii = 20; ii<30; ++ii)
		bitmap[ii].LoadBitmap(IDB_BITMAP1 + ii + 1);
	memset(haveLandmine, 0, sizeof(haveLandmine));
	memset(landmineAround, 0, sizeof(landmineAround));
	memset(status, 0, sizeof(status));
	landmineSet(); //������
}

CMy17MyMineView::~CMy17MyMineView()
{
}

BOOL CMy17MyMineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMy17MyMineView ����

void CMy17MyMineView::OnDraw(CDC* pDC)
{
	CMy17MyMineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	//����
	CBrush mybrush1;                             // ��ˢ
	mybrush1.CreateSolidBrush(RGB(192, 192, 192)); // ��ɫֱˢ 
	CRect myrect1(0, 0, 800, 800);                 //�������
	pDC->FillRect(myrect1, &mybrush1); //�ͻ��ˣ���ɫ����

	//2���ԳƵĺڿ�
	CBrush mybrush;
	mybrush.CreateSolidBrush(RGB(0, 0, 0)); //��ɫ��ˢŶ
	CRect myrect(40, 20, 140, 80);        //��ߺ�ɫ����������ʾ����
	pDC->FillRect(myrect, &mybrush);

	CRect myrect2(650, 20, 750, 80);    //�ұߺ�ɫ����������ʾ����ʱ��
	pDC->FillRect(myrect2, &mybrush);


	//���ڿ�ʼ��CPen������
	CPen mypen;
	CPen*myoldPen;
	mypen.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));  //��ʽ�� ��ȡ���ɫ(��ɫ)
	myoldPen = pDC->SelectObject(&mypen);           //��ֹ�ڴ�й¶,�ɵ�pen

	//-----------------------------���ڿ�İ���
	pDC->MoveTo(40, 80);
	pDC->LineTo(140, 80);
	pDC->LineTo(140, 20);
	pDC->MoveTo(650, 80);
	pDC->LineTo(750, 80);
	pDC->LineTo(750, 20);

	//-----------------------------�������ı߽���
	for (int i = 0; i<m_ColCount; i++)
		for (int j = 0; j<m_RowCount; j++) {
			pDC->MoveTo(20 + i * 30, 100 + j * 30 + 28);
			pDC->LineTo(20 + i * 30, 100 + j * 30);
			pDC->LineTo(20 + i * 30 + 28, 100 + j * 30);
		}

	pDC->SelectObject(myoldPen);

	/*----------------------�ѱ߽��߼Ӵ�һ�㣬�γɵ�Ӱ��Ч��----------------------*/
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
	/*----------------------�ѱ߽��߼Ӵ�һ�㣬�γɵ�Ӱ��Ч��----------------------*/
	/*----------------------*/
	/*----------------�ⲿ�ֻ�����������Ц��----------------------*/
	CDC Dc;
	if (Dc.CreateCompatibleDC(pDC) == FALSE)
		AfxMessageBox(_T("Can't create DC"));
	Dc.SelectObject(restart);
	pDC->StretchBlt(365, 20, 60, 60, &Dc, 0, 0, 30, 30, SRCCOPY);
	/*----------------�ⲿ�ֻ�����������Ц��----------------------*/

	
	/*--------------------�����߼�����------------------------------------*/
	for (int iii = 1; iii <= m_ColCount; iii++)
		for (int jjj = 1; jjj <= m_RowCount; jjj++) {
			if (status[jjj][iii] == 1 && haveLandmine[jjj][iii] == 1) {//�㿪������ط������������׵ġ�
				Dc.SelectObject(bitmap[12]); //��������ȵ��׵�λͼ
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
	/*--------------------�����߼�����------------------------------------*/
	int nOldDC = pDC->SaveDC();
	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->SetBkColor(RGB(0, 0, 0));
	CFont font;

	pDC->SelectObject(&font);
	CString str, str1;
	//�����ж���ʾλ����������λǰ���0
	if (leftNum<10)	str.Format(_T("00%d"), leftNum);
	else             str.Format(_T("0%d"), leftNum);
	if (seconds<10)	str1.Format(_T("00%d"), seconds);
	else if (seconds<100) str1.Format(_T("0%d"), seconds);
	else  str1.Format(_T("%d"), seconds);


	LOGFONT lf;
	CFont fontTemp, *pFontOld;
	pDC->GetCurrentFont()->GetLogFont(&lf);
	lstrcpy(lf.lfFaceName, _T("����"));
	lf.lfWidth = 30;
	lf.lfWeight = FW_HEAVY;
	lf.lfHeight = 59;
	fontTemp.CreateFontIndirect(&lf);      //��������
	pFontOld = pDC->SelectObject(&fontTemp);
	pDC->TextOut(45, 20, str);
	//ʹ�õ�ǰѡ���������ָ��λ������ı��� ����xָ���ı���ʼ���x���ꣻ����yָ���ı���ʼ���y���ꣻ
	//����lpszStringΪҪ������ı��ַ���������nCountָ���ַ����е��ֽڸ���������strΪ����Ҫ������ַ���CString����
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
	status[x][y] = 1;//�㿪�÷���
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
		//����㿪�ķŸ���Χ�ĸ��Ӷ�û�е��ף�
	}
}

void CMy17MyMineView::gameOver()
{
	overflag = true;
	for (int i = 1; i <= m_RowCount; i++)
		for (int j = 1; j <= m_ColCount; j++) {
			if (haveLandmine[i][j] == 1) status[i][j] = 1; //��Ϸ������չ��������׷ֲ�ͼ������
		}
	restart.DeleteObject();
	restart.LoadBitmap(IDB_BITMAP28);
}

// CMy17MyMineView ��ӡ


void CMy17MyMineView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy17MyMineView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMy17MyMineView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMy17MyMineView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CMy17MyMineView ���

#ifdef _DEBUG
void CMy17MyMineView::AssertValid() const
{
	CView::AssertValid();
}

void CMy17MyMineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy17MyMineDoc* CMy17MyMineView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy17MyMineDoc)));
	return (CMy17MyMineDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy17MyMineView ��Ϣ�������
void CMy17MyMineView::OnLButtonDown(UINT nFlags, CPoint point)  //���������������Ϣ������
{
	// TODO: Add your message handler code here and/or call default
	CRect rectlei;
	rectlei.left = 20;
	rectlei.right = 770;
	rectlei.top = 100;
	rectlei.bottom = 580;
	if (point.x < 425 && point.x >= 365 && point.y < 80 && point.y >= 20) {
		myRestart(); //����˸ÿؼ�����ʾ���¿�ʼ��Ϸ
	}
	else if (overflag) return; //�Ѿ�ʧ���ˣ���ô�㶼û��Ӧ��
	if (point.x < 770 && point.x >= 20 && point.y < 580 && point.y >= 100) {
		int hitx = (point.x - 20) / 30 + 1;
		int hity = (point.y - 100) / 30 + 1; //����ĸ�������
		if (status[hity][hitx] == 0) { //û�е����
			if (haveLandmine[hity][hitx] == 1) { //����һ���ף���Ϸ����
				gameOver();
				status[hity][hitx] = 1;
				Invalidate();
			}
			else if (landmineAround[hity][hitx] > 0) { //��ʾ��Χ�˸����ӵ�������
												  // �����ػ�
				status[hity][hitx] = 1;
				CRect rect;
				rect.left = hitx * 30 + 20 - 30;
				rect.right = hitx * 30 + 20;
				rect.top = hity * 30 + 100 - 30;
				rect.bottom = hity * 30 + 100;
				//Invalidate();
				InvalidateRect(&rect);//���»���
			}
			else {
				zeroHit(hity, hitx); //��������
				InvalidateRect(&rectlei);
			}
		}
		else if (status[hity][hitx] == 1) { //�Ѿ��㿪����
			int cnt = 0; //ͳ������ԱߵľŹ���ĸ���
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
	if (overflag) return; //�Ѿ�����
	if (point.x < 770 && point.x >= 20 && point.y < 580 && point.y >= 100) { //��ʾ�ڿͻ�������
		int hitx = (point.x - 20) / 30 + 1; //�õ��ڸ��ӵ��±�
		int hity = (point.y - 100) / 30 + 1;
		if (status[hity][hitx] == 0) {
			if (leftNum == 0) return; //û������
			status[hity][hitx] = 2; //���Ϊ��
			leftNum--;//����һ������
			CRect rect;
			rect.left = hitx * 30 + 20 - 30;
			rect.right = hitx * 30 + 20;
			rect.top = hity * 30 + 100 - 30;
			rect.bottom = hity * 30 + 100;
			InvalidateRect(&rect); //�����������������~

			rect.left = 40;
			rect.right = 140;
			rect.top = 20;
			rect.bottom = 80;
			InvalidateRect(&rect);
		}
		else if (status[hity][hitx] == 2) { //ȡ�����Ϊ����
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
			InvalidateRect(&rect); //�����ⲿ�־���
		}

	}
	//CView::OnRButtonDown(nFlags, point);
}

void CMy17MyMineView::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (overflag) return;//�Ѿ����ˣ�ʱ�䲻��������ʾ
	if (seconds < 999) seconds++;
	CRect rect;
	rect.left = 650;
	rect.right = 750;
	rect.top = 20;
	rect.bottom = 80;
	InvalidateRect(&rect); //�����ⲿ������
	CView::OnTimer(nIDEvent);
}

int CMy17MyMineView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	SetTimer(1, 1000, NULL); //���ö�ʱ��
	return 0;
}