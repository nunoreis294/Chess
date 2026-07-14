# Assistant Notes

Short guidance and notes intended for maintainers and the assistant.

- Coordinate systems: UI uses 1-based board coordinates (1..8). Internal arrays are 0-based. Convert consistently.
- moveHistory: last move used for en passant detection. When simulating en passant, remove the captured pawn from its square in the simulated board before calling isKingChecked.
- King-safety checks: use Board::leavesKingInCheck when filtering possible or attacked squares.
- Naming: functions use camelCase (getPossibleSquares, getAttackedSquares).

Quick commands
- Build (PowerShell): msbuild Chess.sln /p:Configuration=Release

Where to look first for issues
- board.cpp - move generation, check logic, special moves
- gui.cpp - input coordinates, drawing, selection logic

Recent gameplay and UI changes
- End-of-game handling was added for checkmate and draw states.
- The UI now shows a game-over overlay with a Restart button after a finished game.
- A Resign button was added to the side menu so the current player can concede immediately.
- Game result state is evaluated after successful moves and promotions, and the overlay uses the detected outcome.
