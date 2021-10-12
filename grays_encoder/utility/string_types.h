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
#pragma once
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <string>
#include <QString>
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// HashedStringT
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template<class STR_T>
struct HashedStringT
{
	HashedStringT() = default;
	HashedStringT( const char cChar )
	{
		Set( cChar );
	}

	HashedStringT( const char* cStr )
	{
		Set( cStr );
	}

	explicit HashedStringT( const STR_T& str )
	{
		Set( str );
	}

	explicit HashedStringT( const HashedStringT& str )
	{
		Set( str );
	}

	size_t Hash() const
	{
		return m_hash;
	}

	const STR_T& String() const
	{
		return m_string;
	}

	const char* CStr() const
	{
		return "";
		//return m_string.c_str();
	}
	//------------------------------------------------------------------------------
	// Qt String Type
	//------------------------------------------------------------------------------
	//explicit specialisation for Q strings for that one function where they deviate from the STL standard.
// 	template<>
// 	const char* HashedStringT<QString>::CStr() const
// 	{
// 		return m_string.toLatin1().data();
// 	}

	HashedStringT& operator=( const STR_T& str )
	{
		Set( str );
		return *this;
	}

	HashedStringT& operator=( const HashedStringT& str )
	{
		Set( str );
		return *this;
	}

	bool operator==( const HashedStringT& str ) const
	{
		return str.m_hash == m_hash;
	}

	bool operator==( const STR_T& str ) const
	{
		size_t otherHash = std::hash<STR_T>{}(str);
		return m_hash == otherHash;
	}

	HashedStringT& operator+=( const char cChar )
	{
		m_string += cChar;
		m_hash = std::hash<STR_T>{}(m_string);
		return *this;
	}

	HashedStringT& operator+=( const char* cStr )
	{
		m_string.append( cStr );
		m_hash = std::hash<STR_T>{}(m_string);
		return *this;
	}

	HashedStringT& operator+=( const STR_T& stdStr )
	{
		m_string.append( stdStr );
		m_hash = std::hash<STR_T>{}(m_string);
		return *this;
	}

	HashedStringT& operator+=( const HashedStringT& hashStr )
	{
		m_string.append( hashStr.m_string );
		m_hash = std::hash<STR_T>{}(m_string);
		return *this;
	}

private:
	void Set( const char chr )
	{
		m_string.clear();
		m_string += chr;
		m_hash = std::hash<STR_T>{}(m_string);
	}

	void Set( const char* str )
	{
		m_string = str;
		m_hash = std::hash<STR_T>{}(m_string);
	}

	void Set( const STR_T& str )
	{
		m_string = str;
		m_hash = std::hash<STR_T>{}(m_string);
	}

	void Set( const HashedStringT& str )
	{
		m_string = str.m_string;
		m_hash = str.m_hash;
	}

private:
	STR_T m_string;
	size_t m_hash;
};


//------------------------------------------------------------------------------
// STD String Type
//------------------------------------------------------------------------------
using HashedString = HashedStringT<std::string>;
using HashedQString = HashedStringT<QString>;