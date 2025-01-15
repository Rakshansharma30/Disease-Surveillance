#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <limits>
using namespace std;
const vector<string> VALID_SYMPTOMS = {
    "fever", "diarrhea", "muscleweakness", "musclepain",
    "chills", "headache", "nausea", "pain",
    "thickenedskin", "hairthinning", "vomiting", "weightloss",
    "weakness", "anorexia", "lethargy"
};
string cleanSymptom(string symptom) {
    transform(symptom.begin(), symptom.end(), symptom.begin(), ::tolower);
    symptom.erase(remove_if(symptom.begin(), symptom.end(), ::isspace), symptom.end());
    return symptom;
}
double meanSquaredError(const vector<double>& y_true, const vector<double>& y_pred) {
    double error = 0.0;
    int n = y_true.size();
    for (int i = 0; i < n; i++) {
        error += pow(y_true[i] - y_pred[i], 2);
    }
    return error / n;
}
class LinearRegression {
public:
    vector<double> weights;
    double bias;
    double learning_rate;
    int num_iterations;

    LinearRegression(int num_features, double learning_rate = 0.01, int num_iterations = 1000) {
        weights.resize(num_features, 0.0); // Initialize weights to 0
        bias = 0.0; // Initialize bias to 0
        this->learning_rate = learning_rate;
        this->num_iterations = num_iterations;
    }

    // Train the model using Gradient Descent
    void train(const vector<vector<double>>& X, const vector<double>& y) {
        int m = X.size();
        int n = X[0].size();

        for (int iter = 0; iter < num_iterations; iter++) {
            vector<double> dw(n, 0.0);
            double db = 0.0;

            for (int i = 0; i < m; i++) {
                double y_pred = predictSingle(X[i]);
                double error = y_pred - y[i];

                for (int j = 0; j < n; j++) {
                    dw[j] += error * X[i][j];
                }
                db += error;
            }

            for (int j = 0; j < n; j++) {
                weights[j] -= learning_rate * dw[j] / m;
            }
            bias -= learning_rate * db / m;
        }
    }

    // Predict a single example
    double predictSingle(const vector<double>& x) {
        double result = bias;
        for (int i = 0; i < x.size(); i++) {
            result += weights[i] * x[i];
        }
        return result;
    }
};

// Function to display symptoms and get user input
vector<double> getUserSymptoms(const map<string, int>& symptom_map) {
    vector<double> encoded(symptom_map.size(), 0.0);

    cout << "\nEnter the symptoms and their severity levels (1: Mild, 2: Moderate, 3: Severe):" << endl;
    cout << "Available Symptoms: ";
    for (const auto& symptom : VALID_SYMPTOMS) {
        cout << symptom << ", ";
    }
    cout << "\n\nType 'done' when finished entering symptoms.\n" << endl;

    string symptom;
    int severity;
    while (true) {
        cout << "Enter symptom (or 'done'): ";
        cin >> symptom;

        if (cleanSymptom(symptom) == "done") break;

        symptom = cleanSymptom(symptom);
        if (symptom_map.find(symptom) == symptom_map.end()) {
            cout << "Invalid symptom! Please choose from the available symptoms.\n";
            continue;
        }

        cout << "Enter severity (1-3): ";
        while (!(cin >> severity) || severity < 1 || severity > 3) {
            cout << "Invalid input! Please enter a number between 1 and 3: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        encoded[symptom_map.at(symptom)] = static_cast<double>(severity) / 3.0; // Normalize severity
    }

    return encoded;
}

// Main function
int main() {
    // Map symptom names to indices for encoding
    map<string, int> symptom_map;
    for (int i = 0; i < VALID_SYMPTOMS.size(); i++) {
        symptom_map[VALID_SYMPTOMS[i]] = i;
    }

    // Training data: Symptoms (severity levels) and risk scores
    vector<vector<double>> X = {
        {1.0, 0.0, 0.0, 0.67, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, // Example: Mild fever, moderate muscle pain
        {0.33, 0.33, 0.67, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, // Moderate fever, mild diarrhea, mild muscle pain
        {0.67, 0.0, 0.33, 0.33, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}  // Severe fever, mild muscle pain
    };

    vector<double> y = {0.2, 0.5, 0.8}; // Risk scores for training

    // Initialize and train the model
    LinearRegression model(VALID_SYMPTOMS.size());
    model.train(X, y);

    // Get user symptoms
    vector<double> encoded_symptoms = getUserSymptoms(symptom_map);

    // Predict risk score
    double risk_score = model.predictSingle(encoded_symptoms);

    // Display the prediction
    cout << fixed << setprecision(2);
    cout << "\nPredicted Risk Score: " << risk_score * 100 << "%" << endl;

    if (risk_score >= 0.7) {
        cout << "Risk Level: High\n";
    } else if (risk_score >= 0.4) {
        cout << "Risk Level: Moderate\n";
    } else {
        cout << "Risk Level: Low\n";
    }

    return 0;
}
