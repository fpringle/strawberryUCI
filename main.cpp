/*
Copyright (c) 2022, Frederick Pringle
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
*/
#include "interface.h"

int main() {
    chessUCI::chessInterface interface;
    interface.mainLoop();

    return 0;
}
