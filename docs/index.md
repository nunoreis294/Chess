# Chess Documentation

This folder contains Doxygen configuration to generate API documentation for the Chess project. To generate HTML docs:

```
cd docs
doxygen Doxyfile
```

The generated output will appear in docs/doxygen_output/html

## Recent gameplay changes

- Added end-of-game detection for checkmate and draw states.
- Added a game-over overlay with a Restart button.
- Added a Resign button in the side menu to allow players to concede.
- Result messages now distinguish between resignation, checkmate, and draw.
