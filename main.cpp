#include "interface.h"

int main() {
/*
    chessUCI::MessageTypes::InfoMessage infoMessage;

    infoMessage.depth = 2;
    infoMessage.time = 1242;
    infoMessage.score = "cp 214";
    infoMessage.nodes = 2124;
    infoMessage.nps = 34928;

    infoMessage.pv.push_back("e2e4");
    infoMessage.pv.push_back("e7e5");
    infoMessage.pv.push_back("g1f3");

    chessUCI::MessageTypes::OptionMessage checkMessage;
    chessUCI::MessageTypes::OptionMessage spinMessage;
    chessUCI::MessageTypes::OptionMessage comboMessage;
    chessUCI::MessageTypes::OptionMessage buttonMessage;
    chessUCI::MessageTypes::OptionMessage stringMessage;

    checkMessage.name = "Nullmove";
    checkMessage.type = chessUCI::MessageTypes::check_type;
    checkMessage.option_default = "true";

    spinMessage.name = "Selectivity";
    spinMessage.type = chessUCI::MessageTypes::spin_type;
    spinMessage.option_default = "2";
    spinMessage.option_min = "0";
    spinMessage.option_max = "4";

    comboMessage.name = "Style";
    comboMessage.type = chessUCI::MessageTypes::combo_type;
    comboMessage.option_default = "Normal";
    comboMessage.vars.push_back("Solid");
    comboMessage.vars.push_back("Normal");
    comboMessage.vars.push_back("Risky");

    stringMessage.name = "NalimovPath";
    stringMessage.type = chessUCI::MessageTypes::string_type;
    stringMessage.option_default = "c:\\";

    buttonMessage.name = "Clear Hash";
    buttonMessage.type = chessUCI::MessageTypes::button_type;


    chessUCI::chessInterface interface;
    interface.sendInfoMessage(infoMessage);
    interface.sendOptionMessage(checkMessage);
    interface.sendOptionMessage(spinMessage);
    interface.sendOptionMessage(comboMessage);
    interface.sendOptionMessage(stringMessage);
    interface.sendOptionMessage(buttonMessage);

    std::string input = interface.readInput();
    std::vector<std::string> tokens = chessUCI::tokenise_string(input);
    for (std::string s : tokens) {
        std::cout << s << std::endl;
    }
*/


    chessUCI::chessInterface interface;

    std::vector<std::string> goodMessages = {
        "uci",
        "   debug     on  ",
        "\t  debug \t  \t\ton\t  ",
        "debug off",
        "isready",
        "setoption name Option One",
        "setoption name Option One value 1",
        "register later",
        "register name Stefan",
        "register code 45679",
        "register name Stefan code 5679",
        "ucinewgame",
        "position startpos",
        "position startpos moves e2e4 e7e6 g1f3",
        "go infinite searchmoves e2e4 d2d4",
        "go ponder wtime 5 mate 6",
        "go infinite binc 10 nodes 5 movetime 60",
        "stop",
        "ponderhit",
        "quit"
    };

    std::cout << "Parsing good messages\n";
    for (std::string s : goodMessages) {
        std::cout << " Parsing message " << s << std::endl;
        interface.parseMessage(s);
    }

    return 0;
}
