#ifndef _YON_SCENE_CGEOMETRYFACTORY_H_
#define _YON_SCENE_CGEOMETRYFACTORY_H_

#include "IGeometryFactory.h"

namespace yon{
	namespace scene{

		class CGeometryFactory : public IGeometryFactory{
		public:

			virtual IEntity* createCube(const core::dimension3df& size) const;
			virtual IEntity* createSphere(f32 radius,const core::dimension2du& detail) const;
		};
	}
}
#endif