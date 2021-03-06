#ifndef _YON_CORE_MATRIX4_H_
#define _YON_CORE_MATRIX4_H_

#include "config.h"
#include "yonTypes.h"
#include "yonMath.h"
#include "vector3d.h"
#include <memory.h>
namespace yon{
	namespace core{

		template<class T>
		class matrix4{
		private:
			//T m[4][4];
			inline void setByProduct(const matrix4& a,const matrix4& b){
				//TODO 4阶矩阵快速乘法[[C++]GLDemo04_1]
				//矩阵快速乘法http://blog.csdn.net/byhuang/article/details/1480695
				for(int i=0;i<4;++i){
					for(int j=0;j<4;++j){
						m[j][i]=a.m[0][i]*b.m[j][0]+a.m[1][i]*b.m[j][1]+a.m[2][i]*b.m[j][2]+a.m[3][i]*b.m[j][3];
					}
				}
			}
		public:
			T m[4][4];
			matrix4(bool identity=false){
				if(identity==true){
					makeIdentity();
				}
			}
			matrix4(const T t[16]){
				for(int i=0;i<4;++i){
					for(int j=0;j<4;++j){
						m[i][j]=t[(i<<2)+j];
					}
				}
			}
			matrix4(const matrix4<T>& other){
				*this=other;
			}

			const T* pointer() const { return &m[0][0]; }
			T* pointer() { return &m[0][0];}

