// LaoYangWnd.cpp: implementation of the CLaoYangWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LaoYangWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLaoYangWnd::CLaoYangWnd()
{	
	this->cools = 0;
	run = false;	
	FILE * fp = fopen("start.txt","r");
	if(!fp)
	{
		::MessageBox(::GetForegroundWindow(),"找不到start.txt文件","老杨提示",0);
		exit(1);
	}
	int i,j;
	for(i = 0;i<9;i++)
	{
		fscanf(fp,"%d",&start.a[i]);
		if(0 == start.a[i])
			j = i;
	}
	fclose(fp);
	start.pos = j;
	start.index = kangtuo(start.a);
	memset(this->res,0,sizeof(res));//寻路结果
	this->sumway = this->step = 0;//路径长度  当前步数
}

void CLaoYangWnd::SetWndMessage(WNDCLASS *pwc)
{
	this->wc = *pwc;//传递给类中成员
}

void CLaoYangWnd::PreCreateWindow(WinMessage *lpMsg)
{
	::RegisterClass(&this->wc);//注册窗口类
	this->hWnd = ::CreateWindow(lpMsg->lpszClassName,lpMsg->lpWindowName,lpMsg->dwStyle,
		lpMsg->x,lpMsg->y,lpMsg->nWidth,lpMsg->nHeight,lpMsg->hWndParent,
		lpMsg->HMenu,lpMsg->hinstance,lpMsg->lpParam);//根据lpMsg创建窗口

	::ShowWindow(this->hWnd,lpMsg->ShowParam);//根据lpMsg显示窗口
	::UpdateWindow(this->hWnd);//更新窗口
}

void CLaoYangWnd::Initialize(char *BitMapName[])//窗口的位图文件名
{
	this->bfs();	
	this->hdc = GetDC(this->hWnd);//获取前景设备描述表
	this->hbackdc = ::CreateCompatibleDC(this->hdc);//创建与前景兼容的后台设备描述表
	GetClientRect(this->hWnd,&this->clientRC);//获取窗口矩形区域

	HPEN hp = ::CreatePen(PS_SOLID,2,RGB(255,0,255));//创建画笔
	::SelectObject(this->hbackdc,hp);//选入后台设备描述表
	//创建与前景兼容的后台位图
	this->BackMap = ::CreateCompatibleBitmap(this->hdc,this->clientRC.right,clientRC.bottom);
	::SelectObject(this->hbackdc,this->BackMap);//后台位图选入后台设备描述表

	for(int i = 0;i<9;i++)
	this->PlayBitMap[i] = (HBITMAP)LoadImage((this->wc).hInstance,BitMapName[i],IMAGE_BITMAP,
			0,0,LR_LOADFROMFILE);//从文件载入位图	
	this->memDC = ::CreateCompatibleDC(this->hdc);//创建与前景兼容的后台DC用于位图
	this->sumway = this->path[start.index].size()-1;
	if(this->sumway == -1)
	{
		::MessageBox(::GetForegroundWindow(),"无法移动","老杨提示",0);
		return ;
	}
}

//注: 更新操作全部在后台设备描述表完成 然后把后台的显示到前景  因为内存的速度远远大于显示器
void CLaoYangWnd::Update()
{
	static int oldTime=0,oldTime1 = 0;//计时器
	int newTime=GetTickCount();//返回当前时间
	if(newTime-oldTime<50)//50毫秒走一步
		return;
	
	oldTime=newTime;//更新时间
	int i;								
	::FillRect(this->hbackdc,&this->clientRC,(HBRUSH)GetStockObject(WHITE_BRUSH));//白色刷子擦除客户区域
	::TextOutA(this->hbackdc,130,5,"在start.txt文件中设置初始状态,按F1读取F2开始移动\n",48);	
	///////////////画网格//////////////////		
	for(i = 0;i<800;i+=200)
	{
		::MoveToEx(this->hbackdc,i,40,NULL);
		::LineTo(this->hbackdc,i,640);
	}
	for(i = 0;i<800;i+=200)
	{
		::MoveToEx(this->hbackdc,0,i+40,NULL);
		::LineTo(this->hbackdc,600,i+40);
	}
	//////////////贴位图///////////////////
	BITMAP bm;//定义位图实例	
	if(newTime-oldTime1>1000 && sumway>=0 && run)
	{			
		char c = path[start.index][sumway--];
		int x = start.pos/3,y = start.pos%3;
		oldTime1=newTime;
		switch(c)
		{
		case 'u':
			x-=1;
			break;
		case 'd':
			x+=1;
			break;
		case 'l':
			y-=1;
			break;
		case 'r':
			y+=1;
			break;
		}
		int newpos = x*3+y;
		char temp = start.a[newpos];
		start.a[newpos] = 0;
		start.a[start.pos] = temp;
		start.pos = newpos;
		cools++;
	}	
	char buffer[20];sprintf(buffer,"你当前已经移动了%d步,加油啊！",cools);
	::TextOutA(this->hbackdc,200,20,buffer,28);
	if(-1 == sumway)
		run = false;
	for(i = 0;i<9;i++)
	{
		::GetObject(this->PlayBitMap[start.a[i]],sizeof(bm),&bm);//用bm保存载入的位图
		this->w = bm.bmWidth;this->h = bm.bmHeight;	//获取图片的宽度和高度
		::SelectObject(this->memDC,PlayBitMap[start.a[i]]);//载入的位图选入与前景兼容的后台DC(用于位图)
		TransparentBlt(this->hbackdc,(i%3)*200,(i/3)*200+40,
			w,h,this->memDC,0,0,this->w,this->h,GetPixel(this->memDC,0,0));
	}
	//////////////后台设备描述表在前景中显示///////////////////////////
	::BitBlt(this->hdc,0,0,this->clientRC.right,this->clientRC.bottom,this->hbackdc,0,0,SRCCOPY);
}

