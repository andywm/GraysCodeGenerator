/*------------------------------------------------------------------------------
	()      File:   iproperty_menu.h
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
			  Generates an n Gray's encoding pattern
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

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "ui/common/metatypes.h"
#include "utility/bits_helper.h"
#include "QAbstractItemModel"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// ImplmentItemMetadata
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
ImplmentItemMetadata::ImplmentItemMetadata( const std::vector<ItemMetadata>& metadata )
	: m_metadata( metadata )
{

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
const ItemMetadata* ImplmentItemMetadata::Get( int col ) const
{
	auto size = m_metadata.size();
	if ( col < 0 || col >= size )
	{
		return nullptr;
	}
	return &m_metadata[col];
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
ItemMetadata* ImplmentItemMetadata::Get( int col )
{
	auto size = m_metadata.size();
	if ( col < 0 || col >= size )
	{
		return nullptr;
	}
	return &m_metadata[col];
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
ItemMetadata::ItemMetadata( MetadataType type, bool readOnly /*=false*/ )
	: m_dataType( type )
	, m_attributeFlags( TRY_SET_BIT( readOnly, meta_attribs::ReadOnly ) )
{
}

//------------------------------------------------------------------------------
// specialisation for NUMBER type with limits.
//------------------------------------------------------------------------------
ItemMetadata::ItemMetadata( int llim, int ulim, bool readOnly /*=false*/ )
	: m_dataType( MetadataType::Number )
	, m_attributeFlags( TRY_SET_BIT( readOnly, meta_attribs::ReadOnly )
		| TRY_SET_BIT( llim != INT32_MIN, meta_attribs::RangeMin )
		| TRY_SET_BIT( llim != INT32_MAX, meta_attribs::RangeMax ) )
	, m_lowerLimit( llim )
	, m_upperLimit( ulim )
{
}

//------------------------------------------------------------------------------
// specialisation for DECIMAL type with limits.
//------------------------------------------------------------------------------
ItemMetadata::ItemMetadata( float llim, float ulim, bool readOnly /*=false*/ )
	: m_dataType( MetadataType::Decimal )
	, m_attributeFlags( TRY_SET_BIT( readOnly, meta_attribs::ReadOnly )
		| TRY_SET_BIT( llim != FLT_MIN, meta_attribs::RangeMin )
		| TRY_SET_BIT( llim != FLT_MAX, meta_attribs::RangeMax ) )
	, m_lowerLimit( llim )
	, m_upperLimit( ulim )
{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
ItemMetadata::ItemMetadata( const std::vector<EnumDisplayPair>& enums )
	: m_dataType( MetadataType::Enumeration )
	, m_enums( enums )
{

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
const std::vector<EnumDisplayPair>& ItemMetadata::GetEnumeration() const
{
	return m_enums;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
MetadataType ItemMetadata::GetDataType() const
{
	return m_dataType;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
meta_attribs::attribute ItemMetadata::GetAttributes() const
{
	return m_attributeFlags;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
const ItemMetadata* MetadataInterface::GetMetadata( const QModelIndex& index )
{
	//if ( index.model()->inherits( ClassName( MetadataInterface ) ) )
	if ( dynamic_cast<const MetadataInterface*>(index.model()) )
	{
		ImplmentItemMetadata* data = reinterpret_cast<ImplmentItemMetadata*>(index.internalPointer());
		return data->Get( index.column() );
	}

	return nullptr;
}