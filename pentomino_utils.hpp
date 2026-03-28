#include <unordered_map>
#include <array>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <functional>
#include <chrono>

namespace Pentomino{

using RelativeCoord = std::pair<std::size_t, std::size_t>;
constexpr int BOX_SIZE = 100;

std::unordered_map<std::string, std::vector<RelativeCoord>> CollisionLookup = {
    // Piece F
    {"F0",  {{1,0}, {-1,1}, {0,1}, {0,2}} },
    {"F1",  {{-1,1}, {0,1}, {1,1}, {1,2}} },
    {"F2",  {{0,1}, {1,1}, {-1,2}, {0,2}} },
    {"F3",  {{0,1}, {1,1}, {2,1}, {1,2}} },
    {"F0'", {{1,0}, {1,1}, {2,1}, {1,2}} },
    {"F1'", {{-1,1}, {0,1}, {1,1}, {-1,2}} },
    {"F2'", {{-1,1}, {0,1}, {0,2}, {1,2}} },
    {"F3'", {{-2,1}, {-1,1}, {0,1}, {-1,2}} },
    // Piece I
    {"I0",  {{1,0}, {2,0}, {3,0}, {4,0}} },
    {"I1",  {{0,1}, {0,2}, {0,3}, {0,4}} },
    // Piece L
    {"L0",  {{0,1}, {0,2}, {0,3}, {1,3}} },
    {"L1",  {{1,0}, {2,0}, {3,0}, {0,1}} },
    {"L2",  {{1,0}, {1,1}, {1,2}, {1,3}} },
    {"L3",  {{-3,1}, {-2,1}, {-1,1}, {0,1}} },
    {"L0'", {{0,1}, {0,2}, {-1,3}, {0,3}} },
    {"L1'", {{1,0}, {2,0}, {3,0}, {3,1}} },
    {"L2'", {{1,0}, {0,1}, {0,2}, {0,3}} },
    {"L3'", {{0,1}, {1,1}, {2,1}, {3,1}} },
    // Piece N
    {"N0",  {{1,0}, {1,1}, {2,1}, {3,1}} },
    {"N1",  {{0,1}, {-1,1}, {-1,2}, {-1,3}} },
    {"N2",  {{1,0}, {2,0}, {2,1}, {3,1}} },
    {"N3",  {{0,1}, {-1,2}, {0,2}, {-1,3}} },
    {"N0'", {{1,0}, {-2,1}, {-1,1}, {0,1}} },
    {"N1'", {{0,1}, {1,1}, {1,2}, {1,3}} },
    {"N2'", {{1,0}, {2,0}, {-1,1}, {0,1}} },
    {"N3'", {{0,1}, {0,2}, {1,2}, {1,3}} },
    // Piece P
    {"P0",  {{1,0}, {0,1}, {1,1}, {0,2}} },
    {"P1",  {{1,0}, {2,0}, {1,1}, {2,1}} },
    {"P2",  {{-1,1}, {0,1}, {-1,2}, {0,2}} },
    {"P3",  {{1,0}, {0,1}, {1,1}, {2,1}} },
    {"P0'", {{1,0}, {0,1}, {1,1}, {1,2}} },
    {"P1'", {{1,0}, {2,0}, {0,1}, {1,1}} },
    {"P2'", {{0,1}, {1,1}, {0,2}, {1,2}} },
    {"P3'", {{1,0}, {-1,1}, {0,1}, {1,1}} },
    // Piece T
    {"T0",  {{1,0}, {2,0}, {1,1}, {1,2}} },
    {"T1",  {{-2,1}, {-1,1}, {0,1}, {0,2}} },
    {"T2",  {{0,1}, {-1,2}, {0,2}, {1,2}} },
    {"T3",  {{0,1}, {1,1}, {2,1}, {0,2}} },
    // Piece U
    {"U0",  {{2,0}, {0,1}, {1,1}, {2,1}} },
    {"U1",  {{1,0}, {0,1}, {0,2}, {1,2}} },
    {"U2",  {{1,0}, {2,0}, {0,1}, {2,1}} },
    {"U3",  {{1,0}, {1,1}, {0,2}, {1,2}} },
    // Piece V
    {"V0",  {{0,1}, {0,2}, {1,2}, {2,2}} },
    {"V1",  {{0,1}, {-2,2}, {-1,2}, {0,2}} },
    {"V2",  {{1,0}, {2,0}, {2,1}, {2,2}} },
    {"V3",  {{1,0}, {2,0}, {0,1}, {0,2}} },
    // Piece W
    {"W0",  {{0,1}, {1,1}, {1,2}, {2,2}} },
    {"W1",  {{-1,1}, {0,1}, {-2,2}, {-1,2}} },
    {"W2",  {{1,0}, {1,1}, {2,1}, {2,2}} },
    {"W3",  {{1,0}, {-1,1}, {0,1}, {-1,2}} },
    // Piece X
    {"X0",  {{-1,1}, {0,1}, {1,1}, {0,2}} },
    // Piece Y
    {"Y0",  {{-2,1}, {-1,1}, {0,1}, {1,1}} },
    {"Y1",  {{-1,1}, {0,1}, {0,2}, {0,3}} },
    {"Y2",  {{1,0}, {2,0}, {3,0}, {1,1}} },
    {"Y3",  {{0,1}, {0,2}, {1,2}, {0,3}} },
    {"Y0'", {{-1,1}, {0,1}, {1,1}, {2,1}} },
    {"Y1'", {{0,1}, {1,1}, {0,2}, {0,3}} },
    {"Y2'", {{1,0}, {2,0}, {3,0}, {2,1}} },
    {"Y3'", {{0,1}, {-1,2}, {0,2}, {0,3}} },
    // Piece Z
    {"Z0",  {{1,0}, {1,1}, {1,2}, {2,2}} },
    {"Z1",  {{-2,1}, {-1,1}, {0,1}, {-2,2}} },
    {"Z0'",  {{1,0}, {0,1}, {-1,2}, {0,2}} },
    {"Z1'",  {{0,1}, {1,1}, {2,1}, {2,2}} }
};

std::unordered_map<char, std::vector<std::string>> PiecePermutationLookup = {
    {'F', {"F0", "F1", "F2", "F3", "F0'", "F1'", "F2'", "F3'"}},
    {'I', {"I0", "I1"}},
    {'L', {"L0", "L1", "L2", "L3", "L0'", "L1'", "L2'", "L3'"}},
    {'N', {"N0", "N1", "N2", "N3", "N0'", "N1'", "N2'", "N3'"}},
    {'P', {"P0", "P1", "P2", "P3", "P0'", "P1'", "P2'", "P3'"}},
    {'T', {"T0", "T1", "T2", "T3"}},
    {'U', {"U0", "U1", "U2", "U3"}},
    {'V', {"V0", "V1", "V2", "V3"}},
    {'W', {"W0", "W1", "W2", "W3"}},
    {'X', {"X0"}},
    {'Y', {"Y0", "Y1", "Y2", "Y3", "Y0'", "Y1'", "Y2'", "Y3'"}},
    {'Z', {"Z0", "Z1", "Z0'", "Z1'"}}
};

std::array<char, 12> Piece = {'F', 'I', 'L', 'N', 'P', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

inline void VisualisePiece(){
    cv::Point centre(300,300);
    cv::Mat buf(700, 700, CV_8UC3);
    
    for(const char piece : Pentomino::Piece){
        for(const auto permutation : Pentomino::PiecePermutationLookup[piece]){
            buf.setTo(cv::Scalar(0, 0, 0));
            cv::rectangle(buf, centre, cv::Point(centre.x+BOX_SIZE, centre.y+BOX_SIZE), cv::Scalar(255, 255, 255), -1);
            for(const auto pos : Pentomino::CollisionLookup[permutation]){
                cv::rectangle(buf, cv::Point(centre.x+pos.first*BOX_SIZE, centre.y+pos.second*BOX_SIZE), 
                                   cv::Point(centre.x+pos.first*BOX_SIZE+BOX_SIZE, centre.y+pos.second*BOX_SIZE+BOX_SIZE), cv::Scalar(255, 255, 255), -1);
            }
            cv::putText(buf, permutation, cv::Point(20,100), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(0,0,255), 2, cv::LINE_AA);
            cv::imshow("Preview", buf);
            cv::waitKey(0);
        }
    }
}

inline cv::Mat ReadBoard(){
    std::string line, buffer="";
    std::ifstream board_file("../Board.txt");
    std::size_t width;

    if (board_file.is_open()) {
        // First line to get length
        std::getline(board_file, line);
        width = line.length();
        buffer = line;
        while (std::getline(board_file, line)) {
            buffer = buffer + line;
        }
        board_file.close(); 
    }else{
        std::cout << "Could not read file" << std::endl;
        return cv::Mat();
    }

    if(buffer.length() % width != 0){
        std::cout << "Input is not rectangular" << std::endl;
        return cv::Mat();
    }

    std::size_t height = buffer.length() / width;
    cv::Mat boardOut(height, width, CV_8UC1);
    std::size_t buf_counter = 0;
    int space_counter = 0;
    for(int row=0; row<height; row++){
        for(int col=0; col<width; col++){
            if(buffer.at(buf_counter++) == 'O'){
                boardOut.at<uchar>(row, col) = 0;
                space_counter++;
            }else{
                boardOut.at<uchar>(row, col) = 1;
            }
        }    
    }
    if(space_counter%5 != 0){
        std::cout << "Board is invalid size!" << std::endl;
        return cv::Mat();
    }
    return boardOut;
}

inline bool IsFree(const cv::Mat& boardIn, const int row, const int col){
    if(row >= 0 && row < boardIn.rows && col >=0 && col < boardIn.cols){
        return boardIn.at<uchar>(row, col) == 0;
    }
    return false;
}

inline void VisualiseBoard(const cv::Mat& boardIn){
    std::size_t brow = boardIn.rows, bcol = boardIn.cols;
    cv::Mat vis_board(brow*BOX_SIZE, bcol*BOX_SIZE, CV_8UC3, cv::Scalar(0,0,0));
    for(int row=0; row<brow; row++){
        for(int col=0; col<bcol; col++){
            if(IsFree(boardIn, row, col)){
                cv::rectangle(vis_board, cv::Point(col*BOX_SIZE, row*BOX_SIZE), 
                                         cv::Point((col+1)*BOX_SIZE, (row+1)*BOX_SIZE), cv::Scalar(255, 255, 255), -1);
            }
        }    
    }
    cv::imshow("Board", vis_board);
    cv::waitKey(0);
}

// For Visualising Solution
std::array<cv::Scalar, 12> ColourCode = { cv::Scalar(225,30,209), cv::Scalar(30,112,225), cv::Scalar(30,225,46), cv::Scalar(30,143,225),
                                          cv::Scalar(225,30,63), cv::Scalar(95,30,225), cv::Scalar(30,225,192), cv::Scalar(160,225,30),
                                          cv::Scalar(134,80,97), cv::Scalar(90,80,134), cv::Scalar(80,134,117), cv::Scalar(124,134,80) };
cv::Mat gSolutionBoard;
inline void InitSolutionBoard(const cv::Mat& boardIn){
    std::size_t brow = boardIn.rows, bcol = boardIn.cols;
    gSolutionBoard = cv::Mat(brow*BOX_SIZE, bcol*BOX_SIZE, CV_8UC3, cv::Scalar(0,0,0));
}

inline void DrawPieceOnSolution(const int piece_id, const std::string& permutation, const int row, const int col){
    const auto& piece_meta = CollisionLookup[permutation];
    cv::rectangle(gSolutionBoard, cv::Point(col*BOX_SIZE, row*BOX_SIZE)                                             , cv::Point((col+1)*BOX_SIZE                     , (row+1)*BOX_SIZE)                    , ColourCode[piece_id], -1);
    cv::rectangle(gSolutionBoard, cv::Point((col+piece_meta[0].first)*BOX_SIZE, (row+piece_meta[0].second)*BOX_SIZE), cv::Point((col+piece_meta[0].first+1)*BOX_SIZE, (row+piece_meta[0].second+1)*BOX_SIZE), ColourCode[piece_id], -1);
    cv::rectangle(gSolutionBoard, cv::Point((col+piece_meta[1].first)*BOX_SIZE, (row+piece_meta[1].second)*BOX_SIZE), cv::Point((col+piece_meta[1].first+1)*BOX_SIZE, (row+piece_meta[1].second+1)*BOX_SIZE), ColourCode[piece_id], -1);
    cv::rectangle(gSolutionBoard, cv::Point((col+piece_meta[2].first)*BOX_SIZE, (row+piece_meta[2].second)*BOX_SIZE), cv::Point((col+piece_meta[2].first+1)*BOX_SIZE, (row+piece_meta[2].second+1)*BOX_SIZE), ColourCode[piece_id], -1);
    cv::rectangle(gSolutionBoard, cv::Point((col+piece_meta[3].first)*BOX_SIZE, (row+piece_meta[3].second)*BOX_SIZE), cv::Point((col+piece_meta[3].first+1)*BOX_SIZE, (row+piece_meta[3].second+1)*BOX_SIZE), ColourCode[piece_id], -1);
}

inline void ShowSolutionBoard(){
    cv::imshow("Board", gSolutionBoard);
    cv::waitKey(0);
}

// Return false if doesnt fit
inline bool InsertIfFits(cv::Mat& boardIn, const std::string& piece, const int row, const int col){
    if(IsFree(boardIn, row, col)){
        auto iter = CollisionLookup.find(piece);
        if(iter != CollisionLookup.end()){
            std::vector<RelativeCoord>& piece_meta = iter->second;
            if( IsFree(boardIn, row+piece_meta[0].second, col+piece_meta[0].first) &&
                IsFree(boardIn, row+piece_meta[1].second, col+piece_meta[1].first) &&
                IsFree(boardIn, row+piece_meta[2].second, col+piece_meta[2].first) &&
                IsFree(boardIn, row+piece_meta[3].second, col+piece_meta[3].first) )
            {
                boardIn.at<uchar>(row, col) = 1;
                boardIn.at<uchar>(row+piece_meta[0].second, col+piece_meta[0].first) = 1;
                boardIn.at<uchar>(row+piece_meta[1].second, col+piece_meta[1].first) = 1;
                boardIn.at<uchar>(row+piece_meta[2].second, col+piece_meta[2].first) = 1;
                boardIn.at<uchar>(row+piece_meta[3].second, col+piece_meta[3].first) = 1;
                return true;
            }
        }
    }
    return false;
}

inline int ConnectedCount(cv::Mat& partialIn, int row, int col){
    if(IsFree(partialIn, row, col)){
        partialIn.at<uchar>(row, col) = 1;
        return  ConnectedCount(partialIn, row+1, col) +
                ConnectedCount(partialIn, row-1, col) +
                ConnectedCount(partialIn, row, col+1) +
                ConnectedCount(partialIn, row, col-1) + 1; // Myself
    }
    return 0;
}

inline bool SegmentsAreFive(const cv::Mat& partialIn){ // Copy input so i can use as BFS buffer
    cv::Mat buffer = partialIn.clone();
    int brow = partialIn.rows, bcol = partialIn.cols; 
    for(int row=0; row<brow; row++){
        for(int col=0; col<bcol; col++){
            if(IsFree(buffer, row, col)){
                int segment_count = ConnectedCount(buffer, row, col);
                if(segment_count%5 != 0){
                    return false;
                }
            }
        }    
    }
    return true;
}

inline bool FitPiece(const cv::Mat& partialIn, const int piece_id){
    if(piece_id >= 12){ return true; } // Solution found!

    cv::Mat currentBoard = partialIn.clone();
    int brow = partialIn.rows, bcol = partialIn.cols;
    char piece_alphabet = Piece[piece_id];

    for(const auto& permutation : PiecePermutationLookup[piece_alphabet]){
        for(int row=0; row<brow; row++){
            for(int col=0; col<bcol; col++){
                if(InsertIfFits(currentBoard, permutation, row, col)){
                    // VisualiseBoard(currentBoard);
                    // Piece inserted. Now need check if we isolated invalid regions
                    if(SegmentsAreFive(currentBoard)){
                        if(FitPiece(currentBoard, piece_id+1)){ // If this perm find a solution, then solution is found
                            DrawPieceOnSolution(piece_id, permutation, row, col);
                            return true;
                        }
                    }
                    partialIn.copyTo(currentBoard); // Revert the board
                }
            }    
        }
    }    
    return false; // Finish all combination. No solution    
}

inline void SolveBoard(const cv::Mat& boardIn){
    InitSolutionBoard(boardIn);
    std::cout << "Starting Solve..." << std::endl;
    auto start = std::chrono::steady_clock::now();
    // Just try the pieces alphabetically lol
    if(FitPiece(boardIn, 0)){
        std::int64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
        float seconds = static_cast<float>(ms)/1000.0f;
        std::cout << "A Solution was found in " << seconds << " s" << std::endl;
        ShowSolutionBoard();
    }else{
        std::cout << "No Solution" << std::endl;
    }
}

}



