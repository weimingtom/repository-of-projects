#ifndef _YON_VIDEO_CMATERIAL_H_
#define _YON_VIDEO_CMATERIAL_H_

#include "IMaterial.h"
#include "SMaterialLayer.h"

#include "ILogger.h"

namespace yon{
namespace video{

	//TODO ��ΪSMaterial
	class CMaterial : public IMaterial{
	protected:
		ENUM_MATERIAL_TYPE m_materialType;
		ENUM_POLYGON_MODE m_polygonMode;
		ENUM_CULLING_MODE m_cullingMode;
		ENUM_FRONT_FACE m_frontFace;
		ENUM_BLEND_FACTOR m_blendSrc,m_blendDst;
		ENUM_MODULATE m_modulate;
		ENUM_ALPHA_SOURCE m_alphaSource;
		bool m_bChanged;
		SMaterialLayer m_textureLayers[MATERIAL_MAX_TEXTURES];
	public:

		CMaterial()
			:m_materialType(ENUM_MATERIAL_TYPE_SOLID),m_polygonMode(ENUM_POLYGON_MODE_FILL),
			m_cullingMode(ENUM_CULLING_MODE_BACK),m_frontFace(ENUM_FRONT_FACE_CCW),
			m_blendSrc(ENUM_BLEND_FACTOR_SRC_ALPHA),m_blendDst(ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA),
			m_modulate(ENUM_MODULATE_1X),m_alphaSource(ENUM_ALPHA_SOURCE_NONE),m_bChanged(true)
		{
				states.CullFace=true;
				//Logger->setAppender(MASK_APPENDER_VS);
				//Logger->debug(YON_LOG_SUCCEED_FORMAT,"Instance CMaterial");
				//Logger->setAppender(MASK_APPENDER_FILE|MASK_APPENDER_VS);
		}
		CMaterial(ENUM_MATERIAL_TYPE materialType)
			:m_materialType(materialType),m_polygonMode(ENUM_POLYGON_MODE_FILL),
			m_cullingMode(ENUM_CULLING_MODE_BACK),m_frontFace(ENUM_FRONT_FACE_CCW),
			m_blendSrc(ENUM_BLEND_FACTOR_SRC_ALPHA),m_blendDst(ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA),
			m_modulate(ENUM_MODULATE_1X),m_alphaSource(ENUM_ALPHA_SOURCE_NONE){
				//Logger->debug(YON_LOG_SUCCEED_FORMAT,"Instance CMaterial");
		}
		~CMaterial(){
			//Logger->setAppender(MASK_APPENDER_VS);
			//Logger->debug(YON_LOG_SUCCEED_FORMAT,"Release CMaterial");
			//Logger->setAppender(MASK_APPENDER_FILE|MASK_APPENDER_VS);
		}

		virtual ENUM_MATERIAL_TYPE getMaterialType() const{
			return m_materialType;
		}
		virtual void setMaterialType(ENUM_MATERIAL_TYPE type){
			m_materialType=type;
			m_bChanged=true;
		}

		virtual ENUM_POLYGON_MODE getPolygonMode() const{
			return m_polygonMode;
		}
		virtual void setPolygonMode(ENUM_POLYGON_MODE mode){
			m_polygonMode= mode;
			m_bChanged=true;
		}

		virtual void setTexture(u32 i, ITexture* tex)
		{
			if (i>=MATERIAL_MAX_TEXTURES)
				return;
			m_textureLayers[i].texture = tex;
			m_bChanged=true;
		}
		ITexture* getTexture(u32 i) const
		{
			return i < MATERIAL_MAX_TEXTURES ? m_textureLayers[i].texture : 0;
		}

		virtual void setCullingMode(ENUM_CULLING_MODE mode){
			m_cullingMode=mode;
			m_bChanged=true;
		}
		virtual ENUM_CULLING_MODE getCullingMode() const{
			return m_cullingMode;
		}

		virtual void setFrontFace(ENUM_FRONT_FACE f){
			m_frontFace=f;
			m_bChanged=true;
		}
		virtual ENUM_FRONT_FACE getFrontFace() const{
			return m_frontFace;
		}

		virtual ENUM_WRAP_MODE getWrapModeU(u32 index) const{
			return m_textureLayers[index].wrapU;
		}
		virtual void setWrapModeU(u32 index,ENUM_WRAP_MODE mode){
			m_textureLayers[index].wrapU=mode;
			m_bChanged=true;
		}
		virtual ENUM_WRAP_MODE getWrapModeV(u32 index) const{
			return m_textureLayers[index].wrapV;
		}
		virtual void setWrapModeV(u32 index,ENUM_WRAP_MODE mode){
			m_textureLayers[index].wrapV=mode;
			m_bChanged=true;
		}

		virtual ENUM_FILTER_MODE getFilterMode(u32 index) const{
			return m_textureLayers[index].filter;
		}
		virtual void setFilterMode(u32 index,ENUM_FILTER_MODE mode){
			m_textureLayers[index].filter=mode;
			m_bChanged=true;
		}

		virtual ENUM_BLEND_FACTOR getBlendSrcFactor() const{
			return m_blendSrc;
		}
		virtual void setBlendSrcFactor(ENUM_BLEND_FACTOR factor){
			m_blendSrc=factor;
			m_bChanged=true;
		}
		virtual ENUM_BLEND_FACTOR getBlendDstFactor() const{
			return m_blendDst;
		}
		virtual void setBlendDstFactor(ENUM_BLEND_FACTOR factor){
			m_blendDst=factor;
			m_bChanged=true;
		}

		virtual ENUM_MODULATE getModulate() const{
			return m_modulate;
		}
		virtual void setModulate(ENUM_MODULATE m){
			m_modulate=m;
			m_bChanged=true;
		}

		virtual ENUM_ALPHA_SOURCE getAlphaSource() const{
			return m_alphaSource;
		}
		virtual void setAlphaSource(ENUM_ALPHA_SOURCE source){
			m_alphaSource=source;
			m_bChanged=true;
		}

		virtual bool checkChanged(){
			if(m_bChanged)
			{
				m_bChanged=false;
				return true;
			}
			return false;
		}

		virtual void setTextureMatrix(u32 index, const core::matrix4f& mat){
			m_textureLayers[index].textureMatrix=mat;
			m_bChanged=true;
		}
		virtual core::matrix4f& getTextureMatrix(u32 index){
			return m_textureLayers[index].textureMatrix;
		}
	};
}
}
#endif