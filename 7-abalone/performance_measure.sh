#!/bin/bash

# Number of games to be played
NUM_GAMES=1
# Strategy level for players
STRATEGY_X=5
STRATEGY_O=5
DEPTH_X=5
DEPTH_O=5

############################################################################ 
# Set the number of threads for OpenMP
############################################################################
export OMP_NUM_THREADS=8

# Counters for results
wins_X=0
wins_O=0
draws=0

# Function to run a single game
run_game() {
  #########################################################################
  # Change args here
  #########################################################################

  ./player -p 3000 -s $STRATEGY_X $DEPTH_X X  > ./logs/X_player.txt 2>&1 &
  PLAYER_X_PID=$!
  ./player -p 4000 -s $STRATEGY_O $DEPTH_O O  > ./logs/O_player.txt 2>&1 &
  PLAYER_O_PID=$!
  # GAME_OUTPUT=$(./referee -p 3000 -p 4000 -t 15)
  # # If running like this output is not captured and statistics aren't correct.
  ./referee -p 3000 -p 4000 -t 15

  # For measurements
  # ./player -p 3000 -v -n -1 -s $STRATEGY_X $DEPTH_X X > ./logs/X_player.txt 2>&1 &
  # PLAYER_X_PID=$!
  # ./player -p 4000 -v -n -1 -s $STRATEGY_O $DEPTH_O O > ./logs/O_player.txt 2>&1 &
  # PLAYER_O_PID=$!
  # ./referee -p 3000 -p 4000

  #########################################################################

  # Wait for the players to finish
  wait $PLAYER_X_PID
  wait $PLAYER_O_PID

  echo $GAME_OUTPUT > ./logs/game_output.txt

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
    pkill -P $$
    pkill player

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
