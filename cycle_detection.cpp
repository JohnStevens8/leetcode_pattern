 1. House Robber in Circular Array (Corrected)

Actually, let me provide a simpler, more realistic version:

// Problem: Rob houses in circular array, but with state transitions
class CircularRobberSimple {
public:
    int maxMoneyAfterRounds(vector<int>& houses, int rounds) {
        int n = houses.size();
        if (n == 0 || rounds == 0) return 0;
        
        // State: current position in array
        map<int, pair<int, int>> seen; // pos -> {round, money}
        
        int pos = 0, totalMoney = 0;
        
        for (int round = 1; round <= rounds; round++) {
            // Simple strategy: rob every other house in one round
            int roundMoney = 0;
            for (int i = 0; i < n; i += 2) {
                roundMoney += houses[(pos + i) % n];
            }
            pos = (pos + 1) % n; // Move starting position
            totalMoney += roundMoney;
            
            if (seen.count(pos)) {
                auto [startRound, startMoney] = seen[pos];
                int cycleLen = round - startRound;
                int cycleGain = totalMoney - startMoney;
                
                int remaining = rounds - round;
                int fullCycles = remaining / cycleLen;
                
                return totalMoney + fullCycles * cycleGain;
            }
            
            seen[pos] = {round, totalMoney};
        }
        
        return totalMoney;
    }
};

🔧 2. Matrix Power A^N (Corrected)

class MatrixPower {
private:
    vector<vector<long long>> multiply(const vector<vector<long long>>& A, 
                                      const vector<vector<long long>>& B, int MOD) {
        int n = A.size();
        vector<vector<long long>> C(n, vector<long long>(n, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
                }
            }
        }
        return C;
    }
    
    string matrixToString(const vector<vector<long long>>& mat) {
        string result;
        for (const auto& row : mat) {
            for (long long val : row) {
                result += to_string(val) + ",";
            }
            result += ";";
        }
        return result;
    }
    
public:
    vector<vector<long long>> matrixPower(vector<vector<long long>>& A, long long N, int MOD) {
        if (N == 0) {
            int n = A.size();
            vector<vector<long long>> I(n, vector<long long>(n, 0));
            for (int i = 0; i < n; i++) I[i][i] = 1;
            return I;
        }
        if (N == 1) return A;
        
        // Use string representation for state
        map<string, pair<long long, vector<vector<long long>>>> seen;
        
        auto current = A;
        for (long long power = 1; power < N; power++) {
            string state = matrixToString(current);
            
            if (seen.count(state)) {
                auto [startPower, startMatrix] = seen[state];
                long long cycleLen = power - startPower;
                
                if (cycleLen > 0) {
                    long long remaining = N - power;
                    long long pos = startPower + (remaining % cycleLen);
                    
                    // Find matrix at position pos
                    for (auto& [key, value] : seen) {
                        if (value.first == pos) {
                            return value.second;
                        }
                    }
                }
            }
            
            seen[state] = {power, current};
            current = multiply(current, A, MOD);
        }
        
        return current;
    }
};

🔧 3. Digital Root Sequence (Corrected)

class DigitalRootSequence {
private:
    int getDigitalRoot(long long n) {
        if (n == 0) return 0;
        return 1 + (n - 1) % 9;
    }
    
public:
    int nthDigitalRoot(long long n) {
        // Digital root sequence: 1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,...
        // This has obvious cycle of length 9 starting from position 1
        
        if (n <= 0) return 0;
        
        // Direct calculation - no cycle detection needed!
        return 1 + (n - 1) % 9;
        
        // But if we want to demonstrate cycle detection:
        /*
        map<int, long long> seen;
        
        for (long long i = 1; i <= n; i++) {
            int digitalRoot = getDigitalRoot(i);
            
            if (seen.count(digitalRoot) && i > 9) {
                long long cycleStart = seen[digitalRoot];
                long long cycleLen = i - cycleStart;
                
                if (n > cycleStart) {
                    long long pos = cycleStart + (n - cycleStart - 1) % cycleLen;
                    return getDigitalRoot(pos);
                }
            }
            
            if (i <= 9) seen[digitalRoot] = i;
        }
        
        return getDigitalRoot(n);
        */
    }
};

🔧 4. Conway's Game of Life (Corrected)

class GameOfLife {
private:
    string boardToString(const vector<vector<int>>& board) {
        string result;
        for (const auto& row : board) {
            for (int cell : row) {
                result += to_string(cell);
            }
        }
        return result;
    }
    
    vector<vector<int>> nextGeneration(const vector<vector<int>>& board) {
        int m = board.size(), n = board[0].size();
        vector<vector<int>> next(m, vector<int>(n, 0));
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                int live = 0;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        if (di == 0 && dj == 0) continue;
                        int ni = i + di, nj = j + dj;
                        if (ni >= 0 && ni < m && nj >= 0 && nj < n) {
                            live += board[ni][nj];
                        }
                    }
                }
                
                if (board[i][j] == 1) {
                    next[i][j] = (live == 2 || live == 3) ? 1 : 0;
                } else {
                    next[i][j] = (live == 3) ? 1 : 0;
                }
            }
        }
        return next;
    }
    
public:
    vector<vector<int>> simulate(vector<vector<int>>& board, int N) {
        if (N == 0) return board;
        
        map<string, pair<int, vector<vector<int>>>> seen;
        auto current = board;
        
        for (int gen = 0; gen < N; gen++) {
            string state = boardToString(current);
            
            if (seen.count(state)) {
                auto [startGen, startBoard] = seen[state];
                int cycleLen = gen - startGen;
                
                if (cycleLen == 0) {
                    // Stable state
                    return current;
                } else {
                    // Periodic cycle
                    int remaining = N - gen;
                    int pos = startGen + (remaining % cycleLen);
                    
                    // Find board at position pos
                    for (auto& [key, value] : seen) {
                        if (value.first == pos) {
                            return value.second;
                        }
                    }
                }
            }
            
            seen[state] = {gen, current};
            current = nextGeneration(current);
        }
        
        return current;
    }
};

🔧 5. Collatz Sequence Length (Corrected)

class CollatzSequence {
private:
    map<long long, long long> memo;
    
public:
    long long collatzLength(long long n) {
        if (n == 1) return 0;
        if (memo.count(n)) return memo[n];
        
        long long original = n;
        long long steps = 0;
        
        // Follow sequence until we hit a memoized value or 1
        while (n != 1 && !memo.count(n)) {
            if (n % 2 == 0) {
                n /= 2;
            } else {
                n = 3 * n + 1;
            }
            steps++;
            
            // Overflow protection
            if (n > 1e15) return -1;
        }
        
        long long result = steps + (n == 1 ? 0 : memo[n]);
        memo[original] = result;
        return result;
    }
};

🔧 6. Josephus Problem Variant (Corrected)

class JosephusCycle {
public:
    int lastSurvivor(int N, int K, int rounds) {
        // Simple version: eliminate K-th person in each round
        vector<int> people;
        for (int i = 0; i < N; i++) people.push_back(i);
        
        int pos = 0;
        
        for (int round = 0; round < rounds && people.size() > 1; round++) {
            // Find K-th person from current position
            pos = (pos + K - 1) % people.size();
            people.erase(people.begin() + pos);
            
            // Adjust position after removal
            if (pos >= people.size()) pos = 0;
        }
        
        return people.empty() ? -1 : people[0];
    }
};
