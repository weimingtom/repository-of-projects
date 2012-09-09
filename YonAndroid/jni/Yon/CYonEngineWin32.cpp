#include "yonConfig.h"

#ifdef YON_COMPILE_WITH_WIN32

#include "CYonEngineWin32.h"
#include "CCursorControlWin32.h"
#include "CGraphicsAdapter.h"
#include "CGraphicsAdapterWindow.h"
#include "CRandomizer.h"
#include "yonList.h"
#include "ILogger.h"
#include "exception.h"
#include "CI18NManager.h"

namespace yon{
namespace platform{

	struct SEnginePair
	{
		HWND hWnd;
		CYonEngineWin32* engine;
	};
	core::list<SEnginePair> EngineMap;
	HKL KEYBOARD_INPUT_HKL=0;
	u32 KEYBOARD_INPUT_CODEPAGE = 1252;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
	long WINAPI onUnhandledException(EXCEPTION_POINTERS* lpExceptionInfo);

	CYonEngineWin32* getEngineByHWnd(HWND hWnd)
	{
		core::list<SEnginePair>::Iterator it = EngineMap.begin();
		for (; it!= EngineMap.end(); ++it)
			if ((*it).hWnd == hWnd)
				return (*it).engine;

		return NULL;
	}
	void eraseEngineByHWnd(HWND hWnd)
	{
		core::list<SEnginePair>::Iterator it = EngineMap.begin();
		for (; it!= EngineMap.end(); ++it)
		{
			if ((*it).hWnd == hWnd)
			{
				EngineMap.erase(it);
				break;
			}
		}
	}

	const wchar_t* szWindowClass=TEXT("CYonPlatformWin32");
	
	CYonEngineWin32::CYonEngineWin32(const yon::SYonEngineParameters& params)
		:m_hWnd(NULL),m_bExternalWindow(false),m_bAvailable(false),
		m_pVideoDriver(NULL),m_pSceneManager(NULL),
		m_pAudioDriver(NULL),m_pRandomizer(NULL),
		m_pGraphicsAdapter(NULL),m_pFileSystem(NULL),m_pI18nManager(NULL),m_pGraphicsAdapterWindow(NULL),
		m_pUserListener(params.pEventReceiver),m_pTimer(NULL),m_pCallback(params.pCallback),
		m_params(params),m_bClose(false),m_bResized(false),m_pCursorControl(NULL)
	{
		SetUnhandledExceptionFilter(onUnhandledException);

		if(params.windowId==NULL)
		{
			//创建窗口
			initWindow(params);
			m_params.windowId=m_hWnd;
		}
		else
		{
			//绑定到外部窗口
			m_hWnd = static_cast<HWND>(params.windowId);
			RECT r;
			GetWindowRect(m_hWnd, &r);
			m_params.windowSize.w = r.right - r.left;
			m_params.windowSize.h = r.bottom - r.top;
			m_bExternalWindow = true;
		}

		//初始化鼠标控制器
		m_pCursorControl=new CCursorControlWin32(m_params.windowSize,m_hWnd,false);

		//初始化随机生成器
		m_pRandomizer=createRandomizer();

		//初始化计时器
		m_pTimer=createTimer();

		//初始化文件系统
		m_pFileSystem=io::createFileSystem();

		//初始化国际化模块
		m_pI18nManager=i18n::createI18NManager();

		//初始化场景管理器
		m_pSceneManager=scene::createSceneManager(m_pTimer,m_pCursorControl);

		//初始化视频驱动器
		createDriver();

		//初始化Graphics适配器
		m_pGraphicsAdapter=scene::createGraphicsAdapter(m_pVideoDriver,m_pSceneManager);
		m_pGraphicsAdapterWindow=scene::createGraphicsAdapterWindow(m_pVideoDriver,m_pSceneManager);

		//初始化声音驱动器
		m_pAudioDriver=audio::createAudioDriver(m_pFileSystem);

		SEnginePair ep;
		ep.hWnd=m_hWnd;
		ep.engine=this;
		EngineMap.push_back(ep);
		Logger->debug("EngineMap.push_back(ep)\n");
		//系统WM_SIZE消息可能在将本对象push进EngineMap之前已经处理完,为了确保完整性,这里再发送一次WM_SIZE
		PostMessage(m_hWnd,WM_SIZE,0,0);

		SetActiveWindow(m_hWnd);
		SetForegroundWindow(m_hWnd);

		//启动计时器
		m_pTimer->start();

		if(m_pVideoDriver->getState()==video::ENUM_DRIVER_STATE_RUN)
			m_bAvailable=true;
	}
	CYonEngineWin32::~CYonEngineWin32(){
		eraseEngineByHWnd(m_hWnd);
		m_pAudioDriver->drop();
		m_pGraphicsAdapterWindow->drop();
		m_pGraphicsAdapter->drop();
		m_pVideoDriver->drop();
		m_pSceneManager->drop();
		m_pI18nManager->drop();
		m_pFileSystem->drop();
		m_pTimer->drop();
		m_pRandomizer->drop();
		m_pCursorControl->drop();
		if(m_bExternalWindow==false){
			DestroyWindow(m_hWnd);
			Logger->info(YON_LOG_SUCCEED_FORMAT,"Destroy Window");
		}
		if(UnregisterClass(szWindowClass, GetModuleHandle(NULL)))
			Logger->info(YON_LOG_SUCCEED_FORMAT,"UnregisterClass");
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Destroy CYonEngineWin32");
		if(video::DEFAULT_MATERIAL->drop()){
			video::DEFAULT_MATERIAL=NULL;
		}
		if(video::MYGUI_MATERIAL->drop()){
			video::MYGUI_MATERIAL=NULL;
		}
		if(Logger->drop()){
			Logger=NULL;
		}
			
	}

