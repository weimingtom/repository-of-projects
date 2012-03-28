#ifndef _YON_SCENE_IENTITY_H_
#define _YON_SCENE_IENTITY_H_

#include "IReferencable.h"
#include "IUnit.h"

namespace yon{
	namespace scene{

		//�൱��irrlicht��IMesh
		class IEntity : public virtual core::IReferencable{
		public:
			virtual u32 getUnitCount() const = 0;
			virtual IUnit* getUnit(u32 index) const = 0;
		};
	}//scene
}//yon
#endif