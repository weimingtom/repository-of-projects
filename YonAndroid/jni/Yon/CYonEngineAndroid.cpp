#include "config.h"

#ifdef YON_COMPILE_WITH_ANDROID

#include "CYonEngineAndroid.h"
#include "CSceneManager.h"

#include "ILogger.h"

namespace yon{
	namespace platform{


		CYonEngineAndroid::CYonEngineAndroid(const yon::SYonEngineParameters& params):
			m_videoDriver(NULL),m_sceneManager(new scene::CSceneManager()),
			m_params(params),m_bClose(false),m_bResized(true)
		{
			//��ʼ����Ƶ������
			createDriver();
		}
		CYonEngineAndroid::~CYonEngineAndroid(){
			if(m_videoDriver!=NULL)
				m_videoDriver->drop();
			m_sceneManager->drop();
			Logger->info(YON_LOG_SUCCEED_FORMAT,"Destroy CYonEngineAndroid");
			if(Logger->drop()){
				Logger=NULL;
			}
		}

		bool CYonEngineAndroid::run(){
			if(!m_bClose)
				resizeIfNecessary();
			return !m_bClose;
		}
		void CYonEngineAndroid::onResize(u32 w,u32 h){
			m_bResized=true;
			m_params.windowSize.w=w;
			m_params.windowSize.h=h;
			Logger->debug("CYonEngineAndroid::onResize(%d,%d)",w,h);
		}

		void CYonEngineAndroid::resizeIfNecessary()
		{
			if (!m_bResized)
				return;

			m_videoDriver->onResize(m_params.windowSize);
			m_sceneManager->onResize(m_params.windowSize);
			m_bResized = false;
		}

		void CYonEngineAndroid::createDriver(){

#ifdef YON_VIDEO_MODE_OGLES1
			video::ogles1::SOGLES1Parameters params(m_params.windowSize);
			m_videoDriver=new video::ogles1::COGLES1Driver(params);
#endif //Yon_VIDEO_MODE_OGLES1
		}
		//yon::ITimer* yon::platform::CYonEngineAndroid::getTimer(){return NULL;}
	}//platform
}//yon

#endif//YON_COMPILE_WITH_ANDROID