	void CYonEngineWin32::onResize(u32 w,u32 h){
		m_bResized=true;
	}

	bool CYonEngineWin32::run(){
		m_pTimer->tick();
		if(!m_bExternalWindow)
		{
			MSG msg;
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				/*if(msg.message == WM_QUIT)
				{
				m_bClose=true;
				}
				else 
				{
				//TranslateMessage(&msg);
				DispatchMessage(&msg);
				}*/

				/*if(msg.message==WM_DESTROY){
				Logger->debug("PeekMessage WM_DESTROY\n");
				}else if(msg.message==WM_QUIT){
				Logger->debug("PeekMessage WM_QUIT\n");
				}*/
				if (m_bExternalWindow && msg.hwnd == m_hWnd)
					WndProc(m_hWnd, msg.message, msg.wParam, msg.lParam);
				else
					DispatchMessage(&msg);

				if(msg.message == WM_QUIT)
				{
					m_bClose=true;
				}
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

		if (r.right < 2 || r.bottom < 2)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("Ignoring resize operation to (%ld %ld)", r.right, r.bottom).c_str());
		}
		else
		{
			core::dimension2du newsize((u32)r.right, (u32)r.bottom);
			//m_pVideoDriver->onResize(newsize);
			//m_pSceneManager->onResize(newsize);

			event::SEvent evt;
			evt.type=event::ENUM_EVENT_TYPE_SYSTEM;
			evt.systemInput.type=event::ENUM_SYSTEM_INPUT_TYPE_RESIZE;
			evt.systemInput.screenWidth=newsize.w;
			evt.systemInput.screenHeight=newsize.h;
			postEventFromUser(evt);
		}
		m_bResized = false;
	}

	bool CYonEngineWin32::callback(const platform::SCallback& cb){
		bool absorbed = false;
		if(m_pCallback)
			absorbed=m_pCallback->callback(cb);
		return absorbed;
	}

	bool CYonEngineWin32::postEventFromUser(const event::SEvent& evt){

		//Logger->debug("post\r\n");
		if(evt.type==event::ENUM_EVENT_TYPE_SYSTEM&&evt.systemInput.type==event::ENUM_SYSTEM_INPUT_TYPE_CRASH)
		{
			m_bClose=true;
			Logger->error(YON_LOG_FAILED_FORMAT,"Encounter unhandled exception,program will exit!");
			return true;
		}

		bool absorbed = false;
		bool penetrability=evt.type==event::ENUM_EVENT_TYPE_SYSTEM;

		if(penetrability)
		{
			absorbed=m_pVideoDriver->onEvent(evt)||absorbed;
			absorbed=m_pAudioDriver->onEvent(evt)||absorbed;
			if(m_pUserListener)
				absorbed=m_pUserListener->onEvent(evt)||absorbed;
			absorbed=m_pSceneManager->onEvent(evt)||absorbed;
		}
		else
		{
			absorbed=m_pVideoDriver->onEvent(evt);
			if(absorbed)
				return absorbed;
			absorbed=m_pAudioDriver->onEvent(evt);
			if(absorbed)
				return absorbed;
			if (m_pUserListener)
				absorbed = m_pUserListener->onEvent(evt);
			if(absorbed)
				return absorbed;
			absorbed = m_pSceneManager->onEvent(evt);
		}
	
		//deprecated
		/*
		m_pVideoDriver->onEvent(evt);
		m_pAudioDriver->onEvent(evt);

		if (m_pUserListener)
			absorbed = m_pUserListener->onEvent(evt);

		//TODO GUI
		//if (!absorbed && GUIEnvironment)
		//	absorbed = GUIEnvironment->postEventFromUser(event);

		if (!absorbed && m_pSceneManager)
			absorbed = m_pSceneManager->postEventFromUser(evt);
		*/

		return absorbed;
	}

