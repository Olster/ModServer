Plugin-based server.
========================================

All application layer protocols are provided in DLL plugins.

# Why plugins?
Plugins implemented as DLLs can be swapped in run time, without shutting down the whole server. That allows updating plugins while the server
is running, turning some of the plugins on and off.
Such architecture helps to decouple the application layer from transport layer. Plugins don't depend where data comes from, they are only
responsible for parsing the request and provide the response.

# Available plugins
1. HTTP (work in progress).

#TODO:
- [ ] Add HTTP plugin.
- [ ] Plugin verification.
- [ ] Read settings from file.
- [ ] Makefile for UNIX.
- [ ] Add unit tests!
- [ ] Check for memory leaks. Check pointers in general.
- [ ] Use smart pointers to minimize leak probability.
- [ ] Make a branch for older compilers that don't support C++11.
- [ ] Add Debug and Release configs for gyp.
