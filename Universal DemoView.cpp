﻿
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
#include<map>
#include<sstream>
#include<fstream>
#include<bitset>
#include<algorithm>

using std::vector;
using std::string;
using std::stringstream;
using std::bitset;
#define INF 10000

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
	ON_COMMAND(ID_234Tree, &CUniversalDemoView::On234tree)
	ON_COMMAND(ID_DEMO_FRACTALTREE, &CUniversalDemoView::OnfractalTree)
	ON_COMMAND(ID_DEMO_SIERPINSKICARPET, &CUniversalDemoView::OnCarpet)
	ON_COMMAND(ID_DEMO_INSERT, &CUniversalDemoView::OnInsert)
	ON_COMMAND(ID_DEMO_SEARCH, &CUniversalDemoView::OnSearch)
	ON_COMMAND(ID_DEMO_DELETE, &CUniversalDemoView::OnDelete)
	ON_COMMAND(ID_DEMO_SNOWFLAKE, &CUniversalDemoView::OnSnowflake)
	ON_COMMAND(ID_RBT, &CUniversalDemoView::OnRbt)
	ON_COMMAND(ID_CUTTREE, &CUniversalDemoView::OnCuttree)
	ON_COMMAND(ID_COVER, &CUniversalDemoView::OnCover)
	ON_COMMAND(ID_STEPBYSTEP, &CUniversalDemoView::OnStepbystep)
	ON_COMMAND(ID_REFRESH, &CUniversalDemoView::OnRefresh)
	ON_COMMAND(ID_DP_BACKPACK, &CUniversalDemoView::backpack_DP)
	ON_COMMAND(ID_DP_MATRIXCHAINPRODUCTION, &CUniversalDemoView::Matrix_Chain_Production)
	ON_COMMAND(ID_GRAPH_PRIM, &CUniversalDemoView::primAlgorithm)
	ON_COMMAND(ID_DP_OPTIMALBST, &CUniversalDemoView::optimalBST)
	ON_COMMAND(ID_GRAPH_GRAHAMSCAN, &CUniversalDemoView::OnGraphGrahamscan)
	ON_COMMAND(ID_GRAPH_KRUSKAL, &CUniversalDemoView::OnGraphKruskal)
	ON_COMMAND(ID_GRAPH_DIJKSTRA, &CUniversalDemoView::OnGraphDijkstra)
	//ON_COMMAND(ID_GRAPH_FLOYDWARSHALL, &CUniversalDemoView::OnGraphFloydwarshall)
	ON_COMMAND(ID_GRAPH_VORONOIDIAGRAM, &CUniversalDemoView::OnGraphVoronoidiagram)
	ON_COMMAND(ID_SORT_HEAPSORT, &CUniversalDemoView::OnSortHeapsort)
	ON_COMMAND(ID_MIRROR, &CUniversalDemoView::OnMirror)
	ON_COMMAND(ID_FLOYDWARSHALL_DIAGRAPH, &CUniversalDemoView::OnFloydwarshallDiagraph)
	ON_COMMAND(ID_FLOYDWARSHALL_GRAPH, &CUniversalDemoView::OnFloydwarshallGraph)
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
	stepByStep = false;
	cover = true;
	scale = 50;
	limitScale = 3;
	angle = 120;
	display = 0;
	tree = BST();
	T234 = tree234();
	graph= Graph();
	mirror = false;
	digraph = false;
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
	int interval = scale + scale / 10;

	if (enableTree234 && enableRBT)
		MemDC.TextOut(0, nHeight - 20, L"Red-black Tree from 234-tree");
	else if (enableTree234)
		MemDC.TextOut(0, nHeight - 20, L"234 Tree");
	if (enableAVL&&!enableTree234)
		MemDC.TextOut(0, nHeight - 20, L"AVL balance");
	if (stepByStep)
		MemDC.TextOut(0, nHeight - 40, L"Step by step display");
	if (!cover)
		MemDC.TextOut(0, nHeight - 60, L"uncover");

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
		if (enableTree234 && !enableRBT) 
			interval += scale * 2;
		
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
			MemDC.Ellipse(currentPos.x + i * interval - (finalData[i].str.length() - 1) * R / 10, currentPos.y + abs(finalData[i].val) * intervalY, currentPos.x + i * interval + R + (finalData[i].str.length()-1) * R / 5, currentPos.y + abs(finalData[i].val) * intervalY + R);
			CString cs(finalData[i].str.c_str());
			MemDC.TextOut(currentPos.x + i * interval + R / 4, currentPos.y + abs(finalData[i].val) * intervalY + R / 4, cs);
		}
		int height = extraText.size()+12;
		for (int i = 0; i < extraText.size(); i++)
		{
			CString output(extraText[i].c_str());
			MemDC.TextOut(currentPos.x, currentPos.y + (i + 1) * scale * 4 / 5-(height-i)* scale/4, output);
		}
	}
	else if (display == 2)
		fractalTree(&MemDC, 0, CPoint(currentPos.x + nWidth / 2.0, currentPos.y + nHeight / 2.0), 30, 1);
	else if (display == 3)
		carpet(&MemDC, 0, currentPos.x + nWidth / 2.0, currentPos.y + nHeight / 2.0, scale);
	else if (display == 4)
	{
		snowflake(&MemDC, limitScale, currentPos.x+15/10* scale, currentPos.y+ 2 / 10 * scale, currentPos.x + 2 / 10 * scale, currentPos.y + 28 / 10 * scale);
		snowflake(&MemDC, limitScale, currentPos.x + 28 / 10 * scale, currentPos.y + 28 / 10 * scale, currentPos.x + 15 / 10 * scale, currentPos.y + 2 / 10 * scale);
		snowflake(&MemDC, limitScale, currentPos.x + 2 / 10 * scale, currentPos.y + 28 / 10 * scale, currentPos.x + 28 / 10 * scale, currentPos.y + 28 / 10 * scale);
		string limit = "limite: ";
		limit += ((char)(limitScale)+'0');
		CString limitcs(limit.c_str());
		MemDC.TextOut(nWidth - 300, nHeight - 40, limitcs);
	}
	else if (display == 5)
	{
		for (int i=0;i< finalResult.size();i++)
		{
			CString output(finalResult[i].c_str());
			MemDC.TextOut(currentPos.x, currentPos.y + (i + 1) * scale*4/5, output);
		}
	}
	else if (display == 6)
	{
		const int inv = mirror ? -1 : 1;
		int num = graph.vertexs.size();
		int range = scale*num/2;
		for (int i = 0; i < extraText.size(); i++)
		{
			CString output(extraText[i].c_str());
			MemDC.TextOut(currentPos.x-30* scale, currentPos.y + (i + 1) * scale * 4 / 5, output);
		}
		for (int r = 0; r < graph.history.size(); r++)
		{
			double detla = (range * 2 + scale) * r;
			//const flip=()
			CPen red(PS_SOLID, 3, RGB(255, 0, 0));
			CPen black(PS_SOLID, 3, RGB(0, 0, 0));
			vector<Edges> edges = graph.history[r].second;
			vector<Vertexs> vertexs = graph.history[r].first;
			for (int i = 0; i < edges.size(); i++)
			{
				double modfi = 0;
				if(digraph)
					modfi = (edges[i].src > edges[i].dst ? R / 5.0 : -R / 5.0);
				if (edges[i].mark)
					MemDC.SelectObject(&red);
				else MemDC.SelectObject(&black);
				double sX = vertexs[graph.VTindex(edges[i].src)].x;
				double sY = vertexs[graph.VTindex(edges[i].src)].y;
				double dX = vertexs[graph.VTindex(edges[i].dst)].x;
				double dY = vertexs[graph.VTindex(edges[i].dst)].y;

				double srcX = currentPos.x + sX * range;
				double srcY = currentPos.y + sY * inv * range + detla + modfi;
				double dstX = currentPos.x + dX * range;
				double dstY = currentPos.y + dY * inv * range + detla + modfi;
				if(edges[i].val>=0)
				{
					if (digraph)
						modfi = (edges[i].src > edges[i].dst ? 0 : -R);
					CString cs(std::to_string(edges[i].val).c_str());
					MemDC.TextOut((srcX + dstX) / 2, (srcY + dstY) / 2+ modfi, cs);
				}
				MemDC.MoveTo(srcX, srcY);
				MemDC.LineTo(dstX, dstY);
				if(digraph)
				{
					double maxAxis = max(abs(sX - dX), abs(sY - dY));
					double arrowX = 50 * (sX - dX) / maxAxis;
					double arrowY = 50 * (sY - dY) / maxAxis;
					double len = sqrt(arrowX * arrowX + arrowY * arrowY);
					arrowX = arrowX / len * R / 2;
					arrowY = arrowY / len * R / 2;
					dstX = dstX + arrowX;
					dstY = dstY + arrowY;
					MemDC.MoveTo(dstX, dstY);
					MemDC.LineTo(dstX + arrowX * cos(10.0 * DEG) - arrowY * sin(10.0 * DEG), dstY + arrowX * sin(10.0 * DEG) + arrowY * cos(10.0 * DEG));
					MemDC.MoveTo(dstX, dstY);
					MemDC.LineTo(dstX + arrowX * cos(-10.0 * DEG) - arrowY * sin(-10.0 * DEG), dstY + arrowX * sin(-10.0 * DEG) + arrowY * cos(-10.0 * DEG));
				}
			}

			MemDC.SelectObject(&black);
			CBrush unmark, mark;
			unmark.CreateSolidBrush(RGB(255, 255, 255));
			mark.CreateSolidBrush(RGB(128, 255, 255));
			for (int i = 0; i < vertexs.size(); i++)
			{
				if (vertexs[i].mark)
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
				double Vx = currentPos.x + vertexs[i].x * range;
				double Vy = currentPos.y + vertexs[i].y * inv * range + detla;
				MemDC.Ellipse(Vx - R/2.0, Vy - R / 2.0, Vx + R / 2.0, Vy + R / 2.0);
				CString cs(vertexs[i].val);
				MemDC.TextOut(Vx - R/4.0, Vy - R / 4.0, cs);
				if(!vertexs[i].str.empty())
				{
					MemDC.SelectObject(&unmark);
					MemDC.SetTextColor(RGB(150, 150, 150));
					MemDC.SetBkColor(RGB(255, 255, 255));
					CString cstr(vertexs[i].str.c_str());
					MemDC.TextOut(Vx - R / 4.0, Vy - R / 4.0 + scale * 4 / 5, cstr);
				}
			}
			MemDC.SelectObject(&unmark);
			MemDC.SetTextColor(RGB(0, 0, 0));
			MemDC.SetBkColor(RGB(255, 255, 255));
		}
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
			if(i[0] == '-' || (i[0] >= '0' && i[0] <= '9'))
				sortData.push_back({ stoll(i), i});
			else
				sortData.push_back({ (int)i[0], i});
	}
	else if (multinput.size() >= 2)
	{
		int weight[10000] = { 0 };
		bool isPreorder = mode & 1;
		if (!isPreorder) reverse(multinput[0].begin(), multinput[0].end());
		vector<string> insertorder = multiSplit(multinput[0], delim);
		vector<string> inorder = multiSplit(multinput[1], delim);
		if (inorder[0][0] == '-' || (inorder[0][0] >= '0' && inorder[0][0] <= '9')) {
			for (int i = 0; i < inorder.size(); i++)
				weight[stoi(inorder[i])] = i;
			for (int i = 0; i < insertorder.size(); i++)
				sortData.push_back({ weight[stoi(insertorder[i])], insertorder[i] });
		}
		else {
			for (int i = 0; i < inorder.size(); i++)
				weight[inorder[i][0]] = i;
			for (int i = 0; i < insertorder.size(); i++)
				sortData.push_back({ weight[inorder[i][0]], insertorder[i] });
		}
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
		return OnRefresh();
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
	if(!layer)
		carpet(MemDC, layer + 1, x,y,div);

	if (layer && layer <= 1) {
		(*MemDC).Rectangle(x - div, y - div, x + div * 2, y + div * 2);
		carpet(MemDC, layer + 1, x, y, div);
	}
	else if(layer > 1)
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			if (i || j) {
				(*MemDC).Rectangle(x + i * div * 3, y + j * div * 3, x + i * div * 3 + div, y + j * div * 3 + div);
				carpet(MemDC, layer + 1, x + i * div * 3 + div / 3, y + j * div * 3 + div / 3, div / 3);
			}
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
	inputBox.InputStr = L"50,40,1,20,45,60,55,90,80,99\\n1,20,40,45,50,55,60,80,90,99";
	if (inputBox.DoModal() != IDOK) return;
	display = 0;
	preorder.clear();
	inorder.clear();
	postorder.clear();

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",", 1);
	tree = BST();
	if (enableTree234) {
		T234.result.clear();
		display = 0;
		T234.buildTree(sortData);
		if (enableRBT)
			T234.displayAsRBT();
		else T234.preLocateNodes();
		finalData = T234.result;
	}
	else {
		tree.result.clear();
		display = 0;
		tree.buildTree(sortData);
		tree.preLocateNodes();
		finalData = tree.result;
		tree.result.clear();
		tree.preOrder();
		preorder = tree.result;
		tree.result.clear();
		tree.inOrder();
		inorder = tree.result;
		tree.result.clear();
		tree.postOrder();
		postorder = tree.result;
	}
	extraText.clear();
	displayTreeOrder();
	Invalidate();
}
void CUniversalDemoView::postorderToTree()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	inputBox.InputStr = L"A,B,C,D,E,F,G,H,I,J\\nB,A,D,C,F,H,J,I,G,E";
	if (inputBox.DoModal() != IDOK) return;
	display = 0;
	preorder.clear();
	inorder.clear();
	postorder.clear();

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",", 0);
	if (enableTree234) {
		T234.result.clear();
		display = 0;
		T234.buildTree(sortData);
		if (enableRBT)
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
		tree.result.clear();
		tree.preOrder();
		preorder = tree.result;
		tree.result.clear();
		tree.inOrder();
		inorder = tree.result;
		tree.result.clear();
		tree.postOrder();
		postorder = tree.result;
	}
	extraText.clear();
	displayTreeOrder();
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
void CUniversalDemoView::displayTreeOrder()
{
	// TODO: 請在此新增您的實作程式碼.
	string preorderStr = "Preorder: ";
	string inorderStr = "Inorder: ";
	string postorderStr = "Postorder: ";
	for (int i = 0; i < preorder.size(); i++)
		preorderStr += preorder[i].str + (i + 1 == preorder.size() ? "" : " -> ");
	for (int i = 0; i < inorder.size(); i++)
		inorderStr += inorder[i].str + (i + 1 == inorder.size() ? "" : " -> ");
	for (int i = 0; i < postorder.size(); i++)
		postorderStr += postorder[i].str + (i + 1 == postorder.size() ? "" : " -> ");
	extraText.push_back(preorderStr);
	extraText.push_back(inorderStr);
	extraText.push_back(postorderStr);
}
void CUniversalDemoView::insertBST(vector<SortFmt>& sortData)
{
	// TODO: 請在此新增您的實作程式碼.
	preorder.clear();
	inorder.clear();
	postorder.clear();
	if (enableTree234) {
		if (cover)
			T234.result.clear();
		display = 0;
		T234.buildTree(sortData);
		if (enableRBT)
			T234.displayAsRBT();
		else T234.preLocateNodes();
		finalData = T234.result;
	}
	else {
		if (cover)
			tree.result.clear();
		display = 0;
		tree.buildTree(sortData);
		tree.preLocateNodes();
		finalData = tree.result;
		tree.result.clear();
		tree.preOrder();
		preorder = tree.result;
		tree.result.clear();
		tree.inOrder();
		inorder = tree.result;
		tree.result.clear();
		tree.postOrder();
		postorder = tree.result;
	}
}
void CUniversalDemoView::OnInsert()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;
	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",", 0);
	insertBST(sortData);
	extraText.clear();
	if(!enableTree234)
		displayTreeOrder();
	Invalidate();
}
void CUniversalDemoView::OnSearch()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	if(display == 6)
		inputBox.title = L"Graph 查找";
	else if(display == 0)
		inputBox.title = L"binary tree 查找";
	else
		return;

	if (inputBox.DoModal() != IDOK) return;
	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",", 0);
	if (display==6 && sortData.size() == 2 && bestResult.size()) {
		Graph queryGraph = Graph(graph);
		int src = queryGraph.VTindex(sortData[0].str[0]);
		int dst = queryGraph.VTindex(sortData[1].str[0]);
		if (src == -1 || dst == -1)
			return;
		graph.history.clear();
		//if (restoreFWPath(queryGraph, src, dst) == -1)
		//	return graph.record(2);
		string pathStr = "";
		pathStr += queryGraph.vertexs[src].val;
		vector<char> pathC;
		int cost = 0;
		for (int k;true;)
		{
			k = bestResult[src][dst];
			if (k >= INF)
			{
				graph.record(2);
				return Invalidate();
			}
			queryGraph.vertexs[dst].mark = true;
			char kC = queryGraph.vertexs[k].val;
			char dstC = queryGraph.vertexs[dst].val;
			int eg = queryGraph.EGindex(kC, dstC);
			cost += queryGraph.edges[eg].val;
			queryGraph.edges[eg].mark = true;
			pathC.push_back(dstC);
			dst = k;
			if (src == k)
				break;
		}
		int pathN = pathC.size();
		for (int i = 0; i < pathN; i++)
		{
			pathStr += "->";
			pathStr += pathC.back();
			pathC.pop_back();
		}
		extraText.push_back(pathStr+"  Cost: "+std::to_string(cost));
		queryGraph.vertexs[src].mark = true;
		graph.record(2, queryGraph.vertexs,queryGraph.edges);
	}
	else if(display == 0){
		if (cover)
			tree.result.clear();
		display = 0;
		for (SortFmt item : sortData)
			tree.search(item.val);
		tree.preLocateNodes();
		finalData = tree.result;
		extraText.clear();
		displayTreeOrder();
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
		if (cover)
			tree.result.clear();
		display = 0;
		for (SortFmt item : sortData)
			tree.remove(item.val);
		tree.preLocateNodes();
		finalData = tree.result;
		extraText.clear();
		displayTreeOrder();
	}
	
	Invalidate();
}

