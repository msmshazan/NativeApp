#pragma once

#if !defined(internal)
#define internal static
#endif

#define local_persist static
#define global static

#define API extern "C" 

#if defined( _MSC_VER )
#define thread_local_storage __declspec( thread )
#elif defined( __clang__ ) || defined( __GNUC__ )
#define thread_local_storage __thread
#elif __cplusplus >= 201103L
#define thread_local_storage thread_local
#elif __STDC_VERSION_ >= 201112L
#define thread_local_storage _Thread_local
#else
# error Unsupported platform.
#endif
