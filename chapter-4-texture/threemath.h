#ifndef __THREEMATH_H__
#define __THREEMATH_H__

#include <math.h>

#define Float float
#define Int int


const Float kPi = 3.141592653f;
const Float k2Pi = kPi * 2.0f;
const Float kPiOver2 = kPi / 2.0f;
const Float k1OverPi = 1 / kPi;

#define RADIAN(x) (x * kPi / 180)

Float SafeAcos(Float x)
{
	if(x <= -1.0f)
	{
		return kPi;
	}

	if(x >=1.0f)
	{
		return 0.0f;
	}

	return acos(x);
}

/**********************************Vector3**************************/
class Vector3
{

	public:
		Float x;
		Float y;
		Float z;
		
		Vector3():
			x(0),
			y(0),
			z(0)
		{
		}

		Vector3(Float nx,Float ny,Float nz):
			x(nx),
			y(ny),
			z(nz)
		{

		}
		
		Vector3(const Vector3 &other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
		}


		void Set(Float nx,Float ny,Float nz)
		{
			x = nx;
			y = ny;
			z = nz;
		}


		Float Length() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		Float SqrLength() const
		{
			return x * x + y * y + z * z;
		}

		void Normalized()
		{
			Float len = Length();
			if(len > 0)
			{
				x /= len;
				y /= len;
				z /= len;
			}
		}

		static Float Dot(Vector3 a, Vector3 b)
		{
			return a.x * b.x
				+ a.y * b.y
				+ a.z * b.z;
		}

		static Vector3 Cross(Vector3 a, Vector3 b)
		{
			return Vector3(a.y * b.z - a.z * b.y,
					a.z * b.x - a.x * b.z,
					a.x * b.y - a.y * b.x);
		}

		Vector3 operator =(const Vector3 &other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}


		bool operator==(const Vector3 &other) const
		{
			return x == other.x && y == other.y && z== other.z;
		}

		bool operator !=(const Vector3 &other) const
		{
			return x!=other.x || y != other.y || z!=other.z;
		}

		friend Vector3 operator+(Vector3 lvalue,Vector3 rvalue);
		friend Vector3 operator-(Vector3 lvalue, Vector3 rvalue);
		friend Vector3 operator*(Vector3 value, Float x);

};

inline Vector3 operator+(Vector3 lvalue,Vector3 rvalue)
{
	Vector3 temp(lvalue.x + rvalue.x,  lvalue.y + rvalue.y,  lvalue.z + rvalue.z);
	return temp;

}

inline Vector3 operator-(Vector3 lvalue, Vector3 rvalue)
{			
	Vector3 temp(lvalue.x - rvalue.x, lvalue.y - rvalue.y, lvalue.z - rvalue.z);
	return temp;
}

inline Vector3 operator*(Vector3 value, Float x)
{
	Vector3 temp(value.x * x, value.y * x, value.z * x);
	return temp;
}

Float Distance(const Vector3 &a, const Vector3 &b)
{
	return (a - b).Length();
}



/*******************************Quaternion*********************/
class Quaternion
{
	public:
		Quaternion():
			x(0),
			y(0),
			z(0),
			w(1)
		{
		}

		Quaternion(Float x, Float y, Float z, Float w):
			x(x),
			y(y),
			z(z),
			w(w)
		{
		}	

		Quaternion(const Quaternion &other):
			x(other.x),
			y(other.y),
			z(other.z),
			w(other.w)
		{
		}

		void Identity()
		{
			x = 0;
			y = 0;
			z = 0;
			w = 1;
		}
		
		void Normalized()
		{
			Float mag = (Float)sqrt(x * x + y * y + z * z + w * w);
			if(mag > 0.0f)
			{
				Float oneOverMag = 1.0f / mag;
				w *= oneOverMag;
				x *= oneOverMag;
				y *= oneOverMag;
				z *= oneOverMag;
			}
			else
			{
				//有麻烦了
				
				//在发布版中返回单位四元数
				w = 1;
				x = y = z = 0;
			}
		}
				