void CUniversalDemoView::OnAvl()
{
	// TODO: 在此加入您的命令處理常式程式碼
	enableAVL = !enableAVL;
	tree.enableAVL = enableAVL;
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
	T234.RBT = enableRBT;
	T234.result.clear();
	if (enableRBT)
		T234.displayAsRBT();
	else T234.preLocateNodes();
	finalData = T234.result;
	Invalidate();
}
void CUniversalDemoView::OnCuttree()
{
	tree.deleteBST();
	T234.deleteTree();
	preorder.clear();
	inorder.clear();
	postorder.clear();
	extraText.clear();
	tree = BST();
	T234 = tree234();
	tree.enableAVL = enableAVL;
	tree.stepByStep = stepByStep;
	T234.stepByStep = stepByStep;
	finalData = tree.result;
	Invalidate();
}
void CUniversalDemoView::OnCover()
{
	cover = !cover;
	Invalidate();
}
void CUniversalDemoView::OnStepbystep()
{
	stepByStep = !stepByStep;
	tree.stepByStep = stepByStep;
	T234.stepByStep = stepByStep;
	Invalidate();
}
void CUniversalDemoView::OnRefresh()
{
	currentPos.x = 0;
	currentPos.y = 0;
	originPos = currentPos;
	scale = 50;
	limitScale = 3;
	preorder.clear();
	inorder.clear();
	postorder.clear();
	if (display == 0) 
		if (enableTree234) {
			T234.result.clear();
			if (enableRBT)
				T234.displayAsRBT();
			else T234.preLocateNodes();
			finalData = T234.result;
		}
		else {
			tree.result.clear();
			tree.preLocateNodes();
			finalData = tree.result;
			tree.result.clear();
			tree.preOrder();
			preorder = tree.result;
			tree.result.clear();
			tree.inOrder();
			inorder = tree.result;
			tree.result.clear();
			tree.postOrder();
			postorder = tree.result;
		}
	Invalidate();
}
void CUniversalDemoView::OnMirror()
{
	// TODO: 在此加入您的命令處理常式程式碼
	mirror = !mirror;
	Invalidate();
}


