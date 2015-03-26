// LaoYangWnd.h: interface for the CLaoYangWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAOYANGWND_H__B29AF2A7_B9CE_4D0E_A9E1_5AF013B5BA44__INCLUDED_)
#define AFX_LAOYANGWND_H__B29AF2A7_B9CE_4D0E_A9E1_5AF013B5BA44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct WinMessage//CreateWindow()������ShowWindow()���� ����Ҫ�����ṹ
{
	const  char *lpszClassName;//��������
	const  char *lpWindowName;//���ڱ�����
	DWORD dwStyle;//��������
	int x;//�������Ͻ�x����
	int y;//�������Ͻ�y����
	int nWidth;//���ڿ��
	int nHeight;//���ڸ߶�
	HWND hWndParent;//���ڵĸ�����
	HMENU HMenu;//���ڲ˵�
	HINSTANCE hinstance;//����ʵ��
	LPVOID lpParam;//���ڸ��Ӳ���
	int ShowParam;//����ȫΪCreateWindow()�Ĳ��� ����һ��ΪShowWindow()�����Ĳ���
};
struct Node
{
    char a[9];
    int pos;
    int index;
    string pa;
};
class CLaoYangWnd  
{
public:
	
	void SetDrect(int d);
	void SetSumWay();
	void LoadFile();
	CLaoYangWnd();
	virtual ~CLaoYangWnd();
	void SetWndMessage(WNDCLASS *pwc);//WinMain������ƴ�����  Ȼ�󴫸�CLaoYangWnd::wc
	void PreCreateWindow(WinMessage *lpMsg);//���ݲ���lpMsg����Ϣ��ɴ��ڵĴ���  ��ʾ  ����
	void Initialize(char *BitMapName[]);//��ʼ�����е�ÿ��˽�г�Ա ��ǰ�����豸������hdc����̨���豸������hbackdc
	void MessageLoop();//������Ϣѭ��	
	Point res[400];//Ѱ��·���ĵ���
	bool run;
private:	
	int cools;
	int sumway;//��ǰѰ��·���ĳ���  ��res�������Ч��С
	int step;//Ϊ������������  ���ﵱǰ�ߵĲ���
	Node start;
	bool foot[500000];
	string path[500000];
	void bfs();
	int kangtuo(char a[]);
	void Update();//�����ػ洰�ں���  �ɴ�����Ϣѭ����������������ʵ����������
	WNDCLASS wc;//���������
	HWND hWnd;//���ھ��
	HDC hdc;//ǰ�����豸������
	HDC hbackdc;//��̨���豸������
	HDC memDC;//Ϊ�˴洢λͼ  �ڴ����豸������
	HBITMAP BackMap,PlayBitMap[9];//��̨ͼ  ����λͼ
	RECT clientRC;//��Ϸ���ھ���
	int w,h;//����λͼ�Ŀ�ȸ߶�
};

#endif // !defined(AFX_LAOYANGWND_H__B29AF2A7_B9CE_4D0E_A9E1_5AF013B5BA44__INCLUDED_)
