#ifndef __THREEMATH_H__
#define __THREEMATH_H__

#include <math.h>
#include <string.h>

#define Float float
#define Int int


const Float kPi = 3.141592653f;
const Float k2Pi = kPi * 2.0f;
const Float kPiOver2 = kPi / 2.0f;
const Float k1OverPi = 1 / kPi;

#define RADIAN(x) (x * kPi / 180)

template<typename T>
T SafeAcos(T x);

/**************************Vector2*********************/
template <typename T>
class Vector2_
{
	public:
		T x;
		T y;
		
		Vector2_():
			x(0),
			y(0)
		{

		}

		Vector2_(T x, T y)
		{
			this->x = x;
			this->y = y;
		}

		Vector2_(const Vector2_ &other)
		{
			x = other.x;
			y = other.y;
		}
};

/**********************************Vector3**************************/
template<typename T>
class Vector3_
{

	public:
		T x;
		T y;
		T z;
		
		Vector3_():
			x(0),
			y(0),
			z(0)
		{
		}

		Vector3_(T val):
			x(val),
			y(val),
			z(val)
		{
			
		}

		Vector3_(T nx,T ny,T nz):
			x(nx),
			y(ny),
			z(nz)
		{

		}
		
		Vector3_(const Vector3_ &other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
		}


		void Set(T nx,T ny,T nz)
		{
			x = nx;
			y = ny;
			z = nz;
		}


		T Length() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		T SqrLength() const
		{
			return x * x + y * y + z * z;
		}

		void Normalized()
		{
			Vector3_<T> tmp;
			T len = Length();
			if(len > 0)
			{
				x /= len;
				y /= len;
				z /= len;
			}
		}

		static T Dot(Vector3_ a, Vector3_ b)
		{
			return a.x * b.x
				+ a.y * b.y
				+ a.z * b.z;
		}

		static Vector3_ Cross(Vector3_ a, Vector3_ b)
		{
			return Vector3_(a.y * b.z - a.z * b.y,
					a.z * b.x - a.x * b.z,
					a.x * b.y - a.y * b.x);
		}

		Vector3_ operator =(const Vector3_ &other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}


		bool operator==(const Vector3_ &other) const
		{
			return x == other.x && y == other.y && z== other.z;
		}

		bool operator !=(const Vector3_ &other) const
		{
			return x!=other.x || y != other.y || z!=other.z;
		}

		Vector3_ operator +=(const Vector3_ &other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;
		}

		Vector3_ operator -=(const Vector3_ &other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;
		}
};

template<typename T>
inline Vector3_<T> operator+(Vector3_<T> lvalue,Vector3_<T> rvalue)
{
	Vector3_<T> temp(lvalue.x + rvalue.x,  lvalue.y + rvalue.y,  lvalue.z + rvalue.z);
	return temp;

}

template<typename T>
inline Vector3_<T> operator-(Vector3_<T> lvalue, Vector3_<T> rvalue)
{			
	Vector3_<T> temp(lvalue.x - rvalue.x, lvalue.y - rvalue.y, lvalue.z - rvalue.z);
	return temp;
}

template<typename T>
inline Vector3_<T> operator*(Vector3_<T> value, T x)
{
	Vector3_<T> temp(value.x * x, value.y * x, value.z * x);
	return temp;
}

// template<typename T>
// inline Vector3_<T> operator*(Vector3_<T> lvalue,Vector3_<T> rvalue)
// {
// 	return Vector3_<T>(lvalue.x * rvalue.x, lvalue.y * rvalue.y, lvalue.z * rvalue.z);
// }

template<typename T>
inline Vector3_<T> operator*(const Vector3_<T> &lvalue,const Vector3_<T> &rvalue)
{
	return Vector3_<T>(lvalue.x * rvalue.x, lvalue.y * rvalue.y, lvalue.z * rvalue.z);
}

template<typename T>
inline T Distance(const Vector3_<T> &a, const Vector3_<T> &b)
{
	return (a - b).Length();
}



/*******************************Quaternion_*********************/
template<typename T>
class Quaternion_
{
	public:
		Quaternion_():
			x(0),
			y(0),
			z(0),
			w(1)
		{
		}

		Quaternion_(T x, T y, T z, T w):
			x(x),
			y(y),
			z(z),
			w(w)
		{
		}	