string CUniversalDemoView::matrixStrDivide(vector<vector<int>>& best, string letters, int i, int j)
{
	if (i==j)
		return letters.substr(i-1,1);
	return "(" + matrixStrDivide(best, letters, i, best[i][j] - 1) + matrixStrDivide(best, letters, best[i][j], j) + ")";
}
void CUniversalDemoView::backpack_DP()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	inputBox.InputStr = L"786";
	if (inputBox.DoModal() != IDOK) return;
	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",", 0);
	if (!sortData.size())
		return;
	if (cover)
		finalResult.clear();
	display = 5;
	finalResult.push_back("輸入：");
	finalResult.push_back("W="+sortData[0].str);
	finalResult.push_back("輸出：");
	const int capacity = sortData[0].val;
	if (capacity < 14 || capacity>999)
		return;
	Items item[4] = {
		{'A', 3 ,2},
		{'B', 10 ,11},
		{'C', 25 ,27},
		{'D', 27 ,30}
	};
	int result[4] = { 0 };
	vector<int> cost(capacity + 1, 0);
	vector<int> best(capacity + 1, -1);
	for (int j = 0; j < 4; j++)
		for (int i = 1; i <= capacity; i++) {
			if (i - item[j].size >= 0) {
				int newVal = cost[i - item[j].size] + item[j].val;
				if (cost[i] < newVal) {
					cost[i] = newVal;
					best[i] = j;
				}
			}
		}
	finalResult.push_back("今天可販售的最高金額"+ std::to_string(cost[capacity])+ "元，");
	finalResult.push_back("農作物：");
	for (int i = best.size() - 1; i > 0 && ~best[i]; i -= item[best[i]].size)
		result[best[i]]++;
	for (int i = 0; i < 4; i++)
		if(result[i])
		{
			string itemName = "";
			itemName += (char)(i + 'A');
			finalResult.push_back(itemName + "，數量：" + std::to_string(result[i]) + "。\n");
		}

	Invalidate();
}
void CUniversalDemoView::Matrix_Chain_Production()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	inputBox.InputStr = L"矩陣名稱：ABCD 矩陣大小：5*10,10*8,8*5,5*11";
	if (inputBox.DoModal() != IDOK) return;
	CT2CA toString(inputBox.InputStr);
	string inputStr(toString);
	vector<string> multinput;
	char lineDelim[] = " \n";
	multinput = multiSplit(inputStr, lineDelim);
	multinput[1] = inputStr.substr(multinput[0].length());
	int preStr=multinput[0].find("：");
	string letters = multinput[0].substr(preStr+2, multinput[0].length() - preStr);
	preStr = multinput[1].find("：");
	string products = multinput[1].substr(preStr+2, multinput[1].length() - preStr);
	char delim[] = " ,*";
	vector<string> productsNum = multiSplit(products, delim);

	
	if (!productsNum.size())
		return;
	if (cover)
		finalResult.clear();
	display = 5;

	vector<int> matrixs(1,0);
	for (int i = 0; i < productsNum.size(); i+=2)
		matrixs.push_back(stoi(productsNum[i]));
	const int N = matrixs.size()-1;
	matrixs.push_back(stoi(productsNum[productsNum.size() - 1]));

	finalResult.push_back("Input："+inputStr);

	vector<vector<int>> cost(N + 1, vector<int>(N + 1, 0));
	vector<vector<int>> best(N + 1, vector<int>(N + 1, 0));
	for (int i = 1; i <= N; i++)
		for (int j = i + 1; j <= N; j++)
			cost[i][j] = INT32_MAX;
	for (int i = 1; i <= N; i++)
		cost[i][i] = 0;
	for (int j = 1; j <= N - 1; j++) // layer
		for (int i = 1; i <= N - j; i++) { // nums of products
			int u = i + j;
			for (int k = i + 1; k <= u; k++) { // index of layer
				int tmp =	cost[i][k - 1] +
							cost[k][u] +
							matrixs[i] * matrixs[k] * matrixs[u + 1];
				if (tmp < cost[i][u]) {
					cost[i][u] = tmp;
					best[i][u] = k;
				}
			}
		}
	finalResult.push_back("Output："+matrixStrDivide(best, letters, 1, N));
	finalResult.push_back("所需乘法次數：" + std::to_string(cost[1][N]) + " 次");
	Invalidate();
}
void CUniversalDemoView::primAlgorithm()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	inputBox.InputStr = L"代號：ABCDEFG 邊：(A,B,9) (A,C,3) (C,D,6) (B,F,1) (C,F,2) (F,G,4) (F,E,5) (E,G,1) (D,E,1)";
	if (inputBox.DoModal() != IDOK) return;
	// init input
	CT2CA toString(inputBox.InputStr);
	string inputStr(toString);
	if (cover)
		finalResult.clear();
	display = 6;
	limitScale = 3;
	vector<string> multinput;
	char lineDelim[] = " \n";
	multinput = multiSplit(inputStr, lineDelim);
	for (int i = 0; i < multinput.size(); i++)
		finalResult.push_back(multinput[i]);
	int preStr = inputStr.find("：");
	inputStr= inputStr.substr(preStr+2);

	char spaceDelim[] = " ";
	string letters = multiSplit(inputStr, spaceDelim)[0];
	preStr = inputStr.find("：");
	inputStr = inputStr.substr(preStr+2);
	char delim[] = " ()\n\r";
	vector<string> nodes = multiSplit(inputStr, delim);
	char delimNode[] = ",";
	vector<Edges> edges;
	for (int i = 0; i < nodes.size(); i++)
	{
		vector<string> tmp = multiSplit(nodes[i], delimNode);
		edges.push_back({ tmp[0][0],tmp[1][0] ,stoi(tmp[2]),false });
	}
	// init end
	graph = Graph(letters,edges,'A');

	graph.vertexs[graph.VTindex('A')].mark = true;
	graph.record();

	int marked = 0;
	int minimum = -1,last=-2;
	while (last != minimum)
	{
		last = minimum;
		for (int i = 0; i < graph.edges.size(); i++)
			if (graph.VT(graph.edges[i].src).mark ^ graph.VT(graph.edges[i].dst).mark)
				if (minimum == -1 ||
					graph.edges[minimum].mark ||
					graph.edges[i].val < graph.edges[minimum].val)
						minimum = i;
			
		graph.vertexs[graph.VTindex(graph.edges[minimum].dst)].mark = true;
		graph.vertexs[graph.VTindex(graph.edges[minimum].src)].mark = true;
		graph.edges[minimum].mark = true;
		if(last != minimum)
			graph.record();
	}
	

	Invalidate();
}
void CUniversalDemoView::optInsertOrder(vector<vector<int>>& best, vector<int>& order, int i, int j)
{
	if (i == j) {
		order.push_back(i);
		return;
	}
	order.push_back(best[i][j]);
	if (best[i][j] - 1>=i)
		optInsertOrder(best, order, i, best[i][j] - 1);
	if (best[i][j] + 1 <= j)
		optInsertOrder(best, order, best[i][j] + 1, j);
	return;
}
void CUniversalDemoView::optimalBST()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	inputBox.InputStr = L"樹節點：ABCDEFG 頻率：4,2,3,1,5,2,1";
	if (inputBox.DoModal() != IDOK) return;
	CT2CA toString(inputBox.InputStr);
	string inputStr(toString);
	vector<string> multinput;
	char lineDelim[] = " \n";
	multinput = multiSplit(inputStr, lineDelim);
	multinput[1] = inputStr.substr(multinput[0].length());
	int preStr = multinput[0].find("：");
	string letters = multinput[0].substr(preStr + 2, multinput[0].length() - preStr);
	preStr = multinput[1].find("：");
	string products = multinput[1].substr(preStr + 2, multinput[1].length() - preStr);
	char delim[] = " ,";
	vector<string> freqStr = multiSplit(products, delim);
	if (cover)
		extraText.clear();

	vector<int> freq(1, 0);
	for (int i = 0; i < freqStr.size(); i++)
		freq.push_back(stoi(freqStr[i]));
	const int N = freqStr.size();

	extraText.push_back("Input：" + inputStr);

	vector<vector<int>> cost(N + 2, vector<int>(N + 2, 0));
	vector<vector<int>> best(N + 1, vector<int>(N + 1, 0));
	for (int i = 1; i <= N; i++)
		for (int j = i + 1; j <= N + 1; j++)
			cost[i][j] = INT_MAX;
	for (int i = 1; i <= N; i++)
		cost[i][i] = freq[i];
	for (int j = 1; j <= N - 1; j++)
		for (int i = 1; i <= N - j; i++)
		{
			// i+j
			int u = i + j;
			for (int k = i; k <= u; k++)
			{
				int tmp =	cost[i][k - 1] +
							cost[k + 1][u];
				if (tmp < cost[i][u]) {
					cost[i][u] = tmp;
					best[i][u] = k;
				}
			}
			int tmp = 0;
			for (int k = i; k <= u; k++)
				tmp += freq[k];
			cost[i][u] += tmp;
		}
	vector<int> order;
	vector<SortFmt> sortData;
	extraText.push_back("總次數：" + std::to_string(cost[1][N]));
	optInsertOrder(best, order, 1, N);
	for (int i = 0; i < order.size(); i++)
		sortData.push_back({ order[i],letters.substr(order[i] - 1,1)+" "+std::to_string(freq[order[i]])});
	insertBST(sortData);
	Invalidate();
}

