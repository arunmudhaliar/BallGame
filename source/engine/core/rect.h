#ifndef RECT_H
#define RECT_H

#include "vector2.h"
#include "pxMath.h"


template<class T>
class gxRect;
typedef gxRect<float> bxRectf;

template<class T>
class gxRect;
typedef gxRect<int> bxRecti;

template<class T>
class gxRect
{
public:
    gxRect<T>()
    {
    }
    
    gxRect<T>(const gxRect<T>& rect)
    {
        m_pos.set(rect.m_pos.x, rect.m_pos.y);
        m_size.set(rect.m_size.x, rect.m_size.y);
    }
    
    gxRect<T>(const vector2<T>& pos, const vector2<T>& size)
    {
        m_pos=pos;
        m_size=size;
    }
    
    gxRect<T>(T x, T y, T cx, T cy)
    {
        m_pos.set(x, y);
        m_size.set(cx, cy);
    }
    
    void operator=(const gxRect<T>& rect)
    {
        m_pos.set(rect.m_pos.x, rect.m_pos.y);
        m_size.set(rect.m_size.x, rect.m_size.y);
	}
    
    vector2<T> getCenter()
	{
		return vector2<T>(m_pos+m_size*0.5f);
	}
    
    void set(T x, T y, T cx, T cy)
    {
        m_pos.set(x, y);
        m_size.set(cx, cy);
    }   
    
    bool isPointInsideRect(vector2<T> v)
    {
        return (m_pos.x<v.x && m_pos.y<v.y && m_pos.x+m_size.x>v.x && m_pos.y+m_size.y>v.y);
    }
    
    vector2<T> m_pos;
    vector2<T> m_size;
};

#endif