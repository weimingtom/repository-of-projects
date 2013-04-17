#ifndef _YON_VIDEO_IVIDEODRIVER_H_
#define _YON_VIDEO_IVIDEODRIVER_H_

#include "IReferencable.h"
#include "dimension2d.h"
#include "position2d.h"
#include "rect.h"
#include "matrix4.h"
#include "IFileSystem.h"
#include "ITexture.h"
#include "IImageLoader.h"
//#include "IMaterial.h"
#include "SMaterial.h"
#include "SColor.h"
#include "ITimer.h"
#include "SVertex.h"
#include "yonArray.h"
#include "IResizable.h"
#include "ICursorControl.h"
#include "IEventReceiver.h"
#include "SRenderUnit.h"
#include "IHardwareBuffer.h"

namespace yon{

	namespace video{
		class IVideoDriver;
		class IMaterialRenderer;
	}

	namespace scene{
		class IShap;
		class ISceneManager;
		class IGeometryFactory;
		ISceneManager* createSceneManager(video::IVideoDriver* driver,scene::IGeometryFactory* geometryFactory,ITimer* timer,io::IFileSystem* fileSystem,platform::ICursorControl* cursorControl);
	}

	namespace video{

		static u16 COMMON_INDICES[]={0,1,3,3,1,2};

		IImageLoader* createImageLoaderPNG();
		IImageLoader* createImageLoaderTGA();
		IImageLoader* createImageLoaderDDS();
		IImageLoader* createImageLoaderPKM();
		IImageLoader* createImageLoaderKTX();
		IImageLoader* createImageLoaderPVR();

		enum ENUM_DRIVER_TYPE{
			ENUM_DRIVER_TYPE_OGLES1 = 0,
			ENUM_DRIVER_TYPE_OGLES2,
			ENUM_DRIVER_TYPE_COUNT
		};

		enum ENUM_TRANSFORM{
			ENUM_TRANSFORM_VIEW = 0,		//��ͼ�ռ����
			ENUM_TRANSFORM_WORLD,			//����ռ����
			ENUM_TRANSFORM_PROJECTION,		//ͶӰ�ռ����
			ENUM_TRANSFORM_TEXTURE0,		//����0����
			ENUM_TRANSFORM_TEXTURE1,		//����1����
			ENUM_TRANSFORM_COUNT
		};

		const static c8* TRANSFORM_NAMES[]={
			"View",
			"World",
			"Projection",
			"Texture0",
			"Texture1"
		};

		/*
		#define GL_POINTS                         0x0000
		#define GL_LINES                          0x0001
		#define GL_LINE_LOOP                      0x0002
		#define GL_LINE_STRIP                     0x0003
		#define GL_TRIANGLES                      0x0004
		#define GL_TRIANGLE_STRIP                 0x0005
		#define GL_TRIANGLE_FAN                   0x0006
		*/
		enum ENUM_PRIMITIVE_TYPE{
			ENUM_PRIMITIVE_TYPE_POINTS = 0x0000,
			ENUM_PRIMITIVE_TYPE_LINES = 0x0001,
			ENUM_PRIMITIVE_TYPE_TRIANGLES = 0x0004,
			ENUM_PRIMITIVE_TYPE_TRIANGLE_STRIP = 0x0005,
			ENUM_PRIMITIVE_TYPE_TRIANGLE_FAN =0x0006,
			ENUM_PRIMITIVE_TYPE_COUNT = 1
		};

		/*
		#define GL_BYTE                           0x1400
		#define GL_UNSIGNED_BYTE                  0x1401
		#define GL_SHORT                          0x1402
		#define GL_UNSIGNED_SHORT                 0x1403
		#define GL_FLOAT                          0x1406
		#define GL_FIXED                          0x140C
		*/
		enum ENUM_INDEX_TYPE{
			ENUM_INDEX_TYPE_8BIT = 0x1401,
			ENUM_INDEX_TYPE_16BIT = 0x1403,
			ENUM_INDEX_TYPE_COUNT = 2
		};

