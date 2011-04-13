//******************************************************************
//测试用库
//******************************************************************
#pragma once

//数字库 在这个库里描述有关数字的东西
//未测试


#ifdef USE_DYNAMIC_NUMBER
#else

template< int n, typename Value >
class TypeLDivT;

template< typename Value >
class TypeT
{
public:

	typedef TypeT< Value > Type;

	TypeT( Value v ) : _v( v ){}
	TypeT( const Type & rhs ) : _v( rhs._v ){}
	Type & operator = ( const Type & rhs )
	{
		_v = rhs._v;
	}

	Type & operator += ( const Type & rhs )
	{
		_v += rhs._v;
		return ( *this );
	}

	Type & operator -= ( const Type & rhs )
	{
		_v -= rhs._v;
		return ( *this );
	}

	Type & operator *= ( const float & rhs )
	{
		_v *= rhs;
		return ( *this );
	}

	Type & operator /= ( const float & rhs )
	{
		_v /= rhs;
	}

	const Type times( const float t )
	{
		return _v * t;
	}

	const Value GetValue() const { return _v; }
private:


	friend
		const Type operator + ( const Type lhs, const Type rhs )
	{
		return lhs._v + rhs._v;
	}
	friend
		const Type operator - ( const Type lhs, const Type rhs )
	{
		return lhs._v - rhs._v;
	}
	
	Value _v;
};

template< int n, typename Value >
class TypeLDivT
{
public:
	typedef TypeLDivT< n, Value > Type;
	TypeLDivT( Value v ) : _v( v ){}
	TypeLDivT( const Type & rhs ) : _v( rhs._v ){}
	Type & operator = ( const Type & rhs )
	{
		_v = rhs._v;
	}

	Type & operator += ( Type & rhs )
	{
		_v += rhs._v;
		return ( *this );
	}

	Type & operator -= ( Type & rhs )
	{
		_v -= rhs._v;
		return ( *this );
	}

	Type & operator *= ( float & rhs )
	{
		_v *= rhs;
		return ( *this );
	}

	Type & operator /= ( float & rhs )
	{
		_v /= rhs;
	}

	const Type times( float t ) const
	{
		return _v * t;
	}

	const Value GetValue() const{ return _v; }

private:
	Value _v;


	friend
		const Type operator + ( const Type lhs, const Type rhs )
	{
		return lhs._v + rhs._v;
	}
	friend
		const Type operator - ( const Type lhs, const Type rhs )
	{
		return lhs._v - rhs._v;
	}
	friend
		const TypeLDivT< n - 1, Value > operator * ( const Type lhs, const TypeT< Value > rhs )
	{
		return lhs._v * rhs.GetValue();
	}
	friend
		const TypeLDivT< n - 1, Value > operator * ( const TypeT< Value > lhs, const Type rhs )
	{
		return lhs._v * rhs.GetValue();
	}

	friend
		const TypeLDivT< n + 1, Value > operator / ( const Type lhs, const TypeT< Value > rhs )
	{
		return lhs._v / rhs.GetValue();
	}
};


typedef TypeT< float > float_t;
typedef TypeLDivT< 0 ,float > float_l;
typedef TypeLDivT< 1 ,float > float_v;
typedef TypeLDivT< 2 ,float > float_a;

#endif