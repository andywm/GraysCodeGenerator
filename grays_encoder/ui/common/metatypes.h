/*------------------------------------------------------------------------------
	()      File:   metatypes.h
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
				Utility interface to add a metadata class to Qt models so you
				can more easily write delegates which have different types for
				the fields.
------------------------------
------------------------------
License Text - The MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

------------------------------------------------------------------------------*/
#pragma once
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "utility/bits_helper.h"
#include "utility/types_helper.h"
#include "qtmetamacros.h"
#include "QObject"
#include <stdint.h>
#include "QVariant"
#include "qcontainerfwd.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forwards
//------------------------------------------------------------------------------
class QAbstractItemModel;
class ImplmentItemMetadata;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
enum class MetadataType
{
	Undefined,
	Title,
	Number,
	Decimal,
	String,
	Enumeration,
	Boolean,
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace meta_attribs
{
	using attribute = uint32_t;
	enum MetadataAttributes
	{
		RangeMin = SET_ENUM_BIT(0),
		RangeMax = SET_ENUM_BIT(1),
		ReadOnly = SET_ENUM_BIT(2),
	};
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
struct EnumDisplayPair
{
	QString display_value;
	uint32_t numeric_value;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// ItemMetadata
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class ItemMetadata
{
	friend class ImplmentItemMetadata;

public:
	ItemMetadata() = default;
	ItemMetadata( MetadataType type, bool readOnly = false );

	ItemMetadata( int llim, int ulim, bool readOnly = false );
	ItemMetadata( float llim, float ulim, bool readOnly = false );
	ItemMetadata( const std::vector<EnumDisplayPair>& enums );

	MetadataType GetDataType() const;
	meta_attribs::attribute GetAttributes() const;

	const std::vector<EnumDisplayPair>& GetEnumeration() const;

	template<typename T>
	T GetLowerLimit() const
	{
		assert( 0 );
		return T();
	};

	template<>
	float GetLowerLimit<float>() const
	{
		return std::get<float>( m_lowerLimit );
	};

	template<>
	int GetLowerLimit<int>() const
	{
		return std::get<int>( m_lowerLimit );
	};

	template<typename T>
	T GetUpperLimit() const
	{
		assert( 0 );
		return T();
	}

	template<>
	float GetUpperLimit<float>() const
	{
		return std::get<float>( m_upperLimit );
	};

	template<>
	int GetUpperLimit<int>() const
	{
		return std::get<int>( m_upperLimit );
	};

private:
	MetadataType m_dataType = MetadataType::Undefined;
	meta_attribs::attribute m_attributeFlags = 0;
	std::vector<EnumDisplayPair> m_enums;
	std::variant<float, int> m_lowerLimit;
	std::variant<float, int> m_upperLimit;
	bool m_readOnly;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// ItemPropertyMetadata
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class ImplmentItemMetadata
{
public:
	ImplmentItemMetadata( const std::vector<ItemMetadata>& metadata );
	const ItemMetadata* Get( int col ) const;
	ItemMetadata* Get( int col );

private:
	std::vector<ItemMetadata> m_metadata;
};


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// MetadataInterface
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class MetadataInterface
{

public:
	static const ItemMetadata* GetMetadata( const QModelIndex& index );
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// MetadataInterfaceT
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template<class Type>
class MetadataInterfaceT: public MetadataInterface
{
public:
	MetadataInterfaceT()
	{
		static_assert(is_base<ImplmentItemMetadata, Type>::value, "Data must inherit ImplmentItemMetadata");
	}
};
