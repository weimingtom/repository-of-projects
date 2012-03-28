#ifndef _YON_CORE_IRENDERABLE_H_
#define _YON_CORE_IRENDERABLE_H_

#include "IReferencable.h"
#include "IVideoDriver.h"
#include "vector3d.h"

namespace yon{
	namespace core{

		class IRenderable : public virtual IReferencable{
		protected:
			core::vector3df m_position;
			core::vector3df m_rotation;
			core::vector3df m_scale;
		public:
			IRenderable(const core::vector3df& pos=core::vector3df(0,0,0),
				const core::vector3df& rot=core::vector3df(0,0,0),
				const core::vector3df& scale=core::vector3df(1,1,1)):
				m_position(pos),m_rotation(rot),m_scale(scale){}
			virtual void render(video::IVideoDriver* driver) = 0;

			virtual void setPosition(const core::vector3df& pos){
				m_position=pos;
			}
			virtual const core::vector3df& getPosition() const{
				return m_position;
			}

			virtual void setRotation(const core::vector3df& rotation){
				m_rotation=rotation;
			}
			virtual const core::vector3df& getRotation() const{
				return m_rotation;
			}

			virtual void setScale(const core::vector3df& scale){
				m_scale=scale;
			}
			virtual const core::vector3df& getScale() const{
				return m_scale;
			}
		};
	}
}
#endif