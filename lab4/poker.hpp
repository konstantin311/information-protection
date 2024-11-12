#include "lab1_lib.hpp"

void shuffleDeck(std::vector<long long>& deck);

void distributeCards(std::vector<long long>& deck, std::vector<std::vector<long long>>& playersCards, int playersCount, int countCards);

long long decryptCard(long long encryptedCard, int playerIndex, const std::vector<long long>& D, int playersCount, long long p);

std::string cardToString(int card);

void test_poker();