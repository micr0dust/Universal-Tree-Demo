
// Universal DemoView.cpp: CUniversalDemoView 類別的實作
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以定義在實作預覽、縮圖和搜尋篩選條件處理常式的
// ATL 專案中，並允許與該專案共用文件程式碼。
#ifndef SHARED_HANDLERS
#include "Universal Demo.h"
#endif

#include "Universal DemoDoc.h"
#include "Universal DemoView.h"
#include "CInput.h"

#include<vector>
#include<string>
#include<sstream>
#include<fstream>
#include<bitset>

using std::vector;
using std::string;
using std::stringstream;
using std::bitset;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUniversalDemoView

IMPLEMENT_DYNCREATE(CUniversalDemoView, CView)

BEGIN_MESSAGE_MAP(CUniversalDemoView, CView)
	// 標準列印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_DEMO_PREORDER, &CUniversalDemoView::preorderToTree)
	ON_COMMAND(ID_DEMO_POSTORDER, &CUniversalDemoView::postorderToTree)
	ON_COMMAND(ID_AVL, &CUniversalDemoView::OnAvl)
	ON_COMMAND(ID_DEMO_BINARYSEARCH, &CUniversalDemoView::OnOrder)
	ON_COMMAND(ID_234Tree, &CUniversalDemoView::On234tree)
	ON_COMMAND(ID_DEMO_FRACTALTREE, &CUniversalDemoView::OnfractalTree)
	ON_COMMAND(ID_DEMO_SIERPINSKICARPET, &CUniversalDemoView::OnCarpet)
	ON_COMMAND(ID_DEMO_INSERT, &CUniversalDemoView::OnInsert)
	ON_COMMAND(ID_DEMO_SEARCH, &CUniversalDemoView::OnSearch)
	ON_COMMAND(ID_DEMO_DELETE, &CUniversalDemoView::OnDelete)
	ON_COMMAND(ID_DEMO_SNOWFLAKE, &CUniversalDemoView::OnSnowflake)
	ON_COMMAND(ID_RBT, &CUniversalDemoView::OnRbt)
END_MESSAGE_MAP()

// CUniversalDemoView 建構/解構

CUniversalDemoView::CUniversalDemoView() noexcept
{
	// TODO: 在此加入建構程式碼
	original = _T("");
	originPos.x = 0;
	originPos.y = 0;
	currentPos.x = 0;
	currentPos.y = 0;
	dragging = false;
	inputChange = false;
	writeToFile = false;
	enableAVL = false;
	enableTree234 = false;
	enableRBT = false;
	scale = 50;
	limitScale = 3;
	angle = 120;
	display = 0;
}

CUniversalDemoView::~CUniversalDemoView()
{
}

BOOL CUniversalDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此經由修改 CREATESTRUCT cs 
	// 達到修改視窗類別或樣式的目的

	return CView::PreCreateWindow(cs);
}

// CUniversalDemoView 繪圖

