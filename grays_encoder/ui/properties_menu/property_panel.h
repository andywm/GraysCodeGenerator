/*------------------------------------------------------------------------------
	()      File:   property_panel.h
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
#pragma once
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "ui/properties_menu/properties_model.h"
#include "ui/properties_menu/properties_entry.h"
#include "utility/string_types.h"
#include "QTreeView"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forwards
//------------------------------------------------------------------------------
class QObject;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// PropertyPanel
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class PropertyPanel
{
public:
	void Initialise( QTreeView& treeView, QObject* parent );

	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	template<NumericType Type>
	PropertiesEntry& AddProperty( const HashedQString& propertyName,
		const QString& propertyTitle,
		Type defaultValue
	)
	{
		PropertiesEntry& entry = m_propertyModel.addNumeric( propertyName, propertyTitle, defaultValue );
		m_view->reset();
		UpdateEditorMode();

		return entry;
	}

	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	template<>
	PropertiesEntry& AddProperty<const char*>( const HashedQString& propertyName,
		const QString& propertyTitle,
		const char* defaultValue
	)
	{
		PropertiesEntry& entry = m_propertyModel.addString( propertyName, propertyTitle, defaultValue );
		m_view->reset();
		UpdateEditorMode();

		return entry;
	}

	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	PropertiesEntry& AddProperty( const HashedQString& propertyName,
		const QString& propertyTitle,
		bool defaultValue
	)
	{
		PropertiesEntry& entry = m_propertyModel.addBoolean( propertyName, propertyTitle, defaultValue );
		m_view->reset();
		UpdateEditorMode();

		return entry;
	}

	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	template<NumericType Type>
	PropertiesEntry& AddProperty( const HashedQString& propertyName,
		const QString& propertyTitle,
		Type defaultValue,
		Type minVal,
		Type maxVal
	)
	{
		PropertiesEntry& entry = m_propertyModel.addNumeric( propertyName, propertyTitle, defaultValue, minVal, maxVal );
		m_view->reset();
		UpdateEditorMode();

		return entry;
	}

	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	PropertiesEntry& AddProperty( const HashedQString& propertyName,
		const QString& propertyTitle,
		int defaultValue,
		const std::vector<EnumDisplayPair>& enumRange
	)
	{
		PropertiesEntry& entry = m_propertyModel.addEnum( propertyName, propertyTitle, defaultValue, enumRange );
		m_view->reset();
		UpdateEditorMode();

		return entry;
	}

private:
	void UpdateEditorMode();

private:
	PropertiesModel m_propertyModel;
	QTreeView* m_view;
};