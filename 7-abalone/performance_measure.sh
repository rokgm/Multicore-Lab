#!/bin/bash

# Number of games to be played
NUM_GAMES=10
# Strategy level for players
STRATEGY_X=0
STRATEGY_O=2
DEPTH=3

# Counters for results
wins_X=0
wins_O=0
draws=0

# Function to run a single game
run_game() {
  # Start both players in the background
  ./player -s $STRATEGY_X X $DEPTH > /dev/null 2>&1 &
  PLAYER_X_PID=$!
  ./player -s $STRATEGY_O O $DEPTH > /dev/null 2>&1 &
  PLAYER_O_PID=$!

  # Run the game and capture the output
  GAME_OUTPUT=$(./start)

  # Wait for the players to finish
  wait $PLAYER_X_PID
  wait $PLAYER_O_PID

  # Process the game output
  if [[ "$GAME_OUTPUT" == *"O wins the game!"* ]]; then
    ((wins_O++))
  elif [[ "$GAME_OUTPUT" == *"X wins the game!"* ]]; then
    ((wins_X++))
  else
    ((draws++))
  fi
}

# Kill previous instances of the player
pkill -f player

# Main loop to run multiple games
for ((i=1; i<=NUM_GAMES; i++)); do
    echo "Starting game #$i"
    run_game
done

# Print final results
echo "Final Results after $NUM_GAMES games with strategies X: $STRATEGY_X, O: $STRATEGY_O, depth=$DEPTH:"
echo "X wins: $wins_X"
echo "O wins: $wins_O"
echo "Draws: $draws"