void CUniversalDemoView::OnDraw(CDC* /*pDC*/)
{
	CUniversalDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 雙緩衝繪圖前置
	CRect rc;
	GetClientRect(rc);
	int nWidth = rc.Width();
	int nHeight = rc.Height();

	CDC* pDC = GetDC();
	CDC MemDC, MemBG;
	CBitmap MemBitmap;

	MemDC.CreateCompatibleDC(pDC);
	MemBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);
	CBitmap* pOldbitmap = MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(0, 0, nWidth, nHeight, RGB(255, 255, 255));
	CWnd* pwndParent = this->GetParent();
	pwndParent->GetClientRect(&screen);


	// 繪製 finalResult 到記憶體
	CT2CA toString(original);
	string inputStr(toString);
	std::ofstream out;
	if (writeToFile) {
		out.open("out.txt");
		out << inputStr << '\n';
	}

	const int R = scale;
	const int interval = scale + scale / 10;

	if (enableAVL)
		MemDC.TextOut(0, nHeight - 20, L"AVL balance: ON");
	else MemDC.TextOut(0, nHeight - 20, L"AVL balance: OFF");
	if (enableTree234)
		MemDC.TextOut(0, nHeight - 40, L"234 Tree: ON");
	else MemDC.TextOut(0, nHeight - 40, L"234 Tree: OFF");

	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = scale * 3;
	lf.lfWeight = FW_BOLD;
	::lstrcpy(lf.lfFaceName, _T("Arial"));

	CFont font;
	font.CreatePointFontIndirect(&lf);

	CFont* pOldFont = MemDC.SelectObject(&font);
	if (display == 0)
	{
		int intervalY = interval * 3 / 2;
		CPen red(PS_SOLID, 0, RGB(255, 0, 0));
		CPen black(PS_SOLID, 0, RGB(0, 0, 0));
		for (int i = 0; i < finalData.size(); i++)
		{
			if (enableTree234 && finalData[i].mark)
				MemDC.SelectObject(&red);
			else MemDC.SelectObject(&black);
			if (finalData[i].val > 0) {
				MemDC.MoveTo(currentPos.x + i * interval + R / 2, currentPos.y + abs(finalData[i].val) * intervalY + R / 2);
				MemDC.LineTo(currentPos.x + finalData[i].parent * interval + R / 2, currentPos.y + (abs(finalData[i].val) - 1) * intervalY + R / 2);
			}
			else if (finalData[i].val < 0) {
				MemDC.MoveTo(currentPos.x + i * interval + R / 2, currentPos.y + abs(finalData[i].val) * intervalY + R / 2);
				MemDC.LineTo(currentPos.x + finalData[i].parent * interval + R / 2, currentPos.y + (abs(finalData[i].val) - 1) * intervalY + R / 2);
			}
		}
		MemDC.SelectObject(&black);
		CBrush unmark, mark;
		unmark.CreateSolidBrush(RGB(255, 255, 255));
		mark.CreateSolidBrush(RGB(128, 255, 255));
		for (int i = 0; i < finalData.size(); i++)
		{
			if (finalData[i].mark && !enableTree234)
			{
				MemDC.SelectObject(&mark);
				MemDC.SetTextColor(RGB(0, 0, 128));
				MemDC.SetBkColor(RGB(128, 255, 255));
			}
			else {
				MemDC.SelectObject(&unmark);
				MemDC.SetTextColor(RGB(0, 0, 0));
				MemDC.SetBkColor(RGB(255, 255, 255));
			}
			MemDC.Ellipse(currentPos.x + i * interval, currentPos.y + abs(finalData[i].val) * intervalY, currentPos.x + i * interval + R + (finalData[i].str.length()-1) * R / 4, currentPos.y + abs(finalData[i].val) * intervalY + R);
			CString cs(finalData[i].str.c_str());
			MemDC.TextOut(currentPos.x + i * interval + R / 4 + (finalData[i].str.length() - 1) * R / 20, currentPos.y + abs(finalData[i].val) * intervalY + R / 4, cs);
		}
	}
	else if (display == 2)
		fractalTree(&MemDC, 0, CPoint(currentPos.x + nWidth / 2.0, currentPos.y + nHeight / 2.0), -90, 1);
	else if (display == 3)
		carpet(&MemDC, 0, currentPos.x + nWidth / 2.0, currentPos.y + nHeight / 2.0, scale);
	else if (display == 4)
	{
		snowflake(&MemDC, limitScale, currentPos.x+15* scale, currentPos.y+ 2 * scale, currentPos.x + 2 * scale, currentPos.y + 28 * scale);
		snowflake(&MemDC, limitScale, currentPos.x + 28 * scale, currentPos.y + 28 * scale, currentPos.x + 15 * scale, currentPos.y + 2 * scale);
		snowflake(&MemDC, limitScale, currentPos.x + 2 * scale, currentPos.y + 28 * scale, currentPos.x + 28 * scale, currentPos.y + 28 * scale);
	}

		

	// 繪圖
	pDC->BitBlt(0, 0, nWidth, nHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldbitmap);
	MemBitmap.DeleteObject();
	ReleaseDC(pDC);

	if (query.length()) {
		finalResult.erase(finalResult.begin());
		currentPos.y += 20;
		originPos.y += 20;
	}
	else query = "";

	if (writeToFile) {
		out.close();
		writeToFile = false;
	}
}

// CUniversalDemoView 列印
BOOL CUniversalDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 預設的準備列印程式碼
	return DoPreparePrinting(pInfo);
}
void CUniversalDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印前額外的初始設定
}
void CUniversalDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印後的清除程式碼
}