		Quaternion_(const Quaternion_ &other):
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
			T mag = (T)sqrt(x * x + y * y + z * z + w * w);
			if(mag > 0.0f)
			{
				T oneOverMag = 1.0f / mag;
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
				

		T GetEulerAngle()
		{
			//计算半角
			T thetaOver2 = SafeAcos(w);
			return thetaOver2 * 2 * 180;
		}

		Vector3_<T> GetRotationAxis()
		{
			//计算sin^(theta/2)
			
			T sinThetaOver2Sq = 1.0f - w * w;
			//注意保持数值精度
			if(sinThetaOver2Sq <= 0.0f)
			{
				//单位四元数或不精确的数值，只要返回有效的向量即可
				return Vector3_<T>(1.0,0,0);

			}
			
			T oneOverSinThetaOver2 = 1.0f / sqrt(sinThetaOver2Sq);

			return Vector3_<T>( x * oneOverSinThetaOver2,
					y * oneOverSinThetaOver2,
					z * oneOverSinThetaOver2);
		}

		static Quaternion_ Euler(T x, T y, T z)
		{
			T rx = RADIAN(x / 2);
			T ry = RADIAN(y / 2);
			T rz = RADIAN(z / 2);

			T sinx = sin(rx);
			T cosx = cos(rx);
			T siny = sin(ry);
			T cosy = cos(ry);
			T sinz = sin(rz);
			T cosz = cos(rz);

			return Quaternion_(cosy * sinx * cosz + siny * cosx * sinz,
				siny * cosx * cosz - cosy * sinx * sinz,
				cosy * cosx * sinz - siny * sinx *cosz,
				cosy * cosx * cosz + siny * sinx * sinz);
		}

		Quaternion_ Angle(Quaternion_ a, Quaternion_ y)
		{
			T len = sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);

			Quaternion_ conjugate = a.Conjugate();

			return conjugate * y * (1.0f / len);
		}
		
		Quaternion_ Conjugate() const
		{
			return Quaternion_(-x,-y,-z,w);
		}

		void operator=(const Quaternion_ &other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		
		static Quaternion_ Slerp(Quaternion_ start, Quaternion_ end, T p)
		{
			if( p <= 0.0f)
			{
				return start;
			}

			if(p >= 1.0f)
			{
				return end;
			}

			T cosOmega = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;
			if(cosOmega < 0.0f)
			{
				end = end.Conjugate();
				cosOmega = -cosOmega;
			}

			T k0,k1;
			if(cosOmega > 0.9999f)
			{
				//非常接近----使用线性插值
				k0=1.0f - p;
				k1 = p;
			}
			else
			{
				//用三角公式sin^2(omega) + cos^(omega) = 1 计算sin值
				T sinOmega = sqrt(1.0f - cosOmega * cosOmega);
				T omega = atan2(sinOmega,cosOmega);
				T oneOverSinOmega = 1.0f / sinOmega;

				k0 = sin((1.0f - p) * omega) * oneOverSinOmega;
				k1 = sin(p * omega) * oneOverSinOmega;
			}
			
			return Quaternion_(start.x * k0 + end.x * k1,
			start.y * k0 + end.y * k1,
			start.z * k0 + end.z * k1,
		       	start.w * k0 + end.w * k1);
		}

		Quaternion_ Pow(T exponent) const
		{
			if(w > 0.9999f)
			{
				return *this;
			}

			T alpha = acos(w);
			//计算新的alpha值
			T newAlpha = alpha * exponent;
			//计算新的w值
			Quaternion_ result;
			result.w = cos(newAlpha);

			T mult = sin(newAlpha) / sin(alpha);
			result.x = x * mult;
			result.y = y * mult;
			result.z = z * mult;

			return result;
		}

			

		friend Quaternion_<T> operator*(Quaternion_<T> a, Quaternion_<T> b);
		friend Quaternion_<T> operator*(Quaternion_<T> value, T a);
		friend Vector3_<T> operator*(Vector3_<T> v,Quaternion_<T> a);


	public:
		T x;
		T y;
		T z;
		T w;
};

//旋转a再旋转b不再是b*a,而是a*b，因为叉乘换了顺序
template<typename T>
inline Quaternion_<T> operator*(Quaternion_<T> a, Quaternion_<T> b)
{
	return Quaternion_<T>(a.w * b.x + a.x * b.w + a.z * b.y - a.y * b.z,
			a.w * b.y + a.y * b.w + a.x * b.z - a.z * b.x,
			a.w * b.z + a.z * b.w + a.y * b.x - a.x * b.y,
			a.w * b.w - a.x * b.x -a.y * b.y - a.z * b.z);
}

template<typename T>
inline Quaternion_<T> operator*(Quaternion_<T> value, T a) 
{
	return Quaternion_<T>(value.x * a, value.y * a, value.z * a, value.w * a);
}

template<typename T>
inline Vector3_<T> operator*(Vector3_<T> v,Quaternion_<T> a)
{
	Quaternion_<T> q(v.x,v.y,v.z,0);
	Quaternion_<T> res = q * a;
	return Vector3_<T>(res.x,res.y,res.z);
}
		       	
/*********************************Matrix3x3*****************************/

template<typename T>
class Matrix4x4_;

template<typename T>
class Matrix3x3_
{
	using length_t = int;
	public:
		Matrix3x3_()
		{
			memset(m_data,0,9);
		}

