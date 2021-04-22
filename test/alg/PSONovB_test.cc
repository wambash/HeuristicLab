#include "gtest/gtest.h"
#include "test/mock/cec_20_test_func_mock.cc" //first dejong mock.
#include "src/utils/utils.cpp"
#include "src/alg/PSONovB.cpp"

//utils (and hence randomness) is NOT mocked!
//neighboursK_, singleDimensionFes_, popSize_, dimension_, testFunction_...
Algorithm psoNov(4, 12, 4, 2, 1, 1.496180, 1.496180, 0.9, 0.4, 0.2, 0, 0);

TEST(run, isCorrectNumberOfFes)
{
    std::vector<result> res = psoNov.run(1, 1, 1, 1);
    ASSERT_EQ(res.size(), 12);
}

TEST(recountRoGetMostUniqueByDimension, isMostUniqueOK)
{

    Particle test1;
    test1.positionXi = {1, 101};
    test1.dimensionRo = {0, 0};
    test1.dimensionBestRo = {0,0};

    Particle test2;
    test2.positionXi = {1.1, 800000};
    test2.dimensionRo = {0, 0};
    test2.dimensionBestRo = {0,0};

    Particle test3;
    test3.positionXi = {100, 102};
    test3.dimensionRo = {0, 0};
    test3.dimensionBestRo = {0,0};

    Particle test4;
    test4.positionXi = {200, 103};
    test4.dimensionRo = {0, 0};
    test4.dimensionBestRo = {0,0};

    std::vector<Particle> population;

    population.push_back(test1);
    population.push_back(test2);
    population.push_back(test3);
    population.push_back(test4);

    Particle &mostUnique = population[0];

    std::vector<std::vector<double>> positions;
    psoNov.initRo(population, positions);

    psoNov.recountRoGetMostUniqueByDimension(population, mostUnique, positions, 1);

    ASSERT_EQ(mostUnique.positionXi, test2.positionXi);
}
