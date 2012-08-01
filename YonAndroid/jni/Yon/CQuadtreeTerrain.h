#ifndef _YON_SCENE_TERREIN_CQUADTREETERRAIN_H_
#define _YON_SCENE_TERREIN_CQUADTREETERRAIN_H_

#include "ITerrainModel.h"
#include "IUnit.h"
#include "SDynamicShap.h"
#include "aabbox3d.h"

namespace yon{
namespace scene{
namespace terrain{

	//Base on Stefan Roettger's quadtree terrain algorithm
	//Generating the triangulation
	//�����Щ���
	//���v���u��
	//�����੤��
	//���u���v��
	//�����ة���
	//First of all, the resolution should decrease as the distance from the viewer increases.
	//This condition can be guaranteed ensuring that: l/d<C  (1)
	//for some constant C, where l is the distance to the eye point, and d is the edge length of
	//the block, C is a configurable quality parameter.
	//The constant C controls the minimum global resolution.As C increases,the total number of 
	//vertices per frame grows quadratically.Note that the condition is evaluated only once for
	//a complete triangle fan,which consists of up to 10 vertices. In order to allow for efficients 
	//computations, distance measurement is performed using the L1-norm.
	//Lp-norm:(�Ux�Up=(|x1|^p+|x2|^p+...+|xn|^p)^{1/p})
	//
	//With the second criterion we want to increase the resolution for regions of high surface roughness.
	//In fact,we want to minimize the projected pixel error, which is a good measure for image
	//quality. When dropping one level of the hierarchy, new error is introduced at exactly five points:
	//at the center of the quadtree node and the four midpoints of its edges. An upper bound to the
	//approximation error in 3-space can be given by taking the maximum of the absolute values of the 
	//elevation differences dhi.The elevation differences are computed along the edges of the node, as
	//well as along its diagonals, which makes a total of six values per node. The error in 3-space 
	//introduced by dropping one level in the quadtree can now be computed by pre-calculating the 
	//maximum of the absolute values of these elevation differences, or alternatively by pre-calculating 
	//surface roughness values, which we call d2:
	//d2=1/d(max|dhi|)   (2)
	//The d2-values of a node times the edge length d of the node correspond to the approximation error in
	//3-space. Thus, the d2-value times d is an upper bound for the error introduced by dropping on level 
	//of detail.
	//
	//A revised version of the subdivision criterion which includes the d2-vales for handling surface roughness 
	//can now be given in terms of a decision variable f:
	//f=1/(d*C*max(c*d2,1)),subdivide if f<1  (3)
	//The constant C again determines the minimum global resolution, whereas the newly introduced constant c
	//specifies the desired global resolution. The latter constant directly influences the number of polygons to 
	//be rendered per frame. Thus, by adjusting c to the current system load, a constant frame rate can be 
	//maintained.
	//
	//The major issue that remains open is how to guarantee that the level difference of adjacent blocks is less
	//than of equal to one. Since the surface roughness of adjacent blocks may differ significantly, this is necessary
	//to build a conforming mesh without holes. In the following we describe how this can be achieved.
	//First suppose that condition (3) is true for a given block(f2<1), that is, the block has to be subdivided.
	//In this case, all adjacent blocks of twice the edge length have to be subdivided,too. Thus, the following 
	//condition must hold for the decision variable f1 of an adjacent block in order to limit the level differences:
	//f1<f2 �� l1/(d*d21)<l2(d/2*d22)  (4) l1*d22<l2*d21*2->l1/(2*l2)<d21/d22
	//For a point of view falling inside the rectangular region Equation(3) is always satisfied, since l1/d is 
	//always less than the minimum resolution C. Outside this region the value of the fraction l1/(2*l2) is 
	//bounded by 1/2(for an infinitely distant point of view) and the constant K with:
	//1/2<l1/(2*l2)<K(C>2)  (5)
	//K=L1/(2*L2)=C/2(C-1)
	//In other words, if d21/d22 is greater than K, then condition(4) is true, since l1/(2*l2) satisfies condition(5).
	//However, since the d2-values, which correspond to surface roughness, can grow arbitrarily large, Condition(4) is
	//not automatically fulfilled. Thus, if d21/d22��K, then we have to modify the d2-value in the following fashion:
	//Starting with the smallest existing block, we calculate the local d2-values of all blocks and propagate them 
	//up the tree. The d2-value of each block is the maximum of the local value and K times the previously calculated
	//values of the adjacent blocks at the next lower level.
	class CQuadtreeTerrain : public ITerrainModel{
	private:

		IUnit* m_pUnit;
		SDynamicShap3D2T m_shap;

		f32 calculateL1Norm(const core::vector3df& a,const core::vector3df& b);

		//We can't really call it "updating" because we start from scratch every frame
		//This step is test each node to see if it can or cannot be subdivided. Then we store the result in the quadtree
		//matrix. If the node can be subdivided, and we have not yet reached the maximum level of detail, then we want to
		//recurse further down the tree and do the same tests on all the node's children.
		//Like any good CLOD algorithm, we want to make sure that our mesh's detail decrease as the camera eye gets further
		//away from it. This is ensured by using Equation(1)
		//d=|x2-x1|+|y2-y1|+|z2-z1|
		//The constant C allows us control of the minimum resolution of the mesh. Roettger advised, in the conference paper
		//that we are basing our implementation off of, that this value be set to nine as a default. However, it's 
		//important to note that as C increases, the total number of vertices per frame grows quadratically.
		//The final step is to determine whether to subdivide a node. Refering to the equation:
		//f=1/(d*C*max(c,1))
		//After you calculate the value for it, you have one simple thing to test for:
		//If f<1 then subdivide node
		//You then would store true in the quadtree matrix for the current node and continue to refine the node's children.
		void refine();
	public:
		CQuadtreeTerrain(IModel* parent,const core::vector3df& pos,
			const core::vector3df& rot,const core::vector3df& scale);
		~CQuadtreeTerrain();

		virtual f32 getHeight(f32 x,f32 z) const{
			//TODO
			return 0;
		}
		virtual void loadHeightMap(video::IImage* image,ENUM_PATCH_SIZE patchSize);

		virtual void render(video::IVideoDriver* driver);

		virtual void onRegisterForRender();

		virtual u32 getMaterialCount() const{
			return 1;
		}

		virtual video::IMaterial* getMaterial(u32 num) const{
			if(num>=getMaterialCount())
				return NULL;
			return m_pUnit->getMaterial();
		}
	};
}
}
}
#endif