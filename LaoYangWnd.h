// LaoYangWnd.h: interface for the CLaoYangWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAOYANGWND_H__B29AF2A7_B9CE_4D0E_A9E1_5AF013B5BA44__INCLUDED_)
#define AFX_LAOYANGWND_H__B29AF2A7_B9CE_4D0E_A9E1_5AF013B5BA44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct WinMessage//CreateWindow()函数和ShowWindow()函数 所需要参数结构
{
	const  char *lpszClassName;//窗口类名
	const  char *lpWindowName;//窗口标题名
	DWORD dwStyle;//窗口类型
	int x;//窗口左上角x坐标
	int y;//窗口左上角y坐标
	int nWidth;//窗口宽度
	int nHeight;//窗口高度
	HWND hWndParent;//窗口的父窗口
	HMENU HMenu;//窗口菜单
	HINSTANCE hinstance;//窗口实例
	LPVOID lpParam;//窗口附加参数
	int ShowParam;//以上全为CreateWindow()的参数 仅此一个为ShowWindow()函数的参数
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
	void SetWndMessage(WNDCLASS *pwc);//WinMain函数设计窗口类  然后传给CLaoYangWnd::wc
	void PreCreateWindow(WinMessage *lpMsg);//根据参数lpMsg的信息完成窗口的创建  显示  更新
	void Initialize(char *BitMapName[]);//初始化类中的每个私有成员 如前景的设备描述表hdc，后台的设备描述表hbackdc
	void MessageLoop();//窗口消息循环	
	Point res[400];//寻找路径的点结果
	bool run;
private:	
	int cools;
	int sumway;//当前寻得路径的长度  即res数组的有效大小
	int step;//为了是任务行走  人物当前走的步数
	Node start;
	bool foot[500000];
	string path[500000];
	void bfs();
	int kangtuo(char a[]);
	void Update();//更新重绘窗口函数  由窗口消息循环函数连续调用以实现人物行走
	WNDCLASS wc;//窗口类设计
	HWND hWnd;//窗口句柄
	HDC hdc;//前景的设备描述表
	HDC hbackdc;//后台的设备描述表
	HDC memDC;//为了存储位图  内存中设备描述表
	HBITMAP BackMap,PlayBitMap[9];//后台图  人物位图
	RECT clientRC;//游戏窗口矩形
	int w,h;//人物位图的宽度高度
};

#endif // !defined(AFX_LAOYANGWND_H__B29AF2A7_B9CE_4D0E_A9E1_5AF013B5BA44__INCLUDED_)
