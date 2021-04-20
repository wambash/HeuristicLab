#include "gtest/gtest.h"
#include "test/mock/cec_20_test_func_mock.cc" //first dejong mock.
#include "src/utils/utils.cpp"
#include "src/alg/PSONovA.cpp"

//utils (and hence randomness) is NOT mocked!
//neighboursK_, singleDimensionFes_, popSize_, dimension_, testFunction_...
Algorithm psoNov(4, 12, 4, 1, 1, 1.496180, 1.496180, 0.9, 0.4, 0.2, 0, 0);


TEST(run, isCorrectNumberOfFes)
{
    std::vector<result> res = psoNov.run(1, 1, 1, 1);
    ASSERT_EQ(res.size(), 12);
}



//getRo is tested in utils test
TEST(initRo, isMostUniqueOK)
{
   
    Particle test1;
    test1.positionXi = {1};

    Particle test2;
    test2.positionXi = {1.1};

    Particle test3;
    test3.positionXi = {1.2};

    Particle test4;
    test4.positionXi = {500};

    std::vector<Particle> population;

    population.push_back(test1);
    population.push_back(test2);
    population.push_back(test3);
    population.push_back(test4);

    Particle mostUnique = test2;
    
    std::vector<std::vector<double>> positions;

    psoNov.initRo(population, mostUnique, positions);

    ASSERT_EQ(mostUnique.positionXi, test4.positionXi);
}

TEST(recountRo, isMostUniqueOK)
{
   
    Particle test1;
    test1.positionXi = {1};

    Particle test2;
    test2.positionXi = {1.1};

    Particle test3;
    test3.positionXi = {1.2};

    Particle test4;
    test4.positionXi = {500};

    std::vector<Particle> population;

    population.push_back(test1);
    population.push_back(test2);
    population.push_back(test3);
    population.push_back(test4);

    Particle mostUnique = test2;
    
    std::vector<std::vector<double>> positions;

    psoNov.recountRo(population, mostUnique, positions);

    ASSERT_EQ(mostUnique.positionXi, test4.positionXi);
}
