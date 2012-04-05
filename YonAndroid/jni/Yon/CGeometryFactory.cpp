#include "CGeometryFactory.h"
#include "SVertex.h"
#include "CUnit.h"
#include "CEntity.h"
#include "CUnit2D.h"
#include "yonMath.h"

#include "ILogger.h"

using namespace yon::core;

namespace yon{
namespace scene{

	/*
	----------x1,y1
	|			|
	|			|
	x0,y0------
	*/
	IShap* CGeometryFactory::createXYRectangle(s32 x0,s32 y0,s32 x1,s32 y1,f32 u0,f32 v0,f32 u1,f32 v1,const video::SColor& color) const{
		Shap2D* shap=new Shap2D();

		// Create indices
		const static u8 u[6] = {0,  1,  3,  3,  1,  2};

		shap->m_indices.reallocate(6);
		for (u32 i=0; i<6; ++i)
			shap->m_indices.push(u[i]);

		//Create vertexs
		shap->m_vertices.reallocate(4);

		shap->m_vertices.push(S2DVertex((f32)x0,(f32)y0,u0,v0,color));
		shap->m_vertices.push(S2DVertex((f32)x1,(f32)y0,u1,v0,color));
		shap->m_vertices.push(S2DVertex((f32)x1,(f32)y1,u1,v1,color));
		shap->m_vertices.push(S2DVertex((f32)x0,(f32)y1,u0,v1,color));

		return shap;

	}

	IUnit* CGeometryFactory::createUnit(IShap* shap)const {
		if(shap->getDimenMode()==ENUM_DIMEN_MODE_2D){
			CUnit2D* unit=new CUnit2D();
			unit->setShap(shap);
			return unit;
		}else if(shap->getDimenMode()==ENUM_DIMEN_MODE_3D){
			CUnit* unit=new CUnit();
			unit->setShap(shap);
			return unit;
		}
		return NULL;
	}

	IEntity* CGeometryFactory::createEntity(IUnit* unit)const {
		CEntity* entity=new CEntity(unit->getDimenMode());
		entity->addUnit(unit);
		return entity;
	}


