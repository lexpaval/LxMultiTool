![logo](https://raw.githubusercontent.com/lexmazter/LxMultiTool/master/Icons/android.png "Lx Multi Tool Logo") 
# Lx Multi Tool

Lx Multi Tool is a tool inittially created for Huawei Nexus 6P but designed to be modular, easy to modify and port to other device and to be trully cross-platform.

This tool wants to stand out from the rest of the tools out there by providing a intuitive UI that uses most of the ADB and Fastboot commands.
As a bonus, this tool also checks for updates.

The UI has been carefully tested and designed in mind with 3 operating systems (Windows, Linux and MAC).

The features presented in this README are according to the version mentioned in the Version chapter.

##### Fastboot
 - Unlock/Lock Bootloader
 - Flash Recovery Menu (you can choose and add recoveries)
 - Boot Recovery (From Recovery menu)
 - Flash any partition (you can choose what partition and add images)
 - Flash full Stock images (you can choose and add images)
 - Erase (format) partitions (you can choose which of the partitions)
 - Reboot (you can choose again...)
  
##### Adb
 - Sideload .zip files (you can choose and add .zip files)
 - Backup (you can choose the name and place for your backup)
 - Restore (you can choose your backup)
 - Version (it just shows the version of your adb binaries)
 - Logs (DMESG and Logcat)
 - Screenshots (you can directly take screenshots to your PC)
 - Reboot (lots of choices)
 - Install .apk files

##### Tool specific
 - Check for updates
 - Go to xda-developers thread
 - Tool tips for all buttons
 - Context for refresh and delete on all menus
 - Open tool-specific folder from all menus
 - ADB & Fastboot error messages upon failing
 - Progress animations
 - Open a Terminal/CMD session for manual adb/fastboot commands input
 - Device Packages - Consists of an .xml config file
 - Support for Recovery reboot commands.
  
### Version
1.3.2

### Supported devices
 - Huawei Nexus 6P
 - LG Nexus 5X
 - Motorola Nexus 6
 - LG Nexus 5

### Tech

Lx Multi Tool uses a number of technologies to work properly:

* [Qt] - An awesome cross-platform IDE.
* [Dillinger] - Awesome web platform that made this document so handsome!
* [Vmware] - For letting me test this tool on all platforms (Linux and MAC).
* [Icons8] - Not actually a technology, but a nice platform for good-looking FREE icons. (Big thanks!)
* [UnixUtils] - For the sed utility for Windows.

### Installation

In order to use Lx Multi Tool, you need to extract the archive for the respective operating system and run the LxMultiTool executable.

### Development

Want to contribute? Great!

All you need to do is download the [Qt IDE](http://www.qt.io/download-open-source/) for your operating system, clone this repository, open the Qt Creator, click Open Project and then select LxMultiTool .pro.

After that, you should be able to build, debug, edit and so on.

### Porting

With the introduction of DevicePackages, now you can port any version to your own device.
So far, the following can be changed via DevicePackage:
 - Bootloader unlock/lock command.
 - XDA Thread link.
 - Updates download folder link.

### Todos

 - Extend the DevicePackage functionality to support more devices
 - Properly handle fastboot and adb output text for more user friendliness
 - Add more Code Comments
 - Add messages when you want to save files with a name that already exists
 - Add online database to download stuff from (kernels, stock, recoveries)
 - Extend the file downloader to download directly from the tool

License
----

This tool is distributed for free under the [GPLv3] licence. Make sure you read it and understand it before using the code for your own purposes.


**Free Software, Hell Yeah!**

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does it's job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [Qt]: <http://qt.io>
   [Dillinger]: <http://dillinger.io>
   [Vmware]: <https://www.vmware.com/>
   [GPLv3]: <http://www.gnu.org/licenses/gpl-3.0.en.html>
   [Icons8]: <https://www.icons8.com>
   [UnixUtils]: <http://unxutils.sourceforge.net/>



