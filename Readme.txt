Our group has two seperate folders for this project.

The "single ant" folder satisfies the requirements of the project and uses an algorithm we developed so the ant will sufficiently search the maze and find as many points as possible.
While thorough, this algorithm uses quite a few number of steps. 

The "evolution algorithm" folder is supplementry and included because we thought it was a good addition to the project.
It starts with 6000 (or any given number assigned to the constant) original "ants" whose inputs are determined randomly.
Slowly - using a process that mimics evolution - the ants "evolve" for a given number of cycles to find a path that finds the most possible points.
The ant is usually close to fully "evolved" around 150 cycles.
The amount of points found using this method are significantly greater for any given number of steps.
The algorithm works better for lower number of steps, but is still much better than the other algorithm at higher step values.

In both files we included a "game player" which shows a visual representation of the ants actions to make it easier to follow.
While a good addition (especially for debugging for us) we are aware it can be cumbersome to watch.
To solve this simply comment out the "delay(1);" line to make the project go faster.

Both projects also utilize 20x20 mazes. This can be changed by changing the "MAX_X_SIZE" and "MAX_Y_SIZE" requirements.
Each maze we made also had a "D" (door) at some outside portion of the maze, which is where the ant spawns.

-Jacob Gerow, Matthew Leo, Brendan Pattermann, and Joshua Jeung