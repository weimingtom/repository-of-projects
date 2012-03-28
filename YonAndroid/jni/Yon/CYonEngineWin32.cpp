#include "config.h"

#ifdef YON_COMPILE_WITH_WIN32

#include "CYonEngineWin32.h"

#include "ILogger.h"

namespace yon{
namespace platform{

	struct SEnginePair
	{
		HWND hWnd;
		CYonEngineWin32* engine;
	};
	core::list<SEnginePair> EngineMap;

	CYonEngineWin32* getEngineByHWnd(HWND hWnd)
	{
		core::list<SEnginePair>::Iterator it = EngineMap.begin();
		for (; it!= EngineMap.end(); ++it)
			if ((*it).hWnd == hWnd)
				return (*it).engine;

		return NULL;
	}

	
	CYonEngineWin32::CYonEngineWin32(const yon::SYonEngineParameters& params)
		:m_hWnd(NULL),m_bExternalWindow(false),
		m_pVideoDriver(NULL),m_pSceneManager(NULL),m_pFileSystem(NULL),
		m_params(params),m_bClose(false),m_bResized(false)
	{
		if(params.windowId==NULL)
		{
			//��������
			initWindow(params);
		}
		else
		{
			//�󶨵��ⲿ����
			m_hWnd = static_cast<HWND>(params.windowId);
			RECT r;
			GetWindowRect(m_hWnd, &r);
			m_params.windowSize.w = r.right - r.left;
			m_params.windowSize.h = r.bottom - r.top;
			m_bExternalWindow = true;
		}

		//��ʼ���ļ�ϵͳ
		m_pFileSystem=io::createFileSystem();

		//��ʼ����Ƶ������
		createDriver();

		//��ʼ������������
		m_pSceneManager=scene::createSceneManager();

		SEnginePair ep;
		ep.hWnd=m_hWnd;
		ep.engine=this;
		EngineMap.push_back(ep);
		Logger->debug("EngineMap.push_back(ep)\n");
		//ϵͳWM_SIZE��Ϣ�����ڽ�������push��EngineMap֮ǰ�Ѿ�������,Ϊ��ȷ��������,�����ٷ���һ��WM_SIZE
		PostMessage(m_hWnd,WM_SIZE,0,0);
	}
	CYonEngineWin32::~CYonEngineWin32(){
			m_pVideoDriver->drop();
			m_pSceneManager->drop();
			m_pFileSystem->drop();
			DestroyWindow(m_hWnd);
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Destroy Window");
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Destroy CYonEngineWin32");
		if(Logger->drop()){
			Logger=NULL;
		}
	}

		void CYonEngineWin32::onResize(u32 w,u32 h){
			m_bResized=true;
		}

	bool CYonEngineWin32::run(){
		MSG msg;
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				m_bClose=true;
			}
			else 
			{
				//TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if(!m_bClose)
			resizeIfNecessary();
		//Sleep(20);
		return !m_bClose;
	}

	void CYonEngineWin32::resizeIfNecessary()
	{
		if (!m_bResized)
			return;

		RECT r;
		GetClientRect(m_hWnd, &r);

		m_pVideoDriver->onResize(core::dimension2du((u32)r.right, (u32)r.bottom));
		m_pSceneManager->onResize(core::dimension2du((u32)r.right, (u32)r.bottom));
		m_bResized = false;
	}

