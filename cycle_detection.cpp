Corrected Fibonacci Mod Code

class FibonacciMod {
public:
    long long fibMod(long long n, int m) {
        if (n <= 1) return n % m;
        
        // State: (F[i-1] % m, F[i] % m)
        map<pair<int,int>, int> seen;
        
        int prev = 0, curr = 1;
        vector<int> sequence = {0, 1};
        
        for (int i = 2; i <= n; i++) {
            int next = (prev + curr) % m;
            pair<int,int> state = {curr, next};
            
            if (seen.count(state)) {
                // Cycle detected!
                int cycleStart = seen[state];
                int cycleLen = i - cycleStart;
                
                long long pos = cycleStart + (n - cycleStart) % cycleLen;
                return sequence[pos];
            }
            
            seen[state] = i;
            sequence.push_back(next);
            prev = curr;
            curr = next;
        }
        
        return curr;
    }
};

Key corrections:

Removed redundant if (n >= cycleStart) check
Added % m to handle edge case when n = 1 and m is small
Cleaner logic flow


🎯 More Cycle Detection Problems

1. House Robber in Circular Array (Infinite Rounds)

// Problem: Rob houses in circular array for N rounds, maximize money
// State: (current_position, money_state_pattern)

class CircularRobber {
public:
    int maxMoney(vector<int>& houses, int rounds) {
        int n = houses.size();
        map<pair<int,int>, int> seen; // {pos, last_robbed} -> round
        vector<int> maxAtRound;
        
        int pos = 0, maxSoFar = 0;
        bool lastRobbed = false;
        
        for (int round = 0; round < rounds; round++) {
            // Process one complete round
            int roundMax = robOneRound(houses, pos, lastRobbed);
            maxSoFar += roundMax;
            
            pair<int,int> state = {pos, lastRobbed};
            if (seen.count(state)) {
                // Cycle detected - calculate remaining rounds
                int cycleStart = seen[state];
                int cycleLen = round - cycleStart;
                int cycleGain = maxSoFar - maxAtRound[cycleStart];
                
                int remaining = rounds - round - 1;
                int fullCycles = remaining / cycleLen;
                
                return maxSoFar + fullCycles * cycleGain + 
                       handleRemainder(remaining % cycleLen);
            }
            
            seen[state] = round;
            maxAtRound.push_back(maxSoFar);
        }
        return maxSoFar;
    }
};

2. Matrix Power (A^N)

// Problem: Compute A^N mod M efficiently using cycle detection
// State: Current matrix state

class MatrixPower {
public:
    vector<vector<int>> matrixPower(vector<vector<int>>& A, long long N, int MOD) {
        map<vector<vector<int>>, int> seen;
        vector<vector<vector<int>>> powers;
        
        auto current = A;
        powers.push_back(current);
        
        for (int i = 1; i < N; i++) {
            current = multiply(current, A, MOD);
            
            if (seen.count(current)) {
                // Cycle detected!
                int cycleStart = seen[current];
                int cycleLen = i - cycleStart;
                
                long long pos = cycleStart + (N - 1 - cycleStart) % cycleLen;
                return powers[pos];
            }
            
            seen[current] = i;
            powers.push_back(current);
        }
        
        return current;
    }
};

3. Digital Root Sequence

// Problem: Find the Nth term in digital root sequence
// Digital root: sum digits until single digit
// Sequence: 1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,...

class DigitalRootSequence {
public:
    int nthDigitalRoot(long long n) {
        // This has obvious cycle, but let's detect it programmatically
        map<int, int> seen;
        vector<int> sequence;
        
        for (int i = 1; i <= n; i++) {
            int digitalRoot = getDigitalRoot(i);
            
            if (seen.count(digitalRoot) && sequence.size() >= 9) {
                // Cycle detected (after we have enough data)
                int cycleStart = seen[digitalRoot];
                int cycleLen = i - cycleStart;
                
                if (cycleLen > 0 && n > cycleStart) {
                    long long pos = cycleStart + (n - cycleStart - 1) % cycleLen;
                    return sequence[pos];
                }
            }
            
            seen[digitalRoot] = i;
            sequence.push_back(digitalRoot);
        }
        
        return sequence.back();
    }
    
private:
    int getDigitalRoot(int n) {
        return n == 0 ? 0 : 1 + (n - 1) % 9;
    }
};