		enum ENUM_VIDEO_FEATURE{
			ENUM_VIDEO_FEATURE_FBO = 0,
			ENUM_VIDEO_FEATURE_ELEMENT_INDEX_UNSIGNED_INT,
			ENUM_VIDEO_FEATURE_TEXTURE_COMPRESSION_DXT1,
			ENUM_VIDEO_FEATURE_TEXTURE_COMPRESSION_ETC1,
			ENUM_VIDEO_FEATURE_TEXTURE_COMPRESSION_ATITC,
			ENUM_VIDEO_FEATURE_TEXTURE_COMPRESSION_PVRTC,
			//items below are not in GL_EXTENSIONS list,need be processed separately
			ENUM_VIDEO_FEATURE_GENERATE_MIPMAP,
			ENUM_VIDEO_FEATURE_COUNT
		};

		enum ENUM_RENDER_TARGET{
			ENUM_RENDER_TARGET_FRAME_BUFFER = 0,
			ENUM_RENDER_TARGET_RENDER_TEXTURE,

			ENUM_RENDER_TARGET_COUNT
		};

		enum ENUM_DRIVER_STATE{
			ENUM_DRIVER_STATE_NONE= 0,
			ENUM_DRIVER_STATE_RUN,
			ENUM_DRIVER_STATE_TO_SLEEP,
			ENUM_DRIVER_STATE_SLEEP,
			ENUM_DRIVER_STATE_TO_RUN,

			ENUM_DRIVER_STATE_COUNT
		};

		const static c8* DRIVER_STATE_NAME[]=
		{
			"NONE",
			"RUN",
			"TOSLEEP",
			"SLEEP",
			"TORUN"
		};

		struct SClearSetting{
			bool clearBackBuffer;
			bool clearZBuffer;
			video::SColor color;

			SClearSetting(bool backBuffer=true,bool zBuffer=true,video::SColor color=video::COLOR_BLACK)
				:clearBackBuffer(backBuffer),clearZBuffer(zBuffer),color(color){}
		};

		
		//��Ƶ�������ӿ�
		class IVideoDriver:public virtual core::IReferencable,public event::IEventReceiver{
		protected:
			enum ENUM_RENDER_MODE{
				ENUM_RENDER_MODE_NONE = 0,
				ENUM_RENDER_MODE_2D,
				ENUM_RENDER_MODE_3D,
				ENUM_RENDER_MODE_COUNT
			};
			core::matrix4f m_orthdowProjMatrix,m_orthdowViewMatrix;
			ENUM_RENDER_MODE m_renderMode;
			u32 m_textureCreationConfig;
			io::IFileSystem* m_pFileSystem;
			ITimer* m_pTimer;
			core::array<core::IResizable*> m_resizables;

			RenderUnit2DPool m_renderUnit2DPool;
			RenderUnit2DPool m_renderUnit2DBatchPool;
			core::array<u16> m_rectangleIndices;

			virtual video::ITexture* createDeviceDependentTexture(IImage* image, const io::path& name) = 0;
		public:
			IVideoDriver(io::IFileSystem* fs,ITimer* timer)
				:m_renderMode(ENUM_RENDER_MODE_NONE),m_textureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_NONE),m_pFileSystem(fs),m_pTimer(timer),
				m_orthdowProjMatrix(true),m_orthdowViewMatrix(true){
					if(m_pFileSystem)
						m_pFileSystem->grab();
					if(m_pTimer)
						m_pTimer->grab();
			}
			virtual ~IVideoDriver(){
				if(m_pFileSystem)
					m_pFileSystem->drop();
				if(m_pTimer)
					m_pTimer->drop();
				m_resizables.clear();
				m_renderUnit2DPool.clear();
				m_renderUnit2DBatchPool.clear();
			};
			const core::matrix4f& getOrthdowProjMatrix() const{
				return m_orthdowProjMatrix;
			}
			const core::matrix4f& getOrthdowViewMatrix() const{
				return m_orthdowViewMatrix;
			}
			virtual ENUM_DRIVER_STATE getState() const = 0;
			io::IFileSystem* getFileSystem() const{
				return m_pFileSystem;
			}
			ITimer* getTimer() const{
				return m_pTimer;
			}

