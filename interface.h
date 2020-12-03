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

/** A sub-namespace with some enums and structs for parsing convenience. */
namespace MessageTypes {
    /**
     *  An enum representing the different types of message that the engine can
     *  receive from the GUI.
     */
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

    /**
     *  An enum representing the different types of message that the engine can
     *  send the GUI.
     */
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

    /** An enum representing the copyprotect status options. */
    enum CopyProtectMessage {
        checking_copyprotect,
        ok_copyprotect,
        error_copyprotect
    };

    /** An enum representing the registration status options. */
    enum RegistrationMessage {
        checking_registration,
        ok_registration,
        error_registration
    };

    /** A struct representing an info message from the engine to the GUI. */
    struct InfoMessage {
        /** Search depth in plies. */
        uint8_t depth;
        /** Selection search depth in plies. */
        uint8_t seldepth;
        /** Time spent searching in milliseconds. */
        uint32_t time;
        /** Number of nodes searched. */
        uint32_t nodes;
        /** Principal variation, i.e. the best line found. */
        std::vector<std::string> pv;
        /** Number of PVs. Only used in multi-pv mode. */
        uint8_t multipv;
        /** String representation of the score. */
        std::string score;
        /** Currently searching this move. */
        std::string currmove;
        /** Number of the move currently being searched. */
        uint16_t currmovenumber;
        /** How full the hash table is, out of 1000. */
        uint16_t hashfull;
        /** Number of nodes searched per second. */
        uint32_t nps;
        /** Number of hits in the endgame tablebases. */
        uint16_t tbhits;
        /** Number of hits in the shredder endgame tablebases. */
        uint16_t sbhits;
        /** Current CPU usage of the engine, out of 1000. */
        uint16_t cpuload;
        /**
         *  The first move of refutation is refuted by the line formed by the rest
         *  of refutation.
         */
        std::vector<std::string> refutation;
        /** The current line being searched by each cpu. */
        std::vector<std::vector<std::string>> currline;
        /** A string to be displayed by the engine. */
        std::string string;


        InfoMessage();
        /**
         *  Print the info message to an output stream.
         *
         *  \param out              The output stream to print to.
         *  \param infoMessage      The info message to print.
         *
         *  \return                 The output stream.
         */
        friend std::ostream& operator<<(std::ostream& out, const InfoMessage& infoMessage);
    };

    /** An enum representing the possible types of an option. */
    enum OptionTypeValue {
        check_type,
        spin_type,
        combo_type,
        button_type,
        string_type
    };

    /** A struct representing an option message from the engine to the GUI. */
    struct OptionMessage {
        /** The name of the option. */
        std::string name;
        /** The type of the option. See \ref OptionTypeValue. */
        OptionTypeValue type;
        /** The default value for the option. */
        std::string option_default;
        /** The minimum value for the option, if relevant. */
        std::string option_min;
        /** The maximum value for the option, if relevant. */
        std::string option_max;
        /** The possible values of option, if relevant. */
        std::vector<std::string> vars;

        OptionMessage();

        /**
         *  Check if the option message is valid.
         *
         *  \return                 True if the message is valid, false otherwise.
         */
        bool valid() const;

        /**
         *  Print the option message to an output stream.
         *
         *  \param out              The output stream to print to.
         *  \param optionMessage    The option message to print.
         *
         *  \return                 The output stream.
         */
        friend std::ostream& operator<<(std::ostream& out, const OptionMessage& optionMessage);
    };

    /** A struct representing a go message from the GUI to the engine. */
    struct GoMessage {
        /** Restrict search to these move only. */
        std::vector<std::string> searchmoves;
        /** Start searching in ponder mode. */
        bool ponder;
        /** Amount of time white has on the clock in milliseconds. */
        uint32_t wtime;
        /** Amount of time black has on the clock in milliseconds. */
        uint32_t btime;
        /** White increment per move in milliseconds. */
        uint32_t winc;
        /** Black increment per move in milliseconds. */
        uint32_t binc;
        /** Number of moves to the next time control. */
        uint16_t movestogo;
        /** Maximum search depth in plies.*/
        uint8_t depth;
        /** Maximum number of nodes to search. */
        uint32_t nodes;
        /** Search for mate in this many moves. */
        uint8_t mate;
        /** Maximum search time in milliseconds.. */
        uint32_t movetime;
        /** Search forever until a "stop" message from the GUI. */
        bool infinite;

        GoMessage();
    };

} // end of chessUCI::MessageTypes namespace

/** A class to send and receive messages to and from the GUI using the UCI protocol. */
class chessInterface {
private:
    /** The input stream to read from. */
    std::istream& cin;
    /** The output stream to write to. */
    std::ostream& cout;
    /** The erro stream to write to. */
    std::ostream& cerr;

