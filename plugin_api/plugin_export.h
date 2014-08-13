#ifndef SERVER_PLUGIN_PLUGIN_EXPORT_H_
#define SERVER_PLUGIN_PLUGIN_EXPORT_H_

#if defined(WIN32)
  #if defined(PLUGIN_IMPLEMENTATION)
    #define PLUGIN_EXPORT __declspec(dllexport)
  #else  // !defined(PLUGIN_IMPLEMENTATION)
    #define PLUGIN_EXPORT __declspec(dllimport)
  #endif
#else  // !defined(WIN32)
  #if defined(PLUGIN_IMPLEMENTATION)
    #define PLUGIN_EXPORT __attribute__((visibility("default")))
  #else  // !defined(PROTOCOL_IMPLEMENTATION)
    #define PLUGIN_EXPORT __attribute__((visibility("default")))
  #endif
#endif

#endif  // SERVER_PLUGIN_PLUGIN_EXPORT_H_
