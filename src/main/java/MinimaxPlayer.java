package main.java;

public class MinimaxPlayer extends RandomPlayer {

    private double heuristic(int[][] state, int player) {

        return getWinner(state) * player;
    }

    @SuppressWarnings("SameParameterValue")
    private double minimax(int[][] state, int depth) {

        int player = currentPlayer(state);

        return minimax(state, depth, player)[1];
    }

    private double[] minimax(int[][] state, int depth, int player){

        double[] result = {Double.NEGATIVE_INFINITY,-1};

        if (this.isTerminal(state) || depth == 0){
            result[0] = heuristic(state, player);
            return result;
        }

        validMoves(state).forEach((move) -> {
            double[] minimax = minimax(resultingState(state, move), depth-1, player*-1);
            if (result[0] < -minimax[0]) {
                result[0] = -minimax[0];
                result[1] = move;
            }
        });

        return result;

    }

    @Override
    public int play(int[][] state) {
        double result = minimax(state, 99);
        return (int) result;
    }
    public static void main(String[] args) {
        MinimaxPlayer rp = new MinimaxPlayer();
        int move = rp.play(rp.stringToState(args[0]));
        System.out.println(move);
    }
}

