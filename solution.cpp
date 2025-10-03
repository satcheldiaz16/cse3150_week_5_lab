#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

// Forward declaration
int compute_score(const std::vector<std::vector<int>>& board);

#include <iterator>

// TODO: Compress a row: remove zeros, pad with zeros at the end
std::vector<int> compress_row(const std::vector<int>& row) {
    // TODO: Use copy_if to filter non-zero values, then pad with zeros
	std::vector<int> compressed;
	std::copy_if(row.begin(), row.end(), std::back_inserter(compressed), [](int v) {return v!=0;});
	while(compressed.size() < row.size()){
		compressed.push_back(0);
	}
//	std::cout << "original" << row[0] << row[1] << row[2] << row[3];
//	std::cout << "compressed" <<  compressed[0] << compressed[1] << compressed[2] << compressed[3];
//	std::cout << std::endl;
    return compressed;
}

// TODO: Merge a row (assumes already compressed)
std::vector<int> merge_row(std::vector<int> row) {
    // TODO: Implement merging logic - combine adjacent equal tiles
    	auto at = row.begin();
	auto next = row.begin();
	next++;
	while(next!=row.end()){
//		std::cout << *at << " | " << *next << std::endl;
		if(*at==*next){
			*at *= 2;
			*next = 0;
		}
		at++;
		next++;
	}
    return compress_row(row);
}




void write_board_csv(const vector<vector<int>>& board, bool first, const string& stage) {
    ios_base::openmode mode = ios::app;
    if (first) mode = ios::trunc;
    ofstream fout("game_output.csv", mode);
    if (!fout) return;

    // Write stage identifier
    fout << stage << ",";

    // Write board data
    for (int r=0;r<4;r++){
        for (int c=0;c<4;c++){
            fout<<board[r][c];
            if (!(r==3 && c==3)) fout<<",";
        }
    }
    fout<<"\n";
}

void read_board_csv(vector<vector<int>>& board) {
    ifstream fin("game_input.csv");

    string line;
    int r = 0;
    while (getline(fin, line) && r < 4) {
        stringstream ss(line);
        string cell;
        int c = 0;
        while (getline(ss, cell, ',') && c < 4) {
            try {
                board[r][c] = stoi(cell);
            } catch (...) {
                board[r][c] = 0;  // Default to 0 for invalid input
            }
            c++;
        }
        r++;
    }
}

void print_board(const vector<vector<int>>& board) {
    // TODO: Print the score using compute_score(board)
    // TODO: Print the board in a 4x4 format
    // TODO: Use dots (.) for empty cells (value 0)
    // TODO: Use tabs (\t) to separate values for alignment
	std::cout << "Score: " << compute_score(board) << std::endl;
	for(auto& r : board){
		for(auto& c : r){
			if(c==0){
				std::cout << ".";
			}
			else{
				std::cout << c;
			}
			std::cout << "\t";
		}
		std::cout << std::endl;
	}
}

void spawn_tile(std::vector<std::vector<int>>& board) {
    std::vector<std::pair<int,int>> empty;
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            if (board[r][c] == 0) empty.emplace_back(r,c);

    if (empty.empty()) return;

    static std::mt19937 gen(42);  // Fixed seed for deterministic behavior
    std::uniform_int_distribution<> pos_dist(0, empty.size()-1);
    std::uniform_int_distribution<> val_dist(1, 10);

    auto [r, c] = empty[pos_dist(gen)];
    board[r][c] = (val_dist(gen) == 1 ? 4 : 2); // 10% chance of 4
}

// TODO: Implement move_left using compress_row and merge_row
bool move_left(std::vector<std::vector<int>>& board) {
    bool moved = false;
    // TODO: For each row:
    //   1. Compress the row (remove zeros)
    //   2. Merge adjacent equal tiles
    //   3. Check if the row changed
    	for(auto& r : board){
		std::vector<int> temp = compress_row(r);
		temp = merge_row(temp);
		for(int i = 0; i < 4; i++){
//			std::cout << "new: " << temp[i] << " old: " << r[i] << std::endl;
			if(temp[i]!=r[i]) {moved=true;}
		}
		r = temp;
	}
//	std::cout << moved << std::endl;
    return moved;
}

