[![Build Status](https://travis-ci.org/Maschell/fs_wrapper.svg?branch=wut)](https://travis-ci.org/Maschell/fs_wrapper)

TODO

Use "FileReplacerUtils::destroyInstance()" to destroy the instance.

## Usage
	See fs_default_os_wrapper.h to wrap the async FS functions
	See fs_sync_wrapper to use sync fs functions


Link the application with:
```
-lfswrapperwut -lutilswut -ldynamiclibs 
```

You also need to add the include path to your Makefile. Example:

```
export INCLUDE	:= [...] -I$(WUT_ROOT)/include -I$(WUT_ROOT)/include/libutilswut
```

## Dependencies
To be able to use libfswrapperwut, you need to install the following dependencies:

- [wut](https://github.com/decaf-emu/wut)
- [libutilswut](https://github.com/Maschell/libutils/tree/wut) (WUT build) for common functions.