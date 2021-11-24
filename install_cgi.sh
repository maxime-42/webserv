# !/bin/bash 
SYSTEM=`uname -s`
if [[ ! `which php-cgi` ]] || [[ `which php-cgi` = 'php-cgi not found' ]]
then
    echo "which php-cgi not found, installing..."
    if [ $SYSTEM = 'Linux' ]
    then
        sudo apt-get install php-cgi
    else
        brew install php
    fi
fi