// TODO: Implement move_right (hint: reverse, compress, merge, reverse)
bool move_right(std::vector<std::vector<int>>& board) {
    bool moved = false;
    // TODO: Similar to move_left but with reversal
    for(auto& r : board){
    	std::reverse(r.begin(), r.end());
    }
    for(auto& r : board){
    	std::vector<int> temp = compress_row(r);
	temp = merge_row(temp);
	for(int i = 0; i < 4; i++){
		if(temp[i]!=r[i]) {moved=true;}
	}
	r = temp;
    }
    for(auto& r : board){
    	std::reverse(r.begin(), r.end());
    }
    return moved;
}

// TODO: Implement move_up (work with columns)
bool move_up(std::vector<std::vector<int>>& board) {
    bool moved = false;
    std::vector<int> column;
    std::vector<int> temp;
    for(int i = 0; i < 4; i++){
    	for(int j = 0; j < 4; j++){
		if(column.size() < 4){
			column.push_back(board[j][i]);
		}
		else{
			column[j] = board[j][i];
		}
	}
	temp = compress_row(column);
	temp = merge_row(temp);
	for(int k = 0; k < 4; k++){
		if(temp[k]!=column[k]) {moved=true;}
	}
	column = temp;
	for(int j = 0; j < 4; j++){
		board[j][i] = column[j];
	}
    }
    // TODO: Extract column, compress, merge, write back
    return moved;
}

// TODO: Implement move_down (columns with reversal)
bool move_down(std::vector<std::vector<int>>& board) {
    bool moved = false;
    std::vector<int> column;
    std::vector<int> temp;
    for(int i = 0; i < 4; i++){
    	for(int j = 0; j < 4; j++){
		if(column.size() < 4){
			column.push_back(board[j][i]);
		}
		else{
			column[j] = board[j][i];
		}
	}
	std::reverse(column.begin(), column.end());
	temp = compress_row(column);
	temp = merge_row(temp);
	for(int k = 0; k < 4; k++){
		if(temp[k]!=column[k]) {moved=true;}
	}
	column = temp;
	std::reverse(column.begin(), column.end());
	for(int j = 0; j < 4; j++){
		board[j][i] = column[j];
	}
    }
    // TODO: Similar to move_up but with reversal
    return moved;
}



int compute_score(const std::vector<std::vector<int>>& board) {
    int score = 0;
    for (const auto& row : board)
        for (int val : row)
            score += val;
    return score;
}


int main(){
    vector<vector<int>> board(4, vector<int>(4,0));

    // Read initial board from CSV
    read_board_csv(board);

    stack<vector<vector<int>>> history;
    bool first=true;

    while(true){
        print_board(board);
        if (first) {
            write_board_csv(board, true, "initial");
            first = false;
        }

        cout<<"Move (w=up, a=left, s=down, d=right), u=undo, q=quit: ";
        char cmd;
        if (!(cin>>cmd)) break;
        if (cmd=='q') break;

        if (cmd=='u') {
            // TODO: Check if history stack is not empty using !history.empty()
            // If not empty:
            //   1. Set board = history.top() to restore the previous state
            //   2. Remove the top element with history.pop()
            //   3. Call print_board(board) to show the restored board
            //   4. Call write_board_csv(board, false, "undo") to log the undo
            // Use 'continue' to skip the rest of the loop iteration
	    if(!history.empty()){
	    	board = history.top();
		history.pop();
		print_board(board);
		write_board_csv(board, false, "undo");
	    }
	    continue;
        }

        vector<vector<int>> prev = board;
        bool moved=false;
        if (cmd=='a') moved=move_left(board);
        else if (cmd=='d') moved=move_right(board);
        else if (cmd=='w') moved=move_up(board);
        else if (cmd=='s') moved=move_down(board);

        if (moved) {
            // TODO: Push the previous board state to history stack
            // Use: history.push(prev)
		history.push(prev);
            // Write board after merge but before spawn
            write_board_csv(board, false, "merge");

            spawn_tile(board);
            // Write board after spawn
            write_board_csv(board, false, "spawn");
        } else {
            // No move was made
            write_board_csv(board, false, "invalid");
        }
    }
    return 0;
}
