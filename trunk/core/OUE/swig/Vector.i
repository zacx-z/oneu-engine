%module(directors = "1") OUE

namespace OneU
{
	template<typename T>
	struct rect_t
	{
		T left, top, right, bottom;
		rect_t(){}
		rect_t(T left, T top, T right, T bottom)
			: left(left), top(top), right(right), bottom(bottom){}
	};

	template<typename T>
	struct Vector2
	{
		T x, y;
		Vector2(){}
		Vector2(T x, T y) : x(x), y(y){}
		template<typename U>
		Vector2(Vector2<U>& rhs) : x((T)rhs.x), y((T)rhs.y){}
		T length(){ return sqrt(x * x + y * y);}
	};
	template<class T>
	inline Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs){
		return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
	}
	template<class T>
	inline Vector2<T> operator-(const Vector2<T>& lhs){
		return Vector2<T>(-lhs.x, -lhs.y);
	}
	template<class T>
	inline Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs){
		return lhs + (-rhs);
	}
	template<class T>
	inline Vector2<T> operator*(T lhs, const Vector2<T>& rhs){
		return Vector2<T>(lhs * rhs.x, lhs * rhs.y);
	}
	template<class T>
	inline Vector2<T> operator*(const Vector2<T>& lhs, T rhs){
		return rhs * lhs;
	}
	template<class T>
	inline Vector2<T> operator/(const Vector2<T>& lhs, T rhs){
		return Vector2<T>(lhs.x / rhs, lhs.y / rhs);
	}
	template<class T>
	inline T operator*(const Vector2<T>& lhs, const Vector2<T>& rhs){
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	template<typename T>
	struct Vector3
	{
		T x, y, z;
		Vector3(){}
		Vector3(T x, T y, T z) : x(x), y(y), z(z){}
		template<typename U>
		Vector3(Vector3<U>& rhs) : x((T)rhs.x), y((T)rhs.y), z((T)rhs.z){}
		T length(){ return sqrt(x * x + y * y); }

	};
	template<class T>
	inline Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs){
		return Vector3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}
	template<class T>
	inline Vector3<T> operator-(const Vector3<T>& lhs){
		return Vector3<T>(-lhs.x, -lhs.y, -lhs.z);
	}
	template<class T>
	inline Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs){
		return lhs + (-rhs);
	}
	template<class T>
	inline Vector3<T> operator*(T lhs, const Vector3<T>& rhs){
		return Vector3<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
	}
	template<class T>
	inline Vector3<T> operator*(const Vector3<T>& lhs, T rhs){
		return rhs * lhs;
	}
	template<class T>
	inline Vector3<T> operator/(const Vector3<T>& lhs, T rhs){
		return Vector3<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
	}
	template<class T>
	inline T operator*(const Vector3<T>& lhs, const Vector3<T>& rhs){
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	template<typename T>
	struct Vector4
	{
		T x, y, z, w;
		Vector4(){}
		Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w){}
	};


	%template(Recti) rect_t<int>;
	%template(Rectf)    rect_t<float>;

	%template(Vector2i) Vector2<int>;
	%template(Vector2f)    Vector2<float>;

	%template(Vector3i) Vector3<int>;
	%template(Vector3f)    Vector3<float>;

	%template(Vector4i) Vector4<int>;
	%template(Vector4f)    Vector4<float>;
	
	typedef rect_t<int> recti_t;
	typedef rect_t<float> rect;

	typedef Vector2<int> vector2i_t;
	typedef Vector2<float> vector2;

	typedef Vector3<int> vector3i_t;
	typedef Vector3<float> vector3;

	typedef Vector4<int> vector4i_t;
	typedef Vector4<float> vector4;
}
