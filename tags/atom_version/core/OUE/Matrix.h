/*
This source file is part of OneU Engine.
Copyright (c) 2011 Ladace

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/**
 * @file Matrix.h
 * @brief 矩阵
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "OUEDefs.h"
#include "Vector.h"
#include "Math.h"
#include <cmath>

//矩阵右乘顶点进行变换（目前验证D3D中如此）左边矩阵先变换

namespace OneU
{
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 4阶矩阵
	 *
	 * @tparam T 矩阵元素的类型，要求能做数学运算。
	 *
	 */
	/* ----------------------------------------------------------------------------*/
	template<typename T>
	class Matrix4
	{
		T M[16];
	public:
		enum Constructor{
			NOTHING,
			IDENTITY
		};
		Matrix4(Constructor flag = NOTHING){
			switch(flag){
			case NOTHING:
				break;
			case IDENTITY:
				setIdentity();
				break;
			}
		}
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 获取row行col列元素
		 *
		 * @param row 行
		 * @param col 列
		 *
		 * @return row行col列元素
		 */
		/* ----------------------------------------------------------------------------*/
		T& operator()(int row, int col){ return M[row * 4 + col]; }
		const T& operator()(int row, int col) const{ return M[row * 4 + col]; }
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 获取内存上第index个元素
		 *
		 * 矩阵的16个元素分布在连续内存上。该函数用于获取其index个元素。
		 *
		 * @param index 元素序数
		 *
		 * @return 第index个元素
		 */
		/* ----------------------------------------------------------------------------*/
		T& operator[](uint32 index){ return M[index]; }
		const T& operator[](uint32 index) const{ return M[index]; }

		T* data(){ return M; }
		const T* data() const{ return M;}

		Matrix4<T>& operator=(const Matrix4<T>& rhs){
			memcpy(M, rhs.M, sizeof(rhs.M));
			return *this;
		}
		bool operator==(const Matrix4<T>& rhs) const{
			if(
				_scalar_traits<T>::isEqual(M[0][0], rhs.M[0][0]) && _scalar_traits<T>::isEqual(M[0][1], rhs.M[0][1]) && _scalar_traits<T>::isEqual(M[0][2], rhs.M[0][2]) && _scalar_traits<T>::isEqual(M[0][3], rhs.M[0][3]) &&
				_scalar_traits<T>::isEqual(M[1][0], rhs.M[1][0]) && _scalar_traits<T>::isEqual(M[1][1], rhs.M[1][1]) && _scalar_traits<T>::isEqual(M[1][2], rhs.M[1][2]) && _scalar_traits<T>::isEqual(M[1][3], rhs.M[1][3]) &&
				_scalar_traits<T>::isEqual(M[2][0], rhs.M[2][0]) && _scalar_traits<T>::isEqual(M[2][1], rhs.M[2][1]) && _scalar_traits<T>::isEqual(M[2][2], rhs.M[2][2]) && _scalar_traits<T>::isEqual(M[2][3], rhs.M[2][3]) &&
				_scalar_traits<T>::isEqual(M[3][0], rhs.M[3][0]) && _scalar_traits<T>::isEqual(M[3][1], rhs.M[3][1]) && _scalar_traits<T>::isEqual(M[3][2], rhs.M[3][2]) && _scalar_traits<T>::isEqual(M[3][3], rhs.M[3][3])
				)
				return true;
			return false;
		}

		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 赋值为单位矩阵
		 *
		 * @return 自己
		 */
		/* ----------------------------------------------------------------------------*/
		inline Matrix4& setIdentity(){
			memset(M, 0, sizeof(M));
			M[0] = M[5] = M[10] = M[15] = (T)1;
			return *this;
		}
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 判断是否为单位矩阵
		 *
		 * @retval true 是单位矩阵。
		 * @retval false 不是单位矩阵。
		 */
		/* ----------------------------------------------------------------------------*/
		inline bool isIdentity() const;

		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 将两个矩阵的乘积赋值到该矩阵。
		 *
		 * @param lhs 乘法左操作数
		 * @param rhs 乘法右操作数
		 *
		 * @remarks 操作数不能为自己。
		 *
		 * @return 自己
		 */
		/* ----------------------------------------------------------------------------*/
		inline Matrix4& evalProduct(const Matrix4& lhs, const Matrix4& rhs);

		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 设为平移矩阵
		 *
		 * @param translation 平移向量
		 *
		 * @return 自己
		 */
		/* ----------------------------------------------------------------------------*/
		inline Matrix4& setTranslation(const Vector3<T>& translation);
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 设为缩放矩阵
		 *
		 * @param scale 缩放向量
		 *
		 * 每个分量代表对应方向上缩放的倍数。
		 *
		 * @return 自己
		 */
		/* ----------------------------------------------------------------------------*/
		inline Matrix4& setScale(const Vector3<T>& scale);
		//旋转为角度（不是弧度）
		//顺时针方向为正
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 设为旋转矩阵
		 *
		 * @param rotation
		 *
		 * @return 
		 */
		/* ----------------------------------------------------------------------------*/
		inline Matrix4& setRotation(const Vector3<T>& rotation);
		inline Matrix4& setRotationRadians(const Vector3<T>& rotation);
	};

	template<typename T>
	Matrix4<T>& Matrix4<T>::evalProduct(const Matrix4& lhs, const Matrix4& rhs){
		const T* m1 = rhs.M;
		const T* m2 = lhs.M;

		M[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		M[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		M[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		M[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

		M[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		M[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		M[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		M[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

		M[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		M[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		M[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		M[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

		M[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		M[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		M[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		M[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
		return *this;
	}

	template<typename T>
	Matrix4<T>& Matrix4<T>::setTranslation(const Vector3<T>& translation){
		setIdentity();
		M[12] = translation.x;
		M[13] = translation.y;
		M[14] = translation.z;
		return *this;
	}
	template<typename T>
	Matrix4<T>& Matrix4<T>::setScale(const Vector3<T>& scale){
		setIdentity();
		M[0] = scale.x;
		M[5] = scale.y;
		M[10] = scale.z;
		return *this;
	}
	template<typename T>
	Matrix4<T>& Matrix4<T>::setRotation(const Vector3<T>& rotation){
		return setRotationRadians(rotation * (3.14159265359f / 180));
	}
	template<typename T>
	Matrix4<T>& Matrix4<T>::setRotationRadians(const Vector3<T>& rotation){
		setIdentity();
		const double cr = cos(rotation.x);
		const double sr = sin(rotation.x);
		const double cp = cos(rotation.y);
		const double sp = sin(rotation.y);
		const double cy = cos(rotation.z);
		const double sy = sin(rotation.z);

		M[0] = (T)(cp * cy);
		M[1] = (T)(cp * sy);
		M[2] = (T)(-sp);

		const double srsp = sr * sp;
		const double crsp = cr * sp;

		M[4] = (T)(srsp * cy - cr * sy);
		M[5] = (T)(srsp * sy + cr * cy);
		M[6] = (T)(sr * cp);

		M[8] = (T)(crsp * cy + sr * sy);
		M[9] = (T)(crsp * sy - sr * cy);
		M[10] = (T)(cr * cp);

		return *this;
	}

	template<typename T>
	inline Matrix4<T> operator*(const Matrix4<T>& lhs, const Matrix4<T>& rhs){
		return Matrix4<T>().evalProduct(lhs, rhs);
	}
	template<typename T>
	inline bool Matrix4<T>::isIdentity() const{
		if(
			_scalar_traits<T>::isEqual(M[0][0], 1) && _scalar_traits<T>::isEqual(M[0][1], 0) && _scalar_traits<T>::isEqual(M[0][2], 0) && _scalar_traits<T>::isEqual(M[0][3], 0) &&
			_scalar_traits<T>::isEqual(M[1][0], 0) && _scalar_traits<T>::isEqual(M[1][1], 1) && _scalar_traits<T>::isEqual(M[1][2], 0) && _scalar_traits<T>::isEqual(M[1][3], 0) &&
			_scalar_traits<T>::isEqual(M[2][0], 0) && _scalar_traits<T>::isEqual(M[2][1], 0) && _scalar_traits<T>::isEqual(M[2][2], 1) && _scalar_traits<T>::isEqual(M[2][3], 0) &&
			_scalar_traits<T>::isEqual(M[3][0], 0) && _scalar_traits<T>::isEqual(M[3][1], 0) && _scalar_traits<T>::isEqual(M[3][2], 0) && _scalar_traits<T>::isEqual(M[3][3], 1)
			)
			return true;
		return false;
	}

	typedef Matrix4<float> matrix;


	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 获取单位矩阵
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API const matrix& GetIdentityMatrix();
}