    /** Boolean indicating whether the engine is in debug mode. */
    bool debug_mode;
    /** The name of the engine. */
    std::string engine_name;
    /** The registered name of the engine/ */
    std::string registered_name;
    /** The registered code of the engine/ */
    std::string registered_code;

    /** A condition variable for threading. */
    std::condition_variable cv;
    /** A mutex for threading. */
    std::mutex mutex;
    /** A deque to hold the linees read from cin. */
    std::deque<std::string> inputLines;
    /** A deque to hold the lines to be parsed. */
    std::deque<std::string> processLines;
public:
    /** Default constructor for chessInterface. */
    chessInterface();
    /**
     *  Parameterised constructor for chessInterface.
     *
     *  \param in           The input stream to use.
     *  \param out          The output stream to use.
     *  \param err          The error stream to use.
     */
    chessInterface(std::istream& in, std::ostream& out, std::ostream& err);

    /**
     *  Send an "id" message specifying the name of the engine.
     *
     *  \param name         The name of the engine.
     */
    void sendIDNameMessage(std::string name) const;

    /**
     *  Send an "id" message specifying the author of the engine.
     *
     *  \param author       The author of the engine.
     */
    void sendIDAuthorMessage(std::string author) const;

    /** Send a "uciok" message. */
    void sendUCIOkMessage() const;

    /** Send a "readyok" message. */
    void sendReadyOkMessage() const;

    /**
     *  Send a "bestmove" message.
     *
     *  \param move1        The caluclated best move.
     *  \param ponder
     *  \param move2
     */
    void sendBestMoveMessage(std::string move1, bool ponder=false, std::string move2="") const;

    /**
     *  Send a "copyprotect" message.
     *
     *  \param status       The copyprotection status.
     */
    void sendCopyProtectionMessage(MessageTypes::CopyProtectMessage status) const;

    /**
     *  Send a "registration" message.
     *
     *  \param status       The registration status.
     */
    void sendRegistrationMessage(MessageTypes::RegistrationMessage status) const;

    /**
     *  Send an "info" message.
     *
     *  \param infoMessage      The information to send.
     */
    void sendInfoMessage(MessageTypes::InfoMessage infoMessage) const;

    /**
     *  Send an "option" message.
     *
     *  \param optionMessage    The information to send about the options.
     */
    void sendOptionMessage(MessageTypes::OptionMessage optionMessage) const;

    /**
     *  Handle a "uci" message from the GUI.
     */
    void handleUCIMessage();

    /**
     *  Handle a "gebug" message from the GUI.
     *
     *  \param on               Whether the GUI wants the engine to turn
     *                          debuggin on or off.
     */
    void handleDebugMessage(bool on);

    /**
     *  Handle an "isready" message from the GUI.
     */
    void handleIsReadyMessage();

    /**
     *  Handle a "setoption" message from the GUI.
     *
     *  \param name             The name of the option to set.
     *  \param value            The value to set.
     */
    void handleSetOptionMessage(std::string name, std::string value="");

    /**
     *  Handle a "register" message from the GUI.
     *
     *  \param later            Whether or not to register later.
     *  \param name             The name to register.
     *  \param code             The code to register.
     */
    void handleRegisterMessage(bool later, std::string name, std::string code);

    /**
     *  Handle a "ucinewgame" message from the GUI.
     */
    void handleUCINewGameMessage();

    /**
     *  Handle a "position" message from the GUI.
     *
     *  \param position         The position of the board in FEN format.
     *  \param moves            A vector of moves in SAN format.
     */
    void handlePositionMessage(std::string position, std::vector<std::string> moves);

    /**
     *  Handle a "go" message from the GUI.
     *
     *  \param goMessage        A GoMessage object containing the information of
     *                          the "go" message
     */
    void handleGoMessage(MessageTypes::GoMessage goMessage);

    /**
     *  Handle a "stop" message from the GUI.
     */
    void handleStopMessage();

    /**
     *  Handle a "ponderhit" message from the GUI.
     */
    void handlePonderHitMessage();

    /**
     *  Handle a "quit" message from the GUI.
     */
    void handleQuitMessage();

    /**
     *  Handle an invalid message from the GUI.
     *
     *  \param message          The invalid message that was received.
     */
    void handleInvalidMessage(std::string message);

    /**
     *  Read a line of input and return it as a string.
     *
     *  \return                 The line of input from the GUI.
     */
    std::string readInput() const;

    /**
     *  An infinite loop that reads messages from the GUI and adds them to
     *  \ref inputLines.
     */
    void inputLoop();

    /**
     *  An infinite loop that reads messages from the \ref inputLines,
     *  parses them, and takes the appropriate action.
     */
    void processLoop();

    /** The main loop of the interface. */
    void mainLoop();

    /**
     *  Parse a message and take the appropriate action.
     *
     *  \param message          The message to parse.
     */
    void parseMessage(std::string message);
};


} // end of chessUCI namespace

#endif
