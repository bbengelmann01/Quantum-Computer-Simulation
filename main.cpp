#include <bits/stdc++.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

long probsize = 30000;
int OO = 0;
int OI = 0;
int IO = 0;
int II = 0;
int O = 0;
int I = 0;
int timesrun = 2000;

class Qubit {
    public:
        /*The state of a qubit can be represented by a vector.
        The first number represents the probability that, 
        when measured, the qubit will have a state of 0.
        The second number represents the probability that, 
        when measured, the qubit will have a state of 1.
        */
        Qubit(vector<int> state) {
            state_ = state;
            numofhadamards_ = 0;
        };
       
        vector<int> state_;

        int numofhadamards_;
       
        //This overloads the multiplication operation to allow for matrix and vector multiplication

        Qubit operator*(const vector<vector<int>> matrix) {
            vector<int> tempstate;
            for (int i = 0; i < matrix.size(); i++) {
                tempstate.emplace_back(0);
            }
            Qubit q(tempstate);
            if (matrix.size() != state_.size()) {
                return q;
            }
            for (int i = 0; i < matrix.size(); i++) {
                for (int j = 0; j < matrix.size(); j++) {
                    //cout << state_[j] << " * " << matrix[i][j] << endl;
                    q.state_[i] += state_[j] * matrix[i][j];
                }
            }
            return q;
        };
       
        void Hadamard();
        void PauliX();
        void PauliZ();
        void ControlledZ();
        void HadamardTransform();
        void PauliZTransform();
        void Reset();
        string Measure(bool outputprobs);
};

//This is used for combining the states of multiple qubits into one state
Qubit TensorProduct(Qubit a, Qubit b) {
    Qubit product({});
    for (int i = 0; i < a.state_.size(); i++) {
        for (int j = 0; j < b.state_.size(); j++) {
            product.state_.emplace_back(a.state_[i] * b.state_[j]);
        }
    }
    product.numofhadamards_ = a.numofhadamards_ + b.numofhadamards_;
    return product;
};

void Qubit::Hadamard() {
    /*
    The matrix representation of the Hadamard gate is

    (1/sqrt(2))[1  1]
               [1 -1]
    If the Hadamard gate is performed on a qubit in a binary state, 
    it will be put into an equal superposition of the two binary states
    */
   vector<vector<int>> HadamardMatrix = {{1, 1},
                                        {1, -1}};
    Qubit q(state_);
    q = q * HadamardMatrix;
    state_ = q.state_;
    numofhadamards_ ++;
};

void Qubit::PauliX() {
    /*
    The Pauli-X gate serves to switch the two values in the vector representation of the qubit.
    */
    vector<vector<int>> PauliXMatrix = {{0, 1},
                                       {1, 0}};
    Qubit q(state_);
    q = q * PauliXMatrix;
    state_ = q.state_;
};

void Qubit::PauliZ() {
   vector<vector<int>> PauliZMatrix = {{1, 0},
                                      {0, -1}};
    Qubit q(state_);
    q = q * PauliZMatrix;
    state_ = q.state_;
};

void Qubit::ControlledZ() {
    /*
    The Controlled Z gate performes a Z gate on one of the qubits
    only if the value of the other qubit is 1
    
    It does not matter which qubit is which, as the result will be the 
    same both ways
    */
    vector<vector<int>> ControlledZMatrix =
    {{1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, -1}};

    Qubit q(state_);
    q = q * ControlledZMatrix;
    state_ = q.state_;
};

void Qubit::HadamardTransform() {
    /*
    The Hadamard Transform performes a Hadamard gate on each of the individual qubits
    in the tensor product of those qubits
    */
    vector<vector<int>> HadamardTransformMatrix =
    {{1, 1, 1, 1},
    {1, -1, 1, -1},
    {1, 1, -1, -1},
    {1, -1, -1, 1}};

    Qubit q(state_);
    q = q * HadamardTransformMatrix;
    state_ = q.state_;
    numofhadamards_ += 2;
};

void Qubit::PauliZTransform() {
    /*
    The Pauli-Z Transform performes a Pauli-Z gate on each of the individual qubits
    in the tensor product of those qubits
    */
    vector<vector<int>> PauliZTransformMatrix =
    {{1, 0, 0, 0},
    {0, -1, 0, 0},
    {0, 0, -1, 0},
    {0, 0, 0, 1}};

    Qubit q(state_);
    q = q * PauliZTransformMatrix;
    state_ = q.state_;
};

void Qubit::Reset() {

    //This resets the qubit to a state of 0

    state_ = {1, 0};
};