			void registerResizable(core::IResizable* p){
				m_resizables.push_back(p);
			}

			RenderUnit2D* getRenderUnit2D(bool forBatch=false){
				RenderUnit2D* unit=NULL;
				if(forBatch)
				{
					unit=m_renderUnit2DBatchPool.get();
					unit->ForBatch=true;
					return unit;
				}
				else
				{
					unit=m_renderUnit2DPool.get();
					unit->ForBatch=false;
					return unit;
				}
			}
			void recycleRenderUnit2D(RenderUnit2D* unit){
				if(unit->isForBatch())
					m_renderUnit2DBatchPool.recycle(unit);
				else
					m_renderUnit2DPool.recycle(unit);
			}
			//deprecated û���ͨ���ԣ����Է���
			/*const core::array<u16>& getRectangleIndices(u32 verticeCount){
				YON_DEBUG_BREAK_IF(verticeCount%4!=0||verticeCount>=43690);
				u16 indiceCount=(verticeCount>>1)*3;
				u32 count=m_rectangleIndices.capacity();
				u32 vc=(count/3)<<1;
				if(count<indiceCount)
				{
					m_rectangleIndices.set_used(indiceCount);
					static u16 indices[]={0,1,3,3,1,2};
					for(u32 i=count;i<indiceCount;++i)
						m_rectangleIndices[i]=vc+indices[i%6];
				}
				else
					m_rectangleIndices.set_used(indiceCount);
				return m_rectangleIndices;
			}*/

			virtual s64 getVideoMemory() const = 0;
			virtual c8* getVideoMemoryString() const = 0;

			virtual const SClearSetting& getClearSetting() const = 0;
			virtual void begin(bool backBuffer=true,bool zBuffer=true,const video::SColor& color=video::COLOR_DEFAULT) = 0;
			virtual void end() = 0;
			virtual void setViewPort(const core::recti& r) = 0;
			virtual const core::dimension2di& getCurrentRenderTargetSize() const = 0;
			virtual void onResize(const core::dimension2du& size){
				for(u32 i=0;i<m_resizables.size();++i)
					m_resizables[i]->onResize(size);
			}

			//! Get the size of the screen or render window.
			/** \return Size of screen or render window. */
			virtual const core::dimension2di& getScreenSize() const= 0;


			//NOTE����ʹ��RTT�Ĺ����У����������Ҫupdate���������update������ڵ�ǰ֡����ʹ�ô�RTT�ĵ���֮ǰ���У��������ֿհ�RTT���������GPUָ����йأ�
			//virtual bool setRenderTarget(ENUM_RENDER_TARGET target, bool clearTarget=true, bool clearZBuffer=true, video::SColor color=video::COLOR_ZERO) =0;
			virtual void setRenderTarget(video::ITexture* texture,bool backBuffer=true, bool zBuffer=true, const video::SColor& color=video::COLOR_ZERO,bool partial=false,const core::recti& area=core::ZERO_RECTI) =0;


			//�Ƿ�֧��feature����
			virtual bool queryFeature(ENUM_VIDEO_FEATURE feature) const =0;

			//! Returns the maximum texture size supported.
			virtual core::dimension2du getMaxTextureSize() const =0;

			//��2D��XY����ϵ�������Ͻ�Ϊԭ��,��x���ң�y���£�תΪ3DXYY����ϵ������Ļ����Ϊԭ�㣬x���ң�y���ϣ�
			virtual void convertPosCoordinate(const core::position2di& src,core::position2df& dest) = 0;
			virtual void convertPosCoordinate(const core::position2di& src,core::position2di& dest) = 0;
			virtual void convertPosCoordinate(const core::position2df& src,core::position2df& dest) = 0;
			virtual void convertPosCoordinate(s32 srcX,s32 srcY,s32& destX,s32& destY) = 0;