4. Conway's Game of Life (Stable State)

// Problem: Simulate Game of Life for N generations, detect cycles/stable states
// State: Current board configuration

class GameOfLife {
public:
    vector<vector<int>> simulate(vector<vector<int>>& board, int N) {
        map<vector<vector<int>>, int> seen;
        vector<vector<vector<int>>> history;
        
        auto current = board;
        
        for (int gen = 0; gen < N; gen++) {
            if (seen.count(current)) {
                // Cycle or stable state detected!
                int cycleStart = seen[current];
                int cycleLen = gen - cycleStart;
                
                if (cycleLen == 0) {
                    // Stable state - no more changes
                    return current;
                } else {
                    // Periodic cycle
                    int pos = cycleStart + (N - cycleStart) % cycleLen;
                    return history[pos];
                }
            }
            
            seen[current] = gen;
            history.push_back(current);
            current = nextGeneration(current);
        }
        
        return current;
    }
};

5. Collatz Sequence Length

// Problem: Find length of Collatz sequence for very large N
// State: Current number in sequence

class CollatzSequence {
public:
    long long collatzLength(long long n) {
        map<long long, long long> seen; // number -> steps_taken
        
        long long current = n;
        long long steps = 0;
        
        while (current != 1) {
            if (seen.count(current)) {
                // We've entered a previously computed path
                // But Collatz always goes to 1, so this is just optimization
                return steps + seen[current];
            }
            
            seen[current] = steps;
            
            if (current % 2 == 0) {
                current /= 2;
            } else {
                current = 3 * current + 1;
            }
            steps++;
            
            // Overflow protection
            if (current > 1e18) break;
        }
        
        return steps;
    }
};

6. Josephus Problem Variant

// Problem: N people in circle, eliminate every Kth person, repeat for M rounds
// State: (current_position, people_alive_pattern)

class JosephusCycle {
public:
    int lastSurvivor(int N, int K, int rounds) {
        map<pair<int, vector<bool>>, int> seen;
        
        vector<bool> alive(N, true);
        int pos = 0, aliveCount = N;
        
        for (int round = 0; round < rounds && aliveCount > 1; round++) {
            // Eliminate one person this round
            int steps = 0;
            while (steps < K) {
                if (alive[pos]) steps++;
                if (steps < K) pos = (pos + 1) % N;
            }
            
            alive[pos] = false;
            aliveCount--;
            
            pair<int, vector<bool>> state = {pos, alive};
            if (seen.count(state)) {
                // Pattern detected - calculate final result
                // ... cycle detection logic
            }
            
            seen[state] = round;
        }
        
        // Find last survivor
        for (int i = 0; i < N; i++) {
            if (alive[i]) return i;
        }
        return -1;
    }
};

🎯 Common Cycle Detection Pattern

template<typename State, typename Result>
class CycleDetector {
    map<State, int> seen;
    vector<Result> history;
    
public:
    Result solve(int maxSteps) {
        State current = getInitialState();
        
        for (int step = 0; step < maxSteps; step++) {
            if (seen.count(current)) {
                // CYCLE DETECTED!
                int cycleStart = seen[current];
                int cycleLen = step - cycleStart;
                
                int remaining = maxSteps - step;
                int fullCycles = remaining / cycleLen;
                int leftover = remaining % cycleLen;
                
                return calculateResult(cycleStart, cycleLen, 
                                     fullCycles, leftover);
            }
            
            seen[current] = step;
            history.push_back(getCurrentResult());
            current = getNextState(current);
        }
        
        return getCurrentResult();
    }
};
