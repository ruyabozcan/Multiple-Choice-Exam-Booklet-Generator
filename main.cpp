// RUYA BOZCAN
// 200201044
// CS 363
// Assignment 2


#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Question {
    int questionNumber;
    string questionText;
    string optionA;
    string optionB;
    string optionC;
    string optionD;
    char correctAnswer;
};

struct QuestionBank {
    vector<Question> questions;
};

void ReadQuestionsFromFile(QuestionBank& questionBank, const string& fileName) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        exit(EXIT_FAILURE);
    }

    Question question;
    string line;

    while (getline(file, line)) {
        if (line.find("Q") == 0) {
            question.questionNumber = stoi(line.substr(1));
            getline(file, question.questionText);
            getline(file, question.optionA);
            getline(file, question.optionB);
            getline(file, question.optionC);
            getline(file, question.optionD);
            getline(file, line);
            question.correctAnswer = line.back();

            questionBank.questions.push_back(question);
        }
    }

    file.close();
}

void ShuffleOptions(Question& question) {
    vector<string> options = {question.optionA, question.optionB, question.optionC, question.optionD};
    random_shuffle(options.begin(), options.end());

    question.optionA = options[0];
    question.optionB = options[1];
    question.optionC = options[2];
    question.optionD = options[3];
}

vector<Question> SelectRandomQuestions(const QuestionBank& questionBank, int numQuestions) {
    vector<Question> randomQuestions;

    vector<Question> shuffledQuestions = questionBank.questions;
    random_shuffle(shuffledQuestions.begin(), shuffledQuestions.end());

    for (int i = 0; i < numQuestions; ++i) {
        randomQuestions.push_back(shuffledQuestions[i]);
    }

    return randomQuestions;
}

void CreateBooklet(const QuestionBank& questionBank, int numBooklets, int numQuestions) {
    for (int i = 0; i < numBooklets; ++i) {
        vector<Question> bookletQuestions = SelectRandomQuestions(questionBank, numQuestions);

        // Shuffle options for each question
        for (Question& question : bookletQuestions) {
            ShuffleOptions(question);
        }

        // Create booklet files
        ofstream questionsFile("Booklet_" + to_string(i + 1) + "_Questions.txt");
        ofstream answersFile("Booklet_" + to_string(i + 1) + "_Answers.txt");

        if (!questionsFile.is_open() || !answersFile.is_open()) {
            cerr << "Failed to create files." << endl;
            exit(EXIT_FAILURE);
        }

        // Write questions to the questions file
        int currentQuestionNumber = 1;
        for (const Question& question : bookletQuestions) {
            questionsFile << "Q" << currentQuestionNumber << ": " << question.questionText << endl;
            questionsFile << question.optionA << endl;
            questionsFile << question.optionB << endl;
            questionsFile << question.optionC << endl;
            questionsFile << question.optionD << endl;
            questionsFile << endl;

            // Write answers to the answers file
            answersFile << "Q" << currentQuestionNumber << ": ANS: " << question.correctAnswer << ". " << endl;

            currentQuestionNumber++;
        }

        questionsFile.close();
        answersFile.close();
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    QuestionBank questionBank;

    // Read questions from the file
    ReadQuestionsFromFile(questionBank, "sample.txt");

    // Get user input for the number of booklets and questions
    int numBooklets, numQuestions;
    cout << "Enter the number of booklets to create: ";
    cin >> numBooklets;
    cout << "Enter the number of questions per booklet: ";
    cin >> numQuestions;

    // Create booklets and answer key booklets
    CreateBooklet(questionBank, numBooklets, numQuestions);

    cout << "Booklets created successfully." << endl;

    return 0;
}

// RUYA BOZCAN
// 200201044
// CS 363
// Assignment 2
