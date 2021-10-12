/*------------------------------------------------------------------------------
	()      File:   property_panel.cpp
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
				Helper utility to manage a simple property panel.
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
#include "ui/properties_menu/property_panel.h"
#include "ui/properties_menu/properties_delegate.h"
#include "QAbstractItemModel"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PropertyPanel::Initialise( QTreeView& treeView, QObject* parent )
{
	m_view = &treeView;
	m_propertyModel.setParent( parent );

	treeView.setModel( &m_propertyModel );
	treeView.setItemDelegate( new PropertiesDelegate( parent ) );

	UpdateEditorMode();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PropertyPanel::UpdateEditorMode()
{
	if ( m_view == nullptr )
	{
		return;
	}

	for ( int row = 0; row < m_propertyModel.rowCount(); ++row )
	{
		for ( int column = 1; column < m_propertyModel.columnCount(); ++column )
		{
			QModelIndex index = m_propertyModel.index( row, column, QModelIndex() );
			m_view->openPersistentEditor( index );
		}
	}
}