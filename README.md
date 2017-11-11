TODO

Use "FileReplacerUtils::destroyInstance()" to destroy the instance.

## Usage
	See fs_default_os_wrapper.h to wrap the async FS functions
	See fs_sync_wrapper to use sync fs functions


Link the application with:
```
-lfswrapper -lutils -ldynamiclibs 
```

You also need to add the include path to your Makefile. Example:

```
export INCLUDE	:= [...] -I$(PORTLIBS)/include
```

## Dependencies
To be able to use libfswrapper, you need to install the following dependencies:

- Application needs to be loaded from the [homebrew_launcher](https://github.com/dimok789/homebrew_launcher)
- [libutils](https://github.com/Maschell/libutils) for common functions.
- [dynamic_libs](https://github.com/Maschell/dynamic_libs/tree/lib) for access to the functions.