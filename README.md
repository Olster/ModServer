Plugin-based server.
========================================

All application layer protocols are provided in DLL plugins.

# Why plugins?
Plugins implemented as DLLs can be swapped in run time, without shutting down the whole server. That allows updating plugins while the server
is running, turning some of the plugins on and off.
Such architecture helps to decouple the application layer from transport layer. Plugins don't depend where data comes from, they are only
responsible for parsing the request and provide the response.

## Disadvantages.
C++ doesn't have any plugin/module API, so all DLLs must be built with the same compiler to work correctly. For this reason, plugin
verifier will be made that won't allow loading plugins from different compilers.

# Available plugins
1. HTTP (work in profress)

#TODO:
[] Add HTTP plugin.
[] Plugin verification.
[] Read settings from file.
[] Makefile for UNIX.
