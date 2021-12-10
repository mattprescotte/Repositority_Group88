// SFML Demo

#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include <utility>
#include <math.h>
#include <algorithm>
#include <map>
#include <sstream>
#include <iostream>

using namespace std;

// Graph Implementation
vector<pair<pair<string, int>, vector<pair<string, int>>>> graphGenerator(ifstream& inputFile, vector<pair<pair<string, int>, vector<pair<string, int>>> >& playerList)
{
    char startLine[3];
    string name, birthday, currLine;
    int birthdate;
    //stringstream splitter;        
    inputFile.read(startLine, 3);
    while (getline(inputFile, currLine))
    {
        stringstream splitter(currLine);
        getline(splitter, name, ',');
        getline(splitter, birthday);
        birthdate = stoi(birthday);

        if (playerList.size() == 0)
        {
            pair<string, int> sourceVertex(name, birthdate);
            pair<string, int> emptyVertex("", 0);
            vector<pair<string, int>> v;
            v.push_back(emptyVertex);
            pair<pair<string, int>, vector<pair<string, int>>> firstVertex(sourceVertex, v);
            playerList.push_back(firstVertex);
        }
        else if (playerList.size() == 1)
        {
            pair<string, int> secondVertex(name, abs(birthdate - playerList[0].first.second));
            playerList[0].second[0] = secondVertex;
            secondVertex.second = birthdate;
            pair<string, int> sourceVertex(playerList[0].first.first, abs(birthdate - playerList[0].first.second));
            vector<pair<string, int>> v;
            v.push_back(sourceVertex);
            pair<pair<string, int>, vector<pair<string, int>>> nVertex(secondVertex, v);
            playerList.push_back(nVertex);
        }
        else if (playerList.size() > 1)
        {
            pair<string, int> currVertex(name, birthdate);
            pair<string, int> addToSource(name, abs(birthdate - playerList[0].first.second));
            playerList[0].second.push_back(addToSource);
            pair<string, int> addSourceToCurr(playerList[0].first.first, abs(birthdate - playerList[0].first.second));
            vector<pair<string, int>> v;
            v.push_back(addSourceToCurr);
            pair<string, int> priorVertex(playerList[playerList.size() - 1].first.first, abs(birthdate - playerList[playerList.size() - 1].first.second));
            pair<string, int> addToPrior(name, abs(birthdate - playerList[playerList.size() - 1].first.second));
            playerList[playerList.size() - 1].second.push_back(addToPrior);
            v.push_back(priorVertex);
            pair<pair<string, int>, vector<pair<string, int>>> pairToPush(currVertex, v);
            playerList.push_back(pairToPush);

        }
    }
    if (playerList.size() > 2)
    {
        pair<string, int> finalEdge(playerList[1].first.first, abs(playerList[playerList.size() - 1].first.second - playerList[1].first.second));
        playerList[playerList.size() - 1].second.push_back(finalEdge);
        finalEdge.first = playerList[playerList.size() - 1].first.first;
        playerList[1].second.push_back(finalEdge);
    }
    return playerList;
}