		Float GetEulerAngle()
		{
			//计算半角
			Float thetaOver2 = SafeAcos(w);
			return thetaOver2 * 2 * 180;
		}

		Vector3 GetRotationAxis()
		{
			//计算sin^(theta/2)
			
			Float sinThetaOver2Sq = 1.0f - w * w;
			//注意保持数值精度
			if(sinThetaOver2Sq <= 0.0f)
			{
				//单位四元数或不精确的数值，只要返回有效的向量即可
				return Vector3(1.0,0,0);

			}
			
			Float oneOverSinThetaOver2 = 1.0f / sqrt(sinThetaOver2Sq);

			return Vector3( x * oneOverSinThetaOver2,
					y * oneOverSinThetaOver2,
					z * oneOverSinThetaOver2);
		}

		static Quaternion Euler(Float x, Float y, Float z)
		{
			Float rx = RADIAN(x / 2);
			Float ry = RADIAN(y / 2);
			Float rz = RADIAN(z / 2);

			Float sinx = sin(rx);
			Float cosx = cos(rx);
			Float siny = sin(ry);
			Float cosy = cos(ry);
			Float sinz = sin(rz);
			Float cosz = cos(rz);

			return Quaternion(cosy * sinx * cosz + siny * cosx * sinz,
				siny * cosx * cosz - cosy * sinx * sinz,
				cosy * cosx * sinz - siny * sinx *cosz,
				cosy * cosx * cosz + siny * sinx * sinz);
		}

		Quaternion Angle(Quaternion a, Quaternion y)
		{
			Float len = sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);

			Quaternion conjugate = a.Conjugate();

			return conjugate * y * (1.0f / len);
		}
		
		Quaternion Conjugate() const
		{
			return Quaternion(-x,-y,-z,w);
		}

		void operator=(const Quaternion &other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		
		static Quaternion Slerp(Quaternion start, Quaternion end, Float p)
		{
			if( p <= 0.0f)
			{
				return start;
			}

			if(p >= 1.0f)
			{
				return end;
			}

			Float cosOmega = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;
			if(cosOmega < 0.0f)
			{
				end = end.Conjugate();
				cosOmega = -cosOmega;
			}

			Float k0,k1;
			if(cosOmega > 0.9999f)
			{
				//非常接近----使用线性插值
				k0=1.0f - p;
				k1 = p;
			}
			else
			{
				//用三角公式sin^2(omega) + cos^(omega) = 1 计算sin值
				Float sinOmega = sqrt(1.0f - cosOmega * cosOmega);
				Float omega = atan2(sinOmega,cosOmega);
				Float oneOverSinOmega = 1.0f / sinOmega;

				k0 = sin((1.0f - p) * omega) * oneOverSinOmega;
				k1 = sin(p * omega) * oneOverSinOmega;
			}
			
			return Quaternion(start.x * k0 + end.x * k1,
			start.y * k0 + end.y * k1,
			start.z * k0 + end.z * k1,
		       	start.w * k0 + end.w * k1);
		}

		Quaternion Pow(Float exponent) const
		{
			if(w > 0.9999f)
			{
				return *this;
			}

			Float alpha = acos(w);
			//计算新的alpha值
			Float newAlpha = alpha * exponent;
			//计算新的w值
			Quaternion result;
			result.w = cos(newAlpha);

			Float mult = sin(newAlpha) / sin(alpha);
			result.x = x * mult;
			result.y = y * mult;
			result.z = z * mult;

			return result;
		}

			

		friend Quaternion operator*(Quaternion a, Quaternion b);
		friend Quaternion operator*(Quaternion value, Float a);
		friend Vector3 operator*(Vector3 v,Quaternion a);


	public:
		Float x;
		Float y;
		Float z;
		Float w;
};

