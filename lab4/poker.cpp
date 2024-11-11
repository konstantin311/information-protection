#include "poker.hpp"
#include <algorithm>

void shuffleDeck(std::vector<long long>& deck) {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(deck.begin(), deck.end(), g);
}

void distributeCards(std::vector<long long>& deck, std::vector<std::vector<long long>>& playersCards, int playersCount, int countCards) {
    int cardIndex = 0;
    for (int i = 0; i < playersCount; i++) {
        playersCards[i].push_back(deck[cardIndex++]);  
        playersCards[i].push_back(deck[cardIndex++]);  
    }

    for (int i = 0; i < playersCount; i++) {
        std::cout << "Player " << i + 1 << " gets cards: " << playersCards[i][0] << ", " << playersCards[i][1] << std::endl;
    }
}

long long decryptCard(long long encryptedCard, int playerIndex, const std::vector<long long>& D, int playersCount, long long p) {
    long long k = encryptedCard;
    for (int j = 0; j < playersCount; j++) {
        if (j != playerIndex) {
            k = pow_module(k, D[j], p);  
        }
    }
    k = pow_module(k, D[playerIndex], p);
    return k;
}

std::string cardToString(int card) {
    const std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    const std::vector<std::string> suits = {"Hearts", "Diamonds", "Clubs", "Spades"};

    int rankIndex = (card - 2) % 13;
    int suitIndex = (card - 2) / 13;

    return ranks[rankIndex] + " of " + suits[suitIndex];
}

void test_poker(){
    int playersCount;
    int countCards = 52;
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

    std::vector<long long> deck(countCards);
    std::cout<<"Original deck:"<<std::endl;
    for(int i = 0; i < countCards; i++){
        deck[i] = i + 2;
        std::cout<<deck[i]<<" ";
    }

    long long K;
    for(int i = 0 ; i < playersCount; i++){
        for(int j = 0; j < countCards; j++){
            K = pow_module(deck[j],C[i],p);
            deck[j] = K;
        }
        shuffleDeck(deck);
    }

    std::cout << "\nShuffled deck after encryption:" << std::endl;
    for(int i = 0; i < countCards; i++){
        std::cout<<deck[i]<<" ";
    }
    std::cout<<std::endl;

    std::vector<std::vector<long long>> playersCards(playersCount);
    distributeCards(deck, playersCards, playersCount, countCards);

    std::cout << "\nDecrypted cards for each player:" << std::endl;
    for (int i = 0; i < playersCount; i++) {
        for (int j = 0; j < 2; j++) { 
            long long decryptedCard = decryptCard(playersCards[i][j], i, D, playersCount, p);
            std::cout << "Player " << i + 1 << " decrypted card " << j + 1 << ": " << decryptedCard << " (" << cardToString(decryptedCard) << ")" <<std::endl;
        }
    }

}