			inline void makeIdentity(){
				memset(m, 0x0, sizeof(m));
				m[0][0] = 1.0f;
				m[1][1] = 1.0f;
				m[2][2] = 1.0f;
				m[3][3] = 1.0f;
			}
			inline void makeTranspose(){
				T temp;
				for(int i=0;i<4;++i){
					for(int j=0;j<i;++j){
						temp=m[i][j];
						m[i][j]=m[j][i];
						m[j][i]=temp;
					}
				}
			}
			//不存在逆矩阵则返回false
			inline bool makeInverse(){
				int i, j, k;
				T temp;
				matrix4 tempMatrix(*this);
				matrix4 result;
				result.makeIdentity();
				for (i = 0; i<4; i++)
				{
					//寻找主元
					k = i;
					for (j=i+1; j<4; j++)
					{
						if (fabs(tempMatrix.m[j][i]) > fabs(tempMatrix.m[i][i]))
							k = j;
					}
					//如果主元所在行不是第i行，进行行交换
					if (k != i)
					{
						for (j=0; j<4; j++)
						{
							temp = tempMatrix.m[i][j];
							tempMatrix.m[i][j] = tempMatrix.m[k][j];
							tempMatrix.m[k][j] = temp;
							//result伴随交换
							temp = result.m[i][j];
							result.m[i][j]= result.m[k][j];
							result.m[k][j] = temp;
						}
					}
					//判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵
					if (tempMatrix.m[i][i] == 0)
						return false;
					//消去src的第i列除去i行以外的各行元素
					temp = tempMatrix.m[i][i];
					for (j=0; j<4; j++)
					{
						tempMatrix.m[i][j] /=temp;    //主对角线上的元素变为1
						result.m[i][j] /= temp;       //伴随计算
					}
					for (j=0; j<4; j++)	
					{
						if (j != i)						//不是第i行
						{
							temp = tempMatrix.m[j][i];
							for (k=0; k<4; k++)        //第j行元素 - i行元素*j列i行元素
							{
								tempMatrix.m[j][k] -= tempMatrix.m[i][k]*temp;
								result.m[j][k] -= result.m[i][k]*temp;
							}
						}
					}
				}
				*this=result;
				return true;
			}
			inline void translate(T x,T y,T z){
				m[3][0]+=m[0][0]*x+m[1][0]*y+m[2][0]*z;
				m[3][1]+=m[0][1]*x+m[1][1]*y+m[2][1]*z;
				m[3][2]+=m[0][2]*x+m[1][2]*y+m[2][2]*z;
				m[3][3]+=m[0][3]*x+m[1][3]*y+m[2][3]*z;
			}
			inline void setTranslation(T x,T y,T z){
				m[3][0]=x;
				m[3][1]=y;
				m[3][2]=z;
			}
			/*inline void setTranslation(const vector3d<T>& translation){
				m[3][0]=translation.x;
				m[3][1]=translation.y;
				m[3][2]=translation.z;
			}*/
			inline void rotate(f32 degree,f32 x,f32 y,f32 z){
				matrix4 rot;
				rot.makeIdentity();
				rot.setRotation(degree,x,y,z);
				*this*=rot;
			}
			inline void setRotation(f32 degree,f32 x,f32 y,f32 z){
				f32 mag=sqrt(x*x+y*y+z*z);
				f32 sinAngle = sinf (degree * DEGTORAD);
				f32 cosAngle = cosf (degree * DEGTORAD);
				if (mag > 0.0f)
				{
					f32 xx, yy, zz, xy, yz, zx, xs, ys, zs;
					f32 oneMinusCos;

					x /= mag;
					y /= mag;
					z /= mag;

					xx = x * x;
					yy = y * y;
					zz = z * z;
					xy = x * y;
					yz = y * z;
					zx = z * x;
					xs = x * sinAngle;
					ys = y * sinAngle;
					zs = z * sinAngle;
					oneMinusCos = 1.0f - cosAngle;
					//经与glRotatef比对，这里需要进行转置
					/*
					m[0][0] = (T)(oneMinusCos * xx) + cosAngle;
					m[0][1] = (T)(oneMinusCos * xy) - zs;
					m[0][2] = (T)(oneMinusCos * zx) + ys;

					m[1][0] = (T)(oneMinusCos * xy) + zs;
					m[1][1] = (T)(oneMinusCos * yy) + cosAngle;
					m[1][2] = (T)(oneMinusCos * yz) - xs;

					m[2][0] = (T)(oneMinusCos * zx) - ys;
					m[2][1] = (T)(oneMinusCos * yz) + xs;
					m[2][2] = (T)(oneMinusCos * zz) + cosAngle;
					*/
					m[0][0] = (T)(oneMinusCos * xx) + cosAngle;
					m[1][0] = (T)(oneMinusCos * xy) - zs;
					m[2][0] = (T)(oneMinusCos * zx) + ys;

					m[0][1] = (T)(oneMinusCos * xy) + zs;
					m[1][1] = (T)(oneMinusCos * yy) + cosAngle;
					m[2][1] = (T)(oneMinusCos * yz) - xs;

					m[0][2] = (T)(oneMinusCos * zx) - ys;
					m[1][2] = (T)(oneMinusCos * yz) + xs;
					m[2][2] = (T)(oneMinusCos * zz) + cosAngle;
				}
			}
			inline void scale(f32 x,f32 y,f32 z){
				matrix4 temp;
				temp.makeIdentity();
				temp.setScale(x,y,z);
				*this*=temp;
			}
			inline void setScale(f32 x,f32 y,f32 z){
				m[0][0] = (T)x;
				m[1][1] = (T)y;
				m[2][2] = (T)z;
				m[3][3] = (T)1.0f;
			}
			inline matrix4<T> operator*(const matrix4<T>& other) const{
				 matrix4<T> r;
				 r.setByProduct(*this,other);
				 return r;
			}
			inline matrix4<T>& operator=(const matrix4<T>& other){
				if(this==&other)
					return *this;
				memcpy(m,other.m,sizeof(m));
				return *this;
			}
			inline matrix4<T>& operator*=(const matrix4<T>& other){
				matrix4 temp(*this);
				setByProduct(temp,other);
				return *this;
			}
			/*const T& operator[](u32 index) const{
				u32 quotient=index>>2;
				u32 remainder=index-quotient;
				return m[quotient][remainder];
			}*/
			inline void frustum(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ){
				f32	deltaX = right - left;
				f32	deltaY = top - bottom;
				f32	deltaZ = farZ - nearZ;
				matrix4	frust;

				if ( (nearZ <= 0.0f) || (farZ <= 0.0f) ||
					(deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f) )
					return;

				frust.m[0][0] = (T)2.0f * nearZ / deltaX;
				frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = (T)0.0f;

				frust.m[1][1] = (T)2.0f * nearZ / deltaY;
				frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = (T)0.0f;

				frust.m[2][0] = (T)(right + left) / deltaX;
				frust.m[2][1] = (T)(top + bottom) / deltaY;
				frust.m[2][2] = (T)-(nearZ + farZ) / deltaZ;
				frust.m[2][3] = (T)-1.0f;

				frust.m[3][2] = (T)-2.0f * nearZ * farZ / deltaZ;
				frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = (T)0.0f;

				*this*=frust;
			}
			inline void ortho(f32 left, f32 right, float bottom, float top, float nearZ, float farZ)
			{
				f32 deltaX = right - left;
				f32 deltaY = top - bottom;
				f32 deltaZ = farZ - nearZ;
				matrix4  ort;

				if ( (nearZ <= 0.0f) || (farZ <= 0.0f) ||
					(deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f) )
					return;

				ort.m[0][0] = (T)2.0f / deltaX;
				ort.m[0][1] = (T)ort.m[0][2] = ort.m[0][3] = 0.0f;
				ort.m[1][1] = (T)2.0f / deltaY;
				ort.m[1][0] = ort.m[1][2] = ort.m[1][3] = (T)0.0f;
				ort.m[2][2] = (T)-2.0f / deltaZ;
				ort.m[2][0] = ort.m[2][1] = ort.m[2][3] = (T)0.0f;
				ort.m[3][0] = (T)-(right + left) / deltaX;
				ort.m[3][1] = (T)-(top + bottom) / deltaY;
				ort.m[3][2] = (T)-(nearZ + farZ) / deltaZ;
				ort.m[3][3] = (T)1.0f;

				*this*=ort;
			}
			inline void perspective(f32 fovy, f32 aspect, f32 nearZ, f32 farZ)
			{
				f32 frustumW, frustumH;

				frustumH = tanf(fovy*DEGTORAD/2) * nearZ;
				frustumW = frustumH * aspect;

				frustum(-frustumW, frustumW, -frustumH, frustumH, nearZ, farZ );
			}
			inline void lookAt(f32 eyex, f32 eyey, f32 eyez,f32 centerx, f32 centery, f32 centerz,f32 upx, f32 upy, f32 upz)
			{
				f32 x[3], y[3], z[3];
				f32 mag;

				// Z vector
				z[0] = eyex - centerx;
				z[1] = eyey - centery;
				z[2] = eyez - centerz;
				mag = (f32)sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
				if (mag) {
					z[0] /= mag;
					z[1] /= mag;
					z[2] /= mag;
				}

				// Y vector
				y[0] = upx;
				y[1] = upy;
				y[2] = upz;

				// X vector = Y cross Z 
				x[0] = y[1] * z[2] - y[2] * z[1];
				x[1] = -y[0] * z[2] + y[2] * z[0];
				x[2] = y[0] * z[1] - y[1] * z[0];

				// Recompute Y = Z cross X 
				//似乎多余，但可能是为了防止用户提供的数据有误时处理
				y[0] = z[1] * x[2] - z[2] * x[1];
				y[1] = -z[0] * x[2] + z[2] * x[0];
				y[2] = z[0] * x[1] - z[1] * x[0];

				// cross product gives area of parallelogram, which is < 1.0 for
				// non-perpendicular unit-length vectors; so normalize x, y here

				mag = (f32)sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
				if (mag) {
					x[0] /= mag;
					x[1] /= mag;
					x[2] /= mag;
				}

				mag = (f32)sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
				if (mag) {
					y[0] /= mag;
					y[1] /= mag;
					y[2] /= mag;
				}

				matrix4 mat;

				mat.m[0][0]=(T)x[0];
				mat.m[1][0]=(T)x[1];
				mat.m[2][0]=(T)x[2];
				mat.m[3][0]=(T)0;

				mat.m[0][1]=(T)y[0];
				mat.m[1][1]=(T)y[1];
				mat.m[2][1]=(T)y[2];
				mat.m[3][1]=(T)0;

				mat.m[0][2]=(T)z[0];
				mat.m[1][2]=(T)z[1];
				mat.m[2][2]=(T)z[2];
				mat.m[3][2]=(T)0;

				mat.m[0][3]=(T)0.0f;
				mat.m[1][3]=(T)0.0f;
				mat.m[2][3]=(T)0.0f;
				mat.m[3][3]=(T)1.0f;

				*this*=mat;
				translate(-eyex,-eyey,-eyez);
			}
		};

		typedef matrix4<f32> matrix4f;

		YON_API extern const matrix4f IDENTITY_MATRIX;
	}
}
#endif