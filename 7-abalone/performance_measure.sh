#!/bin/bash

# Number of games to be played
NUM_GAMES=4
# Strategy level for players
STRATEGY_X=3
STRATEGY_O=3
DEPTH_X=3
DEPTH_O=3

# Counters for results
wins_X=0
wins_O=0
draws=0

# Function to run a single game
run_game() {
  # Start both players in the background
  ./player -p 3000 -s $STRATEGY_X $DEPTH_X X > /dev/null 2>&1 &
  PLAYER_X_PID=$!
  ./player -p 4000 -s $STRATEGY_O $DEPTH_O O > /dev/null 2>&1 &
  PLAYER_O_PID=$!

  # Run the game and capture the output
  GAME_OUTPUT=$(./referee -p 3000 -p 4000 -t 15)
  # Comment above and uncomment below line to see the game output, game statistics
  # are not correct then.
  # ./referee -p 3000 -p 4000 -t 30

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

# Main loop to run multiple games
for ((i=1; i<=NUM_GAMES; i++)); do
    echo "Starting game #$i"

    # Kill previous instances of the player
    pkill -f player

    run_game
    echo "Intermidiate results after $NUM_GAMES games with strategies X: $STRATEGY_X, O: $STRATEGY_O:"
    echo "X wins: $wins_X"
    echo "O wins: $wins_O"
    echo "Draws: $draws"
done

# Print final results
echo "Final Results after $NUM_GAMES games with strategies X: $STRATEGY_X, O: $STRATEGY_O:"
echo "X wins: $wins_X"
echo "O wins: $wins_O"
echo "Draws: $draws"
