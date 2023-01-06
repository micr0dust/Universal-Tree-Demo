
// Universal DemoView.h: CUniversalDemoView 類別的介面
//

#pragma once
#include<vector>
#include<string>

using std::vector;
using std::string;

struct SortFmt
{
	long long val;
	string str;
	int parent=NULL;
    bool mark = false;
};
struct Node {
	Node(SortFmt value=SortFmt()) : data(value), left(NULL), right(NULL) {}
	SortFmt data;
	Node* left;
	Node* right;
};
struct Node4
{
    SortFmt key1;
    SortFmt key2;
    SortFmt key3;
	Node4* M1;
	Node4* M2;
	Node4* M3;
	Node4* M4;
	int num;
	Node4* parent;
};
class tree234
{
    int flag = 0;
    Node4* newNode4(SortFmt key, Node4* parent, Node4* M1 = NULL, Node4* M2 = NULL)
    {
        Node4* node = new Node4();
        node->key1 = key;
        node->key2 = {0,""};
        node->key3 = { 0,"" };
        node->M1 = M1;
        node->M2 = M2;
        node->M3 = NULL;
        node->M4 = NULL;
        node->num = 1;
        node->parent = parent;
        if (M1)
            node->M1->parent = node;
        if (M2)
            node->M2->parent = node;
        return node;
    }
    bool leaf(Node4* node)
    {
        return (!node->M1);
    }
    Node4* insert(Node4* node, SortFmt key)
    {
        if (!_root)
            _root = newNode4(key, NULL);
        else if (leaf(node))
        {
            switch (node->num)
            {
            case 1:
                if (key.val > node->key1.val)
                {// AK
                    node->key2 = key;
                    node->num++;
                }
                else
                {// KA
                    node->key2 = node->key1;
                    node->key1 = key;
                    node->num++;
                }
                break;
            case 2:
                if (key.val > node->key2.val)
                {// ABK
                    node->key3 = key;
                    node->num++;
                }
                else if (key.val > node->key1.val)
                {// AKB
                    node->key3 = node->key2;
                    node->key2 = key;
                    node->num++;
                }
                else
                {// KAB
                    node->key3 = node->key2;
                    node->key2 = node->key1;
                    node->key1 = key;
                    node->num++;
                }
                break;
            case 3:
                _root = split(node, key.val);
                flag = 1;
                break;
            }
        }
        else
        {
            if (node->key3.val != 0 && key.val > node->key3.val)
            {
                if (!node->M4)
                    node->M4 = newNode4(key, node);
                else
                    _root = insert(node->M4, key);
            }
            else if (node->key2.val != 0 && key.val > node->key2.val)
            {
                if (!node->M3)
                    node->M3 = newNode4(key, node);
                else
                    _root = insert(node->M3, key);
            }
            else if (node->key1.val != 0 && key.val > node->key1.val)
            {
                if (!node->M2)
                    node->M2 = newNode4(key, node);
                else
                    _root = insert(node->M2, key);
            }
            else
            {
                if (!node->M1)
                    node->M1 = newNode4(key, node);
                else
                    _root = insert(node->M1, key);
            }
        }
        return _root;
    }
    Node4* split(Node4* node, int key)
    {
        if (!node->parent) // root
        {
            Node4* tmpN = newNode4(node->key2, NULL);
            tmpN->M1 = newNode4(node->key1, tmpN);
            tmpN->M1->M1 = node->M1;
            if (tmpN->M1->M1 != NULL)
                tmpN->M1->M1->parent = tmpN->M1;
            tmpN->M1->M2 = node->M2;
            if (tmpN->M1->M2 != NULL)
                tmpN->M1->M2->parent = tmpN->M1;// left subtree
            tmpN->M2 = newNode4(node->key3, tmpN);
            tmpN->M2->M1 = node->M3;
            if (tmpN->M2->M1 != NULL)
                tmpN->M2->M1->parent = tmpN->M2;
            tmpN->M2->M2 = node->M4;
            if (tmpN->M2->M2 != NULL)
                tmpN->M2->M2->parent = tmpN->M2;// right subtree
            _root = tmpN;
        }
        else if (node->parent->num == 3)
        {
            _root = split(node->parent, key);
        }
        else // leaf
        {
            // midkey up
            if (node->parent->key2.val != 0 && node->key2.val > node->parent->key2.val)
            {
                node->parent->key3 = node->key2;
                node->parent->num++;
            }
            else if (node->parent->key1.val != 0 && node->key2.val > node->parent->key1.val)
            {
                node->parent->key3 = node->parent->key2;
                node->parent->key2 = node->key2;
                node->parent->num++;
            }
            else
            {
                node->parent->key3 = node->parent->key2;
                node->parent->key2 = node->parent->key1;
                node->parent->key1 = node->key2;
                node->parent->num++;
            }
            // key1,key3 put place
            if (node == node->parent->M1) // M1 split
            {
                node->parent->M4 = node->parent->M3;
                node->parent->M3 = node->parent->M2;
                node->parent->M2 = newNode4(node->key3, node->parent, node->M3, node->M4);
                node->parent->M1 = newNode4(node->key1, node->parent, node->M1, node->M2);
                node = node->parent->M1;
            }
            else if (node == node->parent->M2) // M2 split
            {
                node->parent->M4 = node->parent->M3;
                node->parent->M3 = newNode4(node->key3, node->parent, node->M3, node->M4);
                node->parent->M2 = newNode4(node->key1, node->parent, node->M1, node->M2);
                node = node->parent->M2;
            }
            else if (node == node->parent->M3) // M3 split
            {
                node->parent->M4 = newNode4(node->key3, node->parent, node->M3, node->M4);
                node->parent->M3 = newNode4(node->key1, node->parent, node->M1, node->M2);
                node = node->parent->M3;
            }
        }
        return _root; // rewrite
    }
    void preLocateNodes(Node4* treeNode, int y, int neighbor=0)
    {
        if (!treeNode)
            return;
        if (treeNode->key3.val) {
            int M1s = getNodesMount(treeNode->M1);
            int M2s = getNodesMount(treeNode->M2);
            int M3s = getNodesMount(treeNode->M3);
            int M4s = getNodesMount(treeNode->M4);
            preLocateNodes(treeNode->M1, -(abs(y) + 1), M2s);
            preLocateNodes(treeNode->M2, -(abs(y) + 1));
            int parentX = 
                (y > 0 ?
                    result.size() - M1s - M2s - neighbor - 1 :
                    result.size() + M3s + M4s + neighbor + 1);
            string str = "" + treeNode->key1.str+", " + treeNode->key2.str + ", " + treeNode->key3.str;
            result.push_back({ y,str,parentX });
            preLocateNodes(treeNode->M3, abs(y) + 1);
            preLocateNodes(treeNode->M4, abs(y) + 1, M3s);
        }
        else if (treeNode->key2.val) {
            int M1s = getNodesMount(treeNode->M1);
            int M2s = getNodesMount(treeNode->M2);
            int M3s = getNodesMount(treeNode->M3);
            preLocateNodes(treeNode->M1, -(abs(y) + 1), M2s);
            preLocateNodes(treeNode->M2, -(abs(y) + 1));
            int parentX = (y > 0 ? 
                result.size() - M1s - M2s - neighbor - 1 :
                result.size() + M3s + neighbor + 1);
            string str = "" + treeNode->key1.str + ", " + treeNode->key2.str;
            result.push_back({ y,str,parentX });
            preLocateNodes(treeNode->M3, abs(y) + 1);
        }
        else {
            int M1s = getNodesMount(treeNode->M1);
            int M2s = getNodesMount(treeNode->M2);
            preLocateNodes(treeNode->M1, -(abs(y) + 1));
            int parentX = (y > 0 ?
                result.size() - M1s - neighbor - 1 :
                result.size() + M2s + neighbor + 1);
            result.push_back({ y,treeNode->key1.str,parentX });
            preLocateNodes(treeNode->M2, abs(y) + 1);
        }
    }
    void displayAsRBT(Node4* treeNode, int y)
    {
        if (!treeNode)
            return;

        if (treeNode->key3.val) {
            int M1s = getNodesMount(treeNode->M1);
            int M2s = getNodesMount(treeNode->M2);
            int M3s = getNodesMount(treeNode->M3);
            int M4s = getNodesMount(treeNode->M4);
            displayAsRBT(treeNode->M1, -(abs(y) + 2));
            int parentX = result.size() + M2s + 1;
            result.push_back({ -(abs(y) + 1),treeNode->key1.str,parentX,true });
            displayAsRBT(treeNode->M2, abs(y) + 2);
            parentX = (y > 0 ?
                result.size() - M1s - M2s - 1 - 1 :
                result.size() + M3s + M4s + 1 + 1);
            result.push_back({ y,treeNode->key2.str,parentX });
            displayAsRBT(treeNode->M3, -(abs(y) + 2));
            parentX =result.size() - M3s - 1;
            result.push_back({ abs(y) + 1,treeNode->key3.str,parentX,true });
            displayAsRBT(treeNode->M4, abs(y) + 2);
        }
        else if (treeNode->key2.val) {
            int M1s = getNodesMount(treeNode->M1);
            int M2s = getNodesMount(treeNode->M2);
            int M3s = getNodesMount(treeNode->M3);
            displayAsRBT(treeNode->M1, -(abs(y) + 2));
            int parentX = result.size() + M3s + 1;
            result.push_back({ -(abs(y) + 1),treeNode->key1.str,parentX,true });
            displayAsRBT(treeNode->M2, abs(y) + 2);
            parentX = (y > 0 ?
                result.size() - M1s - M2s - 1-1 :
                result.size() + M3s + 1);
            result.push_back({ y,treeNode->key2.str,parentX });
            displayAsRBT(treeNode->M3, abs(y) + 1);
        }
        else {
            int M1s = getNodesMount(treeNode->M1);
            int M2s = getNodesMount(treeNode->M2);
            displayAsRBT(treeNode->M1, -(abs(y) + 1));
            int parentX = (y > 0 ?
                result.size() - M1s - 1 :
                result.size() + M2s + 1);
            result.push_back({ y,treeNode->key1.str,parentX });
            displayAsRBT(treeNode->M2, abs(y) + 1);
        }
    }
public:
    vector<SortFmt> result;
    Node4* _root;
    tree234()
    {
        _root = NULL;
    }; // constructor
    void insert(SortFmt key) {
        do
        {
            if (flag)
                flag = 0;
            insert(_root, key);
        } while (flag);
    }
    void buildTree(vector<SortFmt> data) {
        for (SortFmt item : data)
            insert(item);
    }
    void preLocateNodes() { preLocateNodes(_root, 0); }
    void displayAsRBT() { displayAsRBT(_root, 0); }
    int getNodesMount(Node4* treeNode)
    {
        if (!treeNode)
            return NULL;
        int count = 0;
        if (treeNode->M1)
            count += getNodesMount(treeNode->M1);
        if (treeNode->M2)
            count += getNodesMount(treeNode->M2);
        if (treeNode->M3)
            count += getNodesMount(treeNode->M3);
        if (treeNode->M4)
            count += getNodesMount(treeNode->M4);
        return count + 1;
    }
};
class BST {
private:
	Node* _root;
    Node* insert(Node* treeNode, SortFmt data)
    {
        if (!treeNode)
            return treeNode = new Node(data);
        else if (data.val <= treeNode->data.val)
            treeNode->left = insert(treeNode->left, data);
        else
            treeNode->right = insert(treeNode->right, data);
        if (enableAVL) {
            int detla = balanced(treeNode);
            if (detla < -1) {
                if (balanced(treeNode->left) == -1)
                    return rightRotate(treeNode);
                else if (balanced(treeNode->left) == 1) {
                    treeNode->left = leftRotate(treeNode->left);
                    return rightRotate(treeNode);
                }
            }
            else if (detla > 1)
            {
                if (balanced(treeNode->right) == 1)
                    return leftRotate(treeNode);
                else if (balanced(treeNode->right) == -1) {
                    treeNode->right = rightRotate(treeNode->right);
                    return leftRotate(treeNode);
                }
            }
        }

        return treeNode;
    }
    Node* remove(Node* root, char key)
    {
        if (!root)
            return root;
        if (key < root->data.str[0])
            root->left = remove(root->left, key);
        else if (key > root->data.str[0])
            root->right = remove(root->right, key);
        else {
            if (!root->left && !root->right)
                return NULL;
            else if (!root->left) {
                Node* alter = root->right;
                delete(root);
                return alter;
            }
            else if (root->right == NULL) {
                Node* alter = root->left;
                delete(root);
                return alter;
            }
            Node* alter = getMinNode(root->right);
            root->data = alter->data;
            root->right = remove(root->right, alter->data.str[0]);
        }
        return root;
    }
    int getHeight(Node* treeNode)
    {
        if (!treeNode)
            return 0;
        return 1 + max(getHeight(treeNode->left), getHeight(treeNode->right));
    }
    int balanced(Node* treeNode)
    {
        if (!treeNode) return 0;
        return getHeight(treeNode->right) - getHeight(treeNode->left);
    }
    void deleteBST(Node* treeNode)
    {
        if (!treeNode)
            return;

        Node* curTreeNode = treeNode;
        Node* leftTreeNode = treeNode->left;
        Node* rightTreeNode = treeNode->right;
        delete(curTreeNode);
        deleteBST(leftTreeNode);
        deleteBST(rightTreeNode);
    }
    void inOrder(Node* treeNode)
    {
        if (!treeNode)
            return;
        inOrder(treeNode->left);
        result.push_back(treeNode->data);
        inOrder(treeNode->right);
    }
    void preOrder(Node* treeNode)
    {
        if (!treeNode)
            return;
        result.push_back(treeNode->data);
        preOrder(treeNode->left);
        preOrder(treeNode->right);
    }
    void postOrder(Node* treeNode)
    {
        if (!treeNode)
            return;
        postOrder(treeNode->left);
        postOrder(treeNode->right);
        result.push_back(treeNode->data);
    }
    void preLocateNodes(Node* treeNode, int y)
    {
        if (!treeNode)
            return;
        preLocateNodes(treeNode->left, -(abs(y) + 1));
        int parentX = (y > 0 ? result.size() - getNodesMount(treeNode->left) - 1 : result.size() + getNodesMount(treeNode->right) + 1);
        result.push_back({ y,treeNode->data.str,parentX, treeNode->data.mark });
        preLocateNodes(treeNode->right, abs(y) + 1);
    }
    int rightLen(Node* treeNode)
    {
        //the BST is empty!
        if (!treeNode)
            return NULL;
        int count = 0;
        for (; treeNode->right; count++)
            treeNode = treeNode->right;
        return count;
    }
    int leftLen(Node* treeNode)
    {
        if (!treeNode)
            return NULL;
        int count = 0;
        for (; treeNode->left; count++)
            treeNode = treeNode->left;
        return count;
    }
    int nodesMount(Node* treeNode)
    {
        if (!treeNode)
            return NULL;
        int count = 0;
        if (treeNode->left)
            count += nodesMount(treeNode->left);
        if (treeNode->right)
            count += nodesMount(treeNode->right);
        return count + 1;
    }
    void search(Node* treeNode, char c)
    {
        if (!treeNode)
            return;
        if(treeNode->data.str[0] == c)
            treeNode->data.mark = true;
        if (c <= treeNode->data.str[0])
            search(treeNode->left, c);
        else search(treeNode->right, c);
    }
public:
	vector<SortFmt> result;
    BST()
    {
        enableAVL = false;
        _root = NULL;
    }
    ~BST()
    {
        deleteBST();
        result.clear();
    }