	IEntity* CGeometryFactory::createXYPlane2D(const core::dimension2df& size) const{
		CUnit2D* unit=new CUnit2D();

		// Create indices
		const u8 u[6] = {
			0,  2,  1,
			0,  3,  2
		};

		unit->m_indices.reallocate(6);
		for (u32 i=0; i<6; ++i)
			unit->m_indices.push(u[i]);

		//Create vertexs
		unit->m_vertices.reallocate(4);

		f32 phw,phh,mhw,mhh;
		phw=size.w/2;
		phh=size.h/2;
		mhw=phw-size.w;
		mhh=phh-size.h;

		f32 u0,u1,v0,v1;
		u0=v0=0;
		u1=v1=1;

		unit->m_vertices.push(S2DVertex(mhw,mhh,u0,v1,video::COLOR_WHITE));
		unit->m_vertices.push(S2DVertex(mhw,phh,u0,v0,video::COLOR_WHITE));
		unit->m_vertices.push(S2DVertex(phw,phh,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(S2DVertex(phw,mhh,u1,v1,video::COLOR_WHITE));

		CEntity* entity=new CEntity(ENUM_DIMEN_MODE_2D);
		entity->addUnit(unit);

		unit->drop();

		return entity;

	}

	IEntity* CGeometryFactory::createXYPlane(const core::dimension2df& size,f32 z) const{
		/*CUnit* unit=new CUnit();

		// Create indices
		const u16 u[6] = {
			0,  2,  1,
			0,  3,  2
		};

		unit->m_indices.reallocate(6);
		for (u32 i=0; i<6; ++i)
			unit->m_indices.push(u[i]);

		//Create vertexs
		unit->m_vertices.reallocate(4);

		f32 phw,phh,mhw,mhh;
		phw=size.w/2;
		phh=size.h/2;
		mhw=phw-size.w;
		mhh=phh-size.h;

		f32 u0,u1,v0,v1;
		u0=v0=0;
		u1=v1=1;

		unit->m_vertices.push(SVertex(mhw,mhh,z,u0,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,phh,z,u0,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,phh,z,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,mhh,z,u1,v1,video::COLOR_WHITE));

		CEntity* entity=new CEntity();
		entity->addUnit(unit);

		unit->drop();

		return entity;*/

		CUnit* unit=new CUnit();
		Shap3D* shap=new Shap3D();

		// Create indices
		const u16 u[6] = {
			0,  2,  1,
			0,  3,  2
		};

		shap->m_indices.reallocate(6);
		for (u32 i=0; i<6; ++i)
			shap->m_indices.push(u[i]);

		//Create vertexs
		shap->m_vertices.reallocate(4);

		f32 phw,phh,mhw,mhh;
		phw=size.w/2;
		phh=size.h/2;
		mhw=phw-size.w;
		mhh=phh-size.h;

		f32 u0,u1,v0,v1;
		u0=v0=0;
		u1=v1=1;

		shap->m_vertices.push(SVertex(mhw,mhh,z,u0,v1,video::COLOR_WHITE));
		shap->m_vertices.push(SVertex(mhw,phh,z,u0,v0,video::COLOR_WHITE));
		shap->m_vertices.push(SVertex(phw,phh,z,u1,v0,video::COLOR_WHITE));
		shap->m_vertices.push(SVertex(phw,mhh,z,u1,v1,video::COLOR_WHITE));

		unit->setShap(shap);
		shap->drop();

		CEntity* entity=new CEntity();
		entity->addUnit(unit);

		unit->drop();

		return entity;

	}

	IEntity* CGeometryFactory::createCube(const core::dimension3df& size) const{

		CUnit* unit=new CUnit();

		// Create indices
		const u16 u[36] = {
			0,  2,  1,
			0,  3,  2, 
			4,  5,  6,
			4,  6,  7,
			8,  9,  10,
			8,  10, 11, 
			12, 15, 14,
			12, 14, 13, 
			16, 17, 18,
			16, 18, 19, 
			20, 23, 22,
			20, 22, 21
		};
		unit->m_indices.reallocate(36);
		for (u32 i=0; i<36; ++i)
			unit->m_indices.push(u[i]);

		//Create vertexs
		unit->m_vertices.reallocate(24);

		f32 phw,phh,phd,mhw,mhh,mhd;
		phw=size.w/2;
		phh=size.h/2;
		phd=size.d/2;
		mhw=phw-size.w;
		mhh=phh-size.h;
		mhd=phd-size.d;

		f32 u0,u1,v0,v1;
		u0=v0=0;
		u1=v1=1;

		Logger->debug("half cube size:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",phw,phh,phd,mhw,mhh,mhd);

		unit->m_vertices.push(SVertex(mhw,mhh,mhd,u0,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,mhh,phd,u0,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,mhh,phd,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,mhh,mhd,u1,v1,video::COLOR_WHITE));

		unit->m_vertices.push(SVertex(mhw,phh,mhd,u0,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,phh,phd,u0,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,phh,phd,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,phh,mhd,u1,v1,video::COLOR_WHITE));

		unit->m_vertices.push(SVertex(mhw,mhh,mhd,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,phh,mhd,u1,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,phh,mhd,u0,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,mhh,mhd,u0,v0,video::COLOR_WHITE));

		unit->m_vertices.push(SVertex(mhw,mhh,phd,u0,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,phh,phd,u0,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,phh,phd,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,mhh,phd,u1,v1,video::COLOR_WHITE));

		//左
		unit->m_vertices.push(SVertex(mhw,mhh,mhd,u0,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,mhh,phd,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,phh,phd,u1,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,phh,mhd,u0,v1,video::COLOR_WHITE));

		//右
		unit->m_vertices.push(SVertex(phw,mhh,mhd,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,mhh,phd,u0,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,phh,phd,u0,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,phh,mhd,u1,v1,video::COLOR_WHITE));

		CEntity* entity=new CEntity();
		entity->addUnit(unit);

		unit->drop();

		return entity;
	}
	IShap* CGeometryFactory::createCube(f32 width,f32 height,f32 depth,const video::SColor& color) const{
		Shap3D* shap=new Shap3D();

		// Create indices
		const u16 u[36] = {
			0,  2,  1,
			0,  3,  2, 
			4,  5,  6,
			4,  6,  7,
			8,  9,  10,
			8,  10, 11, 
			12, 15, 14,
			12, 14, 13, 
			16, 17, 18,
			16, 18, 19, 
			20, 23, 22,
			20, 22, 21
		};
		shap->m_indices.reallocate(36);
		for (u32 i=0; i<36; ++i)
			shap->m_indices.push(u[i]);

		//Create vertexs
		shap->m_vertices.reallocate(24);

		f32 phw,phh,phd,mhw,mhh,mhd;
		phw=width/2;
		phh=height/2;
		phd=depth/2;
		mhw=phw-width;
		mhh=phh-height;
		mhd=phd-depth;

		f32 u0,u1,v0,v1;
		u0=v0=0;
		u1=v1=1;

		Logger->debug("half cube size:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",phw,phh,phd,mhw,mhh,mhd);

		shap->m_vertices.push(SVertex(mhw,mhh,mhd,u0,v1,color));
		shap->m_vertices.push(SVertex(mhw,mhh,phd,u0,v0,color));
		shap->m_vertices.push(SVertex(phw,mhh,phd,u1,v0,color));
		shap->m_vertices.push(SVertex(phw,mhh,mhd,u1,v1,color));

		shap->m_vertices.push(SVertex(mhw,phh,mhd,u0,v1,color));
		shap->m_vertices.push(SVertex(mhw,phh,phd,u0,v0,color));
		shap->m_vertices.push(SVertex(phw,phh,phd,u1,v0,color));
		shap->m_vertices.push(SVertex(phw,phh,mhd,u1,v1,color));

		shap->m_vertices.push(SVertex(mhw,mhh,mhd,u1,v0,color));
		shap->m_vertices.push(SVertex(mhw,phh,mhd,u1,v1,color));
		shap->m_vertices.push(SVertex(phw,phh,mhd,u0,v1,color));
		shap->m_vertices.push(SVertex(phw,mhh,mhd,u0,v0,color));

		shap->m_vertices.push(SVertex(mhw,mhh,phd,u0,v1,color));
		shap->m_vertices.push(SVertex(mhw,phh,phd,u0,v0,color));
		shap->m_vertices.push(SVertex(phw,phh,phd,u1,v0,color));
		shap->m_vertices.push(SVertex(phw,mhh,phd,u1,v1,color));

		//左
		shap->m_vertices.push(SVertex(mhw,mhh,mhd,u0,v0,color));
		shap->m_vertices.push(SVertex(mhw,mhh,phd,u1,v0,color));
		shap->m_vertices.push(SVertex(mhw,phh,phd,u1,v1,color));
		shap->m_vertices.push(SVertex(mhw,phh,mhd,u0,v1,color));

		//右
		shap->m_vertices.push(SVertex(phw,mhh,mhd,u1,v0,color));
		shap->m_vertices.push(SVertex(phw,mhh,phd,u0,v0,color));
		shap->m_vertices.push(SVertex(phw,phh,phd,u0,v1,color));
		shap->m_vertices.push(SVertex(phw,phh,mhd,u1,v1,color));

		return shap;
	}
	IShap* CGeometryFactory::createSphere(f32 radius,u32 hSteps,u32 vSteps) const{
		//CUnit* unit=new CUnit();
		Shap3D* shap=new Shap3D();

		f32 dtheta=(float)360/hSteps;	//水平方向步增
		f32 dphi=(float)180/vSteps;		//垂直方向步增

		u32 numVertices=vSteps*hSteps<<2;
		u32 numIndices=vSteps*hSteps*6;
	
		shap->m_vertices.reallocate(numVertices);
		shap->m_indices.reallocate(numIndices);

		u32 index=0;
		//u32 index2=0;
		f32 phi=0;
		f32 theta=0;
		f32 x,y,z;
		f32 u,v;
		vector3df temp;
		for(u32 i=0;i<vSteps;++i,phi+=dphi)
		{
			for(u32 j=0;j<hSteps;++j,theta+=dtheta)
			{
				z = (f32) (radius * sinf(phi*DEGTORAD) * cosf(DEGTORAD*theta));
				x = (f32) (radius * sinf(phi*DEGTORAD) * sinf(DEGTORAD*theta));
				y = (f32) (radius * cosf(phi*DEGTORAD));
				temp.set(x,y,z);
				temp.normalize();
				u = (f32)j/hSteps;
				v = (f32)asinf(temp.y)/PI+0.5f;
				shap->m_vertices.push(SVertex(x,y,z,u,v,video::COLOR_WHITE));
				/*NORMALIZE_VERTEX_TO((*vertices)[index*3+0],(*vertices)[index*3+1],(*vertices)[index*3+2],temp);
				if(texCoords!=NULL)
				{
					(*texCoords)[index*2+0]=(float)j/hSteps;
					(*texCoords)[index*2+1]=(float)asinf(temp[1])/M_PI+0.5f;
				}
				if(normals!=NULL)
				{
					(*normals)[index*3+0]=temp[0];
					(*normals)[index*3+1]=temp[1];
					(*normals)[index*3+2]=temp[2];
				}*/
				++index;
				z = (float) (radius * sinf((phi+dphi)*DEGTORAD) * cosf(DEGTORAD*theta));
				x = (float) (radius * sinf((phi+dphi)*DEGTORAD) * sinf(DEGTORAD*theta));
				y = (float) (radius * cosf((phi+dphi)*DEGTORAD));
				temp.set(x,y,z);
				temp.normalize();
				u = (f32)j/hSteps;
				v = (f32)asinf(temp.y)/PI+0.5f;
				shap->m_vertices.push(SVertex(x,y,z,u,v,video::COLOR_WHITE));
				/*NORMALIZE_VERTEX_TO((*vertices)[index*3+0],(*vertices)[index*3+1],(*vertices)[index*3+2],temp);
				if(texCoords!=NULL)
				{
					(*texCoords)[index*2+0]=(float)j/hSteps;
					(*texCoords)[index*2+1]=(float)asinf(temp[1])/M_PI+0.5f;
				}
				if(normals!=NULL)
				{
					(*normals)[index*3+0]=temp[0];
					(*normals)[index*3+1]=temp[1];
					(*normals)[index*3+2]=temp[2];
				}*/
				++index;
				z  = (float) (radius * sinf((phi+dphi)*DEGTORAD) * cosf(DEGTORAD*(theta+dtheta)));
				x  = (float) (radius * sinf((phi+dphi)*DEGTORAD) * sinf(DEGTORAD*(theta+dtheta)));
				y  = (float) (radius * cosf((phi+dphi)*DEGTORAD));
				temp.set(x,y,z);
				temp.normalize();
				u = (f32)(j+1)/hSteps;
				v = (f32)asinf(temp.y)/PI+0.5f;
				shap->m_vertices.push(SVertex(x,y,z,u,v,video::COLOR_WHITE));
				/*NORMALIZE_VERTEX_TO((*vertices)[index*3+0],(*vertices)[index*3+1],(*vertices)[index*3+2],temp);
				if(texCoords!=NULL)
				{
					(*texCoords)[index*2+0]=(float)(j+1)/hSteps;
					(*texCoords)[index*2+1]=(float)asinf(temp[1])/M_PI+0.5f;
				}
				if(normals!=NULL)
				{
					(*normals)[index*3+0]=temp[0];
					(*normals)[index*3+1]=temp[1];
					(*normals)[index*3+2]=temp[2];
				}*/
				++index;
				z  = (float) (radius * sinf(phi*DEGTORAD) * cosf(DEGTORAD*(theta+dtheta)));
				x  = (float) (radius * sinf(phi*DEGTORAD) * sinf(DEGTORAD*(theta+dtheta)));
				y  = (float) (radius * cosf(phi*DEGTORAD));
				temp.set(x,y,z);
				temp.normalize();
				u = (f32)(j+1)/hSteps;
				v = (f32)asinf(temp.y)/PI+0.5f;
				shap->m_vertices.push(SVertex(x,y,z,u,v,video::COLOR_WHITE));
				/*NORMALIZE_VERTEX_TO((*vertices)[index*3+0],(*vertices)[index*3+1],(*vertices)[index*3+2],temp);
				if(texCoords!=NULL)
				{
					(*texCoords)[index*2+0]=(float)(j+1)/hSteps;
					(*texCoords)[index*2+1]=(float)asinf(temp[1])/M_PI+0.5f;
				}
				if(normals!=NULL)
				{
					(*normals)[index*3+0]=temp[0];
					(*normals)[index*3+1]=temp[1];
					(*normals)[index*3+2]=temp[2];
				}*/
				++index;

				shap->m_indices.push(index-4);
				shap->m_indices.push(index-3);
				shap->m_indices.push(index-2);
				shap->m_indices.push(index-4);
				shap->m_indices.push(index-2);
				shap->m_indices.push(index-1);

				/*if(indices!=NULL)
				{
					(*indices)[index2++]=index-4;
					(*indices)[index2++]=index-3;
					(*indices)[index2++]=index-2;
					(*indices)[index2++]=index-4;
					(*indices)[index2++]=index-2;
					(*indices)[index2++]=index-1;
				}*/
			}
		}

		//CEntity* entity=new CEntity();
		//entity->addUnit(unit);

		//unit->drop();

		/*
		private void build()
		{
		int r, c;

		Number3d n = new Number3d();
		Number3d pos = new Number3d();
		Number3d posFull = new Number3d();

		if( defaultColor() == null ) defaultColor(new Color4());
		// Build vertices

		for (r = 0; r <= _rows; r++)
		{
		float v = (float)r / (float)_rows; // [0,1]
		float theta1 = v * (float)Math.PI; // [0,PI]

		n.setAll(0,1,0);
		n.rotateZ(theta1); 

		// each 'row' assigned random color. for the hell of it.

		for (c = 0; c <= _cols; c++)
		{
		float u = (float)c / (float)_cols; // [0,1]
		float theta2 = u * (float)(Math.PI * 2f); // [0,2PI]
		pos.setAllFrom(n);
		pos.rotateY(theta2);

		posFull.setAllFrom(pos);
		posFull.multiply(_radius);


		this.vertices().addVertex(posFull.x,posFull.y,posFull.z,  u,v,  pos.x,pos.y,pos.z,  defaultColor().r,defaultColor().g,defaultColor().b,defaultColor().a);
		}
		}


		// Add faces

		int colLength = _cols + 1;

		for (r = 0; r < _rows; r++)
		{
		int offset = r * colLength; 

		for (c = 0; c < _cols; c++)
		{
		int ul = offset  +  c;
		int ur = offset  +  c+1;
		int br = offset  +  (int)(c + 1 + colLength);
		int bl = offset  +  (int)(c + 0 + colLength);

		Utils.addQuad(this, ul,ur,br,bl);
		}
		}
		}
		*/

		return shap;
	}

	//x=(R+r*cosθ)cosΦ
	//y=(R+r*cosθ)sinΦ
	//z=r*sinθ
	IShap* CGeometryFactory::createTorus(f32 cirRadius,f32 orbitRadius,u32 cirSteps,u32 orbitSteps,const video::SColor& color) const{
		Shap3D* shap=new Shap3D();

		f32 dtheta=(float)PI2/cirSteps;		//形圆角步增
		f32 dphi=(float)PI2/orbitSteps;		//轨圆角步增

		u32 numVertices=cirSteps*orbitSteps<<2;
		u32 numIndices=cirSteps*orbitSteps*6;

		shap->m_vertices.reallocate(numVertices);
		shap->m_indices.reallocate(numIndices);

		u32 index=0;
		f32 phi=0;
		f32 theta=0;
		f32 x,y,z;
		f32 u,v;
		for(u32 i=0;i<orbitSteps;++i,phi+=dphi)
		{
			for(u32 j=0;j<cirSteps;++j,theta+=dtheta)
			{
				
				x = (f32) ((cirRadius * cosf(theta)+orbitRadius)*cosf(phi+dphi));
				y = (f32) ((cirRadius * cosf(theta)+orbitRadius)*sinf(phi+dphi));
				z = (f32) (cirRadius * sinf(theta));
				u = (f32)(i+1)/orbitSteps;
				v = (f32)j/cirSteps;
				shap->m_vertices.push(SVertex(x,y,z,u,v,color));
				++index;

				x = (f32) ((cirRadius * cosf(theta+dtheta)+orbitRadius)*cosf(phi+dphi));
				y = (f32) ((cirRadius * cosf(theta+dtheta)+orbitRadius)*sinf(phi+dphi));
				z = (f32) (cirRadius * sinf(theta+dtheta));
				u = (f32)(i+1)/orbitSteps;
				v = (f32)(j+1)/cirSteps;
				shap->m_vertices.push(SVertex(x,y,z,u,v,color));
				++index;

				x = (f32) ((cirRadius * cosf(theta+dtheta)+orbitRadius)*cosf(phi));
				y = (f32) ((cirRadius * cosf(theta+dtheta)+orbitRadius)*sinf(phi));
				z = (f32) (cirRadius * sinf(theta+dtheta));
				u = (f32)i/orbitSteps;
				v = (f32)(j+1)/cirSteps;
				shap->m_vertices.push(SVertex(x,y,z,u,v,color));
				++index;

				x = (f32) ((cirRadius * cosf(theta)+orbitRadius)*cosf(phi));
				y = (f32) ((cirRadius * cosf(theta)+orbitRadius)*sinf(phi));
				z = (f32) (cirRadius * sinf(theta));
				u = (f32)i/orbitSteps;
				v = (f32)j/cirSteps;
				shap->m_vertices.push(SVertex(x,y,z,u,v,color));
				++index;

				shap->m_indices.push(index-4);//0
				shap->m_indices.push(index-3);//1
				shap->m_indices.push(index-2);//2
				shap->m_indices.push(index-4);//0
				shap->m_indices.push(index-2);//2
				shap->m_indices.push(index-1);//3
			}
		}

		return shap;
	}
}//scene
}//yon