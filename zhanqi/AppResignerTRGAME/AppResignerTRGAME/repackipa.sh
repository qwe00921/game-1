#!/bin/sh

#  repackipa.sh
#  AppResignerTRGAME
#
#  Created by apache on 15/3/1.
#  Copyright (c) 2015年 shenliming. All rights reserved.
set -x
source ~/.bash_profile
export PATH=/opt/local/bin:$PATH

EXT_DIR="/tmp/IPA_EXT_DIR"
BUILD_DIR="/tmp/IPA_BUILD_DIR"
TMP_DIR="/tmp/IPA_TMP_ENC"

APP_SUB_DIR=${APP_DIR#$EXT_DIR}
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cp -af $EXT_DIR/ $BUILD_DIR

if [[ -n "$DELETE_DIRS" ]]; then
	for i in "$DELETE_DIRS"
	do
    #echo "$APP_DIR/GameResources/$i" >> /tmp/delete
    rm -rf "$BUILD_DIR/$APP_SUB_DIR/GameResources/$i"
done
fi

if [[ -n "$NORMAL_COPY_DIRS" ]]; then
	for i in "$NORMAL_COPY_DIRS"
	do
		if [ -d $i ];then
			cp -af "$i/" "$BUILD_DIR/$APP_SUB_DIR/GameResources"
		fi

		if [ -f $i ];then
			cp -af "$i" "$BUILD_DIR/$APP_SUB_DIR/GameResources"
		fi

	done
fi


rm -rf $TMP_DIR
mkdir $TMP_DIR

if [[ -n $ENC_COPY_DIRS ]]; then
	for i in $ENC_COPY_DIRS
	do
		if [ -d $i ];then
			cp -af "$i/" $TMP_DIR
		fi

		if [ -f $i ];then
			cp -af "$i" $TMP_DIR
		fi
	done
fi

if [[ -n "$ENC_COPY_DIRS" ]]; then
	trgameEncoder --completeext eex_new json dat id --partext bmp e5 png jpg --dir $TMP_DIR
	cp -avf "$TMP_DIR/ $BUILD_DIR/$APP_SUB_DIR/GameResources"
fi

cp "$INFO_PLIST_DIR" "$BUILD_DIR/$APP_SUB_DIR/Info.plist"

plutil -convert binary1 "$BUILD_DIR/$APP_SUB_DIR/Info.plist"

jq '.EncryptResource=true' < "$BUILD_DIR/$APP_SUB_DIR/GameResources/function.txt" > /tmp/IPA_INFO_PLIST_JQ1
jq '.EnableKTPlay=false' < /tmp/IPA_INFO_PLIST_JQ1 > /tmp/IPA_INFO_PLIST_JQ2

echo "*$CHANNEL_IDS*"
sed -e "s/BUNDLENAME/$BUNDLE_ID/" "$RESOURCE_PATH/archived-expanded-entitlements.xcent" > "$BUILD_DIR/$APP_SUB_DIR/archived-expanded-entitlements.xcent"

#change the .app dir name and the execute file name
if [ -n "$NEW_EXENAME" ]; then
	OLD_EXENAME=$( basename "$APP_SUB_DIR" .app )
	mv "$BUILD_DIR/$APP_SUB_DIR/$OLD_EXENAME" "$BUILD_DIR/$APP_SUB_DIR/$NEW_EXENAME"
	NEW_DIRNAME=${APP_SUB_DIR/$OLD_EXENAME/$NEW_EXENAME}
	mv "$BUILD_DIR/$APP_SUB_DIR" "$BUILD_DIR/$NEW_DIRNAME"
	APP_SUB_DIR=$NEW_DIRNAME
fi


time=`date  +%Y_%m%d_%H%M`
if [[ -n "$CHANNEL_IDS" ]]; then
	for i in "$CHANNEL_IDS"
	do
		if [ "$i" == "IOSZhengShiBan" ]; then
			sed -e "s/BUNDLENAME/$BUNDLE_ID/" "$RESOURCE_PATH/entitlements" > /tmp/IPA_ENTITLEMENTS
			if [ -n $PROFILE_NAME ]; then
				cp "$RESOURCE_PATH/$PROFILE_NAME""_AppStore.mobileprovision" "$BUILD_DIR/$APP_SUB_DIR/embedded.mobileprovision"
			else
				cp "$RESOURCE_PATH/XC.mobileprovision" "$BUILD_DIR/$APP_SUB_DIR/embedded.mobileprovision"
			fi
		else
			sed -e "s/BUNDLENAME/$BUNDLE_ID/" "$RESOURCE_PATH/entitlements_adhoc" > /tmp/IPA_ENTITLEMENTS
			if [ -n $PROFILE_NAME ]; then
				cp "$RESOURCE_PATH/$PROFILE_NAME""_AdHoc.mobileprovision" "$BUILD_DIR/$APP_SUB_DIR/embedded.mobileprovision"
			else
				cp "$RESOURCE_PATH/XC_adhoc.mobileprovision" "$BUILD_DIR/$APP_SUB_DIR/embedded.mobileprovision"
			fi
		fi

		jq ".TalkingDataChannelId=\"$i\"" < /tmp/IPA_INFO_PLIST_JQ2 > "$BUILD_DIR/$APP_SUB_DIR/GameResources/function.txt"
		codesign --force -vvvv --sign 3BF94FCD589266D9D91239411232C05A379A8D40 --entitlements /tmp/IPA_ENTITLEMENTS "$BUILD_DIR/$APP_SUB_DIR"
		cd $BUILD_DIR
		rm -f ~/Downloads/repack_"$BUNDLE_ID"_"$i"_V"$VERSION_NUM"_B"$BUILD_NUM"_"$time".ipa
		zip -r ~/Downloads/repack_"$BUNDLE_ID"_"$i"_V"$VERSION_NUM"_B"$BUILD_NUM"_"$time".ipa *
	done
else
	cp /tmp/IPA_INFO_PLIST_JQ2 "$BUILD_DIR/$APP_SUB_DIR/GameResources/function.txt"
	codesign --force -vvvv --sign 3BF94FCD589266D9D91239411232C05A379A8D40 --entitlements /tmp/IPA_ENTITLEMENTS "$BUILD_DIR/$APP_SUB_DIR"

	cd $BUILD_DIR
	rm -f ~/Downloads/repack_"$BUNDLE_ID"_V"$VERSION_NUM"_B"$BUILD_NUM"_"$time".ipa
	zip -r ~/Downloads/repack_"$BUNDLE_ID"_V"$VERSION_NUM"_B"$BUILD_NUM"_"$time".ipa *
fi
