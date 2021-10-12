/*------------------------------------------------------------------------------
	()      File:   simple_event.h
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
				Simple list based event dispatcher.
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
template<class... ParamPack>
class SimpleEventDispatcher
{
private:
	struct Subscriber
	{
		void* obj;
		std::function<void( ParamPack... )> fn;

		bool operator==( Subscriber& other )
		{
			return other.obj == obj;
		}	
		
		bool operator==( void* other )
		{
			return other == obj;
		}
	};
	std::vector<Subscriber> m_subscribers;

public:
	template<class BaseType, void (BaseType::*fn)(ParamPack...)>
	void Subscribe( BaseType& obj )
	{
		void (BaseType::*objfn)(ParamPack...) = fn;

		auto Callback = [&obj, objfn]( ParamPack&&... params )
		{
			(obj.*objfn)( std::forward<ParamPack>( params )... );
		};
		Subscriber newSub = { &obj, Callback };

		auto iter = std::find( m_subscribers.begin(), m_subscribers.end(), &obj );
		if( iter == m_subscribers.end() )
		{
			m_subscribers.push_back( newSub );
		}
	}

	template<class BaseType>
	void UnSubscribe( BaseType& obj )
	{
		auto iter = std::find( m_subscribers.begin(), m_subscribers.end(), &obj );
		if( iter != m_subscribers.end() )
		{
			m_subscribers.erase( iter );
		}
	}

	void Invoke( ParamPack... params )
	{
		for( Subscriber& subscriber : m_subscribers )
		{
			subscriber.fn( std::forward<ParamPack>( params )... );
		}
	}
};