vector< pair< pair<string, string>, int>> Kruskals(vector<pair<pair<string, int>, vector<pair<string, int>>> >& playerList)
{
    vector<pair<pair<string, string>, int>> kruskalMST, kruskalList;
    for (int i = 0; i < playerList.size(); i++)
    {
        for (int j = 0; j < playerList[i].second.size(); j++)
        {
            pair<string, string> edge(playerList[i].first.first, playerList[i].second[j].first);
            int weight = playerList[i].second[j].second;
            bool alreadyEntered = false;
            pair<pair<string, string>, int> kruskalEdge(edge, weight);
            for (int k = 0; k < kruskalList.size(); k++)
            {
                if (edge.first == kruskalList[k].first.second && edge.second == kruskalList[k].first.first && weight == kruskalList[k].second)
                {
                    alreadyEntered = true;
                }
            }

            if (!alreadyEntered)
            {
                kruskalList.push_back(kruskalEdge);
            }
        }
    }

    int minIndex;
    for (int i = 0; i < kruskalList.size() - 1; i++)
    {
        minIndex = i;
        for (int j = i + 1; j < kruskalList.size(); j++)
        {
            if (kruskalList[j].second < kruskalList[minIndex].second)
            {
                minIndex = j;
            }
        }

        pair<pair<string, string>, int> temp = kruskalList[minIndex];
        kruskalList[minIndex] = kruskalList[i];
        kruskalList[i] = temp;

    }

    map<string, bool> insertedVertices;
    for (int i = 0; i < kruskalList.size(); i++)
    {
        if (insertedVertices.find(kruskalList[i].first.first) == insertedVertices.end() || insertedVertices.find(kruskalList[i].first.second) == insertedVertices.end())
        {
            kruskalMST.push_back(kruskalList[i]);
            insertedVertices[kruskalList[i].first.first] = true;
            insertedVertices[kruskalList[i].first.second] = true;
        }
    }

    return kruskalMST;
}

vector< pair< pair<string, string>, int>> Prims(vector<pair<pair<string, int>, vector<pair<string, int>>>>& playerList)
{
    vector<pair<pair<string, string>, int>> primsMST;
    vector<string> sortedVertices, unsortedVertices;
    map<string, bool> insertedVertices;
    sortedVertices.push_back(playerList[0].first.first);
    insertedVertices[playerList[0].first.first] = true;
    for (int i = 1; i < playerList.size(); i++)
    {
        unsortedVertices.push_back(playerList[i].first.first);
    }

    while (!unsortedVertices.empty())
    {
        int min = 2147483647;
        pair<string, string>  edge;
        int weight = 0;
        pair<pair<string, string>, int> toPush;
        for (int i = 0; i < playerList.size(); i++)
        {
            for (int j = 0; j < sortedVertices.size(); j++)
            {
                if (playerList[i].first.first == sortedVertices[j])
                {
                    for (int k = 0; k < playerList[i].second.size(); k++)
                    {
                        if ((insertedVertices.find(playerList[i].first.first) == insertedVertices.end() || insertedVertices.find(playerList[i].second[k].first) == insertedVertices.end()) && min > playerList[i].second[k].second)
                        {
                            min = playerList[i].second[k].second;
                            edge.first = playerList[i].first.first;
                            edge.second = playerList[i].second[k].first;
                            weight = min;
                        }
                    }
                }
            }
        }
        toPush.first = edge;
        toPush.second = weight;
        primsMST.push_back(toPush);
        insertedVertices[edge.second] = true;
        sortedVertices.push_back(edge.second);
        for (int i = 0; i < unsortedVertices.size(); i++)
        {
            if (edge.second == unsortedVertices[i])
            {
                unsortedVertices.erase(unsortedVertices.begin() + i);
            }
        }

    }

    return primsMST;
}

double averageCal(vector< pair< pair<string, string>, int>> mstList)
{
    int numerator, denominator;
    numerator = 0;
    denominator = mstList.size();
    for (int i = 0; i < mstList.size(); i++)
    {
        numerator += mstList[i].second;
    }

    return (double)numerator / denominator;
}

class TextureManager {
    static unordered_map<string, sf::Texture> textures;
    static void LoadTexture(string fileName);
public:
    static sf::Texture& GetTexture(string textureName);
    static void Clear();
};
unordered_map<string, sf::Texture> TextureManager::textures;
void TextureManager::LoadTexture(string fileName) {
    string path = "";
    path += fileName;
    textures[fileName].loadFromFile(path);
}
sf::Texture& TextureManager::GetTexture(string textureName) {
    if (textures.find(textureName) == textures.end()) {
        LoadTexture(textureName);
    }
    return textures[textureName];
}
void TextureManager::Clear() {
    textures.clear();
}

