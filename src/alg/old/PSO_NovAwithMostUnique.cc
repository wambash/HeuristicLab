#include <vector>

struct result
{
    int fez;
    double cost;
};

struct Particle
{
    std::vector<double> positionXi;
    std::vector<double> velocityVectorVi;
    std::vector<double> pBestPi;
    double bestCost;
    double ro;
    double bestRo;
};

class Algorithm
{

private:
    int neighboursK_, singleDimensionFes_, popSize_, dimension_, testFunction_;
    double c1_, c2_, wStart_, wEnd_, w_, vMax_, boundaryLow_, boundaryUp_;

    void initParticleRandom(Particle &p)
    {
        std::vector<double> randomPosition = utils::generateRandomRange(dimension_, boundaryLow_, boundaryUp_);
        p = {randomPosition, utils::generateRandomRange(dimension_, 0, 0), randomPosition};
        cec20_test_func(p.pBestPi.data(), &p.bestCost, dimension_, 1, testFunction_);
    }

    void initPopulationReturnBest(std::vector<Particle> &population, Particle &gBestParticle)
    {
        for (int i = 0; i < popSize_; i++)
        {
            Particle p;
            initParticleRandom(p);

            //find best swarm position and cost
            if (p.bestCost < gBestParticle.bestCost)
            {
                gBestParticle.positionXi = p.pBestPi;
                gBestParticle.bestCost = p.bestCost;
            }
            population.push_back(p);
        }
    }

    void backToBoundaries(Particle &particle, int iteration)
    {
        if (boundaryLow_ > particle.positionXi[iteration] || particle.positionXi[iteration] > boundaryUp_)
        {
            double randomNum = utils::generateRandomDouble(boundaryLow_, boundaryUp_);
            particle.positionXi[iteration] = randomNum;
            particle.velocityVectorVi[iteration] = 0;
        }
    }

public:
    Algorithm(int neighboursK = 3, int singleDimensionFes = 5000, int popSize = 25, int dimension = 0, int testFunction = 0, double c1 = 1.496180, double c2 = 1.496180,
              double wStart = 0.9, double wEnd = 0.4, double vMax = 0.2, double boundaryLow = 0, double boundaryUp = 0)
        : neighboursK_(neighboursK), singleDimensionFes_(singleDimensionFes), popSize_(popSize), dimension_(dimension), testFunction_(testFunction),
          c1_(c1), c2_(c2), wStart_(wStart), wEnd_(wEnd), w_(wStart),
          vMax_(vMax), boundaryLow_(boundaryLow), boundaryUp_(boundaryUp)
    {
    }

    void dimensionMove(Particle &currentParticle, Particle &gBestParticle, int d)
    {
        double rp = utils::generateRandomDouble(0, 1);
        double rg = utils::generateRandomDouble(0, 1);

        double inertia = w_ * currentParticle.velocityVectorVi[d];
        double attractionToSelf = c1_ * rp * (currentParticle.pBestPi[d] - currentParticle.positionXi[d]);
        double attractionToBest = c2_ * rg * (gBestParticle.positionXi[d] - currentParticle.positionXi[d]);
        double potentialVectorD = inertia + attractionToSelf + attractionToBest;
        //Update the particle's velocity
        double vParam = vMax_ * (boundaryUp_ - boundaryLow_);
        currentParticle.velocityVectorVi[d] = fmax(fmin(potentialVectorD, vParam), -vParam);
        //Update the particle's position
        currentParticle.positionXi[d] = currentParticle.positionXi[d] + currentParticle.velocityVectorVi[d];

        backToBoundaries(currentParticle, d);
    }

    void getPositions(std::vector<Particle> &population, std::vector<std::vector<double>> &positions)
    {
        positions.clear();

        for (auto &particle : population)
        {
            positions.push_back(particle.positionXi);
        }
    }

    void initRo(std::vector<Particle> &population, Particle &mostUnique, std::vector<std::vector<double>> &positions)
    {
        getPositions(population, positions);
        mostUnique.ro = 0;

        for (int i = 0; i < popSize_; i++)
        {
            double initialRo = utils::getRo(population[i].positionXi, positions, neighboursK_);
            population[i].ro = initialRo;
            population[i].bestRo = initialRo;
            if (population[i].ro > mostUnique.ro)
            {
                mostUnique = population[i];
            }
        }
    }

    void recountRo(std::vector<Particle> &population, Particle &mostUnique, std::vector<std::vector<double>> &positions)
    {

        getPositions(population, positions);

        for (int i = 0; i < popSize_; i++)
        {
            population[i].ro = utils::getRo(population[i].positionXi, positions, neighboursK_);
            if (population[i].ro > population[i].bestRo)
            {
                population[i].bestRo = population[i].ro;
            }
            if (population[i].ro > mostUnique.ro)
            {
                mostUnique.ro = population[i].ro;
                mostUnique = population[i];
            }
        }
    }

    std::vector<result> run(int dimensionsCount, int testFunction, double boundaryLow, double boundaryUp)
    {
        dimension_ = dimensionsCount;
        testFunction_ = testFunction;
        boundaryLow_ = boundaryLow;
        boundaryUp_ = boundaryUp;
        int MAX_FEZ = singleDimensionFes_ * dimensionsCount;
        int generations = MAX_FEZ / popSize_;

        std::vector<result> best_results;
        int fezCounter = 0;
        std::vector<Particle> population;

        Particle gBestParticle;
        initParticleRandom(gBestParticle);
        initPopulationReturnBest(population, gBestParticle);

        Particle mostUnique = population[0];
        std::vector<std::vector<double>> positions;
        initRo(population, mostUnique, positions);

        for (int g = 0; g < generations; g++)
        {
            for (int i = 0; i < population.size(); i++)
            {
                Particle &currentParticle = population[i];

                for (int d = 0; d < dimensionsCount; d++)
                {
                    dimensionMove(currentParticle, gBestParticle, d);
                }

                double newXiCost = 0;
                cec20_test_func(currentParticle.positionXi.data(), &newXiCost, dimensionsCount, 1, testFunction);
                fezCounter++;

                //A - personal best se bude vybírat na základě novelty, global na základě účelovky
                //pokud ta částice byla unikátnější než předtím, tak ji uprav pBest pozici a bestCost na newXiCost
                double previousRo = currentParticle.bestRo;
                recountRo(population, mostUnique, positions);
                if (currentParticle.bestRo > previousRo)
                {
                    currentParticle.pBestPi = currentParticle.positionXi;
                    currentParticle.bestCost = newXiCost;
                }

                if (newXiCost < gBestParticle.bestCost)
                {
                    //Update the swarm's best known position
                    gBestParticle.bestCost = newXiCost;
                    gBestParticle.positionXi = currentParticle.positionXi;
                }

                best_results.push_back({fezCounter, gBestParticle.bestCost});
            }
            //after generation run, recount w
            w_ = wStart_ - ((wStart_ - wEnd_) * g) / generations;
        }

        return best_results;
    }
};