    void insert(SortFmt data) { _root = insert(_root, data); }
    void remove(char c) { 
        Node* pointerNode = new Node();
        pointerNode->right = _root;
        _root = remove(pointerNode, c)->right;
        delete(pointerNode);
    }

	int getHeight() { return getHeight(_root); }
	int rightLength(Node* treeNode) { return rightLen(treeNode); };
	int leftLength(Node* treeNode) { return rightLen(treeNode); };
	int getNodesMount(Node* treeNode) { return nodesMount(treeNode); };
    void search(char c) { search(_root,c); };

    Node* getMaxNode(Node* treeNode)
    {
        if (!treeNode)
            return NULL;
        Node* node = treeNode;
        while (node->right)
            node = node->right;
        return node;
    }
    Node* getMinNode(Node* treeNode)
    {
        if (!treeNode)
            return NULL;
        Node* node = treeNode;
        while (node->left)
            node = node->left;
        return node;
    }

    Node* rightRotate(Node* treeNode) {
        Node* mid = treeNode->left;
        Node* sub = mid->right;
        mid->right = treeNode;
        treeNode->left = sub;
        return mid;
    }
    Node* leftRotate(Node* treeNode) {
        Node* mid = treeNode->right;
        Node* sub = mid->left;
        mid->left = treeNode;
        treeNode->right = sub;
        return mid;
    }

