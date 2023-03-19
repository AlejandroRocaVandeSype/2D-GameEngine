#include "TrashTheCache.h"
#include <chrono>

using namespace std::chrono;

TrashTheCache::TrashTheCache()
    : m_SIZE { 60000000 }
    , m_TOTAL_STEPSIZE { 1024 }
    , m_Samples { 20 }
    , m_TimingsIdx{ 0 }
{
    // Initialize array
    for (int index{ 0 }; index < m_MAX_TIMINGS; ++index)
    {
        m_FinalTimings[index] = 0.f;
    }

}
// Run the test
bool TrashTheCache::RunExercise(int version)
{

    switch (version)
    {
    case 1:
        return RunVersion1();
        break;
    case 2:
        return RunVersion2();
        break;
    }
   
    return false;
}

bool TrashTheCache::RunVersion1()
{
    int* arrInts = new int[m_SIZE];
    std::vector<long long> vSamples;            // Store the samples for each stepsize
    vSamples.reserve(m_Samples);

    for (int stepsize{ 1 }; stepsize <= m_TOTAL_STEPSIZE; stepsize *= 2)
    {
        int sampleIdx{ 0 };
        while (sampleIdx < m_Samples)  // Get all the samples we want
        {
            auto start = high_resolution_clock::now();

            for (int intsIdx{ 0 }; intsIdx < m_SIZE; intsIdx += stepsize)
            {
                arrInts[intsIdx] *= 2;
            }

            auto end = high_resolution_clock::now();
            auto elapsedTime = duration_cast<microseconds>(end - start).count();

            // One sample measured
            vSamples.push_back(elapsedTime);

            sampleIdx++;
        }

        // Delete faster and slower values ( container is already sorted )
        vSamples.pop_back();
        vSamples.erase(vSamples.begin());

        CalculateAverageTime(vSamples);

        vSamples.clear();

    }

    delete[] arrInts;
    m_TimingsIdx = 0;       // Reset the index for next calculation

    return true;
}

bool TrashTheCache::RunVersion2()
{
    GameObject3D* arrayGameObjects = new GameObject3D[m_SIZE];
    std::vector<long long> vSamples;            // Store the samples for each stepsize
    vSamples.reserve(m_Samples);

    for (int stepsize{ 1 }; stepsize <= m_TOTAL_STEPSIZE; stepsize *= 2)
    {
        int sampleIdx{ 0 };
        while (sampleIdx < m_Samples)  // Get all the samples we want
        {
            auto start = high_resolution_clock::now();

            for (int gameObjectsIdx{ 0 }; gameObjectsIdx < m_SIZE; gameObjectsIdx += stepsize)
            {
                arrayGameObjects[gameObjectsIdx].ID *= 2;
            }

            auto end = high_resolution_clock::now();
            auto elapsedTime = duration_cast<milliseconds>(end - start).count();

            // One sample measured
            vSamples.push_back(elapsedTime);

            sampleIdx++;
        }

        // Delete fastest and slowest values ( container is already sorted )
        vSamples.pop_back();
        vSamples.erase(vSamples.begin());

        CalculateAverageTime(vSamples);

        vSamples.clear();

    }

    delete[] arrayGameObjects;
    m_TimingsIdx = 0;

    return true;
}


// Add all sample results and divide by the amount of samples
void TrashTheCache::CalculateAverageTime(const std::vector<long long>& vSamples)
{
    float averageTime{};

    for (size_t index{}; index < vSamples.size(); index++)
    {
        averageTime += float(vSamples.at(index));
    }

    m_FinalTimings[m_TimingsIdx] = float(averageTime / vSamples.size());
    m_TimingsIdx++;

}

int& TrashTheCache::GetSamples()
{
	return m_Samples;
}

float* TrashTheCache::GetTimingsValues()
{
    return m_FinalTimings;
}

int TrashTheCache::GetTotalTimings()
{
    return m_MAX_TIMINGS;
}