APP_STL := c++_shared
NDK_TOOLCHAIN_VERSION := clang
#APP_STL := gnustl_static

#APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -D_GLIBCXX_USE_C99 -std=c++11 -fsigned-char
APP_CPPFLAGS := -frtti -fexceptions -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
APP_LDFLAGS := -latomic
APP_PLATFORM := android-19

APP_SHORT_COMMANDS := true
APP_ABI := armeabi-v7a
#APP_CPPFLAGS += -DGAME_RES_MINI//´òÐ¡°ü
ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif
