#include "interface.h"

#include <regex>
#include <sstream>
#include <mutex>
#include <thread>
#include <deque>
#include <condition_variable>
#include <string>
#include <chrono>

#define DUMMY_HANDLING

namespace chessUCI {

std::string upper_string(std::string s) {
    std::stringstream ss;
    for (char c : s) {
        ss << char(toupper(c));
    }
    return ss.str();
}

std::string lower_string(std::string s) {
    std::stringstream ss;
    for (char c : s) {
        ss << char(tolower(c));
    }
    return ss.str();
}

bool is_integer(std::string s) {
    for (char c : s) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

std::vector<std::string> tokenise_string(std::string s) {

    const std::regex re("\\s+");
    std::sregex_token_iterator it{s.begin(), s.end(), re, -1};
    std::vector<std::string> tokens{it, {}};
    tokens.erase(
        std::remove_if(tokens.begin(),
                       tokens.end(),
                       [](std::string const& str) {
                           return str.size() == 0;
                       }),
        tokens.end());

    return tokens;
    std::vector<std::string> _tokens;
    std::string token;
    std::stringstream ss(s);
    while (std::getline(ss, token, ' ')) {
        if (token.empty()) continue;
        _tokens.push_back(token);
    }
    return _tokens;
}

namespace MessageTypes {

InfoMessage::InfoMessage() {
    depth = 0;
    seldepth = 0;
    time = 0;
    nodes = 0;
    multipv = 0;
    currmovenumber = 0;
    hashfull = 0;
    nps = 0;
    tbhits = 0;
    sbhits = 0;
    cpuload = 0;
}

std::ostream& operator<<(std::ostream& out, const InfoMessage& infoMessage) {
    out << "info ";

    if (infoMessage.depth > 0) out << "depth " << + infoMessage.depth << " ";
    if (infoMessage.seldepth > 0) out << "seldepth " << + infoMessage.seldepth << " ";
    if (infoMessage.time > 0) out << "time " << infoMessage.time << " ";
    if (infoMessage.nodes > 0) out << "nodes " << infoMessage.nodes << " ";
    if (infoMessage.multipv > 0) out << "multipv " << infoMessage.multipv << " ";
    if (! infoMessage.score.empty()) out << "score " << infoMessage.score << " ";
    if (! infoMessage.currmove.empty()) out << "currmove " << infoMessage.currmove << " ";
    if (infoMessage.currmovenumber > 0) out << "currmovenumber " << infoMessage.currmovenumber << " ";
    if (infoMessage.hashfull > 0) out << "hashfull " << infoMessage.hashfull << " ";
    if (infoMessage.nps > 0) out << "nps " << infoMessage.nps << " ";
    if (infoMessage.tbhits > 0) out << "tbhits " << infoMessage.tbhits << " ";
    if (infoMessage.sbhits > 0) out << "sbhits " << infoMessage.sbhits << " ";
    if (infoMessage.cpuload > 0) out << "cpuload " << infoMessage.cpuload << " ";
    if (infoMessage.refutation.size()) {
        out << "refutation ";
        for (std::string s : infoMessage.refutation) out << s << " ";
    }
    if (infoMessage.pv.size()) {
        out << "pv ";
        for (std::string s : infoMessage.pv) out << s << " ";
    }

    bool is_empty;
    int num_nonempty=0;
    for (std::vector<std::string> line : infoMessage.currline) {
        if (line.size()) num_nonempty++;
    }
    if (num_nonempty) {
        int i=1;
        is_empty = true;
        for (std::vector<std::string> line : infoMessage.currline) {
            if (line.size() == 0) continue;
            if (is_empty) {
                out << "currline ";
                is_empty = false;
            }
            if (num_nonempty > 1) out << i << " ";
            for (std::string move : line) {
                out << move << " ";
            }
            i++;
        }
    }
    if (infoMessage.string.size()) out << "string " << infoMessage.string;
    out << "\n";
    return out;
}


OptionMessage::OptionMessage() {
}

bool OptionMessage::valid() const {
    if (name == "") {
        return false;
    }
    else if (name == "Hash") {
        if (type != spin_type) return false;
    }
    else if (name == "NalimovPath") {
        if (type != string_type) return false;
    }
    else if (name == "NalimovCache") {
        if (type != spin_type) return false;
    }
    else if (name == "Ponder") {
        if (type != check_type) return false;
    }
    else if (name == "OwnBook") {
        if (type != check_type) return false;
    }
    else if (name == "MultiPV") {
        if (type != spin_type) return false;
        if (option_default != "1") return false;
    }
    else if (name == "UCI_ShowCurrLine") {
        if (type != check_type) return false;
    }
    else if (name == "UCI_ShowRefutations") {
        if (type != check_type) return false;
    }
    else if (name == "UCI_LimittStrength") {
        if (type != check_type) return false;
    }
    else if (name == "UCI_Elo") {
        if (type != spin_type) return false;
    }
    else if (name == "UCI_AnalyseMode") {
        if (type != check_type) return false;
    }
    else if (name == "UCI_Opponent") {
        if (type != string_type) return false;
    }
    else if (name == "UCI_EngineAbout") {
        if (type != string_type) return false;
    }
    else if (name == "UCI_ShredderbasesPath") {
        if (type != string_type) return false;
    }

    std::string lower = lower_string(option_default);

    switch (type) {
        case check_type:
            if (!(lower == "true" || lower == "false")) return false;
            break;
        case spin_type:
            if (option_default.empty() ||
                option_min.empty() ||
                option_max.empty()) return false;

            if (! (is_integer(option_default) &&
                   is_integer(option_min) &&
                   is_integer(option_max))) return false;

            break;
        case combo_type:
            if (vars.empty()) return false;
            if (option_default.empty()) return false;
            break;
        case button_type:
            break;
        case string_type:
            break;
        default:
            return false;
            break;
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const OptionMessage& optionMessage) {
    if (!optionMessage.valid()) return out;

    out << "option "
        << "name " << optionMessage.name << " ";
    switch(optionMessage.type) {
        case check_type:
            out << "type check "
                << "default " << optionMessage.option_default;
            break;
        case spin_type:
            out << "type spin "
                << "default " << optionMessage.option_default << " "
                << "min " << optionMessage.option_min << " "
                << "max " << optionMessage.option_max;
            break;
        case combo_type:
            out << "type combo "
                << "default " << optionMessage.option_default;
            for (std::string var : optionMessage.vars) {
                out << " var " << var;
            }
            break;
        case button_type:
            out << "type button";
            break;
        case string_type:
            out << "type string "
                << "default ";
            if (optionMessage.option_default.empty()) out << "<empty>";
            else out << optionMessage.option_default;
            break;
        default:
            break;
    }
    out << "\n";
    return out;
}

GoMessage::GoMessage() {
    ponder = false;
    wtime = 0;
    btime = 0;
    winc = 0;
    binc = 0;
    movestogo = 0;
    depth = 0;
    nodes = 0;
    mate = 0;
    movetime = 0;
    infinite = false;
}

} // end of chessUCI::MessageTypes namespace


chessInterface::chessInterface() :
                    cin(std::cin),
                    cout(std::cout),
                    cerr(std::cerr) {

    debug_mode = false;
    engine_name = "strawberry";
}

chessInterface::chessInterface(std::istream& in, std::ostream& out, std::ostream& err)  :
                    cin(in),
                    cout(out),
                    cerr(err) {

    debug_mode = false;
    engine_name = "strawberry";
}

void chessInterface::sendIDNameMessage(std::string name) const {
    cout << "id name " << name << "\n";
}
void chessInterface::sendIDAuthorMessage(std::string author) const {
    cout << "id author " << author << "\n";
}
void chessInterface::sendUCIOkMessage() const {
    cout << "uciok\n";
}
void chessInterface::sendReadyOkMessage() const {
    cout << "readyok\n;";
}
void chessInterface::sendBestMoveMessage(std::string move1, bool ponder, std::string move2) const {
    cout << "bestmove " << move1;
    if (ponder) {
        cout << " ponder " << move2;
    }
    cout<< "\n";
}
void chessInterface::sendCopyProtectionMessage(MessageTypes::CopyProtectMessage status) const {
    cout << "copyprotection ";
    switch (status) {
        case MessageTypes::CopyProtectMessage::checking_copyprotect:
            cout << "checking\n";
            break;
        case MessageTypes::CopyProtectMessage::ok_copyprotect:
            cout << "ok\n";
            break;
        case MessageTypes::CopyProtectMessage::error_copyprotect:
            cout << "error\n";
            break;
    }
}
void chessInterface::sendRegistrationMessage(MessageTypes::RegistrationMessage status) const {
    cout << "registration ";
    switch (status) {
        case MessageTypes::RegistrationMessage::checking_registration:
            cout << "checking\n";
            break;
        case MessageTypes::RegistrationMessage::ok_registration:
            cout << "ok\n";
            break;
        case MessageTypes::RegistrationMessage::error_registration:
            cout << "error\n";
            break;
    }
}
void chessInterface::sendInfoMessage(MessageTypes::InfoMessage infoMessage) const {
    cout << infoMessage;
}
void chessInterface::sendOptionMessage(MessageTypes::OptionMessage optionMessage) const {
    cout << optionMessage;
}

std::string chessInterface::readInput() const {
    std::string inputLine;
    std::getline(cin, inputLine);
    return inputLine;
}


#ifdef DUMMY_HANDLING
    void chessInterface::handleUCIMessage() {
        std::cout << "\t\tUCI message\n";
    }
    void chessInterface::handleDebugMessage(bool on) {
        std::cout << "\t\tDebug message: "
                  << (on ? "on" : "off") << "\n";
    }
    void chessInterface::handleIsReadyMessage() {
        std::cout << "\t\tIs ready message\n";
    }
    void chessInterface::handleSetOptionMessage(std::string name, std::string value) {
        std::cout << "\t\tSet option message: "
                  << "name = " << name
                  << ", value = " << value << "\n";
    }
    void chessInterface::handleRegisterMessage(bool later, std::string name, std::string code) {
        std::cout << "\t\tRegister message: ";
        if (later) std::cout << "later\n";
        else std::cout << "name = " << name
                       << ", code = " << code << "\n";
    }
    void chessInterface::handleUCINewGameMessage() {
        std::cout << "\t\tUCI new game message\n";
    }
    void chessInterface::handlePositionMessage(std::string position, std::vector<std::string> moves) {
        std::cout << "\t\tPosition message: "
                  << "position = " << position
                  << ", moves =";
        for (std::string s : moves) std::cout << " " << s;
        std::cout << "\n";
    }
    void chessInterface::handleGoMessage(MessageTypes::GoMessage goMessage) {
        std::cout << "\t\tGo message:\n";
        std::cout << "\t\tponder = " << (goMessage.ponder ? "true\n" : "false\n");
        std::cout << "\t\tinfinite = " << (goMessage.infinite ? "true\n" : "false\n");
        if (goMessage.wtime) std::cout << "\t\twtime = " << goMessage.wtime << "\n";
        if (goMessage.btime) std::cout << "\t\tbtime = " << goMessage.btime << "\n";
        if (goMessage.winc) std::cout << "\t\twinc = " << goMessage.winc << "\n";
        if (goMessage.binc) std::cout << "\t\tbinc = " << goMessage.binc << "\n";
        if (goMessage.movestogo) std::cout << "\t\tmovestogo = " << goMessage.movestogo << "\n";
        if (goMessage.depth) std::cout << "\t\tdepth = " << +goMessage.depth << "\n";
        if (goMessage.nodes) std::cout << "\t\tnodes = " << goMessage.nodes << "\n";
        if (goMessage.mate) std::cout << "\t\tmate = " << +goMessage.mate << "\n";
        if (goMessage.movetime) std::cout << "\t\tmovetime = " << goMessage.movetime << "\n";
        if (!goMessage.searchmoves.empty()) {
            std::cout << "\t\tsearchmoves =";
            for (std::string s : goMessage.searchmoves) {
                std::cout << " " << s;
            }
            std::cout << "\n";
        }
    }
    void chessInterface::handleStopMessage() {
        std::cout << "\t\tStop message\n";
    }
    void chessInterface::handlePonderHitMessage() {
        std::cout << "\t\tPonder Hit message\n";
    }
    void chessInterface::handleQuitMessage() {
        std::cout << "\t\tQuit message\n";
    }
#else
    void chessInterface::handleUCIMessage() {
        // send ID
        sendIDNameMessage(engine_name);
        sendIDAuthorMessage("Freddy Pringle");

        // send options
        // ...

        // ready
        sendUCIOkMessage();
    }
    void chessInterface::handleDebugMessage(bool on) {
        debug_mode = on;
    }
    void chessInterface::handleIsReadyMessage() {
        sendReadyOkMessage();
    }
    void chessInterface::handleSetOptionMessage(std::string name, std::string value) {
    }
    void chessInterface::handleRegisterMessage(bool later, std::string name, std::string code) {
        if (later) return;
        if (!name.empty()) registered_name = name;
        if (!code.empty()) registered_code = code;
    }
    void chessInterface::handleUCINewGameMessage() {
    }
    void chessInterface::handlePositionMessage(std::string position, std::vector<std::string> moves) {
    }
    void chessInterface::handleGoMessage(MessageTypes::GoMessage goMessage) {
    }
    void chessInterface::handleStopMessage() {
    }
    void chessInterface::handlePonderHitMessage() {
    }
    void chessInterface::handleQuitMessage() {
    }
#endif  // DUMMY_HANDLING


void chessInterface::inputLoop() {
    std::string tmp;
    while (true) {
        tmp = readInput();
        std::lock_guard<std::mutex> lock{mutex};
        inputLines.push_back(std::move(tmp));
        cv.notify_one();
    }
}

void chessInterface::processLoop() {
    while (true) {
        {
            std::unique_lock<std::mutex> lock{mutex};
            if (cv.wait_for(lock, std::chrono::seconds(0), [&]{
                return !inputLines.empty();
            })) {
                std::swap(inputLines, processLines);
            }
        }
        if (!processLines.empty()) {
            for (auto&& line : processLines) {
                parseMessage(line);
            }
            processLines.clear();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void chessInterface::mainLoop() {
    std::thread io{&chessUCI::chessInterface::inputLoop, this};
    processLoop();
}

void chessInterface::handleInvalidMessage(std::string message) {
    cerr << "Invalid message received: " << message << "\n";
}

namespace {

    bool isGoToken(std::string token) {
        return token == "searchmoves" ||
               token == "ponder" ||
               token == "wtime" ||
               token == "btime" ||
               token == "winc" ||
               token == "binc" ||
               token == "movestogo" ||
               token == "depth" ||
               token == "nodes" ||
               token == "mate" ||
               token == "movetime" ||
               token == "infinite";
    }

}

void chessInterface::parseMessage(std::string message) {
    std::vector<std::string> tokens = tokenise_string(message);
    if (tokens.empty()) return;

    std::string message_type = tokens[0];

    if (message_type == "uci") {
        handleUCIMessage();
    }
    else if (message_type == "debug") {
        if (tokens.size() < 2) {
            handleInvalidMessage(message);
            return;
        }
        if (tokens[1] == "on") handleDebugMessage(true);
        else handleDebugMessage(false);
    }
    else if (message_type == "isready") {
        handleIsReadyMessage();
    }
    else if (message_type == "setoption") {
        if (tokens.size() < 3 || tokens[1] != "name") {
            handleInvalidMessage(message);
        }
        std::stringstream option_name;
        std::stringstream option_value;
        int i = 2;
        bool reading_value = false;
        while (i < tokens.size()) {
            if (tokens[i] == "value") reading_value = true;
            else if (reading_value) {
                if (! option_value.str().empty()) option_value << " ";
                option_value << tokens[i];
            }
            else {
                if (! option_name.str().empty()) option_name << " ";
                option_name << tokens[i];
            }
            i++;
        }
        handleSetOptionMessage(option_name.str(), option_value.str());
    }
    else if (message_type == "register") {
        if (tokens.size() < 2) handleInvalidMessage(message);
        else if (tokens[1] == "later") {
            if (tokens.size() > 2) handleInvalidMessage(message);
            else handleRegisterMessage(true, "", "");
        }
        else {
            std::stringstream register_name;
            std::stringstream register_code;
            int i = 1;
            bool reading_code = false;
            while (i < tokens.size()) {
                if (tokens[i] == "name") reading_code = false;
                else if (tokens[i] == "code") reading_code = true;
                else if (reading_code) {
                    if (! register_code.str().empty()) register_code << " ";
                    register_code << tokens[i];
                }
                else {
                    if (! register_name.str().empty()) register_name << " ";
                    register_name << tokens[i];
                }
                i++;
            }
            handleRegisterMessage(false, register_name.str(), register_code.str());
        }
    }
    else if (message_type == "ucinewgame") {
        handleUCINewGameMessage();
    }
    else if (message_type == "position") {  // word moves
        if (tokens.size() < 2) handleInvalidMessage(message);
        if (tokens[1] == "fen" && tokens.size() < 3) handleInvalidMessage(message);
        std::vector<std::string> moves(tokens.begin() + 2, tokens.end());
        if (tokens[1] == "fen") moves.erase(moves.begin());
        if (moves.size()) moves.erase(moves.begin());
        handlePositionMessage(tokens[1], moves);
    }
    else if (message_type == "go") {
        MessageTypes::GoMessage goMessage;
        int i = 1;
        while (i < tokens.size()) {
            if (tokens[i] == "searchmoves") {
                i++;
                if (!(i < tokens.size() && !isGoToken(tokens[i]))) {
                    handleInvalidMessage(message);
                    return;
                }
                while (i < tokens.size() && !isGoToken(tokens[i])) {
                    goMessage.searchmoves.push_back(tokens[i]);
                    i++;
                }
            }
            else if (tokens[i] == "ponder") {
                goMessage.ponder = true;
                i++;
            }
            else if (tokens[i] == "wtime") {
                if (i + 1 >= tokens.size() || (! is_integer(tokens[i+1]))) {
                    handleInvalidMessage(message);
                    return;
                }
                goMessage.wtime = std::stoi(tokens[i+1]);
                i += 2;
            }
            else if (tokens[i] == "btime") {
                if (i + 1 >= tokens.size() || (! is_integer(tokens[i+1]))) {
                    handleInvalidMessage(message);
                    return;
                }
                goMessage.btime = std::stoi(tokens[i+1]);
                i += 2;
            }
            else if (tokens[i] == "winc") {
                if (i + 1 >= tokens.size() || (! is_integer(tokens[i+1]))) {
                    handleInvalidMessage(message);
                    return;
                }
                goMessage.winc = std::stoi(tokens[i+1]);
                i += 2;
            }
            else if (tokens[i] == "binc") {
                if (i + 1 >= tokens.size() || (! is_integer(tokens[i+1]))) {
                    handleInvalidMessage(message);
                    return;
                }
                goMessage.binc = std::stoi(tokens[i+1]);
                i += 2;
            }
            else if (tokens[i] == "movestogo") {
                if (i + 1 >= tokens.size() || (! is_integer(tokens[i+1]))) {
                    handleInvalidMessage(message);
                    return;
                }
                goMessage.movestogo = std::stoi(tokens[i+1]);
                i += 2;
            }
            else if (tokens[i] == "depth") {
                if (i + 1 >= tokens.size() || (! is_integer(tokens[i+1]))) {
                    handleInvalidMessage(message);
                    return;
                }
                goMessage.depth = std::stoi(tokens[i+1]);
                i += 2;
            }
            else if (tokens[i] == "nodes") {
                if (i + 1 >= tokens.size() || (! is_integer(tokens[i+1]))) {
                    handleInvalidMessage(message);
                    return;
                }
                goMessage.nodes = std::stoi(tokens[i+1]);
                i += 2;
            }
            else if (tokens[i] == "mate") {
                if (i + 1 >= tokens.size() || (! is_integer(tokens[i+1]))) {
                    handleInvalidMessage(message);
                    return;
                }
                goMessage.mate = std::stoi(tokens[i+1]);
                i += 2;
            }
            else if (tokens[i] == "movetime") {
                if (i + 1 >= tokens.size() || (! is_integer(tokens[i+1]))) {
                    handleInvalidMessage(message);
                    return;
                }
                goMessage.movetime = std::stoi(tokens[i+1]);
                i += 2;
            }
            else if (tokens[i] == "infinite") {
                goMessage.infinite = true;
                i++;
            }
            else {
                handleInvalidMessage(message);
                return;
            }
        }
        handleGoMessage(goMessage);
    }
    else if (message_type == "stop") {
        handleStopMessage();
    }
    else if (message_type == "ponderhit") {
        handlePonderHitMessage();
    }
    else if (message_type == "quit") {
        handleQuitMessage();
    }
    else {
        handleInvalidMessage(message);
    }
}

} // end of chessUCI namespace