			//������������
			void setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG config,bool on){
				m_textureCreationConfig = (m_textureCreationConfig & (~config)) |((((u32)!on)-1) & config);
			}
			bool getTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG config) const{
				return (m_textureCreationConfig&config)!=0;
			}

			//��ȡ��Ⱦ���������Ĭ����ɫ��������ΪNULL
			virtual ITexture* getRenderTarget() const = 0;
			//����һ��������Ⱦ���������,size������2��N����,���ҳ��ȶ����ɳ����󻺳����Ĵ�С
			//@param autoOffScreen : true,use off screen render texture if possible; otherwise use on screen one imperatively
			virtual ITexture* addRenderTargetTexture(const core::dimension2du& size,const io::path& name = "rtt", video::ENUM_COLOR_FORMAT format = video::ENUM_COLOR_FORMAT_R5G6B5, bool autoOffScreen=true, bool fixSize=true) =0;
			//����texture:��������addRenderTargetTexture���ɵ�texture
			//����color:render target�ı���ɫ
			//virtual bool setRenderTarget(video::ITexture* texture,
			//	bool clearBackBuffer=true, bool clearZBuffer=true,video::SColor color=video::COLOR_BLACK) =0;


			virtual IImage* createImageFromFile(const io::path& filename,bool translateIntoGray=false) = 0;
			virtual IImage* createImageFromFile(io::IReadStream* file,bool translateIntoGray=false) =0;

			//NOTE:RESERVE/MIPMAP��Ҫ�Լ�����
			virtual ITexture* addTexture(const core::dimension2du& size,const io::path& name, ENUM_COLOR_FORMAT format = ENUM_COLOR_FORMAT_R8G8B8A8) = 0;
			virtual bool setTexture(u32 stage, const video::ITexture* texture) = 0;
			virtual ITexture* getTexture(const io::path& filename) = 0;
			virtual video::ITexture* findTexture(const io::path& filename) = 0;
			virtual void removeTexture(ITexture* texture) =0;
			virtual void grabTexture(ITexture* texture) = 0;
			virtual bool dropTexture(ITexture* texture) = 0;
			virtual bool dropReserved(ITexture* texture) = 0;

			virtual s32 addMaterialRenderer(IMaterialRenderer* renderer, const c8* name =NULL,bool addReference=true) =0;

			virtual void setTransform(ENUM_TRANSFORM transform, const core::matrix4f& mat) =0;
			virtual const core::matrix4f& getTransform(ENUM_TRANSFORM transform) const =0;

			virtual void setMaterial(const SMaterial& material) = 0;
			//virtual void setMaterial(IMaterial* material) = 0;
			//virtual IMaterial* createMaterial() = 0;

			//virtual void drawUnit(scene::IUnit* unit,bool force2D=false) = 0;
			virtual void drawShap(scene::IShap* shap,bool force2D=false) = 0;
			virtual void draw2DImage(const video::ITexture* texture, const core::position2di& destPos,
				const core::recti& sourceRect, const core::recti* clipRect =NULL,
				video::SColor color=video::COLOR_WHITE, bool useAlphaChannelOfTexture=false) =0;
			virtual void drawVertexPrimitiveList(const void* const vertices, u32 vertexCount,
				const void* const indice, u32 indexCount,
				ENUM_PRIMITIVE_TYPE pType=ENUM_PRIMITIVE_TYPE_TRIANGLES,
				scene::ENUM_VERTEX_TYPE vType=scene::ENUM_VERTEX_TYPE_3V1T1C,
				bool force2D=false) =0;
			virtual void draw2DLine(const core::position2di& start,const core::position2di& end,video::SColor color=video::COLOR_WHITE,f32 lineWidth=1.0f) =0;
			virtual void draw3DLine(const core::vector3df& start,const core::vector3df& end, video::SColor color=video::COLOR_WHITE,f32 lineWidth=1.0f) =0;
			virtual u32 getFPS() const = 0;
			virtual u32 getPrimitiveCountDrawn(bool average=false) const = 0;
			virtual u32 getDrawCall() const = 0;
			virtual u32 getTextureChange() const = 0;
		};
	}
}
#endif