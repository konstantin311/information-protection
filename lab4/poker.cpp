#include "poker.hpp"

struct Card {
    std::string suit;
    std::string rank;
};

std::unordered_map<int, Card> createDeck() {
    std::unordered_map<int, Card> deck;
    std::vector<std::string> suits = {"Hearts", "Diamonds", "Clubs", "Spades"};
    std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
    
    int index = 0;
    for (const auto& suit : suits) {
        for (const auto& rank : ranks) {
            deck[index++] = Card{suit, rank};
        }
    }

    return deck;
}

void printDeck(const std::unordered_map<int, Card>& deck) {
    for (const auto& [key, card] : deck) {
        std::cout << "Card " << key + 1 << ": " << card.rank << " of " << card.suit << std::endl;
    }
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
        C[i] = findCoprime(p - 1);
        D[i] = modularInverse(C[i], p - 1);
    }
    /*for (int i = 0 ; i < playersCount; i++){
    std::cout<<C[i]<<std::endl;
    std::cout<<D[i]<<std::endl;
    }*/

    
    std::unordered_map<int, Card> deck = createDeck();
    
    //printDeck(deck);
    
}