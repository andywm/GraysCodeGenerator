/*------------------------------------------------------------------------------
	()      File:   properties_model.cpp
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
#include "ui/properties_menu/properties_model.h"
#include "ui/properties_menu/properties_entry.h"
#include "QVariant"
#include "QString"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// PropertiesModel
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
PropertiesModel::PropertiesModel( QObject* parent )
	: QAbstractItemModel( parent )
{
	rootItem = new PropertiesEntry( { tr( "Field" ), tr( "Value" ) }, {} );
}

PropertiesEntry& PropertiesModel::addEntry( const std::vector<QVariant>& data, const std::vector<ItemMetadata>& metadata )
{
	PropertiesEntry* child = new PropertiesEntry( data, metadata );
	rootItem->AppendChild( child );
	
	return *child;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PropertiesModel::setupModelData( const QStringList& qtLines, PropertiesEntry* parent )
{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
PropertiesModel::~PropertiesModel()
{
	delete rootItem;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
QModelIndex PropertiesModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( !hasIndex( row, column, parent ) )
	{
		return QModelIndex();
	}

	PropertiesEntry* parentItem = nullptr;

	if ( !parent.isValid() )
	{
		parentItem = rootItem;
	}
	else
	{
		parentItem = static_cast<PropertiesEntry*>(parent.internalPointer());
	}

	if ( PropertiesEntry* childItem = parentItem->GetChild( row ) )
	{
		return createIndex( row, column, childItem );
	}

	return QModelIndex();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
QModelIndex PropertiesModel::parent( const QModelIndex& index ) const
{
	if ( !index.isValid() )
	{
		return QModelIndex();
	}

	PropertiesEntry* childItem = static_cast<PropertiesEntry*>(index.internalPointer());
	PropertiesEntry* parentItem = childItem->GetParentItem();

	if ( parentItem == rootItem )
	{
		return QModelIndex();
	}

	return createIndex( parentItem->GetRow(), 0, parentItem );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int PropertiesModel::rowCount( const QModelIndex& parent ) const
{
	PropertiesEntry* parentItem = nullptr;
	if ( parent.column() > 0 )
	{
		return 0;
	}

	if ( !parent.isValid() )
	{
		parentItem = rootItem;
	}
	else
	{
		parentItem = static_cast<PropertiesEntry*>(parent.internalPointer());
	}

	return parentItem->GetChildCount();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int PropertiesModel::columnCount( const QModelIndex& parent ) const
{
	if( parent.isValid() )
	{
		return static_cast<PropertiesEntry*>(parent.internalPointer())->GetChildCount();
	}

	return rootItem->GetColumnCount();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
QVariant PropertiesModel::data( const QModelIndex& index, int role ) const
{
	if( !index.isValid()  || !(role == Qt::DisplayRole || role == Qt::EditRole) )
	{
		return QVariant();
	}

	PropertiesEntry* item = static_cast<PropertiesEntry*>(index.internalPointer());
	if( const ItemMetadata* metadata = item->Get( index.column() ) )
	{
		if( role == Qt::EditRole && (metadata->GetDataType() == MetadataType::Title) )
		{
			return QVariant();
		}
	}

	return item->GetData( index.column() );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool PropertiesModel::setData( const QModelIndex& index, const QVariant& value, int role/*=Qt::EditRole */ )
{
	if( !index.isValid() || role != Qt::EditRole )
	{
		return false;
	}

	PropertiesEntry* item = static_cast<PropertiesEntry*>(index.internalPointer());
	item->SetData( index.column(), value );
	dataChanged( index, index );

	return true;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Qt::ItemFlags PropertiesModel::flags( const QModelIndex& index ) const
{
	if ( !index.isValid() )
	{
		return Qt::NoItemFlags;
	}

	if ( index.column() == 1 )
	{
		return (Qt::ItemFlags)(QAbstractItemModel::flags( index ) | Qt::EditRole);
	}

	return QAbstractItemModel::flags( index );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
QVariant PropertiesModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
	{
		return rootItem->GetData( section );
	}

	return QVariant();
}
