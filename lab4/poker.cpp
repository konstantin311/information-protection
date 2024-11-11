#include "poker.hpp"

struct Card {
    std::string suit;
    std::string rank;
};

std::vector<Card> createDeck() {
    std::vector<Card> deck;
    std::vector<std::string> suits = {"Hearts", "Diamonds", "Clubs", "Spades"};
    std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};

    for (const auto& suit : suits) {
        for (const auto& rank : ranks) {
            deck.push_back(Card{suit, rank});
        }
    }

    return deck;
}

void test_poker(){
    int playersCount;
    do{
    std::cout<<"Print count players(2..23)"<<std::endl;
    std::cin>>playersCount;
    }while(playersCount < 2 || playersCount > 23);
    std::random_device rd;
    std::mt19937 gen(rd());
    long long q; 
    long long p;
    do {
        do {
            q = generateRandomPrime();
        } while (!millerRabinTest(q, 100));
        p = 2 * q + 1;
    } while (!millerRabinTest(p, 100));
    std::cout << "q = " << q << std::endl;
    std::cout << "p = " << p << std::endl;

    std::vector<long long> C(playersCount);
    std::vector<long long> D(playersCount);
    std::uniform_int_distribution<> dist(1 ,p - 1);
    for (int i = 0 ; i < playersCount; i++){
        C[i] = dist(gen);
        D[i] = modularInverse(C[i],p-1);
    }

}