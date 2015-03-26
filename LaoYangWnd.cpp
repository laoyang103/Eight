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
		::MessageBox(::GetForegroundWindow(),"�Ҳ���start.txt�ļ�","������ʾ",0);
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
	memset(this->res,0,sizeof(res));//Ѱ·���
	this->sumway = this->step = 0;//·������  ��ǰ����
}

void CLaoYangWnd::SetWndMessage(WNDCLASS *pwc)
{
	this->wc = *pwc;//���ݸ����г�Ա
}

void CLaoYangWnd::PreCreateWindow(WinMessage *lpMsg)
{
	::RegisterClass(&this->wc);//ע�ᴰ����
	this->hWnd = ::CreateWindow(lpMsg->lpszClassName,lpMsg->lpWindowName,lpMsg->dwStyle,
		lpMsg->x,lpMsg->y,lpMsg->nWidth,lpMsg->nHeight,lpMsg->hWndParent,
		lpMsg->HMenu,lpMsg->hinstance,lpMsg->lpParam);//����lpMsg��������

	::ShowWindow(this->hWnd,lpMsg->ShowParam);//����lpMsg��ʾ����
	::UpdateWindow(this->hWnd);//���´���
}

void CLaoYangWnd::Initialize(char *BitMapName[])//���ڵ�λͼ�ļ���
{
	this->bfs();	
	this->hdc = GetDC(this->hWnd);//��ȡǰ���豸������
	this->hbackdc = ::CreateCompatibleDC(this->hdc);//������ǰ�����ݵĺ�̨�豸������
	GetClientRect(this->hWnd,&this->clientRC);//��ȡ���ھ�������

	HPEN hp = ::CreatePen(PS_SOLID,2,RGB(255,0,255));//��������
	::SelectObject(this->hbackdc,hp);//ѡ���̨�豸������
	//������ǰ�����ݵĺ�̨λͼ
	this->BackMap = ::CreateCompatibleBitmap(this->hdc,this->clientRC.right,clientRC.bottom);
	::SelectObject(this->hbackdc,this->BackMap);//��̨λͼѡ���̨�豸������

	for(int i = 0;i<9;i++)
	this->PlayBitMap[i] = (HBITMAP)LoadImage((this->wc).hInstance,BitMapName[i],IMAGE_BITMAP,
			0,0,LR_LOADFROMFILE);//���ļ�����λͼ	
	this->memDC = ::CreateCompatibleDC(this->hdc);//������ǰ�����ݵĺ�̨DC����λͼ
	this->sumway = this->path[start.index].size()-1;
	if(this->sumway == -1)
	{
		::MessageBox(::GetForegroundWindow(),"�޷��ƶ�","������ʾ",0);
		return ;
	}
}

//ע: ���²���ȫ���ں�̨�豸��������� Ȼ��Ѻ�̨����ʾ��ǰ��  ��Ϊ�ڴ���ٶ�ԶԶ������ʾ��
void CLaoYangWnd::Update()
{
	static int oldTime=0,oldTime1 = 0;//��ʱ��
	int newTime=GetTickCount();//���ص�ǰʱ��
	if(newTime-oldTime<50)//50������һ��
		return;
	
	oldTime=newTime;//����ʱ��
	int i;								
	::FillRect(this->hbackdc,&this->clientRC,(HBRUSH)GetStockObject(WHITE_BRUSH));//��ɫˢ�Ӳ����ͻ�����
	::TextOutA(this->hbackdc,130,5,"��start.txt�ļ������ó�ʼ״̬,��F1��ȡF2��ʼ�ƶ�\n",48);	
	///////////////������//////////////////		
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
	//////////////��λͼ///////////////////
	BITMAP bm;//����λͼʵ��	
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
	char buffer[20];sprintf(buffer,"�㵱ǰ�Ѿ��ƶ���%d��,���Ͱ���",cools);
	::TextOutA(this->hbackdc,200,20,buffer,28);
	if(-1 == sumway)
		run = false;
	for(i = 0;i<9;i++)
	{
		::GetObject(this->PlayBitMap[start.a[i]],sizeof(bm),&bm);//��bm���������λͼ
		this->w = bm.bmWidth;this->h = bm.bmHeight;	//��ȡͼƬ�Ŀ�Ⱥ͸߶�
		::SelectObject(this->memDC,PlayBitMap[start.a[i]]);//�����λͼѡ����ǰ�����ݵĺ�̨DC(����λͼ)
		TransparentBlt(this->hbackdc,(i%3)*200,(i/3)*200+40,
			w,h,this->memDC,0,0,this->w,this->h,GetPixel(this->memDC,0,0));
	}
	//////////////��̨�豸��������ǰ������ʾ///////////////////////////
	::BitBlt(this->hdc,0,0,this->clientRC.right,this->clientRC.bottom,this->hbackdc,0,0,SRCCOPY);
}

void CLaoYangWnd::MessageLoop()
{
	MSG msg;
	memset(&msg,0,sizeof(msg));
	while(msg.message!=WM_QUIT)
	{
		this->Update();//����Ϣѭ���и��»���
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
	//������Դ
	ReleaseDC(hWnd,hdc);
	DeleteDC(hbackdc);
}



int CLaoYangWnd::kangtuo(char a[])
{
	int fac[]={1,1,2,6,24,120,720,5040,40320};//����չ������
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
		::MessageBox(::GetForegroundWindow(),"�Ҳ���start.txt�ļ�","������ʾ",0);
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
		::MessageBox(::GetForegroundWindow(),"�޷��ƶ�","������ʾ",0);
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