	//
	//����: WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	//hWnd: handle to window ���ھ��
	//message: message identifier  ��Ϣ��ʶ
	//wParam: first message parameter
	//lParam: second message parameter
	//Ŀ��: ���������ڵ���Ϣ��
	//
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
		CYonEngineWin32* engine=NULL;
		switch(uiMsg) {
		case WM_CREATE:
			Logger->debug("WM_CREATE\n");
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_ACTIVATE:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SIZE:
			Logger->debug("WM_SIZE\n");
			engine=getEngineByHWnd(hWnd);
			if(engine){
				engine->onResize(0,0);
			}else{
				Logger->warn(YON_LOG_WARN_FORMAT,"getEngineByHWnd==NULL");
			}
			return 0;
		}
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}
	bool CYonEngineWin32::initWindow(const yon::SYonEngineParameters& params){

		WNDCLASS wc;
		RECT wRect;
		HINSTANCE hInstance;
		const c16* szWindowClass=TEXT("CYonPlatformWin32");

		wRect.left = 0L;
		wRect.right = (long)params.windowSize.w;
		wRect.top = 0L;
		wRect.bottom = (long)params.windowSize.h;

		//��GetModuleHandle()������õ�ǰ����ʵ�����
		hInstance = GetModuleHandle(NULL);

		//ʹ��|�Ѷ��ִ�����ʽ������һ�� 
		//���ڵ���ʽ   
		//CS_HREDRAW  ��ʾ��ˮƽ������ȷ����仯ʱ�ػ���������   
		//CS_VREDRAW  ��ʾ����ֱ����߶ȷ����仯ʱ�ػ���������   
		//CS_NOCLOSE  ����ϵͳ�˵��е�Close����,����û�йرհ�ť   
		//CS_DBLCLKS  ���û�˫�����ʱ�򴰿ڹ��̺����������˫����Ϣ
		//ͨ������������GetDC��BeginPaintʱ��Windows��Ĭ��ֵ����һ���µ��豸���ݣ�
		//��������������һ�иı����豸������ ReleaseDC��EndPaint�����ͷ�ʱ�����ᶪʧ��
		//��Ȼ��ͨ����������ַ����Ѿ������������ˣ���������������Ҫ���ͷ��豸����֮��
		//��Ȼ��������ж��豸�������������ĸı䣬�Ա�����һ�κ���GetDC ��BeginPaintʱ
		//������Ȼ�ܹ������á�Ϊ�ˣ����ڵ�¼�������ʱ����CS_OWNDC������봰������һ���֣�
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.cbClsExtra = 0;			//���������������û�����Windows�ڲ��ṩ����Ŀռ��Ա���������봰��
		wc.cbWndExtra = 0;			//ʵ��������ϵ,ͨ��������ռ�
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);		//���ڵ�ͼ��,���ΪNULL,ϵͳ���ṩһ��Ĭ�ϵ�ͼ��
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;		//�˵�,ָ���˵���Դ������,NULL�Ǳ�ʾû�в˵�,
		wc.lpszClassName = szWindowClass;

		//ע�ᴰ����
		RegisterClass(&wc);

		//�ú�����������ͻ����δ�С��������Ҫ�Ĵ��ھ��εĴ�С��������Ĵ��ھ��������Դ��͸�CreateWindowEx���������ڴ���һ���ͻ��������С�Ĵ��ڡ� 
		AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

		m_hWnd = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			szWindowClass,						//����������
			params.windowCaption.c_str(),		//���ڱ��� 
			WS_OVERLAPPEDWINDOW |				//������ʽ,������ʽ
												//�����ʽҪ��WNDCLASS����ʽ����,�����ָ��ĳ�����崰�ڵ���ʽ   
												//��WNDCLASS����ʽ��ָ���ڸô���������д��ڶ����е���ʽ   
												//WS_OVERLAPPED  һ���ɲ������   
												//WS_CAPTION     �б�����   
												//WS_SYSMENU     �ڱ���������ϵͳ�˵�,WS_CAPTIONһ��ʹ��   
												//WS_THICKFRAME  ���пɵ��߿򴰿�   
												//WS_MINIMIZEBOX ����С��ť,�����趨WS_SYSMENU   
												//WS_MAXIMIZEBOX �����ť,�����趨WS_SYSMENU
												//WS_CLIPCHILDREN��ʹ�ø������ڻ���ʱ�������ϵ��Ӵ����λ�ò�ȥ����������Ƭ���������Ӵ���ȥ����
												//WS_CLIPSIBLING�����������Ӵ��壬ʹ�ø��Ӵ������յ�WM_PAINTʱͬʱ���乲��������Ӵ��壨MSDN�����벻�ڸ����򣬼�����֮�ص���Ҳ�ᱻPAINT��
			WS_CLIPSIBLINGS | 
			WS_CLIPCHILDREN, 
			0, 0,								//x������y����
			wRect.right-wRect.left,				//��
			wRect.bottom-wRect.top,				//��
			NULL,								//������
			NULL,								//���ز˵�
			hInstance,							//����ʵ�����
			NULL								//���ڴ���ʱ���������ָ��
												//���ĵ�ʱ����ָ��CLIENTCREATESTRUCT
		);

		YON_DEBUG_BREAK_IF(!m_hWnd)

		if (!m_hWnd)
		{
			Logger->info(YON_LOG_FAILED_FORMAT,"Create window");
			return false;
		}

		ShowWindow(m_hWnd, SW_SHOW);
		SetForegroundWindow(m_hWnd);
		SetFocus(m_hWnd);

		return true;
	}

	void CYonEngineWin32::createDriver(){

#ifdef YON_VIDEO_MODE_OGLES1
			video::ogles1::SOGLES1Parameters params(m_hWnd,m_params.windowSize);
			m_pVideoDriver=new video::ogles1::COGLES1Driver(params,m_pFileSystem);
#endif //YON_VIDEO_MODE_OGLES1
	}
	//yon::ITimer* yon::platform::CYonEngineWin32::getTimer(){return NULL;}
}//namespace platform
}//namespace yon
#endif