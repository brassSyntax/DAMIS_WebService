# Installation with CentOS 7 Minimal VM

CentOS 7 Minimal download: http://isoredirect.centos.org/centos/7/isos/x86_64/CentOS-7-x86_64-Minimal-1708.iso .
Use either VMWare Player or VirtualBox.

Before clicking "Begin Installation" button make sure to enable network connection
and keep automatic partitioning.

During installation set root password, create a user and mark it as admin.
For simplicity use "damis" for username, user and root passwords.

Once it's done reboot, mark VM as finished installing and login with your user.
Then install GNOME:

    sudo yum -y groupinstall "GNOME Desktop"

Once it's done run:

    startx

By now you should be able to use the GUI and login with your user.

## Prerequisites

Open the terminal and run the following commands:

```
# install git and gcc
sudo yum -y install git gcc gcc-c++

# gcc related dependencies
sudo yum -y install libgcc.i686 glibc-devel.i686 libgcc glibc-devel

# install cURL lib
sudo yum -y install libcurl.x86_64 libcurl-devel.x86_64

# install extra repo and codeblocks from it
sudo yum -y install epel-release
sudo yum -y install codeblocks
```

If you encountered no errors, proceed to the next step.

## CodeBlocks project setup

In CodeBlocks, go to ```Settings -> Compiler...``` and turn on C++11 flag

![Compiler Settings](/docs/compiler-settings.png)

Then, in the same window go to linker settings and add ```libcurl.so``` file from
```/lib64/``` by CentOS default.

![Linker Settings](/docs/linker-settings.png)

Now you can build the project.