// CUniversalDemoView 診斷
#ifdef _DEBUG
void CUniversalDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CUniversalDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUniversalDemoDoc* CUniversalDemoView::GetDocument() const // 內嵌非偵錯版本
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUniversalDemoDoc)));
	return (CUniversalDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CUniversalDemoView 訊息處理常式
vector<string> CUniversalDemoView::multiSplit(string str, char* delim)
{
	vector<string> result;
	result.push_back(str);
	for (int i = 0; i < sizeof(delim); i++)
	{
		vector<string> tmp;
		for (int j = 0; j < result.size(); j++)
		{
			stringstream ss(result[j]);
			for (string token; getline(ss, token, delim[i]);)
				if (token.length())
					tmp.push_back(token);
		}
		result = tmp;
	}
	return result;
}
void CUniversalDemoView::printData(vector<SortFmt> sortData)
{
	string display;
	size_t LEN = sortData.size();
	for (int i = 0; i < LEN; i++)
		display += sortData[i].str + (i == LEN - 1 ? "" : sortDelim);
	finalResult.push_back(display);
}
vector<SortFmt> CUniversalDemoView::initialize(CString input, string delimStr, int mode)
{
	// reset
	finalResult.clear();
	query = "";
	originPos = currentPos;
	inputChange = false;
	CT2CA toString(original = input);
	string inputStr(toString);
	vector<string> multinput;
	char lineDelim[] = "\n";
	multinput = multiSplit(inputStr, lineDelim);
	char delim[] = " ,;\n";
	sortDelim = delimStr;
	vector<SortFmt> sortData;
	if (multinput.size() == 1) {
		splited = multiSplit(inputStr, delim);

		for (string i : splited)
			if (!mode)
				sortData.push_back({ (int)i[0], i});
			else if (mode == 1)
				sortData.push_back({ (i[0] - 'A') * 1000 + (200 - std::stoll(i.substr(1, 3))) ,i });
	}
	else if (multinput.size() == 2)
	{
		int weight[26] = { 0 };
		bool isPreorder = mode & 1;
		if (!isPreorder) reverse(multinput[0].begin(), multinput[0].end());
		vector<string> insertorder = multiSplit(multinput[0], delim);
		vector<string> inorder = multiSplit(multinput[1], delim);
		for (int i = 0; i < inorder.size(); i++)
			weight[inorder[i][0] - 'A'] = i;
		for (int i = 0; i < insertorder.size(); i++)
			sortData.push_back({ weight[insertorder[i][0] - 'A'], insertorder[i] });
	}

	return sortData;
}
BOOL CUniversalDemoView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	if (nFlags == MK_CONTROL && zDelta > 0 && scale < INT32_MAX)
	{
		scale = scale + scale / 10;
		Invalidate();
	}
	else if (nFlags == MK_CONTROL && zDelta < 0)
	{
		if(scale > 10)
			scale = scale - scale / 10;
		else scale = 10;
		Invalidate();
	}
	else if (zDelta > 0)
	{
		if (angle < 360)
			angle += 0.02;
		else angle = 0;
		Invalidate();
	}
	else if (zDelta < 0)
	{
		if (angle >=0)
			angle -= 0.02;
		else angle = 359;
		Invalidate();
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
BOOL CUniversalDemoView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	return FALSE;
}
void CUniversalDemoView::OnLButtonDown(UINT nFlags, CPoint point)
{
	BeginPos = point;
	dragging = true;
	CView::OnLButtonDown(nFlags, point);
}
void CUniversalDemoView::OnLButtonUp(UINT nFlags, CPoint point)
{
	dragging = false;
	originPos = currentPos;
	CView::OnLButtonUp(nFlags, point);
}
void CUniversalDemoView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (dragging) {
		currentPos.x = originPos.x + point.x - BeginPos.x;
		currentPos.y = originPos.y + point.y - BeginPos.y;
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}
void CUniversalDemoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//if ((nChar == 'F' || nChar == 'f') && VK_CONTROL);
		//OnQuery();
	if ((nChar == 'X' || nChar == 'x') && VK_CONTROL)
		if (limitScale < INT32_MAX)
			limitScale += 1;
	if ((nChar == 'Z' || nChar == 'z') && VK_CONTROL)
		if (limitScale > 0)
			limitScale -= 1;
	if ((nChar == 'R' || nChar == 'r') && VK_CONTROL)
	{
		currentPos.x = 0;
		currentPos.y = 0;
	}
	if ((nChar == 'F' || nChar == 'f') && VK_CONTROL)
		OnSearch();
	if ((nChar == 'A' || nChar == 'a') && VK_CONTROL)
		OnInsert();
	
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CUniversalDemoView::OnWrite()
{
	writeToFile = true;
	Invalidate();
}
int CUniversalDemoView::bits(int x, int shift, int bit)
{
	//把 x 的值從第 bit+1 的 bit 開始提 shift 個所得的值
	int mask = 0xffffffff >> (32 - bit) << shift;
	bitset<32> in(x);
	bitset<32> result((x & mask) >> shift);
	return result.to_ulong();
}
void CUniversalDemoView::fractalTree(CDC* MemDC, int layer, CPoint pos, double angles, double cum)
{
	if (layer > limitScale) return;

	for (int i = -1; i <= 1; i++)
	{
		CPoint dest = CPoint(pos.x + cos(angle * i + angles) * scale / cum, pos.y + sin(angle * i + angles) * scale / cum);
		(*MemDC).MoveTo(pos.x, pos.y);
		(*MemDC).LineTo(dest.x, dest.y);
		fractalTree(MemDC, layer + 1, dest, angles + angle * i, cum*2);
	}
}
void CUniversalDemoView::carpet(CDC* MemDC, int layer, double x, double y, double div)
{
	if (layer > limitScale) return;
	CBrush black;
	black.CreateSolidBrush(RGB(0, 0, 0));
	(*MemDC).SelectObject(&black);
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			if (i || j) {
				(*MemDC).Rectangle(x + i * div * 3, y + j * div * 3, x + i * div * 3 + div, y + j * div * 3 + div);
				carpet(MemDC, layer + 1, x + i * div * 3 + div / 3, y + j * div * 3 + div / 3, div / 3);
			}
			else
				(*MemDC).Rectangle(x - div, y - div, x + div * 2, y + div * 2);
}
void CUniversalDemoView::snowflake(CDC* MemDC, int iter, int x,int y, int x5, int y5)
{
	if (!iter) {
		(*MemDC).MoveTo(x,y);
		(*MemDC).LineTo(x5,y5);
	}
	else {
		int dx = (x5 - x) / 3;
		int dy = (y5 - y) / 3;
		int x2 = x + dx;
		int y2 = y + dy;
		int x3 = ((x + x5)/2.0 + sqrt(3) * (y - y5) / 6);
		int y3 = ((y + y5)/2.0 + sqrt(3) * (x5 - x) / 6);
		int x4 = 2 * dx + x;
		int y4 = 2 * dy + y;
		snowflake(MemDC, iter - 1, x,y, x2, y2);
		snowflake(MemDC, iter - 1, x2, y2, x3, y3);
		snowflake(MemDC, iter - 1, x3, y3, x4, y4);
		snowflake(MemDC, iter - 1, x4, y4, x5, y5);
	}
}


void CUniversalDemoView::preorderToTree()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;
	display = 0;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",", 1);
	tree = BST();
	tree.enableAVL = enableAVL;
	tree.buildTree(sortData);
	tree.preLocateNodes();
	finalData = tree.result;
	Invalidate();
}
void CUniversalDemoView::postorderToTree()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;
	display = 0;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",", 0);
	tree = BST();
	tree.enableAVL = enableAVL;
	tree.buildTree(sortData);
	tree.preLocateNodes();
	finalData = tree.result;
	Invalidate();
}
void CUniversalDemoView::OnOrder()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",", 0);
	if (enableTree234) {
		display = 0;
		T234 = tree234();
		T234.buildTree(sortData);
		if (enableRBT)
			T234.displayAsRBT();
		else T234.preLocateNodes();
		finalData = T234.result;
	}
	else {
		display = 0;
		tree = BST();
		tree.enableAVL = enableAVL;
		tree.buildTree(sortData);
		tree.preLocateNodes();
		finalData = tree.result;
	}
	Invalidate();
}
void CUniversalDemoView::OnfractalTree()
{
	scale = 50;
	limitScale = 3;
	display = 2;
	Invalidate();
}
void CUniversalDemoView::OnCarpet()
{
	scale = 50;
	limitScale = 3;
	display = 3;
	Invalidate();
}
void CUniversalDemoView::OnSnowflake()
{
	scale = 50;
	limitScale = 3;
	display = 4;
	Invalidate();
}
void CUniversalDemoView::OnInsert()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",", 0);
	if (enableTree234) {
		T234.result.clear();
		display = 0;
		T234.buildTree(sortData);
		if(enableRBT)
			T234.displayAsRBT();
		else T234.preLocateNodes();
		finalData = T234.result;
	}
	else {
		tree.result.clear();
		display = 0;
		tree.enableAVL = enableAVL;
		tree.buildTree(sortData);
		tree.preLocateNodes();
		finalData = tree.result;
	}
	Invalidate();
}
void CUniversalDemoView::OnSearch()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",", 0);
	if (enableTree234) {
		T234.result.clear();
		display = 1;
		T234.preLocateNodes();
		finalData = T234.result;
	}
	else {
		tree.result.clear();
		display = 0;
		for (SortFmt item : sortData)
			tree.search(item.str[0]);
		tree.preLocateNodes();
		finalData = tree.result;
	}
	Invalidate();
}
void CUniversalDemoView::OnDelete()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",", 0);
	if (enableTree234) {
		//
	}
	else {
		tree.result.clear();
		display = 0;
		for (SortFmt item : sortData)
			tree.remove(item.str[0]);
		tree.preLocateNodes();
		finalData = tree.result;
	}
	Invalidate();
}

void CUniversalDemoView::OnAvl()
{
	// TODO: 在此加入您的命令處理常式程式碼
	enableAVL = !enableAVL;
	Invalidate();
}
void CUniversalDemoView::On234tree()
{
	// TODO: 在此加入您的命令處理常式程式碼
	enableTree234 = !enableTree234;
	Invalidate();
}
void CUniversalDemoView::OnRbt()
{
	if (!enableTree234) return;
	enableRBT = !enableRBT;
	T234.result.clear();
	if (enableRBT)
		T234.displayAsRBT();
	else T234.preLocateNodes();
	finalData = T234.result;
	Invalidate();
}