	//不能使用成员函数声明：
	//Obviously SetUnhandledExceptionFilter wait a pointer to ordinar function (exception handler). 
	//But non-static member function is not an ordinar function and a pointer to it is not a pointer to ordinar function. 
	//Make it a static mamber (if possible): static member functions treated as ordinar.
	//http://www.daniweb.com/software-development/cpp/threads/155970/setunhandledexceptionfilter-problem#
	long WINAPI onUnhandledException(EXCEPTION_POINTERS* lpExceptionInfo){
		event::SEvent evt;
		evt.type=event::ENUM_EVENT_TYPE_SYSTEM;
		evt.systemInput.type=event::ENUM_SYSTEM_INPUT_TYPE_CRASH;
		core::list<SEnginePair>::Iterator it = EngineMap.begin();
		for (; it!= EngineMap.end(); ++it)
			(*it).engine->postEventFromUser(evt);
		MessageBox(NULL,TEXT("Encounter unhandled exception,program will exit!"),TEXT("Error"),MB_OK);
		throw core::exception("Encounter unhandled exception,program will exit!)");
		return EXCEPTION_EXECUTE_HANDLER;
	}
	//
	//函数: WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	//hWnd: handle to window 窗口句柄
	//message: message identifier  消息标识
	//wParam: first message parameter
	//lParam: second message parameter
	//目的: 处理主窗口的消息。
	//
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {

		event::SEvent evt;

		static s32 clickCount=0;
		if (GetCapture() != hWnd && clickCount > 0)
			clickCount = 0;

		struct MessageMap
		{
			s32 group;
			UINT winMessage;
			s32 engineMessage;
		};

		static MessageMap mouseMap[] =
		{
			{0, WM_LBUTTONDOWN, event::ENUM_MOUSE_INPUT_TYPE_LDOWN},
			{1, WM_LBUTTONUP,   event::ENUM_MOUSE_INPUT_TYPE_LUP},
			{0, WM_RBUTTONDOWN, event::ENUM_MOUSE_INPUT_TYPE_RDOWN},
			{1, WM_RBUTTONUP,   event::ENUM_MOUSE_INPUT_TYPE_RUP},
			{0, WM_MBUTTONDOWN, event::ENUM_MOUSE_INPUT_TYPE_MDOWN},
			{1, WM_MBUTTONUP,   event::ENUM_MOUSE_INPUT_TYPE_MUP},
			{2, WM_MOUSEMOVE,   event::ENUM_MOUSE_INPUT_TYPE_MOVE},
			{3, WM_MOUSEWHEEL,  event::ENUM_MOUSE_INPUT_TYPE_WHEEL},
			{-1, 0, 0}
		};

		MessageMap * m = mouseMap;
		while ( m->group >=0 && m->winMessage != uiMsg )
			m += 1;
		if ( m->group >= 0 )
		{
			if ( m->group == 0 )	// down
			{
				clickCount++;
				SetCapture(hWnd);
			}
			else
			{
				if ( m->group == 1 )	// up
				{
					clickCount--;
					if (clickCount<1)
					{
						clickCount=0;
						ReleaseCapture();
					}
				}
			}
			evt.type=event::ENUM_EVENT_TYPE_MOUSE;
			evt.mouseInput.type = (event::ENUM_MOUSE_INPUT_TYPE) m->engineMessage;
			evt.mouseInput.x = (short)LOWORD(lParam);
			evt.mouseInput.y = (short)HIWORD(lParam);
			evt.mouseInput.buttonMasks = wParam & ( MK_LBUTTON | MK_RBUTTON | MK_MBUTTON);
			evt.mouseInput.wheel = 0.f;

			// wheel
			if ( m->group == 3 )
			{
				POINT p;
				p.x = 0; p.y = 0;
				ClientToScreen(hWnd, &p);
				evt.mouseInput.x -= p.x;
				evt.mouseInput.y -= p.y;
				evt.mouseInput.wheel = ((f32)((short)HIWORD(wParam))) / (f32)WHEEL_DELTA;
			}

			IYonEngine* engine = getEngineByHWnd(hWnd);
			if (engine)
			{
				engine->postEventFromUser(evt);

				//TODO 双击事件/三击事件
			}
			return 0;
		}


		CYonEngineWin32* engine=NULL;
		switch(uiMsg) {
		case WM_PAINT:
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0;
		case WM_ERASEBKGND:
			return 0;
		case WM_CREATE:
			Logger->debug("WM_CREATE\n");
			break;
		case WM_DESTROY:
			Logger->debug("WM_DESTROY\n");
			PostQuitMessage(0);
			return 0;
		case WM_ACTIVATE:
			return 0;
		/*case WM_SYSKEYDOWN:
			Logger->debug("WM_SYSKEYDOWN:%c\n",wParam);
			return 0;
		case WM_SYSKEYUP:
			Logger->debug("WM_SYSKEYUP:%c\n",wParam);
			return 0;
		case WM_KEYDOWN:
			Logger->debug("WM_KEYDOWN:%c(%d)\n",wParam,wParam);
			return 0;
		case WM_KEYUP:
			Logger->debug("WM_KEYUP:%c(%d)\n",wParam,wParam);
			return 0;*/
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
			{
				BYTE allKeys[256];
				evt.type=event::ENUM_EVENT_TYPE_KEY;
				evt.keyInput.key=(event::ENUM_KEY)wParam;
				evt.keyInput.pressDown = (uiMsg==WM_KEYDOWN || uiMsg == WM_SYSKEYDOWN);

				const UINT MY_MAPVK_VSC_TO_VK_EX = 3; // MAPVK_VSC_TO_VK_EX should be in SDK according to MSDN, but isn't in mine.
				if(evt.keyInput.key==event::ENUM_KEY_SHIFT)
				{
					// this will fail on systems before windows NT/2000/XP, not sure _what_ will return there instead.
					evt.keyInput.key = (event::ENUM_KEY)MapVirtualKey( ((lParam>>16) & 255), MY_MAPVK_VSC_TO_VK_EX );
					//lshift==>160 rshift==>161
				}
				if(evt.keyInput.key==event::ENUM_KEY_CONTROL)
				{
					evt.keyInput.key = (event::ENUM_KEY)MapVirtualKey( ((lParam>>16) & 255), MY_MAPVK_VSC_TO_VK_EX );
					// some keyboards will just return LEFT for both - left and right keys. So also check extend bit.
					if (lParam & 0x1000000)
						evt.keyInput.key = event::ENUM_KEY_RCONTROL;
					//lcontrol==>162 rcontrol==>163
				}
				if (evt.keyInput.key==event::ENUM_KEY_MENU)
				{
					evt.keyInput.key = (event::ENUM_KEY)MapVirtualKey( ((lParam>>16) & 255), MY_MAPVK_VSC_TO_VK_EX );
					if (lParam & 0x1000000)
						evt.keyInput.key = event::ENUM_KEY_RMENU;
					//lalt==>164 ralt==>165
					//Logger->debug("%d\n",evt.keyInput.key);
				}

				GetKeyboardState(allKeys);

				evt.keyInput.shiftPressed = ((allKeys[VK_SHIFT] & 0x80)!=0);
				evt.keyInput.controlPressed = ((allKeys[VK_CONTROL] & 0x80)!=0);
				evt.keyInput.alternatePressed = ((allKeys[VK_MENU] & 0x80)!=0);

				//Logger->debug("s:%d,c:%d,a:%d  %d\n",evt.keyInput.shiftPressed,evt.keyInput.controlPressed,evt.keyInput.alternatePressed,evt.keyInput.key);

				// Handle unicode and deadkeys in a way that works since Windows 95 and nt4.0
				// Using ToUnicode instead would be shorter, but would to my knowledge not run on 95 and 98.
				WORD keyChars[2];
				UINT scanCode = HIWORD(lParam);
				int conversionResult = ToAsciiEx(wParam,scanCode,allKeys,keyChars,0,KEYBOARD_INPUT_HKL);
				if (conversionResult == 1)
				{
					WORD unicodeChar;
					MultiByteToWideChar(
						KEYBOARD_INPUT_CODEPAGE,
						MB_PRECOMPOSED, // default
						(LPCSTR)keyChars,
						sizeof(keyChars),
						(WCHAR*)&unicodeChar,
						1 );
					evt.keyInput.character = unicodeChar;
				}
				else
					evt.keyInput.character = 0;

				IYonEngine* engine = getEngineByHWnd(hWnd);
				if (engine)
					engine->postEventFromUser(evt);

				if (uiMsg == WM_SYSKEYDOWN || uiMsg == WM_SYSKEYUP)
					return DefWindowProc(hWnd, uiMsg, wParam, lParam);
				else
					return 0;
				return 0;
			}
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
		

		wRect.left = 0L;
		wRect.right = (long)params.windowSize.w;
		wRect.top = 0L;
		wRect.bottom = (long)params.windowSize.h;

		//用GetModuleHandle()函数获得当前程序实例句柄
		hInstance = GetModuleHandle(NULL);

		//使用|把多种窗口样式连接在一起 
		//窗口的样式   
		//CS_HREDRAW  表示当水平方向宽度发生变化时重绘整个窗口   
		//CS_VREDRAW  表示当垂直方向高度发生变化时重绘整个窗口   
		//CS_NOCLOSE  禁用系统菜单中的Close命令,既是没有关闭按钮   
		//CS_DBLCLKS  当用户双击鼠标时向窗口过程函数发送鼠标双击消息
		//通常，在您呼叫GetDC或BeginPaint时，Windows用默认值建立一个新的设备内容，
		//您对属性所做的一切改变在设备内容用 ReleaseDC或EndPaint呼叫释放时，都会丢失。
		//虽然在通常情况下这种方法已经很令人满意了，但是您还可能想要在释放设备内容之后，
		//仍然保存程序中对设备内容属性所做的改变，以便在下一次呼叫GetDC 和BeginPaint时
		//它们仍然能够起作用。为此，可在登录窗口类别时，将CS_OWNDC旗标纳入窗口类别的一部分：
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.cbClsExtra = 0;			//这两个变量允许用户请求Windows内部提供额外的空间以便额外数据与窗口
		wc.cbWndExtra = 0;			//实例发生联系,通常不分配空间
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);		//窗口的图标,如果为NULL,系统将提供一个默认的图标
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;		//菜单,指定菜单资源的名字,NULL是表示没有菜单,
		wc.lpszClassName = szWindowClass;

		//注册窗口类
		RegisterClass(&wc);

		//该函数依据所需客户矩形大小，计算需要的窗口矩形的大小。计算出的窗口矩形随后可以传送给CreateWindowEx函数，用于创建一个客户区所需大小的窗口。 
		AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

		m_hWnd = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			szWindowClass,						//窗口类名称
			params.windowCaption.c_str(),		//窗口标题 
			WS_OVERLAPPEDWINDOW |				//窗口样式,多种样式
												//这个样式要与WNDCLASS的样式区别开,这个是指定某个具体窗口的样式   
												//而WNDCLASS的样式是指基于该窗口类的所有窗口都具有的样式   
												//WS_OVERLAPPED  一个可层叠窗口   
												//WS_CAPTION     有标题栏   
												//WS_SYSMENU     在标题栏带有系统菜单,WS_CAPTION一起使用   
												//WS_THICKFRAME  具有可调边框窗口   
												//WS_MINIMIZEBOX 有最小按钮,必须设定WS_SYSMENU   
												//WS_MAXIMIZEBOX 有最大按钮,必须设定WS_SYSMENU
												//WS_CLIPCHILDREN，使得父窗体在绘制时留出其上的子窗体的位置不去画它，而那片区域留着子窗体去画。
												//WS_CLIPSIBLING，必须用于子窗体，使得该子窗体在收到WM_PAINT时同时令其共父的诸多子窗体（MSDN讲必须不在该区域，即不与之重叠）也会被PAINT。
			WS_CLIPSIBLINGS | 
			WS_CLIPCHILDREN, 
			0, 0,								//x坐标与y坐标
			wRect.right-wRect.left,				//宽
			wRect.bottom-wRect.top,				//高
			NULL,								//父窗口
			NULL,								//加载菜单
			hInstance,							//窗口实例标记
			NULL								//窗口创建时传入的数据指针
												//多文档时必须指向CLIENTCREATESTRUCT
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
			video::ogles1::SOGLES1Parameters params(m_hWnd,m_params.windowSize,m_params.fpsLimit);
			m_pVideoDriver=new video::ogles1::COGLES1Driver(params,m_pFileSystem,m_pTimer,m_pSceneManager->getGeometryFactory());
#endif //YON_VIDEO_MODE_OGLES1
	}
	//yon::ITimer* yon::platform::CYonEngineWin32::getTimer(){return NULL;}
}//namespace platform
}//namespace yon
#endif