//旋转a再旋转b不再是b*a,而是a*b，因为叉乘换了顺序
inline Quaternion operator*(Quaternion a, Quaternion b)
{
	return Quaternion(a.w * b.x + a.x * b.w + a.z * b.y - a.y * b.z,
			a.w * b.y + a.y * b.w + a.x * b.z - a.z * b.x,
			a.w * b.z + a.z * b.w + a.y * b.x - a.x * b.y,
			a.w * b.w - a.x * b.x -a.y * b.y - a.z * b.z);
}

inline Quaternion operator*(Quaternion value, Float a) 
{
	return Quaternion(value.x * a, value.y * a, value.z * a, value.w * a);
}

inline Vector3 operator*(Vector3 v,Quaternion a)
{
	Quaternion q(v.x,v.y,v.z,0);
	Quaternion res = q * a;
	return Vector3(res.x,res.y,res.z);
}
		       	

/************************************Matrix4x4**************************************/
class Matrix4x4
{
	public:
		Matrix4x4()
		{
			for(int i=0;i < 16; ++i)
			{
				m_data[i] = 0;
			}
		}

		~Matrix4x4()
		{
		}

		inline static Matrix4x4 Identity()
		{
			Matrix4x4 m;
			m.m_data[0] = m.m_data[5] = m.m_data[10] = m.m_data[15] = 1;
			return m;
		}

		void Rotate(const Vector3 &axis,Float theta)
		{
			Float c = cos(RADIAN(theta));
			Float s = sin(RADIAN(theta));
			Matrix4x4 m;
			m.m_data[0] = axis.x * axis.x * (1 - c)  + c;
			m.m_data[1] = axis.x * axis.y * (1 - c) + axis.z * s;
			m.m_data[2] = axis.x * axis.z * (1 - c) - axis.y * s;
			m.m_data[3] = 0;
			m.m_data[4] = axis.x * axis.y * (1 - c) - axis.z * s;
			m.m_data[5] = axis.y * axis.y * (1 - c) + c;
			m.m_data[6] = axis.y * axis.z * (1 - c) + axis.x * s;
			m.m_data[7] = 0;
			m.m_data[8] = axis.x * axis.z * (1 - c) + axis.y * s;
			m.m_data[9] = axis.y * axis.z * (1 - c) -axis.x * s;
			m.m_data[10] = axis.z * axis.z * (1- c) + c;
			m.m_data[11] = 0;

			m.m_data[12] = 0;
			m.m_data[13] = 0;
			m.m_data[14] = 0;
			m.m_data[15] = 1;

			*this *= m;
		}

		void Scale(float s)
		{
			m_data[0] *=s;
			m_data[5] *=s;
			m_data[10] *= s;		
		}

		void Translate(const Vector3 &t)
		{
			m_data[12] += t.x;
			m_data[13] += t.y;
			m_data[14] += t.z;
		}


		Float* Get()
		{
			return m_data;
		}


		void operator=(const Matrix4x4 &other)
		{
			for(int i = 0; i < 16; ++i)
			{
				m_data[i] = other.m_data[i];
			}
		}

		Matrix4x4 operator*=(const Matrix4x4 &other)
		{
			*this = *this * other;
			return *this;
		}

		friend Matrix4x4 operator*(const Matrix4x4 &a, const Matrix4x4 &b);
		friend Matrix4x4 Frustum(Float left,Float right,Float bottom, Float top, Float near, Float far);
		friend Matrix4x4 Ortho(Float left, Float right,Float bottom, Float top, Float near, Float far);

	private:
		Float m_data[16];
};

