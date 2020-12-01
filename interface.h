#ifndef __CHESS_INTERFACE_H
#define __CHESS_INTERFACE_H

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <deque>
#include <thread>
#include <condition_variable>
#include <chrono>


/**
 *  \namespace chessUCI
 *  \brief The namespace for the UCI interface of the chess engine.
 *
 *  The namespace chessUCI contains all the classes, functions, constants etc
 *  required so that strawberry can communicate with a GUI via the
 *  Universal Chess Interface (UCI) protocol.
 */
namespace chessUCI {

/**
 *  Convenience function to convert a string to uppercase.
 *
 *  \param s                The string to convert.
 *  \return                 The string s converted to uppercase.
 */
std::string upper_string(std::string s);

/**
 *  Convenience function to convert a string to lowercase.
 *
 *  \param s                The string to convert.
 *  \return                 The string s converted to lowercase.
 */
std::string lower_string(std::string s);

/**
 *  Convenience function to test if a string represents an integer.
 *
 *  \param s                The string to test.
 *  \return                 True if s represents an integer, false otherwise.
 */
bool is_integer(std::string s);

/**
 *  Convenience function to split a string into tokens based on a delimiter.
 *
 *  \param s                The string to split.
 *  \return                 A vector of the split token.
 */
std::vector<std::string> tokenise_string(std::string s);

namespace MessageTypes {
    enum GUIMessage {
        uci_message,
        debug_message,
        isready_message,
        setoption_message,
        register_message,
        ucinewgame_message,
        position_message,
        go_message,
        stop_message,
        ponderhit_message,
        quit_message
    };
    enum EngineMessage {
        id_message,
        uciok_message,
        readyok_message,
        bestmove_message,
        copyprotection_message,
        registration_message,
        info_message,
        option_message
    };
    enum CopyProtectMessage {
        checking_copyprotect,
        ok_copyprotect,
        error_copyprotect
    };
    enum RegistrationMessage {
        checking_registration,
        ok_registration,
        error_registration
    };
    struct InfoMessage {
        uint8_t depth;
        uint8_t seldepth;
        uint32_t time;
        uint32_t nodes;
        std::vector<std::string> pv;
        uint8_t multipv;
        std::string score;
        std::string currmove;
        uint16_t currmovenumber;
        uint16_t hashfull;
        uint32_t nps;
        uint16_t tbhits;
        uint16_t sbhits;
        uint16_t cpuload;
        std::vector<std::string> refutation;
        std::vector<std::vector<std::string>> currline;
        std::string string;


        InfoMessage();
        friend std::ostream& operator<<(std::ostream& out, const InfoMessage& infoMessage);
    };
    enum OptionTypeValue {
        check_type,
        spin_type,
        combo_type,
        button_type,
        string_type
    };
    struct OptionMessage {
        std::string name;
        OptionTypeValue type;
        std::string option_default;
        std::string option_min;
        std::string option_max;
        std::vector<std::string> vars;

        OptionMessage();
        bool valid() const;
        friend std::ostream& operator<<(std::ostream& out, const OptionMessage& optionMessage);
    };
    struct GoMessage {
        std::vector<std::string> searchmoves;
        bool ponder;
        uint32_t wtime;
        uint32_t btime;
        uint32_t winc;
        uint32_t binc;
        uint16_t movestogo;
        uint8_t depth;
        uint32_t nodes;
        uint8_t mate;
        uint32_t movetime;
        bool infinite;

        GoMessage();
    };

} // end of chessUCI::MessageTypes namespace


class chessInterface {
private:
    std::istream& cin;
    std::ostream& cout;
    std::ostream& cerr;

    bool debug_mode;
    std::string engine_name;
    std::string registered_name;
    std::string registered_code;

    std::condition_variable cv;
    std::mutex mutex;
    std::deque<std::string> inputLines;
    std::deque<std::string> processLines;
public:
    chessInterface();
    chessInterface(std::istream& in, std::ostream& out, std::ostream& err);

    void sendIDNameMessage(std::string name) const;
    void sendIDAuthorMessage(std::string author) const;
    void sendUCIOkMessage() const;
    void sendReadyOkMessage() const;
    void sendBestMoveMessage(std::string move1, bool ponder=false, std::string move2="") const;
    void sendCopyProtectionMessage(MessageTypes::CopyProtectMessage status) const;
    void sendRegistrationMessage(MessageTypes::RegistrationMessage status) const;
    void sendInfoMessage(MessageTypes::InfoMessage infoMessage) const;
    void sendOptionMessage(MessageTypes::OptionMessage optionMessage) const;

    void handleUCIMessage();
    void handleDebugMessage(bool on);
    void handleIsReadyMessage();
    void handleSetOptionMessage(std::string name, std::string value="");
    void handleRegisterMessage(bool later, std::string name, std::string code);
    void handleUCINewGameMessage();
    void handlePositionMessage(std::string position, std::vector<std::string> moves);
    void handleGoMessage(MessageTypes::GoMessage goMessage);
    void handleStopMessage();
    void handlePonderHitMessage();
    void handleQuitMessage();

    void handleInvalidMessage(std::string message);

    std::string readInput() const;
    void inputLoop();
    void processLoop();
    void mainLoop();
    void parseMessage(std::string message);
};


} // end of chessUCI namespace

#endif
