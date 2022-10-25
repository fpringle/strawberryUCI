# Copyright (c) 2022, Frederick Pringle
# All rights reserved.
# 
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.

TEMPLATE = app
TARGET = build/uci
CORE_DIR = $${_PRO_FILE_PWD}../core
OBJECTS_DIR = obj
MOC_DIR = obj/unix

INCLUDEPATH += $${CORE_DIR}/include \
               include

QT -= core gui

HEADERS += include/interface.h \
           $${CORE_DIR}/include/action.h \
           $${CORE_DIR}/include/board.h \
           $${CORE_DIR}/include/eval.h \
           $${CORE_DIR}/include/hash.h \
           $${CORE_DIR}/include/init.h \
           $${CORE_DIR}/include/move.h \
           $${CORE_DIR}/include/play.h \
           $${CORE_DIR}/include/search.h \
           $${CORE_DIR}/include/tree.h \
           $${CORE_DIR}/include/twiddle.h \
           $${CORE_DIR}/include/typedefs.h

SOURCES += src/interface.cpp \
           src/main.cpp

win32 {
    LIBS += $${CORE_DIR}/obj/win32/action.o \
            $${CORE_DIR}/obj/win32/board.o \
            $${CORE_DIR}/obj/win32/check.o \
            $${CORE_DIR}/obj/win32/eval.o \
            $${CORE_DIR}/obj/win32/hash.o \
            $${CORE_DIR}/obj/win32/init.o \
            $${CORE_DIR}/obj/win32/move.o \
            $${CORE_DIR}/obj/win32/play.o \
            $${CORE_DIR}/obj/win32/search.o


    PRE_TARGETDEPS +=   \
                        $${CORE_DIR}/obj/win32/action.o \
                        $${CORE_DIR}/obj/win32/board.o \
                        $${CORE_DIR}/obj/win32/check.o \
                        $${CORE_DIR}/obj/win32/eval.o \
                        $${CORE_DIR}/obj/win32/hash.o \
                        $${CORE_DIR}/obj/win32/init.o \
                        $${CORE_DIR}/obj/win32/move.o \
                        $${CORE_DIR}/obj/win32/play.o \
                        $${CORE_DIR}/obj/win32/search.o \
                        make_core_win32_object_dir


    make_core_win32_object_dir.target = make_core_win32_object_dir
    make_core_win32_object_dir.commands = "@test -d $${CORE_DIR}/obj/win32 || mkdir -p $${CORE_DIR}/obj/win32"

    actiontarget.target = $${CORE_DIR}/obj/win32/action.o
    actiontarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/win32/action.o $${CORE_DIR}/src/action.cpp
    actiontarget.depends = make_core_win32_object_dir

    boardtarget.target = $${CORE_DIR}/obj/win32/board.o
    boardtarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/win32/board.o $${CORE_DIR}/src/board.cpp
    boardtarget.depends = make_core_win32_object_dir

    checktarget.target = $${CORE_DIR}/obj/win32/check.o
    checktarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/win32/check.o $${CORE_DIR}/src/check.cpp
    checktarget.depends = make_core_win32_object_dir

    evaltarget.target = $${CORE_DIR}/obj/win32/eval.o
    evaltarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/win32/eval.o $${CORE_DIR}/src/eval.cpp
    evaltarget.depends = make_core_win32_object_dir

    hashtarget.target = $${CORE_DIR}/obj/win32/hash.o
    hashtarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/win32/hash.o $${CORE_DIR}/src/hash.cpp
    hashtarget.depends = make_core_win32_object_dir

    inittarget.target = $${CORE_DIR}/obj/win32/init.o
    inittarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/win32/init.o $${CORE_DIR}/src/init.cpp
    inittarget.depends = make_core_win32_object_dir

    movetarget.target = $${CORE_DIR}/obj/win32/move.o
    movetarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/win32/move.o $${CORE_DIR}/src/move.cpp
    movetarget.depends = make_core_win32_object_dir

    playtarget.target = $${CORE_DIR}/obj/win32/play.o
    playtarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/win32/play.o $${CORE_DIR}/src/play.cpp
    playtarget.depends = make_core_win32_object_dir

    searchtarget.target = $${CORE_DIR}/obj/win32/search.o
    searchtarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/win32/search.o $${CORE_DIR}/src/search.cpp
    searchtarget.depends = make_core_win32_object_dir

    QMAKE_EXTRA_TARGETS += \
                            actiontarget \
                            boardtarget \
                            checktarget \
                            evaltarget \
                            hashtarget \
                            inittarget \
                            movetarget \
                            playtarget \
                            searchtarget \
                            make_core_win32_object_dir

} else:unix {
    LIBS += $${CORE_DIR}/obj/action.o \
            $${CORE_DIR}/obj/board.o \
            $${CORE_DIR}/obj/check.o \
            $${CORE_DIR}/obj/eval.o \
            $${CORE_DIR}/obj/hash.o \
            $${CORE_DIR}/obj/init.o \
            $${CORE_DIR}/obj/move.o \
            $${CORE_DIR}/obj/play.o \
            $${CORE_DIR}/obj/search.o


    PRE_TARGETDEPS +=   \
                        $${CORE_DIR}/obj/action.o \
                        $${CORE_DIR}/obj/board.o \
                        $${CORE_DIR}/obj/check.o \
                        $${CORE_DIR}/obj/eval.o \
                        $${CORE_DIR}/obj/hash.o \
                        $${CORE_DIR}/obj/init.o \
                        $${CORE_DIR}/obj/move.o \
                        $${CORE_DIR}/obj/play.o \
                        $${CORE_DIR}/obj/search.o

    make_core_object_dir.target = make_core_object_dir
    make_core_object_dir.commands = "@test -d $${CORE_DIR}/obj || mkdir -p $${CORE_DIR}/obj"

    actiontarget.target = $${CORE_DIR}/obj/action.o
    actiontarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/action.o $${CORE_DIR}/src/action.cpp
    actiontarget.depends = make_core_object_dir

    boardtarget.target = $${CORE_DIR}/obj/board.o
    boardtarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/board.o $${CORE_DIR}/src/board.cpp
    boardtarget.depends = make_core_object_dir

    checktarget.target = $${CORE_DIR}/obj/check.o
    checktarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/check.o $${CORE_DIR}/src/check.cpp
    checktarget.depends = make_core_object_dir

    evaltarget.target = $${CORE_DIR}/obj/eval.o
    evaltarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/eval.o $${CORE_DIR}/src/eval.cpp
    evaltarget.depends = make_core_object_dir

    hashtarget.target = $${CORE_DIR}/obj/hash.o
    hashtarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/hash.o $${CORE_DIR}/src/hash.cpp
    hashtarget.depends = make_core_object_dir

    inittarget.target = $${CORE_DIR}/obj/init.o
    inittarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/init.o $${CORE_DIR}/src/init.cpp
    inittarget.depends = make_core_object_dir

    movetarget.target = $${CORE_DIR}/obj/move.o
    movetarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/move.o $${CORE_DIR}/src/move.cpp
    movetarget.depends = make_core_object_dir

    playtarget.target = $${CORE_DIR}/obj/play.o
    playtarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/play.o $${CORE_DIR}/src/play.cpp
    playtarget.depends = make_core_object_dir

    searchtarget.target = $${CORE_DIR}/obj/search.o
    searchtarget.commands = $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $${CORE_DIR}/obj/search.o $${CORE_DIR}/src/search.cpp
    searchtarget.depends = make_core_object_dir

    QMAKE_EXTRA_TARGETS += \
                            actiontarget \
                            boardtarget \
                            checktarget \
                            evaltarget \
                            hashtarget \
                            inittarget \
                            movetarget \
                            playtarget \
                            searchtarget \
                            make_core_object_dir
}