		Matrix3x3_(const Matrix4x4_<T>& mat4)
		{
			m_data[0] = mat4[0];
			m_data[1] = mat4[1];
			m_data[2] = mat4[2];
			m_data[3] = mat4[4];
			m_data[4] = mat4[5];
			m_data[5] = mat4[6];
			m_data[6] = mat4[8];
			m_data[7] = mat4[9];
			m_data[8] = mat4[10];
		}

		inline Matrix3x3_ Identity()
		{
			memset(m_data,0,9);
			m_data[0] = 1;
			m_data[4] = 1;
			m_data[8] = 1;
		}

		T* Get()
		{
			return m_data;
		}

		T& operator[](length_t index)
		{
			return m_data[index];
		}

		const T& operator[](length_t index) const
		{
			return m_data[index];
		}

	private:
		T m_data[9];
};

/************************************Matrix4x4**************************************/
template<typename T>
class Matrix4x4_
{
	public:

		using lengh_t = int;

		Matrix4x4_()
		{
			for(int i=0;i < 16; ++i)
			{
				m_data[i] = 0;
			}
		}

		Matrix4x4_(const Matrix3x3_<T>& mat3)
		{
			m_data[0] = mat3[0];
			m_data[4] = mat3[3];
			m_data[8] = mat3[6];
			m_data[1] = mat3[1];
			m_data[5] = mat3[4];
			m_data[9] = mat3[7];
			m_data[2] = mat3[2];
			m_data[6] = mat3[5];
			m_data[10] = mat3[8];

			m_data[3] = 0;
			m_data[7] = 0;
			m_data[11] = 0;
			m_data[12] = 0;
			m_data[13] = 0;
			m_data[14] = 0;
			m_data[15] = 0;
		}

		~Matrix4x4_()
		{
		}

		inline static Matrix4x4_ Identity()
		{
			Matrix4x4_ m;
			m.m_data[0] = m.m_data[5] = m.m_data[10] = m.m_data[15] = 1;
			return m;
		}

		//使用左手法则
		void Rotate(const Vector3_<T> &axis,T theta)
		{
			T c = cos(RADIAN(theta));
			T s = sin(RADIAN(theta));
			T a = 1.0f - c;
			T ax = a * axis.x;
			T ay = a * axis.y;
			T az = a * axis.z;

			Matrix4x4_ m;
			m.m_data[0] = ax * axis.x + c;
			m.m_data[4] = ax * axis.y + axis.z * s;
			m.m_data[8] = ax * axis.z - axis.y * s;
			m.m_data[12] = 0;
			m.m_data[1] = ay * axis.x  - axis.z * s;
			m.m_data[5] = ay * axis.y  + c;
			m.m_data[9] = ay * axis.z  + axis.x * s;
			m.m_data[13] = 0;
			m.m_data[2] = az * axis.x + axis.y * s;
			m.m_data[6] = az * axis.y - axis.x * s;
			m.m_data[10] = az * axis.z + c;
			m.m_data[14] = 0;

			m.m_data[3] = 0;
			m.m_data[7] = 0;
			m.m_data[11] = 0;
			m.m_data[15] = 1;

			*this *= m;
		}

		void Scale(T s)
		{
			m_data[0] *=s;
			m_data[5] *=s;
			m_data[10] *= s;		
		}

		void Translate(const Vector3_<T> &t)
		{
			m_data[12] += t.x;
			m_data[13] += t.y;
			m_data[14] += t.z;
		}

		void Translate(T x,T y,T z)
		{
			m_data[12] += x, 
			m_data[13] += y;
			m_data[14] += z;
		}


		T* Get()
		{
			return m_data;
		}


		void operator=(const Matrix4x4_ &other)
		{
			for(int i = 0; i < 16; ++i)
			{
				m_data[i] = other.m_data[i];
			}
		}


		Matrix4x4_ operator*=(const Matrix4x4_ &other)
		{
			*this = *this * other;
			return *this;
		}

		T& operator[](lengh_t i)
		{
			return m_data[i];
		}

		const T& operator[](lengh_t i) const
		{
			return m_data[i];
		}


	private:
		T m_data[16];
};

//a * b transfrom is "b" transfrom first,not a
template<typename T>
inline Matrix4x4_<T> operator*(Matrix4x4_<T> const &a, Matrix4x4_<T> const &b)
{
	Matrix4x4_<T> m;
	m[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
	m[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
	m[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
	m[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];
	m[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];
	m[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];
	m[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
	m[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];
	m[8] = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];
	m[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];
	m[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
	m[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11];
	m[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];
	m[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];
	m[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
	m[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];

	return m;

}		

template<typename T>
inline Vector3_<T> operator*(const Matrix4x4_<T> &a, const Vector3_<T> &v)
{
	Vector3_<T> tv;
	tv.x = a[0] * v.x + a[4] * v.y + a[8] * v.z + a[12] * 1;
	tv.y = a[1] * v.x + a[5] * v.y + a[9] * v.z + a[13] * 1;
	tv.z = a[2] * v.x + a[6] * v.y + a[10] * v.z + a[14] * 1;
	return tv;
}

//左手坐标系
template<typename T>
inline Matrix4x4_<T> Frustum(T left,T right,T bottom, T top, T near, T far)
{
	Matrix4x4_<T> m;
	m[0] = (2 * near) / (right - left);
	m[8] = (right + left) / (right - left);
	
	m[5] = (2 * near) / (top - bottom);
	m[9] = (top + bottom) / (top - bottom);

	m[10] = -1 * (far + near) / (far - near);
	m[14] = -2 * far * near / (far - near);

	m[11] = -1;
	
	return m;	
}

template<typename T>
inline Matrix4x4_<T> Perspective(T fovy,T aspect, T near,T far)
{
	T top = near * tan(fovy / 2.0f);
	T right = top * aspect;
	return Frustum(-right,right,-top,top,near,far);
}

template<typename T>
inline Matrix4x4_<T> Ortho(T left,T right, T bottom, T top, T near, T far)
{
	Matrix4x4_<T> m;
	m[0] = 2 / (right - left);
	m[5] = 2 / (top - bottom);
	m[10] = -2 / (far - near);
	m[12] = -(right + left) / (right - left);
	m[13] = -(top + bottom) / (top - bottom);
	m[14] = -(far + near) / (far - near);
	m[15] = 1;

	return m;
}

template<typename T>
inline Matrix4x4_<T> LookAt(Vector3_<T> position,Vector3_<T> target,Vector3_<T> up)
{
	Vector3_<T> front = target - position;
	front.Normalized();
	Vector3_<T> right = Vector3_<T>::Cross(front,up);
	right.Normalized();
	up = Vector3_<T>::Cross(right,front);
	up.Normalized();

	Matrix4x4_<T> mat = Matrix4x4_<T>::Identity();
	mat[0] = right.x;
	mat[4] = right.y;
	mat[8] = right.z;

	mat[1] = up.x;
	mat[5] = up.y;
	mat[9] = up.z;

	mat[2] = -front.x;
	mat[6] = -front.y;
	mat[10] = -front.z;

	mat[12] = -Vector3_<T>::Dot(position,right);
	mat[13] = -Vector3_<T>::Dot(position,up);
	mat[14] = Vector3_<T>::Dot(position,front);

	return mat;
}


using Vector2 = Vector2_<float>;
using Vector3 = Vector3_<float>;
using Matrix4x4 = Matrix4x4_<float>;
using Matrix3x3 = Matrix3x3_<float>;

#endif