/*
This measures the state of a qubit

The "outputprobs" boolean determines if the probabilities of 
each binary state being measured will be printed to the console
*/

string Qubit::Measure(bool outputprobs) {
    long randlong = rand() % probsize;
    double hadamardcoefficient = 1.0;
    vector<double> probs = {};
   
    for (int i = 0; i < numofhadamards_; i++) {
        hadamardcoefficient *= 1/sqrt(2.0);
    }
   
    for (int i = 0; i < state_.size(); i++) {
        probs.emplace_back(static_cast<double>(state_[i]) * hadamardcoefficient);
        probs[i] = probs[i] * probs[i];
    }
    if (state_.size() == 2) {
        if (outputprobs == true) {
            cout << "0: " << probs[0] << endl;
            cout << "1: " << probs[1] << endl;
        }
        if (randlong <= probs[0] * probsize) {
            return "0";
        } else {
            return "1";
        }
    } else {
        if (outputprobs == true) {
            cout << "00: " << probs[0] << endl;
            cout << "01: " << probs[1] << endl;
            cout << "10: " << probs[2] << endl;
            cout << "11: " << probs[3] << endl;
        }
        if (randlong <= probs[0] * probsize) {
            return "00";
        } else if (randlong <= probs[1] * probsize) {
            return "01";
        } else if (randlong <= probs[2] * probsize) {
            return "10";
        } else {
            return "11";
        }
    }

    return "0";
};

/*
The "output" boolean controlls whether the probabilities of each individual measurement
will be printed to the console
*/

string GroversSearch(bool output) {
    string res;
    Qubit a({1, 0});
    Qubit b({1, 0});
    a.Hadamard();
    b.Hadamard();
    Qubit c = TensorProduct(a, b);
    c.ControlledZ();
    c.HadamardTransform();
    c.PauliZTransform();
    c.ControlledZ();
    c.HadamardTransform();
    res = c.Measure(output);
    if (res == "00") {
        OO++;
    } else if (res == "01") {
        OI++;
    } else if (res == "10") {
        IO++;
    } else if (res == "11") {
        II ++;
    } else if (res == "0") {
        I++;
    } else {
        O++;
    }
    return res;
}

/*
GroversSearchAlgorithm runs GroversSearch a specified number of times,
then outputs the results
*/

void GroversSearchAlgorithm(int iterations) {
    for (int i = 0; i < iterations; i++) {
        GroversSearch(true);
    }
    cout << endl;
    cout << "Number of times result was 00: " << OO << endl;
    cout << "Number of times result was 01: " << OI << endl;
    cout << "Number of times result was 10: " << IO << endl;
    cout << "Number of times result was 11: " << II << endl;
    cout << endl;
    cout << "Total number of times run: " << iterations << endl;
}

string DoubleH(bool output) {
    string res;
    Qubit a({1, 0});
    a.Hadamard();
    a.Hadamard();
    res = a.Measure(output);
    if (res == "0") {
        O++;
    } else {
        I++;
    }
    return res;
}

/*
DoubleHAlgorithm runs DoubleH a specified number of times,
then outputs the results
*/

void DoubleHAlgorithm(int iterations) {
    for (int i = 0; i < iterations; i++) {
        DoubleH(true);
    }
    cout << endl;
    cout << "Number of times result was 0: " << O << endl;
    cout << "Number of times result was 1: " << I << endl;
    cout << endl;
    cout << "Total number of times run: " << iterations << endl;
}

string QuadH(bool output) {
    string res;
    Qubit a({1, 0});
    Qubit b({1, 0});
    Qubit c = TensorProduct(a, b);
    c.HadamardTransform();
    c.HadamardTransform();
    res = c.Measure(output);
    if (res == "00") {
        OO++;
    } else if (res == "01") {
        OI++;
    } else if (res == "10") {
        IO++;
    } else if (res == "11") {
        II ++;
    } else if (res == "0") {
        I++;
    } else {
        O++;
    }
    return res;
}

/*
QuadHAlgorithm runs QuadH a specified number of times,
then outputs the results
*/

void QuadHAlgorithm(int iterations) {
    for (int i = 0; i < iterations; i++) {
    QuadH(true);
    }
    cout << endl;
    cout << "Number of times result was 00: " << OO << endl;
    cout << "Number of times result was 01: " << OI << endl;
    cout << "Number of times result was 10: " << IO << endl;
    cout << "Number of times result was 11: " << II << endl;
    cout << endl;
    cout << "Total number of times run: " << iterations << endl;
}

int main() {
    srand(time(NULL));
    return 0;
}
