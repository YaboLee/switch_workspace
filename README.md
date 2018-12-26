# Switch Workstaion in Ubuntu

# Dependencis

You can skip this long info.

1. xdotool
    This is an efficient tool for you to run specific commands.
    ```
    This tool lets you simulate keyboard input and mouse
    activity, move and resize windows, etc. It does this
    using X11’s XTEST extension and other Xlib functions.

    Additionally, you can search for windows and move, 
    resize, hide, and modify window properties like the
    title. If your window manager supports it, you can use
    xdotool to switch desktops, move windows between
    desktops, and change the number of desktops.
    ```
2. libinput
    This is an interface for you to get event from hardware.

    ```
    libinput is a library that provides a full input stack
    for display servers and other applications that need to
    handle input devices provided by the kernel.

    libinput provides device detection, event handling and
    abstraction so minimize the amount of custom input code
    the user of libinput need to provide the common set of
    functionality that users expect. Input event processing
    includes scaling touch coordinates, generating relative
    pointer events from touchpads, pointer acceleration, etc.

    libinput is not used directly by applications. Think of
    it more as a device driver than an application library.
    See What is libinput? for more details.
    ```
# Test Environment

    Ubuntu 18.04

# Usage

```bash
    # Make sure you have installed the dependencies
    sudo apt-get install libinput-dev xdotool

    # Compile the file
    # Ignore the warnings
    make

    # Run
    # This requires root privilage
    # do not ask me how to kill background process
    sudo ./switch_workspace &

    # Use four fingers to swipe left/right/up/down
```

# Notes:

Sometimes you may need to change some config in ``main.c`` due to the different versions of Linux distributions.

On different laptop or PC, the mask code for Touchpad could be different. You may want to change the parameter in ``scan_device()``
Use ``cat /proc/bus/input/devices`` to find the name entry with **Touchpad** or similar. Use the **EV** number of HEX as the parameter.

These are what you may want to change for different switch method according to your own shorcuts.

```c
    #define LEFT_SWIPE "xdotool key alt+ctrl+Up"
    #define RIGHT_SWIPE "xdotool key alt+ctrl+Down"
    #define DISPLAY_ALL "xdotool key super"
```

# TO DO

1. Change windows with three fingers.
    This can be difficult since Ubuntu itself does not provide way to change to specific window. i.e. On Mac, you could change window by three fingers.

2. Auto start in the background.
