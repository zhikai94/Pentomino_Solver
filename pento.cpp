#include "pentomino_utils.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(void){
    // Pentomino::VisualisePiece();
    auto board = Pentomino::ReadBoard();
    Pentomino::VisualiseBoard(board);

    Pentomino::SolveBoard(board);

    std::cout << "--x--" << std::endl;
    return 0;
}