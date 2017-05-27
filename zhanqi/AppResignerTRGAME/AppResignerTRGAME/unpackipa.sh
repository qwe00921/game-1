#!/bin/sh

#  unpackipa.sh
#  AppResignerTRGAME
#
#  Created by apache on 15/2/28.
#  Copyright (c) 2015年 shenliming. All rights reserved.
set -x
input_ipa=$1
output_dir='/tmp/IPA_EXT_DIR'
rm -rf $output_dir
mkdir $output_dir

if [ $? != 0 ]; then
	exit 2
fi

cd $output_dir
unzip -q $input_ipa

if [ $? != 0 ]; then
	exit 2
fi

rm -f "/tmp/IPA_EXT_appdir"
app_dir=`find $output_dir -name "*.app" -type d`
if [[ -n $app_dir ]]; then
    /bin/echo -n $app_dir > /tmp/IPA_EXT_appdir
    plutil -convert xml1 $app_dir/Info.plist
fi