int main()
{
    string input;
    ifstream inputFile("Copy of Vertex Values.csv");
    //ifstream inputFile("../Vertex Values.xlsx");

    //cout << "Enter numeric value: " << endl;
    //cout << "1. Basketball" << endl;
    //cout << "2. Baseball" << endl;
    //cout << "3. Football" << endl;
    //cout << "4. Hockey" << endl;
    //cout << "5. Soccer" << endl;

    //cin >> input;
    /*
    * if(input == "1")
    {
        //inputFile.open("basketball.csv");
    }
    else if(input == "2")
    {
        //inputFile.open("baseball.csv");
    }
    else if(input == "3")
    {
        //inputFile.open("football.csv");
    }
    else if (input == "4")
    {
        //inputFIle.open("hockey.csv");
    }
    else if (input == "5")
    {
        //inputFile.open("soccer.csv");
    }
    else
    {
        cout << "Invalid input. Please input a numeric value" << endl;
    }
    */

    vector<pair<pair<string, int>, vector<pair<string, int>>> > playerList;
    vector< pair< pair<string, string>, int>> kruskalMST, primsMST;
    double prim = 0;
    double kruskal = 0;
    if (inputFile.is_open())
    {
        playerList = graphGenerator(inputFile, playerList);
        kruskalMST = Kruskals(playerList);
        primsMST = Prims(playerList);

        //for (int i = 0; i < primsMST.size(); i++)
        //{
        //    cout << primsMST[i].second << endl;
        //}

        kruskal = averageCal(kruskalMST);
        prim = averageCal(primsMST);
        //cout << primsMST.size() << endl;                                
        /*
        cout << kruskalMST.size() << endl;
        for (int i = 0; i < kruskalMST.size(); i++)
        {
            cout << kruskalMST[i].second << endl;
        }
        */

        /*
        cout << playerList[3].first.first << endl;
        cout << playerList[3].second[2].first << endl;
        cout << playerList[3].second[2].second << endl;
        */
    }
    inputFile.close();

    // User Interface
    int mode = 0;

    sf::RenderWindow menuWindow(sf::VideoMode(1280, 835), "Birthday Clusters!");

    sf::Sprite background(TextureManager::GetTexture("soccerfield.png"));

    sf::Font font1;
    if (!font1.loadFromFile("Soccerleague.ttf")) {
        cout << "Error loading" << endl;
        system("pause");
    }
    sf::Font font2;
    if (!font2.loadFromFile("arial.ttf")) {
        cout << "Error loading" << endl;
        system("pause");
    }
    sf::Text myText;
    myText.setFont(font1);
    myText.setString("Birthdays in Sports");
    myText.setOutlineThickness(4);
    myText.setCharacterSize(100);
    myText.setPosition(200, 50);

    sf::Sprite instructions;
    instructions.setTexture(TextureManager::GetTexture("instructions.png"));
    instructions.setPosition(140, 200);

    sf::Sprite button0;
    button0.setTexture(TextureManager::GetTexture("button0.png"));
    button0.setPosition(540, 550);

    sf::Sprite button1;
    button1.setTexture(TextureManager::GetTexture("button1.png"));
    button1.setPosition(540, 550);

    bool filePresent = false;
    ifstream loadedFile;
    while (menuWindow.isOpen()) {
        menuWindow.draw(background);
        menuWindow.draw(myText);

        sf::Event event;

        //check if the file is ready
        /*loadedFile.close();
        loadedFile.open("input.csv");
        if (loadedFile) {
            filePresent = true;
        }
        else {
            filePresent = false;
        }*/

        if (mode == 0) {
            while (menuWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    menuWindow.close();
                }
                else if (event.type == sf::Event::MouseButtonPressed) {
                    if (true) {
                        sf::Vector2i position = sf::Mouse::getPosition(menuWindow);
                        if (position.x < button1.getPosition().x + 200 && position.x > button1.getPosition().x && position.x < button1.getPosition().y + 200 && position.x > button1.getPosition().y) {
                            mode = 1;
                        }
                    }
                }
            }
            menuWindow.draw(instructions);
            if (true) {
                menuWindow.draw(button1);
            }
            else {
                menuWindow.draw(button0);
            }
            menuWindow.display();

        }
        if (mode == 1) {
            //get results and post results
            vector<pair<pair<string, int>, vector<pair<string, int>>> > playerList;

            menuWindow.display();
            // Output
            while (true) {
                // Output
                cout << "Program started" << endl;
                sf::RenderWindow window(sf::VideoMode(1600, 900), "Bar Graph");

                double width1 = 100;
                double height1 = 350;
                double width2 = 100;
                double height2 = 500;
                double width3 = 100;
                double height3 = 70;
                double width4 = 100;
                double height4 = 400;
                double width5 = 100;
                double height5 = 100;

                // Average Birthdate Weights for Sports
                double varBasketball = prim;
                double varBaseball = 0;
                double varFootball = 0;
                double varHockey = 0;
                double varSoccer = 0;

                // Change rectangle height
                // Do Math to figure out ratio of window to birthdate axis
                double scaleFactor = 2.5;
                height1 = scaleFactor * varBasketball;
                //height2 = scaleFactor * varBaseball;
                //height3 = scaleFactor * varFootball;
                //height4 = scaleFactor * varHockey;
                //height5 = scaleFactor * varSoccer;

                // Create Axes

                // Y axis Title = Birthdates
                sf::Text birthdates;
                sf::Font font;
                font.loadFromFile("OpenSans-Bold.ttf");
                birthdates.setFont(font);
                birthdates.setString("Birthdate");
                birthdates.setCharacterSize(45);
                birthdates.setFillColor(sf::Color::White);
                birthdates.move(10, 500);
                birthdates.setRotation(270.f);

                // Y axis #'s = 0-200
                // Used Ratio of window to birthdates (0-200) to determine axis
                sf::Text y0;
                y0.setFont(font);
                y0.setString("0");
                y0.setCharacterSize(30);
                y0.setFillColor(sf::Color::White);
                y0.move(140, 670);

                sf::Text y1;
                y1.setFont(font);
                y1.setString("20");
                y1.setCharacterSize(30);
                y1.setFillColor(sf::Color::White);
                y1.move(140, 620);

                sf::Text y2;
                y2.setFont(font);
                y2.setString("40");
                y2.setCharacterSize(30);
                y2.setFillColor(sf::Color::White);
                y2.move(140, 570);

                sf::Text y3;
                y3.setFont(font);
                y3.setString("60");
                y3.setCharacterSize(30);
                y3.setFillColor(sf::Color::White);
                y3.move(140, 520);

                sf::Text y4;
                y4.setFont(font);
                y4.setString("80");
                y4.setCharacterSize(30);
                y4.setFillColor(sf::Color::White);
                y4.move(140, 470);

                sf::Text y5;
                y5.setFont(font);
                y5.setString("100");
                y5.setCharacterSize(30);
                y5.setFillColor(sf::Color::White);
                y5.move(140, 420);

                sf::Text y6;
                y6.setFont(font);
                y6.setString("120");
                y6.setCharacterSize(30);
                y6.setFillColor(sf::Color::White);
                y6.move(140, 370);

                sf::Text y7;
                y7.setFont(font);
                y7.setString("140");
                y7.setCharacterSize(30);
                y7.setFillColor(sf::Color::White);
                y7.move(140, 320);

                sf::Text y8;
                y8.setFont(font);
                y8.setString("160");
                y8.setCharacterSize(30);
                y8.setFillColor(sf::Color::White);
                y8.move(140, 270);

                sf::Text y9;
                y9.setFont(font);
                y9.setString("180");
                y9.setCharacterSize(30);
                y9.setFillColor(sf::Color::White);
                y9.move(140, 220);

                sf::Text y10;
                y10.setFont(font);
                y10.setString("200");
                y10.setCharacterSize(30);
                y10.setFillColor(sf::Color::White);
                y10.move(140, 170);

                // X axis Title = Sports
                sf::Text sports;
                sports.setFont(font);
                sports.setString("Sports");
                sports.setCharacterSize(45);
                sports.setFillColor(sf::Color::White);
                sports.move(800, 775);

                // Main Title = Statistical Analysis of Birth Dates in Sports
                sf::Text maintitle;
                maintitle.setFont(font);
                maintitle.setString("Statistical Analysis of Birth Dates in Sports");
                maintitle.setCharacterSize(55);
                maintitle.setFillColor(sf::Color::White);
                maintitle.move(300, 50);

                // Change Axes

                // Create Rectangles
                sf::RectangleShape rec1(sf::Vector2f(width1, height1));
                sf::RectangleShape rec2(sf::Vector2f(width2, height2));
                sf::RectangleShape rec3(sf::Vector2f(width3, height3));
                sf::RectangleShape rec4(sf::Vector2f(width4, height4));
                sf::RectangleShape rec5(sf::Vector2f(width5, height5));

                // Move Rectangles
                rec1.move(330, 700);
                rec2.move(630, 700);
                rec3.move(930, 700);
                rec4.move(1230, 700);
                rec5.move(1530, 700);

                // Set Rectangle Colors
                rec1.setFillColor(sf::Color::Green);
                rec2.setFillColor(sf::Color::Blue);
                rec3.setFillColor(sf::Color::Magenta);
                rec4.setFillColor(sf::Color::Red);
                rec5.setFillColor(sf::Color::Yellow);

                // Rotate Window
                rec1.setRotation(180.f);
                rec2.setRotation(180.f);
                rec3.setRotation(180.f);
                rec4.setRotation(180.f);
                rec5.setRotation(180.f);

                // Create Text
                sf::Text text1;
                text1.setFont(font);
                text1.setString("Basketball");
                text1.setCharacterSize(35);
                text1.setFillColor(sf::Color::Green);
                text1.move(190, 720);

                sf::Text text2;
                text2.setFont(font);
                text2.setString("Baseball");
                text2.setCharacterSize(35);
                text2.setFillColor(sf::Color::Blue);
                text2.move(505, 720);

                sf::Text text3;
                text3.setFont(font);
                text3.setString("Football");
                text3.setCharacterSize(35);
                text3.setFillColor(sf::Color::Magenta);
                text3.move(805, 720);

                sf::Text text4;
                text4.setFont(font);
                text4.setString("Hockey");
                text4.setCharacterSize(35);
                text4.setFillColor(sf::Color::Red);
                text4.move(1117, 720);

                sf::Text text5;
                text5.setFont(font);
                text5.setString("Soccer");
                text5.setCharacterSize(35);
                text5.setFillColor(sf::Color::Yellow);
                text5.move(1425, 720);

                // Create Window and Make Bar Graph
                while (window.isOpen())
                {
                    sf::Event event;
                    while (window.pollEvent(event))
                    {
                        if (event.type == sf::Event::Closed)
                            window.close();
                    }

                    window.clear();
                    window.draw(rec1);
                    window.draw(rec2);
                    window.draw(rec3);
                    window.draw(rec4);
                    window.draw(rec5);
                    window.draw(text1);
                    window.draw(text2);
                    window.draw(text3);
                    window.draw(text4);
                    window.draw(text5);
                    window.draw(sports);
                    window.draw(birthdates);
                    window.draw(maintitle);
                    window.draw(y0);
                    window.draw(y1);
                    window.draw(y2);
                    window.draw(y3);
                    window.draw(y4);
                    window.draw(y5);
                    window.draw(y6);
                    window.draw(y7);
                    window.draw(y8);
                    window.draw(y9);
                    window.draw(y10);
                    window.display();
                }
                break;
            }
            break;
        }

    }
    TextureManager::Clear();

    cout << "Program ended" << endl;
    return 0;
}