	void deleteBST() { deleteBST(_root); }

	void inOrder() { inOrder(_root); }
	void preOrder() { preOrder(_root); }
	void postOrder() { postOrder(_root); }
	void preLocateNodes() { preLocateNodes(_root, 0); }
    void buildTree(vector<SortFmt> data) {
        for (SortFmt item : data)
            insert(item);
    }

	// setting
	bool enableAVL;
};
class CUniversalDemoView : public CView
{
protected: // 僅從序列化建立
	CUniversalDemoView() noexcept;
	DECLARE_DYNCREATE(CUniversalDemoView)

// 屬性
public:
	CUniversalDemoDoc* GetDocument() const;

// 作業
public:

// 覆寫
public:
	virtual void OnDraw(CDC* pDC);  // 覆寫以描繪此檢視
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 程式碼實作
public:
	virtual ~CUniversalDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 產生的訊息對應函式
protected:
	DECLARE_MESSAGE_MAP()
public:
    tree234 T234;
	BST tree;
	CString original;
	string query;
	vector<string> splited;
	vector<SortFmt> finalData;
	vector<string> finalResult;
	bool inputChange;
	RECT screen;
	string sortDelim;
	CPoint BeginPos;
	CPoint originPos;
	CPoint currentPos;
	bool writeToFile;
	bool dragging;
	bool enableAVL;
	bool enableTree234;
    bool enableRBT;
    int display;
    int scale;
    int limitScale;
    double angle;

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnWrite();

	vector<string> multiSplit(string str, char* delim);
	vector<SortFmt> initialize(CString input, string delimStr, int mode = 0);
	void printData(vector<SortFmt> sortData);
	int bits(int x, int shift, int bit);
	//	afx_msg void OnInput();
	afx_msg void preorderToTree();
	afx_msg void postorderToTree();
	afx_msg void OnAvl();
	afx_msg void OnOrder();
	afx_msg void On234tree();
	void fractalTree(CDC* MemDC, int layer, CPoint pos, double angle, double cum);
    void carpet(CDC* MemDC, int layer, double x, double y, double div);
    void snowflake(CDC* MemDC, int iter, int x, int y, int x5, int y5);
    afx_msg void OnfractalTree();
    afx_msg void OnCarpet();
    afx_msg void OnSnowflake();
    afx_msg void OnInsert();
    afx_msg void OnSearch();
    afx_msg void OnDelete();
    afx_msg void OnRbt();
};

#ifndef _DEBUG  // 對 Universal DemoView.cpp 中的版本進行偵錯
inline CUniversalDemoDoc* CUniversalDemoView::GetDocument() const
   { return reinterpret_cast<CUniversalDemoDoc*>(m_pDocument); }
#endif

