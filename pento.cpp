#include "pentomino_utils.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char** argv){
    if(argc != 2){
        std::cout << "Usage: ./pentomino <board.txt>" << std::endl;
        return 0;
    }
    std::string filename = argv[1];

    // Pentomino::VisualisePiece();
    auto board = Pentomino::ReadBoard(filename);
    Pentomino::VisualiseBoard(board);

    Pentomino::SolveBoard(board);

    std::cout << "--x--" << std::endl;
    return 0;
}