/*------------------------------------------------------------------------------
	()      File:   properties_entry.cpp
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
				
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
#include "ui/properties_menu/properties_entry.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// PropertiesEntry
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
PropertiesEntry::PropertiesEntry( const std::vector<QVariant>& data, const std::vector<ItemMetadata>& metadata, PropertiesEntry* parent /*= nullptr*/ )
	: ImplmentItemMetadata( metadata )
	, m_itemData( data )
	, m_parent( parent )
{

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
PropertiesEntry::~PropertiesEntry()
{
	qDeleteAll( m_children );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PropertiesEntry::AppendChild( PropertiesEntry* child )
{
	m_children.push_back( child );
	child->m_parent = this;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
PropertiesEntry* PropertiesEntry::GetChild( int row )
{
	if ( row < 0 || row >= m_children.size() )
	{
		return nullptr;
	}

	return m_children.at( row );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int PropertiesEntry::GetChildCount() const
{
	return m_children.size();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int PropertiesEntry::GetColumnCount() const
{
	return m_itemData.size();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
QVariant PropertiesEntry::GetData( int column ) const
{
	if ( column < 0 || column >= m_itemData.size() )
	{
		return QVariant();
	}

	return m_itemData.at( column );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int PropertiesEntry::GetRow() const
{
	if ( m_parent )
	{
		auto selfIter = std::find( m_parent->m_children.begin(), m_parent->m_children.end(), this );
		if( selfIter != m_parent->m_children.end() )
		{
			return std::distance( m_parent->m_children.begin(), selfIter );
		}	
	}

	return 0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
PropertiesEntry* PropertiesEntry::GetParentItem() const
{
	return m_parent;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool PropertiesEntry::SetData( int column, const QVariant& value )
{
	if ( column < 0 || column >= m_itemData.size() )
	{
		return false;
	}

	m_itemData[column] = value;

	//data changed
	m_listeners.Invoke( value );

	return true;
}
