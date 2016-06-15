#ifndef GXDEBUG_H
#define GXDEBUG_H

#include <stdio.h>

#ifdef WIN32
#include <Windows.h>
#endif

//#define LOG_DEBUB_VERBOSE
//#define LOG_DEBUG_ENGINE


#if defined(WIN32) //&& defined(_DEBUG)
extern "C" {
	void DEBUG_PRINT(const char* format, ...);
}
#elif defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
extern "C" {
	void DEBUG_PRINT(const char* format, ...);
}
#elif defined(ANDROID)
extern "C" {
	#include <android/log.h>
	#define  DEBUG_PRINT(...)  __android_log_print(ANDROID_LOG_INFO,"android_native",__VA_ARGS__)
}
#endif

extern "C" {
	void DEBUG_ASSERT(bool condition, const char* string);
#if !defined(WIN32)
	void DEBUG_DRAW_LINE(int x1, int y1, int x2, int y2);
    void DEBUG_DRAW_RECT(int x, int y, int cx, int cy);
#endif
    void DEBUG_DRAW_RECTf(float x, float y, float cx, float cy);
#if !defined(WIN32)
    void DEBUG_DRAW_FILL_RECT(int x, int y, int cx, int cy);
#endif
    void DEBUG_DRAW_FILL_RECTf(float x, float y, float cx, float cy);
    void DEBUG_DRAW_FILL_RECT_ALPA(float x, float y, float cx, float cy);
}
#endif