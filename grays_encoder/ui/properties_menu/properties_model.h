/*------------------------------------------------------------------------------
	()      File:   properties_model.h
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
#pragma once
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "ui/common/metatypes.h"
#include "utility/string_types.h"
#include "utility/types_helper.h"
#include "QAbstractItemModel"
#include "QObject"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forwards
//------------------------------------------------------------------------------
class PropertiesEntry;


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// PropertiesModel
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class PropertiesModel: public QAbstractItemModel, public MetadataInterfaceT<PropertiesEntry>
{
	Q_OBJECT;

public:
	explicit PropertiesModel( QObject* parent = nullptr );
	~PropertiesModel();

	template<NumericType ValueT>
	PropertiesEntry& addNumeric( const HashedQString& propertyName,
		const QString& propertyTitle,
		ValueT defaultValue,
		ValueT minVal = -std::numeric_limits<ValueT>::max(),
		ValueT maxVal = +std::numeric_limits<ValueT>::max()
	)
	{
		std::vector<QVariant> values = { propertyTitle, defaultValue };
		std::vector<ItemMetadata> metadata = { ItemMetadata( MetadataType::Title ), ItemMetadata( minVal, maxVal ) };
	
		return addEntry( values, metadata );
	}

	PropertiesEntry& addBoolean( const HashedQString& propertyName,
		const QString& propertyTitle,
		int defaultValue
	)
	{
		std::vector<QVariant> values = { propertyTitle, defaultValue };
		std::vector<ItemMetadata> metadata{ ItemMetadata( MetadataType::Title ), ItemMetadata( MetadataType::Boolean ) };

		return addEntry( values, metadata );
	}	
	
	PropertiesEntry& addString( const HashedQString& propertyName,
		const QString& propertyTitle,
		const QString& defaultValue
	)
	{
		std::vector<QVariant> values = { propertyTitle, defaultValue };
		std::vector<ItemMetadata> metadata{ ItemMetadata( MetadataType::Title ), ItemMetadata( MetadataType::String ) };

		return addEntry( values, metadata );
	}

	PropertiesEntry& addEnum( const HashedQString& propertyName,
		const QString& propertyTitle,
		int defaultValue,
		const std::vector<EnumDisplayPair>& enumRange
	)
	{
		std::vector<QVariant> values = { propertyTitle, defaultValue };
		std::vector<ItemMetadata> metadata{ ItemMetadata( MetadataType::Title ), ItemMetadata( enumRange ) };

		return addEntry( values, metadata );
	}


	QVariant data( const QModelIndex& index, int role ) const override;
	bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole ) override;
	Qt::ItemFlags flags( const QModelIndex& index ) const override;
	//bool setHeaderData( int section, Qt::Orientation orientation, const QVariant& value, int role = Qt::EditRole ) override;
	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;
	QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const override;
	QModelIndex parent( const QModelIndex& index ) const override;
	int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
	int columnCount( const QModelIndex& parent = QModelIndex() ) const override;

private:
	PropertiesEntry& addEntry( const std::vector<QVariant>& data, const std::vector<ItemMetadata>& metadata );
	void setupModelData( const QStringList& lines, PropertiesEntry* parent );

	PropertiesEntry* rootItem;
};