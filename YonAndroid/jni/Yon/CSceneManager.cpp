#include "CSceneManager.h"
#include "CGeometryFactory.h"
#include "CModel.h"
#include "COrthoCamera.h"
#include "CPerspCamera.h"

#include "ILogger.h"

namespace yon{
namespace scene{

	CSceneManager::CSceneManager()
			:m_geometryFactory(new CGeometryFactory()),m_activeCamera(NULL),
		IModel(NULL){
			Logger->info(YON_LOG_SUCCEED_FORMAT,"Instance CSceneManager");
	}

	CSceneManager::~CSceneManager(){
		m_geometryFactory->drop();

		for(u32 i=0;i<m_cameras.size();++i){
			m_cameras[i]->drop();
		}

		if (m_activeCamera)
			m_activeCamera->drop();
		m_activeCamera = NULL;

		Logger->info(YON_LOG_SUCCEED_FORMAT,"Release CSceneManager");
	}

	IModel* CSceneManager::addModel(IEntity* entity){
		CModel* model=new CModel(this,entity);
		addChild(model);
		model->drop();

		Logger->debug("CSceneManager::addModel size:%d\n",m_children.size());
		return model;
	}

	camera::ICamera* CSceneManager::addCamera(const core::vector3df& pos,const core::vector3df& up,
		const core::vector3df& lookat,bool makeActive){
			camera::COrthoCamera* camera=new camera::COrthoCamera(pos,up,lookat);
			//camera::CPerspCamera* camera=new camera::CPerspCamera(pos,up,lookat);
			m_cameras.push(camera);
			if(makeActive){
				setActiveCamera(camera);
			}
			Logger->debug("CSceneManager::addCamera size:%d\n",m_cameras.size());
			return camera;
	}
	void CSceneManager::setActiveCamera(camera::ICamera* camera){
		if(camera){
			camera->grab();
		}
		if(m_activeCamera){
			m_activeCamera->drop();
		}
		m_activeCamera=camera;
	}

	void CSceneManager::render(video::IVideoDriver* driver){
		YON_DEBUG_BREAK_IF(m_activeCamera==NULL);

		m_activeCamera->render(driver);

		core::list<IModel*>::Iterator it = m_children.begin();
		for (; it != m_children.end(); ++it)
		{
			(*it)->render(driver);
		}
	}

	void CSceneManager::onResize(const core::dimension2du& size){
		for(u32 i=0;i<m_cameras.size();++i){
			m_cameras[i]->onResize(size);
		}
	}

	const IGeometryFactory* CSceneManager::getGeometryFactory() const{
		return m_geometryFactory;
	}

	ISceneManager* createSceneManager(){
		return new CSceneManager();
	}
}//scene
}//yon