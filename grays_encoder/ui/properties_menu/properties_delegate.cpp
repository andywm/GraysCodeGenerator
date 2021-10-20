/*------------------------------------------------------------------------------
	()      File:   properties_delegate.cpp
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
#include "ui/properties_menu/properties_delegate.h"
#include "ui/common/metatypes.h"
#include "QSpinBox"
#include "QCheckBox"
#include "QComboBox"
#include "QObject"
#include "QLineEdit"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// SpinBoxDelegate
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
PropertiesDelegate::PropertiesDelegate( QObject* parent /*=nullptr */)
	: QStyledItemDelegate( parent ) 
{

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
QWidget* PropertiesDelegate::createEditor( QWidget* parent,
									   const QStyleOptionViewItem& option,
									   const QModelIndex& index ) const
{
	const ItemMetadata* metadata = MetadataInterface::GetMetadata( index );
	if ( !metadata )
	{
		return nullptr;
	}

	QWidget* widget = nullptr;

	switch ( metadata->GetDataType() )
	{
	case MetadataType::Enumeration:
	{
		QComboBox* editor = new  QComboBox( parent );
		editor->setFrame( false );

		widget = editor;
		break;
	}
	case MetadataType::Number:
	{
		QSpinBox* editor = new QSpinBox( parent );
		editor->setFrame( false );
		editor->setMinimum( metadata->GetLowerLimit<int>() );
		editor->setMaximum( metadata->GetUpperLimit<int>() );

		widget = editor;
		break;
	}
	case MetadataType::Decimal:
	{
		QDoubleSpinBox* editor = new QDoubleSpinBox( parent );
		editor->setFrame( false );
		editor->setMinimum( metadata->GetLowerLimit<float>() );
		editor->setMaximum( metadata->GetUpperLimit<float>() );

		widget = editor;
		break;
	}
	case MetadataType::Boolean:
	{
		QCheckBox* editor = new QCheckBox( parent );

		widget = editor;
		break;
	}
	case MetadataType::String:
	{
		QLineEdit* editor = new QLineEdit( parent );

		widget = editor;
		break;
	}
	}

	return widget;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PropertiesDelegate::setEditorData( QWidget* editor,
									const QModelIndex& index ) const
{
	const ItemMetadata* metadata = MetadataInterface::GetMetadata( index );
	if ( !metadata )
	{
		return;
	}

	switch ( metadata->GetDataType() )
	{
	case MetadataType::Enumeration:
	{
		int value = index.model()->data( index, Qt::EditRole ).toInt();
		QComboBox* spinBox = static_cast<QComboBox*>(editor);

		bool refreshOpts = spinBox->count() == 0;

		int index = 0;
		for ( const EnumDisplayPair& pair : metadata->GetEnumeration() )
		{
			if ( refreshOpts )
			{
				spinBox->addItem( pair.display_value );
			}

			if ( value == pair.numeric_value )
			{
				spinBox->setCurrentIndex( index );
			}
			index++;
		}

		break;
	}
	case MetadataType::Number:
	{
		int value = index.model()->data( index, Qt::EditRole ).toInt();

		QSpinBox* spinBox = static_cast<QSpinBox*>(editor);
		spinBox->setValue( value );

		break;
	}
	case MetadataType::Decimal:
	{
		float value = index.model()->data( index, Qt::EditRole ).toFloat();

		QDoubleSpinBox* spinBox = static_cast<QDoubleSpinBox*>(editor);
		spinBox->setValue( value );

		break;
	}
	case MetadataType::Boolean:
	{
		bool value = index.model()->data( index, Qt::EditRole ).toBool();

		QCheckBox* spinBox = static_cast<QCheckBox*>(editor);
		spinBox->setChecked( value );

		break;
	}
	case MetadataType::String:
	{
		const QString& value = index.model()->data( index, Qt::EditRole ).toString();

		QLineEdit* spinBox = static_cast<QLineEdit*>(editor);
		spinBox->setText( value );

		break;
	}
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PropertiesDelegate::setModelData( QWidget* editor, QAbstractItemModel* model,
								   const QModelIndex& index ) const
{
	const ItemMetadata* metadata = MetadataInterface::GetMetadata( index );
	if ( !metadata )
	{
		return;
	}

	switch ( metadata->GetDataType() )
	{
	case MetadataType::Enumeration:
	{
		QComboBox* spinBox = static_cast<QComboBox*>(editor);
		int value = spinBox->currentIndex();

		const std::vector<EnumDisplayPair>& enumRange = metadata->GetEnumeration();
		if ( value < enumRange.size() )
		{
			model->setData( index, enumRange[value].numeric_value, Qt::EditRole );
		}

		break;
	}
	case MetadataType::Number:
	{
		QSpinBox* spinBox = static_cast<QSpinBox*>(editor);
		spinBox->interpretText();
		int value = spinBox->value();

		model->setData( index, value, Qt::EditRole );

		break;
	}
	case MetadataType::Decimal:
	{
		QDoubleSpinBox* spinBox = static_cast<QDoubleSpinBox*>(editor);
		spinBox->interpretText();
		float value = spinBox->value();

		model->setData( index, value, Qt::EditRole );
		break;
	}
	case MetadataType::Boolean:
	{
		QCheckBox* spinBox = static_cast<QCheckBox*>(editor);

		bool value = spinBox->isChecked();
		model->setData( index, value, Qt::EditRole );

		break;
	}
	case MetadataType::String:
	{
		QLineEdit* spinBox = static_cast<QLineEdit*>(editor);
		model->setData( index, spinBox->text(), Qt::EditRole );

		break;
	}
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PropertiesDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option,
						 const QModelIndex& index ) const
{
	QStyledItemDelegate::paint( painter, option, index );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PropertiesDelegate::updateEditorGeometry( QWidget* editor,
										   const QStyleOptionViewItem& option,
										   const QModelIndex&/* index */ ) const
{
	editor->setGeometry( option.rect );
}

