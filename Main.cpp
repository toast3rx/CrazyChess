
#include <bits/stdc++.h>

#include <cassert>

#include "Bot.h"
#include "Move.h"
#include "Piece.h"
#include "PlaySide.h"

static PlaySide sideToMove;
static PlaySide engineSide;

static void toggleSideToMove() {
    static const PlaySide switchTable[] = {
        [BLACK] = WHITE,
        [WHITE] = BLACK,
        [NONE] = NONE
    };

    sideToMove = switchTable[sideToMove];
}

static std::string constructFeaturesPayload() {
  std::stringstream payload;
  payload << "feature"
          << " done=0"
          << " sigint=0"
          << " sigterm=0"
          << " san=0"
          << " reuse=0"
          << " usermove=1"
          << " analyze=0"
          << " ping=0"
          << " setboard=0"
          << " level=0"
          << " variants=\"crazyhouse\""
          << " name=\"" << Bot::getBotName() << "\" myname=\""
          << Bot::getBotName() << "\" done=1\n";
  return payload.str();
}

static std::string serializeMove(Move* move) {
  if (move->isNormal())
    return move->getSource().value() + move->getDestination().value();
  else if (move->isPromotion()) {
    std::string pieceCode = "";
    switch (move->getReplacement().value()) {
      case Piece::BISHOP:
        pieceCode = "b";
        break;
      case Piece::KNIGHT:
        pieceCode = "n";
        break;
      case Piece::ROOK:
        pieceCode = "r";
        break;
      case Piece::QUEEN:
        pieceCode = "q";
        break;
      default:
        break;
    }
    return move->getSource().value() + move->getDestination().value() +
           pieceCode;
  } else if (move->isDropIn()) {
    std::string pieceCode = "";
    switch (move->getReplacement().value()) {
      case Piece::BISHOP:
        pieceCode = "B";
        break;
      case Piece::KNIGHT:
        pieceCode = "N";
        break;
      case Piece::ROOK:
        pieceCode = "R";
        break;
      case Piece::QUEEN:
        pieceCode = "Q";
        break;
      case Piece::PAWN:
        pieceCode = "P";
        break;
      default:
        break;
    };
    return pieceCode + "@" + move->getDestination().value();
  }

  return "resign";
}

static Move* deserializeMove(std::string s) {
  if (s[1] == '@') {
    /* Drop-in */
    std::optional<Piece> piece;
    switch (s[0]) {
      case 'P':
        piece = Piece::PAWN;
        break;
      case 'R':
        piece = Piece::ROOK;
        break;
      case 'B':
        piece = Piece::BISHOP;
        break;
      case 'N':
        piece = Piece::KNIGHT;
        break;
      case 'Q':
        piece = Piece::QUEEN;
        break;
      case 'K':
        piece = Piece::KING; /* This is an illegal move */
        break;
      default:
        piece = {};
        break;
    };
    return Move::dropIn(s.substr(2, 4), piece);
  } else if (s.length() == 5) {
    /* Pawn promotion */
    std::optional<Piece> piece;
    switch (s[4]) {
      case 'p':
        piece = Piece::PAWN; /* This is an illegal move */
        break;
      case 'r':
        piece = Piece::ROOK;
        break;
      case 'b':
        piece = Piece::BISHOP;
        break;
      case 'n':
        piece = Piece::KNIGHT;
        break;
      case 'q':
        piece = Piece::QUEEN;
        break;
      case 'k':
        piece = Piece::KING; /* This is an illegal move */
        break;
      default:
        piece = {};
        break;
    };
    return Move::promote(s.substr(0, 2), s.substr(2, 4), piece);
  }

  /* Normal move/capture/castle/en passant */
  return Move::moveTo(s.substr(0, 2), s.substr(2, 4));
}

class EngineComponents {
 private:
  enum EngineState {
    HANDSHAKE_DONE = 0,
    RECV_NEW = 1,
    PLAYING = 2,
    FORCE_MODE = 3
  };
  void emitMove(Move* move) {
    if (move->isDropIn() || move->isNormal() || move->isPromotion())
      std::cout << "move ";
    std::cout << serializeMove(move) << "\n";
  }

 public:
  std::optional<Bot*> bot;
  std::optional<EngineState> state;
  std::optional<std::string> bufferedCmd;
  std::istream& scanner;
  bool isStarted;

  void performHandshake() {
      /* Await start command ("xboard") */
      std::string command;
      getline(scanner, command);
      assert(command == "xboard");
      std::cout << "\n";

      getline(scanner, command);
      assert(command.rfind("protover", 0) == 0);

      /* Respond with features */
      std::string features = constructFeaturesPayload();
      std::cout << features;

      while (true) {
          getline(scanner, command);
          if (command == "new" || command == "force" || command == "go" || command == "quit") {
              bufferedCmd = command;
              break;
          }
      }

      state = EngineState::HANDSHAKE_DONE;
  }

  void newGame() {
      delete bot.value_or(nullptr);
      bot = new Bot();
      state = EngineState::RECV_NEW;
      engineSide = PlaySide::NONE;
      sideToMove = PlaySide::WHITE;
      isStarted = false;
  }

  void enterForceMode() {
      state = EngineState::FORCE_MODE;
  }

  void leaveForceMode() {
    /* Called upon receiving "go" */
    state = EngineState::PLAYING;

    if (!isStarted) {
      isStarted = true;
      engineSide = sideToMove;
    }

    /* Make next move (go is issued when it's the bot's turn) */
    Move *move = bot.value()->calculateNextMove();
    emitMove(move);

    delete move;
    toggleSideToMove();
  }

  void processIncomingMove(Move *move) {
    if (state.value() == FORCE_MODE || state.value() == RECV_NEW) {
      bot.value()->recordMove(move, sideToMove);
      toggleSideToMove();
    } else if (state.value() == PLAYING) {
      bot.value()->recordMove(move, sideToMove);
      toggleSideToMove();

      Move *response = bot.value()->calculateNextMove();
      emitMove(response);

      delete response;
      toggleSideToMove();
    } else {
      std::cerr << "[WARNING]: Unexpected move received (prior to new command)\n";
    }
  }

  EngineComponents() : scanner(std::cin) {
    bot = {};
    state = {};
    bufferedCmd = {};
    scanner.rdbuf()->pubsetbuf(0, 0);
    isStarted = false;
  }

  void executeOneCommand() {
    std::string nextCmd;
    if (bufferedCmd.has_value()) {
      nextCmd = bufferedCmd.value();
      bufferedCmd = {};
    } else {
      getline(scanner, nextCmd);
    }

    std::stringstream command_stream(nextCmd);
    std::string command;
    getline(command_stream, command, ' ');

    if (command == "quit") {
      exit(0);
    } else if (command == "new") {
      newGame();
    } else if (command == "force") {
      enterForceMode();
    } else if (command == "go") {
      leaveForceMode();
    } else if (command == "usermove") {
      std::string movePayload;
      getline(command_stream, movePayload, ' ');
      Move* incomingMove = deserializeMove(movePayload);

      processIncomingMove(incomingMove);
      delete incomingMove;
    }
  }
};

int main() {
  EngineComponents* engine = new EngineComponents();
  engine->performHandshake();

  while (true) {
    /* Fetch and execute next command */

    engine->executeOneCommand();
  }
  return 0;
}
