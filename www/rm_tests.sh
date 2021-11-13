#!/bin/bash
chmod 777 dir_no_access_rights

rm -rf dir_no_access_rights
chmod 777 files/file_no_access_rights.html
rm files/file_no_access_rights.html