inline Matrix4x4 operator*(const Matrix4x4 &a, const Matrix4x4 &b)
{
	Matrix4x4 m;
	m.m_data[0] = a.m_data[0] * b.m_data[0] + a.m_data[1] * b.m_data[4] + a.m_data[2] * b.m_data[8] + a.m_data[3] * b.m_data[12];
	m.m_data[1] = a.m_data[0] * b.m_data[1] + a.m_data[1] * b.m_data[5] + a.m_data[2] * b.m_data[9] + a.m_data[3] * b.m_data[13];
	m.m_data[2] = a.m_data[0] * b.m_data[2] + a.m_data[1] * b.m_data[6] + a.m_data[2] * b.m_data[10] + a.m_data[3] * b.m_data[14];
	m.m_data[3] = a.m_data[0] * b.m_data[3] + a.m_data[1] * b.m_data[7] + a.m_data[2] * b.m_data[11] + a.m_data[3] * b.m_data[15];
	m.m_data[4] = a.m_data[4] * b.m_data[0] + a.m_data[5] * b.m_data[4] + a.m_data[6] * b.m_data[8] + a.m_data[7] * b.m_data[12];
	m.m_data[5] = a.m_data[4] * b.m_data[1] + a.m_data[5] * b.m_data[5] + a.m_data[6] * b.m_data[9] + a.m_data[7] * b.m_data[13];
	m.m_data[6] = a.m_data[4] * b.m_data[2] + a.m_data[5] * b.m_data[6] + a.m_data[6] * b.m_data[10] + a.m_data[7] * b.m_data[14];
	m.m_data[7] = a.m_data[4] * b.m_data[3] + a.m_data[5] * b.m_data[7] + a.m_data[6] * b.m_data[11] + a.m_data[7] * b.m_data[15];
	m.m_data[8] = a.m_data[8] * b.m_data[0] + a.m_data[9] * b.m_data[4] + a.m_data[10] * b.m_data[8] + a.m_data[11] * b.m_data[12];
	m.m_data[9] = a.m_data[8] * b.m_data[1] + a.m_data[9] * b.m_data[5] + a.m_data[10] * b.m_data[9] + a.m_data[11] * b.m_data[13];
	m.m_data[10] = a.m_data[8] * b.m_data[2] + a.m_data[9] * b.m_data[6] + a.m_data[10] * b.m_data[10] + a.m_data[11] * b.m_data[14];
	m.m_data[11] = a.m_data[8] * b.m_data[3] + a.m_data[9] * b.m_data[7] + a.m_data[10] * b.m_data[11] + a.m_data[11] * b.m_data[15];
	m.m_data[12] = a.m_data[12] * b.m_data[0] + a.m_data[13] * b.m_data[4] + a.m_data[14] * b.m_data[8] + a.m_data[15] * b.m_data[12];
	m.m_data[13] = a.m_data[12] * b.m_data[1] + a.m_data[13] * b.m_data[5] + a.m_data[14] * b.m_data[9] + a.m_data[15] * b.m_data[13];
	m.m_data[14] = a.m_data[12] * b.m_data[2] + a.m_data[13] * b.m_data[6] + a.m_data[14] * b.m_data[10] + a.m_data[15] * b.m_data[14];
	m.m_data[15] = a.m_data[12] * b.m_data[3] + a.m_data[13] * b.m_data[7] + a.m_data[14] * b.m_data[11] + a.m_data[15] * b.m_data[15];

	return m;

}		

inline Matrix4x4 Frustum(Float left,Float right,Float bottom, Float top, Float near, Float far)
{
	Matrix4x4 m;
	m.m_data[0] = (2 * near) / (right - left);
	m.m_data[8] = (right + left) / (right - left);
	
	m.m_data[5] = (2 * near) / (top - bottom);
	m.m_data[9] = (top + bottom) / (top - bottom);

	m.m_data[10] = -1 * (far + near) / (far - near);
	m.m_data[14] = -2 * far * near / (far - near);

	m.m_data[11] = -1;
	
	return m;	
}

inline Matrix4x4 Ortho(Float left,Float right, Float bottom, Float top, Float near, Float far)
{
	Matrix4x4 m;
	m.m_data[0] = 2 / (right - left);
	m.m_data[5] = 2 / (top - bottom);
	m.m_data[10] = -2 / (far - near);
	m.m_data[12] = -(right + left) / (right - left);
	m.m_data[13] = -(top + bottom) / (top - bottom);
	m.m_data[14] = -(far + near) / (far - near);
	m.m_data[15] = 1;

	return m;
}
#endif