void CUniversalDemoView::OnGraphGrahamscan()
{
	CInput inputBox;
	inputBox.InputStr = L"對位置：(0,3)(1,1)(2,2)(4,4)(0,0)(1,2)(3,1)(3,3)";
	if (inputBox.DoModal() != IDOK) return;
	// init input
	CT2CA toString(inputBox.InputStr);
	string inputStr(toString);
	if (cover)
		finalResult.clear();
	extraText.clear();
	display = 6;
	digraph = false;
	limitScale = 3;
	vector<string> multinput;
	char lineDelim[] = " \n";
	multinput = multiSplit(inputStr, lineDelim);
	for (int i = 0; i < multinput.size(); i++)
		finalResult.push_back(multinput[i]);
	//int preStr = inputStr.find("：");
	//inputStr = inputStr.substr(preStr + 2);

	//char spaceDelim[] = " ";
	//string letters = multiSplit(inputStr, spaceDelim)[0];
	int preStr = inputStr.find("：");
	inputStr = inputStr.substr(preStr + 2);
	char delim[] = " ()\n\r";
	vector<string> nodes = multiSplit(inputStr, delim);
	char delimNode[] = ",";
	vector<Vertexs> vertices;
	int maxVal = 0;
	const int scaleGh = 10;
	for (int i = 0; i < nodes.size(); i++)
	{
		vector<string> tmp = multiSplit(nodes[i], delimNode);
		if(tmp.size()==3)
		{
			maxVal = max(max(stoi(tmp[1]), stoi(tmp[2])), maxVal);
			vertices.push_back({ tmp[0][0],stod(tmp[1]) ,stod(tmp[2]),false });
		}
		else if (tmp.size() == 2)
		{
			maxVal = max(max(stoi(tmp[0]), stoi(tmp[1])), maxVal);
			vertices.push_back({ (char)('A' + i),stod(tmp[0]) ,stod(tmp[1]),false });
		}
		else if (tmp.size() == 1)
		{
			maxVal = max(max(stoi(nodes[i]), stoi(nodes[i+1])), maxVal);
			vertices.push_back({ (char)('A' + i),stod(nodes[i]) ,stod(nodes[i+1]),false });
			i++;
		}
	}
	// init
	graph = Graph(vertices, 'A');

	// Find the anchor point
	int ymin = graph.vertexs[0].y, min = 0;
	for (int i = 1; i < graph.vertexs.size(); i++)
	{
		int y = graph.vertexs[i].y;

		// Pick the bottom-most or choose the left
		// most point in case of tie
		if ((y < ymin) || (ymin == y && graph.vertexs[i].x < graph.vertexs[min].x))
			ymin = graph.vertexs[i].y, min = i;
	}
	std::swap(graph.vertexs[0], graph.vertexs[min]);
	graph.sortGh();
	graph.verticeMap();

	vertices.clear();
	vertices.push_back(graph.vertexs[0]);
	int m = 1; // Initialize size of modified array
	for (int i = 1; i < graph.vertexs.size(); i++)
	{
		// Keep removing i while angle of i and i+1 is same
		// with respect to p0
		for (; i < graph.vertexs.size() - 1 && graph.orientation(graph.vertexs[0], graph.vertexs[i], graph.vertexs[i + 1]) == 0; i++);
		vertices.push_back(graph.vertexs[i]);
	}
	if (vertices.size() < 3)
		return;
	vector<Vertexs> hull;
	hull.push_back(vertices[0]);
	hull.push_back(vertices[1]);
	hull.push_back(vertices[2]);
	// Process remaining n-3 points
	for (int i = 3; i < vertices.size(); i++)
	{
		// Keep removing top while the angle formed by
		// points next-to-top, top, and points[i] makes
		// a non-left turn
		while (hull.size() > 1 && graph.orientation(hull[hull.size() - 2], hull[hull.size() - 1], vertices[i]) != 2)
			hull.pop_back();
		hull.push_back(vertices[i]);
	}
	graph.verticeMap();
	graph.vertexs[graph.VTindex(hull[0].val)].mark = true;
	for (int i = 1; i < hull.size(); i++)
	{
		graph.vertexs[graph.VTindex(hull[i].val)].mark = true;
		graph.edges.push_back({ hull[i - 1].val,hull[i].val,-1 });
	}
	graph.edges.push_back({ hull.back().val,hull[0].val,-1});
	graph.record(4);
	Invalidate();
}
void CUniversalDemoView::OnGraphKruskal()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	inputBox.InputStr = L"代號：ABCDEFG 邊：(A,B,9) (A,C,3) (C,D,6) (B,F,1) (C,F,2) (F,G,4) (F,E,5) (E,G,1) (D,E,1)";
	if (inputBox.DoModal() != IDOK) return;
	// init input
	CT2CA toString(inputBox.InputStr);
	string inputStr(toString);
	if (cover)
		finalResult.clear();
	extraText.clear();
	display = 6;
	digraph = false;
	limitScale = 3;
	vector<string> multinput;
	char lineDelim[] = " \n";
	multinput = multiSplit(inputStr, lineDelim);
	for (int i = 0; i < multinput.size(); i++)
		finalResult.push_back(multinput[i]);
	int preStr = inputStr.find("：");
	inputStr = inputStr.substr(preStr + 2);

	char spaceDelim[] = " ";
	string letters = multiSplit(inputStr, spaceDelim)[0];
	preStr = inputStr.find("：");
	inputStr = inputStr.substr(preStr + 2);
	char delim[] = " ()\n\r";
	vector<string> nodes = multiSplit(inputStr, delim);
	char delimNode[] = ", ";
	vector<Edges> edges;
	for (int i = 0; i < nodes.size(); i++)
	{
		vector<string> tmp = multiSplit(nodes[i], delimNode);
		edges.push_back({ tmp[0][0],tmp[1][0] ,stoi(tmp[2]),false });
	}
	// init end
	graph = Graph(letters, edges, 'A');
	DSU un= DSU(letters.length());
	graph.sortByEdgeCost();
	int unionN = 1;
	for (int i = 0; i < graph.edges.size();i++) {
		int a= graph.VTindex(graph.edges[i].src),b= graph.VTindex(graph.edges[i].dst);
		if (un.find(a) != un.find(b))
		{
			un.unite(a, b);
			graph.vertexs[a].mark = true;
			graph.vertexs[b].mark = true;
			graph.edges[i].mark = true;
			unionN++;
			graph.record();
		}
		if (unionN >= letters.length())
			break;
	}
	Invalidate();
}
void CUniversalDemoView::OnGraphDijkstra()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	inputBox.InputStr = L"出發點： A\n 路徑：B C 5,D E 8,A B 6,C D 12";
	if (inputBox.DoModal() != IDOK) return;
	// init input
	CT2CA toString(inputBox.InputStr);
	string inputStr(toString);
	if (cover)
		finalResult.clear();
	extraText.clear();
	display = 6;
	digraph = false;
	limitScale = 3;
	vector<string> multinput;
	char lineDelim[] = " \n";
	extraText.push_back(inputStr);
	multinput = multiSplit(inputStr, lineDelim);
	//for (int i = 0; i < multinput.size(); i++)
	//	finalResult.push_back(multinput[i]);
	int preStr = inputStr.find("：");
	inputStr = inputStr.substr(preStr + 2);

	char spaceDelim[] = " ";
	string pt = multiSplit(inputStr, spaceDelim)[0];
	char startP = pt[0];
	preStr = inputStr.find("：");
	inputStr = inputStr.substr(preStr + 2);
	char delim[] = ",";
	vector<string> nodes = multiSplit(inputStr, delim);
	char delimNode[] = " ";
	string letters = "";
	for (int i = 0; i < nodes.size(); i++)
	{
		vector<string> tmp = multiSplit(nodes[i], delimNode);
		if (letters.find(tmp[0]) == -1)
			letters += tmp[0];
		if (letters.find(tmp[1]) == -1)
			letters += tmp[1];
	}
	//string nesL="";
	//for (int i = 0; i < letters.length(); i++)
	//	nesL += ('A' + i);
	//letters = nesL;
	bool flag;
	string input = letters;
	do
	{
		flag = false;
		for (int i = 1; i < input.length(); i++)
			if (input[i - 1] > input[i])
			{
				char tmp = input[i];
				input[i] = input[i - 1];
				input[i - 1] = tmp;
				flag = true;
			}
	} while (flag);

	letters = input;
	const int N = letters.length();
	vector<Edges> edges;
	vector<vector<int>> matrix(N, vector<int>(N, INF));
	vector<vector<int>> mapToEg(N, vector<int>(N, -1));
	int chatIndex=0;
	int cmapIdex[200] = {0};
	for (int i = 0; i < N; i++)
		cmapIdex[letters[i]] = i;
	for (int i = 0; i < nodes.size(); i++)
	{
		vector<string> tmp = multiSplit(nodes[i], delimNode);
		char src = tmp[0][0], dst = tmp[1][0];
		int weight = stoi(tmp[2]);
		edges.push_back({ src,dst ,weight,false });
		mapToEg[cmapIdex[src]][cmapIdex[dst]]= mapToEg[cmapIdex[dst]][cmapIdex[src]] = i;
		matrix[cmapIdex[src]][cmapIdex[dst]] = matrix[cmapIdex[dst]][cmapIdex[src]] = weight;
	}
	// init end
	graph = Graph(letters, edges, 'A');
	graph.vertexs[graph.VTindex(startP)].mark = true;
	
	vector<int> dist(N, INF);
	vector<bool> visited(N,false);
	
	dist[cmapIdex[startP]] = 0;
	vector<int> source(N, 0);
	for (int i = 0; i < source.size(); i++)
		source[i] = i;

	int offset = 15;
	string title = "";
	for (int i = 0; i < offset; i++)
		title += " ";
	for (int i = 0; i < N; i++)
	{
		title += letters[i];
		for (int i = 0; i < offset; i++)
			title += " ";
	}

	for (int i = 0; i < N; i++)
	{
		int m = miniDist(dist, visited);
		graph.vertexs[graph.VTindex(letters[m])].mark = visited[m] = true;
		for (int k = 0; k < N; k++)
		{
			if (!visited[k] &&
				matrix[m][k] &&
				dist[m] != INF &&
				matrix[m][k] != INF &&
				dist[m] + matrix[m][k] < dist[k])
			{
				source[k] = m;
				dist[k] = dist[m] + matrix[m][k];
			}
		}
		if (source[m]!=m)
			graph.edges[ mapToEg[source[m]][m] ].mark = true;
		string includeC = "Including ";
		includeC += letters[m];
		if (i == 0)
			includeC = "Initial";
		extraText.push_back(includeC);
		for (int i = 0; i < letters.length(); i++)
			graph.vertexs[graph.VTindex(letters[i])].str = (dist[i] >= INF ? "INF" : std::to_string(dist[i]));
		graph.record(2, vector<Vertexs>(), vector<Edges>(),i*2);
		string titleBuff = "";
		for (int i = 0; i < offset - 2; i++)
			titleBuff += " ";
		for (int i = 0; i < letters.length(); i++)
		{
			titleBuff += letters[i];
			for (int k = titleBuff.length(); k < offset * (i + 2) + 1; k++)
				titleBuff += " ";
		}
		extraText.push_back(titleBuff);
		string lineBuff = "";
		lineBuff += startP;
		for (int i = 0; i < offset - 2; i++)
			lineBuff += " ";
		for (int j = 0; j < N; j++)
		{
			lineBuff += (dist[j]<INF?std::to_string(dist[j]):"9999");
			for (int k = lineBuff.length(); k < offset * (j + 2) + 1; k++)
				lineBuff += " ";
		}
		extraText.push_back(lineBuff);
	}
	dist.push_back(2);

	Invalidate();
}
int CUniversalDemoView::miniDist(vector<int>& dist, vector<bool>& visited)
{
	int minim = INT_MAX, indx=0;

	for (int i = 0; i < visited.size(); i++)
		if (!visited[i] && dist[i] <= minim)
			minim = dist[indx = i];
	return indx;
}
void CUniversalDemoView::OnFloydwarshallGraph()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	inputBox.InputStr = L"路徑：(A,B,8) (A,C,9) (A,D,6) (B,C,7) (B,E,9) (B,F,4) (C,D,6) (C,E,2) (C,F,3) (D,F,2)(E,G,3)(F,G,3)";
	if (inputBox.DoModal() != IDOK) return;
	// init input
	CT2CA toString(inputBox.InputStr);
	string inputStr(toString);
	if (cover)
	{
		finalResult.clear();
		extraText.clear();
	}
	display = 6;
	limitScale = 4;
	//
	extraText.clear();
	extraText.push_back(inputStr);
	//
	/*
	vector<string> multinput;
	char lineDelim[] = " \n";
	multinput = multiSplit(inputStr, lineDelim);
	for (int i = 0; i < multinput.size(); i++)
		finalResult.push_back(multinput[i]);
	int preStr = inputStr.find("：");
	inputStr = inputStr.substr(preStr + 2);
*/
	char spaceDelim[] = " ";
	//string letters = multiSplit(inputStr, spaceDelim)[0];
	string letters = "";
	int preStr = inputStr.find("：");
	inputStr = inputStr.substr(preStr + 2);
	char delim[] = " ()\n\r";
	vector<string> nodes = multiSplit(inputStr, delim);
	char delimNode[] = ", ";
	vector<Edges> edges;
	for (int i = 0; i < nodes.size(); i++)
	{
		vector<string> tmp = multiSplit(nodes[i], delimNode);
		if (letters.find(tmp[0]) == -1)
			letters += tmp[0];
		if (letters.find(tmp[1]) == -1)
			letters += tmp[1];
	}
	string nesL = "";
	for (int i = 0; i < letters.length(); i++)
		nesL += ('A' + i);
	letters = nesL;
	const int N = letters.length();
	vector<vector<int>> cost(N, vector<int>(N, INF));
	vector<vector<int>> mapToEg(N + 1, vector<int>(N, -1));
	vector<vector<int>> best(N, vector<int>(N, INF));
	int chatIndex = 0;
	int cmapIdex[200] = { 0 };
	for (int i = 0; i < N; i++)
	{
		cmapIdex[letters[i]] = i;
		cost[i][i] = 0;
		best[i][i] = i;
	}
	for (int i = 0; i < nodes.size(); i++)
	{
		vector<string> tmp = multiSplit(nodes[i], delimNode);
		char src = tmp[0][0], dst = tmp[1][0];
		int weight = stoi(tmp[2]);
		edges.push_back({ src,dst ,weight,false });
		edges.push_back({ dst,src ,weight,false });
		mapToEg[cmapIdex[src]][cmapIdex[dst]] = i;
		best[cmapIdex[src]][cmapIdex[dst]] = cmapIdex[src];
		best[cmapIdex[dst]][cmapIdex[src]] = cmapIdex[dst];
		cost[cmapIdex[src]][cmapIdex[dst]]= cost[cmapIdex[dst]][cmapIdex[src]] = weight;
	}
	// init end
	graph = Graph(letters, edges, 'A');

	for (int k = 0; k < N; k++)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
			{
				int tmp = cost[i][k] + cost[k][j];
				if (cost[i][j] > tmp) {
					cost[i][j] = tmp;
					best[i][j] = best[k][j];
				}
			}
	bestResult = best;
	int offset = 15;
	string title = "";
	for (int i = 0; i < offset; i++)
		title += " ";
	for (int i = 0; i < N; i++)
	{
		title += letters[i];
		for (int i = 0; i < offset; i++)
			title += " ";
	}
	
	extraText.push_back(title);
	for (int i = 0; i < N; i++)
	{
		string lineBuff = "";
		lineBuff += letters[i];
		for (int i = 0; i < offset-2; i++)
			lineBuff += " ";
		for (int j = 0; j < N; j++)
		{
			if (cost[i][j] >= INF)
				lineBuff += "INF/";
			else
			{
				lineBuff += std::to_string(cost[i][j]) + "/";
				lineBuff += letters[best[i][j]];
			}
			for (int k = lineBuff.length()+(cost[i][j]<INF?1:0); k < offset * (j + 2)+1; k++)
				lineBuff += " ";
		}
		
		extraText.push_back(lineBuff);
	}
	digraph = true;
	graph.record(2);
	Invalidate();
}
void CUniversalDemoView::OnFloydwarshallDiagraph()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CInput inputBox;
	inputBox.InputStr = L"路徑：(A,B,8) (B,A,10) (A,C,9) (A,D,6) (B,C,7) (B,E,9) (B,F,4) (C,B,2) (C,D,6) (C,E,2) (C,F,3) (D,F,2)(E,G,3)(F,G,3)";
	if (inputBox.DoModal() != IDOK) return;
	// init input
	CT2CA toString(inputBox.InputStr);
	string inputStr(toString);
	if (cover)
	{
		finalResult.clear();
		extraText.clear();
	}
	display = 6;
	limitScale = 4;
	//
	extraText.clear();
	extraText.push_back(inputStr);
	//
	/*
	vector<string> multinput;
	char lineDelim[] = " \n";
	multinput = multiSplit(inputStr, lineDelim);
	for (int i = 0; i < multinput.size(); i++)
		finalResult.push_back(multinput[i]);
	int preStr = inputStr.find("：");
	inputStr = inputStr.substr(preStr + 2);
*/
	char spaceDelim[] = " ";
	//string letters = multiSplit(inputStr, spaceDelim)[0];
	string letters = "";
	int preStr = inputStr.find("：");
	inputStr = inputStr.substr(preStr + 2);
	char delim[] = " ()\n\r";
	vector<string> nodes = multiSplit(inputStr, delim);
	char delimNode[] = ", ";
	vector<Edges> edges;
	for (int i = 0; i < nodes.size(); i++)
	{
		vector<string> tmp = multiSplit(nodes[i], delimNode);
		if (letters.find(tmp[0]) == -1)
			letters += tmp[0];
		if (letters.find(tmp[1]) == -1)
			letters += tmp[1];
	}
	string nesL = "";
	for (int i = 0; i < letters.length(); i++)
		nesL += ('A' + i);
	letters = nesL;
	const int N = letters.length();
	vector<vector<int>> cost(N, vector<int>(N, INF));
	vector<vector<int>> mapToEg(N + 1, vector<int>(N, -1));
	vector<vector<int>> best(N, vector<int>(N, INF));
	int chatIndex = 0;
	int cmapIdex[200] = { 0 };
	for (int i = 0; i < N; i++)
	{
		cmapIdex[letters[i]] = i;
		cost[i][i] = 0;
		best[i][i] = i;
	}
	for (int i = 0; i < nodes.size(); i++)
	{
		vector<string> tmp = multiSplit(nodes[i], delimNode);
		char src = tmp[0][0], dst = tmp[1][0];
		int weight = stoi(tmp[2]);
		edges.push_back({ src,dst ,weight,false });
		mapToEg[cmapIdex[src]][cmapIdex[dst]] = i;
		best[cmapIdex[src]][cmapIdex[dst]] = cmapIdex[src];
		cost[cmapIdex[src]][cmapIdex[dst]] = weight;
	}
	// init end
	graph = Graph(letters, edges, 'A');

	for (int k = 0; k < N; k++)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
			{
				int tmp = cost[i][k] + cost[k][j];
				if (cost[i][j] > tmp) {
					cost[i][j] = tmp;
					best[i][j] = best[k][j];
				}
			}
	bestResult = best;
	int offset = 15;
	string title = "";
	for (int i = 0; i < offset; i++)
		title += " ";
	for (int i = 0; i < N; i++)
	{
		title += letters[i];
		for (int i = 0; i < offset; i++)
			title += " ";
	}

	extraText.push_back(title);
	for (int i = 0; i < N; i++)
	{
		string lineBuff = "";
		lineBuff += letters[i];
		for (int i = 0; i < offset - 2; i++)
			lineBuff += " ";
		for (int j = 0; j < N; j++)
		{
			if (cost[i][j] >= INF)
				lineBuff += "INF/";
			else
			{
				lineBuff += std::to_string(cost[i][j]) + "/";
				lineBuff += letters[best[i][j]];
			}
			for (int k = lineBuff.length() + (cost[i][j] < INF ? 1 : 0); k < offset * (j + 2) + 1; k++)
				lineBuff += " ";
		}

		extraText.push_back(lineBuff);
	}
	digraph = true;
	graph.record(2);
	Invalidate();
}
void CUniversalDemoView::OnGraphVoronoidiagram()
{
	return;
	CInput inputBox;
	inputBox.InputStr = L"對位置：(0,3)(1,1)(2,2)(4,4)(0,0)(1,2)(3,1)(3,3)";
	if (inputBox.DoModal() != IDOK) return;
	// init input
	CT2CA toString(inputBox.InputStr);
	string inputStr(toString);
	if (cover)
		finalResult.clear();
	extraText.clear();
	display = 6;
	digraph = false;
	limitScale = 3;
	vector<string> multinput;
	char lineDelim[] = " \n";
	multinput = multiSplit(inputStr, lineDelim);
	for (int i = 0; i < multinput.size(); i++)
		finalResult.push_back(multinput[i]);
	//int preStr = inputStr.find("：");
	//inputStr = inputStr.substr(preStr + 2);

	//char spaceDelim[] = " ";
	//string letters = multiSplit(inputStr, spaceDelim)[0];
	int preStr = inputStr.find("：");
	inputStr = inputStr.substr(preStr + 2);
	char delim[] = " ()\n\r";
	vector<string> nodes = multiSplit(inputStr, delim);
	char delimNode[] = ",";
	vector<Vertexs> vertices;
	int maxVal = 0;
	const int scaleGh = 10;
	for (int i = 0; i < nodes.size(); i++)
	{
		vector<string> tmp = multiSplit(nodes[i], delimNode);
		if (tmp.size() == 3)
		{
			maxVal = max(max(stoi(tmp[1]), stoi(tmp[2])), maxVal);
			vertices.push_back({ tmp[0][0],stod(tmp[1]) ,stod(tmp[2]),false });
		}
		else if (tmp.size() == 2)
		{
			maxVal = max(max(stoi(tmp[0]), stoi(tmp[1])), maxVal);
			vertices.push_back({ (char)('A' + i),stod(tmp[0]) ,stod(tmp[1]),false });
		}
		else if (tmp.size() == 1)
		{
			maxVal = max(max(stoi(nodes[i]), stoi(nodes[i + 1])), maxVal);
			vertices.push_back({ (char)('A' + i),stod(nodes[i]) ,stod(nodes[i + 1]),false });
			i++;
		}
	}
	// init
	Diagram voronoi = Diagram(vertices,1000);

}
void CUniversalDemoView::OnSortHeapsort()
{
	CInput inputBox;
	inputBox.InputStr = L"E B A D H C G F";
	if (inputBox.DoModal() != IDOK) return;
	cover = false;
	finalData.clear();
	extraText.clear();
	vector<SortFmt> sortData;

	display = 0;
	sortData = initialize(inputBox.InputStr, ";");
	if (sortData.size() == 1 && sortData[0].str.length() > 1)
	{
		string tmp="";
		for (int i = 0; i < sortData[0].str.length(); i++, tmp += ';')
			tmp += sortData[0].str[i];
		CString cs(tmp.c_str());
		sortData = initialize(cs, ";");
	}
	const int N = sortData.size();
	arrToHeapTree(sortData);
	for (int i = N / 2 - 1; i >= 0; i--)
		heapify(sortData, N, i);
	arrToHeapTree(sortData);
	// Heap sort
	for (int i = N - 1; i >= 0; i--)
	{
		std::swap(sortData[0], sortData[i]);
		sortData[i].mark = true;
		// Heapify root element to get highest element at root again
		heapify(sortData, i, 0);
		arrToHeapTree(sortData);
	}
	cover = true;
	string out1 = "";
	for (int i = 0; i < sortData.size(); i++)
	{
		if (i)
			out1 += ',';
		out1 += sortData[i].str;
	}
	extraText.push_back(out1);
	Invalidate();
}
void CUniversalDemoView::heapify(vector<SortFmt>& arr, int n, int i)
{
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && arr[left].val > arr[largest].val)
		largest = left;

	if (right < n && arr[right].val > arr[largest].val)
		largest = right;

	// Swap and continue heapifying if root is not largest
	if (largest != i)
	{
		std::swap(arr[i], arr[largest]);
		heapify(arr, n, largest);
	}
}
void CUniversalDemoView::arrToHeapTree(vector<SortFmt>& sortData)
{
	//cover = false;
	tree = BST();
	vector<SortFmt> preorderRes, inorderRes;
	vector<SortFmt> treeAcData;
	getHeapTreePreorder(sortData, preorderRes,0);
	getHeapTreeInorder(sortData, inorderRes, 0);
	std::map<string, int> mapOrder;
	for (int i = 0; i < preorderRes.size(); i++)
		mapOrder.insert(std::pair<string, int>(preorderRes[i].str, i));
	for (int i = 0; i < inorderRes.size(); i++)
		treeAcData.push_back({ mapOrder[inorderRes[i].str], inorderRes[i].str,NULL,inorderRes[i].mark });

	tree.buildTree(treeAcData);
	tree.result = finalData;
	tree.preLocateNodes();
	finalData = tree.result;
}
void CUniversalDemoView::getHeapTreePreorder(vector<SortFmt>& sortData, vector<SortFmt>& res, int dst)
{
	//0 12 3456 78901234
	if (dst * 2 + 1 < sortData.size())
		getHeapTreePreorder(sortData, res, dst*2+1);
	res.push_back(sortData[dst]);
	if (dst * 2 + 2 < sortData.size())
		getHeapTreePreorder(sortData, res, dst*2+2);
	return;
}
void CUniversalDemoView::getHeapTreeInorder(vector<SortFmt>& sortData, vector<SortFmt>& res, int dst)
{
	res.push_back(sortData[dst]);
	if (dst * 2 + 1 < sortData.size())
		getHeapTreeInorder(sortData, res, dst * 2 + 1);
	if (dst * 2 + 2 < sortData.size())
		getHeapTreeInorder(sortData, res, dst * 2 + 2);
	return;
}
int CUniversalDemoView::restoreFWPath(Graph& queryGraph,int src, int dst)
{
	int k = bestResult[src][dst];
	if (k >= INF)
		return -1;
	if (k == src || k == dst)
	{
		queryGraph.vertexs[k].mark = true;
		char kC = queryGraph.vertexs[k].val;
		char dstC = queryGraph.vertexs[dst].val;
		int eg = queryGraph.EGindex(kC, dstC);
		queryGraph.edges[eg].mark = true;
		return src;
	}
	int res1 = restoreFWPath(queryGraph,src, k);
	int res2 = restoreFWPath(queryGraph,k, dst);
	if (res1 == -1 || res2 == -1)
		return -1;
	return src;
}

