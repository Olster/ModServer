#ifndef PROTOCOL_EXPORT_H_
#define PROTOCOL_EXPORT_H_

#if defined(WIN32)
#if defined(PROTOCOL_IMPLEMENTATION)
#define PROTOCOL_EXPORT __declspec(dllexport)
#else // defined(PROTOCOL_IMPLEMENTATION)
#define PROTOCOL_EXPORT __declspec(dllimport)
#endif
#else // defined(WIN32)
#if defined(PROTOCOL_IMPLEMENTATION)
#define PROTOCOL_EXPORT __attribute__((visibility("default")))
#else // defined(PROTOCOL_IMPLEMENTATION)
#define PROTOCOL_EXPORT __attribute__((visibility("default")))
#endif
#endif

#endif // PROTOCOL_EXPORT_H_
