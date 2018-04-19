# Installation with CentOS 7 Minimal VM

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

Open the terminal and run the following script:

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