void CLaoYangWnd::MessageLoop()
{
	MSG msg;
	memset(&msg,0,sizeof(msg));
	while(msg.message!=WM_QUIT)
	{
		this->Update();//在消息循环中更新画面
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{			
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}		
		Sleep(1);
	}
}

CLaoYangWnd::~CLaoYangWnd()
{
	//清理资源
	ReleaseDC(hWnd,hdc);
	DeleteDC(hbackdc);
}



int CLaoYangWnd::kangtuo(char a[])
{
	int fac[]={1,1,2,6,24,120,720,5040,40320};//康托展开判重
    int i,j,sum = 0,t = 0;
    for(i = 0;i<9;i++)
    {
        t = 0;
        for(j = i+1;j<9;j++)
            if(a[j]<a[i])
                t++;
        sum += t*fac[9-i-1];
    }
    return sum+1;
}

void CLaoYangWnd::bfs()
{
	char h[4] = {'d','u','r','l'};
    int i,x,y;
    Node now,next;now.pa = "";next.pa = "";
    for(i = 0;i<8;i++)now.a[i] = i+1;
    now.a[8] = 0;now.pos = 8;
    now.index = 46234;foot[46234] = true;    
    queue<Node> q;q.push(now);
    int dec[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    while(!q.empty())
    {
        now = q.front();
        q.pop();
        for(i = 0;i<4;i++)
        {
            x = now.pos/3+dec[i][0];
            y = now.pos%3+dec[i][1];
            if(x<0 || y<0 || x>2 || y>2)
                continue;
            next = now;
            next.pos = 3*x+y;
            next.a[next.pos] = 0;
            next.a[now.pos] = now.a[next.pos];
            next.index = kangtuo(next.a);
            if(!foot[next.index])
            {
                foot[next.index] = true;
                next.pa += h[i];
                path[next.index] = next.pa;
                q.push(next);
            }
        }
    }
    
}

void CLaoYangWnd::LoadFile()
{
	FILE * fp = fopen("start.txt","r");
	if(!fp)
	{
		::MessageBox(::GetForegroundWindow(),"找不到start.txt文件","老杨提示",0);
		exit(1);
	}
	int i,j;
	for(i = 0;i<9;i++)
	{
		fscanf(fp,"%d",&start.a[i]);
		if(0 == start.a[i])
			j = i;
	}
	start.pos = j;
	start.index = kangtuo(start.a);
	fclose(fp);
	cools = 0;
}

void CLaoYangWnd::SetSumWay()
{
	this->sumway = this->path[start.index].size()-1;
	if(this->sumway == -1)
	{
		::MessageBox(::GetForegroundWindow(),"无法移动","老杨提示",0);
		return ;
	}
}

void CLaoYangWnd::SetDrect(int d)
{
	int x = start.pos/3,y = start.pos%3;
	switch(d)
	{
	case 1:
		if(x>0)
		{x-=1;cools++;}
		break;
	case 0:
		if(x<2)
		{x+=1;cools++;}
		break;
	case 3:
		if(y>0)
		{y-=1;cools++;}
		break;
	case 2:
		if(y<2)
		{y+=1;cools++;}
		break;
	}
	int newpos = x*3+y;
	char temp = start.a[newpos];
	start.a[newpos] = 0;
	start.a[start.pos] = temp;
	start.pos = newpos;
	start.index = kangtuo(start.a);
}
