#!/bin/bash

# --- Detect OS distribution ---
OS_DISTRIBUTION=''
if [ -f /etc/redhat-release ] ; then
    OS_DISTRIBUTION=`cat /etc/redhat-release |sed s/\ release.*//`
elif [ -f /etc/debian_version ] ; then
    OS_DISTRIBUTION=`cat /etc/lsb-release | grep '^DISTRIB_ID' | awk -F=  '{ print $2 }'`
fi

# --- Lowercase some of the names ---
OS_DISTRIBUTION=${OS_DISTRIBUTION,,}

echo "OS_DISTRIBUTION   : $OS_DISTRIBUTION"

# Currently the setup script only supports Ubuntu
if [[ "$OS_DISTRIBUTION" == "ubuntu" ]]; then
    echo "Installing needed libraries and tools for Ubuntu"
    sudo apt-get install -y graphviz cmake libboost-all-dev
fi

