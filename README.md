# Gomoku AI Project

Welcome to the repository for our **Gomoku AI Project**, a team effort by two Epitech students. This project was completed within a two-week timeline as part of our school curriculum, and the primary goal was to develop the most effective Gomoku AI possible.

## Overview
Gomoku, also known as "Five in a Row," is a classic board game where the objective is to align five pieces in a row on a grid. The project involved implementing an intelligent AI that could:

- Efficiently evaluate the game board.
- Make strategic decisions to win against opponents.

## Key Features
- **Algorithm**: The core of our AI relies on the **Min-Max algorithm** enhanced with **Alpha-Beta pruning** to improve efficiency by reducing the number of nodes explored.
- **Handwritten Evaluation Function**: We designed a custom evaluation function to assess the board state. This function identifies and evaluates patterns crucial to the game's strategy, such as lines of two, three, or four consecutive pieces.
- **Optimizations**:
  - **Rotated Bit Boards**: These were utilized to accelerate pattern detection.
  - **Threat Search**: When an immediate threat was identified, our AI performed focused searches to address it.
  - **Proximity Pruning**: The search tree was limited to moves within two spaces of existing pieces, significantly reducing computational overhead.

## Challenges
Initially, we aimed to implement a **machine learning-based evaluation function** to leverage data-driven insights for board evaluation. Unfortunately, due to the tight two-week timeline, we pivoted to a handwritten evaluation after experimenting with ML approaches.

## Tournament Victory
After the completion of the project, our school organized a Gomoku AI tournament. Competing against other teams, our AI emerged as the winner, demonstrating the effectiveness of our approach and optimizations.

## Technologies Used
- **Programming Language**: C++
- **External Libraries**: No external libraries were used during this project since they were forbidden.

## Protocol Compatibility
Our AI is designed to work with the **Gomoku AI Protocol**. For more details about the protocol, refer to the official documentation: [Gomoku AI Protocol](https://plastovicka.github.io/protocl2en.htm).

## How to Run
1. Clone this repository:
   ```bash
   git clone git@github.com:emartin2109/GomokuAI.git
   ```
2. Compile the code:
   ```bash
   make
   ```
3. Run the AI:
   ```bash
   ./pbrain-gomoku-ai
   ```

## Team
- **Eliot Martin**
- **Jean-Baptiste Stoquer**

## Acknowledgments
This project was an excellent opportunity to deepen our understanding of AI algorithms and optimization techniques. Many thanks to our mentors and peers at Epitech for their support.
