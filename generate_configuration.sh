#!/bin/bash

user_config() {
    echo "Generating user configuration . . .";
    # Get the configuration item from build.config
    CONFIG_PATH=`grep USER_CONFIG_PATH build.config | perl -pe 's/^.+\(.+ "//' | sed 's/^"//' | sed 's/")$//'`
    CONFIG_PATH=`echo $CONFIG_PATH | sed "s|^~|$HOME|"`
    echo "Configuration file is "$CONFIG_PATH
    
    echo "overload-path="`pwd` > `echo $CONFIG_PATH`
    echo "libc-path="`cat /proc/self/maps | grep libc-2 | head -n 1 | awk '{print $6}'` >> $CONFIG_PATH
}

system_config() {
    echo "System config generation NYI.";
}

usage() {
    echo $0": a script to generate an Aesalon configuration file." ;
    echo "    Copyright (C) 2010 strange <kawk256@gmail.com>" ;
    echo "Usage:" $0": --user | --system <install-path>" ;
    echo "    --user: generate user configuration" ;
    echo "    --system: generate system-wide configuration" ;
    echo ;
}

if [[ $1 == "--user" ]]; then
    user_config
elif [[ $1 == "--system" ]]; then
    if [[ ! -d $2 ]]; then
        echo "Given install-path is not a directory." ;
    else
        system_config $2 ; 
    fi
else
    usage $0
fi
